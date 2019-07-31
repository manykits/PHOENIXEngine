// PX2TriStrip.cpp

#include "PX2TriStrip.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Triangles, TriStrip);
PX2_IMPLEMENT_STREAM(TriStrip);
PX2_IMPLEMENT_FACTORY(TriStrip);
PX2_IMPLEMENT_DEFAULT_NAMES(Triangles, TriStrip);
PX2_IMPLEMENT_DEFAULT_STREAM(Triangles, TriStrip);

//----------------------------------------------------------------------------
TriStrip::TriStrip ()
	:
Triangles(PT_TRISTRIP)
{
}
//----------------------------------------------------------------------------
TriStrip::TriStrip (VertexFormat* vformat, VertexBuffer* vbuffer,
					int indexSize)
					:
Triangles(PT_TRISTRIP, vformat, vbuffer, 0)
{
	int numVertices = mVBuffer->GetNumElements();
	mIBuffer = new0 IndexBuffer(numVertices, indexSize);
	int i;

	if (indexSize == 2)
	{
		short* indices = (short*)mIBuffer->GetData();
		for (i = 0; i < numVertices; ++i)
		{
			indices[i] = (short)i;
		}
	}
	else // indexSize == 4
	{
		int* indices = (int*)mIBuffer->GetData();
		for (i = 0; i < numVertices; ++i)
		{
			indices[i] = i;
		}
	}
}
//----------------------------------------------------------------------------
TriStrip::TriStrip (VertexFormat* vformat, VertexBuffer* vbuffer,
					IndexBuffer* ibuffer)
					:
Triangles(PT_TRISTRIP, vformat, vbuffer, ibuffer)
{
	assertion(mIBuffer->GetNumElements() >= mVBuffer->GetNumElements(),
		"Not enough elements for the index buffer\n");
}
//----------------------------------------------------------------------------
TriStrip::~TriStrip ()
{
}
//----------------------------------------------------------------------------
void TriStrip::CalIndexs (int indexSize)
{
	int numVertices = mVBuffer->GetNumElements();
	mIBuffer = new0 IndexBuffer(numVertices, indexSize);
	int i;

	if (indexSize == 2)
	{
		short* indices = (short*)mIBuffer->GetData();
		for (i = 0; i < numVertices; ++i)
		{
			indices[i] = (short)i;
		}
	}
	else // indexSize == 4
	{
		int* indices = (int*)mIBuffer->GetData();
		for (i = 0; i < numVertices; ++i)
		{
			indices[i] = i;
		}
	}
}
//----------------------------------------------------------------------------
int TriStrip::GetNumTriangles () const
{
	return mIBuffer->GetNumElements() - 2;
}
//----------------------------------------------------------------------------
bool TriStrip::GetTriangle (int i, int& v0, int& v1, int& v2) const
{
	if (0 <= i && i < GetNumTriangles())
	{
		int* indices = (int*)mIBuffer->GetData();
		v0 = indices[i];
		if (i & 1)
		{
			v1 = indices[i + 2];
			v2 = indices[i + 1];
		}
		else
		{
			v1 = indices[i + 1];
			v2 = indices[i + 2];
		}

		return (v0 != v1 && v0 != v2 && v1 != v2);
	}
	return false;
}
//----------------------------------------------------------------------------