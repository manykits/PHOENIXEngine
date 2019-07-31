package org.appplay.bluetooth;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.UUID;

import org.appplay.lib.AppPlayBaseActivity;

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

public class BluetoothSPPService 
{
	private static final String TAG = "appplay.ap";
	private static final String NAME_SECURE = "Bluetooth Secure";
	
	private AppPlayBaseActivity TheAppPlayBaseActivity;
	
    // Unique UUID for this application
    private static final UUID UUID_DEVICE_SELF =
            UUID.fromString("fa87c0d0-afac-11de-8a39-0800200c9a66");
    private static final UUID UUID_DEVICE_OTHER =
            UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    
    private BluetoothAdapter mAdapter;
    private AcceptThread mSecureAcceptThread;
    private ConnectThread mConnectThread;
    private ConnectedThread mConnectedThread;
    private int mState;
    private boolean mIsAndroid = BluetoothSPPState.IS_DEVICE_SELF;
    
    public BluetoothSPPService(AppPlayBaseActivity activity, Handler handler) 
    {
    	TheAppPlayBaseActivity = activity;
    	
        mAdapter = BluetoothAdapter.getDefaultAdapter();
        mState = BluetoothSPPState.STATE_NONE;
    }
    
    private synchronized void SetState(int state) 
    {
        Log.d(TAG, "setState() " + mState + " -> " + state);
        mState = state;
        
        // Give the new state to the Handler so the UI Activity can update
        BluetoothSPP.TheBluetoothSPP.msHandler.obtainMessage(BluetoothSPPState.MESSAGE_STATE_CHANGE, state, -1).sendToTarget();
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
        
        SetState(BluetoothSPPState.STATE_LISTEN);
        
        // Start the thread to listen on a BluetoothServerSocket
        if (mSecureAcceptThread == null)
        {
            mSecureAcceptThread = new AcceptThread(isAndroid);
            mSecureAcceptThread.start();
            BluetoothSPPService.this.mIsAndroid = isAndroid;
        }
    }
    
    public synchronized void Connect(BluetoothDevice device)
    {
        // Cancel any thread attempting to make a connection
        if (mState == BluetoothSPPState.STATE_CONNECTING) 
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
        SetState(BluetoothSPPState.STATE_CONNECTING);
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
        Message msg = BluetoothSPP.TheBluetoothSPP.msHandler.obtainMessage(BluetoothSPPState.MESSAGE_DEVICE_NAME);
        Bundle bundle = new Bundle();
        bundle.putString(BluetoothSPPState.DEVICE_NAME, device.getName());
        bundle.putString(BluetoothSPPState.DEVICE_ADDRESS, device.getAddress());
        msg.setData(bundle);
        BluetoothSPP.TheBluetoothSPP.msHandler.sendMessage(msg);

        SetState(BluetoothSPPState.STATE_CONNECTED);
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
        
        SetState(BluetoothSPPState.STATE_NONE);
    }
    
    public void Write(byte[] out)
    {
        // Create temporary object
        ConnectedThread r;
        // Synchronize a copy of the ConnectedThread
        synchronized (this) 
        {
            if (mState != BluetoothSPPState.STATE_CONNECTED) 
            	return;
            r = mConnectedThread;
        }
        // Perform the write unsynchronized
        r.write(out);
    }
    
    private void ConnectionFailed() 
    {
        // Start the service over to restart listening mode
        BluetoothSPPService.this.Start(BluetoothSPPService.this.mIsAndroid);
    }

