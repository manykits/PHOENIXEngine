// AppPlayJNI.cpp

#include "AppPlayJNI.hpp"
using namespace std;
using namespace appplay;

#define JAVAVM JNIHelper::GetJavaVM()

extern "C"
{

	//----------------------------------------------------------------------------
	static bool GetEnv_ (JNIEnv **env)
	{
		if (JAVAVM->GetEnv((void**)env, JNI_VERSION_1_4) != JNI_OK)
		{
			return false;
		}

		if (JAVAVM->AttachCurrentThread(env, 0) < 0)
		{
			return false;
		}

		return true;
	}
	//----------------------------------------------------------------------------
	static jclass GetClassID_(const char *className, JNIEnv *env)
	{
		JNIEnv *envTemp = env;
		jclass ret = 0;

		if (!envTemp)
		{
			if (!GetEnv_(&envTemp))
			{
				return 0;
			}
		}

		ret = envTemp->FindClass(className);
		if (!ret)
		{
			return 0;
		}

		return ret;
	}
	//----------------------------------------------------------------------------
	static bool GetStaticMethodInfo_(JNIMethodInfo &methodinfo, 
		const char *className, const char *methodName, const char *paramCode)
	{
		jmethodID methodID = 0;
		JNIEnv *envTemp = 0;

		if (!GetEnv_(&envTemp))
			return false;

		jclass classID = GetClassID_(className, envTemp);
		methodID = envTemp->GetStaticMethodID(classID, methodName, paramCode);
		if (!methodID)
		{
			return false;
		}

		methodinfo.classID = classID;
		methodinfo.env = envTemp;
		methodinfo.methodID = methodID;

		return true;
	}
	//----------------------------------------------------------------------------
	static bool GetMethodInfo_(JNIMethodInfo &methodinfo,
		const char *className, const char *methodName, const char *paramCode)
	{
		jmethodID methodID = 0;
		JNIEnv *envTemp = 0;

		if (!GetEnv_(&envTemp))
			return false;

		jclass classID = GetClassID_(className, envTemp);
		methodID = envTemp->GetMethodID(classID, methodName, paramCode);
		if (!methodID)
		{
			return false;
		}

		methodinfo.classID = classID;
		methodinfo.env = envTemp;
		methodinfo.methodID = methodID;

		return true;
	}
	//----------------------------------------------------------------------------
	static string JString2string_(jstring jstr)
	{
		JNIEnv *env = 0;

		jboolean isCopy;
		if (!GetEnv_(&env))
		{
			return "";
		}

		const char* chars = env->GetStringUTFChars(jstr, &isCopy);
		string ret(chars);
		if (isCopy)
		{
			env->ReleaseStringUTFChars(jstr, chars);
		}

		return ret;
	}
	//----------------------------------------------------------------------------

}

