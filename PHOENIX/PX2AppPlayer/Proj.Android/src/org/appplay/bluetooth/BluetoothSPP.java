package org.appplay.bluetooth;

import java.util.ArrayList;
import java.util.Set;

import org.appplay.bluetooth.BluetoothSPPListener.AutoConnectionListener;
import org.appplay.bluetooth.BluetoothSPPListener.BluetoothConnectionListener;
import org.appplay.lib.AppPlayBaseActivity;
import org.appplay.lib.AppPlayNatives;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.content.Intent;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.ArrayAdapter;

public class BluetoothSPP 
{	
    private AppPlayBaseActivity mContext;    
    private BluetoothAdapter mBluetoothAdapter = null;
	
    private BluetoothSPPListener.BluetoothStateListener mBluetoothStateListener = null;
    private BluetoothSPPListener.OnDataReceivedListener mDataReceivedListener = null;
    private BluetoothSPPListener.BluetoothConnectionListener mBluetoothConnectionListener = null;
    private BluetoothSPPListener.AutoConnectionListener mAutoConnectionListener = null;
    
    private BluetoothSPPService mChatService = null;

    private String mDeviceName = null;
    private String mDeviceAddress = null;

    private boolean mIsAutoConnecting = false;
    private boolean mIsAutoConnectionEnabled = false;
    private boolean mIsConnected = false;
    private boolean mIsConnecting = false;
    private boolean mIsServiceRunning = false;
    
    private String Keyword = "";
    private boolean mIsAndroidSelf = BluetoothSPPState.IS_DEVICE_SELF;
    
    private BluetoothSPPListener.BluetoothConnectionListener bcl;
    private int c = 0;
    
    static public BluetoothSPP TheBluetoothSPP;
    
    public BluetoothSPP(AppPlayBaseActivity context)
    {
    	TheBluetoothSPP = this;
        mContext = context;
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (null != mBluetoothAdapter)
        {
    		SetBluetoothConnectionListener(new BluetoothConnectionListener() {
    			public void onDeviceConnected(String name, String address) {	
    				mContext.BluetoothOnConnected();
    			}

    			public void onDeviceDisconnected() {
    				mContext.BluetoothOnDisConnected();
    			}

    			public void onDeviceConnectionFailed() {
    				Log.i("Check", "Unable to connect");

    				mContext.BluetoothOnConnectFailed();
    			}
    		});

    		SetAutoConnectionListener(new AutoConnectionListener() {
    			public void onNewConnection(String name, String address) {
    				Log.i("Check", "New Connection - " + name + " - " + address);
    			}

    			public void onAutoConnectionStarted() {
    				Log.i("Check", "Auto menu_connection started");
    			}
    		});	
        }
    }
    
    public boolean IsBluetoothAvailable() 
    {
        try 
        {
            if (mBluetoothAdapter == null || mBluetoothAdapter.getAddress().equals(null))
                return false;
        }
        catch (NullPointerException e) 
        {
             return false;
        }
        return true;
    }
    
    public boolean IsBluetoothEnabled() 
    {
        return mBluetoothAdapter.isEnabled();
    }
    
    public boolean IsServiceAvailable()
    {
        return mChatService != null;
    }
    
    public boolean IsAutoConnecting()
    {
        return mIsAutoConnecting;
    }
    
    public boolean StartDiscovery() 
    {
        return mBluetoothAdapter.startDiscovery();
    }
    
    public boolean IsDiscovery()
    {
        return mBluetoothAdapter.isDiscovering();
    }
    
    public boolean CancelDiscovery() 
    {
        return mBluetoothAdapter.cancelDiscovery();
    }
    
    public BluetoothAdapter GetBluetoothAdapter() 
    {
        return mBluetoothAdapter;
    }
    
    public int GetServiceState() 
    {
        if(mChatService != null) 
            return mChatService.GetState();
        else 
            return -1;
    }
    
