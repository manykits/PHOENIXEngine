// PX2FBXImporter.hpp

#ifndef PX2FBXIMPORTER_HPP
#define PX2FBXIMPORTER_HPP

#include "PX2FBXPre.hpp"
#include "PX2APoint.hpp"
#include "PX2AVector.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2Node.hpp"
#include "PX2FBXMesh.hpp"

namespace PX2
{

	class FBXIMPORTER_DLL_ITEM FBXImporter
	{
	public:
		FBXImporter();
		~FBXImporter();

		static FBXImporter* New();
		static void Delete(FBXImporter *importer);

		bool Import(const std::string &filename);
		void Clear();
		Node *GetPX2Node();

	protected:
		void ProcessSkeletonHierarchy(FbxNode* inRootNode);
		void ProcessSkeletonHierarchyRecursively(FbxNode* inNode, int inDepth, int myIndex, int inParentIndex);
		void ProcessGeometry(FbxNode* inNode);
		void ProcessControlPoints(FbxNode* inNode);
		void ProcessJointsAndAnimations(FbxNode* inNode, FbxMesh *mesh);
		unsigned int FindJointIndex(FbxNode *fbxNode);
		void ProcessMesh(FbxNode* inNode, FbxMesh *mesh);
		void AssociateMaterialToMesh(FbxNode* inNode, FbxMesh *mesh);
		void ProcessMaterials(FbxNode* inNode, FbxMesh *mesh);
		void ProcessMaterialAttribute(FbxSurfaceMaterial* inMaterial, unsigned int inMaterialIndex, FbxMesh *mesh);
		void ProcessMaterialTexture(FbxSurfaceMaterial* inMaterial, Mtl* ioMaterial);
		void Optimize();
		Node *ConvertToPX2();
		Node *PX2ProcessNode(FbxNode* fbxNode);
		void PX2ProcessAnimFrames();
		void PX2ProcessMesh(FbxNode* fbxNode);
		void PX2ProcessMtl(FbxNode* fbxNode);		
		Movable *PX2ConvertMeshs(FbxMesh* mesh);
		void PX2ProcessMtl( FbxMesh *fbxmesh);

		bool HasSkeleton();

		Skeleton mSkeleton;
		std::map<FbxMesh *, Mesh*> mMeshes;

	protected:
		APoint _CovertP(FbxDouble3 &val);
		AVector _CovertV(FbxDouble3 &val);
		APoint _ConvertP(FbxVector4 &val);
		AVector _ConvertV(FbxVector4 &val);

		VertexFormat *_GetVF(FbxMesh* fbxMesh);

		std::string mModelFilename;
		std::string mOutPathFilename;
		std::string mBaseFilename;

		FbxManager *mFbxManager;
		FbxImporter *mFbxImporter;
		FbxIOSettings *mFbxIOSettings;

		FbxScene *mFbxScene;

		NodePtr mPX2Scene;
		std::map<FbxNode*, Node*> mFbxNode2Nodes;
		std::map<FbxMesh*, std::vector<TriMesh*> > mFbxMesh2Meshes;
		std::map<TriMesh*, Mtl*> mTriMesh2Mtls;
	};

}

#endif