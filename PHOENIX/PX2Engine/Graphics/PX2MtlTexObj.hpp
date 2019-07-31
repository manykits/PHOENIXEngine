// PX2MtlObj.hpp

#ifndef PX2MTLTEXOBJ_HPP
#define PX2MTLTEXOBJ_HPP

#include "PX2Controlledable.hpp"
#include "PX2Float2.hpp"
#include "PX2Texture.hpp"
#include "PX2TexPackData.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM MtlTexObj : public Object
	{
	public:
		MtlTexObj();
		virtual ~MtlTexObj();

		enum TexMode
		{
			TM_TEX,
			TM_TEXPACK_ELE,
			TM_TEX_ANIM,
			TM_TEXS_ANIM,
			TM_TEXPACK_ANIM,
			TM_MAX_MODE
		};
		void SetTexMode(TexMode mode);
		TexMode GetTexMode() const;

		void SetUserNumAnim(int numAim);
		int GetUserNumAnim() const;
		int GetNumAnimFrames();

		void SetAnimPlayOnce(bool playOnce);
		bool IsAnimPlayOnce() const;

		void SetTexPackEle(TexPackElement *ele);
		TexPackElement *GetTexPackElement();
		void SetTexPack(TexPack *texPack);
		TexPack *GetTexPack();

		int GetUVRect(int startRandomIndex, float age,
			float &uBegin, float &uEnd, float &vBegin, float &vEnd);

	protected:
		TexMode mTexMode;

		TexPackElementPtr mPackEle;
		TexPackPtr mPack;
		Float2 mTexAnimRepeat;

		int mUserNumAnim;
		bool mIsAnimPlayOnce;
		float mAnimInterval;
	};
	typedef Pointer0<MtlTexObj> MtlTexObjPtr;

}

#endif