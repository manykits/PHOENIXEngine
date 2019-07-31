// PX2Arduino.hpp

#ifndef PX2ARDUINO_HPP
#define PX2ARDUINO_HPP

#include "PX2UnityPre.hpp"
#include "PX2Object.hpp"
#include "PX2Bluetooth.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2Serial.hpp"
#include "PX2NetClientConnector.hpp"
#include "PX2NetServer.hpp"
#include "PX2Timestamp.hpp"
#include "PX2Runnable.hpp"

namespace PX2
{

	typedef void(*ArduinoToSendCallback) (const std::string &callback);

	const int Arduino_SocketTCP_MsgID = 7;

	enum ArduinoRecvType
	{
		ART_RECV_DIST,
		ART_RECV_IR,
		ART_RECV_RC,
		ART_MAXZ_TYPE
	};
	typedef void(*ArduinoReceiveCallback) (ArduinoRecvType type, int val);

	#define NUMDISTMAT 6
	#define NUMSERVO 5
	#define NUMSTEPMOTO 4
	
	class PX2_ENGINE_ITEM Arduino : public Object, public Runnable
	{
		PX2_NEW(Arduino);
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(Arduino);

	public:
		Arduino();
		virtual ~Arduino();

		enum Mode
		{
			M_SERIAL,
			M_BLUETOOTH,
			M_SOCKETUDP_Broadcast,
			M_ESP_SOCKETTCP_Connector, // ESP8266
			M_SOCKETTCP_Connector, // SocketTCP_MsgID
			M_EMPTY,
			M_MAX_TYPE
		};

		void Reset();
		bool Initlize(Mode mode, const std::string &port="", int baudrate = 9600);
		bool InitlizeSocketUDP_Broadcast(int targetRobotID, int udpPort);
		bool InitlizeESPSocketTCP_Connector(const std::string &ip = "192.168.4.1", int port = 8888);
		bool InitlizeSocketTCP_Connector(ClientConnector *clientConnector);
		bool InitlizeEmpty();
		bool IsInitlized();
		void Terminate();

		void Update(float elapsedSeconds);
		virtual void Run();

		void SendToGetNetID();
		void SetNetID(int id);
		int GetNetID() const;

		Mode GetMode() const;
		Serial &GetSerial();

		bool AddArduinoReceiveCallbacks(ArduinoReceiveCallback callback);
		bool IsHasArduinoReceiveCallback(ArduinoReceiveCallback callback) const;
		bool AddScriptHandler(const std::string &handler);

		// arduino used -------------------------
	public:
		enum Pin
		{
			P_0 = 0,
			P_1,
			P_2,
			P_3,
			P_4,
			P_5,
			P_6,
			P_7,
			P_8,
			P_9,
			P_10,
			P_11,
			P_12,
			P_13,
			P_A0,
			P_A1,
			P_A2,
			P_A3,
			P_A4,
			P_A5,
			P_A6,
			P_MAX_TYPE
		};

		enum PMode
		{
			PM_INPUT,
			PM_OUTPUT,
			PM_MAX_TYPE
		};
		static std::string PinStr[P_MAX_TYPE];
		static std::string PModeStr[PM_MAX_TYPE];

		void PinMode(Pin pin, PMode mode);
		void DigitalWrite(Pin pin, bool isHigh = true);
		void AnalogWrite(Pin pin, int val);
		int DigitalRead(Pin pin);
		int AnalogRead(Pin pin);

		// servo
		void ServoInit(int i, Pin pin);
		void ServoWrite(int i, int val);

		// dist
		void DistInit(Pin pinTrig, Pin pinEcho);
		void DistTest();
		float GetDist() const;

		// vehicle
		void VehicleInitMotoBoard();
		void VehicleInitMotoBoard4567();
		void VehicleInitMotoBoard298N(Pin pinLA, Pin pinLB, Pin pinLS, Pin pinRA, Pin pinRB, Pin pinRS);
		// pinLA D2
		// pinRA D3
		void VehicleSpeedInit(Pin pinLA, Pin pinLB, Pin pinRA, Pin pinRB);
		enum DirectionType
		{
			DT_NONE,
			DT_FORWARD,
			DT_BACKWARD,
			DT_MAX_TYPE
		};
		static std::string sDirectionTypeStr[DT_MAX_TYPE];

		enum SimpleDirectionType
		{
			SDT_NONE,
			SDT_FORWARD,
			SDT_BACKWARD,
			SDT_LEFT,
			SDT_RIGHT,
			SDT_MAX_TYPE
		};
		static std::string sSimpleDirectionTypeStr[SDT_MAX_TYPE];

		void Run(int motoIndex, DirectionType type, int speed);
		void RunSpeed(int motoIndex, int speed);
		void RunSpeed(int motoIndex, float speedReal, float radius, int reduction);
		void Run(SimpleDirectionType dt, int speed);
		int GetSpeed(int motoIndex) const;
		int GetSpeedDir(int motoIndex) const;
		void Stop();