    public void Start(boolean isAndroidSelf) 
    {
        mChatService = new BluetoothSPPService(mContext, msHandler);        
		Log.d("appplay.ap", "BluetoothSPP SetupService");
    	
        if (mChatService != null) 
        {
            if (mChatService.GetState() == BluetoothSPPState.STATE_NONE) 
            {
                mIsServiceRunning = true;
                mChatService.Start(isAndroidSelf);
                BluetoothSPP.this.mIsAndroidSelf = isAndroidSelf;
            }
        }
    }
    
    public void Shutdown() 
    {
        if (mChatService != null) 
        {
            mIsServiceRunning = false;
            mChatService.Stop();
        }
        
        new Handler().postDelayed(new Runnable()
        {
            public void run() 
            {
                if (mChatService != null)
                {
                    mIsServiceRunning = false;
                    mChatService.Stop();
                }
            }
        }, 500);
    }
    
    public void SetDeviceTarget(boolean isAndroidSelf)
    {
    	Shutdown();
    	Start(isAndroidSelf);
        BluetoothSPP.this.mIsAndroidSelf = isAndroidSelf;
    }
    
    public final Handler msHandler = new Handler()
    {
    	@Override
        public void handleMessage(Message msg)
    	{
            switch (msg.what) 
            {
			case BluetoothSPPState.MESSAGE_WRITE:
                break;
            case BluetoothSPPState.MESSAGE_READ:
                byte[] readBuf = (byte[]) msg.obj;
                String readMessage = new String(readBuf);
                if(readBuf != null && readBuf.length > 0) 
                {
                    if(mDataReceivedListener != null)
                        mDataReceivedListener.onDataReceived(readBuf, readMessage);
                    
                    AppPlayBaseActivity.BluetoothOnRecived(readBuf);
                }
                break;
            case BluetoothSPPState.MESSAGE_DEVICE_NAME:
                mDeviceName = msg.getData().getString(BluetoothSPPState.DEVICE_NAME);
                mDeviceAddress = msg.getData().getString(BluetoothSPPState.DEVICE_ADDRESS);
                if(mBluetoothConnectionListener != null)
                    mBluetoothConnectionListener.onDeviceConnected(mDeviceName, mDeviceAddress);
                mIsConnected = true;
                break;
            case BluetoothSPPState.MESSAGE_TOAST:
                break;
            case BluetoothSPPState.MESSAGE_STATE_CHANGE:
                if(mBluetoothStateListener != null)
                    mBluetoothStateListener.onServiceStateChanged(msg.arg1);
                
                if(mIsConnected && msg.arg1 != BluetoothSPPState.STATE_CONNECTED) 
                {
                    if(mBluetoothConnectionListener != null)
                        mBluetoothConnectionListener.onDeviceDisconnected();
                    if(mIsAutoConnectionEnabled) 
                    {
                    	mIsAutoConnectionEnabled = false;
                        AutoConnect(Keyword);
                    }
                    mIsConnected = false;
                    mDeviceName = null;
                    mDeviceAddress = null;
                }
                
                if(!mIsConnecting && msg.arg1 == BluetoothSPPState.STATE_CONNECTING)
                {
                	mIsConnecting = true;
                } 
                else if(mIsConnecting) 
                {
                    if(msg.arg1 != BluetoothSPPState.STATE_CONNECTED) 
                    {
                        if(mBluetoothConnectionListener != null)
                            mBluetoothConnectionListener.onDeviceConnectionFailed();
                    }
                    mIsConnecting = false;
                }
                
                break;
            }
        }
    };
    
    public Set<BluetoothDevice> GetBondedDevices()
    {
    	return mBluetoothAdapter.getBondedDevices();
    }
    
    public void StopAutoConnect() 
    {
        mIsAutoConnectionEnabled = false;
    }

    public void Connect(String address)
    {
        BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(address);
        if (null != device)
        {
            mChatService.Connect(device);	
        }
        else
        {
            Log.i("appplay.ap", "connect device is null");
        }
    }
    
    public void Disconnect() 
    {
        if(mChatService != null) 
        {
            mIsServiceRunning = false;
            mChatService.Stop();
            
            if(mChatService.GetState() == BluetoothSPPState.STATE_NONE)
            {
                mIsServiceRunning = true;
                mChatService.Start(BluetoothSPP.this.mIsAndroidSelf);
            }
        }
    }
    
