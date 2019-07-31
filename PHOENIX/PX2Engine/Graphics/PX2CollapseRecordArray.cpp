// PX2CollapseRecordArray.cpp

#include "PX2CollapseRecordArray.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Object, CollapseRecordArray);
PX2_IMPLEMENT_STREAM(CollapseRecordArray);
PX2_IMPLEMENT_FACTORY(CollapseRecordArray);
PX2_IMPLEMENT_DEFAULT_NAMES(Object, CollapseRecordArray);

//----------------------------------------------------------------------------
CollapseRecordArray::CollapseRecordArray (int numRecords,
										  CollapseRecord* records)
										  :
mNumRecords(numRecords),
mRecords(records)
{
}
//----------------------------------------------------------------------------
CollapseRecordArray::~CollapseRecordArray ()
{
	delete1(mRecords);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
CollapseRecordArray::CollapseRecordArray (LoadConstructor value)
:
Object(value),
mNumRecords(0),
mRecords(0)
{
}
//----------------------------------------------------------------------------
void CollapseRecordArray::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Object::Load(source);
	PX2_VERSION_LOAD(source);

	source.Read(mNumRecords);
	mRecords = new1<CollapseRecord>(mNumRecords);

	for (int i = 0; i < mNumRecords; ++i)
	{
		source.Read(mRecords[i].VKeep);
		source.Read(mRecords[i].VThrow);
		source.Read(mRecords[i].NumVertices);
		source.Read(mRecords[i].NumTriangles);
		source.Read(mRecords[i].NumIndices);
		if (mRecords[i].NumIndices > 0)
		{
			mRecords[i].Indices = new1<int>(mRecords[i].NumIndices);
			source.ReadVR(mRecords[i].NumIndices, mRecords[i].Indices);
		}
		else
		{
			mRecords[i].Indices = 0;
		}
	}

	PX2_END_DEBUG_STREAM_LOAD(CollapseRecordArray, source);
}
//----------------------------------------------------------------------------
void CollapseRecordArray::Link (InStream& source)
{
	Object::Link(source);
}
//----------------------------------------------------------------------------
void CollapseRecordArray::PostLink ()
{
	Object::PostLink();
}
//----------------------------------------------------------------------------
bool CollapseRecordArray::Register (OutStream& target) const
{
	return Object::Register(target);
}
//----------------------------------------------------------------------------
void CollapseRecordArray::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Object::Save(target);
	PX2_VERSION_SAVE(target);

	target.Write(mNumRecords);
	for (int i = 0; i < mNumRecords; ++i)
	{
		target.Write(mRecords[i].VKeep);
		target.Write(mRecords[i].VThrow);
		target.Write(mRecords[i].NumVertices);
		target.Write(mRecords[i].NumTriangles);
		target.Write(mRecords[i].NumIndices);
		if (mRecords[i].Indices)
		{
			target.WriteN(mRecords[i].NumIndices, mRecords[i].Indices);
		}
	}

	PX2_END_DEBUG_STREAM_SAVE(CollapseRecordArray, target);
}
//----------------------------------------------------------------------------
int CollapseRecordArray::GetStreamingSize (Stream &stream) const
{
	int size = Object::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	size += sizeof(mNumRecords);
	for (int i = 0; i < mNumRecords; ++i)
	{
		size += sizeof(mRecords[i].VKeep);
		size += sizeof(mRecords[i].VThrow);
		size += sizeof(mRecords[i].NumVertices);
		size += sizeof(mRecords[i].NumIndices);
		size += sizeof(mRecords[i].Indices);
		size += mRecords[i].NumIndices*sizeof(mRecords[i].Indices[0]);
	}
	return size;
}
//----------------------------------------------------------------------------
