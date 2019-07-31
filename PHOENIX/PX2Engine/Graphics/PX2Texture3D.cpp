// PX2Texture3D.cpp

#include "PX2GraphicsPre.hpp"
#include "PX2Texture3D.hpp"
#include "PX2BitHacks.hpp"
#include "PX2Renderer.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Texture, Texture3D);
PX2_IMPLEMENT_STREAM(Texture3D);
PX2_IMPLEMENT_FACTORY(Texture3D);
PX2_IMPLEMENT_DEFAULT_NAMES(Texture, Texture3D);
PX2_IMPLEMENT_DEFAULT_STREAM(Texture, Texture3D);

//----------------------------------------------------------------------------
Texture3D::Texture3D (Format tformat, int dimension0, int dimension1,
    int dimension2, int numLevels, Buffer::Usage usage)
    :
    Texture(tformat, TT_3D, usage, numLevels)
{
    assertion(dimension0 > 0, "Dimension0 must be positive\n");
    assertion(dimension1 > 0, "Dimension1 must be positive\n");
    assertion(dimension2 > 0, "Dimension2 must be positive\n");
    mDimension[0][0] = dimension0;
    mDimension[1][0] = dimension1;
    mDimension[2][0] = dimension2;

    unsigned int logDim0 = Log2OfPowerOfTwo((unsigned int)dimension0);
    unsigned int logDim1 = Log2OfPowerOfTwo((unsigned int)dimension1);
    unsigned int logDim2 = Log2OfPowerOfTwo((unsigned int)dimension2);
    int maxLevels = logDim0;
    if ((int)logDim1 > maxLevels)
    {
        maxLevels = logDim1;
    }
    if ((int)logDim2 > maxLevels)
    {
        maxLevels = logDim2;
    }
    ++maxLevels;

    if (numLevels == 0)
    {
		// 使用最大级别
        mNumLevels = maxLevels;
    }
    else if (numLevels <= maxLevels)
    {
        // 使用用户想要的级别
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
Texture3D::~Texture3D ()
{
    Renderer::UnbindAll(this);
}
//----------------------------------------------------------------------------
void Texture3D::GenerateMipmaps ()
{
    // 如果纹理级别数量不等于最大纹理级别数，将其从渲染器中释放，之后重新计算。
    unsigned int logDim0 = Log2OfPowerOfTwo((unsigned int)mDimension[0][0]);
    unsigned int logDim1 = Log2OfPowerOfTwo((unsigned int)mDimension[1][0]);
    unsigned int logDim2 = Log2OfPowerOfTwo((unsigned int)mDimension[2][0]);
    int maxLevels = logDim0;
    if ((int)logDim1 > maxLevels)
    {
        maxLevels = logDim1;
    }
    if ((int)logDim2 > maxLevels)
    {
        maxLevels = logDim2;
    }
    ++maxLevels;

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

	// 生成mipmap的零时内存
    Float4* rgba = new1<Float4>(
        mDimension[0][0]*mDimension[1][0]*mDimension[2][0]);

    char* texels = mData;
    int width = mDimension[0][0];
    int height = mDimension[1][0];
    int thickness = mDimension[2][0];
    int level;
    for (level = 1; level < mNumLevels; ++level)
    {
        char* texelsNext = mData + mLevelOffsets[level];
        int widthNext = mDimension[0][level];
        int heightNext = mDimension[1][level];
        int thicknessNext = mDimension[2][level];

        GenerateNextMipmap(width, height, thickness, texels, widthNext,
            heightNext, thicknessNext, texelsNext, rgba);

        width = widthNext;
        height = heightNext;
        thickness = thicknessNext;
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
bool Texture3D::HasMipmaps () const
{
    unsigned int logDim0 = Log2OfPowerOfTwo((unsigned int)mDimension[0][0]);
    unsigned int logDim1 = Log2OfPowerOfTwo((unsigned int)mDimension[1][0]);
    unsigned int logDim2 = Log2OfPowerOfTwo((unsigned int)mDimension[2][0]);
    int maxLevels = logDim0;
    if ((int)logDim1 > maxLevels)
    {
        maxLevels = logDim1;
    }
    if ((int)logDim2 > maxLevels)
    {
        maxLevels = logDim2;
    }
    ++maxLevels;
    return mNumLevels == maxLevels;
}
//----------------------------------------------------------------------------
char* Texture3D::GetData (int level) const
{
    if (mData && 0 <= level && level < mNumLevels)
    {
        return mData + mLevelOffsets[level];
    }

    assertion(false, "Null pointer or invalid level in GetData\n");
    return 0;
}
//----------------------------------------------------------------------------
void Texture3D::ComputeNumLevelBytes ()
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
    int dim1 = mDimension[1][0];
    int dim2 = mDimension[2][0];
    mNumTotalBytes = 0;
    for (level = 0; level < mNumLevels; ++level)
    {
        mNumLevelBytes[level] = msPixelSize[mFormat]*dim0*dim1*dim2;
        mNumTotalBytes += mNumLevelBytes[level];
        mDimension[0][level] = dim0;
        mDimension[1][level] = dim1;
        mDimension[2][level] = dim2;

        if (dim0 > 1)
        {
            dim0 >>= 1;
        }
        if (dim1 > 1)
        {
            dim1 >>= 1;
        }
        if (dim2 > 1)
        {
            dim2 >>= 1;
        }
    }

    mLevelOffsets[0] = 0;
    for (level = 0; level < mNumLevels-1; ++level)
    {
        mLevelOffsets[level+1] = mLevelOffsets[level] + mNumLevelBytes[level];
    }
}
//----------------------------------------------------------------------------
void Texture3D::GenerateNextMipmap (int width, int height, int thickness,
    const char* texels, int widthNext, int heightNext, int thicknessNext,
    char* texelsNext, Float4* rgba)
{
    // Convert the texels from native format to 32-bit RGBA.
    int widthHeight = width*height;
    int numTexels = widthHeight*thickness;
    msConvertFrom[mFormat](numTexels, texels, rgba);

    // Create the next miplevel in-place.
    for (int i2 = 0; i2 < thicknessNext; ++i2)
    {
        for (int i1 = 0; i1 < heightNext; ++i1)
        {
            for (int i0 = 0; i0 < widthNext; ++i0)
            {
                int j = i0 + widthNext*(i1 + heightNext*i2);
                int base = 2*(i0 + width*(i1 + height*i2));
                for (int c = 0; c < 4; ++c)
                {
                    rgba[j][c] = 0.125f*(
                        rgba[base][c] +
                        rgba[base + 1][c] +
                        rgba[base + width][c] +
                        rgba[base + width + 1][c] +
                        rgba[base + widthHeight][c] +
                        rgba[base + 1 + widthHeight][c] +
                        rgba[base + width + widthHeight][c] +
                        rgba[base + width + 1 + widthHeight][c]
                    );
                }
            }
        }
    }

    // Convert the texels from 32-bit RGBA to native format.
    int numTexelsNext = widthNext*heightNext*thicknessNext;
    msConvertTo[mFormat](numTexelsNext, rgba, texelsNext);
}
//----------------------------------------------------------------------------
