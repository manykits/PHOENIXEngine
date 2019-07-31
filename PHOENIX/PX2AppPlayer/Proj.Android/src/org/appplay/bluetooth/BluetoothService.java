package org.appplay.bluetooth;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.UUID;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

public class BluetoothService 
{
	private static final String TAG = "Bluetooth Service";
	private static final String NAME_SECURE = "Bluetooth Secure";
	
    // Unique UUID for this application
    private static final UUID UUID_DEVICE_SELF =
            UUID.fromString("fa87c0d0-afac-11de-8a39-0800200c9a66");
    private static final UUID UUID_DEVICE_OTHER =
            UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    
    private final BluetoothAdapter mAdapter;
    private final Handler mHandler;
    private AcceptThread mSecureAcceptThread;
    private ConnectThread mConnectThread;
    private ConnectedThread mConnectedThread;
    private int mState;
    private boolean mIsAndroid = BluetoothState.IS_DEVICE_SELF;
    
    public BluetoothService(Context context, Handler handler) 
    {
        mAdapter = BluetoothAdapter.getDefaultAdapter();
        mState = BluetoothState.STATE_NONE;
        mHandler = handler;
    }
    
    private synchronized void SetState(int state) 
    {
        Log.d(TAG, "setState() " + mState + " -> " + state);
        mState = state;
        
        // Give the new state to the Handler so the UI Activity can update
        mHandler.obtainMessage(BluetoothState.MESSAGE_STATE_CHANGE, state, -1).sendToTarget();
    }
    
    public synchronized int GetState()
    {
        return mState;
    }
    
    public synchronized void Start(boolean isAndroid) 
    {
        // Cancel any thread attempting to make a connection
        if (mConnectThread != null) 
        {
        	mConnectThread.cancel(); 
        	mConnectThread = null;
        }
        // Cancel any thread currently running a connection
        if (mConnectedThread != null) 
        {
        	mConnectedThread.cancel();
        	mConnectedThread = null;
        }
        
        SetState(BluetoothState.STATE_LISTEN);
        
        // Start the thread to listen on a BluetoothServerSocket
        if (mSecureAcceptThread == null)
        {
            mSecureAcceptThread = new AcceptThread(isAndroid);
            mSecureAcceptThread.start();
            BluetoothService.this.mIsAndroid = isAndroid;
        }
    }
    
    public synchronized void Connect(BluetoothDevice device)
    {
        // Cancel any thread attempting to make a connection
        if (mState == BluetoothState.STATE_CONNECTING) 
        {
            if (mConnectThread != null) 
            {
            	mConnectThread.cancel(); 
            	mConnectThread = null;
            }
        }

        // Cancel any thread currently running a connection
        if (mConnectedThread != null) 
        {
        	mConnectedThread.cancel(); 
        	mConnectedThread = null;
        }

        // Start the thread to connect with the given device
        mConnectThread = new ConnectThread(device);
        mConnectThread.start();
        SetState(BluetoothState.STATE_CONNECTING);
    }
    
    public synchronized void Connected(BluetoothSocket socket, BluetoothDevice
            device, final String socketType)
    {
        // Cancel the thread that completed the connection
        if (mConnectThread != null) 
        {
        	mConnectThread.cancel();
        	mConnectThread = null;
        }

        // Cancel any thread currently running a connection
        if (mConnectedThread != null) 
        {
        	mConnectedThread.cancel(); 
        	mConnectedThread = null;
        }

        // Cancel the accept thread because we only want to connect to one device
        if (mSecureAcceptThread != null) {
            mSecureAcceptThread.cancel();
            mSecureAcceptThread = null;
        }

        // Start the thread to manage the connection and perform transmissions
        mConnectedThread = new ConnectedThread(socket, socketType);
        mConnectedThread.start();

        // Send the name of the connected device back to the UI Activity
        Message msg = mHandler.obtainMessage(BluetoothState.MESSAGE_DEVICE_NAME);
        Bundle bundle = new Bundle();
        bundle.putString(BluetoothState.DEVICE_NAME, device.getName());
        bundle.putString(BluetoothState.DEVICE_ADDRESS, device.getAddress());
        msg.setData(bundle);
        mHandler.sendMessage(msg);

        SetState(BluetoothState.STATE_CONNECTED);
    }
    
