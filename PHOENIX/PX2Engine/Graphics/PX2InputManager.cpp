// PX2InputManager.cpp

#include "PX2InputManager.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
InputManager::InputManager()
{
	mInputEventListener = CreateAddListener(0);
}
//----------------------------------------------------------------------------
InputManager::~InputManager()
{
}
//----------------------------------------------------------------------------
InputEventListener *InputManager::CreateAddListener(int id)
{
	std::map<int, InputEventListenerPtr>::iterator
		it = mInputEventListenerMap.find(id);

	if (it != mInputEventListenerMap.end())
	{
		return it->second;
	}
	else
	{
		InputEventListener *listener = new0 InputEventListener(id);
		mInputEventListenerMap[id] = listener;

		return listener;
	}
}
//----------------------------------------------------------------------------
InputEventListener *InputManager::GetInputListener(int id)
{
	std::map<int, InputEventListenerPtr>::iterator
		it = mInputEventListenerMap.find(id);

	if (it != mInputEventListenerMap.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------