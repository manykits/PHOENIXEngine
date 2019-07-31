// PX2STEAMEdu.cpp

#include "PX2STEAMEdu.hpp"
#include "PX2FileIO.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2ScopedCS.hpp"
#include "PX2StringTokenizer.hpp"
#include "PX2PlatformSDK.hpp"
#include "PX2Base64.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
STEAMEduManager::STEAMEduManager()
{
	TheSemaphore = new0 Semaphore(0, 99999);

	CurReq = 0;
	CurResp = 0;

	mMCPins.push_back(std::pair<std::string, std::string>("10", "11")); // 1
	mMCPins.push_back(std::pair<std::string, std::string>("9", "3")); // 2
	mMCPins.push_back(std::pair<std::string, std::string>("13", "12")); // 3
	mMCPins.push_back(std::pair<std::string, std::string>("2", "8")); // 4
	mMCPins.push_back(std::pair<std::string, std::string>("0", "1")); // 5
	mMCPins.push_back(std::pair<std::string, std::string>("A3", "A2")); // 6
	mMCPins.push_back(std::pair<std::string, std::string>("A1", "A6")); // 7
	mMCPins.push_back(std::pair<std::string, std::string>("A0", "A7")); // 8
	mMCPins.push_back(std::pair<std::string, std::string>("6", "7")); // 9
	mMCPins.push_back(std::pair<std::string, std::string>("4", "5")); // 10
}
//----------------------------------------------------------------------------
STEAMEduManager::~STEAMEduManager()
{
	if (TheSemaphore)
	{
		delete0(TheSemaphore);
		TheSemaphore = 0;
	}
}
//----------------------------------------------------------------------------
void STEAMEduManager::Initlize()
{
	ProcessSeconds = 0;

	CurReq = 0;
	CurResp = 0;

	ServerSocket socket(8389);
	HTTPServerParams *param = new0 HTTPServerParams();
	param->SetMaxQueued(500);
	param->SetMaxThreads(1);

	mHttpServer = new0 HTTPServer(new0 STEAMEduHandlerFactory(), socket, param);
	mHttpServer->Start();
}
//----------------------------------------------------------------------------
void STEAMEduManager::Terminate()
{
	mHttpServer->Stop();
	mHttpServer = 0;
}
//----------------------------------------------------------------------------
void STEAMEduManager::Update(float elapsedSeconds)
{
	if (CurReq && CurResp)
	{
		ProcessSeconds += elapsedSeconds;

		//if (ProcessSeconds > 0.02f)
		{
			ProcessHttp(*CurReq, *CurResp);
			TheSemaphore->Set();

			ProcessSeconds = 0.0f;
			CurReq = 0;
			CurResp = 0;
		}
	}

	std::map<std::string, ArduinoPtr>::iterator it = Arduinos.begin();
	for (; it != Arduinos.end(); it++)
	{
		Arduino *ardu = it->second;
		if (ardu)
		{
			ardu->Update(elapsedSeconds);
		}
	}
}
//----------------------------------------------------------------------------
void STEAMEduManager::OpenArduino(const std::string &filename)
{
#if defined _WIN32 || defined (WIN32)
	std::string cmdStr = "arduino " + filename;
	WinExec(cmdStr.c_str(), SW_SHOW);
#endif
}
//----------------------------------------------------------------------------
Arduino *STEAMEduManager::GetArduinoByName(const std::string &name)
{
	auto it = Arduinos.find(name);
	if (it != Arduinos.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------
void STEAMEduManager::ProcessHttp(HTTPServerRequest& request, 
	HTTPServerResponse& response)
{
	// ÔÊÐí¿çÓò!
	std::string url = request.GetURI();

	StringTokenizer stkQuestion(url, "?");
	if (stkQuestion.Count() > 1)
	{
		int outLength = 0;
		std::string valStr = Base64::Decode(stkQuestion[1], outLength);
		StringTokenizer stkEqual(valStr, "$");

		std::string cmdStr;
		std::string targetStr;
		std::string contentStr;
		std::string count;
		if (stkEqual.Count() > 0)
			cmdStr = stkEqual[0];
		if (stkEqual.Count() > 1)
			targetStr = stkEqual[1];
		if (stkEqual.Count() > 2)
			contentStr = stkEqual[2];
		if (stkEqual.Count() > 3)
			count = stkEqual[3];

		std::cout << "count" << count << std::endl;

		bool findLib = (std::string::npos != cmdStr.find("libraries"));
		bool findBlockly = (std::string::npos != cmdStr.find("blockly_"));

		if (findLib)
		{
			response.add("Access-Control-Allow-Origin", "*");
			std::string path = "Data/CodeSnap/codesnap/www/" + cmdStr;
			response.SendFile(path, "file");
		}
		else if ("Costumes/COSTUMES" == cmdStr)
		{
			response.add("Access-Control-Allow-Origin", "*");
			response.SendFile("Data/CodeSnap/codesnap/www/Costumes/", "file");
		}
		else if ("tools.xml" == cmdStr)
		{
			response.add("Access-Control-Allow-Origin", "*");
			response.SendFile("Data/CodeSnap/codesnap/www/tools.xml", "file");
		}
		else if ("device" == cmdStr)
		{
			if ("list" == contentStr)
			{
				std::string sendCnt;

				Serial serial;
				auto lst = serial.GetPortList();
				for (int i = 0; i < (int)lst.size(); i++)
				{
					sendCnt += lst[i];

					if (i != (int)(lst.size() - 1))
						sendCnt += "_";
				}

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << sendCnt;

				return;
			}
			else
			{
				std::string strParam0;
				std::string strParam1;
				std::string strParam2;

				StringTokenizer stk_(contentStr, "_");
				if (stk_.Count() > 0)
					strParam0 = stk_[0];
				if (stk_.Count() > 1)
					strParam1 = stk_[1];
				if (stk_.Count() > 2)
					strParam2 = stk_[2];

				if ("connect" == strParam0)
				{
					ArduinoPtr arduBefore = PX2_STEAMEDU.GetArduinoByName(strParam1);
					if (arduBefore)
					{
						if (arduBefore->IsInitlized())
						{
							response.add("Access-Control-Allow-Origin", "*");
							std::ostream& ostr = response.Send();
							ostr << "suc";

							return;
						}
					}
					else
					{
						ArduinoPtr ardu = new0 Arduino();
						ardu->Initlize(Arduino::M_SERIAL, strParam1, 9600);

						response.add("Access-Control-Allow-Origin", "*");
						std::ostream& ostr = response.Send();

						if (ardu->IsInitlized())
						{
							PX2_STEAMEDU.Arduinos[strParam1] = ardu;
							ostr << "suc";
						}
						else
						{
							PX2_STEAMEDU.Arduinos.erase(strParam1);

							ostr << "failed";
						}
					}

					return;
				}
				else if ("connectwifi" == strParam0)
				{
					ArduinoPtr ardu = new0 Arduino();
					ardu->InitlizeESPSocketTCP_Connector();

					response.add("Access-Control-Allow-Origin", "*");
					std::ostream& ostr = response.Send();

					if (ardu->IsInitlized())
					{
						PX2_STEAMEDU.Arduinos[strParam1] = ardu;

						ostr << "suc";
					}
					else
					{
						PX2_STEAMEDU.Arduinos.erase(strParam1);

						ostr << "failed";
					}

					return;
				}
				else if ("disconnect" == strParam0)
				{
					Arduino *ardu = PX2_STEAMEDU.GetArduinoByName(targetStr);
					if (ardu)
					{
						ardu->Terminate();
						PX2_STEAMEDU.Arduinos.erase(targetStr);

						response.add("Access-Control-Allow-Origin", "*");
						std::ostream& ostr = response.Send();
						ostr << "suc";
					}
					else
					{
						response.add("Access-Control-Allow-Origin", "*");
						std::ostream& ostr = response.Send();
						ostr << "failed";
					}

					return;
				}
			}
		}
		else if ("phoenix" == cmdStr)
		{
			std::string strParam0;
			StringTokenizer stk_(contentStr, ",");
			if (stk_.Count() > 0)
				strParam0 = stk_[0];

			if ("777" == strParam0)
			{
				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "abcdefg";
			}
			else if ("1001" == strParam0)
			{
				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << PX2_PFSDK.RinGUN_GetCMD();
			}
			else if ("1002" == strParam0)
			{
				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << PX2_PFSDK.RinGun_GetPosX();
			}
			else if ("1003" == strParam0)
			{
				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << PX2_PFSDK.RinGun_GetPosY();
			}
			else if ("1004" == strParam0)
			{
				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << PX2_PFSDK.RinGun_IsFiring();
			}
		}
		else if ("arduino" == cmdStr)
		{
			Arduino *ardu = PX2_STEAMEDU.GetArduinoByName(targetStr);
			if (!ardu)
			{
				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "failed";
				return;
			}

			std::string strParam0;
			std::string strParam1;
			std::string strParam2;
			std::string strParam3;
			std::string strParam4;
			std::string strParam5;
			std::string strParam6;
			std::string strParam7;
			std::string strParam8;
			std::string strParam9;
			std::string strParam10;

			StringTokenizer stk_(contentStr, ",");
			if (stk_.Count() > 0)
				strParam0 = stk_[0];
			if (stk_.Count() > 1)
				strParam1 = stk_[1];
			if (stk_.Count() > 2)
				strParam2 = stk_[2];
			if (stk_.Count() > 3)
				strParam3 = stk_[3];
			if (stk_.Count() > 4)
				strParam4 = stk_[4];
			if (stk_.Count() > 5)
				strParam5 = stk_[5];
			if (stk_.Count() > 6)
				strParam6 = stk_[6];
			if (stk_.Count() > 7)
				strParam7 = stk_[7];
			if (stk_.Count() > 8)
				strParam8 = stk_[8];
			if (stk_.Count() > 9)
				strParam9 = stk_[9];
			if (stk_.Count() > 10)
				strParam10 = stk_[10];

			//OT_PM,
			//OT_DW,
			//OT_AW,
			//OT_RETURN_DR,
			//OT_RETURN_AR,
			//OT_SVR_I,
			//OT_SVR_W,
			//OT_DST_I,
			//OT_DST_T,
			//OT_RETURN_DIST,
			//OT_MOTO_I,
			//OT_MOTO_RUN,
			//OT_MOTO_RUNSIMPLE,
			//OT_MOTO_STOP,

			if (Arduino::sOptTypeStr[Arduino::OT_PM] == strParam0)
			{
				Arduino::Pin pin = Arduino::_NetStr2Pin(strParam1);
				Arduino::PMode pm = Arduino::_NetStr2PinMode(strParam2);

				ardu->PinMode(pin, pm);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_DW] == strParam0)
			{
				Arduino::Pin pin = Arduino::_NetStr2Pin(strParam1);
				bool val = Arduino::_HighLow2Bool(strParam2);

				ardu->DigitalWrite(pin, val);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_AW] == strParam0)
			{
				Arduino::Pin pin = Arduino::_NetStr2Pin(strParam1);
				int val = Arduino::_NetStr2Int(strParam2);

				ardu->AnalogWrite(pin, val);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_RETURN_DR] == strParam0)
			{
				Arduino::Pin pin = Arduino::_NetStr2Pin(strParam1);
				int val = ardu->DigitalRead(pin);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << val;
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_RETURN_AR] == strParam0)
			{
				Arduino::Pin pin = Arduino::_NetStr2Pin(strParam1);
				int val = ardu->AnalogRead(pin);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << val;
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_SVR_I] == strParam0)
			{
				int svrIndex = Arduino::_NetStr2Int(strParam1);
				Arduino::Pin pin = Arduino::_NetStr2Pin(strParam2);

				ardu->ServoInit(svrIndex, pin);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_SVR_W] == strParam0)
			{
				int svrIndex = Arduino::_NetStr2Int(strParam1);
				int val = Arduino::_NetStr2Int(strParam2);

				ardu->ServoWrite(svrIndex, val);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_DST_I] == strParam0)
			{
				Arduino::Pin pinTrigger = Arduino::_NetStr2Pin(strParam1);
				Arduino::Pin pinEcho = Arduino::_NetStr2Pin(strParam2);

				ardu->DistInit(pinTrigger, pinEcho);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_DST_T] == strParam0)
			{
				ardu->DistTest();

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_RETURN_DIST] == strParam0)
			{
				ardu->DistTest();
				float dst = ardu->GetDist();

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << dst;
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_MOTO_I] == strParam0)
			{
				ardu->VehicleInitMotoBoard();

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_MOTO_RUN] == strParam0)
			{
				int mi = Arduino::_NetStr2Int(strParam1);
				Arduino::DirectionType dt = Arduino::_NetStr2Dir(strParam2);
				int speed = Arduino::_NetStr2Int(strParam3);
				ardu->Run(mi, dt, speed);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_MOTO_RUNSIMPLE] == strParam0)
			{
				Arduino::SimpleDirectionType sdt = Arduino::_NetStr2SimpleDir(strParam1);
				int speed = Arduino::_NetStr2Int(strParam2);
				ardu->Run(sdt, speed);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_MOTO_STOP] == strParam0)
			{
				ardu->Stop();

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_MOTO_I_SPD] == strParam0)
			{
				Arduino::Pin pin0 = Arduino::_NetStr2Pin(strParam1);
				Arduino::Pin pin1 = Arduino::_NetStr2Pin(strParam2);
				Arduino::Pin pin2 = Arduino::_NetStr2Pin(strParam3);
				Arduino::Pin pin3 = Arduino::_NetStr2Pin(strParam4);
				ardu->VehicleSpeedInit(pin0, pin1, pin2, pin3);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_RETURN_MOTOSPD] == strParam0)
			{
				int index = Arduino::_NetStr2Int(strParam1);
				int speed = ardu->GetSpeed(index);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << speed;
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_MOTO_I_DRIVER4567] == strParam0)
			{
				ardu->VehicleInitMotoBoard4567();

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_MOTO_I_DRIVER298N] == strParam0)
			{
				Arduino::Pin pinL0 = Arduino::_NetStr2Pin(strParam1);
				Arduino::Pin pinL1 = Arduino::_NetStr2Pin(strParam2);
				Arduino::Pin pinLS = Arduino::_NetStr2Pin(strParam3);
				Arduino::Pin pinR0 = Arduino::_NetStr2Pin(strParam4);
				Arduino::Pin pinR1 = Arduino::_NetStr2Pin(strParam5);
				Arduino::Pin pinRS = Arduino::_NetStr2Pin(strParam6);

				ardu->VehicleInitMotoBoard298N(pinL0, pinL1, pinLS, pinR0, pinR1, pinRS);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_MP3_INIT] == strParam0)
			{
				Arduino::Pin pinR = Arduino::_NetStr2Pin(strParam1);
				Arduino::Pin pinT = Arduino::_NetStr2Pin(strParam2);

				ardu->MP3Init(pinR, pinT);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_MP3_DO] == strParam0)
			{
				Arduino::MP3PlayType type = Arduino::MPT_PLAY;
				if ("play" == strParam1)
				{
					type = Arduino::MPT_PLAY;
				}
				else if ("pause" == strParam1)
				{
					type = Arduino::MPT_PAUSE;
				}
				else if ("stop" == strParam1)
				{
					type = Arduino::MPT_STOP;
				}
				else if ("next" == strParam1)
				{
					type = Arduino::MPT_NEXT;
				}
				else if ("before" == strParam1)
				{
					type = Arduino::MPT_BEFORE;
				}
				else if ("random" == strParam1)
				{
					type = Arduino::MPT_RANDOM;
				}
				else if ("loop_single" == strParam1)
				{
					type = Arduino::MPT_LOOP_SINGLE;
				}
				else if ("loop_single_close" == strParam1)
				{
					type = Arduino::MPT_LOOP_SINGLE_CLOSE;
				}
				else if ("loop_all" == strParam1)
				{
					type = Arduino::MPT_LOOP_ALL;
				}
				else if ("loop_all_close" == strParam1)
				{
					type = Arduino::MPT_LOOP_ALL_CLOSE;
				}
				else if ("volume_increase" == strParam1)
				{
					type = Arduino::MPT_VOLUME_INCREASE;
				}
				else if ("volume_decrease" == strParam1)
				{
					type = Arduino::MPT_VOLUME_DECREASE;
				}

				ardu->MP3DO(type);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_MP3_PLAYFOLDER] == strParam0)
			{
				int param0 = Arduino::_NetStr2Int(strParam1);
				int param1 = Arduino::_NetStr2Int(strParam2);

				ardu->MP3Play(param0, param1);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_MP3_SETVOLUME] == strParam0)
			{
				int val = Arduino::_NetStr2Int(strParam1);

				ardu->MP3SetVolume(val);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_IR_INIT] == strParam0)
			{
				Arduino::Pin pin = Arduino::_NetStr2Pin(strParam1);
				ardu->IRInit(pin);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_IR_SEND] == strParam0)
			{
				int val = Arduino::_NetStr2Int(strParam1);
				ardu->IRSend(val);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_RETURN_IR] == strParam0)
			{
				int irValue = ardu->GetIRReceive();

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << irValue;
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_HX711_I] == strParam0)
			{
				int index = Arduino::_NetStr2Int(strParam1);
				Arduino::Pin pinOut = Arduino::_NetStr2Pin(strParam2);
				Arduino::Pin pinClk = Arduino::_NetStr2Pin(strParam3);
				ardu->WeightInit(index, pinOut, pinClk);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_HX711_TEST] == strParam0)
			{
				int index = Arduino::_NetStr2Int(strParam1);
				ardu->WeightTest(index);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_RETURN_HX711] == strParam0)
			{
				int index = Arduino::_NetStr2Int(strParam1);
				float val = ardu->GetWeight(index);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << val;
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_DHT_I] == strParam0)
			{
				Arduino::Pin pin = Arduino::_NetStr2Pin(strParam1);
				ardu->DHTInit(pin);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_RETURN_DHTTEMP] == strParam0)
			{
				float temp = ardu->GetTemperature();

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << temp;
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_RETURN_DHTHUMI] == strParam0)
			{
				float humi = ardu->GetHumidity();

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << humi;
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_LEDSTRIP_I] == strParam0)
			{
				Arduino::Pin pin = Arduino::_NetStr2Pin(strParam1);
				int num = Arduino::_NetStr2Int(strParam2);

				ardu->RGBLEDInit(pin, num);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_LEDSTRIP_SET] == strParam0)
			{
				int index = Arduino::_NetStr2Int(strParam1);
				int r = Arduino::_NetStr2Int(strParam2);
				int g = Arduino::_NetStr2Int(strParam3);
				int b = Arduino::_NetStr2Int(strParam4);

				ardu->RGBLEDSetColor(index, r, g, b);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_SEGMENT_I] == strParam0)
			{
				Arduino::Pin pinClick = Arduino::_NetStr2Pin(strParam1);
				Arduino::Pin pinData = Arduino::_NetStr2Pin(strParam2);
				ardu->SegmentInit(pinClick, pinData);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_SEGMENT_BRIGHTNESS] == strParam0)
			{
				int brightness = Arduino::_NetStr2Int(strParam1);
				ardu->SegmentSetBrightness(brightness);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_SEGMENT_CLEAR] == strParam0)
			{
				ardu->SegmentClear();

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_SEGMENT_DISPLAY] == strParam0)
			{
				int type = Arduino::_NetStr2Int(strParam1);
				float val = Arduino::_NetStr2Float(strParam2);

				if (1 == type)
					ardu->SegmentDisplayInt((int)val);
				else if (2 == type)
					ardu->SegmentDisplayFloat(val);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_LEDMATRIX_I] == strParam0)
			{
				Arduino::Pin pinClick = Arduino::_NetStr2Pin(strParam1);
				Arduino::Pin pinData = Arduino::_NetStr2Pin(strParam2);
				ardu->LEDMatrixInit(pinClick, pinData);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_LEDMATRIX_BRIGHTNESS] == strParam0)
			{
				int brightness = Arduino::_NetStr2Int(strParam1);
				ardu->LEDMatrixSetBrightness(brightness);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_LEDMATRIX_CLEARSCREEN] == strParam0)
			{
				ardu->LEDMatrixClearScreen();

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_LEDMATRIX_LIGHTAT] == strParam0)
			{
				int x = Arduino::_NetStr2Int(strParam1);
				int y = Arduino::_NetStr2Int(strParam2);
				int width = Arduino::_NetStr2Int(strParam3);
				bool onOff = Arduino::_NetStr2Bool(strParam4);

				ardu->LEDMatrixLightAt(x, y, width, onOff);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
		}
		else if ("makerclock" == cmdStr)
		{
			Arduino *ardu = PX2_STEAMEDU.GetArduinoByName(targetStr);
			if (!ardu)
			{
				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "failed";
				return;
			}

			std::string strParam0;
			std::string strParam1;
			std::string strParam2;
			std::string strParam3;
			std::string strParam4;
			std::string strParam5;
			std::string strParam6;
			std::string strParam7;
			std::string strParam8;
			std::string strParam9;
			std::string strParam10;

			StringTokenizer stk_(contentStr, ",");
			if (stk_.Count() > 0)
				strParam0 = stk_[0];
			if (stk_.Count() > 1)
				strParam1 = stk_[1];
			if (stk_.Count() > 2)
				strParam2 = stk_[2];
			if (stk_.Count() > 3)
				strParam3 = stk_[3];
			if (stk_.Count() > 4)
				strParam4 = stk_[4];
			if (stk_.Count() > 5)
				strParam5 = stk_[5];
			if (stk_.Count() > 6)
				strParam6 = stk_[6];
			if (stk_.Count() > 7)
				strParam7 = stk_[7];
			if (stk_.Count() > 8)
				strParam8 = stk_[8];
			if (stk_.Count() > 9)
				strParam9 = stk_[9];
			if (stk_.Count() > 10)
				strParam10 = stk_[10];

			if (Arduino::sOptTypeStr[Arduino::OT_MC_INTERNAL_LIGHT] == strParam0)
			{
				int mcPin = 3;
				std::string strPin0 = mMCPins[mcPin - 1].first;
				std::string strPin1 = mMCPins[mcPin - 1].second;
				Arduino::Pin pin0 = Arduino::_NetStr2Pin(strPin0);
				Arduino::Pin pin1 = Arduino::_NetStr2Pin(strPin1);

				bool val = Arduino::_NetStr2Bool(strParam1);

				ardu->PinMode(pin0, Arduino::PM_OUTPUT);
				ardu->DigitalWrite(pin0, val);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_MC_LIGHT] == strParam0)
			{
				int mcPin = Arduino::_NetStr2Int(strParam1);
				bool isHigh = Arduino::_NetStr2Bool(strParam2);

				std::string strPin0 = mMCPins[mcPin - 1].first;
				std::string strPin1 = mMCPins[mcPin - 1].second;
				Arduino::Pin pin0 = Arduino::_NetStr2Pin(strPin0);
				Arduino::Pin pin1 = Arduino::_NetStr2Pin(strPin1);

				ardu->PinMode(pin0, Arduino::PM_OUTPUT);
				ardu->DigitalWrite(pin0, isHigh);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_MC_SEGMENT] == strParam0)
			{
				int mcPin = Arduino::_NetStr2Int(strParam1);
				int val = Arduino::_NetStr2Int(strParam2);

				ardu->MCSegmentSet(mcPin, val);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_MC_MOTO] == strParam0)
			{
				int mcPin = Arduino::_NetStr2Int(strParam1);
				int val = Arduino::_NetStr2Int(strParam2);

				int pin = 9;
				if (1 == mcPin)
					pin = 9;
				else if (2 == mcPin)
					pin = 10;

				ardu->MCMoto(pin, val);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_MC_DISTTEST] == strParam0)
			{
				int mcPin = Arduino::_NetStr2Int(strParam1);

				ardu->MCTestDist(mcPin);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "suc";
			}
			else if (Arduino::sOptTypeStr[Arduino::OT_RETURN_DIST] == strParam0)
			{
				float dst = ardu->GetDist();

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << dst;
			}
		}
		else if (findBlockly)
		{
			std::string strParam0;
			std::string strParam1;
			std::string strParam2;

			StringTokenizer stk_(cmdStr, "_");
			if (stk_.Count() > 0)
				strParam0 = stk_[0];
			if (stk_.Count() > 1)
				strParam1 = stk_[1];
			if (stk_.Count() > 2)
				strParam2 = stk_[2];

			std::string valStrCMD = strParam1;
			std::string valStrFilename = strParam2;

			// blockly
			if ("uploadArduino" == valStrCMD)
			{
				std::string writeablePath = ResourceManager::GetWriteablePath();

				std::string dstFilename = valStrFilename;
				std::string dstDir = dstFilename + "/";
				std::string dstFile = dstFilename + ".ino";
				PX2_RM.CreateFloder(writeablePath, dstDir);

				std::string dstFloderFilename = writeablePath + dstDir + dstFile;

				FileIO::Save(dstFloderFilename, false,
					contentStr.length(), contentStr.c_str());

				PX2_STEAMEDU.OpenArduino(dstFloderFilename);

				response.add("Access-Control-Allow-Origin", "*");
				std::ostream& ostr = response.Send();
				ostr << "";
			}
		}
		else
		{
			assertion(false, "can not go to here0");
		}
	}
	else
	{
		assertion(false, "can not go to here1");
	}
}
//----------------------------------------------------------------------------