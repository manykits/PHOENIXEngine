// PX2RibbonEmitter.hpp

#ifndef PX2RIBBONEMITTER_HPP
#define PX2RIBBONEMITTER_HPP

#include "PX2EffectPre.hpp"
#include "PX2Effectable.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM RibbonEmitter : public Effectable
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(RibbonEmitter);

	public:
		RibbonEmitter();
		virtual ~RibbonEmitter();

		void SetLength (float length);
		float GetLength () const;

		void SetSegLength (float length);
		float GetSegLength () const;

		virtual const std::vector<std::string> &GetAllModuleNames_EA () const;
		virtual const std::vector<std::string> &GetAllModuleNames_EO () const;

		static std::vector<std::string> msModuleNames_EA;
		static std::vector<std::string> msModuleNames_EO;
	
public_internal:
		int GetNumMaxVertexs () const;
		int GetNumMaxIndexs () const;
		virtual void GenBuffers ();

	protected:
		void UpdateMaxNumVertexs ();

		float mLength;
		float mSegLength;
		int mNumMaxVertexs;
		int mNumMaxIndexs;
	};

#include "PX2RibbonEmitter.inl"
	PX2_REGISTER_STREAM(RibbonEmitter);
	typedef Pointer0<RibbonEmitter> RibbonEmitterPtr;

}

#endif