    public synchronized void Stop()
    {
        if (mConnectThread != null)
        {
            mConnectThread.cancel();
            mConnectThread = null;
        }

        if (mConnectedThread != null) 
        {
            mConnectedThread.cancel();
            mConnectedThread = null;
        }

        if (mSecureAcceptThread != null) 
        {
            mSecureAcceptThread.cancel();
            mSecureAcceptThread.kill();
            mSecureAcceptThread = null;
        }
        
        SetState(BluetoothState.STATE_NONE);
    }
    
    public void Write(byte[] out)
    {
        // Create temporary object
        ConnectedThread r;
        // Synchronize a copy of the ConnectedThread
        synchronized (this) {
            if (mState != BluetoothState.STATE_CONNECTED) return;
            r = mConnectedThread;
        }
        // Perform the write unsynchronized
        r.write(out);
    }
    
    private void ConnectionFailed() 
    {
        // Start the service over to restart listening mode
        BluetoothService.this.Start(BluetoothService.this.mIsAndroid);
    }

    private void ConnectionLost() 
    {
        // Start the service over to restart listening mode
        BluetoothService.this.Start(BluetoothService.this.mIsAndroid);
    }
    
    private class AcceptThread extends Thread 
    {
        // The local server socket
        private BluetoothServerSocket mmServerSocket;
        private String mSocketType;
        boolean isRunning = true;

        public AcceptThread(boolean isSelf)
        {
            BluetoothServerSocket tmp = null;

            // Create a new listening server socket
            try 
            {
                if(isSelf)
                    tmp = mAdapter.listenUsingRfcommWithServiceRecord(NAME_SECURE, UUID_DEVICE_SELF);
                else
                    tmp = mAdapter.listenUsingRfcommWithServiceRecord(NAME_SECURE, UUID_DEVICE_OTHER);
            } catch (IOException e) { }
            
            mmServerSocket = tmp;
        }

        public void run() 
        {
            setName("AcceptThread" + mSocketType);
            BluetoothSocket socket = null;

            // Listen to the server socket if we're not connected
            while (mState != BluetoothState.STATE_CONNECTED && isRunning) 
            {
                try 
                {
                    // This is a blocking call and will only return on a
                    // successful connection or an exception
                    socket = mmServerSocket.accept();
                } 
                catch (IOException e)
                {
                    break;
                }

                // If a connection was accepted
                if (socket != null) 
                {
                    synchronized (BluetoothService.this) 
                    {
                        switch (mState) {
                        case BluetoothState.STATE_LISTEN:
                        case BluetoothState.STATE_CONNECTING:
                            // Situation normal. Start the connected thread.
                            Connected(socket, socket.getRemoteDevice(),
                                    mSocketType);
                            break;
                        case BluetoothState.STATE_NONE:
                        case BluetoothState.STATE_CONNECTED:
                            // Either not ready or already connected. Terminate new socket.
                            try 
                            {
                                socket.close();
                            }
                            catch (IOException e)
                            {                            	
                            }
                            break;
                        }
                    }
                }
            }
        }
        
        public void cancel() 
        {
            try 
            {
                mmServerSocket.close();
                mmServerSocket = null;
            }
            catch (IOException e) 
            {            	
            }
        }

        public void kill() 
        {
            isRunning = false;
        }
    }
    
    private class ConnectThread extends Thread 
    {
        private final BluetoothSocket mSocket;
        private final BluetoothDevice mDevice;
        private String mSocketType;

