// PX2Texture.cpp

#include "PX2Texture.hpp"
#include "PX2Texture1D.hpp"
#include "PX2Texture2D.hpp"
#include "PX2Texture3D.hpp"
#include "PX2TextureCube.hpp"
#include "PX2BitHacks.hpp"
#include "PX2Color.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Object, Texture);
PX2_IMPLEMENT_STREAM(Texture);
PX2_IMPLEMENT_ABSTRACT_FACTORY(Texture);
PX2_IMPLEMENT_DEFAULT_NAMES(Object, Texture);

int Texture::msNumDimensions[TT_QUANTITY] =
{
	1,  // TT_1D
	2,  // TT_2D
	3,  // TT_3D
	2   // TT_CUBE
};

int Texture::msPixelSize[TF_QUANTITY] =
{
	0,   // TF_NONE
	2,   // TF_R5G6B5
	2,   // TF_A1R5G5B5
	2,   // TF_A4R4G4B4
	1,   // TF_A8
	1,   // TF_L8
	2,   // TF_A8L8
	3,   // TF_R8G8B8
	4,   // TF_A8R8G8B8
	4,   // TF_A8B8G8R8
	2,   // TF_L16
	4,   // TF_G16R16
	8,   // TF_A16B16G16R16
	2,   // TF_R16F
	4,   // TF_G16R16F
	8,   // TF_A16B16G16R16F
	4,   // TF_R32F
	8,   // TF_G32R32F
	16,  // TF_A32B32G32R32F,
	0,   // TF_DXT1 (special handling)
	0,   // TF_DXT3 (special handling)
	0,   // TF_DXT5 (special handling)
	2,	 // TF_D16,
	4    // TF_D24S8
};

bool Texture::msMipmapable[TF_QUANTITY] =
{
	false,  // TF_NONE
	true,   // TF_R5G6B5
	true,   // TF_A1R5G5B5
	true,   // TF_A4R4G4B4
	true,   // TF_A8
	true,   // TF_L8
	true,   // TF_A8L8
	true,   // TF_R8G8B8
	true,   // TF_A8R8G8B8
	true,   // TF_A8B8G8R8
	true,   // TF_L16
	true,   // TF_G16R16
	true,   // TF_A16B16G16R16
	true,   // TF_R16F
	true,   // TF_G16R16F
	true,   // TF_A16B16G16R16F
	false,  // TF_R32F
	false,  // TF_G32R32F
	false,  // TF_A32B32G32R32F,
	true,   // TF_DXT1 (special handling)
	true,   // TF_DXT3 (special handling)
	true,   // TF_DXT5 (special handling)
	false,	// TF_D16
	false   // TF_D24S8
};

Texture::ConvertFrom Texture::msConvertFrom[TF_QUANTITY] =
{
	0,                                    // TF_NONE
	&Color::ConvertFromR5G6B5,            // TF_R5G6B5
	&Color::ConvertFromA1R5G5B5,          // TF_A1R5G5B5
	&Color::ConvertFromA4R4G4B4,          // TF_A4R4G4B4
	&Color::ConvertFromA8,                // TF_A8
	&Color::ConvertFromL8,                // TF_L8
	&Color::ConvertFromA8L8,              // TF_A8L8
	&Color::ConvertFromR8G8B8,            // TF_R8G8B8
	&Color::ConvertFromA8R8G8B8,          // TF_A8R8G8B8
	&Color::ConvertFromA8B8G8R8,          // TF_A8B8G8R8
	&Color::ConvertFromL16,               // TF_L16
	&Color::ConvertFromG16R16,            // TF_G16R16
	&Color::ConvertFromA16B16G16R16,      // TF_A16B16G16R16
	&Color::ConvertFromR16F,              // TF_R16F
	&Color::ConvertFromG16R16F,           // TF_G16R16F
	&Color::ConvertFromA16B16G16R16F,     // TF_A16B16G16R16F
	&Color::ConvertFromR32F,              // TF_R32F
	&Color::ConvertFromG32R32F,           // TF_G32R32F
	&Color::ConvertFromA32B32G32R32F,     // TF_A32B32G32R32F
	0,                                    // TF_DXT1
	0,                                    // TF_DXT3
	0,                                    // TF_DXT5
	0,									  // TF_D16
	0                                     // TF_D24S8
};

Texture::ConvertTo Texture::msConvertTo[TF_QUANTITY] =
{
	0,                                    // TF_NONE
	&Color::ConvertToR5G6B5,              // TF_R5G6B5
	&Color::ConvertToA1R5G5B5,            // TF_A1R5G5B5
	&Color::ConvertToA4R4G4B4,            // TF_A4R4G4B4
	&Color::ConvertToA8,                  // TF_A8
	&Color::ConvertToL8,                  // TF_L8
	&Color::ConvertToA8L8,                // TF_A8L8
	&Color::ConvertToR8G8B8,              // TF_R8G8B8
	&Color::ConvertToA8R8G8B8,            // TF_A8R8G8B8
	&Color::ConvertToA8B8G8R8,            // TF_A8B8G8R8
	&Color::ConvertToL16,                 // TF_L16
	&Color::ConvertToG16R16,              // TF_G16R16
	&Color::ConvertToA16B16G16R16,        // TF_A16B16G16R16
	&Color::ConvertToR16F,                // TF_R16F
	&Color::ConvertToG16R16F,             // TF_G16R16F
	&Color::ConvertToA16B16G16R16F,       // TF_A16B16G16R16F
	&Color::ConvertToR32F,                // TF_R32F
	&Color::ConvertToG32R32F,             // TF_G32R32F
	&Color::ConvertToA32B32G32R32F,       // TF_A32B32G32R32F
	0,                                    // TF_DXT1
	0,                                    // TF_DXT3
	0,                                    // TF_DXT5
	0,									  // TF_D16
	0                                     // TF_D24S8
};

