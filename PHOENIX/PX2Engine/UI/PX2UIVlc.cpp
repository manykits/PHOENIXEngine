// PX2UIVlc.cpp

#include "PX2UIVlc.hpp"
#include "PX2Renderer.hpp"
#include "PX2Log.hpp"
#include "PX2Application.hpp"
#include "PX2HardCamera.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UIVlc);
PX2_IMPLEMENT_STREAM(UIVlc);
PX2_IMPLEMENT_FACTORY(UIVlc);

//----------------------------------------------------------------------------
UIVlc::UIVlc()
{
	mFPicBox = new0 UIFPicBox();
	AttachChild(mFPicBox);
	mFPicBox->LocalTransform.SetTranslateY(-1.0f);
	mFPicBox->SetAnchorHor(0.0f, 1.0f);
	mFPicBox->SetAnchorVer(0.0f, 1.0f);
	mFPicBox->GetUIPicBox()->SetTexture("Data/engine/white.png");

	mFPicBoxRight = new0 UIFPicBox();
	AttachChild(mFPicBoxRight);
	mFPicBoxRight->LocalTransform.SetTranslateY(-1.0f);
	mFPicBoxRight->GetUIPicBox()->SetTexture("Data/engine/white.png");
	mFPicBoxRight->Show(false);

	mVLCMemObj = new0 VLCMemObj();
	mVLC = new0 VLC();
	mVLC->SetMem(mVLCMemObj);

	mType = T_HARDCAMERA;
	mMode = M_NORMAL;

	SetWidget(true);

	SetName("UIVlc");
}
//----------------------------------------------------------------------------
UIVlc::~UIVlc()
{
	PX2_HARDCAMERA.SetObj(0);
	PX2_HARDCAMERA.ClearHardCameraCallbacks();

	mFPicBox->GetUIPicBox()->SetTexture(0);
	mFPicBoxRight->GetUIPicBox()->SetTexture(0);

	mVLC = 0;
	mVLCMemObj = 0;
}
//----------------------------------------------------------------------------
static void _HardCameraCallback(int width, int height, const char* buf, int size)
{
	void *object = PX2_HARDCAMERA.GetObj();
	if (object)
	{
		UIVlc *uiVLC = (UIVlc*)object;
		VLCMemObj *vlcMemObj = uiVLC->GetVLCMemObj();
		int mediaWidth = vlcMemObj->GetMediaWidth();
		int mediaHeight = vlcMemObj->GetMediaHeight();
		
		if (mediaWidth != width || mediaHeight != height)
		{
			if (width > 0 && height > 0)
			{
				vlcMemObj->SetMediaWidthHeight(width, height);
				vlcMemObj->OnFormatSetup();
			}
		}

		vlcMemObj->OnFrameReady(width, height, buf, size);
	}
}
//----------------------------------------------------------------------------
void UIVlc::SetType(Type type)
{
	mType = type;

	if (T_HARDCAMERA == mType)
	{
		PX2_HARDCAMERA.SetObj(this);
		PX2_HARDCAMERA.ClearHardCameraCallbacks();
		PX2_HARDCAMERA.AddHardCameraCallback(_HardCameraCallback);
	}
}
//----------------------------------------------------------------------------
UIVlc::Type UIVlc::GetType() const
{
	return mType;
}
//----------------------------------------------------------------------------
void UIVlc::ShowPic(bool isShow)
{
	mVLCMemObj->ShowPic(isShow);
}
//----------------------------------------------------------------------------
void UIVlc::SetMode(Mode m)
{
	mMode = m;
	_UpdateOnMode(mMode);
}
//----------------------------------------------------------------------------
void UIVlc::SetHorSpaw(bool spaw)
{
	if (spaw)
	{
		if (mFPicBox)
		{
			mFPicBox->LocalTransform.SetScale(APoint(-1.0f, 1.0, 1.0));
			mFPicBox->GetUIPicBox()->SetDoubleSide(spaw);
		}

		if (mFPicBoxRight)
		{
			mFPicBoxRight->LocalTransform.SetScale(APoint(-1.0f, 1.0, 1.0));
			mFPicBoxRight->GetUIPicBox()->SetDoubleSide(spaw);
		}
	}
	else
	{
		if (mFPicBox)
		{
			mFPicBox->LocalTransform.SetScale(APoint(1.0f, 1.0, 1.0));
		}

		if (mFPicBoxRight)
		{
			mFPicBoxRight->LocalTransform.SetScale(APoint(1.0f, 1.0, 1.0));
		}
	}
}
//----------------------------------------------------------------------------
void UIVlc::StartVLC(const std::string &filename)
{
	PX2_UNUSED(filename);

	if (mVLC)
	{
		mVLC->Start(filename);
	}
}
//----------------------------------------------------------------------------
void UIVlc::StopVLC()
{
	if (mVLC)
	{
		mVLC->Stop();
	}
}
//----------------------------------------------------------------------------
UIVlc::Mode UIVlc::GetMode() const
{
	return mMode;
}
//----------------------------------------------------------------------------
void UIVlc::SendToGetCameraView(std::string &url, int port)
{
	SocketAddress skAddr(url, (int16_t)port);

	std::string strBuf = "getcameraview";

	UDPServer *udpServer = PX2_APP.GetEngineUDPServer();
	udpServer->GetSocket().SendTo(strBuf.c_str(),
		(int)strBuf.length(), skAddr);
}
//----------------------------------------------------------------------------
void UIVlc::_UpdateOnMode(Mode m)
{
	if (M_NORMAL == m)
	{
		mFPicBox->Show(true);
		mFPicBox->SetAnchorHor(0.0f, 1.0f);
		mFPicBox->SetAnchorVer(0.0f, 1.0f);
		mFPicBoxRight->Show(false);
	}
	else if (M_VR == m)
	{
		mFPicBox->Show(true);
		mFPicBox->SetAnchorHor(0.0f, 0.5f);
		mFPicBox->SetAnchorVer(0.0f, 1.0f);

		mFPicBoxRight->Show(true);
		mFPicBoxRight->SetAnchorHor(0.5f, 1.0f);
		mFPicBoxRight->SetAnchorVer(0.0f, 1.0f);
	}
}
//----------------------------------------------------------------------------
void UIVlc::UpdateWorldData(double applicationTime,
	double elapsedTime)
{
	UIFrame::UpdateWorldData(applicationTime, elapsedTime);

	if (mVLCMemObj && mVLCMemObj->IsTextureUpdated())
	{
		Texture2D *tex2D = mVLCMemObj->GetTex2D();
		if (tex2D)
		{
			mFPicBox->GetUIPicBox()->SetTexture(tex2D);
			if (M_VR == mMode)
				mFPicBoxRight->GetUIPicBox()->SetTexture(tex2D);
			else
				mFPicBoxRight->GetUIPicBox()->SetTexture(0);

			Renderer::UpdateAll(tex2D, 0);
		}

		mVLCMemObj->SetTextureUpdated(false);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIVlc::UIVlc(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void UIVlc::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UIVlc, source);
}
//----------------------------------------------------------------------------
void UIVlc::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void UIVlc::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool UIVlc::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIVlc::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UIVlc, target);
}
//----------------------------------------------------------------------------
int UIVlc::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------