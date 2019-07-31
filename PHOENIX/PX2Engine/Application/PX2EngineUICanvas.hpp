// PX2EngineUICanvas.hpp

#ifndef PX2ENGINEUICANVAS_HPP
#define PX2ENGINEUICANVAS_HPP

#include "PX2Singleton_NeedNew.hpp"
#include "PX2Canvas.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM EngineUICanvas : public Canvas,
		public Singleton<EngineUICanvas>
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(EngineUICanvas);
	
	public:
		EngineUICanvas();
		virtual ~EngineUICanvas();

		void Clear();

		virtual void OnSizeChanged();

		virtual void Draw(Renderer *renderer);

	protected:
		virtual void UpdateWorldData(double applicationTime,
			double elapsedTime);
		virtual void _Draw(Camera *camera, Renderer *renderer, Culler *culler);

		// Top Movable
	public:
		void SetSuperTopMoveable(Movable *movable);
		Movable *GetSuperTopMovbale();
		void PushTopMovable(Movable *movable);
		void PopTopMovable();

		Movable *GetTopestMovable();

	protected:
		virtual void _DoPick(const CanvasInputData &inputData);

		MovablePtr mSuperTopMovable;
		std::deque<MovablePtr> mTopMovables;
	};
	PX2_REGISTER_STREAM(EngineUICanvas);
	typedef Pointer0<EngineUICanvas> EngineUICanvasPtr;

}

#endif