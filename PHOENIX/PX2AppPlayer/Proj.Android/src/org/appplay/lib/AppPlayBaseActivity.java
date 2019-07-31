package org.appplay.lib;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.lang.reflect.Method;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Set;

import android.Manifest;
import android.R.string;
import android.provider.Settings;

import org.apache.http.conn.util.InetAddressUtils;
import org.appplay.ai.VoiceSDK;
import org.appplay.ai.VoiceSDKCreater;
import org.appplay.ap.AppPlayApplication;
import org.appplay.ap.R;
import org.appplay.bluetooth.BluetoothLE;
import org.appplay.bluetooth.BluetoothSPPListener.AutoConnectionListener;
import org.appplay.bluetooth.BluetoothSPPListener.BluetoothConnectionListener;
import org.appplay.bluetooth.BluetoothSPP;
import org.appplay.bluetooth.BluetoothSPPState;
import org.appplay.platformsdk.PlatformSDKNatives;
import org.appplay.platformsdk.PlatformSDK;
import org.appplay.platformsdk.PlatformSDKCreater;

import android.hardware.Camera;
import android.hardware.Camera.CameraInfo;
import android.hardware.Camera.PreviewCallback;
import android.hardware.Camera.Size;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.ActivityManager;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.ActivityManager.RunningAppProcessInfo;
import android.app.PendingIntent;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.ActivityInfo;
import android.content.pm.ApplicationInfo;
import android.content.pm.ConfigurationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.ImageFormat;
import android.graphics.Rect;
import android.graphics.SurfaceTexture;
import android.graphics.YuvImage;
import android.hardware.Camera.PreviewCallback;
import android.hardware.Camera.Size;
import android.hardware.usb.UsbConstants;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbEndpoint;
import android.hardware.usb.UsbInterface;
import android.hardware.usb.UsbManager;
import android.hardware.usb.UsbRequest;
import android.location.Criteria;
import android.location.GpsSatellite;
import android.location.GpsStatus;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.location.LocationProvider;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.telephony.TelephonyManager;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.Toast;

public class AppPlayBaseActivity extends Activity
{
	// Init
	public static boolean sIsInitlized = false;
	
	// Application
    public AppPlayApplication mApplication;
	
	// Activtiy
	public static AppPlayBaseActivity sTheActivity;

	// AppPlay LibSO
	public static String sLibSO_AppPlay_Filename = "";
	public static String sLibSO_AppPlay_Dir = "";
	private static String sLibSO_AppPlay_Name = "AppPlay";

	// Vsersion
	public static String sVersion_Dir = "";
	public static String sVersion_Filename = "";
	public static String sVersion_Filename_Temp = "";

	// Siews
	public AppPlayUpdateLayout mUpdateView = null;
	public AppPlayGLView TheGLView = null;

	private static String msPackageName; // use static to support export
	private boolean mIsAppForeground = true;

	// Bluetooth
	BluetoothSPP mBt = null;
	BluetoothLE mBLE = null;
	
	ArrayList<String> mPairedDeviceStrs = new ArrayList<String>();
	ArrayList<String> mScanedDeviceStrs = new ArrayList<String>();
	
	// usb
    private final int VendorID = 0x20BC;    //这里要改成自己的硬件ID
    private final int ProductID = 0x5500;
    
    private PendingIntent mPermissionIntent;
    private static final String ACTION_USB_PERMISSION =
            "com.demo.xprinter.USB_PERMISSION";

	private static final int REQUEST_LOCATION_PERMISSION = 111;
	private static final int REQUEST_RECORDING_PERMISSION = 112;
    
	private UsbManager mUsbManager;
	private UsbDevice mDeviceFound;
    private UsbInterface mUsbInterfaceFound = null;
    private UsbDeviceConnection mUsbDeviceConnection = null;
    private UsbEndpoint mEndpointOut = null;
    private UsbEndpoint mEndpointIn = null;
    
	// Camera
    private Camera mCamera;
    private SurfaceTexture mSurfaceTexture;
    private int mCameraWidth = 300;
    private int mCameraHeight = 300;
    
    // HardCamera
    //private HardCamera mHardCamera = null;
    
    // Wifi
    //private WifiHelper mWifiHelper = null;
    
	// View
	private static FrameLayout sFrame = null;
	private AppPlayWebViewNatives mWebViewNatives = null;
	
	// Location
	private static LocationManager mlocationManager = null;
	
	// Sensor
	private SensorManager mSensorManager;
	private Sensor mAccelerometer;
	
	// Wake lock
	private WakeLock mWakeLock;

	public static FrameLayout GetFrameLayout() {
		return AppPlayBaseActivity.sFrame;
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		Log.d("appplay.ap", "AppPlayBaseActivity::onCreate");

		sTheActivity = this;
		mApplication = (AppPlayApplication) getApplication();

		super.onCreate(savedInstanceState);
		
	    requestWindowFeature(Window.FEATURE_NO_TITLE); 
	    getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, 
	        WindowManager.LayoutParams.FLAG_FULLSCREEN);

		// Package
		String packageName = getApplication().getPackageName();
		PackageManager packMgr = getApplication().getPackageManager();
		ApplicationInfo info = null;
		try {
			info = packMgr
					.getApplicationInfo(packageName, MODE_WORLD_WRITEABLE);
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}

		String dataDir = info.dataDir;

		sLibSO_AppPlay_Dir = dataDir;
		sLibSO_AppPlay_Filename = dataDir + "/lib" + sLibSO_AppPlay_Name + ".so";
		
		sVersion_Dir = dataDir;
		sVersion_Filename = info.dataDir + "/version.xml";
		sVersion_Filename_Temp = info.dataDir + "/version_Temp.xml";
     
		// Initlize
		Log.d("appplay.ap", "begin - AppPlayActivity::onCreate");

		AppPlayMetaData.Initlize(getApplicationContext());
		
		// ble
		_BluetoothSetBle(false);
		
		if (!isLocationOpen(getApplicationContext())) 
		{
            Intent enableLocate = new Intent(Settings.ACTION_LOCATION_SOURCE_SETTINGS);
            startActivityForResult(enableLocate, REQUEST_LOCATION_PERMISSION);
        }
		