    private void ConnectionLost() 
    {
        // Start the service over to restart listening mode
        BluetoothSPPService.this.Start(BluetoothSPPService.this.mIsAndroid);
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
            while (mState != BluetoothSPPState.STATE_CONNECTED && isRunning) 
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
                    synchronized (BluetoothSPPService.this) 
                    {
                        switch (mState) {
                        case BluetoothSPPState.STATE_LISTEN:
                        case BluetoothSPPState.STATE_CONNECTING:
                            // Situation normal. Start the connected thread.
                            Connected(socket, socket.getRemoteDevice(),
                                    mSocketType);
                            break;
                        case BluetoothSPPState.STATE_NONE:
                        case BluetoothSPPState.STATE_CONNECTED:
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
        private BluetoothSocket mSocket;
        private BluetoothDevice mDevice;
        private String mSocketType;

        public ConnectThread(BluetoothDevice device)
        {
            mDevice = device;
            BluetoothSocket tmp = null;

            try 
            {
                if(BluetoothSPPService.this.mIsAndroid)
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
            synchronized (BluetoothSPPService.this) {
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
        private BluetoothSocket mSocket;
        private InputStream mmInStream;
        private OutputStream mmOutStream;

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
            int recvedNum = 0;
            int recvedNumAA = 0;
            int aMsgSize = 6;
            boolean isMsgError = false;
            
            boolean isStartReadMsg = false;

            // Keep listening to the InputStream while connected
            while (true)
            {
                try 
                {
                	if (TheAppPlayBaseActivity.IsBluetoothSetDataProtocolHex())
                	{
                        int data = mmInStream.read();
                        if(data == 0xAA) 
                        { 
                        	if (isMsgError)
                        	{
                        		isMsgError = false;
                        		recvedNum = 0;
                        		recvedNumAA = 0;
                        		aMsgSize = -1;
                        	}
                        	
                        	if (0 == recvedNumAA)
                        	{
                        		recvedNum = 0;
                            	recvedNumAA = 1;
                                arr_byte = new ArrayList<Integer>();   
                        	}                 
                        	else if (1 == recvedNumAA)
                        	{
                        		recvedNumAA++;
                        	}
                        } 
                        else
                        {
                        	if (1 == recvedNumAA)
                        	{
                        		// 这条消息解包出错，忽略
                        		isMsgError = true;
                        	}
                        	if (2 == recvedNumAA)
                        	{
                        		recvedNumAA = 0;
                        		
                        		if (data == 0x02)
                        		{
                        			aMsgSize = 6;
                        		}
                        		else if (data == 0x03)
                        		{
                        			aMsgSize = 7;
                        		}
                        		else if (data == 0x04)
                        		{
                        			aMsgSize = 8;
                        		}
                        	}                    	
                        }   
                        
                        if (!isMsgError)
                        {
                            arr_byte.add(data);
                            recvedNum++;
                            if (recvedNum >=aMsgSize && aMsgSize>0)
                            {   
                            	if (!isMsgError)
                            	{
                                    buffer = new byte[arr_byte.size()];
                                    for(int i = 0 ; i < arr_byte.size() ; i++)
                                    {
                                        buffer[i] = arr_byte.get(i).byteValue();
                                    }        
                                    
                                    int meslength = buffer.length;                                
                                    
                                    // Send the obtained bytes to the UI Activity
                                    BluetoothSPP.TheBluetoothSPP.msHandler.obtainMessage(BluetoothSPPState.MESSAGE_READ,
                                    		meslength, -1, buffer).sendToTarget();
                                    arr_byte = new ArrayList<Integer>();          
                                    
                                    recvedNum = 0;
                                    aMsgSize = -1;
                            	}                 	
                            }	
                        }
                	}
                	else
                	{
                        int data = mmInStream.read();
                        if ('\n' == data)
                        {                        	
                        	if (isStartReadMsg)
                        	{
                            	arr_byte.add(data);
                            	recvedNum++;
                        		
                        		isStartReadMsg = false;
                        		
                                buffer = new byte[arr_byte.size()];
                                for(int i = 0 ; i < arr_byte.size() ; i++)
                                {
                                    buffer[i] = arr_byte.get(i).byteValue();
                                }
                                recvedNum = 0;
                                // Send the obtained bytes to the UI Activity
                                BluetoothSPP.TheBluetoothSPP.msHandler.obtainMessage(BluetoothSPPState.MESSAGE_READ,
                                		buffer.length, -1, buffer).sendToTarget();
                                
                                arr_byte = new ArrayList<Integer>();
                                recvedNum = 0;
                        	}
                        }
                        else
                        {
                        	isStartReadMsg = true;
                        	
                            arr_byte.add(data);
                            recvedNum++;
                        }
                	}  
                } 
                catch (IOException e)
                {
                    ConnectionLost();
                    // Start the service over to restart listening mode
                    BluetoothSPPService.this.Start(BluetoothSPPService.this.mIsAndroid);
                    break;
                }
            }
        }

        // Write to the connected OutStream.
        // @param buffer  The bytes to write
        public void write(byte[] buffer)
        {
            try
            {
                mmOutStream.write(buffer);
                BluetoothSPP.TheBluetoothSPP.msHandler.obtainMessage(BluetoothSPPState.MESSAGE_WRITE
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
