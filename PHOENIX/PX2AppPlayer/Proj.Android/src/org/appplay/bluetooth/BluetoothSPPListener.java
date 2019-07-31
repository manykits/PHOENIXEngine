package org.appplay.bluetooth;

public class BluetoothSPPListener {
	
	BluetoothSPPListener()
	{
	}
	
    public interface BluetoothStateListener
    {
        public void onServiceStateChanged(int state);
    }
    
    public interface OnDataReceivedListener 
    {
        public void onDataReceived(byte[] data, String message);
    }
    
    public interface BluetoothConnectionListener 
    {
        public void onDeviceConnected(String name, String address);
        public void onDeviceDisconnected();
        public void onDeviceConnectionFailed();
    }
    
    public interface AutoConnectionListener 
    {
        public void onAutoConnectionStarted();
        public void onNewConnection(String name, String address);
    }

}
