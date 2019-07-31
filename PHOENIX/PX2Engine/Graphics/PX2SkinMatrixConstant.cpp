// PX2SkinMatrixConstant.cpp

#include "PX2SkinMatrixConstant.hpp"
#include "PX2Camera.hpp"
#include "PX2Renderable.hpp"
#include "PX2SkinController.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, ShaderFloat, SkinMatrixConstant);
PX2_IMPLEMENT_STREAM(SkinMatrixConstant);
PX2_IMPLEMENT_FACTORY(SkinMatrixConstant);
PX2_IMPLEMENT_DEFAULT_NAMES(ShaderFloat, SkinMatrixConstant);

//----------------------------------------------------------------------------
SkinMatrixConstant::SkinMatrixConstant (int numMatrix)
	:
ShaderFloat(numMatrix*3),
mSkinContrl(0)
{
	mAllowUpdater = true;
}
//----------------------------------------------------------------------------
SkinMatrixConstant::~SkinMatrixConstant ()
{
}
//----------------------------------------------------------------------------
void SkinMatrixConstant::Update(const ShaderStruct *struc)
{
	const Renderable *renderable = struc->TheRenderable;

	if (!mSkinContrl)
	{
		for (int i=0; i<renderable->GetNumControllers(); i++)
		{
			SkinController *skinCtrl = DynamicCast<SkinController>(
				renderable->GetController(i));
			if (skinCtrl)
			{
				mSkinContrl = skinCtrl;
			}
		}
	}

	if (!mSkinContrl)
		return;

	int numBones = mSkinContrl->GetNumBones();
	int allowNumBones = GetNumRegisters()/3;

	if (numBones <= allowNumBones)
	{
		for (int i=0; i<numBones; i++)
		{
			const HMatrix &mat = mSkinContrl->GetGPUMatrix()[i];
			const float* source = (const float*)mat;
			float* target = mData + 12*i;
			for (int j = 0; j < 12; ++j)
			{
				*target++ = *source++;
			}
		}

		for (int i=numBones; i<allowNumBones; i++)
		{
			const float *source = (const float*)HMatrix::IDENTITY;

			float* target = mData + 12*i;
			for (int j = 0; j < 12; ++j)
			{
				*target++ = *source++;
			}
		}
	}
	else
	{
		//assertion(false, "bones num too big.\n");
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
SkinMatrixConstant::SkinMatrixConstant (LoadConstructor value)
	:
ShaderFloat(value),
mSkinContrl(0)
{
}
//----------------------------------------------------------------------------
void SkinMatrixConstant::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	ShaderFloat::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(SkinMatrixConstant, source);
}
//----------------------------------------------------------------------------
void SkinMatrixConstant::Link (InStream& source)
{
	ShaderFloat::Link(source);
}
//----------------------------------------------------------------------------
void SkinMatrixConstant::PostLink ()
{
	ShaderFloat::PostLink();
}
//----------------------------------------------------------------------------
bool SkinMatrixConstant::Register (OutStream& target) const
{
	if (ShaderFloat::Register(target))
	{
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void SkinMatrixConstant::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	ShaderFloat::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(SkinMatrixConstant, target);
}
//----------------------------------------------------------------------------
int SkinMatrixConstant::GetStreamingSize (Stream &stream) const
{
	int size = ShaderFloat::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	return size;
}
//----------------------------------------------------------------------------
