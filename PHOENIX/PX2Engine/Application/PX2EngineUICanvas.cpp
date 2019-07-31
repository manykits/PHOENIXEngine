// PX2EngineUICanvas.cpp

#include "PX2EngineUICanvas.hpp"
#include "PX2ProjectEvent.hpp"
#include "PX2Project.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2UIFPicBox.hpp"
#include "PX2ScriptManager.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Canvas, EngineUICanvas);
PX2_IMPLEMENT_STREAM(EngineUICanvas);
PX2_IMPLEMENT_FACTORY(EngineUICanvas);
PX2_IMPLEMENT_DEFAULT_NAMES(Canvas, EngineUICanvas);

//----------------------------------------------------------------------------
EngineUICanvas::EngineUICanvas()
{
	SetName("EngineUICanvas");
	SetPickPosRecal(true);

	EnableUICameraAutoAdjust(true);
	SetRenderNodeUpdate(true);

	PX2_SC_LUA->SetUserTypePointer("PX2_ENGINEUICANVAS", "EngineUICanvas", this);
}
//----------------------------------------------------------------------------
EngineUICanvas::~EngineUICanvas()
{
}
//----------------------------------------------------------------------------
void EngineUICanvas::Clear()
{
	mSuperTopMovable = 0;
	mTopMovables.clear();
}
//----------------------------------------------------------------------------
void EngineUICanvas::OnSizeChanged()
{
	Canvas::OnSizeChanged();

	Project *proj = Project::GetSingletonPtr();
	if (proj)
	{
		UI *ui = proj->GetUI();
		if (ui)
		{
            const Sizef &sz = GetSize();
			ui->SetSize(sz);
		}
	}

	Event *ent = ProjectES::CreateEventX(ProjectES::UICanvasSizeChanged);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void EngineUICanvas::UpdateWorldData(double applicationTime,
	double elapsedTime)
{
	Project *proj = Project::GetSingletonPtr();
	if (proj)
	{
		UI *ui = proj->GetUI();
		if (ui) PX2_GR.SetCurEnvirParamController(ui->GetEnvirParamController());
	}

	Canvas::UpdateWorldData(applicationTime, elapsedTime);
}
//----------------------------------------------------------------------------
void EngineUICanvas::_Draw(Camera *camera, Renderer *renderer, Culler *culler)
{
	Canvas::_Draw(camera, renderer, culler);
}
//----------------------------------------------------------------------------
void EngineUICanvas::SetSuperTopMoveable(Movable *movable)
{
	mSuperTopMovable = movable;
}
//----------------------------------------------------------------------------
void EngineUICanvas::PushTopMovable(Movable *movable)
{
	mTopMovables.push_back(movable);
}
//----------------------------------------------------------------------------
void EngineUICanvas::PopTopMovable()
{
	if (!mTopMovables.empty())
		mTopMovables.pop_back();
}
//----------------------------------------------------------------------------
Movable *EngineUICanvas::GetTopestMovable()
{
	if (mSuperTopMovable)
		return mSuperTopMovable;

	if (!mTopMovables.empty())
		return mTopMovables.back();

	return 0;
}
//----------------------------------------------------------------------------
void EngineUICanvas::_DoPick(const CanvasInputData &inputData)
{
	_ClearInRangePickWidget();

	if (RenderWindow::IsScreenDrag())
		return;

	SizeNode *prePicUIFrame = 0;

	if (mRenderNode)
	{
		SizeNode *topestUIFrame = DynamicCast<SizeNode>(GetTopestMovable());
		SizeNode *uiFrame = DynamicCast<SizeNode>(mRenderNode);

		if (topestUIFrame)
			prePicUIFrame = topestUIFrame;
		else
			prePicUIFrame = uiFrame;
	}

	if (prePicUIFrame)
		prePicUIFrame->PreCanvasPick(inputData, this);

	for (int i = 0; i < (int)mUpdateChild.size(); i++)
	{
		SizeNode *uiFrame = DynamicCast<SizeNode>(mUpdateChild[i]);
		if (uiFrame)
		{
			uiFrame->PreCanvasPick(inputData, this);
		}
	}

	SizeNode *inRangeWidget = 0;
	if ((int)mInRangeWidgets.size() > 0)
	{
		_SortInRangePickWidget();
		inRangeWidget = *(mInRangeWidgets.begin());
	}

	if (inRangeWidget && inRangeWidget->IsEnable())
	{
		inRangeWidget->OnWidgetPicked(inputData);

		mPickedWidget = inRangeWidget;
	}
	else
	{
		mPickedWidget = 0;
	}
}
//----------------------------------------------------------------------------
void EngineUICanvas::Draw(Renderer *renderer)
{
	Project *proj = Project::GetSingletonPtr();
	if (proj)
	{
		UI *ui = proj->GetUI();
		if (ui) PX2_GR.SetCurEnvirParamController(ui->GetEnvirParamController());
	}

	Canvas::Draw(renderer);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// EngineUICanvas
//----------------------------------------------------------------------------
EngineUICanvas::EngineUICanvas(LoadConstructor value) :
Canvas(value)
{
}
//----------------------------------------------------------------------------
void EngineUICanvas::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Canvas::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EngineUICanvas, source);
}
//----------------------------------------------------------------------------
void EngineUICanvas::Link(InStream& source)
{
	Canvas::Link(source);
}
//----------------------------------------------------------------------------
void EngineUICanvas::PostLink()
{
	Canvas::PostLink();
}
//----------------------------------------------------------------------------
bool EngineUICanvas::Register(OutStream& target) const
{
	if (Canvas::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EngineUICanvas::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Canvas::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EngineUICanvas, target);
}
//----------------------------------------------------------------------------
int EngineUICanvas::GetStreamingSize(Stream &stream) const
{
	int size = Canvas::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------
