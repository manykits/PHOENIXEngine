// PX2OpenGLESVertexFormat.cpp

#ifdef PX2_USE_OPENGLES

#include "PX2OpenGLESVertexFormat.hpp"
#include "PX2OpenGLESMapping.hpp"
#include "PX2Renderer.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PdrVertexFormat::PdrVertexFormat (Renderer*, const VertexFormat* vformat)
{
	mStride = vformat->GetStride();

	VertexFormat::AttributeType type;

	int i = vformat->GetIndex(VertexFormat::AU_POSITION);
	if (i >= 0)
	{
		mHasPosition = 1;
		type = vformat->GetAttributeType(i);
		mPositionChannels = gOGLAttributeChannels[type];
		mPositionType = gOGLAttributeType[type];
		mPositionOffset = vformat->GetOffset(i);
	}
	else
	{
		mHasPosition = 0;
		mPositionChannels = 0;
		mPositionType = 0;
		mPositionOffset = 0;
	}

	i = vformat->GetIndex(VertexFormat::AU_NORMAL);
	if (i >= 0)
	{
		mHasNormal = 1;
		type = vformat->GetAttributeType(i);
		mNormalChannels = gOGLAttributeChannels[type];
		mNormalType = gOGLAttributeType[vformat->GetAttributeType(i)];
		mNormalOffset = vformat->GetOffset(i);
	}
	else
	{
		mHasNormal = 0;
		mNormalChannels = 0;
		mNormalType = 0;
		mNormalOffset = 0;
	}

	i = vformat->GetIndex(VertexFormat::AU_TANGENT);
	if (i >= 0)
	{
		mHasTangent = 1;
		type = vformat->GetAttributeType(i);
		mTangentChannels = gOGLAttributeChannels[type];
		mTangentType = gOGLAttributeType[vformat->GetAttributeType(i)];
		mTangentOffset = vformat->GetOffset(i);
	}
	else
	{
		mHasTangent = 0;
		mTangentChannels = 0;
		mTangentType = 0;
		mTangentOffset = 0;
	}

	i = vformat->GetIndex(VertexFormat::AU_BINORMAL);
	if (i >= 0)
	{
		mHasBinormal = 1;
		type = vformat->GetAttributeType(i);
		mBinormalChannels = gOGLAttributeChannels[type];
		mBinormalType = gOGLAttributeType[vformat->GetAttributeType(i)];
		mBinormalOffset = vformat->GetOffset(i);
	}
	else
	{
		mHasBinormal = 0;
		mBinormalChannels = 0;
		mBinormalType = 0;
		mBinormalOffset = 0;
	}

	unsigned int unit;
	for (unit = 0; unit < VertexFormat::AM_MAX_COLOR_UNITS; ++unit)
	{
		i = vformat->GetIndex(VertexFormat::AU_COLOR, unit);
		if (i >= 0)
		{
			mHasColor[unit] = 1;
			type = vformat->GetAttributeType(i);
			mColorChannels[unit] = gOGLAttributeChannels[type];
			mColorType[unit] =
				gOGLAttributeType[vformat->GetAttributeType(i)];
			mColorOffset[unit] = vformat->GetOffset(i);
		}
		else
		{
			mHasColor[unit] = 0;
			mColorChannels[unit] = 0;
			mColorType[unit] = 0;
			mColorOffset[unit] = 0;
		}
	}

	for (unit = 0; unit < VertexFormat::AM_MAX_TCOORD_UNITS; ++unit)
	{
		i = vformat->GetIndex(VertexFormat::AU_TEXCOORD, unit);
		if (i >= 0)
		{
			mHasTCoord[unit] = 1;
			type = vformat->GetAttributeType(i);
			mTCoordChannels[unit] = gOGLAttributeChannels[type];
			mTCoordType[unit] =
				gOGLAttributeType[vformat->GetAttributeType(i)];
			mTCoordOffset[unit] = vformat->GetOffset(i);
		}
		else
		{
			mHasTCoord[unit] = 0;
			mTCoordChannels[unit] = 0;
			mTCoordType[unit] = 0;
			mTCoordOffset[unit] = 0;
		}
	}

	i = vformat->GetIndex(VertexFormat::AU_BLENDINDICES);
	if (i >= 0)
	{
		mHasBlendIndices = 1;
		type = vformat->GetAttributeType(i);
		mBlendIndicesChannels = gOGLAttributeChannels[type];
		mBlendIndicesType = gOGLAttributeType[vformat->GetAttributeType(i)];
		mBlendIndicesOffset = vformat->GetOffset(i);
	}
	else
	{
		mHasBlendIndices = 0;
		mBlendIndicesChannels = 0;
		mBlendIndicesType = 0;
		mBlendIndicesOffset = 0;
	}

	i = vformat->GetIndex(VertexFormat::AU_BLENDWEIGHT);
	if (i >= 0)
	{
		mHasBlendWeight = 1;
		type = vformat->GetAttributeType(i);
		mBlendWeightChannels = gOGLAttributeChannels[type];
		mBlendWeightType = gOGLAttributeType[vformat->GetAttributeType(i)];
		mBlendWeightOffset = vformat->GetOffset(i);
	}
	else
	{
		mHasBlendWeight = 0;
		mBlendWeightChannels = 0;
		mBlendWeightType = 0;
		mBlendWeightOffset = 0;
	}

	i = vformat->GetIndex(VertexFormat::AU_FOGCOORD);
	if (i >= 0)
	{
		mHasFogCoord = 1;
		type = vformat->GetAttributeType(i);
		mFogCoordChannels = gOGLAttributeChannels[type];
		mFogCoordType = gOGLAttributeType[vformat->GetAttributeType(i)];
		mFogCoordOffset = vformat->GetOffset(i);
	}
	else
	{
		mHasFogCoord = 0;
		mFogCoordChannels = 0;
		mFogCoordType = 0;
		mFogCoordOffset = 0;
	}

	i = vformat->GetIndex(VertexFormat::AU_PSIZE);
	if (i >= 0)
	{
		mHasPSize = 1;
		type = vformat->GetAttributeType(i);
		mPSizeChannels = gOGLAttributeChannels[type];
		mPSizeType = gOGLAttributeType[vformat->GetAttributeType(i)];
		mPSizeOffset = vformat->GetOffset(i);
	}
	else
	{
		mHasPSize = 0;
		mPSizeChannels = 0;
		mPSizeType = 0;
		mPSizeOffset = 0;
	}
}
//----------------------------------------------------------------------------
PdrVertexFormat::~PdrVertexFormat ()
{
	if (mHasPosition)
	{
		PX2_GL_CHECK(glDisableVertexAttribArray(ALP_POSITION));
	}

	if (mHasNormal)
	{
		PX2_GL_CHECK(glDisableVertexAttribArray(ALP_NORMAL));
	}

	unsigned int unit;
	for (unit = 0; unit < VertexFormat::AM_MAX_COLOR_UNITS; ++unit)
	{
		if (mHasColor[unit])
			PX2_GL_CHECK(glDisableVertexAttribArray(ALP_COLOR0+ unit));
	}

	for (unit = 0; unit < VertexFormat::AM_MAX_TCOORD_UNITS; ++unit)
	{
		if (mHasTCoord[unit])
			PX2_GL_CHECK(glDisableVertexAttribArray(ALP_TEXCOORD0 + unit));
	}
}
//----------------------------------------------------------------------------
void PdrVertexFormat::Enable (Renderer *renderer)
{
	PX2_UNUSED(renderer);

	if (mHasPosition)
	{
		PX2_GL_CHECK(glVertexAttribPointer(ALP_POSITION, mPositionChannels, mPositionType, GL_FALSE, 
			mStride, (const void*)mPositionOffset));
		PX2_GL_CHECK(glEnableVertexAttribArray(ALP_POSITION));
	}

	if (mHasNormal)
	{
		PX2_GL_CHECK(glVertexAttribPointer(ALP_NORMAL, mNormalChannels, mNormalType, GL_FALSE,
			mStride, (const void*)mNormalOffset));
		PX2_GL_CHECK(glEnableVertexAttribArray(ALP_NORMAL));
	}

	unsigned int unit;
	for (unit = 0; unit < VertexFormat::AM_MAX_COLOR_UNITS; ++unit)
	{
		if (mHasColor[unit])
		{
			PX2_GL_CHECK(glVertexAttribPointer(ALP_COLOR0 + unit, mColorChannels[unit], 
				mColorType[unit], GL_FALSE,	mStride, (const void*)mColorOffset[unit]));
			PX2_GL_CHECK(glEnableVertexAttribArray(ALP_COLOR0 + unit));
		}
	}

	for (unit = 0; unit < VertexFormat::AM_MAX_TCOORD_UNITS; ++unit)
	{
		if (mHasTCoord[unit])
		{
			PX2_GL_CHECK(glVertexAttribPointer(ALP_TEXCOORD0 + unit, mTCoordChannels[unit], 
				mTCoordType[unit], GL_FALSE, mStride, (const void*)mTCoordOffset[unit]));
			PX2_GL_CHECK(glEnableVertexAttribArray(ALP_TEXCOORD0 + unit));
		}
	}
}
//----------------------------------------------------------------------------
void PdrVertexFormat::Disable (Renderer*)
{
	if (mHasPosition)
	{
		PX2_GL_CHECK(glDisableVertexAttribArray(ALP_POSITION));
	}

	if (mHasNormal)
	{
		PX2_GL_CHECK(glDisableVertexAttribArray(ALP_NORMAL));
	}

	unsigned int unit;
	for (unit = 0; unit < VertexFormat::AM_MAX_COLOR_UNITS; ++unit)
	{
		if (mHasColor[unit])
			PX2_GL_CHECK(glDisableVertexAttribArray(ALP_COLOR0 + unit));
	}
	
	for (unit = 0; unit < VertexFormat::AM_MAX_TCOORD_UNITS; ++unit)
	{
		if (mHasTCoord[unit])
			PX2_GL_CHECK(glDisableVertexAttribArray(ALP_TEXCOORD0 + unit));
	}
}
//----------------------------------------------------------------------------

#endif