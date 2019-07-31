// PX2UIWebFrame.hpp

#ifndef PX2UIWEBFRAME_HPP
#define PX2UIWEBFRAME_HPP

#include "PX2UIPre.hpp"
#include "PX2UIFrame.hpp"
#include "PX2UIFPicBox.hpp"
#include "PX2InputEventData.hpp"

#if PX2_USE_AWESOMIUM
namespace Awesomium
{
	class WebCore;
	class WebView;
	class BitmapSurface;
}
#endif

namespace PX2
{

	class PX2_ENGINE_ITEM UIWebFrame : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(UIWebFrame);
		PX2_NEW(UIWebFrame);

	public:
		UIWebFrame();
		virtual ~UIWebFrame();

		void ShowNativeView(bool show);
		bool IsShowNativeView() const;

		void SetUpdateToTex(bool updateToTex);
		bool IsUpdateToTex() const;

		void SetAcceptKeyboardInput(bool acpt);
		bool IsAcceptKeyboardInput() const;

		void LoadHTMLString(const std::string & str, const std::string & baseURL);
		void LoadURL(const std::string &url);
		void LoadFile(const std::string &fileName);

		void StopLoading();
		void Reload();

		bool CanGoBack();
		bool CanGoForward();
		void GoBack();
		void GoForward();
		void SelectAll();
		void Undo();
		void ReDo();
		void Cut();
		void Copy();
		void Paste();

		void CreateJSGlobalVaiable(const std::string &name, const std::string &val);
		std::string EvaluateJS(const std::string &js);

		void SetScalesPageToFit(const bool scalesPageToFit);

		static bool ShouldStartLoading(const int viewTag, const std::string &url);
		static void DidFinishLoading(const int viewTag, const std::string &url);
		static void DidFailLoading(const int viewTag, const std::string &url);
		static void DoJsCallback(const int viewTag, const std::string &message);
		static void DoSetImageData(const int viewTag, const std::string &imageData);

		virtual bool OnBeginLoading(const std::string &url);
		virtual void OnFinishLoading(const std::string &url);
		virtual void OnFailLoading(const std::string &url);
		virtual void OnDocumentReady(const std::string &url);
		virtual void OnJSCallback(const std::string &message);

		virtual void OnEvent(Event *ent);

	protected:
		void _Init();
		virtual void OnSizeChanged();
		virtual void OnPivotChanged();
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);
		virtual void OnCulled(Culler& culler);
		void _OnSetImageData(const std::string &imageData);
		void _GetWebViewImage();
		void _UpdateTex2D();
		virtual void OnWidgetPicked(const CanvasInputData &inputData);

		UIFPicBoxPtr mImagePicBox;
		Texture2DPtr mTex2D;

		std::string mWebViewImageData;
		bool mIsTexNeedUpdate;
		bool mIsUpdateToTex;
		bool mIsShowNativeView;
		bool mIsAcceptKeyboardInput;
		std::string mURL;

		int mViewTag;

		static int mNumberOfViews;

#if defined PX2_USE_AWESOMIUM
		void OnMousePressed(MouseButtonID code);
		void OnMouseReleased(MouseButtonID code);
		void OnKeyCodePressed(KeyCode code);
		void OnKeyCodeReleased(KeyCode code);
		void OnKeyCodeChar(const std::wstring &charStr);
		void OnKeyTyped(wchar_t unichar);
		void OnCursorMoved(int x, int y);
		void OnMouseWheeled(int delta);

		void MouseButtonHelper(MouseButtonID code, bool isUp);
		void KeyboardButtonHelper(KeyCode code, bool isUp);
		int ConvertKeyCode(KeyCode kc);

		static Awesomium::WebCore *msWebCore;
		Awesomium::WebView *mWebView;
		Awesomium::BitmapSurface *mBitmapSurface;
		ObjectPtr mObject;
#endif
	};

	PX2_REGISTER_STREAM(UIWebFrame);
	typedef Pointer0<UIWebFrame> UIWebFramePtr;

}

#endif