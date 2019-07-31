// PX2AIMeshUtilities.cpp

#include "PX2AIMeshUtilities.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
RawMesh::RawMesh(
	float* const vertexBuffer,
	const size_t vertexBufferSize,
	int* const indexBuffer,
	const size_t indexBufferSize,
	const Vector3f& position,
	const HQuaternion& orientation,
	const Vector3f& scale)
	:
	vertexBuffer(vertexBuffer),
	vertexBufferSize(vertexBufferSize),
	vertexCount(0),
	indexBuffer(indexBuffer),
	indexBufferSize(indexBufferSize),
	indexCount(0),
	position(position),
	orientation(orientation),
	scale(scale)
{
	assert(vertexBuffer);
	assert(indexBuffer);
	assert(vertexBufferSize);
	assert(indexBufferSize);
}
//----------------------------------------------------------------------------
RawMesh::RawMesh(const RawMesh& mesh)
	: vertexBuffer(mesh.vertexBuffer),
	vertexBufferSize(mesh.vertexBufferSize),
	vertexCount(mesh.vertexCount),
	indexBuffer(mesh.indexBuffer),
	indexBufferSize(mesh.indexBufferSize),
	indexCount(mesh.indexCount),
	position(mesh.position),
	orientation(mesh.orientation),
	scale(mesh.scale)
{
}
//----------------------------------------------------------------------------
bool MeshUtilities::ConvertToRawMesh(TriMesh& mesh, RawMesh& rawMesh)
{
	bool addedShared = false;
	size_t currentOffset = 0;
	size_t sharedOffset = 0;
	size_t nextOffset = 0;
	size_t index_offset = 0;

	size_t vertexCount = 0;
	size_t indexCount = 0;

	GetMeshInformation(mesh, vertexCount, indexCount);

	assert(rawMesh.vertexBufferSize >= (vertexCount * 3));
	assert(rawMesh.indexBufferSize >= indexCount);

	if (rawMesh.vertexBufferSize < vertexCount * 3 ||
		rawMesh.indexBufferSize < indexCount)
	{
		return false;
	}

	VertexBufferAccessor vba(&mesh);
	for (int i = 0; i < vba.GetNumVertices(); i++)
	{
		Float3 pos = vba.Position<Float3>(i);

		const size_t bufferOffset = i * 3;

		rawMesh.vertexBuffer[bufferOffset] = pos.X();
		rawMesh.vertexBuffer[bufferOffset + 1] = pos.Y();
		rawMesh.vertexBuffer[bufferOffset + 2] = pos.Z();
	}

	for (int i = 0; i < mesh.GetNumTriangles(); i++)
	{
		const size_t bufferOffset = i * 3;

		int v0 = 0;
		int v1 = 1;
		int v2 = 2;
		if (mesh.GetTriangle(i, v0, v1, v2))
		{
			rawMesh.indexBuffer[bufferOffset] = v0;
			rawMesh.indexBuffer[bufferOffset+1] = v1;
			rawMesh.indexBuffer[index_offset+2] = v2;
		}
	}

	return true;
}
//----------------------------------------------------------------------------
void MeshUtilities::GetMeshInformation(const TriMesh& mesh, 
	size_t& vertexCount, size_t& indexCount)
{
	vertexCount = mesh.GetVertexBuffer()->GetNumElements();
	indexCount = mesh.GetIndexBuffer()->GetNumElements();
}
//----------------------------------------------------------------------------