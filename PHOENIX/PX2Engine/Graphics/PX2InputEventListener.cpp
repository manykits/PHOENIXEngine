// PX2InputEventListener.cpp

#include "PX2InputEventListener.hpp"
#include "PX2EventWorld.hpp"
#include "PX2GraphicsRoot.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
InputEventListener::InputEventListener(int viewID) :
mViewID(viewID)
{
}
//----------------------------------------------------------------------------
InputEventListener::~InputEventListener()
{
}
//----------------------------------------------------------------------------
void InputEventListener::OnInputEventData(InputEventData &data)
{
	data.ViewID = mViewID;

	mCurInputEventData = data;

	Event *ent = 0;
	if (InputEventSpace::KeyPressed == data.TheEventType)
	{
		ent = PX2_CREATEEVENTEX(InputEventSpace, KeyPressed);
		ent->SetDataStr0(data.KChar);
	}
	else if (InputEventSpace::KeyReleased == data.TheEventType)
	{
		ent = PX2_CREATEEVENTEX(InputEventSpace, KeyReleased);
		ent->SetDataStr0(data.KChar);
	}
	else
	{
		ent = InputEventSpace::CreateEventX(data.TheEventType);
		ent->SetDataStr0("unknow");
	}

	ent->SetData<InputEventData>(data);
	EventWorld::GetSingleton().BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void InputEventListener::SetViewSize(const Sizef &size)
{
	mViewSize = size;
}
//----------------------------------------------------------------------------
const Sizef &InputEventListener::GetViewSize() const
{
	return mViewSize;
}
//----------------------------------------------------------------------------
void InputEventListener::KeyPressed(KeyCode code)
{
	InputEventData data;
	data.TheEventType = InputEventSpace::KeyPressed;
	data.KCode = code;
	data.KChar = InputEventData::KCode2KChar(code);

	OnInputEventData(data);
}
//----------------------------------------------------------------------------
void InputEventListener::KeyReleased(KeyCode code)
{
	InputEventData data;
	data.TheEventType = InputEventSpace::KeyReleased;
	data.KCode = code;
	data.KChar = InputEventData::KCode2KChar(code);

	OnInputEventData(data);
}
//----------------------------------------------------------------------------
void InputEventListener::KeyChar(const std::wstring &charStr)
{
	InputEventData data;
	data.TheEventType = InputEventSpace::KeyChar;
	data.KCharW = charStr;

	OnInputEventData(data);
}
//----------------------------------------------------------------------------
void InputEventListener::MouseMoved(const APoint &pos)
{
	InputEventData data;
	data.TheEventType = InputEventSpace::MouseMoved;
	data.MTPos = pos;
	TouchState ts;
	ts.Pos = pos;
	data.TPoses.push_back(ts);

	OnInputEventData(data);
}
//----------------------------------------------------------------------------
void InputEventListener::MousePressed(MouseButtonID id, const APoint &pos)
{
	InputEventData data;
	data.TheEventType = InputEventSpace::MousePressed;
	data.MButtonID = id;
	data.MTPos = pos;
	TouchState ts;
	ts.Pos = pos;
	data.TPoses.push_back(ts);

	OnInputEventData(data);
}
//----------------------------------------------------------------------------
void InputEventListener::MouseDoublePressed(MouseButtonID id, 
	const APoint &pos)
{
	InputEventData data;
	data.TheEventType = InputEventSpace::MouseDoublePressed;
	data.MButtonID = id;
	data.MTPos = pos;
	TouchState ts;
	ts.Pos = pos;
	data.TPoses.push_back(ts);

	OnInputEventData(data);
}
//----------------------------------------------------------------------------
void InputEventListener::MouseReleased(MouseButtonID id, const APoint &pos)
{
	InputEventData data;
	data.TheEventType = InputEventSpace::MouseReleased;
	data.MButtonID = id;
	data.MTPos = pos;
	TouchState ts;
	ts.Pos = pos;
	data.TPoses.push_back(ts);

	OnInputEventData(data);
}
//----------------------------------------------------------------------------
void InputEventListener::MouseWheeled(float val, const APoint &pos)
{
	InputEventData data;
	data.TheEventType = InputEventSpace::MouseWheeled;
	data.MButtonID = MBID_MIDDLE;
	data.MWheel = val;
	data.MTPos = pos;
	TouchState ts;
	ts.Pos = pos;
	data.TPoses.push_back(ts);

	OnInputEventData(data);
}
//----------------------------------------------------------------------------
void InputEventListener::TouchMoved(int num, int ids[], float xs[], float ys[])
{
	InputEventData data;
	data.TheEventType = InputEventSpace::TouchMoved;

	_OnTouchData(data, num, ids, xs, ys);

	OnInputEventData(data);
}
//----------------------------------------------------------------------------
void InputEventListener::TouchPressed(int num, int ids[], float xs[], 
	float ys[])
{
	InputEventData data;
	data.TheEventType = InputEventSpace::TouchPressed;

	_OnTouchData(data, num, ids, xs, ys);

	OnInputEventData(data);
}
//----------------------------------------------------------------------------
void InputEventListener::TouchReleased(int num, int ids[], float xs[], 
	float ys[])
{
	InputEventData data;
	data.TheEventType = InputEventSpace::TouchReleased;

	_OnTouchData(data, num, ids, xs, ys);

	OnInputEventData(data);
}
//----------------------------------------------------------------------------
void InputEventListener::TouchCancelled(int num, int ids[], float xs[],
	float ys[])
{
	InputEventData data;
	data.TheEventType = InputEventSpace::TouchCancelled;

	_OnTouchData(data, num, ids, xs, ys);

	OnInputEventData(data);
}
//----------------------------------------------------------------------------
void InputEventListener::_OnTouchData(InputEventData &data, int num,
	int ids[], float xs[], float ys[])
{
	for (int i = 0; i < num; i++)
	{
		int id = ids[i];
		float x = xs[i];
		float y = ys[i];

		y = mViewSize.Height - y;

		APoint pos = APoint(x, 0.0f, y);

		TouchState ts;
		ts.ID = id;
		ts.Pos = pos;
		data.TPoses.push_back(ts);

		if (0 == i)
		{
			data.MTPos = pos;
		}
	}
}
//----------------------------------------------------------------------------
void InputEventListener::KeypadMsg(bool back, bool menu)
{
	PX2_UNUSED(back);
	PX2_UNUSED(menu);
}
//----------------------------------------------------------------------------
