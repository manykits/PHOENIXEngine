// PX2Texture2D.cpp

#include "PX2GraphicsPre.hpp"
#include "PX2Texture2D.hpp"
#include "PX2BitHacks.hpp"
#include "PX2Renderer.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Texture, Texture2D);
PX2_IMPLEMENT_STREAM(Texture2D);
PX2_IMPLEMENT_FACTORY(Texture2D);
PX2_IMPLEMENT_DEFAULT_NAMES(Texture, Texture2D);
PX2_IMPLEMENT_DEFAULT_STREAM(Texture, Texture2D);

//----------------------------------------------------------------------------
Texture2D::Texture2D (Format tformat, int dimension0, int dimension1,
					  int numLevels, Buffer::Usage usage)
					  :
Texture(tformat, TT_2D, usage, numLevels)
{
	assertion(dimension0 > 0, "Dimension0 must be positive\n");
	assertion(dimension1 > 0, "Dimension1 must be positive\n");
	mDimension[0][0] = dimension0;
	mDimension[1][0] = dimension1;

	unsigned int logDim0 = Log2OfPowerOfTwo((unsigned int)dimension0);
	unsigned int logDim1 = Log2OfPowerOfTwo((unsigned int)dimension1);
	int maxLevels = (int)(logDim0 >= logDim1 ? logDim0 + 1 : logDim1 + 1);

	if (numLevels == 0)
	{
		// 使用最大levels
		mNumLevels = maxLevels;
	}
	else if (numLevels <= maxLevels)
	{
		// 使用特定levels
		mNumLevels = numLevels;
	}
	else
	{
		assertion(false, "Invalid number of levels\n");
	}

	ComputeNumLevelBytes();
	mData = new1<char>(mNumTotalBytes);
	memset(mData, 0, mNumTotalBytes*sizeof(char));
}
//----------------------------------------------------------------------------
Texture2D::~Texture2D ()
{
	Renderer::UnbindAll(this);
}
//----------------------------------------------------------------------------
bool Texture2D::CanGenMinmaps () const
{
	if (GetWidth()==GetHeight() && IsPowerOfTwo(GetWidth()) && GetWidth()>=64)
		return true;

	return false;
}
//----------------------------------------------------------------------------
void Texture2D::GenerateMipmaps ()
{
	// 如果纹理级别数量不等于最大纹理级别数，将其从渲染器中释放，之后重新计算。
	unsigned int logDim0 = Log2OfPowerOfTwo((unsigned int)mDimension[0][0]);
	unsigned int logDim1 = Log2OfPowerOfTwo((unsigned int)mDimension[1][0]);
	int maxLevels = (int)(logDim0 >= logDim1 ? logDim0 + 1 : logDim1 + 1);
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
	Float4* rgba = new1<Float4>(mDimension[0][0]*mDimension[1][0]);

	char* texels = mData;
	int width = mDimension[0][0];
	int height = mDimension[1][0];
	int level;
	for (level = 1; level < mNumLevels; ++level)
	{
		char* texelsNext = mData + mLevelOffsets[level];
		int widthNext = mDimension[0][level];
		int heightNext = mDimension[1][level];

		GenerateNextMipmap(width, height, texels, widthNext, heightNext,
			texelsNext, rgba);

		width = widthNext;
		height = heightNext;
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
bool Texture2D::HasMipmaps () const
{
	unsigned int logDim0 = Log2OfPowerOfTwo((unsigned int)mDimension[0][0]);
	unsigned int logDim1 = Log2OfPowerOfTwo((unsigned int)mDimension[1][0]);
	int maxLevels = (int)(logDim0 >= logDim1 ? logDim0 + 1 : logDim1 + 1);
	return mNumLevels == maxLevels;
}
//----------------------------------------------------------------------------
char* Texture2D::GetData (int level) const
{
	if (mData && 0 <= level && level < mNumLevels)
	{
		return mData + mLevelOffsets[level];
	}

	assertion(false, "Null pointer or invalid level in GetData\n");
	return 0;
}
//----------------------------------------------------------------------------
void Texture2D::ComputeNumLevelBytes ()
{
	if (mFormat == TF_R16F
		|| mFormat == TF_G16R16F
		|| mFormat == TF_A16B16G16R16F)
	{
		if (mNumLevels > 1)
		{
			assertion(false, "No mipmaps for 32-bit float textures\n");
			mNumLevels = 1;
		}
	}
	else if (mFormat == TF_R32F
		||  mFormat == TF_G32R32F
		||  mFormat == TF_A32B32G32R32F)
	{
		if (mNumLevels > 1)
		{
			assertion(false, "No mipmaps for 32-bit float textures\n");
			mNumLevels = 1;
		}
	}
	else if (mFormat == TF_D24S8)
	{
		if (mNumLevels > 1)
		{
			assertion(false, "No mipmaps for 2D depth textures\n");
			mNumLevels = 1;
		}
	}

	int dim0 = mDimension[0][0];
	int dim1 = mDimension[1][0];
	int level;
	mNumTotalBytes = 0;

	if (mFormat == TF_DXT1)
	{
		for (level = 0; level < mNumLevels; ++level)
		{
			int max0 = dim0/4;
			if (max0 < 1)
			{
				max0 = 1;
			}
			int max1 = dim1/4;
			if (max1 < 1)
			{
				max1 = 1;
			}

			mNumLevelBytes[level] = 8*max0*max1;
			mNumTotalBytes += mNumLevelBytes[level];
			mDimension[0][level] = dim0;
			mDimension[1][level] = dim1;
			mDimension[2][level] = 1;

			if (dim0 > 1)
			{
				dim0 >>= 1;
			}
			if (dim1 > 1)
			{
				dim1 >>= 1;
			}
		}
	}
	else if (mFormat == TF_DXT3 || mFormat == TF_DXT5)
	{
		for (level = 0; level < mNumLevels; ++level)
		{
			int max0 = dim0/4;
			if (max0 < 1)
			{
				max0 = 1;
			}
			int max1 = dim1/4;
			if (max1 < 1)
			{
				max1 = 1;
			}

			mNumLevelBytes[level] = 16*max0*max1;
			mNumTotalBytes += mNumLevelBytes[level];
			mDimension[0][level] = dim0;
			mDimension[1][level] = dim1;
			mDimension[2][level] = 1;

			if (dim0 > 1)
			{
				dim0 >>= 1;
			}
			if (dim1 > 1)
			{
				dim1 >>= 1;
			}
		}
	}
	else if (mFormat==TF_RGB_PVRTC_2B || mFormat==TF_RGBA_PVRTC_2B)
	{
		for (level = 0; level < mNumLevels; ++level)
		{
			int blockSize = 8 * 4; // Pixel by pixel block size for 2bpp
			int widthBlocks = dim0 / 8;
			int heightBlocks = dim1 / 4;
			int bpp = 2;

			if (widthBlocks < 2)
			{
				widthBlocks = 2;
			}
			if (heightBlocks < 2)
			{
				heightBlocks = 2;
			}

			mNumLevelBytes[level] = widthBlocks * heightBlocks * ((blockSize  * bpp) / 8);;
			mNumTotalBytes += mNumLevelBytes[level];
			mDimension[0][level] = dim0;
			mDimension[1][level] = dim1;
			mDimension[2][level] = 1;

			if (dim0 > 1)
			{
				dim0 >>= 1;
			}
			if (dim1 > 1)
			{
				dim1 >>= 1;
			}
		}
	}
	else if (mFormat==TF_RGB_PVRTC_4B || mFormat==TF_RGBA_PVRTC_4B || mFormat==TF_RGB_ETC1)
	{
		for (level = 0; level < mNumLevels; ++level)
		{
			int blockSize = 4 * 4; // Pixel by pixel block size for 4bpp
			int widthBlocks = dim0 / 4;
			int heightBlocks = dim1 / 4;
			int bpp = 4;

			if (widthBlocks < 2)
			{
				widthBlocks = 2;
			}
			if (heightBlocks < 2)
			{
				heightBlocks = 2;
			}

			mNumLevelBytes[level] = widthBlocks * heightBlocks * ((blockSize  * bpp) / 8);;
			mNumTotalBytes += mNumLevelBytes[level];
			mDimension[0][level] = dim0;
			mDimension[1][level] = dim1;
			mDimension[2][level] = 1;

			if (dim0 > 1)
			{
				dim0 >>= 1;
			}
			if (dim1 > 1)
			{
				dim1 >>= 1;
			}
		}
	}
	else
	{
		for (level = 0; level < mNumLevels; ++level)
		{
			mNumLevelBytes[level] = msPixelSize[mFormat]*dim0*dim1;
			mNumTotalBytes += mNumLevelBytes[level];
			mDimension[0][level] = dim0;
			mDimension[1][level] = dim1;
			mDimension[2][level] = 1;

			if (dim0 > 1)
			{
				dim0 >>= 1;
			}
			if (dim1 > 1)
			{
				dim1 >>= 1;
			}
		}
	}

	mLevelOffsets[0] = 0;
	for (level = 0; level < mNumLevels-1; ++level)
	{
		mLevelOffsets[level+1] = mLevelOffsets[level] + mNumLevelBytes[level];
	}
}
//----------------------------------------------------------------------------
void Texture2D::GenerateNextMipmap (int width, int height, const char* texels,
									int widthNext, int heightNext, char* texelsNext, Float4* rgba)
{
	// 将原始格式转换到32-bitRGBA
	int numTexels = width*height;
	msConvertFrom[mFormat](numTexels, texels, rgba);

	// 创建miplevel
	for (int i1 = 0; i1 < heightNext; ++i1)
	{
		for (int i0 = 0; i0 < widthNext; ++i0)
		{
			int j = i0 + widthNext*i1;
			int base = 2*(i0 + width*i1);
			for (int c = 0; c < 4; ++c)
			{
				rgba[j][c] = 0.25f*(rgba[base][c] + rgba[base + 1][c]
				+ rgba[base + width][c] + rgba[base + width + 1][c]);
				rgba[j][c] = rgba[base][c];
			}
		}
	}

	// 将32-bit格式转换到原始格式
	int numTexelsNext = widthNext*heightNext;
	msConvertTo[mFormat](numTexelsNext, rgba, texelsNext);
}
//----------------------------------------------------------------------------