		enum MP3PlayType
		{
			MPT_PLAY = 0,
			MPT_PAUSE,
			MPT_STOP,
			MPT_NEXT,
			MPT_BEFORE,
			MPT_RANDOM,
			MPT_LOOP_SINGLE,
			MPT_LOOP_SINGLE_CLOSE,
			MPT_LOOP_ALL,
			MPT_LOOP_ALL_CLOSE,
			MPT_VOLUME_INCREASE,
			MPT_VOLUME_DECREASE,
			MPT_MAX_TYPE
		};
		void MP3Init(Pin pinR, Pin pinT);
		void MP3DO(MP3PlayType mpt);
		void MP3Play(int folder, int index);
		void MP3SetVolume(int volume);

		void IRInit(Pin pinReceive);
		void IRSend(int val);
		int GetIRReceive() const;

		void WeightInit(int i, Pin pinOut, Pin pinClk);
		void WeightTest(int i);
		float GetWeight(int i);

		void DistMatInit(int i, Pin pinTrig, Pin pinEcho);
		float GetMatDist(int i) const;

		void DHTInit(Pin pin);
		float GetTemperature() const;
		float GetHumidity() const;

		void RGBLEDInit(Pin pin, int num);
		void RGBLEDSetColor(int index, int r, int g, int b);

		void SegmentInit(Pin clickPin, Pin dataPin);
		void SegmentSetBrightness(int brightness);
		void SegmentClear();
		void SegmentDisplayInt(int val);
		void SegmentDisplayFloat(float val);

		void LEDMatrixInit(Pin pinClk, Pin pinData);
		void LEDMatrixSetBrightness(int brightness);
		void LEDMatrixClearScreen();
		void LEDMatrixLightAt(int x, int y, int width, bool onOff);

		void StepMotoInit(int index, Pin pinVCC, Pin pincPLS, Pin pinDir, Pin pinEnable);
		void StepMotoEnable(int index, bool enable);
		void StepMotoDir(int index, bool forward);
		void StepMotoStep(int index, int delay);

		void AxisInit();
		float AxisGetYAxis() const;
		float AxisGetXAxis() const;
		float AxisGetZAxis() const;
		float AxisGetPitch() const;
		float AxisGetRoll() const;
		float AxisGetYaw() const;

		void RCInit(Pin pin);
		void RCSend(long val);
		int GetRCReceive() const;

		bool AddArduinoToSendCallback(ArduinoToSendCallback callback);
		bool IsHasArduinoToSendCallback(ArduinoToSendCallback callback) const;

		const std::string &GetLastSendContentString() const;
		const std::string &GetLastSendString() const;

	public_internal:
		void _Send(const std::string &str);
		void _SetDist(float dist);
		void _SetIRReceive(int irReceive);
		void _SetPinVal(Pin pin, int val);
		void _SetWeight(int index, float weight);
		void _SetSpeedLR(int left, int dirL, int right, int dirR);
		void _SetDistMat(int index, float val);
		void _SetAxis(unsigned long timeMilliseconds, float yAxis, float xAxis, float zAxis, float pitch,
			float roll, float yaw);
		void _SetRCReceive(int val);
		void _SetHumidity(float humi);
		void _SetTemperature(float temp);

		enum OptionType
		{
			OT_TOGET_NETID, // phoenixframe
			OT_RETURN_NETID,
			OT_PM,
			OT_DW,
			OT_AW,
			OT_RETURN_DR,
			OT_RETURN_AR,
			OT_SVR_I,
			OT_SVR_W,
			OT_DST_I,
			OT_DST_T, // ·ÏÆú
			OT_RETURN_DIST,
			OT_MOTO_I,
			OT_MOTO_RUN,
			OT_MOTO_RUNSIMPLE,
			OT_MOTO_STOP,
			OT_MOTO_I_SPD,
			OT_RETURN_MOTOSPD,
			OT_MOTO_I_DRIVER4567,
			OT_MOTO_I_DRIVER298N,
			OT_MP3_INIT,
			OT_MP3_DO,
			OT_MP3_PLAYFOLDER,
			OT_MP3_SETVOLUME,
			OT_IR_INIT,
			OT_IR_SEND,
			OT_RETURN_IR,
			OT_HX711_I,
			OT_HX711_TEST,
			OT_RETURN_HX711,
			OT_DSTMAT_I,
			OT_RETURN_DSTMAT,
			OT_AXIS_I,
			OT_RETURN_AXIS,
			OT_SET_TIME,
			OT_RC_INIT,
			OT_RC_SEND,
			OT_RETRUN_RC,
			OT_DHT_I,
			OT_RETURN_DHTTEMP,
			OT_RETURN_DHTHUMI,
			OT_LEDSTRIP_I,
			OT_LEDSTRIP_SET,
			OT_SEGMENT_I,
			OT_SEGMENT_BRIGHTNESS,
			OT_SEGMENT_CLEAR,
			OT_SEGMENT_DISPLAY,
			OT_LEDMATRIX_I,
			OT_LEDMATRIX_BRIGHTNESS,
			OT_LEDMATRIX_CLEARSCREEN,
			OT_LEDMATRIX_LIGHTAT,
			OT_STEPMOTO_I,
			OT_STEPMOTO_ENABLE,
			OT_STEPMOTO_DIR,
			OT_STEPMOTO_STEP,
			OT_MC_INTERNAL_LIGHT, // makerclock
			OT_MC_LIGHT,
			OT_MC_SEGMENT,
			OT_MC_MOTO,
			OT_MC_DISTTEST,
			OT_MB_MOTO,	// mbot
			OT_MB_SEND,
			OT_MB_BUZZER,
			OT_VERSION,
			OT_MAX_TYPE
		};
		static std::string sOptTypeStr[OT_MAX_TYPE];