//----------------------------------------------------------------------------
JavaVM *JNIHelper::msJavaVM = 0;
//----------------------------------------------------------------------------
JavaVM *JNIHelper::GetJavaVM()
{
	return msJavaVM;
}
//----------------------------------------------------------------------------
void JNIHelper::SetJavaVM(JavaVM *javaVM)
{
	msJavaVM = javaVM;
}
//----------------------------------------------------------------------------
jclass JNIHelper::GetClassID(const char *className, JNIEnv *env)
{
	return GetClassID_(className, env);
}
//----------------------------------------------------------------------------
bool JNIHelper::GetStaticMethodInfo(JNIMethodInfo &methodinfo,
	const char *className, const char *methodName, const char *paramCode)
{
	return GetStaticMethodInfo_(methodinfo, className, methodName, paramCode);
}
//----------------------------------------------------------------------------
bool JNIHelper::GetMethodInfo(JNIMethodInfo &methodinfo, const char *className,
	const char *methodName, const char *paramCode)
{
	return GetMethodInfo_(methodinfo, className, methodName, paramCode);
}
//----------------------------------------------------------------------------
string JNIHelper::JString2string(jstring str)
{
	return JString2string_(str);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
extern "C"
{

	static std::string sAppPlayPackageName = "";
	const char *GetPackageNameJNI()
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t,
			"org/appplay/lib/AppPlayBaseActivity",
			"GetPackageName",
			"()Ljava/lang/String;"))
		{
			jstring jstr = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);

			sAppPlayPackageName = JNIHelper::JString2string(jstr);

			t.env->DeleteLocalRef(jstr);

			return sAppPlayPackageName.c_str();
		}

		return 0;
	}

	void SetKeyboardStateJNI (int open) 
	{
        if (open>0) 
		{
            OpenKeyboardJNI();
        }
		else 
		{
            CloseKeyboardJNI();
        }
    }

    void OpenKeyboardJNI() 
	{
        JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t, "org/appplay/lib/AppPlayGLView", "OpenIMEKeyboard", "()V"))
		{
            t.env->CallStaticVoidMethod(t.classID, t.methodID);
            t.env->DeleteLocalRef(t.classID);
        }
    }

    void CloseKeyboardJNI() 
	{
        JNIMethodInfo t;

        if (JNIHelper::GetStaticMethodInfo(t, "org/appplay/lib/AppPlayGLView", "CloseIMEKeyboard", "()V")) 
		{
            t.env->CallStaticVoidMethod(t.classID, t.methodID);
            t.env->DeleteLocalRef(t.classID);
        }
    }
	
	void SetScreenOrientate(int ori)
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t,
			"org/appplay/lib/AppPlayBaseActivity",
			"SetScreenOrientation",
			"(I)V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID, ori);
		}
	}
	
	void PlatformSDKLoginJNI()
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t,
			"org/appplay/lib/AppPlayBaseActivity",
			"PlatformSDKLogin",
			"()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void PlatformSDKLogoutJNI()
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t,
			"org/appplay/lib/AppPlayBaseActivity",
			"PlatformSDKLogout",
			"()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void PlatformSDKSynPayJNI (
		const char *productID, const char *productName,
		float productPrice, float productOriginPrice, int count,
		const char *payDescription)
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t,
			"org/appplay/lib/AppPlayBaseActivity",
			"PlatformSDKSynPay",
			"(Ljava/lang/String;Ljava/lang/String;FFILjava/lang/String;)V"))
		{
			jstring jProductID = t.env->NewStringUTF(productID);
			jstring jProductName = t.env->NewStringUTF(productName);
			jstring jPayDescription = t.env->NewStringUTF(payDescription);
			t.env->CallStaticVoidMethod(t.classID, t.methodID, jProductID, jProductName,
				productPrice, productOriginPrice, count, jPayDescription);
			t.env->DeleteLocalRef(t.classID);
			t.env->DeleteLocalRef(jProductID);
			t.env->DeleteLocalRef(jProductName);
			t.env->DeleteLocalRef(jPayDescription);
		}
	}

	void PlatformSDKASynPayJNI (
		const char *productID, const char *productName,
		float productPrice, float productOriginPrice, int count,
		const char *payDescription)
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t,
			"org/appplay/lib/AppPlayBaseActivity",
			"PlatformSDKASynPay",
			"(Ljava/lang/String;Ljava/lang/String;FFILjava/lang/String;)V"))
		{
			jstring jProductID = t.env->NewStringUTF(productID);
			jstring jProductName = t.env->NewStringUTF(productName);
			jstring jPayDescription = t.env->NewStringUTF(payDescription);
			t.env->CallStaticVoidMethod(t.classID, t.methodID, jProductID, jProductName,
				productPrice, productOriginPrice, count, jPayDescription);
			t.env->DeleteLocalRef(t.classID);
			t.env->DeleteLocalRef(jProductID);
			t.env->DeleteLocalRef(jProductName);
			t.env->DeleteLocalRef(jPayDescription);
		}
	}

	void BluetoothSetWriteServiceID(const char *strID)
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t, "org/appplay/lib/AppPlayBaseActivity", "BluetoothSetWriteServiceID", "(Ljava/lang/String;)V"))
		{
			jstring jAddr = t.env->NewStringUTF(strID);
			t.env->CallStaticVoidMethod(t.classID, t.methodID, jAddr);
			t.env->DeleteLocalRef(jAddr);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void BluetoothSetWriteCharaID(const char *strID)
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t, "org/appplay/lib/AppPlayBaseActivity", "BluetoothSetWriteCharaID", "(Ljava/lang/String;)V"))
		{
			jstring jAddr = t.env->NewStringUTF(strID);
			t.env->CallStaticVoidMethod(t.classID, t.methodID, jAddr);
			t.env->DeleteLocalRef(jAddr);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void BluetoothSetReadServiceID(const char *strID)
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t, "org/appplay/lib/AppPlayBaseActivity", "BluetoothSetReadServiceID", "(Ljava/lang/String;)V"))
		{
			jstring jAddr = t.env->NewStringUTF(strID);
			t.env->CallStaticVoidMethod(t.classID, t.methodID, jAddr);
			t.env->DeleteLocalRef(jAddr);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void BluetoothSetReadCharaID(const char *strID)
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t, "org/appplay/lib/AppPlayBaseActivity", "BluetoothSetReadCharaID", "(Ljava/lang/String;)V"))
		{
			jstring jAddr = t.env->NewStringUTF(strID);
			t.env->CallStaticVoidMethod(t.classID, t.methodID, jAddr);
			t.env->DeleteLocalRef(jAddr);
			t.env->DeleteLocalRef(t.classID);
		}
	}
	
	void BluetoothSetBle(bool isBle)
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t, "org/appplay/lib/AppPlayBaseActivity", "BluetoothSetBle", "(Z)V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID, isBle);
		}
	}

	void BluetoothSetDataProtocolHex(bool isBle)
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t, "org/appplay/lib/AppPlayBaseActivity", "BluetoothSetDataProtocolHex", "(Z)V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID, isBle);
		}
	}

	bool BluetoothIsAvailable()
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t, "org/appplay/lib/AppPlayBaseActivity", "BluetoothIsAvailable", "()Z"))
		{
			jboolean ret = (jboolean)t.env->CallStaticBooleanMethod(t.classID, t.methodID);
			bool bRet = ret;
			t.env->DeleteLocalRef(t.classID);

			return bRet;
		}

		return false;
	}

	bool BluetoothIsConnected()
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t, "org/appplay/lib/AppPlayBaseActivity", "BluetoothIsConnected", "()Z"))
		{
			jboolean ret = (jboolean)t.env->CallStaticBooleanMethod(t.classID, t.methodID);
			bool bRet = ret;
			t.env->DeleteLocalRef(t.classID);

			return bRet;
		}

		return false;
	}

	static std::string pairedDevicesStr = "";
	std::string BluetoothGetPairedDevices()
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t,
			"org/appplay/lib/AppPlayBaseActivity",
			"BluetoothGetPairedDevices",
			"()Ljava/lang/String;"))
		{
			jstring jstr = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);

			pairedDevicesStr = JNIHelper::JString2string(jstr);

			t.env->DeleteLocalRef(jstr);

			return pairedDevicesStr;
		}

		return "";
	}

	void BluetoothDisConnect()
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t, "org/appplay/lib/AppPlayBaseActivity", "BluetoothDisconnect", "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void BluetoothConnect(const char *addr)
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t, "org/appplay/lib/AppPlayBaseActivity", "BluetoothConnect", "(Ljava/lang/String;)V"))
		{
			jstring jAddr = t.env->NewStringUTF(addr);
			t.env->CallStaticVoidMethod(t.classID, t.methodID, jAddr);
			t.env->DeleteLocalRef(jAddr);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void BluetoothSend(const char *buf, int length, bool withRead)
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t, "org/appplay/lib/AppPlayBaseActivity", "BluetoothSend", "([BZ)V"))
		{
			jbyteArray bytes = t.env->NewByteArray(length);
			t.env->SetByteArrayRegion(bytes, 0, length, (jbyte*)buf);

			t.env->CallStaticVoidMethod(t.classID, t.methodID, bytes, withRead);
			
			//t.env->ReleaseByteArrayElements(bytes, 0, 0);
			t.env->DeleteLocalRef(bytes);

			t.env->DeleteLocalRef(t.classID);
		}
	}

	void BluetoothDoDiscovery()
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t, "org/appplay/lib/AppPlayBaseActivity", "BluetoothDoDiscovery", "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void BluetoothCancelDiscovery()
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t, "org/appplay/lib/AppPlayBaseActivity", "BluetoothCancelDiscovery", "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
	}
	
	void WifiConnect(const char *addr, const char *password)
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t, "org/appplay/lib/AppPlayBaseActivity", "WifiConnect", "(Ljava/lang/String;Ljava/lang/String;)V"))
		{
			jstring jAddr = t.env->NewStringUTF(addr);
			jstring jpwd = t.env->NewStringUTF(password);
			
			t.env->CallStaticVoidMethod(t.classID, t.methodID, jAddr, jpwd);
			
			t.env->DeleteLocalRef(jAddr);
			t.env->DeleteLocalRef(jpwd);
			
			t.env->DeleteLocalRef(t.classID);
		}
	}
	
	void WifiDisConnect()
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t, "org/appplay/lib/AppPlayBaseActivity", "WifiDisconnect", "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
	}
	
	void WifiDoDiscovery()
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t, "org/appplay/lib/AppPlayBaseActivity", "WifiDoDiscovery", "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
	}
	
	void WifiCancelDiscovery()
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t, "org/appplay/lib/AppPlayBaseActivity", "WifiCancelDiscovery", "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
	}
	
	void VoiceStartVoiceListening()
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t, "org/appplay/lib/AppPlayBaseActivity", "StartVoiceListening", "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
	}
	
	void VoiceEndVoiceListening()
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t, "org/appplay/lib/AppPlayBaseActivity", "EndVoiceListening", "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void VoiceEnableAutoSpeak(bool isEnable)
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t, "org/appplay/lib/AppPlayBaseActivity", "EnableAutoSpeak", "(Z)V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID, isEnable);
		}
	}

	void VoiceEnableAutoSpeakTTS(bool isEnable)
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t, "org/appplay/lib/AppPlayBaseActivity", "EnableAutoSpeakTTS", "(Z)V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID, isEnable);
		}
	}

	void VoiceSpeak(const char *text)
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t, "org/appplay/lib/AppPlayBaseActivity", "VoiceSpeak", "(Ljava/lang/String;)V"))
		{
			jstring jBuf = t.env->NewStringUTF(text);
			t.env->CallStaticVoidMethod(t.classID, t.methodID, jBuf);
			t.env->DeleteLocalRef(jBuf);
			t.env->DeleteLocalRef(t.classID);
		}
	}
	
	void CameraOpenStream(int type)
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t,
			"org/appplay/lib/AppPlayBaseActivity",
			"OpenCameraStream",
			"(I)V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID, type);
		}
	}
	
	void CameraCloseStream()
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t, "org/appplay/lib/AppPlayBaseActivity", "CloseCameraStream", "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void PlatformSDKStartAccelerator()
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t, "org/appplay/lib/AppPlayBaseActivity", "StartAccelerator", "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void PlatformSDKRegistAccelerator()
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t, "org/appplay/lib/AppPlayBaseActivity", "RegistAccelerator", "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void PlatformSDKUnRegistAccelerator()
	{
		JNIMethodInfo t;

		if (JNIHelper::GetStaticMethodInfo(t, "org/appplay/lib/AppPlayBaseActivity", "UnRegistAccelerator", "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
	}
}
//----------------------------------------------------------------------------
