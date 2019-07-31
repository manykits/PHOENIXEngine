// PX2AIMeshUtilities.hpp

#ifndef PX2AIMESHUTILITIES_HPP
#define PX2AIMESHUTILITIES_HPP

#include "PX2CorePre.hpp"
#include "PX2Vector3.hpp"
#include "PX2HQuaternion.hpp"
#include "PX2TriMesh.hpp"

namespace PX2
{

	struct RawMesh
	{
		float* const vertexBuffer;
		const size_t vertexBufferSize;
		size_t vertexCount;

		int* const indexBuffer;
		const size_t indexBufferSize;
		size_t indexCount;

		const Vector3f& position;
		const HQuaternion& orientation;
		const Vector3f& scale;

		RawMesh(
			float* const vertexBuffer,
			const size_t vertexBufferSize,
			int* const indexBuffer,
			const size_t indexBufferSize,
			const Vector3f& position = Vector3f::ZERO,
			const HQuaternion& orientation = HQuaternion::IDENTITY,
			const Vector3f& scale = Vector3f::UNIT);

		RawMesh(const RawMesh& mesh);

	private:
		RawMesh& operator=(const RawMesh& mesh);
	};

	class MeshUtilities
	{
	public:
		static bool ConvertToRawMesh(
			TriMesh& mesh, RawMesh& rawMesh);

		static void GetMeshInformation(
			const TriMesh& mesh, size_t& vertexCount, size_t& indexCount);

	private:
		MeshUtilities();
		MeshUtilities(const MeshUtilities&);
		~MeshUtilities();
		MeshUtilities& operator=(const MeshUtilities&);
	};


}

#endif