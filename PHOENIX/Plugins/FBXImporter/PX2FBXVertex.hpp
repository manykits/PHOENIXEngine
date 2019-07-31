// PX2FBXVertex.hpp

#ifndef PX2FBXVERTEX_HPP
#define PX2FBXVERTEX_HPP

#include "PX2UnityPre.hpp"
#include "PX2APoint.hpp"
#include "PX2AVector.hpp"
#include "PX2Float2.hpp"
#include "PX2FBXPre.hpp"

namespace PX2
{

	struct FBXIMPORTER_DLL_ITEM PNTVertex
	{
		APoint mPosition;
		AVector mNormal;
		Float2 mUV;

		bool operator==(const PNTVertex& rhs) const
		{
			return mPosition == rhs.mPosition &&
				mNormal == rhs.mNormal &&
				mUV == rhs.mUV;
		}
	};

	struct FBXIMPORTER_DLL_ITEM VertexBlendingInfo
	{
		unsigned int BoneIndex;
		double BoneWeight;

		VertexBlendingInfo() :
			BoneIndex(0),
			BoneWeight(0.0)
		{}

		bool operator < (const VertexBlendingInfo& rhs)
		{
			return (BoneWeight > rhs.BoneWeight);
		}
	};

	struct FBXIMPORTER_DLL_ITEM PNTIWVertex
	{
		APoint mPosition;
		Float4 mColor;
		AVector mNormal;
		Float2 mUV;
		std::vector<VertexBlendingInfo> mVertexBlendingInfos;

		void SortBlendingInfoByWeight()
		{
			std::sort(mVertexBlendingInfos.begin(), mVertexBlendingInfos.end());
		}

		bool operator==(const PNTIWVertex& rhs) const
		{
			bool sameBlendingInfo = true;
			if (!(mVertexBlendingInfos.empty() && rhs.mVertexBlendingInfos.empty()))
			{
				for (unsigned int i = 0; i < 4; ++i)
				{
					if (mVertexBlendingInfos[i].BoneIndex != rhs.mVertexBlendingInfos[i].BoneIndex ||
						abs(mVertexBlendingInfos[i].BoneWeight - rhs.mVertexBlendingInfos[i].BoneWeight) > 0.001)
					{
						sameBlendingInfo = false;
						break;
					}
				}
			}

			bool result1 = (mPosition - rhs.mPosition).SquaredLength() <= 0.00001f;
			bool result2 = (mNormal - rhs.mNormal).SquaredLength() <= 0.00001f;
			bool result3 = ((mUV[0] - rhs.mUV[0])*(mUV[0] - rhs.mUV[0]) + (mUV[1] - rhs.mUV[1])*(mUV[1] - rhs.mUV[1])) <= 0.00001f;

			return result1 && result2 && result3 && sameBlendingInfo;
		}
	};

}

#endif