		// wake always
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, 
				WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);		

        //保持cpu一直运行，不管屏幕是否黑屏  
		PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);  
		mWakeLock = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "CPUKeepRunning");  
		mWakeLock.acquire();

		// voice
		if (AppPlayMetaData.sIsVoiceable)
			VoiceSDK.sTheVoiceSDK = VoiceSDKCreater.Create(this);
		
		// usb
		/*
		mPermissionIntent = PendingIntent.getBroadcast(this, 0, new Intent(ACTION_USB_PERMISSION), 0);
        IntentFilter filter = new IntentFilter(ACTION_USB_PERMISSION);
        registerReceiver(mUsbReceiver, filter);
		mUsbManager = (UsbManager)getSystemService(Context.USB_SERVICE);
		_EnumUSBDevice();
		_FindInterface();	
        if (!mUsbManager.hasPermission(mDeviceFound)) {
        	mUsbManager.requestPermission(mDeviceFound, mPermissionIntent);
        }
        else
        {
    		_OpenDevice();
    		_AssignEndpoint();	
        }
        */

		// camera
		//DisplayMetrics dm = new DisplayMetrics();
		//getWindowManager().getDefaultDisplay().getMetrics(dm);
		mCameraWidth = 400;
		mCameraHeight = 300;
		
		//_InitCamera(0);
		
		// hardcamera
		//mHardCamera = new HardCamera(this);
		
		// wifi
		//mWifiHelper = new WifiHelper(this); 

		// frame
		ViewGroup.LayoutParams framelayout_params = 
				new FrameLayout.LayoutParams(FrameLayout.LayoutParams.MATCH_PARENT,
						FrameLayout.LayoutParams.MATCH_PARENT);

		FrameLayout framelayout = new FrameLayout(
				AppPlayBaseActivity.sTheActivity);		
		framelayout.setLayoutParams(framelayout_params);
		
		sFrame = framelayout;
		setContentView(sFrame);
		
		if (null == savedInstanceState)
		{
			mWebViewNatives = new AppPlayWebViewNatives(sFrame);	
		}
		else
		{
			 mWebViewNatives = new AppPlayWebViewNatives(sFrame);             
			 String[] strs = savedInstanceState.getStringArray("WEBVIEW");             
			 mWebViewNatives.setAllWebviews(strs);
			 
			 savedInstanceState.clear();
		}

		if (AppPlayMetaData.sIsNettable)
			PlatformSDK.sThePlatformSDK = PlatformSDKCreater.Create(this);
		else
			Show_GLView();

		Log.d("appplay.ap", "end - AppPlayActivity::onCreate");
	}
	
	@Override
	protected void onSaveInstanceState(Bundle outState) {
		
		Log.d("appplay.ap", "begin - AppPlayActivity::onSaveInstanceState");
		
		if (null != mWebViewNatives)
			outState.putStringArray("WEBVIEW", mWebViewNatives.getAllWebviews());
		
		super.onSaveInstanceState(outState);
		
		Log.d("appplay.ap", "end - AppPlayActivity::onSaveInstanceState");
	}

	private void _SetDeviceID() {
		// Device IDs
		TelephonyManager telephonyManager = (TelephonyManager) getSystemService(Context.TELEPHONY_SERVICE);
		String deviceIDStr = telephonyManager.getDeviceId();
		String subScriberIDStr = telephonyManager.getSubscriberId();

		String serialnumStr = null;
		try {
			Class<?> c = Class.forName("android.os.SystemProperties");
			Method get = c.getMethod("get", String.class, String.class);
			serialnumStr = (String) (get.invoke(c, "ro.serialno", "unknown"));
		} catch (Exception ignored) {
		}
		String serialnumStr2 = null;
		try {
			Class myclass = Class.forName("android.os.SystemProperties");
			Method[] methods = myclass.getMethods();
			Object[] params = new Object[] { new String("ro.serialno"),
					new String("Unknown") };
			serialnumStr2 = (String) (methods[2].invoke(myclass, params));
		} catch (Exception ignored) {
		}

		String androidIDStr = Settings.Secure.getString(getContentResolver(),
				Settings.Secure.ANDROID_ID);

		String myDeviceIDStr = deviceIDStr + serialnumStr;

		Log.d("appplay.ap", "begin - AppPlayNatives.nativeSetDeviceIDStr");
		AppPlayNatives.nativeSetDeviceIDStr(myDeviceIDStr);
		Log.d("appplay.ap", "end - AppPlayNatives.nativeSetDeviceIDStr");
	}

	@Override
	public void onStart() {
		Log.d("appplay.lib", "AppPlayBaseActivity::onStart");
		
		super.onStart();		
	}

	public void onActivityResult(int requestCode, int resultCode, Intent data) 
	{
		if (requestCode == BluetoothSPPState.REQUEST_ENABLE_BT)
		{
			if (resultCode == Activity.RESULT_OK) 
			{
				if (null != mBt)
					mBt.Start(BluetoothSPPState.IS_DEVICE_SELF);
				
				if (null != mBLE)
				{
					mBLE.Initlize();						
					_BtGetPairedDevices();	
				}				
			} 
			else
			{
			}
		}
		
		if (requestCode == REQUEST_LOCATION_PERMISSION) {
            if (isLocationOpen(getApplicationContext())) {
                Log.i("fang", " request location permission success");
                //Android6.0需要动态申请权限
                if (ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_COARSE_LOCATION)
                        != PackageManager.PERMISSION_GRANTED) {
                    //请求权限
                    ActivityCompat.requestPermissions(this,
                            new String[]{Manifest.permission.ACCESS_COARSE_LOCATION,
                                    Manifest.permission.ACCESS_FINE_LOCATION},
                           REQUEST_LOCATION_PERMISSION);
                    if (ActivityCompat.shouldShowRequestPermissionRationale(this,
                            Manifest.permission.ACCESS_COARSE_LOCATION)) {
                        //判断是否需要解释
                       
                    }
                }
 
            } else {
            }
        }
	}

	@Override
	protected void onStop() {
		Log.d("appplay.lib", "AppPlayBaseActivity::onStop");

		super.onStop();

		if (_IsAppOnForeground()) {
			mIsAppForeground = false;
		}
	}

	@Override
	protected void onPause() {
		Log.d("appplay.lib", "AppPlayBaseActivity::onPause");

		super.onPause();

		if (null != TheGLView)
			TheGLView.onPause();
		
		if (null != mWebViewNatives)
			mWebViewNatives.pauseAll();
		
		_StartBackTask();
	}

	@Override
	protected void onResume() {

		Log.d("appplay.lib", "AppPlayBaseActivity::onResume");

		super.onResume();
		
		if (null != TheGLView)
			TheGLView.onResume();		

		if (!mIsAppForeground) 
		{
			if (null != PlatformSDK.sThePlatformSDK)
				PlatformSDK.sThePlatformSDK.OnResume();
			
			if (null != mWebViewNatives)
				mWebViewNatives.resumeAll();

			mIsAppForeground = true;
		}		
		
		_RemoveBackTask();
		
		//before
        Intent intent = getIntent();
        String action = intent.getAction();
		
        UsbDevice device = (UsbDevice)intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);
        if (UsbManager.ACTION_USB_DEVICE_ATTACHED.equals(action)) 
        {
            setUsbDevice(device);
        } 
        else if (UsbManager.ACTION_USB_DEVICE_DETACHED.equals(action)) 
        {
            if (mDeviceFound != null && mDeviceFound.equals(device))
            {
            	setUsbDevice(null);
            }
        }
	}
	
	private void setUsbDevice(UsbDevice device) 
	{
		 mUsbInterfaceFound = null;
	     mEndpointOut = null;
	     mEndpointIn = null;
	     
	     for (int i = 0; i < device.getInterfaceCount(); i++) 
	     {      
	    	 UsbInterface usbif = device.getInterface(i);
	    	 
	    	 UsbEndpoint tOut = null;
	    	 UsbEndpoint tIn = null;
	
	    	 int tEndpointCnt = usbif.getEndpointCount();
	    	 if (tEndpointCnt >= 2) 
	    	 {
	    		 for (int j = 0; j < tEndpointCnt; j++)
	    		 {
	    			 if (usbif.getEndpoint(j).getType() == UsbConstants.USB_ENDPOINT_XFER_BULK) 
	    			 {
	    				 if (usbif.getEndpoint(j).getDirection() == UsbConstants.USB_DIR_OUT) 
	    				 {
	    					 tOut = usbif.getEndpoint(j);
	    				 } 
	    				 else if (usbif.getEndpoint(j).getDirection() == UsbConstants.USB_DIR_IN)
	    				 {
	    					 tIn = usbif.getEndpoint(j);
	    				 }
	    			}
	    		}
	    		 
	    		if (tOut != null && tIn != null)
	    		{
	    			 // This interface have both USB_DIR_OUT
	    			 // and USB_DIR_IN of USB_ENDPOINT_XFER_BULK
	    			 mUsbInterfaceFound = usbif;
	    			 mEndpointOut = tOut;
	    			 mEndpointIn = tIn;
	    		}
	    	}
	    }
	        
	    if (mUsbInterfaceFound == null)
	    {
	    	return;
	    }
	
	    mDeviceFound = device;
	        
	    if (device != null) 
	    {
	    	UsbDeviceConnection connection = mUsbManager.openDevice(device);
	    	if (connection != null && 
	             connection.claimInterface(mUsbInterfaceFound, true)) 
	    	{
	    		mUsbDeviceConnection = connection;
	    		
	    		//Thread thread = new Thread(this);
	    		//thread.start();
	    	}
	    	else
	    	{
	    		mUsbDeviceConnection = null;
	    	}
	    }
	}
	
	// usb
	private void sendCommand(int control) 
	 {
		 synchronized (this) 
		 {
			 if (mUsbDeviceConnection != null) 
			 {
				 byte[] message = new byte[1];
				 message[0] = (byte)control;
				 
				 mUsbDeviceConnection.bulkTransfer(mEndpointOut, message, message.length, 0);
	         }
	     }
	}
	
	// timer
	private void _StartBackTask()
	{
		handler.postDelayed(runnable, TIME); //每隔1s执行  
	}
	
	private void _RemoveBackTask()
	{
		handler.removeCallbacks(runnable);
	}
	
    private int TIME = 200;
    Handler handler = new Handler();  
    Runnable runnable = new Runnable() {  
  
        @Override  
        public void run() {  
            // handler自带方法实现定时器  
            try {
            	
            	if (!mIsAppForeground)
            	{
            		AppPlayNatives.nativeOdle();
            	}
            	            	
                handler.postDelayed(this, TIME);
            } catch (Exception e) {  
                // TODO Auto-generated catch block  
                e.printStackTrace();  
                System.out.println("exception...");  
            }  
        }  
    };  
    
    private final BroadcastReceiver mUsbReceiver = new BroadcastReceiver() {

        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (ACTION_USB_PERMISSION.equals(action)) {
                synchronized (this) {
                    UsbDevice device = (UsbDevice) intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);
                    if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
                        if (device != null) {
                        	
                    		_OpenDevice();
                    		_AssignEndpoint();	
                    		
                        }
                    } else {
                       
                        //finish();
                    }
                }
            }
        }
    };
    
	private void _EnumUSBDevice()
	{
        if (mUsbManager == null)
            return;
 
        HashMap<String, UsbDevice> deviceList = mUsbManager.getDeviceList();
        if (!deviceList.isEmpty())
        { 
        	// deviceList不为空
			StringBuffer sb = new StringBuffer();
			for (UsbDevice device : deviceList.values()) 
			{
			    sb.append(device.toString());
			    sb.append("\n");
				
				// 输出设备信息
				Log.d("appplay.ap", "DeviceInfo: " + device.getVendorId() + " , "
				                        + device.getProductId());
		 
				// 枚举到设备
				if (device.getVendorId() == VendorID && device.getProductId() == ProductID)
				{
					mDeviceFound = device;
				    Log.d("appplay.ap", "枚举设备成功");
				}
			}
		}
	}
	
	private void _FindInterface()
	{
      if (mDeviceFound != null) 
      {
            Log.d("", "interfaceCounts : " + mDeviceFound.getInterfaceCount());
            int interfaceCount = mDeviceFound.getInterfaceCount();
            for (int i = 0; i < interfaceCount; i++) 
            {
                UsbInterface intf = mDeviceFound.getInterface(i);
                // 根据手上的设备做一些判断，其实这些信息都可以在枚举到设备时打印出来
                
                int classID = intf.getInterfaceClass();
                int interfaceSubClass = intf.getInterfaceSubclass();
                int interfaceProtocol = intf.getInterfaceProtocol();
                
                if (intf.getInterfaceClass() == 3
                        && intf.getInterfaceSubclass() == 0
                        && intf.getInterfaceProtocol() == 0) 
                {
                	// fire gun
                	mUsbInterfaceFound = intf;
                    Log.d("appplay.ap", "找到我的设备接口");
                }
                break;
            }
        }
	}
	
	private void _OpenDevice()
	{
		if (null == mUsbManager)
			return;
		
       if (mUsbInterfaceFound != null)
       {
            UsbDeviceConnection conn = null;
            // 在open前判断是否有连接权限；对于连接权限可以静态分配，也可以动态分配权限，可以查阅相关资料
            if (mUsbManager.hasPermission(mDeviceFound)) {
                conn = mUsbManager.openDevice(mDeviceFound);
            }
 
            if (conn == null) {
                return;
            }
 
            if (conn.claimInterface(mUsbInterfaceFound, true)) 
            {
            	mUsbDeviceConnection = conn; // 到此你的android设备已经连上HID设备
                Log.d("appplay.ap", "打开设备成功");
                
                new ConnectionThread().start();
            } else 
            {
                conn.close();
            }
        }
	}
	
    /**
     * 分配端点，IN | OUT，即输入输出;此处我直接用1为OUT端点，0为IN，当然你也可以通过判断
     */
     //USB_ENDPOINT_XFER_BULK 
     /* 
     #define USB_ENDPOINT_XFER_CONTROL 0 --控制传输
     #define USB_ENDPOINT_XFER_ISOC 1 --等时传输
     #define USB_ENDPOINT_XFER_BULK 2 --块传输
     #define USB_ENDPOINT_XFER_INT 3 --中断传输 
     **/
    private void _AssignEndpoint()
    {
	    if (mUsbInterfaceFound != null) 
	    {
	       for (int i = 0; i < mUsbInterfaceFound.getEndpointCount(); i++) { 
	
	          UsbEndpoint ep = mUsbInterfaceFound.getEndpoint(i);
	
	          if (ep.getType() == UsbConstants.USB_ENDPOINT_XFER_INT) 
	          { 
		          if (ep.getDirection() == UsbConstants.USB_DIR_OUT) 
		          { 
		        	  mEndpointOut = ep;
		          } 
		          else
		          { 
		        	  mEndpointIn = ep;
		          } 
		      }
	        }
	     }
    }
    
    private class ConnectionThread extends Thread {
        @Override
        public void run() {
        	while (true)
        	{	    		
        		if (null != mEndpointIn)
        		{
                	byte [] readedData = usbRead(9);
                	if (readedData.length == 9)
                	{
                		String strHex = _BytesToHexString(readedData);
                		AppPlayNatives.nativeUSBSendData(strHex.getBytes());
                	}
                	
                	byte [] sendData = new byte[4];
                	sendData[0] = 0x30;
                	sendData[1] = 0x01;
                	sendData[2] = 0x02;
                	sendData[3] = 0x03;
                	usbWrite(sendData);
        		}	
        	}
        }
    }
    
	public void _USBReceive(final byte[] data)
	{
		runOnUiThread(new Runnable() {

			@Override
			public void run() {
				AppPlayNatives.nativeUSBSendData(data);	
			}
		});
	}
    
	// usb

    public void usbWrite(byte[] data) {
    	usbWrite(data, 0, data.length);
    }

    public void usbWrite(byte[] data, int size) {
    	usbWrite(data, 0, size);
    }

    public void usbWrite(byte[] data, int offset, int size) {
        if (offset != 0) {
            data = Arrays.copyOfRange(data, offset, size);
        }
        if (mEndpointOut == null) {
            // use the control endpoint

        } else {
        	mUsbDeviceConnection.bulkTransfer(mEndpointOut, data, size, 1000);
        }
    }
	
	public byte[] usbRead(int size) {
        return _usbRead(size, -1);
    }

    public byte[] _usbRead(int size, int timeout) 
    {
        byte[] buffer = new byte[size];
        int bytesRead = mUsbDeviceConnection.bulkTransfer(mEndpointIn, buffer, size, timeout);
        if (bytesRead < size) 
        {
            buffer = Arrays.copyOfRange(buffer, 0, bytesRead);
        }
        
        return buffer;
    }
	
    public static String _BytesToHexString(byte[] src)
    {
        StringBuilder stringBuilder = new StringBuilder();
        
        if (src == null || src.length <= 0) 
        {
            return null;  
        }
        
        for (int i = 0; i < src.length; i++) 
        {
            int v = src[i] & 0xFF;  
            String hv = Integer.toHexString(v);  
            if (hv.length() < 2) {  
                stringBuilder.append(0);  
            }  
            stringBuilder.append(hv);  
        }  
        
        return stringBuilder.toString();  
    }  
  

	@Override
	public void onDestroy() {
		super.onDestroy();
		
		if (null != mWakeLock)
		{
			mWakeLock.release();
		}
		
		if (null != VoiceSDK.sTheVoiceSDK)
			VoiceSDK.sTheVoiceSDK.destory();

		if (null != TheGLView)
			TheGLView.Destory();

		if (null != PlatformSDK.sThePlatformSDK)
			PlatformSDK.sThePlatformSDK.Term();
		
		_ShutDownBluetooth();
		
		_ShutdownCamera();
	}	

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		if (keyCode == KeyEvent.KEYCODE_BACK) {
			if (null != PlatformSDK.sThePlatformSDK) {
				PlatformSDK.sThePlatformSDK.OnExist();
			}
			
			moveTaskToBack(false);
			
			return true;
		}

		return super.onKeyDown(keyCode, event);
	}

	// -- internal functions implemented ourself --

	private boolean _IsOpenGLES20Valied() {
		ActivityManager am = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
		ConfigurationInfo info = am.getDeviceConfigurationInfo();

		return (info.reqGlEsVersion >= 0x20000);
	}

	private void _SetPackageName(String packageName) {
		msPackageName = packageName;
		Log.d("appplay.ap", "PackageName:" + msPackageName);

		String apkFilePath = "";
		ApplicationInfo appInfo = null;
		PackageManager packMgmr = getApplication().getPackageManager();
		try {
			appInfo = packMgmr.getApplicationInfo(packageName, 0);
		} catch (NameNotFoundException e) {
			e.printStackTrace();
			throw new RuntimeException("Unable to locate assets, aborting...");
		}
		apkFilePath = appInfo.sourceDir;
		Log.d("appplay.ap", "ResourcePath:" + apkFilePath);

		// set apkPath
		AppPlayNatives.nativeSetApkDataPath(apkFilePath);

		// set
		if (AppPlayMetaData.sIsNettable && AppPlayMetaData.sIsTest) {
			AppPlayNatives.nativeSetDataUpdateServerType("ResourceServerTest");
		}
	}
	
	private void _SetIPMac()
	{
		String ip = getLocalHostIp();
		String mac = getLocalMac();		
		AppPlayNatives.nativeIPMac(ip, mac);
	}
	

	private boolean _IsAppOnForeground() {
		ActivityManager activityManager = (ActivityManager) getApplicationContext()
				.getSystemService(Context.ACTIVITY_SERVICE);
		String packageName = getApplicationContext().getPackageName();
		List<RunningAppProcessInfo> appProcesses = activityManager
				.getRunningAppProcesses();
		if (appProcesses == null)
			return false;
		for (RunningAppProcessInfo appProcess : appProcesses) {
			if (appProcess.processName.equals(packageName)
					&& appProcess.importance == RunningAppProcessInfo.IMPORTANCE_FOREGROUND) {
				return true;
			}
		}
		return false;
	}

	// -- gen functions can be called implemented ourself --

	// show a updateview to load LibSO from server
	public void Show_UpdateView() {
		runOnUiThread(new Runnable() {
			public void run() {
				mUpdateView = new AppPlayUpdateLayout(
						AppPlayBaseActivity.sTheActivity,
						R.layout.appplayupdateview);

				setContentView(mUpdateView);

				mUpdateView.CheckVersion();
			}
		});
	}
	
	private void loadSO(String soName, String soFilename)
	{
		File fileLibSO = new File(soFilename);
		if (fileLibSO.exists()) {
			// a updated so, load it from a update dir

			Log.d("appplay.lib", "begin - load sLibSO(from dir):" + soFilename);
			System.load(soFilename);
			Log.d("appplay.lib", "end - load sLibSO(form dir):"	+ soFilename);
		}
		else
		{ // load so packaged with the first apk

			Log.d("appplay.lib", "begin - load sLibSO(form init packaged):"	+ soName);
			try 
			{
				System.loadLibrary(soName);
			}
			catch (UnsatisfiedLinkError ulink) 
			{
				ulink.printStackTrace();
				Log.d("appplay.lib", "end - load sLibSO(form init packaged Failed):" + soName);
			}

			Log.d("appplay.lib", "end - load sLibSO(form init packaged):" + soName);
		}
	}

	// updated ok, let's show our opengles view
	public void Show_GLView() {
		runOnUiThread(new Runnable() {
			public void run() {

				System.loadLibrary("fmodL");
				
				loadSO(sLibSO_AppPlay_Name, sLibSO_AppPlay_Filename);

				_SetDeviceID();

				// package name
				String packageName = getApplication().getPackageName();
				_SetPackageName(packageName);
				
				// ip mac
				_SetIPMac();

				// set platformsdk
				PlatformSDKNatives
						.SetPlatformSDK(PlatformSDKCreater.sSDK_CurrentName);

				// -- begin FrameLayout --

				// PX2EditText layout
				ViewGroup.LayoutParams edittext_layout_params = new ViewGroup.LayoutParams(
						ViewGroup.LayoutParams.FILL_PARENT,
						ViewGroup.LayoutParams.WRAP_CONTENT);
				AppPlayEditText edittext = new AppPlayEditText(
						AppPlayBaseActivity.sTheActivity);
				edittext.setLayoutParams(edittext_layout_params);

				// ...add to FrameLayout
				sFrame.addView(edittext);

				// view
				if (_IsOpenGLES20Valied()) {
					TheGLView = new AppPlayGLView(
							AppPlayBaseActivity.sTheActivity);
				} else {
					Log.d("appplay.lib", "info - Don't support gles2.0");
					finish();
				}

				sFrame.addView(TheGLView);

				TheGLView.SetEditText(edittext);

				// -- end FrameLayout --

				// hide soupdateview
				if (null != mUpdateView)
					mUpdateView.setVisibility(View.GONE);
				
				if (null != VoiceSDK.sTheVoiceSDK)
					VoiceSDK.sTheVoiceSDK.init();
				
				AppPlayBaseActivity.sIsInitlized = true;
			}
		});
	}

	public void Show_NoNetDlg() {
		Dialog alertDialog = new AlertDialog.Builder(this).setTitle("注意")
				.setMessage("您的网络已经断开，请连接！")
				.setPositiveButton("确定", new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int which) {
						dialog.dismiss();
					}
				}).create();
		alertDialog.show();
	}

	public void Show_NoWifiDialog() {
		Dialog alertDialog = new AlertDialog.Builder(this).setTitle("注意")
				.setMessage("游戏需要更新，您处在非wifi网络环境下，确定进行更新？")
				.setIcon(R.drawable.appicon)
				.setPositiveButton("确定", new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int which) {
						dialog.dismiss();
					}
				})
				.setNegativeButton("取消", new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int which) {
						dialog.dismiss();
						((AppPlayBaseActivity) sTheActivity).MyExit();
					}
				}).create();
		alertDialog.show();
	}

	public void Show_ConnectResServerFailedDlg() {
		Dialog alertDialog = new AlertDialog.Builder(this).setTitle("注意")
				.setMessage("连接服务器失败！")
				.setPositiveButton("确定", new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int which) {
						dialog.dismiss();
					}
				}).create();
		alertDialog.show();
	}

	public void Show_HasNewAPKDlg() {
		Dialog alertDialog = new AlertDialog.Builder(this).setTitle("注意")
				.setMessage("安装包已有新版本,请下载安装.")
				.setPositiveButton("确定", new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int which) {
						dialog.dismiss();
						((AppPlayBaseActivity) sTheActivity).MyExit();
					}
				}).create();
		alertDialog.show();
	}

	public void MyExit() {
		runOnUiThread(new Runnable() {
			public void run() {
				android.os.Process.killProcess(android.os.Process.myPid());
			}
		});
	}
	
	public void _SetScreenOrientation(final int or)
	{
		runOnUiThread(new Runnable() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				if (0 == or)
					((AppPlayBaseActivity) (sTheActivity))
							.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
				else if (1 == or)
					((AppPlayBaseActivity) (sTheActivity))
							.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);	
			}
		});
	}

	public static void SetScreenOrientation(int or) {
		((AppPlayBaseActivity) (sTheActivity))._SetScreenOrientation(or);
	}

	public static String GetPackageName() {
		return msPackageName;
	}
	
   public String getLocalHostIp()
    {
        String ipaddress = "";
        try
        {
            Enumeration<NetworkInterface> en = NetworkInterface.getNetworkInterfaces();
            
            // 遍历所用的网络接口
            while (en.hasMoreElements())
            {
                NetworkInterface nif = en.nextElement(); // 得到每一个网络接口绑定的所有ip
                Enumeration<InetAddress> inet = nif.getInetAddresses();
                
                // 遍历每一个接口绑定的所有ip
                while (inet.hasMoreElements())
                {
                    InetAddress ip = inet.nextElement();
                    if (!ip.isLoopbackAddress() &&
                    		InetAddressUtils.isIPv4Address(ip.getHostAddress()))
                    {
                        return ipaddress = ip.getHostAddress();
                    }
                }
            }
        }
        catch (SocketException e)
        {
            Log.e("feige", "获取本地ip地址失败");
            e.printStackTrace();
        }
        
        return ipaddress;
    }
   
   public String getLocalMac()
   {
       String mac = "";
       WifiManager wifiMng = (WifiManager) getSystemService(Context.WIFI_SERVICE);
       if (null !=wifiMng)
       {
           WifiInfo wifiInfor = wifiMng.getConnectionInfo();
           mac = wifiInfor.getMacAddress();   
       }
       return mac;
   }

	// platform sdk functions

	public static void PlatformSDKLogin() {
		((AppPlayBaseActivity) (sTheActivity))._ThirdPlatformLogin1();
	}

	public void _ThirdPlatformLogin1() {
		this.runOnUiThread(new Runnable() {
			public void run() {
				if (null != PlatformSDK.sThePlatformSDK)
					PlatformSDK.sThePlatformSDK.Login();
			}
		});
	}

	public static void PlatformSDKLogout() {
		((AppPlayBaseActivity) (sTheActivity))._Show_LogoutExitDlg();
	}

	public void _Show_LogoutExitDlg() {
		this.runOnUiThread(new Runnable() {
			public void run() {
				if (null != PlatformSDK.sThePlatformSDK)
					PlatformSDK.sThePlatformSDK.OnLogoutExist();
			}
		});
	}

	public static void PlatformSDKSynPay(final String productID,
			final String productName, final float productPrice,
			final float productOrginalPrice, final int count,
			final String payDescription) {
		((AppPlayBaseActivity) (sTheActivity))._SynPay(productID, productName,
				productPrice, productOrginalPrice, count, payDescription);
	}

	public void _SynPay(final String productID, final String productName,
			final float productPrice, final float productOrginalPrice,
			final int count, final String payDescription) {
		this.runOnUiThread(new Runnable() {
			public void run() {
				if (null != PlatformSDK.sThePlatformSDK)
					PlatformSDK.sThePlatformSDK.SynPay(productID, productName,
							productPrice, productOrginalPrice, count,
							payDescription);
			}
		});
	}

	public static void PlatformSDKASynPay(final String productID,
			final String productName, final float productPrice,
			final float productOrginalPrice, final int count,
			final String payDescription) {
		((AppPlayBaseActivity) (sTheActivity))._ASynPay(productID, productName,
				productPrice, productOrginalPrice, count, payDescription);
	}

	public void _ASynPay(final String productID, final String productName,
			final float productPrice, final float productOrginalPrice,
			final int count, final String payDescription) {
		this.runOnUiThread(new Runnable() {
			public void run() {
				if (null != PlatformSDK.sThePlatformSDK)
					PlatformSDK.sThePlatformSDK.ASynPay(productID, productName,
							productPrice, productOrginalPrice, count,
							payDescription);
			}
		});
	}

	// bluetooth
	
	// bluetooth
	private boolean IsBluetoothBle = false;
	public static void BluetoothSetBle(boolean isBle)
	{
		if (null != sTheActivity) 
		{
			//sTheActivity._BluetoothSetBle(isBle);
		}
	}
	
	public static boolean isLocationOpen(final Context context){
        LocationManager manager = (LocationManager) context.getSystemService(Context.LOCATION_SERVICE);
        //gps定位
        boolean isGpsProvider = manager.isProviderEnabled(LocationManager.GPS_PROVIDER);
        //网络定位
        boolean isNetWorkProvider = manager.isProviderEnabled(LocationManager.NETWORK_PROVIDER);
        return isGpsProvider|| isNetWorkProvider;
	}
	
	private void _BluetoothSetBle(boolean isBle)
	{
		_ShutDownBluetooth();		
		
		IsBluetoothBle = isBle;

		if (AppPlayMetaData.sIsBluetoothable)
		{
			BluetoothAdapter bleAdapter = BluetoothAdapter.getDefaultAdapter();
			if (null != bleAdapter)
			{
				if (!IsBluetoothBle)
					mBt = new BluetoothSPP(this);
				
				// 2.0
				if (null != mBt)
				{
					if (!mBt.IsBluetoothAvailable()) 
					{
					}

					_BtGetPairedDevices();

					IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
					this.registerReceiver(mReceiver, filter);
					filter = new IntentFilter(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
					this.registerReceiver(mReceiver, filter);	
				}
				
				if (IsBluetoothBle)
					mBLE = new BluetoothLE(this);
				
				// 4.0
				if (null != mBLE)
				{
					if (!getPackageManager().hasSystemFeature(PackageManager.FEATURE_BLUETOOTH_LE))
					{
					}
					
					if (!mBLE.IsBluetoothAvailable()) 
					{
					}
					else
					{
						mBLE.Initlize();						
						_BtGetPairedDevices();		
					}
				}
			}
		}
		
		if (AppPlayMetaData.sIsBluetoothable)
		{
			if (null != mBt)
			{
				if (!mBt.IsBluetoothEnabled())
				{
					Intent intent = new Intent(
							BluetoothAdapter.ACTION_REQUEST_ENABLE);
					startActivityForResult(intent, BluetoothSPPState.REQUEST_ENABLE_BT);
				} 
				else 
				{
					if (!mBt.IsServiceAvailable())
					{
						mBt.Start(BluetoothSPPState.IS_DEVICE_SELF);
					}
				}	
			}
			
			if (null != mBLE)
			{
				if (!mBLE.IsBluetoothEnabled())
				{
					Intent intent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
					startActivityForResult(intent, BluetoothSPPState.REQUEST_ENABLE_BT);
				}
				else
				{
					/*_*/
				}
			}
		}
	}
	
	private boolean IsBluetoothDataProtocolHex = false;
	public boolean IsBluetoothSetDataProtocolHex()
	{
		return IsBluetoothDataProtocolHex;
	}
	public static void BluetoothSetDataProtocolHex(boolean isHex)
	{
		if (null != sTheActivity) 
		{
			sTheActivity._BluetoothSetDataProtocolHex(isHex);
		}
	}
	private void _BluetoothSetDataProtocolHex(boolean isHex)
	{
		IsBluetoothDataProtocolHex = isHex;
	}
	
	private void _ShutDownBluetooth()
	{
		if (null != mBt)
		{
			mBt.Shutdown();
			mBt.Disconnect();
			unregisterReceiver(mReceiver);
			mBt.CancelDiscovery();			
			mBt = null;
		}
		
		if (null != mBLE)
		{
			mBLE.Shutdown();
			mBLE.CancelDiscovery();			
			mBLE = null;
		}
	}
	
	public String WriteServiceID = new String("0000ffe0-0000-1000-8000-00805f9b34fb");
	public String WriteCharaID = new String("0000ffe1-0000-1000-8000-00805f9b34fb");
	public String ReadServiceID = new String("0000ffe0-0000-1000-8000-00805f9b34fb"); // ?
	public String ReadCharaID = new String("0000ffe1-0000-1000-8000-00805f9b34fb"); // ?
	public static void BluetoothSetWriteServiceID(String writeServiceID)
	{
		if (null != sTheActivity) 
		{
			sTheActivity._BluetoothSetWriteServiceID(writeServiceID);
		}
	}
	private void _BluetoothSetWriteServiceID(String writeServiceID)
	{
		WriteServiceID = writeServiceID;
	}
	
	public static void BluetoothSetWriteCharaID(String writeCharaID)
	{
		if (null != sTheActivity) 
		{
			sTheActivity._BluetoothSetWriteCharaID(writeCharaID);
		}
	}
	private void _BluetoothSetWriteCharaID(String writeCharaID)
	{
		WriteCharaID = writeCharaID;
	}
	
	public static void BluetoothSetReadServiceID(String readServiceID)
	{
		if (null != sTheActivity) 
		{
			sTheActivity._BluetoothSetReadServiceID(readServiceID);
		}
	}
	private void _BluetoothSetReadServiceID(String readServiceID)
	{
		ReadServiceID = readServiceID;
	}
	
	public static void BluetoothSetReadCharaID(String readCharaID)
	{
		if (null != sTheActivity) 
		{
			sTheActivity._BluetoothSetReadCharaID(readCharaID);
		}
	}
	private void _BluetoothSetReadCharaID(String readCharaID)
	{
		ReadCharaID = readCharaID;
	}
	
	public static boolean BluetoothIsAvailable() {
		if (null != sTheActivity.mBt) {
			return sTheActivity.mBt.IsBluetoothAvailable();
		}
		
		if (null != sTheActivity.mBLE) {
			return sTheActivity.mBLE.IsBluetoothAvailable();
		}
		
		return false;
	}

	public static boolean BluetoothIsConnected() {
		if (null != sTheActivity.mBt) {
			if (sTheActivity.mBt.IsBluetoothAvailable()) {
				return sTheActivity.mBt.GetServiceState() == BluetoothSPPState.STATE_CONNECTED;
			}
		}
		
		if (null != sTheActivity.mBLE) {
			if (sTheActivity.mBLE.IsBluetoothAvailable()) {
				return sTheActivity.mBLE.GetConnectionState() == BluetoothSPPState.STATE_CONNECTED;
			}
		}

		return false;
	}

	public static void BluetoothDisconnect() {
		if (null != sTheActivity) {
			sTheActivity.runOnUiThread(new Runnable() {
				public void run() {
					if (null != sTheActivity.mBt) {
						if (sTheActivity.mBt.IsBluetoothAvailable()) {
							sTheActivity.mBt.Disconnect();
						}
					}
					if (null != sTheActivity.mBLE) {
						if (sTheActivity.mBLE.IsBluetoothAvailable()) {
							sTheActivity.mBLE.Disconnect();
						}
					}	
				}
			});		
		}
	}

	public static void BluetoothConnect(String address) {
		if (null != sTheActivity) {
			sTheActivity._BluetoothConnect(address);
		}
	}
	
	private void _BluetoothConnect(final String address) {
		if (null != sTheActivity) {
			sTheActivity.runOnUiThread(new Runnable() {
				public void run() {
					if (null != mBt) {
						if (mBt.IsBluetoothAvailable()) {
							mBt.Connect(address);
						}
					}
					if (null != mBLE) {
						if (mBLE.IsBluetoothAvailable()) {
							mBLE.Connect(address);
						}
					}	
				}
			});		
		}
	}	
	
	public static void BluetoothOnConnected() 
	{
		if (null != sTheActivity) {
			sTheActivity.runOnUiThread(new Runnable() {
				public void run() {
					sTheActivity._BluetoothOnConnected();
				}
			});		
		}
	}
	
	private void _BluetoothOnConnected()
	{
		AppPlayNatives.nativeBluetoothOnConnected();
	}
	
	public static void BluetoothOnDisConnected() 
	{
		if (null != sTheActivity) {
			sTheActivity.runOnUiThread(new Runnable() {
				public void run() {
					sTheActivity._BluetoothOnDisConnected();
				}
			});		
		}
	}
	
	private void _BluetoothOnDisConnected()
	{
		AppPlayNatives.nativeBluetoothOnDisConnected();
	}
	
	public static void BluetoothOnConnectFailed() 
	{
		if (null != sTheActivity) {
			sTheActivity.runOnUiThread(new Runnable() {
				public void run() {
					sTheActivity._BluetoothOnConnectFailed();
				}
			});		
		}
	}
	
	private void _BluetoothOnConnectFailed()
	{
		AppPlayNatives.nativeBluetoothOnConnectFailed();
	}	

	public static void BluetoothSend(byte[] bytes, boolean needCallback) {
		if (null != sTheActivity) {
				sTheActivity._BluetoothSend(bytes, needCallback);
		}
	}
	
	private void _BluetoothSend(final byte[] bytes, final boolean withRead) {
		if (null != sTheActivity) {
			sTheActivity.runOnUiThread(new Runnable() {
				public void run() {
					if (null != mBt) {
						if (mBt.IsBluetoothAvailable()) {
							mBt.Send(bytes);
						}
					}
					if (null != mBLE && mBLE.IsBluetoothAvailable()) {
						if (mBLE.IsBluetoothAvailable()) {
							mBLE.Send(bytes, withRead);
						}
					}	
				}
			});			
		}
	}

	public static String BluetoothGetPairedDevices() {
		String allStr = new String();

		if (null != sTheActivity.mBt) {
			if (sTheActivity.mBt.IsBluetoothAvailable()) {
				sTheActivity._BtGetPairedDevices();

				for (int i = 0; i < sTheActivity.mPairedDeviceStrs.size(); i++) {
					String str = sTheActivity.mPairedDeviceStrs.get(i);
					allStr += str;
					allStr += ";";
				}
			}
		}
		
		if (null != sTheActivity.mBLE) {
			if (sTheActivity.mBLE.IsBluetoothAvailable()) {
				sTheActivity._BtGetPairedDevices();

				for (int i = 0; i < sTheActivity.mPairedDeviceStrs.size(); i++) {
					String str = sTheActivity.mPairedDeviceStrs.get(i);
					allStr += str;
					allStr += ";";
				}
			}
		}

		return allStr;
	}

	public static void BluetoothDoDiscovery() {
		if (null != sTheActivity) {
			sTheActivity.runOnUiThread(new Runnable() {
				public void run() {
					if (null != sTheActivity.mBt) {
						if (sTheActivity.mBt.IsBluetoothAvailable()) {
							sTheActivity._BtDoDiscovery();
						}
					}
					if (null != sTheActivity.mBLE && sTheActivity.mBLE.IsBluetoothAvailable()) {
						if (sTheActivity.mBLE.IsBluetoothAvailable()) {
							sTheActivity._BtDoDiscovery();
						}
					}	
				}
			});			
		}
	}

	public static void BluetoothCancelDiscovery() {		
		if (null != sTheActivity) {
			sTheActivity.runOnUiThread(new Runnable() {
				public void run() {
					if (null != sTheActivity.mBt) {
						if (sTheActivity.mBt.IsBluetoothAvailable()) {
							sTheActivity.mBt.CancelDiscovery();
						}
					}
					if (null != sTheActivity.mBLE && sTheActivity.mBLE.IsBluetoothAvailable()) {
						if (sTheActivity.mBLE.IsBluetoothAvailable()) {
							sTheActivity.mBLE.CancelDiscovery();
						}
					}
				}
			});
		}
	}
	
	public static void BluetoothOnRecived(final byte[] str)
	{
		if (null != sTheActivity) {
			sTheActivity.runOnUiThread(new Runnable() {
				public void run() {					
					AppPlayNatives.nativeBluetoothReceived(str);
				}
			});
		}
	}
	
	private void _BtGetPairedDevices() {
		mPairedDeviceStrs.clear();
		
		if (null != mBt)
		{
			Set<BluetoothDevice> devices = mBt.GetBondedDevices();
			for (BluetoothDevice device : devices) 
			{
				mPairedDeviceStrs.add(device.getName() + "$" + device.getAddress()
					+ "$" + Integer.toString(-1));
			}
		}
		
		if (null != mBLE && mBLE.IsBluetoothAvailable())
		{
			Set<BluetoothDevice> devices = mBLE.GetBondedDevices();
			for (BluetoothDevice device : devices) 
			{
				mPairedDeviceStrs.add(device.getName() + "$" + device.getAddress()
					+ "$" + Integer.toString(-1));
			}
		}
	}

	private void _BtDoDiscovery() {
		mScanedDeviceStrs.clear();
		
		if (null != mBt)
		{
			mBt.CancelDiscovery();			
			mBt.StartDiscovery();	
		}
		if (null != mBLE && mBLE.IsBluetoothAvailable())
		{			
			mBLE.CancelDiscovery();
			mBLE.StartDiscovery();
		}

		Log.d("appplay.ap", "AppPlayBaseActivity::_BtDoDiscovery");
	}

	public void _BtOnDiscoveryNewDevice(String strDevice) {
		mScanedDeviceStrs.add(strDevice);
		AppPlayNatives.nativeBluetoothDiscoveryNewDevice(strDevice);
	}

	private final BroadcastReceiver mReceiver = new BroadcastReceiver() {
		public void onReceive(Context context, Intent intent) {
			String action = intent.getAction();

			// When discovery finds a device
			if (BluetoothDevice.ACTION_FOUND.equals(action))
			{
				// Get the BluetoothDevice object from the Intent
				BluetoothDevice device = intent
						.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);

				String strDevice = device.getName() + "$" + device.getAddress() + "$"
									+ Integer.toString(-1);

				AppPlayBaseActivity.sTheActivity._BtOnDiscoveryNewDevice(strDevice);
			} 
			else if (BluetoothAdapter.ACTION_DISCOVERY_FINISHED.equals(action))
			{
				AppPlayNatives.nativeBluetoothDiscoveryFinished();
			}
		}
	};
	
	// wifi
	public static void WifiConnect(final String path, final String password)
	{
		if (null != sTheActivity) {
			sTheActivity.runOnUiThread(new Runnable() {
				public void run() {
					sTheActivity._WifiConnect(path, password);					
				}
			});			
		}
	}
	
	private void _WifiConnect(String path, String password)
	{
		//mHardCamera.connectDevice(path, password);
	}
	
	public static void WifiDisconnect()
	{
		if (null != sTheActivity) {
			sTheActivity.runOnUiThread(new Runnable() {
				public void run() {
					sTheActivity._WifiDisconnect();					
				}
			});			
		}
	}
	
	private void _WifiDisconnect()
	{
		//mHardCamera.disconnectDevice();
	}
	
	public static void WifiDoDiscovery() {
		if (null != sTheActivity) {
			sTheActivity.runOnUiThread(new Runnable() {
				public void run() {
					sTheActivity._WifiDoDiscovery();					
				}
			});			
		}
	}
	
	public void _WifiDoDiscovery(){
		//mWifiHelper.ScanWifi();
	}

	public static void WifiCancelDiscovery() {		
		if (null != sTheActivity) {
			sTheActivity.runOnUiThread(new Runnable() {
				public void run() {
					sTheActivity._WifiCancelDiscovery();	
				}
			});
		}
	}
	
	public void _WifiCancelDiscovery(){
		
	}

	// voice recognize

	// Voice
	public static void StartVoiceListening() {
		if (null != sTheActivity) {
			sTheActivity.runOnUiThread(new Runnable() {
				public void run() {
					if (null != VoiceSDK.sTheVoiceSDK)
						VoiceSDK.sTheVoiceSDK.StartVoiceListening();
				}
			});
		}
	}

	public static void EndVoiceListening() {
		if (null != sTheActivity) {
			sTheActivity.runOnUiThread(new Runnable() {
				public void run() {
					if (null != VoiceSDK.sTheVoiceSDK)
						VoiceSDK.sTheVoiceSDK.EndVoiceListening();
				}
			});
		}
	}
	
	public static void VoiceSpeak(String text){
		if (null != sTheActivity)
		{
			sTheActivity._VoiceSpeak(text);
		}
	}
	
	private void _VoiceSpeak(final String text)
	{	
		this.runOnUiThread(new Runnable() {
			public void run() {
				if (null != VoiceSDK.sTheVoiceSDK)
					VoiceSDK.sTheVoiceSDK.speak(text);
			}
		});
	}
	
	public static void EnableAutoSpeak(boolean autoSpeak)
	{
		if (null != sTheActivity)
		{
			sTheActivity._EnableAutoSpeak(autoSpeak);
		}
	}
	
	private void _EnableAutoSpeak(final boolean autoSpeak)
	{
		this.runOnUiThread(new Runnable() {
			public void run() {
				if (null != VoiceSDK.sTheVoiceSDK)
					VoiceSDK.sTheVoiceSDK.enableAutoSpeach(autoSpeak);
			}
		});
	}
	
	public static void EnableAutoSpeakTTS(boolean autoSpeak)
	{
		if (null != sTheActivity)
		{
			sTheActivity._EnableAutoSpeakTTS(autoSpeak);
		}
	}
	
	private void _EnableAutoSpeakTTS(final boolean autoSpeak)
	{
		this.runOnUiThread(new Runnable() {
			public void run() {
				if (null != VoiceSDK.sTheVoiceSDK)
					VoiceSDK.sTheVoiceSDK.enableAutoSpeachTTS(autoSpeak);
			}
		});
	}
		
	// Camera
	public static void OpenCameraStream(final int type)
	{
		if (null != sTheActivity) {
			sTheActivity.runOnUiThread(new Runnable() {
				public void run() {
					AppPlayBaseActivity.sTheActivity._InitCamera(type);
				}
			});
		}
	}
	
	public static void CloseCameraStream()
	{
		if (null != sTheActivity) {
			sTheActivity.runOnUiThread(new Runnable() {
				public void run() {
					AppPlayBaseActivity.sTheActivity._ShutdownCamera();
				}
			});
		}
	}
	
	private static final int MAGIC_TEXTURE_ID = 10;
	
	// 0 后置，1前置
	@SuppressLint("NewApi") public void _InitCamera(int type)
	{
		if (null != mCamera)
		{
			mCamera.stopPreview();
			mCamera = null;
		}
		
		if (0 == type)
		{
			mCamera = Camera.open();
		}
		else
		{
			int k = 0;
			if((k=FindFrontCamera())!=-1)
			{
				mCamera = Camera.open(k);
			}
		}
		
		if (mCamera != null)
		{	
			try
			{	
				mSurfaceTexture=new SurfaceTexture(MAGIC_TEXTURE_ID);
				mSurfaceTexture.setDefaultBufferSize(mCameraWidth, mCameraHeight);
				
				Camera.Parameters parameters = mCamera.getParameters();
				parameters.setPreviewSize(mCameraWidth, mCameraHeight);
				parameters.setPictureFormat(ImageFormat.NV21); // 设置图片格式
				parameters.setPictureSize(mCameraWidth, mCameraHeight); // 设置照片的大小
				mCamera.setPreviewTexture(mSurfaceTexture);
				mCamera.setDisplayOrientation(90);				
				mCamera.setPreviewCallback(new PreviewCallback() {				
					@Override
					public void onPreviewFrame(byte[] data, Camera c) {
				        //处理data  
				        Camera.Size previewSize = mCamera.getParameters().getPreviewSize();//获取尺寸,格式转换的时候要用到  
				        BitmapFactory.Options newOpts = new BitmapFactory.Options();  
				        newOpts.inJustDecodeBounds = true;  
				        YuvImage yuvimage = new YuvImage(  
				                data,  
				                ImageFormat.NV21,  
				                previewSize.width,  
				                previewSize.height,  
				                null);  
				        ByteArrayOutputStream baos = new ByteArrayOutputStream();  
				        yuvimage.compressToJpeg(new Rect(0, 0, previewSize.width, previewSize.height), 100, baos);// 80--JPG图片的质量[0-100],100最高  
				        byte[] rawImage = baos.toByteArray();  
				        //将rawImage转换成bitmap  
				        BitmapFactory.Options options = new BitmapFactory.Options();  
				        options.inPreferredConfig = Bitmap.Config.ARGB_8888;  
				        
				        Bitmap bitmap = BitmapFactory.decodeByteArray(rawImage, 0, rawImage.length, options); 
                        if (bitmap != null)
                        {
                            try 
                            {    
		                    	int bitWidth = bitmap.getWidth();
		                    	int bitHeight = bitmap.getHeight();                            	
		  
		                		ByteBuffer buf = ByteBuffer.allocate(bitmap.getRowBytes() * bitmap.getHeight());
		                		bitmap.copyPixelsToBuffer(buf);
		                		byte [] imageData = buf.array();
		                    	int length = imageData.length;
		                    	AppPlayBaseActivity.SetCameraData(bitWidth, bitHeight, imageData);
                            }
                            finally
                            {
                                if (!bitmap.isRecycled()) 
                                {
                                    bitmap.recycle();
                                }
                            }
                        }
					}					
				});
				mCamera.startPreview(); // 开始预览	
				mCamera.autoFocus(null); // 自动对焦
			}
			catch (Exception e) 
			{
				e.printStackTrace();
			}
		}
	}
	
	public static void SetCameraData(final int width, final int height, final byte[] data)
	{
		if (null != sTheActivity) {
			sTheActivity.runOnUiThread(new Runnable() {
				public void run() {
					AppPlayNatives.nativeCameraSendFrame(width, height, data);
				}
			});
		}
	}
	
	public void _ShutdownCamera()
	{
		if (null != mCamera)
		{
			mCamera.setPreviewCallback(null);
			mCamera.stopPreview();
			mCamera.release();
			mCamera = null;
		}
	}	
	
	// 调用前置摄像头
	private int FindFrontCamera()
	{
		 int cameraCount = 0;  
		 Camera.CameraInfo cameraInfo = new Camera.CameraInfo();  
		 cameraCount = Camera.getNumberOfCameras(); // get cameras number   
        
		 for ( int camIdx = 0; camIdx < cameraCount;camIdx++ ) 
		 {  
		     Camera.getCameraInfo( camIdx, cameraInfo ); // get camerainfo   
		     if (cameraInfo.facing ==Camera.CameraInfo.CAMERA_FACING_FRONT) 
		     {   
		    	 // 代表摄像头的方位，目前有定义值两个分别为CAMERA_FACING_FRONT前置和CAMERA_FACING_BACK后置   
		    	 return camIdx;
		     }  
		 }
		 
		 return -1;  
	} 
	
	// 调用后置摄像头
	private int FindBackCamera()
	{  
		int cameraCount = 0;  
		Camera.CameraInfo cameraInfo = new Camera.CameraInfo();  
		cameraCount = Camera.getNumberOfCameras(); // get cameras number   
		
		for ( int camIdx = 0; camIdx < cameraCount;camIdx++ ) 
		{  
		   Camera.getCameraInfo( camIdx, cameraInfo ); // get camerainfo   
		   if ( cameraInfo.facing ==Camera.CameraInfo.CAMERA_FACING_BACK )
		   {   
		       // 代表摄像头的方位，目前有定义值两个分别为CAMERA_FACING_FRONT前置和CAMERA_FACING_BACK后置   
			   return camIdx;  
		   }  
		}
		
		return -1;  
	}  
	
	// Location
	void StartGPS()
	{
		// Location
		mlocationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);
        if (!mlocationManager.isProviderEnabled(LocationManager.GPS_PROVIDER)) 
        {  
			// 返回开启GPS导航设置界面  
            Intent intent = new Intent(Settings.ACTION_LOCATION_SOURCE_SETTINGS);  
            startActivityForResult(intent, 0);  
        }
        String bestProvider = mlocationManager.getBestProvider(_GetCriteria(), true); 
        Location location = mlocationManager.getLastKnownLocation(bestProvider);  
        mlocationManager.addGpsStatusListener(mGpsStatusListener);
        mlocationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 1000, 1, mLocationListener);
	}
	
    private Criteria _GetCriteria() 
    {  
        Criteria criteria = new Criteria();  
        // 设置定位精确度 Criteria.ACCURACY_COARSE比较粗略，Criteria.ACCURACY_FINE则比较精细  
        criteria.setAccuracy(Criteria.ACCURACY_FINE);  
        // 设置是否要求速度  
        criteria.setSpeedRequired(false);  
        // 设置是否允许运营商收费  
        criteria.setCostAllowed(false);  
        // 设置是否需要方位信息  
        criteria.setBearingRequired(false);  
        // 设置是否需要海拔信息  
        criteria.setAltitudeRequired(false);  
        // 设置对电源的需求  
        criteria.setPowerRequirement(Criteria.POWER_LOW);  
        return criteria;  
    }    
        
    // 位置监听  
    private LocationListener mLocationListener = new LocationListener() {  
  
        /** 
         * 位置信息变化时触发 
         */  
        public void onLocationChanged(Location location) {
        	//Log.d("appplay.lib", "时间：" + location.getTime());  
           // Log.d("appplay.lib", "经度：" + location.getLongitude());  
           // Log.d("appplay.lib", "纬度：" + location.getLatitude());  
           // Log.d("appplay.lib", "海拔：" + location.getAltitude());   
        }  
  
        /** 
         * GPS状态变化时触发 
         */  
        public void onStatusChanged(String provider, int status, Bundle extras) {  
            switch (status) {  
            case LocationProvider.AVAILABLE:  
               // Log.d("appplay.lib", "当前GPS状态为可见状态");  
                
            	break;  
            // GPS状态为服务区外时  
            case LocationProvider.OUT_OF_SERVICE:  
            	
               // Log.d("appplay.lib", "当前GPS状态为服务区外状态");  
                break;  
            // GPS状态为暂停服务时  
            case LocationProvider.TEMPORARILY_UNAVAILABLE:  
               // Log.d("appplay.lib", "当前GPS状态为暂停服务状态");  
                break;  
            }  
        }  
  
        /** 
         * GPS开启时触发 
         */  
        public void onProviderEnabled(String provider) {  
            Location location = mlocationManager.getLastKnownLocation(provider);   
        }  
  
        /** 
         * GPS禁用时触发 
         */  
        public void onProviderDisabled(String provider) {      
        }  
  
    };  
  
    // 状态监听  
    GpsStatus.Listener mGpsStatusListener = new GpsStatus.Listener() {  
        public void onGpsStatusChanged(int event) {  
            switch (event) {  
            // 第一次定位  
            case GpsStatus.GPS_EVENT_FIRST_FIX:  
              //  Log.i("appplay.lib", "第一次定位");  
                break;  
                
            // 卫星状态改变  
            case GpsStatus.GPS_EVENT_SATELLITE_STATUS:  
               // Log.i("appplay.lib", "卫星状态改变");  
                // 获取当前状态  
                GpsStatus gpsStatus = mlocationManager.getGpsStatus(null);  
                // 获取卫星颗数的默认最大值  
                int maxSatellites = gpsStatus.getMaxSatellites();  
                // 创建一个迭代器保存所有卫星  
                Iterator<GpsSatellite> iters = gpsStatus.getSatellites()  
                        .iterator();  
                int count = 0;  
                while (iters.hasNext() && count <= maxSatellites) {  
                    GpsSatellite s = iters.next();  
                    count++;  
                }  
              //  System.out.println("搜索到：" + count + "颗卫星");  
                break;  
            // 定位启动  
            case GpsStatus.GPS_EVENT_STARTED:  
               // Log.i("appplay.lib", "定位启动");  
                break;  
            // 定位结束  
            case GpsStatus.GPS_EVENT_STOPPED:  
               // Log.i("appplay.lib", "定位结束");  
                break;  
            }  
        };  
    };  
    
	public static void StartAccelerator()
	{
		if (null != sTheActivity) {
			sTheActivity.runOnUiThread(new Runnable() {
				public void run() {
					AppPlayBaseActivity.sTheActivity._StartAccelerator();
				}
			});
		}
	}
    
	public void _StartAccelerator()
    {
        // Sensor
        mSensorManager = (SensorManager)getSystemService(SENSOR_SERVICE);
        mAccelerometer = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
    }
	
	public static void RegistAccelerator()
	{
		if (null != sTheActivity) {
			sTheActivity.runOnUiThread(new Runnable() {
				public void run() {
					AppPlayBaseActivity.sTheActivity._RegistAccelerator();
				}
			});
		}
	}
    
    void _RegistAccelerator()
    {
    	if (null != mSensorManager)
    		mSensorManager.registerListener(mSensorEventListener, mAccelerometer, SensorManager.SENSOR_DELAY_NORMAL);
    }
    
	public static void UnRegistAccelerator()
	{
		if (null != sTheActivity) {
			sTheActivity.runOnUiThread(new Runnable() {
				public void run() {
					AppPlayBaseActivity.sTheActivity._UnRegistAccelerator();
				}
			});
		}
	}
    
    void _UnRegistAccelerator()
    {
		if (null !=mSensorManager)
			mSensorManager.unregisterListener(mSensorEventListener);
    }
    
    SensorEventListener mSensorEventListener  = new SensorEventListener()
    {
		@Override
		public void onSensorChanged(SensorEvent event) {
			// TODO Auto-generated method stub
	        float xValue = event.values[0];// Acceleration minus Gx on the x-axis
	        float yValue = event.values[1];//Acceleration minus Gy on the y-axis
	        float zValue = event.values[2];//Acceleration minus Gz on the z-axis
	        
	        AppPlayNatives.nativeAcceleratorChanged(xValue, yValue, zValue);
		}

		@Override
		public void onAccuracyChanged(Sensor sensor, int accuracy) {
			// TODO Auto-generated method stub	
		}
    	
    };
}
