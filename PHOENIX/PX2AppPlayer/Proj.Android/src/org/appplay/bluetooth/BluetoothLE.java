package org.appplay.bluetooth;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Set;
import java.util.UUID;
import java.util.Vector;

import org.appplay.bluetooth.BluetoothSPPListener.AutoConnectionListener;
import org.appplay.bluetooth.BluetoothSPPListener.BluetoothConnectionListener;
import org.appplay.lib.AppPlayBaseActivity;
import org.appplay.lib.AppPlayNatives;

import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothProfile;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.os.Binder;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.util.Log;

public class BluetoothLE extends Service {
	private Context mContext;
	private BluetoothAdapter mBluetoothAdapter = null;
	private BluetoothGatt mBluetoothGatt = null;

	private BluetoothSPPListener.BluetoothStateListener mBluetoothStateListener = null;
	private BluetoothSPPListener.OnDataReceivedListener mDataReceivedListener = null;
	private BluetoothSPPListener.BluetoothConnectionListener mBluetoothConnectionListener = null;
	private BluetoothSPPListener.AutoConnectionListener mAutoConnectionListener = null;

	private boolean mScanning;
	private Handler mHandler;
	private ArrayList<BluetoothDevice> mLeDevices = new ArrayList<BluetoothDevice>();

	private String mBluetoothDeviceAddress;

	private int mConnectionState = BluetoothSPPState.STATE_NONE;

	private String TAG = "BluetoothLE";
	private static final long SCAN_PERIOD = 10000;

	private BluetoothGattCharacteristic mCharacWrite = null;
	private BluetoothGattCharacteristic mCharacRead = null;

	private ArrayList<BluetoothLESendData> mSendList = new ArrayList<BluetoothLESendData>();
	public boolean mIsCanSend = true;
	public boolean mIsWithRead = true;
	private static int SendItemWirteBytes = 0;
	private static int CurSendItemBytes = 0;
	private static final String SendDataTag = "SendData";
	private static final String RecvDataTag = "RecvData";
	private static final int BLE_SEND_OK = 1;
	private static final int BLE_RECV_OK = 2;

	private BluetoothAdapter.LeScanCallback mLeScanCallback = null;
	private BluetoothGattCallback mGattCallback = null;

	public BluetoothLE(Context context) {
		mContext = context;

		// API必须在以上android4.3
		mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
	}

