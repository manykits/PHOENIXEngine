// PX2FBXMesh.hpp

#ifndef PX2FBXMESH_HPP
#define PX2FBXMESH_HPP

#include "PX2FBXPre.hpp"
#include "PX2FBXVertex.hpp"
#include "PX2FBXUtilities.hpp"
#include "PX2FBXMat.hpp"

namespace PX2
{

	struct FBXIMPORTER_DLL_ITEM Mesh
	{
		Mesh()
		{

		}
		~Mesh()
		{
			mTriangles.clear();
			mVertices.clear();
			for (auto itr = mMaterialLookUp.begin(); itr != mMaterialLookUp.end(); ++itr)
			{
				delete itr->second;
			}
			mMaterialLookUp.clear();
		}

		FbxMesh *mFbxMesh;

		std::unordered_map<unsigned int, CtrlPoint*> mControlPoints;

		int mFbxMeshTriangleCount;
		std::vector<PNTIWVertex> mVertices;
		std::vector<Triangle> mTriangles;
		std::unordered_map<unsigned int, Mtl*> mMaterialLookUp;

		void Optimize()
		{
			std::vector<PNTIWVertex> uniqueVertices;
			for (unsigned int i = 0; i < mTriangles.size(); ++i)
			{
				for (unsigned int j = 0; j < 3; ++j)
				{
					if (FindVertex(mVertices[i * 3 + j], uniqueVertices) == -1)
					{
						uniqueVertices.push_back(mVertices[i * 3 + j]);
					}
				}
			}

			for (unsigned int i = 0; i < mTriangles.size(); ++i)
			{
				for (unsigned int j = 0; j < 3; ++j)
				{
					mTriangles[i].mIndices[j] =
						FindVertex(mVertices[i * 3 + j], uniqueVertices);
				}
			}

			mVertices.clear();
			mVertices = uniqueVertices;
			uniqueVertices.clear();

			std::sort(mTriangles.begin(), mTriangles.end());
		}
	
		int FindVertex(const PNTIWVertex& inTargetVertex,
			const std::vector<PNTIWVertex>& uniqueVertices)
		{
			for (unsigned int i = 0; i < uniqueVertices.size(); ++i)
			{
				if (inTargetVertex == uniqueVertices[i])
				{
					return i;
				}
			}

			return -1;
		}
	};

}

#endif