//----------------------------------------------------------------------------
Texture::Texture (Format tformat, Type type, Buffer::Usage usage,
				  int numLevels)
				  :
mFormat(tformat),
mType(type),
mUsage(usage),
mNumLevels(numLevels),
mNumDimensions(msNumDimensions[type]),
mNumTotalBytes(0),
mData(0),
PdrPointer(0)
{
	for (int level = 0; level < MM_MAX_MIPMAP_LEVELS; ++level)
	{
		mDimension[0][level] = 0;
		mDimension[1][level] = 0;
		mDimension[2][level] = 0;
		mNumLevelBytes[level] = 0;
		mLevelOffsets[level] = 0;
	}

	for (int i = 0; i < MAX_USER_FIELDS; ++i)
	{
		mUserField[i] = 0;
	}

#if defined(_WIN32) || defined(WIN32)
	mIsBindDoClearData = true;
#else
	mIsBindDoClearData = true;
#endif
}
//----------------------------------------------------------------------------
Texture::~Texture ()
{
	delete1(mData);
}
//----------------------------------------------------------------------------
void Texture::SetBindDoClearData (bool doClearData)
{
	mIsBindDoClearData = doClearData;
}
//----------------------------------------------------------------------------
void Texture::ClearData ()
{
	delete1(mData);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
Texture::Texture (LoadConstructor value) :
Object(value),
mFormat(TF_NONE),
mType(TT_QUANTITY),
mUsage(Buffer::BU_QUANTITY),
mNumLevels(0),
mNumDimensions(0),
mData(0),
PdrPointer(0)
{
	for (int level = 0; level < MM_MAX_MIPMAP_LEVELS; ++level)
	{
		mNumLevelBytes[level] = 0;
		mDimension[0][level] = 0;
		mDimension[1][level] = 0;
		mDimension[2][level] = 0;
		mLevelOffsets[level] = 0;
	}

	for (int i = 0; i < MAX_USER_FIELDS; ++i)
	{
		mUserField[i] = 0;
	}

#if defined(_WIN32) || defined(WIN32)
	mIsBindDoClearData = true;
#else
	mIsBindDoClearData = true;
#endif
}
//----------------------------------------------------------------------------
void Texture::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Object::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadEnum(mFormat);
	source.ReadEnum(mType);
	source.ReadEnum(mUsage);
	source.Read(mNumLevels);
	source.Read(mNumDimensions);
	source.ReadVV(MM_MAX_MIPMAP_LEVELS, mDimension[0]);
	source.ReadVV(MM_MAX_MIPMAP_LEVELS, mDimension[1]);
	source.ReadVV(MM_MAX_MIPMAP_LEVELS, mDimension[2]);
	source.ReadVV(MM_MAX_MIPMAP_LEVELS, mNumLevelBytes);
	source.Read(mNumTotalBytes);
	source.ReadVV(MM_MAX_MIPMAP_LEVELS, mLevelOffsets);
	source.ReadVV(MAX_USER_FIELDS, mUserField);
	source.ReadVR(mNumTotalBytes, mData);

	PX2_END_DEBUG_STREAM_LOAD(Texture, source);
}
//----------------------------------------------------------------------------
void Texture::Link (InStream& source)
{
	Object::Link(source);
}
//----------------------------------------------------------------------------
void Texture::PostLink ()
{
	Object::PostLink();
}
//----------------------------------------------------------------------------
bool Texture::Register (OutStream& target) const
{
	return Object::Register(target);
}
//----------------------------------------------------------------------------
void Texture::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Object::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteEnum(mFormat);
	target.WriteEnum(mType);
	target.WriteEnum(mUsage);
	target.Write(mNumLevels);
	target.Write(mNumDimensions);
	target.WriteN(MM_MAX_MIPMAP_LEVELS, mDimension[0]);
	target.WriteN(MM_MAX_MIPMAP_LEVELS, mDimension[1]);
	target.WriteN(MM_MAX_MIPMAP_LEVELS, mDimension[2]);
	target.WriteN(MM_MAX_MIPMAP_LEVELS, mNumLevelBytes);
	target.Write(mNumTotalBytes);
	target.WriteN(MM_MAX_MIPMAP_LEVELS, mLevelOffsets);
	target.WriteN(MAX_USER_FIELDS, mUserField);
	target.WriteN(mNumTotalBytes, mData);

	PX2_END_DEBUG_STREAM_SAVE(Texture, target);
}
//----------------------------------------------------------------------------
int Texture::GetStreamingSize (Stream &stream) const
{
	int size = Object::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	size += PX2_ENUMSIZE(mFormat);
	size += PX2_ENUMSIZE(mType);
	size += PX2_ENUMSIZE(mUsage);
	size += sizeof(mNumLevels);
	size += sizeof(mNumDimensions);
	size += MM_MAX_MIPMAP_LEVELS*sizeof(mDimension[0][0]);
	size += MM_MAX_MIPMAP_LEVELS*sizeof(mDimension[1][0]);
	size += MM_MAX_MIPMAP_LEVELS*sizeof(mDimension[2][0]);
	size += MM_MAX_MIPMAP_LEVELS*sizeof(mNumLevelBytes[0]);
	size += sizeof(mNumTotalBytes);
	size += MM_MAX_MIPMAP_LEVELS*sizeof(mLevelOffsets[0]);
	size += MAX_USER_FIELDS*sizeof(mUserField[0]);
	size += mNumTotalBytes;
	return size;
}
//----------------------------------------------------------------------------