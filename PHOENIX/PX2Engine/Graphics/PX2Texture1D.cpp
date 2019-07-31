// PX2Texture1D.cpp

#include "PX2Texture1D.hpp"
#include "PX2BitHacks.hpp"
#include "PX2Renderer.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Texture, Texture1D);
PX2_IMPLEMENT_STREAM(Texture1D);
PX2_IMPLEMENT_FACTORY(Texture1D);
PX2_IMPLEMENT_DEFAULT_NAMES(Texture, Texture1D);
PX2_IMPLEMENT_DEFAULT_STREAM(Texture, Texture1D);

//----------------------------------------------------------------------------
Texture1D::Texture1D (Format tformat, int dimension0, int numLevels,
					  Buffer::Usage usage)
					  :
Texture(tformat, TT_1D, usage, numLevels)
{
	assertion(dimension0 > 0, "Dimension0 must be positive\n");
	mDimension[0][0] = dimension0;

	unsigned int logDim0 = Log2OfPowerOfTwo((unsigned int)dimension0);
	int maxLevels = (int)logDim0 + 1;

	if (numLevels == 0)
	{
		// 使用最大level
		mNumLevels = maxLevels;
	}
	else if (numLevels <= maxLevels)
	{
		// 使用特定level
		mNumLevels = numLevels;
	}
	else
	{
		assertion(false, "Invalid number of levels\n");
	}

	ComputeNumLevelBytes();
	mData = new1<char>(mNumTotalBytes);
}
//----------------------------------------------------------------------------
Texture1D::~Texture1D ()
{
	Renderer::UnbindAll(this);
}
//----------------------------------------------------------------------------
void Texture1D::GenerateMipmaps ()
{
	// 如果纹理级别数量不等于最大纹理级别数，将其从渲染器中释放，之后重新计算。
	unsigned int logDim0 = Log2OfPowerOfTwo((unsigned int)mDimension[0][0]);
	int maxLevels = (int)logDim0 + 1;
	bool retainBindings = true;
	if (mNumLevels != maxLevels)
	{
		retainBindings = false;
		Renderer::UnbindAll(this);
		mNumLevels = maxLevels;
		ComputeNumLevelBytes();

		char* newData = new1<char>(mNumTotalBytes);
		memcpy(newData, mData, mNumLevelBytes[0]);
		delete1(mData);
		mData = newData;
	}

	// 临时存储用来生产mipmap
	Float4* rgba = new1<Float4>(mDimension[0][0]);

	char* texels = mData;
	int length = mDimension[0][0];
	int level;
	for (level = 1; level < mNumLevels; ++level)
	{
		char* texelsNext = mData + mLevelOffsets[level];
		int lengthNext = mDimension[0][level];

		GenerateNextMipmap(length, texels, lengthNext, texelsNext, rgba);

		length = lengthNext;
		texels = texelsNext;
	}

	delete1(rgba);

	if (retainBindings)
	{
		for (level = 0; level < mNumLevels; ++level)
		{
			Renderer::UpdateAll(this, level);
		}
	}
}
//----------------------------------------------------------------------------
bool Texture1D::HasMipmaps () const
{
	unsigned int logDim0 = Log2OfPowerOfTwo((unsigned int)mDimension[0][0]);
	int maxLevels = (int)logDim0 + 1;
	return mNumLevels == maxLevels;
}
//----------------------------------------------------------------------------
char* Texture1D::GetData (int level) const
{
	if (mData && 0 <= level && level < mNumLevels)
	{
		return mData + mLevelOffsets[level];
	}

	assertion(false, "Null pointer or invalid level in GetData\n");
	return 0;
}
//----------------------------------------------------------------------------
void Texture1D::ComputeNumLevelBytes ()
{
	if (mFormat == TF_R32F
		||  mFormat == TF_G32R32F
		||  mFormat == TF_A32B32G32R32F)
	{
		if (mNumLevels > 1)
		{
			assertion(false, "No mipmaps for 32-bit float textures\n");
			mNumLevels = 1;
		}
	}
	else if (mFormat == TF_DXT1 || mFormat == TF_DXT3 || mFormat == TF_DXT5)
	{
		assertion(false, "No DXT compression for 1D textures\n");
		mNumLevels = 1;
	}
	else if (mFormat == TF_D24S8)
	{
		assertion(false, "Depth textures must be 2D\n");
		mNumLevels = 1;
	}

	int level;
	int dim0 = mDimension[0][0];
	mNumTotalBytes = 0;
	for (level = 0; level < mNumLevels; ++level)
	{
		mNumLevelBytes[level] = msPixelSize[mFormat]*dim0;
		mNumTotalBytes += mNumLevelBytes[level];
		mDimension[0][level] = dim0;
		mDimension[1][level] = 1;
		mDimension[2][level] = 1;

		if (dim0 > 1)
		{
			dim0 >>= 1;
		}
	}

	mLevelOffsets[0] = 0;
	for (level = 0; level < mNumLevels-1; ++level)
	{
		mLevelOffsets[level+1] = mLevelOffsets[level] + mNumLevelBytes[level];
	}
}
//----------------------------------------------------------------------------
void Texture1D::GenerateNextMipmap (int length, const char* texels,
									int lengthNext, char* texelsNext, Float4* rgba)
{
	// 将原始格式转换到32-bitRGBA
	msConvertFrom[mFormat](length, texels, rgba);

	// 创建miplevel
	for (int i = 0; i < lengthNext; ++i)
	{
		int base = 2*i;
		for (int c = 0; c < 4; ++c)
		{
			rgba[i][c] = 0.5f*(rgba[base][c] + rgba[base+1][c]);
		}
	}

	// 将32-bit格式转换到原始格式
	msConvertTo[mFormat](lengthNext, rgba, texelsNext);
}
//----------------------------------------------------------------------------