// PX2UIWebFrame.cpp

#include "PX2UIWebFrame.hpp"
#include "PX2Renderer.hpp"
#include "PX2Log.hpp"
#include "PX2EngineUICanvas.hpp"
#include "PX2Mutex.hpp"
#include "PX2ScopedCS.hpp"
#include "PX2GraphicsRoot.hpp"
using namespace PX2;

#if defined (__ANDROID__)

#include "AppPlayJNI.hpp"
using namespace appplay;

static std::string s_strWebViewHtmSource;

#define CLASS_NAME "org/appplay/lib/AppPlayWebViewNatives"

int createWebViewJNI() {
	JNIMethodInfo t;
	if (JNIHelper::GetStaticMethodInfo(t, CLASS_NAME, "createWebView", "()I")) {
		jint viewTag = t.env->CallStaticIntMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		return viewTag;
	}
	return -1;
}

void removeWebViewJNI(const int index) {
	JNIMethodInfo t;
	if (JNIHelper::GetStaticMethodInfo(t, CLASS_NAME, "removeWebView", "(I)V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID, index);
		t.env->DeleteLocalRef(t.classID);
	}
}

void getWebViewImageJNI(const int index) {
	JNIMethodInfo t;

	if (JNIHelper::GetStaticMethodInfo(t, CLASS_NAME, "getWebViewImage", "(I)V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID, index);
		t.env->DeleteLocalRef(t.classID);
	}
}

void setWebViewRectJNI(const int index, const int left, const int top, const int width, const int height) {
	JNIMethodInfo t;
	if (JNIHelper::GetStaticMethodInfo(t, CLASS_NAME, "setWebViewRect", "(IIIII)V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID, index, left, top, width, height);
		t.env->DeleteLocalRef(t.classID);
	}
}

void setJavascriptInterfaceSchemeJNI(const int index, const std::string &scheme) {
	JNIMethodInfo t;
	if (JNIHelper::GetStaticMethodInfo(t, CLASS_NAME, "setJavascriptInterfaceScheme", "(ILjava/lang/String;)V")) {
		jstring jScheme = t.env->NewStringUTF(scheme.c_str());
		t.env->CallStaticVoidMethod(t.classID, t.methodID, index, jScheme);

		t.env->DeleteLocalRef(jScheme);
		t.env->DeleteLocalRef(t.classID);
	}
}

void loadHTMLStringJNI(const int index, const std::string &string, const std::string &baseURL) {
	JNIMethodInfo t;
	if (JNIHelper::GetStaticMethodInfo(t, CLASS_NAME, "loadHTMLString", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
		jstring jString = t.env->NewStringUTF(string.c_str());
		jstring jBaseURL = t.env->NewStringUTF(baseURL.c_str());
		t.env->CallStaticVoidMethod(t.classID, t.methodID, index, jString, jBaseURL);

		t.env->DeleteLocalRef(jString);
		t.env->DeleteLocalRef(jBaseURL);
		t.env->DeleteLocalRef(t.classID);
	}
}

void loadUrlJNI(const int index, const std::string &url) {
	JNIMethodInfo t;
	if (JNIHelper::GetStaticMethodInfo(t, CLASS_NAME, "loadUrl", "(ILjava/lang/String;)V")) {
		jstring jUrl = t.env->NewStringUTF(url.c_str());
		t.env->CallStaticVoidMethod(t.classID, t.methodID, index, jUrl);

		t.env->DeleteLocalRef(jUrl);
		t.env->DeleteLocalRef(t.classID);
	}
}

void loadFileJNI(const int index, const std::string &filePath) {
	JNIMethodInfo t;
	if (JNIHelper::GetStaticMethodInfo(t, CLASS_NAME, "loadFile", "(ILjava/lang/String;)V")) {
		jstring jFilePath = t.env->NewStringUTF(filePath.c_str());
		t.env->CallStaticVoidMethod(t.classID, t.methodID, index, jFilePath);

		t.env->DeleteLocalRef(jFilePath);
		t.env->DeleteLocalRef(t.classID);
	}
}

void stopLoadingJNI(const int index) {
	JNIMethodInfo t;
	if (JNIHelper::GetStaticMethodInfo(t, CLASS_NAME, "stopLoading", "(I)V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID, index);
		t.env->DeleteLocalRef(t.classID);
	}
}

void reloadJNI(const int index) {
	JNIMethodInfo t;
	if (JNIHelper::GetStaticMethodInfo(t, CLASS_NAME, "reload", "(I)V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID, index);
		t.env->DeleteLocalRef(t.classID);
	}
}

bool canGoBackJNI(const int index) {
	JNIMethodInfo t;
	if (JNIHelper::GetStaticMethodInfo(t, CLASS_NAME, "canGoBack", "(I)Z")) {
		jboolean ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID, index);
		t.env->DeleteLocalRef(t.classID);
		return ret;
	}
	return false;
}

bool canGoForwardJNI(const int index) {
	JNIMethodInfo t;
	if (JNIHelper::GetStaticMethodInfo(t, CLASS_NAME, "canGoForward", "(I)Z")) {
		jboolean ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID, index);
		t.env->DeleteLocalRef(t.classID);
		return ret;
	}
	return false;
}

void goBackJNI(const int index) {
	JNIMethodInfo t;
	if (JNIHelper::GetStaticMethodInfo(t, CLASS_NAME, "goBack", "(I)V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID, index);
		t.env->DeleteLocalRef(t.classID);
	}
}

void goForwardJNI(const int index) {
	JNIMethodInfo t;
	if (JNIHelper::GetStaticMethodInfo(t, CLASS_NAME, "goForward", "(I)V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID, index);
		t.env->DeleteLocalRef(t.classID);
	}
}

std::string evaluateJSJNI(const int index, const std::string &js) {
	JNIMethodInfo t;
	if (JNIHelper::GetStaticMethodInfo(t, CLASS_NAME, "evaluateJS", "(ILjava/lang/String;)V")) {

		s_strWebViewHtmSource.clear();
		jstring jjs = t.env->NewStringUTF(js.c_str());
		t.env->CallStaticVoidMethod(t.classID, t.methodID, index, jjs);

		t.env->DeleteLocalRef(jjs);
		t.env->DeleteLocalRef(t.classID);
	}
	return s_strWebViewHtmSource;
}

void setScalesPageToFitJNI(const int index, const bool scalesPageToFit) {
	JNIMethodInfo t;
	if (JNIHelper::GetStaticMethodInfo(t, CLASS_NAME, "setScalesPageToFit", "(IZ)V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID, index, scalesPageToFit);
		t.env->DeleteLocalRef(t.classID);
	}
}

void setWebViewVisibleJNI(const int index, const bool visible) {
	JNIMethodInfo t;
	if (JNIHelper::GetStaticMethodInfo(t, CLASS_NAME, "setVisible", "(IZ)V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID, index, visible);
		t.env->DeleteLocalRef(t.classID);
	}
}
#elif defined PX2_USE_AWESOMIUM
#include <Awesomium/WebURL.h>
#include <Awesomium/WebString.h>
#include <Awesomium/STLHelpers.h>
#include <Awesomium/WebKeyboardCodes.h>
#include <Awesomium/WebCore.h>
#include <Awesomium/WebView.h>
#include <Awesomium/BitmapSurface.h>
using namespace Awesomium;
#endif

std::string getUrlStringByFileName(const std::string &fileName) {
	//const std::string basePath("file:///android_asset/");
	//std::string fullPath = FileUtils::getInstance()->fullPathForFilename(fileName);
	//const std::string assetsPath("assets/");

	//std::string urlString;
	//if (fullPath.find(assetsPath) != std::string::npos) {
	//	urlString = fullPath.replace(fullPath.find_first_of(assetsPath), assetsPath.length(), basePath);
	//}
	//else {
	//	urlString = fullPath;
	//}

	//return urlString;

	return "";
}
//----------------------------------------------------------------------------
void _OnDrawCallback (Renderer *renderer, Renderable *renderable)
{
	PX2_UNUSED(renderer);

	UIWebFrame *webFrame = renderable->GetUserData<UIWebFrame*>("WebFrame");
	if (webFrame)
	{
		if (!webFrame->IsShowNativeView())
		{
			webFrame->ShowNativeView(false);
		}
	}
}

#if defined PX2_USE_AWESOMIUM
class WebFrameHandler  : public Object, public Awesomium::JSMethodHandler, public Awesomium::WebViewListener::Load
{
public:
	WebFrameHandler()
	{
		mWebFrame = 0;
	}
	virtual ~WebFrameHandler()
	{
	}

	void SetWebFrame(UIWebFrame *webFrame)
	{
		mWebFrame = webFrame;
	}
	UIWebFrame *GetWebFrame()
	{
		return mWebFrame;
	}

	void OnMethodCall(Awesomium::WebView* caller,
		unsigned int remote_object_id,
		const Awesomium::WebString& method_name,
		const Awesomium::JSArray& args)
	{
		PX2_UNUSED(caller);
		PX2_UNUSED(remote_object_id);
		PX2_UNUSED(method_name);
		PX2_UNUSED(args);
	}
	Awesomium::JSValue OnMethodCallWithReturnValue(
		Awesomium::WebView* caller, unsigned int remote_object_id,
		const Awesomium::WebString& method_name,
		const Awesomium::JSArray& args)
	{
		PX2_UNUSED(caller);
		PX2_UNUSED(remote_object_id);
		PX2_UNUSED(method_name);
		PX2_UNUSED(args);

		return Awesomium::JSValue::Undefined();
	}
	void OnBeginLoadingFrame(Awesomium::WebView* caller,
		int64 frame_id, bool is_main_frame, const Awesomium::WebURL& url,
		bool is_error_page)
	{
		PX2_UNUSED(caller);
		PX2_UNUSED(frame_id);
		PX2_UNUSED(is_main_frame);
		PX2_UNUSED(url);
		PX2_UNUSED(is_error_page);

		if (mWebFrame)
		{
			mWebFrame->OnBeginLoading("");
		}
	}
	void OnFailLoadingFrame(Awesomium::WebView* caller,
		int64 frame_id, bool is_main_frame, const Awesomium::WebURL& url,
		int error_code, const Awesomium::WebString& error_desc)
	{
		PX2_UNUSED(caller);
		PX2_UNUSED(frame_id);
		PX2_UNUSED(is_main_frame);
		PX2_UNUSED(url);
		PX2_UNUSED(error_code);
		PX2_UNUSED(error_desc);

		if (mWebFrame)
		{
			mWebFrame->OnFailLoading("");
		}		
	}
	void  OnFinishLoadingFrame(Awesomium::WebView* caller,
		int64 frame_id, bool is_main_frame, const Awesomium::WebURL& url)
	{
		PX2_UNUSED(caller);
		PX2_UNUSED(frame_id);
		PX2_UNUSED(is_main_frame);
		PX2_UNUSED(url);

		if (mWebFrame)
		{
			mWebFrame->OnFinishLoading("");
		}
	}

	void OnDocumentReady(Awesomium::WebView* caller,
		const Awesomium::WebURL& url)
	{
		PX2_UNUSED(caller);
		PX2_UNUSED(url);

		if (mWebFrame)
		{
			mWebFrame->OnDocumentReady("");
		}
	}

protected:
	UIWebFrame *mWebFrame;
};
#endif

#if defined PX2_USE_AWESOMIUM
Awesomium::WebCore *UIWebFrame::msWebCore = 0;
#endif
//----------------------------------------------------------------------------
int UIWebFrame::mNumberOfViews = 0;
static std::map<int, UIWebFrame*> sWebViews;
//----------------------------------------------------------------------------
PX2_IMPLEMENT_RTTI(PX2, UIFrame, UIWebFrame);
PX2_IMPLEMENT_STREAM(UIWebFrame);
PX2_IMPLEMENT_FACTORY(UIWebFrame);
//----------------------------------------------------------------------------
UIWebFrame::UIWebFrame() :
mViewTag(-1),
mIsTexNeedUpdate(true),
mIsUpdateToTex(false),
mIsShowNativeView(false),
mIsAcceptKeyboardInput(true)
{
	mImagePicBox = new0 UIFPicBox();
	AttachChild(mImagePicBox);
	mImagePicBox->LocalTransform.SetTranslateY(-1.0f);
	mImagePicBox->SetAnchorHor(0.0f, 1.0f);
	mImagePicBox->SetAnchorVer(0.0, 1.0f);
	mImagePicBox->GetUIPicBox()->SetUserData("WebFrame", this);
	Material *mtl = mImagePicBox->GetUIPicBox()->GetMaterialInstance()->GetMaterial();
	mtl->GetPixelShader(0, 0)->SetFilter(0, Shader::SF_LINEAR);
#if defined (__ANDROID__)
	mImagePicBox->GetUIPicBox()->SetOnDrawCallback(_OnDrawCallback);
#endif

	_Init();

	ShowNativeView(true);
	SetScalesPageToFit(true);

	SetWidget(true);

	ComeInEventWorld();
}
//----------------------------------------------------------------------------
void UIWebFrame::_Init()
{
	mNumberOfViews++;

#if defined (__ANDROID__)
	mViewTag = createWebViewJNI();
	sWebViews[mViewTag] = this;
#elif defined PX2_USE_AWESOMIUM
	if (!msWebCore)
	{
		WebConfig config;
		msWebCore = WebCore::Initialize(config);

		if (!msWebCore)
		{
			assertion(false, "web core initlize failed!");
			PX2_LOG_ERROR("web core initlize failed!");
		}
	}

	WebFrameHandler *handler = new WebFrameHandler();
	handler->SetWebFrame(this);
	mObject = handler;

	mWebView = msWebCore->CreateWebView(512, 512);
	mWebView->set_js_method_handler(handler);
	mWebView->set_load_listener(handler);

#if defined WIN32 || defined _WIN32

	RenderWindow *rw = PX2_GR.GetMainWindow();
	void *hwnd = rw->GetWindowHandle();
	mWebView->set_parent_window((NativeWindow)hwnd);

#endif

	mBitmapSurface = 0;
#endif
}
//----------------------------------------------------------------------------
UIWebFrame::~UIWebFrame()
{
	mNumberOfViews--;

#if defined (__ANDROID__)
	removeWebViewJNI(mViewTag);
	sWebViews.erase(mViewTag);
#elif defined PX2_USE_AWESOMIUM
	mObject = 0;

	mWebView->Destroy();

	//if (msWebCore && mNumberOfViews == 0)
	//{
	//	msWebCore->Shutdown();
	//	msWebCore = 0;
	//}

	mWebView = 0;
	mBitmapSurface = 0;
#endif
}
//----------------------------------------------------------------------------
void UIWebFrame::ShowNativeView(bool show)
{
#if defined (__ANDROID__)
	setWebViewVisibleJNI(mViewTag, show);
#endif

	mIsShowNativeView = show;
}
//----------------------------------------------------------------------------
void UIWebFrame::SetUpdateToTex(bool updateToTex)
{
	mIsUpdateToTex = updateToTex;
}
//----------------------------------------------------------------------------
bool UIWebFrame::IsUpdateToTex() const
{
	return mIsUpdateToTex;
}
//----------------------------------------------------------------------------
void UIWebFrame::SetAcceptKeyboardInput(bool acpt)
{
	mIsAcceptKeyboardInput = acpt;
}
//----------------------------------------------------------------------------
bool UIWebFrame::IsAcceptKeyboardInput() const
{
	return mIsAcceptKeyboardInput;
}
//----------------------------------------------------------------------------
bool UIWebFrame::IsShowNativeView() const
{
	return mIsShowNativeView;
}
//----------------------------------------------------------------------------
void UIWebFrame::LoadHTMLString(const std::string &string, 
	const std::string &baseURL)
{
	PX2_UNUSED(string);
	PX2_UNUSED(baseURL);

#if defined (__ANDROID__)
	loadHTMLStringJNI(mViewTag, string, baseURL);
#endif
}
//----------------------------------------------------------------------------
void UIWebFrame::LoadURL(const std::string &url)
{
#if defined (__ANDROID__)
	loadUrlJNI(mViewTag, url);
#elif defined PX2_USE_AWESOMIUM
	mWebView->LoadURL(WebURL(WSLit(url.c_str())));
	mIsTexNeedUpdate = true;
#else
	PX2_UNUSED(url);
#endif

	mURL = url;
}
//----------------------------------------------------------------------------
void UIWebFrame::LoadFile(const std::string &fileName)
{
	PX2_UNUSED(fileName);

#if defined (__ANDROID__)
	std::string fullPath = getUrlStringByFileName(fileName);
	loadFileJNI(mViewTag, fullPath);
#else

#if defined PX2_USE_AWESOMIUM
	mWebView->LoadURL(WebURL(WSLit(fileName.c_str())));
#endif

	mIsTexNeedUpdate = true;
#endif
}
//----------------------------------------------------------------------------
void UIWebFrame::StopLoading()
{
#if defined (__ANDROID__)
	stopLoadingJNI(mViewTag);
#endif
}
//----------------------------------------------------------------------------
void UIWebFrame::Reload()
{
#if defined (__ANDROID__)
	reloadJNI(mViewTag);
#endif
}
//----------------------------------------------------------------------------
bool UIWebFrame::CanGoBack()
{
#if defined (__ANDROID__)
	return canGoBackJNI(mViewTag);      
#endif

	return false;
}
//----------------------------------------------------------------------------
bool UIWebFrame::CanGoForward()
{
#if defined (__ANDROID__)
	return canGoForwardJNI(mViewTag);
#endif

	return false;
}
//----------------------------------------------------------------------------
void UIWebFrame::GoBack()
{
#if defined (__ANDROID__)
	goBackJNI(mViewTag);
#endif
}
//----------------------------------------------------------------------------
void UIWebFrame::GoForward()
{
#if defined (__ANDROID__)
	goForwardJNI(mViewTag);
#endif
}
//----------------------------------------------------------------------------
void UIWebFrame::SelectAll()
{
#if defined PX2_USE_AWESOMIUM
	mWebView->SelectAll();
#endif
}
//----------------------------------------------------------------------------
void UIWebFrame::Undo()
{
#if defined PX2_USE_AWESOMIUM
	mWebView->Undo();
#endif
}
//----------------------------------------------------------------------------
void UIWebFrame::ReDo()
{
#if defined PX2_USE_AWESOMIUM
	mWebView->Redo();
#endif
}
//----------------------------------------------------------------------------
void UIWebFrame::Cut()
{
#if defined PX2_USE_AWESOMIUM
	mWebView->Cut();
#endif
}
//----------------------------------------------------------------------------
void UIWebFrame::Copy()
{
#if defined PX2_USE_AWESOMIUM
	mWebView->Copy();
#endif
}
//----------------------------------------------------------------------------
void UIWebFrame::Paste()
{
#if defined PX2_USE_AWESOMIUM
	mWebView->Paste();
#endif
}
//----------------------------------------------------------------------------
void UIWebFrame::CreateJSGlobalVaiable(const std::string &name, 
	const std::string &val)
{
#if defined (__ANDROID__)

#else

#if defined PX2_USE_AWESOMIUM
	std::string strEmpty;
	JSValue sjVal = mWebView->CreateGlobalJavascriptObject(
		WebString::CreateFromUTF8(name.c_str(), 0));
	sjVal = WebString::CreateFromUTF8(val.c_str(), 0);
#endif // _DEBUG

#endif
}
//----------------------------------------------------------------------------
std::string UIWebFrame::EvaluateJS(const std::string &js)
{
#if defined (__ANDROID__)
	return evaluateJSJNI(mViewTag, js);
#elif defined PX2_USE_AWESOMIUM
	std::string strEmpty;
	mWebView->ExecuteJavascript(WebString::CreateFromUTF8(js.c_str(), js.length()),
		WebString::CreateFromUTF8(strEmpty.c_str(), 0));

	return "";
#else
	return "";
#endif
}
//----------------------------------------------------------------------------
void UIWebFrame::SetScalesPageToFit(const bool scalesPageToFit)
{
	PX2_UNUSED(scalesPageToFit);

#if defined (__ANDROID__)
	setScalesPageToFitJNI(mViewTag, scalesPageToFit);
#endif
}
//----------------------------------------------------------------------------
void UIWebFrame::OnEvent(Event *ent)
{
	UIFrame::OnEvent(ent);

#if defined PX2_USE_AWESOMIUM
	if (mIsAcceptKeyboardInput)
	{
		//if (InputEventSpace::IsEqual(ent, InputEventSpace::KeyPressed))
		//{
		//	InputEventData ied = ent->GetData<InputEventData>();
		//	OnKeyCodePressed(ied.KCode);
		//}
		//else if (InputEventSpace::IsEqual(ent, InputEventSpace::KeyReleased))
		//{
		//	InputEventData ied = ent->GetData<InputEventData>();
		//	OnKeyCodeReleased(ied.KCode);
		//}
		//else if (InputEventSpace::IsEqual(ent, InputEventSpace::KeyChar))
		//{
		//	InputEventData ied = ent->GetData<InputEventData>();
		//	OnKeyCodeChar(ied.KChar);
		//}

		if (InputEventSpace::IsEqual(ent, InputEventSpace::MsgRawWin32))
		{
			InputEventData data = ent->GetData<InputEventData>();
			WebKeyboardEvent ent(data.Msg, data.WParam, data.LParam);
			mWebView->InjectKeyboardEvent(ent);
		}
	}
#endif
}
//----------------------------------------------------------------------------
void UIWebFrame::OnSizeChanged()
{
	UIFrame::OnSizeChanged();

#if defined PX2_USE_AWESOMIUM

	int iWidth = (int)mSize.Width;
	int iHeight = (int)mSize.Height;

	if (iWidth > 0 && iHeight > 0)
	{
		mWebView->Resize(iWidth, iHeight);
	}

#endif
}
//----------------------------------------------------------------------------
void UIWebFrame::OnPivotChanged()
{
	UIFrame::OnPivotChanged();
}
//----------------------------------------------------------------------------
void UIWebFrame::UpdateWorldData(double applicationTime, double elapsedTime)
{
	UIFrame::UpdateWorldData(applicationTime, elapsedTime);

	Rectf canvasScreenRect;
	int screenLeft = 0;
	int top = 0;
	int screenWidth = 0;
	int screenHeight = 0;
	EngineUICanvas *engineCanvas = EngineUICanvas::GetSingletonPtr();
	if (engineCanvas)
	{
		canvasScreenRect = EngineUICanvas::GetSingleton().GetScreenRect();
		Rectf screenRect = GetScreenRect();
		top = canvasScreenRect.Height() - screenRect.Top;

		screenLeft = screenRect.Left;
		screenWidth = (int)screenRect.Width();
		screenHeight = (int)screenRect.Height();
	}

	bool isNeedReGenTex = false;
	if (mTex2D)
	{
		int texWidth = mTex2D->GetWidth();
		int texHeight = mTex2D->GetHeight();
		if (screenWidth != texWidth || screenHeight != texHeight)
		{
			if (screenWidth != 0 && screenHeight != 0)
			{
				isNeedReGenTex = true;
			}
		}
	}
	else
	{
		if (screenWidth != 0 && screenHeight != 0)
		{
			isNeedReGenTex = true;
		}
	}

	if (isNeedReGenTex && screenWidth>0 && screenHeight>0)
	{
		mWebViewImageData.clear();
		int width = screenWidth;
		int height = screenHeight;
		mTex2D = new0 Texture2D(Texture::TF_A8R8G8B8, width, height, 1);
		mImagePicBox->GetUIPicBox()->SetTexture(mTex2D);

		isNeedReGenTex = false;
	}

#if defined (__ANDROID__) 
	if (engineCanvas)
	{
		setWebViewRectJNI(mViewTag, screenLeft, top, screenWidth, screenHeight);
	}

#elif defined PX2_USE_AWESOMIUM
	if (msWebCore)
		msWebCore->Update();
#endif

	if (mIsUpdateToTex)
	{
		_GetWebViewImage();

		if (mIsTexNeedUpdate)
		{
			_UpdateTex2D();

			mIsTexNeedUpdate = false;
		}
	}
}
//----------------------------------------------------------------------------
void UIWebFrame::OnCulled(Culler& culler)
{
	PX2_UNUSED(culler);
	ShowNativeView(false);
}
//----------------------------------------------------------------------------
void UIWebFrame::_UpdateTex2D()
{
	if (!mWebViewImageData.empty())
	{
		if (mTex2D)
		{
			int width = mTex2D->GetWidth();
			int height = mTex2D->GetHeight();
			char* pDest = mTex2D->GetData(0);

			int offsetSrc = 0;
			int offsetDst = 0;

			for (int row = 0; row < width; ++row)
			{
				for (int col = 0; col < height; ++col)
				{
#if defined PX2_USE_AWESOMIUM
					pDest[offsetDst + 0] = mWebViewImageData[offsetSrc + 0]; // b
					pDest[offsetDst + 1] = mWebViewImageData[offsetSrc + 1]; // g 
					pDest[offsetDst + 2] = mWebViewImageData[offsetSrc + 2]; // r
					pDest[offsetDst + 3] = mWebViewImageData[offsetSrc + 3];

					offsetSrc += 4;
					offsetDst += 4;
#elif defined (__ANDROID__) 
					pDest[offsetDst + 0] = mWebViewImageData[offsetSrc + 0]; // b
					pDest[offsetDst + 1] = mWebViewImageData[offsetSrc + 1]; // g 
					pDest[offsetDst + 2] = mWebViewImageData[offsetSrc + 2]; // r
					pDest[offsetDst + 3] = 255;

					offsetSrc += 4;
					offsetDst += 4;
#endif
				}
			}

			Renderer::UpdateAll(mTex2D, 0);
		}
	}
}
//----------------------------------------------------------------------------
void UIWebFrame::_GetWebViewImage()
{
#if defined (__ANDROID__) 
	getWebViewImageJNI(mViewTag);
#elif defined PX2_USE_AWESOMIUM
	mBitmapSurface = (BitmapSurface*)mWebView->surface();
	if (!mBitmapSurface)
		return;

	int iSrcWidth = mBitmapSurface->width();
	int iSrcHeight = mBitmapSurface->height();
	if (iSrcWidth != (int)mSize.Width || iSrcHeight != (int)mSize.Height)
	{
		mWebView->Resize((int)mSize.Width, (int)mSize.Height);
	}

	char* pSrc = reinterpret_cast<char*>(const_cast<unsigned char*>((
		mBitmapSurface)->buffer()));

	char* pDest = mTex2D->GetData(0);

	int offsetSrc = 0;
	int offsetDst = 0;

	for (int row = 0; row < iSrcHeight; ++row)
	{
		for (int col = 0; col < iSrcWidth; ++col)
		{
			char a = pSrc[offsetSrc];
			char r = pSrc[offsetSrc + 1];
			char g = pSrc[offsetSrc + 2];
			char b = pSrc[offsetSrc + 3];
			pDest[offsetDst] = a;
			pDest[offsetDst + 1] = r;
			pDest[offsetDst + 2] = g;
			pDest[offsetDst + 3] = b;

			offsetSrc += 4;
			offsetDst += 4;
		}
	}

	Renderer::UpdateAll(mTex2D, 0);
#endif
}
//----------------------------------------------------------------------------
bool UIWebFrame::ShouldStartLoading(const int viewTag, const std::string &url)
{
	PX2_LOG_INFO("ShouldStartLoading %d, url %s", viewTag, url.c_str());

	std::map<int, UIWebFrame*>::iterator it = sWebViews.find(viewTag);
	if (it != sWebViews.end()) 
	{
		UIWebFrame* webView = it->second;
		if (webView)
		{
			if (!webView->OnBeginLoading(url))
				return false;
		}
	}
	return true;
}
//----------------------------------------------------------------------------
void UIWebFrame::DidFinishLoading(const int viewTag, const std::string &url)
{
	PX2_LOG_INFO("DidFinishLoading %d, url %s", viewTag, url.c_str());

	std::map<int, UIWebFrame*>::iterator it = sWebViews.find(viewTag);
	if (it != sWebViews.end()) 
	{
		UIWebFrame* webView = it->second;
		if (webView) 
		{
			webView->OnFinishLoading(url);
		}
	}
}
//----------------------------------------------------------------------------
void UIWebFrame::DidFailLoading(const int viewTag, const std::string &url)
{
	std::map<int, UIWebFrame*>::iterator it = sWebViews.find(viewTag);
	if (it != sWebViews.end()) 
	{
		UIWebFrame* webView = it->second;
		if (webView)
		{
			webView->OnFailLoading(url);
		}
	}
}
//----------------------------------------------------------------------------
void UIWebFrame::DoJsCallback(const int viewTag, const std::string &message)
{
	std::map<int, UIWebFrame*>::iterator it = sWebViews.find(viewTag);
	if (it != sWebViews.end()) 
	{
		UIWebFrame* webView = it->second;
		if (webView)
		{
			webView->OnJSCallback(message);
		}
	}
}
//----------------------------------------------------------------------------
void UIWebFrame::DoSetImageData(const int viewTag, const std::string &imageData)
{
	std::map<int, UIWebFrame*>::iterator it = sWebViews.find(viewTag);
	if (it != sWebViews.end())
	{
		UIWebFrame* webView = it->second;
		if (webView)
		{
			webView->_OnSetImageData(imageData);
		}
	}
}
//----------------------------------------------------------------------------
void UIWebFrame::_OnSetImageData(const std::string &imageData)
{
	mWebViewImageData = imageData;
	mIsTexNeedUpdate = true;
}
//----------------------------------------------------------------------------
bool UIWebFrame::OnBeginLoading(const std::string &url)
{
	PX2_LOG_INFO("OnBeginLoading %s", url.c_str());

	return true;
}
//----------------------------------------------------------------------------
void UIWebFrame::OnFinishLoading(const std::string &url)
{
	PX2_LOG_INFO("OnFinishLoading %s", url.c_str());
	SetScalesPageToFit(true);
	mIsTexNeedUpdate = true;
}
//----------------------------------------------------------------------------
void UIWebFrame::OnFailLoading(const std::string &url)
{
	PX2_LOG_INFO("OnFailLoading %s", url.c_str());
}
//----------------------------------------------------------------------------
void UIWebFrame::OnDocumentReady(const std::string &url)
{
	PX2_LOG_INFO("OnFailLoading %s", url.c_str());
}
//----------------------------------------------------------------------------
void UIWebFrame::OnJSCallback(const std::string &message)
{
	PX2_LOG_INFO("OnJSCallback %s", message.c_str());
}
#if defined PX2_USE_AWESOMIUM
//----------------------------------------------------------------------------
void UIWebFrame::OnMousePressed(MouseButtonID code)
{
	MouseButtonHelper(code, false);
}
//----------------------------------------------------------------------------
void UIWebFrame::OnMouseReleased(MouseButtonID code)
{
	MouseButtonHelper(code, true);
}
//----------------------------------------------------------------------------
void UIWebFrame::OnKeyCodePressed(KeyCode code)
{
	KeyboardButtonHelper(code, false);
}
//----------------------------------------------------------------------------
void UIWebFrame::OnKeyCodeReleased(KeyCode code)
{
	KeyboardButtonHelper(code, true);
}
//----------------------------------------------------------------------------
void UIWebFrame::OnKeyCodeChar(const std::wstring &charStr)
{
	WebKeyboardEvent ent;
	ent.type = WebKeyboardEvent::kTypeChar;
	for (int i = 0; i < (int)charStr.size(); i++)
	{
		if (0<=i && i<4)
			ent.text[i] = charStr[i];
	}

	mWebView->InjectKeyboardEvent(ent);
}
//----------------------------------------------------------------------------
void UIWebFrame::OnKeyTyped(wchar_t unichar)
{
	WebKeyboardEvent ent;
	ent.text[0] = unichar;
	ent.type = WebKeyboardEvent::kTypeChar;

	mWebView->InjectKeyboardEvent(ent);
}
//----------------------------------------------------------------------------
void UIWebFrame::OnCursorMoved(int x, int y)
{
	mWebView->InjectMouseMove(x, y);
}
//----------------------------------------------------------------------------
void UIWebFrame::OnMouseWheeled(int delta)
{
	mWebView->InjectMouseWheel((int)(delta*5.0f), 0);
}
//----------------------------------------------------------------------------
void UIWebFrame::MouseButtonHelper(MouseButtonID code, bool isUp)
{
	MouseButton mouseButton;

	switch (code)
	{
	case MBID_LEFT:
		mouseButton = kMouseButton_Left;
		break;
	case MBID_RIGHT:
		mouseButton = kMouseButton_Right;
		break;
	case MBID_MIDDLE:
		mouseButton = kMouseButton_Middle;
		break;
	default:
		mouseButton = kMouseButton_Left;
		break;
	}

	isUp ? mWebView->InjectMouseUp(mouseButton) :
		mWebView->InjectMouseDown(mouseButton);
}
//----------------------------------------------------------------------------
int UIWebFrame::ConvertKeyCode(KeyCode kc)
{
	int key = 0;
	if (KC_0 == kc)
	{
		key = KeyCodes::AK_0;
	}
	else if (KC_1 <= kc && kc <= KC_9)
	{
		key = KeyCodes::AK_1 + (kc - KC_1);
	}
	else if (KC_A <= kc && kc <= KC_Z)
	{
		key = KeyCodes::AK_A + (kc - KC_A);
	}
	else if (KC_A <= kc && kc <= KC_Z)
	{
		key = KeyCodes::AK_A + (kc - KC_A);
	}
	else if (KC_BACK == kc)
	{
		key = KeyCodes::AK_BACK;
	}
	else if (KC_TAB == kc)
	{
		key = KeyCodes::AK_TAB;
	}
	else if (KC_RETURN == kc)
	{
		key = KeyCodes::AK_RETURN;
	}
	else if (KC_ESCAPE == kc)
	{
		key = KeyCodes::AK_ESCAPE;
	}
	else if (KC_SPACE == kc)
	{
		key = KeyCodes::AK_SPACE;
	}
	else if (KC_DELETE == kc)
	{
		key = KeyCodes::AK_DELETE;
	}
	else if (KC_LSHIFT == kc)
	{
		key = KeyCodes::AK_LSHIFT;
	}
	else if (KC_LCONTROL == kc)
	{
		key = KeyCodes::AK_LCONTROL;
	}
	else if (KC_LMENU == kc)
	{
		key = KeyCodes::AK_LMENU;
	}
	else if (KC_RSHIFT == kc)
	{
		key = KeyCodes::AK_RSHIFT;
	}
	else if (KC_RCONTROL == kc)
	{
		key = KeyCodes::AK_RCONTROL;
	}
	else if (KC_LMENU == kc)
	{
		key = KeyCodes::AK_RMENU;
	}
	else if (KC_CAPITAL == kc)
	{
		key = KeyCodes::AK_CAPITAL;
	}
	else if (KC_LEFT == kc)
	{
		key = KeyCodes::AK_LEFT;
	}
	else if (KC_RIGHT == kc)
	{
		key = KeyCodes::AK_RIGHT;
	}
	else if (KC_DOWN == kc)
	{
		key = KeyCodes::AK_DOWN;
	}
	else if (KC_UP == kc)
	{
		key = KeyCodes::AK_UP;
	}
	else if (KC_LCONTROL == kc)
	{
		key = KeyCodes::AK_LCONTROL;
	}
	else if (KC_NUMLOCK == kc)
	{
		key = KeyCodes::AK_NUMLOCK;
	}
	else if (KC_SCROLL == kc)
	{
		key = KeyCodes::AK_SCROLL;
	}
	else if (KC_ESCAPE == kc)
	{
		key = KeyCodes::AK_ESCAPE;
	}

	return key;
}
//----------------------------------------------------------------------------
void UIWebFrame::KeyboardButtonHelper(KeyCode code, bool isUp)
{
	WebKeyboardEvent ent;

	ent.virtual_key_code = ConvertKeyCode(code);

	ent.type = isUp ? WebKeyboardEvent::kTypeKeyUp :
		WebKeyboardEvent::kTypeKeyDown;

	mWebView->InjectKeyboardEvent(ent);
}
#endif
//----------------------------------------------------------------------------
void UIWebFrame::OnWidgetPicked(const CanvasInputData &inputData)
{
	UIFrame::OnWidgetPicked(inputData);

#if defined PX2_USE_AWESOMIUM
	MouseButtonID mbID = MBID_LEFT;
	if (CanvasInputData::MT_LEFT == inputData.TheMouseTag)
	{
		mbID = MBID_LEFT;
	}
	else if (CanvasInputData::MT_RIGHT == inputData.TheMouseTag)
	{
		mbID = MBID_RIGHT;
	}
	else if (CanvasInputData::MT_MIDDLE == inputData.TheMouseTag)
	{
		mbID = MBID_MIDDLE;
	}

	if (UIPT_PRESSED == inputData.PickType)
	{
		OnMousePressed(mbID);
	}
	else if (UIPT_RELEASED == inputData.PickType)
	{
		OnMouseReleased(mbID);
	}
	else if (UIPT_MOVED == inputData.PickType)
	{
		const Rectf &worldRect = GetWorldRect();

		int iPosX = (int)inputData.LogicPos.X();
		iPosX -= (int)worldRect.Left;

		int iPosZ = (int)inputData.LogicPos.Z();
		iPosZ -= (int)worldRect.Bottom;

		int iHeight = (int)GetSize().Height;
		iPosZ = iHeight - iPosZ;

		OnCursorMoved(iPosX, iPosZ);
	}
	else if (UIPT_WHELLED == inputData.PickType)
	{
		OnMouseWheeled((int)inputData.Wheel);
	}
#endif
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIWebFrame::UIWebFrame(LoadConstructor value) :
UIFrame(value),
mViewTag(-1),
mIsTexNeedUpdate(true),
mIsUpdateToTex(false),
mIsShowNativeView(false)
{
#if defined PX2_USE_AWESOMIUM
	mWebView = 0;
	mBitmapSurface = 0;
#endif

	_Init();

	ComeInEventWorld();
}
//----------------------------------------------------------------------------
void UIWebFrame::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadPointer(mImagePicBox);
	source.ReadBool(mIsUpdateToTex);
	source.ReadBool(mIsShowNativeView);
	source.ReadBool(mIsAcceptKeyboardInput);
	source.ReadString(mURL);

	PX2_END_DEBUG_STREAM_LOAD(UIWebFrame, source);
}
//----------------------------------------------------------------------------
void UIWebFrame::Link(InStream& source)
{
	UIFrame::Link(source);

	source.ResolveLink(mImagePicBox);
}
//----------------------------------------------------------------------------
void UIWebFrame::PostLink()
{
	UIFrame::PostLink();

	if (mImagePicBox)
	{
		mImagePicBox->PostLink();
		mImagePicBox->GetUIPicBox()->SetOnDrawCallback(_OnDrawCallback);
		mImagePicBox->GetUIPicBox()->SetUserData("WebFrame", this);
	}

	LoadURL(mURL);
}
//----------------------------------------------------------------------------
bool UIWebFrame::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		target.Register(mImagePicBox);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIWebFrame::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	target.WritePointer(mImagePicBox);

	target.WriteBool(mIsUpdateToTex);
	target.WriteBool(mIsShowNativeView);
	target.WriteBool(mIsAcceptKeyboardInput);
	target.WriteString(mURL);

	PX2_END_DEBUG_STREAM_SAVE(UIWebFrame, target);
}
//----------------------------------------------------------------------------
int UIWebFrame::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_POINTERSIZE(mImagePicBox);

	size += PX2_BOOLSIZE(mIsUpdateToTex);
	size += PX2_BOOLSIZE(mIsShowNativeView);
	size += PX2_BOOLSIZE(mIsAcceptKeyboardInput);
	size += PX2_STRINGSIZE(mURL);

	return size;
}
//----------------------------------------------------------------------------