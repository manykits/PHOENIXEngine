// PX2SwitchNode.hpp

#ifndef PX2SWITCHNODE_HPP
#define PX2SWITCHNODE_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Node.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM SwitchNode : public Node
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(SwitchNode);

	public:
		SwitchNode ();
		virtual ~SwitchNode ();

		static SwitchNode *Create();

		virtual void Play ();
		virtual bool IsPlaying () const;
		virtual void Pause();
		virtual void Reset ();

		enum { SN_INVALID_CHILD = -1 };

		inline void SetActiveChild (int activeChild);
		inline void SetActiveChild (Movable *activeChild);
		inline int GetActiveChild () const;
		inline Movable *GetActiveChildPointer ();
		inline void DisableAllChildren ();

	protected:
		virtual void OnGetVisibleSet (Culler& culler, bool noCull);

		int mActiveChild;
	};

	PX2_REGISTER_STREAM(SwitchNode);
	typedef Pointer0<SwitchNode> SwitchNodePtr;
#include "PX2SwitchNode.inl"

}

#endif