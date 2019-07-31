#include "org_appplay_lib_AppPlayWebViewNatives.h"
#include "AppPlay.hpp"
#include "AppPlayJNI.hpp"
#include "PX2UIWebFrame.hpp"
#include "PX2Log.hpp"
using namespace appplay;

static std::string s_cszWebViewImageData;
static std::string s_cszWebViewHtmSource;

#ifdef __cplusplus
extern "C"
{
#endif

JNIEXPORT jboolean JNICALL Java_org_appplay_lib_AppPlayWebViewNatives_shouldStartLoading
  (JNIEnv *env, jclass, jint index, jstring jurl)
  {
		const char* charUrl = env->GetStringUTFChars(jurl, NULL);
		std::string url = charUrl;
		env->ReleaseStringUTFChars(jurl, charUrl);
		return PX2::UIWebFrame::ShouldStartLoading(index, url);
  }

JNIEXPORT void JNICALL Java_org_appplay_lib_AppPlayWebViewNatives_didFinishLoading
  (JNIEnv *env, jclass, jint index, jstring jurl)
  {
		const char* charUrl = env->GetStringUTFChars(jurl, NULL);
		std::string url = charUrl;
		env->ReleaseStringUTFChars(jurl, charUrl);
		PX2::UIWebFrame::DidFinishLoading(index, url);  
  }

JNIEXPORT void JNICALL Java_org_appplay_lib_AppPlayWebViewNatives_didFailLoading
  (JNIEnv *env, jclass, jint index, jstring jurl)
  {
		const char* charUrl = env->GetStringUTFChars(jurl, NULL);
        std::string url = charUrl;
        env->ReleaseStringUTFChars(jurl, charUrl);
		PX2::UIWebFrame::DidFailLoading(index, url);
  }

JNIEXPORT void JNICALL Java_org_appplay_lib_AppPlayWebViewNatives_onJsCallback
  (JNIEnv *env, jclass, jint index, jstring jmessage)
  {
	  	const char* charMessage = env->GetStringUTFChars(jmessage, NULL);
        std::string message = charMessage;
        env->ReleaseStringUTFChars(jmessage, charMessage);
		PX2::UIWebFrame::DoJsCallback(index, message);
  }

JNIEXPORT void JNICALL Java_org_appplay_lib_AppPlayWebViewNatives_onSetByteArrayBuffer
  (JNIEnv *env, jclass, jint index, jbyteArray buf, jint len)
  { 
	  	s_cszWebViewImageData.resize(len);
		env->GetByteArrayRegion(buf, 0, len, (jbyte *)&s_cszWebViewImageData[0]);
		
		PX2::UIWebFrame::DoSetImageData(index, s_cszWebViewImageData);
  }

JNIEXPORT void JNICALL Java_org_appplay_lib_AppPlayWebViewNatives_didLoadHtmlSource
  (JNIEnv *env, jclass, jstring jurl)
  {
	  	const char* charHtml = env->GetStringUTFChars(jurl, NULL);
		s_cszWebViewHtmSource = charHtml;
		env->ReleaseStringUTFChars(jurl, charHtml);
  }

JNIEXPORT void JNICALL Java_org_appplay_lib_AppPlayWebViewNatives_pause
  (JNIEnv *, jclass)
  {
	 //CrossApp::CAApplication::getApplication()->pause();
  }

JNIEXPORT void JNICALL Java_org_appplay_lib_AppPlayWebViewNatives_resume
  (JNIEnv *, jclass)
  {
	 //CrossApp::CAApplication::getApplication()->resume();
  }


#ifdef __cplusplus
}
#endif