	public void Initlize() {
		mHandler = new Handler();

		// listener
		SetBluetoothConnectionListener(new BluetoothConnectionListener() {
			public void onDeviceConnected(String name, String address) {
				AppPlayNatives.nativeBluetoothOnConnected();
			}

			public void onDeviceDisconnected() {
				AppPlayNatives.nativeBluetoothOnDisConnected();
			}

			public void onDeviceConnectionFailed() {
				Log.i("Check", "Unable to connect");

				AppPlayNatives.nativeBluetoothOnConnectFailed();
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

		mLeScanCallback = new BluetoothAdapter.LeScanCallback() {
			@Override
			public void onLeScan(final BluetoothDevice device, final int rssi,
					byte[] scanRecord) {
				AppPlayBaseActivity.sTheActivity.runOnUiThread(new Runnable() {
					@Override
					public void run() {
						String name = device.getName();
						String address = device.getAddress();

						if (!mLeDevices.contains(device)) {
							String strDevice = name + "$" + address + "$"
									+ Integer.toString(rssi);
							AppPlayBaseActivity.sTheActivity
									._BtOnDiscoveryNewDevice(strDevice);

							mLeDevices.add(device);
						}
					}
				});
			}
		};

		mGattCallback = new BluetoothGattCallback() {
			@Override
			// 当连接上设备或者失去连接时会回调该函数
			public void onConnectionStateChange(BluetoothGatt gatt, int status,
					int newState) {
				if (newState == BluetoothProfile.STATE_CONNECTED) { //
					mConnectionState = BluetoothSPPState.STATE_CONNECTED;

					if (null != mBluetoothConnectionListener) {
						String name = gatt.getDevice().getName();
						String address = gatt.getDevice().getAddress();

						mBluetoothConnectionListener.onDeviceConnected(name,
								address);
						Log.i(TAG, "Connected to GATT server.");

						boolean descoverService = mBluetoothGatt
								.discoverServices();
						Log.i(TAG, "Attempting to start service discovery:"
								+ descoverService);
					}
				} else if (newState == BluetoothProfile.STATE_DISCONNECTING) {
					mConnectionState = BluetoothSPPState.STATE_NONE;

					if (null != mBluetoothConnectionListener) {
						mBluetoothConnectionListener.onDeviceConnectionFailed();
					}
				} else if (newState == BluetoothProfile.STATE_DISCONNECTED) {
					mConnectionState = BluetoothSPPState.STATE_NONE;
					mCharacWrite = null;
					mCharacRead = null;

					if (null != mBluetoothConnectionListener) {
						mBluetoothConnectionListener.onDeviceDisconnected();
					}
				}
			}

			public List<BluetoothGattService> getSupportedGattServices() {
				if (mBluetoothGatt == null)
					return null;

				return mBluetoothGatt.getServices();
			}

			public void setCharacteristicNotification(
					BluetoothGattCharacteristic chara, boolean enabled) {
				if (mBluetoothAdapter == null || mBluetoothGatt == null) {
					Log.w(TAG, "BluetoothAdapter not initialized");
					return;
				}

				boolean retSuc = mBluetoothGatt.setCharacteristicNotification(
						chara, enabled);
				if (retSuc) {
					for (BluetoothGattDescriptor dp : chara.getDescriptors()) {
						if (dp != null) {
							// if ((chara.getProperties() &
							// BluetoothGattCharacteristic.PROPERTY_NOTIFY) !=
							// 0)
							// {
							// dp.setValue(BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE);
							// }
							// else if ((chara.getProperties() &
							// BluetoothGattCharacteristic.PROPERTY_INDICATE) !=
							// 0)
							// {
							// dp.setValue(BluetoothGattDescriptor.ENABLE_INDICATION_VALUE);
							// }
							dp.setValue(BluetoothGattDescriptor.ENABLE_INDICATION_VALUE);
							mBluetoothGatt.writeDescriptor(dp);
						}
					}
				}
				mBluetoothGatt.setCharacteristicNotification(chara, enabled);
			}

			private void onInitServices(List<BluetoothGattService> services) {
				if (null == services)
					return;

				for (BluetoothGattService gattService : services) {
					if (gattService
							.getUuid()
							.toString()
							.equalsIgnoreCase(
									AppPlayBaseActivity.sTheActivity.WriteServiceID)) {
						List<BluetoothGattCharacteristic> charas = gattService
								.getCharacteristics();
						for (BluetoothGattCharacteristic chara : charas) {
							if (chara
									.getUuid()
									.toString()
									.equalsIgnoreCase(
											AppPlayBaseActivity.sTheActivity.WriteCharaID)) {
								mCharacWrite = chara;
							}
						}
					}

					if (gattService
							.getUuid()
							.toString()
							.equalsIgnoreCase(
									AppPlayBaseActivity.sTheActivity.ReadServiceID)) {
						List<BluetoothGattCharacteristic> charas = gattService
								.getCharacteristics();
						for (BluetoothGattCharacteristic chara : charas) {
							if (chara
									.getUuid()
									.toString()
									.equalsIgnoreCase(
											AppPlayBaseActivity.sTheActivity.ReadCharaID)) {
								mCharacRead = chara;
								setCharacteristicNotification(mCharacRead, true);
							}
						}
					}
				}

				if (mCharacWrite == null) {
					Log.e(TAG, "Write charateristic not found!");
				}

				if (mCharacRead == null) {
					Log.e(TAG, "Read charateristic not found!");
				}
			}

			@Override
			public void onServicesDiscovered(BluetoothGatt gatt, int status) {
				if (status == BluetoothGatt.GATT_SUCCESS) {
					// 找到服务了
					// 在这里可以对服务进行解析，寻找到你需要的服务

					List<BluetoothGattService> services = getSupportedGattServices();
					onInitServices(services);
				} else {
					Log.w(TAG, "onServicesDiscovered received: " + status);
				}
			}

			@Override
			// 当读取设备时会回调该函数
			public void onCharacteristicRead(BluetoothGatt gatt,
					BluetoothGattCharacteristic characteristic, int status) {
				System.out.println("onCharacteristicRead");
				if (status == BluetoothGatt.GATT_SUCCESS) {
					// 读取到的数据存在characteristic当中，
					// 可以通过characteristic.getValue();函数取出。然后再进行解析操作

					int charaProp = characteristic.getProperties();
					if ((charaProp | BluetoothGattCharacteristic.PROPERTY_NOTIFY) > 0) {
						final byte[] data = characteristic.getValue();
						if (data != null && data.length > 0) {
							_ProcessReadVal(data);
						}
					}
				}
			}

			private void _ProcessReadVal(byte[] str) {
				Message msg = mBLESendHandler.obtainMessage(BLE_RECV_OK);
				Bundle bundle = new Bundle();
				bundle.putByteArray(RecvDataTag, str);
				msg.setData(bundle);
				mBLESendHandler.sendMessage(msg);
			}

			@Override
			// 当向设备Descriptor中写数据时，会回调该函数
			public void onDescriptorWrite(BluetoothGatt gatt,
					BluetoothGattDescriptor descriptor, int status) {
				System.out.println("onDescriptorWriteonDescriptorWrite = "
						+ status + ", descriptor ="
						+ descriptor.getUuid().toString());
			}

			@Override
			// 设备发出通知时会调用到该接口
			public void onCharacteristicChanged(BluetoothGatt gatt,
					BluetoothGattCharacteristic characteristic) {
				System.out.println("--------onCharacteristicChanged-----");

				if (characteristic.getValue() != null) {
					byte[] str = characteristic.getValue();
					_ProcessReadVal(str);
				}
			}

			@Override
			public void onReadRemoteRssi(BluetoothGatt gatt, int rssi,
					int status) {
				System.out.println("rssi = " + rssi);
			}

			@Override
			// 当向Characteristic写数据时会回调该函数
			public void onCharacteristicWrite(BluetoothGatt gatt,
					BluetoothGattCharacteristic characteristic, int status) {
				byte[] bytes = characteristic.getValue();
				int writeLength = bytes.length;

				Log.w(TAG, "onCharacteristicWrite" + writeLength);

				SendItemWirteBytes += writeLength;

				if (SendItemWirteBytes >= CurSendItemBytes) {
					SendItemWirteBytes -= CurSendItemBytes;

					Message msg = mBLESendHandler.obtainMessage(BLE_SEND_OK);
					mBLESendHandler.sendMessage(msg);
				}
			};
		};
	}

	private final Handler mBLESendHandler = new Handler() {
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case BLE_SEND_OK:
				mIsCanSend = true;
				if (mIsWithRead)
					mBluetoothGatt.readCharacteristic(mCharacRead);
				_SendAItem();
				break;
			case BLE_RECV_OK:
				byte[] recvString = msg.getData().getByteArray(RecvDataTag);
				AppPlayBaseActivity.BluetoothOnRecived(recvString);
				break;
			}
		}
	};

	public boolean IsBluetoothAvailable() {
		if (null == mBluetoothAdapter)
			return false;

		if (!mContext.getPackageManager().hasSystemFeature(
				PackageManager.FEATURE_BLUETOOTH_LE)) {
			return false;
		}

		return true;
	}

	public boolean IsBluetoothEnabled() {
		return mBluetoothAdapter.isEnabled();
	}

	public int GetConnectionState() {
		return mConnectionState;
	}

	public Set<BluetoothDevice> GetBondedDevices() {
		return mBluetoothAdapter.getBondedDevices();
	}

	public void Shutdown() {
		mIsCanSend = true;
		mIsWithRead = false;
		SendItemWirteBytes = 0;
		mSendList.clear();

		if (mBluetoothGatt == null) {
			return;
		}
		mBluetoothGatt.close();
		mBluetoothGatt = null;
	}

	public void StartDiscovery() {
		mLeDevices.clear();

		mHandler.postDelayed(new Runnable() {
			@Override
			public void run() {
				mScanning = false;
				mBluetoothAdapter.stopLeScan(mLeScanCallback);
			}
		}, SCAN_PERIOD); // 10秒后停止搜索

		mScanning = true;

		mBluetoothAdapter.startLeScan(mLeScanCallback); // 开始搜索
	}

	public void CancelDiscovery() {
		mScanning = false;
		mBluetoothAdapter.stopLeScan(mLeScanCallback);// 停止搜索
	}

	public boolean Connect(String address) {
		mIsCanSend = true;
		mIsWithRead = false;
		SendItemWirteBytes = 0;
		mSendList.clear();

		if (mBluetoothAdapter == null || address == null) {
			Log.w(TAG,
					"BluetoothAdapter not initialized or unspecified address.");
			return false;
		}

		// Previously connected device. Try to reconnect. (先前连接的设备。 尝试重新连接)
		if (mBluetoothDeviceAddress != null
				&& address.equals(mBluetoothDeviceAddress)
				&& mBluetoothGatt != null) {
			Log.d(TAG,
					"Trying to use an existing mBluetoothGatt for connection.");

			if (mBluetoothGatt.connect()) {
				mConnectionState = BluetoothSPPState.STATE_CONNECTING;
				return true;
			} else {
				return false;
			}
		}

		final BluetoothDevice device = mBluetoothAdapter
				.getRemoteDevice(address);
		if (device == null) {
			Log.w(TAG, "Device not found.  Unable to connect.");
			return false;
		}

		// 该函数才是真正的去进行连接
		mBluetoothGatt = device.connectGatt(mContext, false, mGattCallback);
		Log.d(TAG, "Trying to create a new connection.");
		mBluetoothDeviceAddress = address;
		mConnectionState = BluetoothSPPState.STATE_CONNECTING;

		return true;
	}

	public void Disconnect() {
		mIsCanSend = true;
		mIsWithRead = false;
		mSendList.clear();

		if (mBluetoothAdapter == null || mBluetoothGatt == null) {
			Log.w(TAG, "BluetoothAdapter not initialized");
			return;
		}
		mBluetoothGatt.disconnect();
		mBluetoothGatt = null;
	}

	public void Send(byte[] data, boolean withRead) {
		if (mConnectionState == BluetoothSPPState.STATE_CONNECTED) {
			BluetoothLESendData dataSend = new BluetoothLESendData();
			dataSend.Data = data;
			dataSend.IsWithRead = withRead;

			mSendList.add(dataSend);

			if (mIsCanSend) {
				_SendAItem();
			}
		}
	}

	private void _SendAItem() {
		CurSendItemBytes = 0;
		int count = mSendList.size();
		if (count > 0) {
			BluetoothLESendData data = mSendList.get(0);
			mIsWithRead = data.IsWithRead;
			byte[] strData = data.Data;
			_WriteDataToDevice(strData);

			mSendList.remove(0);
			mIsCanSend = false;
		}
	}

	private void _WriteDataToDevice(byte[] data) {
		Log.i(TAG, "data:" + data + "length:" + data.length);

		if (mBluetoothGatt == null)
			return;

		if (null != mCharacWrite) {
			mCharacWrite.setValue(data);
			mBluetoothGatt.writeCharacteristic(mCharacWrite);
		}
	}

	// listener
	public void SetBluetoothStateListener(
			BluetoothSPPListener.BluetoothStateListener listener) {
		mBluetoothStateListener = listener;
	}

	public void setOnDataReceivedListener(
			BluetoothSPPListener.OnDataReceivedListener listener) {
		mDataReceivedListener = listener;
	}

	public void SetBluetoothConnectionListener(
			BluetoothSPPListener.BluetoothConnectionListener listener) {
		mBluetoothConnectionListener = listener;
	}

	public void SetAutoConnectionListener(
			BluetoothSPPListener.AutoConnectionListener listener) {
		mAutoConnectionListener = listener;
	}

	// binder
	public class LocalBinder extends Binder {
		BluetoothLE getService() {
			return BluetoothLE.this;
		}
	}

	@Override
	public IBinder onBind(Intent intent) {
		return mBinder;
	}

	private final IBinder mBinder = new LocalBinder();

	public List<BluetoothGattService> getSupportedGattServices() {
		if (mBluetoothGatt == null)
			return null;

		return mBluetoothGatt.getServices();
	}

	public BluetoothGattService getSupportedGattServices(UUID uuid) {
		BluetoothGattService bluetoothGattService;

		if (mBluetoothGatt == null)
			return null;

		bluetoothGattService = mBluetoothGatt.getService(uuid);
		return bluetoothGattService;
	}

}
