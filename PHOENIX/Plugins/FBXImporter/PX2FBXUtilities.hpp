// PX2FBXUtilities.hpp

#ifndef PX2FBXUTILITIES_HPP
#define PX2FBXUTILITIES_HPP

#include "PX2UnityPre.hpp"
#include "PX2APoint.hpp"
#include "PX2HMatrix.hpp"
#include "PX2FBXPre.hpp"

namespace PX2
{

	struct FBXIMPORTER_DLL_ITEM BlendingPair
	{
		unsigned int BoneIndex;
		double BoneWeight;

		BlendingPair() :
			BoneIndex(0),
			BoneWeight(0)
		{}
	};

	struct FBXIMPORTER_DLL_ITEM CtrlPoint
	{
		APoint mPosition;
		std::vector<BlendingPair> mBlendingInfo;

		CtrlPoint()
		{
			mBlendingInfo.reserve(4);
		}
	};

	struct FBXIMPORTER_DLL_ITEM Keyframe
	{
		FbxLongLong mFrameIndex;
		
		FbxVector4 mScale;
		FbxVector4 mRotate;
		FbxVector4 mTranslate;

		Keyframe* mNext;

		Keyframe() :
			mNext(nullptr)
		{}
	};

	struct FBXIMPORTER_DLL_ITEM AnimInfo
	{
		float start;
		float stop;
		float framerate;
		bool translate;
		bool rotate;
		bool scale;

		AnimInfo() :
			start(FLT_MAX), 
			stop(-1.f),
			framerate(0.f), 
			translate(false), 
			rotate(false), 
			scale(false) 
		{
		}

		inline AnimInfo& operator += (const AnimInfo& rhs)
		{
			start = std::min(rhs.start, start);
			stop = std::max(rhs.stop, stop);
			framerate = std::max(rhs.framerate, framerate);
			translate = translate || rhs.translate;
			rotate = rotate || rhs.rotate;
			scale = scale || rhs.scale;

			return *this;
		}
	};


	struct FBXIMPORTER_DLL_ITEM Joint
	{
		int mParentIndex;

		HMatrix TMMatrix;

		Keyframe* mAnimation;
		FbxNode* mNode;
	};

	struct FBXIMPORTER_DLL_ITEM Skeleton
	{
		bool HasJoint(FbxNode *fbxNode)
		{
			for (int i = 0; i < (int)mJoints.size(); i++)
			{
				if (fbxNode == mJoints[i].mNode)
					return true;
			}

			return false;
		}

		std::vector<Joint> mJoints;
	};

	struct FBXIMPORTER_DLL_ITEM Triangle
	{
		std::vector<unsigned int> mIndices;
		std::string mMaterialName;
		unsigned int mMaterialIndex;

		bool operator<(const Triangle& rhs)
		{
			return mMaterialIndex < rhs.mMaterialIndex;
		}
	};

	class FBXIMPORTER_DLL_ITEM Utilities
	{
	public:
		static FbxAMatrix GetGeometryTransformation(FbxNode* inNode);
		static HMatrix FbxMatrixToPX2(FbxAMatrix &fbxMat);

		static std::string GetFileName(const std::string& inInput);
	};


}

#endif