    public void SetBluetoothStateListener (BluetoothSPPListener.BluetoothStateListener listener) 
    {
        mBluetoothStateListener = listener;
    }
    
    public void setOnDataReceivedListener (BluetoothSPPListener.OnDataReceivedListener listener) 
    {
        mDataReceivedListener = listener;
    }
    
    public void SetBluetoothConnectionListener (BluetoothSPPListener.BluetoothConnectionListener listener)
    {
        mBluetoothConnectionListener = listener;
    }
    
    public void SetAutoConnectionListener(BluetoothSPPListener.AutoConnectionListener listener)
    {
        mAutoConnectionListener = listener;
    }
    
    public void Enable() 
    {
        mBluetoothAdapter.enable();
    }
    
    public void Send(byte[] data) 
    {
        if(mChatService.GetState() == BluetoothSPPState.STATE_CONNECTED) 
        {
        	mChatService.Write(data);
        }
    }   
    
    public String GetConnectedDeviceName() 
    {
        return mDeviceName;
    }
    
    public String GetConnectedDeviceAddress() 
    {
        return mDeviceAddress;
    }
    
    public String[] GetPairedDeviceName() 
    {
        int c = 0;
        Set<BluetoothDevice> devices = mBluetoothAdapter.getBondedDevices();
        
        String[] name_list = new String[devices.size()];
        for(BluetoothDevice device : devices) 
        {  
            name_list[c] = device.getName();
            c++;
        }  
        return name_list;
    }
    
    public String[] GetPairedDeviceAddress() 
    {
        int c = 0;
        Set<BluetoothDevice> devices = mBluetoothAdapter.getBondedDevices();  
        String[] address_list = new String[devices.size()];
        for(BluetoothDevice device : devices) 
        {  
            address_list[c] = device.getAddress();
            c++;
        }  
        return address_list;
    }
    
    public void AutoConnect(String keywordName) 
    {
        if(!mIsAutoConnectionEnabled)
        {
            Keyword = keywordName;
            mIsAutoConnectionEnabled = true;
            mIsAutoConnecting = true;
            
            if(mAutoConnectionListener != null)
                mAutoConnectionListener.onAutoConnectionStarted();
            
            final ArrayList<String> arr_filter_address = new ArrayList<String>();
            final ArrayList<String> arr_filter_name = new ArrayList<String>();
            
            String[] arr_name = GetPairedDeviceName();
            String[] arr_address = GetPairedDeviceAddress();
            
            for(int i = 0 ; i < arr_name.length ; i++) 
            {
                if(arr_name[i].contains(keywordName)) 
                {
                    arr_filter_address.add(arr_address[i]);
                    arr_filter_name.add(arr_name[i]);
                }
            }
    
            bcl = new BluetoothSPPListener.BluetoothConnectionListener() 
            {
                public void onDeviceConnected(String name, String address) 
                {
                    bcl = null;
                    mIsAutoConnecting = false;
                }
    
                public void onDeviceDisconnected() { }
                
                public void onDeviceConnectionFailed()
                {
                	Log.e("CHeck", "Failed");
                    if(mIsServiceRunning) 
                    {
                        if(mIsAutoConnectionEnabled) 
                        {
                            c++;
                            
                            if(c >= arr_filter_address.size())
                                c = 0;
                            
                            Connect(arr_filter_address.get(c));
                        	Log.e("CHeck", "Connect");
                        	
                            if(mAutoConnectionListener != null)
                            {
                                mAutoConnectionListener.onNewConnection(arr_filter_name.get(c)
                                        , arr_filter_address.get(c));	
                            }
                        }
                        else 
                        {
                            bcl = null;
                            mIsAutoConnecting = false;
                        }
                    }
                }
            };

            SetBluetoothConnectionListener(bcl);
            c = 0;
            
            if(mAutoConnectionListener != null)
            {
                mAutoConnectionListener.onNewConnection(arr_name[c], arr_address[c]);	
            }
            if(arr_filter_address.size() > 0)
            {
                Connect(arr_filter_address.get(c));	
            }
            else 
            {
            }
        }
    }
}
