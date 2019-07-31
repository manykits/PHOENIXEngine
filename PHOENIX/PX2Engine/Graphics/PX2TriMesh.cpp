// PX2TriMesh.cpp

#include "PX2TriMesh.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Triangles, TriMesh);
PX2_IMPLEMENT_STREAM(TriMesh);
PX2_IMPLEMENT_FACTORY(TriMesh);
PX2_IMPLEMENT_DEFAULT_NAMES(Triangles, TriMesh);
PX2_IMPLEMENT_DEFAULT_STREAM(Triangles, TriMesh);

//----------------------------------------------------------------------------
TriMesh::TriMesh ()
:
Triangles(PT_TRIMESH)
{
}
//----------------------------------------------------------------------------
TriMesh::TriMesh (VertexFormat* vformat, VertexBuffer* vbuffer,
				  IndexBuffer* ibuffer)
				  :
Triangles(PT_TRIMESH, vformat, vbuffer, ibuffer)
{
}
//----------------------------------------------------------------------------
TriMesh::~TriMesh ()
{
}
//----------------------------------------------------------------------------
int TriMesh::GetNumTriangles () const
{
	int ibNumElements = GetIBNumElements();
	if (0 != ibNumElements)
	{
		return ibNumElements/3;
	}
	else
	{
		return mIBuffer->GetNumElements()/3;
	}
}
//----------------------------------------------------------------------------
bool TriMesh::GetTriangle (int i, int& v0, int& v1, int& v2) const
{
	int elementSize = mIBuffer->GetElementSize();

	if (4 == elementSize)
	{
		if (0 <= i && i < GetNumTriangles())
		{
			int* indices = 3*i + (int*)mIBuffer->GetData();
			v0 = *indices++;
			v1 = *indices++;
			v2 = *indices;
			return true;
		}
	}
	else if (2 == elementSize)
	{
		if (0 <= i && i < GetNumTriangles())
		{
			unsigned short* indices = 3*i + (unsigned short*)mIBuffer->GetData();
			v0 = (int)(*indices++);
			v1 = (int)(*indices++);
			v2 = (int)(*indices);
			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
