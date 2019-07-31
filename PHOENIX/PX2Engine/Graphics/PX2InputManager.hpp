// PX2InputManager.hpp

#ifndef PX2INPUTMANAGER_HPP
#define PX2INPUTMANAGER_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2InputEventListener.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM InputManager : public Singleton<InputManager>
	{
	public:
		InputManager();
		~InputManager();

		InputEventListener *GetDefaultListener();
		InputEventListener *CreateAddListener(int id);
		InputEventListener *GetInputListener(int id);

	protected:
		InputEventListenerPtr mInputEventListener;
		std::map<int, InputEventListenerPtr> mInputEventListenerMap;
	};

#include "PX2InputManager.inl"

#define PX2_INPUTMAN InputManager::GetSingleton()

}

#endif