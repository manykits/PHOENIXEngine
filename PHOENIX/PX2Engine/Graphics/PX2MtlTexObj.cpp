// PX2MaterialTextureObj.cpp

#include "PX2MtlTexObj.hpp"
#include "PX2Math.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
MtlTexObj::MtlTexObj() :
mTexMode(TM_TEX),
mPackEle(0),
mPack(0)
{
	mTexAnimRepeat = Float2::UNIT;
	mUserNumAnim = -1;
	mIsAnimPlayOnce = false;
	mAnimInterval = 1.0f;
}
//----------------------------------------------------------------------------
MtlTexObj::~MtlTexObj()
{
}
//----------------------------------------------------------------------------
void MtlTexObj::SetUserNumAnim(int numAim)
{
	mUserNumAnim = numAim;
}
//----------------------------------------------------------------------------
int MtlTexObj::GetUserNumAnim() const
{
	return mUserNumAnim;
}
//----------------------------------------------------------------------------
int MtlTexObj::GetNumAnimFrames()
{
	if (mUserNumAnim > -1)
	{
		return mUserNumAnim;
	}

	if (TM_TEX == mTexMode)
	{
		return 1;
	}
	else if (TM_TEXPACK_ELE == mTexMode)
	{
		return 1;
	}
	else if (TM_TEX_ANIM == mTexMode)
	{
		return (int)(mTexAnimRepeat[0] * mTexAnimRepeat[1]);
	}
	else if (TM_TEXS_ANIM == mTexMode || TM_TEXPACK_ANIM == mTexMode)
	{
		return (int)mPack->Elements.size();
	}

	return 0;
}
//----------------------------------------------------------------------------
void MtlTexObj::SetAnimPlayOnce(bool playOnce)
{
	mIsAnimPlayOnce = playOnce;
}
//----------------------------------------------------------------------------
bool MtlTexObj::IsAnimPlayOnce() const
{
	return mIsAnimPlayOnce;
}
//----------------------------------------------------------------------------
void MtlTexObj::SetTexPackEle(TexPackElement *ele)
{
	mPackEle = ele;
}
//----------------------------------------------------------------------------
TexPackElement *MtlTexObj::GetTexPackElement()
{
	return mPackEle;
}
//----------------------------------------------------------------------------
void MtlTexObj::SetTexPack(TexPack *texPack)
{
	mPack = texPack;
}
//----------------------------------------------------------------------------
TexPack *MtlTexObj::GetTexPack()
{
	return mPack;
}
//----------------------------------------------------------------------------
int MtlTexObj::GetUVRect(int startRandomIndex, float age, 
	float &uBegin, float &uEnd, float &vBegin, float &vEnd)
{
	if (TM_TEX == mTexMode)
	{
		/*_*/
		return 0;
	}
	else if (TM_TEXPACK_ELE == mTexMode)
	{
		if (mPackEle->IsValid())
		{
			uBegin = (float)mPackEle->X / (float)mPackEle->TexWidth;
			uEnd = (float)(mPackEle->X + mPackEle->W) / (float)mPackEle->TexWidth;
			vBegin = (float)(mPackEle->TexHeight - mPackEle->Y - mPackEle->H) / (float)mPackEle->TexHeight;
			vEnd = (float)(mPackEle->TexHeight - mPackEle->Y) / (float)mPackEle->TexHeight;
		}

		return 0;
	}
	else if (TM_TEX_ANIM == mTexMode)
	{
		float uInterval = 1.0f / mTexAnimRepeat[0];
		float vInterval = 1.0f / mTexAnimRepeat[1];

		int index = startRandomIndex + (int)(age / mAnimInterval);
		int numAnimFrames = GetNumAnimFrames();

		if (IsAnimPlayOnce())
		{
			if (index >= (numAnimFrames - 1))
				index = numAnimFrames - 1;
		}
		index %= numAnimFrames;

		int xIndex = index % (int)mTexAnimRepeat[0];
		int yIndex = index / (int)mTexAnimRepeat[0];

		uBegin = (float)xIndex / mTexAnimRepeat[0];
		uEnd = uBegin + uInterval;

		uBegin = Mathf::Clamp(uBegin, 0.0f, 1.0f);
		uEnd = Mathf::Clamp(uEnd, 0.0f, 1.0f);

		vBegin = 1.0f - (yIndex + 1)*vInterval;
		vEnd = vBegin + vInterval;

		vBegin = Mathf::Clamp(vBegin, 0.0f, 1.0f);
		vEnd = Mathf::Clamp(vEnd, 0.0f, 1.0f);

		return index;
	}
	else if (TM_TEXS_ANIM == mTexMode)
	{
		/*_*/
		return 0;
	}
	else if (TM_TEXPACK_ANIM == mTexMode)
	{
		const std::vector<TexPackElement> &animFrames = mPack->Elements;
		int numAnims = GetNumAnimFrames();
		if (numAnims > 0)
		{
			int index = startRandomIndex + (int)(age/mAnimInterval);

			if (IsAnimPlayOnce())
			{
				if (index >= (numAnims - 1))
					index = numAnims - 1;
			}

			index = index%numAnims;

			const TexPackElement &ele = animFrames[index];

			uBegin = (float)ele.X / (float)ele.TexWidth;
			uEnd = (float)(ele.X + ele.W) / (float)ele.TexWidth;
			vBegin = (float)(ele.TexHeight - ele.Y - ele.H) / (float)ele.TexHeight;
			vEnd = (float)(ele.TexHeight - ele.Y) / (float)ele.TexHeight;

			return index;
		}
	}

	return 0;
}
//----------------------------------------------------------------------------