		static std::string _Pin2Str(Pin pin);
		static std::string _PinMode2Str(PMode pm);
		static std::string _Bool2Str(bool bVal);
		static std::string _Int2Str(int val);
		static std::string _DirectionType2Str(DirectionType dt);
		static std::string _SimpleDirectionType2Str(SimpleDirectionType dt);

		static Pin _NetStr2Pin(const std::string &str);
		static PMode _NetStr2PinMode(const std::string &str);
		static bool _NetStr2Bool(const std::string &str);
		static bool _HighLow2Bool(const std::string &str);
		static int _NetStr2Int(const std::string &str);
		static float _NetStr2Float(const std::string &str);
		static DirectionType _NetStr2Dir(const std::string &str);
		static SimpleDirectionType _NetStr2SimpleDir(const std::string &str);

		class PX2_ENGINE_ITEM AxisObj
		{
		public:
			AxisObj();
			~AxisObj();

			bool IsValied;
			Timestamp TStamp;
			float AX;
			float AY;
			float AZ;
			float P;
			float R;
			float Y;
		};

		std::vector<AxisObj> &GetAxisObjs();
		AxisObj GetCurAxisObj();

		std::string NetRecvStr;

	private:
		void _OnCallback(ArduinoRecvType type, int value);
		void _OnToSendCallback(const std::string &str);
		void _SetTime();

		ThreadPtr mThread;
		Mutex *mMutexThread;
		bool mIsThreadRun;
		std::vector<std::string> mThreadSend0;

		bool mIsBlockLoopDoBreak;
		int mBlockLoopUpdateTimes;

		std::vector<ArduinoReceiveCallback> mCallbacks;
		std::vector<std::string> mRecvHandlers;

		std::vector<ArduinoToSendCallback> mToSendCallbacks;

		Mode mMode;
		int mNetID;
		int mTargetRobotID;
		int mRobotUDPPort;
		std::string mTcpIP;
		int mTcpPort;

		bool mIsEverSetTime;
		float mEverSetTime;
		Timestamp mArduinoSetTime;

		std::string mEndStr;

		Serial mSerial;
		ClientConnectorPtr mConnector;
		Server *mServer;

		float mDist;
		int mIRReceive;

		int mLastServoValue[NUMSERVO];

		float mWeight[4];

		int mPinValue[P_MAX_TYPE];
		int mSpeed[4];
		int mSpeedDir[4];

		std::string mLastSendContentString;
		std::string mLastSendString;

		float mDistMat[NUMDISTMAT];

		std::vector<AxisObj> mAxisObjs;
		AxisObj mCurAxisObj;
		float mYAxis;
		float mXAxis;
		float mZAxis;
		float mPitch;
		float mRoll;
		float mYaw;

		int mRCReceiveVal;

		float mDHTTemperature;
		float mDHTHumidity;

#define NumMaxStepMoto 4
		bool mStepMotoEnable[NumMaxStepMoto];
		bool mStepMotoDir[NumMaxStepMoto];
		int mStepMotoRunDelay[NumMaxStepMoto];

		// makeblock
	public:
		void MCSegmentSet(int pin, int val);
		void MCMoto(int pin, int speed);
		void MCTestDist(int pin);

		// mbot
	public:
		void MBotInit();
		void MBotMoto(int leftRight, int speed);
		
		void MBotIRSend(int val);
		int MBotIRGetReceivedValue();

		void MBotIRBuzzer(bool on);
		void MBotIsButtonPressed() const;
		int MBotGetLightSensorValue() const;
	};
	PX2_REGISTER_STREAM(Arduino);
	typedef Pointer0<Arduino> ArduinoPtr;

}

#endif