        public ConnectThread(BluetoothDevice device)
        {
            mDevice = device;
            BluetoothSocket tmp = null;

            // Get a BluetoothSocket for a connection with the
            // given BluetoothDevice
            try 
            {
                if(BluetoothService.this.mIsAndroid)
                    tmp = device.createRfcommSocketToServiceRecord(UUID_DEVICE_SELF);
                else
                    tmp = device.createRfcommSocketToServiceRecord(UUID_DEVICE_OTHER);
            }
            catch (IOException e)
            {            	
            }
            
            mSocket = tmp;
        }

        public void run()
        {
            // Always cancel discovery because it will slow down a connection
            mAdapter.cancelDiscovery();

            // Make a connection to the BluetoothSocket
            try 
            {
                // This is a blocking call and will only return on a
                // successful connection or an exception
                mSocket.connect();
            }
            catch (IOException e)
            {
                // Close the socket
                try 
                {
                    mSocket.close();
                }
                catch (IOException e2)
                {                 	
                }
                ConnectionFailed();
                return;
            }

            // Reset the ConnectThread because we're done
            synchronized (BluetoothService.this) {
                mConnectThread = null;
            }

            // Start the connected thread
            Connected(mSocket, mDevice, mSocketType);
        }

        public void cancel() {
            try {
                mSocket.close();
            } catch (IOException e) { }
        }
    }
    
    private class ConnectedThread extends Thread 
    {
        private final BluetoothSocket mSocket;
        private final InputStream mmInStream;
        private final OutputStream mmOutStream;

        public ConnectedThread(BluetoothSocket socket, String socketType)
        {
            mSocket = socket;
            InputStream tmpIn = null;
            OutputStream tmpOut = null;

            // Get the BluetoothSocket input and output streams
            try 
            {
                tmpIn = socket.getInputStream();
                tmpOut = socket.getOutputStream();
            } 
            catch (IOException e)
            {            	
            }

            mmInStream = tmpIn;
            mmOutStream = tmpOut;
        }

        public void run()
        {
            byte[] buffer;
            ArrayList<Integer> arr_byte = new ArrayList<Integer>();

            // Keep listening to the InputStream while connected
            while (true)
            {
                try 
                {
                    int data = mmInStream.read();
                    if(data == 0x0A) 
                    { 
                    } 
                    else if(data == 0x0D)
                    {
                        buffer = new byte[arr_byte.size()];
                        for(int i = 0 ; i < arr_byte.size() ; i++)
                        {
                            buffer[i] = arr_byte.get(i).byteValue();
                        }
                        // Send the obtained bytes to the UI Activity
                        mHandler.obtainMessage(BluetoothState.MESSAGE_READ,
                        		buffer.length, -1, buffer).sendToTarget();
                        arr_byte = new ArrayList<Integer>();
                    } 
                    else
                    {
                        arr_byte.add(data);
                    }
                } 
                catch (IOException e)
                {
                    ConnectionLost();
                    // Start the service over to restart listening mode
                    BluetoothService.this.Start(BluetoothService.this.mIsAndroid);
                    break;
                }
            }
        }

        // Write to the connected OutStream.
        // @param buffer  The bytes to write
        public void write(byte[] buffer) 
        {
            try
            {/*
                byte[] buffer2 = new byte[buffer.length + 2];
                for(int i = 0 ; i < buffer.length ; i++) 
                    buffer2[i] = buffer[i];
                buffer2[buffer2.length - 2] = 0x0A;
                buffer2[buffer2.length - 1] = 0x0D;*/
                mmOutStream.write(buffer);
                // Share the sent message back to the UI Activity
                mHandler.obtainMessage(BluetoothState.MESSAGE_WRITE
                        , -1, -1, buffer).sendToTarget();
            } 
            catch (IOException e)
            {             	
            }
        }

        public void cancel()
        {
            try
            {
                mSocket.close();
            } 
            catch (IOException e)
            {             	
            }
        }
    }

}
