// PX2VoxelBlock.cpp

#include "PX2VoxelBlock.hpp"
#include "PX2VoxelManager.hpp"
#include "PX2Time.hpp"
#include "PX2VoxelDefine.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
VoxelBlock::VoxelBlock() :
IndexX(-1),
IndexY(-1),
IndexZ(-1),
MtlType(1),
Left(0),
Right(0),
Front(0),
Back(0),
Bottom(0),
Top(0)
{
	Color = Float4::WHITE;
}
//----------------------------------------------------------------------------
VoxelBlock::~VoxelBlock()
{
}
//----------------------------------------------------------------------------
void VoxelBlock::CalNumFace(int &num)
{
	if (0 == MtlType) return;

	if (!Left)
	{
		++num;
	}
	else if (Left && 0 == Left->MtlType)
	{
		++num;
	}

	if (!Right)
	{
		++num;
	}
	else if (Right && 0 == Right->MtlType)
	{
		++num;
	}

	if (!Front)
	{
		++num;
	}
	else if (Front && 0 == Front->MtlType)
	{
		++num;
	}

	if (!Back)
	{
		++num;
	}
	else if (Back && 0 == Back->MtlType)
	{
		++num;
	}

	if (!Bottom)
	{
		++num;
	}
	else if (Bottom && 0 == Bottom->MtlType)
	{
		++num;
	}

	if (!Top)
	{
		++num;
	}
	else if (Top && 0 == Top->MtlType)
	{
		++num;
	}
}
//----------------------------------------------------------------------------
void VoxelBlock::GenMesh(VertexBufferAccessor &vba, IndexBuffer *ib,
	int &startFace)
{
	int startVertex = startFace * 4;
	unsigned short startIndex = (unsigned short)(startFace * 6);
	unsigned short* indices = (unsigned short*)ib->GetData();

	if (0 == MtlType) 
		return;

	AVector offset((float)IndexX*V_SIZE, (float)IndexY*V_SIZE, (float)IndexZ*V_SIZE);

	int startVertex1 = startVertex;
	int startIndex1 = startIndex;
	if (!Left || (Left && 0 == Left->MtlType))
	{
		vba.Position<Float3>(startVertex1 + 0) = APoint(0.0f, V_SIZE, 0.0f) + offset;
		vba.Position<Float3>(startVertex1 + 1) = APoint(0.0f, 0.0f, 0.0f) + offset;
		vba.Position<Float3>(startVertex1 + 2) = APoint(0.0f, V_SIZE, V_SIZE) + offset;
		vba.Position<Float3>(startVertex1 + 3) = APoint(0.0f, 0.0f, V_SIZE) + offset;

		if (vba.HasNormal())
		{
			vba.Normal<Float3>(startVertex1 + 0) = Float3(-1.0f, 0.0f, 0.0f);
			vba.Normal<Float3>(startVertex1 + 1) = Float3(-1.0f, 0.0f, 0.0f);
			vba.Normal<Float3>(startVertex1 + 2) = Float3(-1.0f, 0.0f, 0.0f);
			vba.Normal<Float3>(startVertex1 + 3) = Float3(-1.0f, 0.0f, 0.0f);
		}

		if (vba.HasColor(0))
		{
			vba.Color<Float4>(0, startVertex1 + 0) = Color;
			vba.Color<Float4>(0, startVertex1 + 1) = Color;
			vba.Color<Float4>(0, startVertex1 + 2) = Color;
			vba.Color<Float4>(0, startVertex1 + 3) = Color;
		}

		if (vba.HasTCoord(0))
		{
			VoxelBlockMtl *vbm = PX2_VOXELM.GetVoxelBlockMtlByType(MtlType);
			std::string faceName = vbm->Left;
			if (Top && 0 != Top->MtlType)
			{
				faceName = vbm->Bottom;
			}
			const TexPackElement &ele = PX2_VOXELM.GetTexPackEle(faceName);

			float u0 = 0.0f;
			float u1 = 0.0f;
			float v0 = 0.0f;
			float v1 = 0.0f;
			if (ele.IsValid())
			{
				u0 = (float)ele.X / (float)ele.TexWidth;
				u1 = (float)(ele.X + ele.W) / (float)ele.TexWidth;
				v0 = (float)(ele.TexHeight - ele.Y - ele.H) / (float)ele.TexHeight;
				v1 = (float)(ele.TexHeight - ele.Y) / (float)ele.TexHeight;

				// adjust
				u0 += 0.01f;
				u1 -= 0.01f;
				v0 += 0.01f;
				v1 -= 0.01f;
			}
			Float2 uvs[4];
			uvs[0] = Float2(u0, v0);
			uvs[1] = Float2(u1, v0);
			uvs[2] = Float2(u0, v1);
			uvs[3] = Float2(u1, v1);

			vba.TCoord<Float2>(0, startVertex1 + 0) = uvs[0];
			vba.TCoord<Float2>(0, startVertex1 + 1) = uvs[1];
			vba.TCoord<Float2>(0, startVertex1 + 2) = uvs[2];
			vba.TCoord<Float2>(0, startVertex1 + 3) = uvs[3];
		}

		indices[startIndex1 + 0] = (unsigned short)(startVertex1 + 0);
		indices[startIndex1 + 1] = (unsigned short)(startVertex1 + 1);
		indices[startIndex1 + 2] = (unsigned short)(startVertex1 + 3);
		indices[startIndex1 + 3] = (unsigned short)(startVertex1 + 0);
		indices[startIndex1 + 4] = (unsigned short)(startVertex1 + 3);
		indices[startIndex1 + 5] = (unsigned short)(startVertex1 + 2);

		startVertex1 += 4;
		startIndex1 += 6;

		++startFace;
	}

	if (!Right || (Right && 0 == Right->MtlType))
	{
		vba.Position<Float3>(startVertex1 + 0) = APoint(V_SIZE, 0.0f, 0.0f) + offset;
		vba.Position<Float3>(startVertex1 + 1) = APoint(V_SIZE, V_SIZE, 0.0f) + offset;
		vba.Position<Float3>(startVertex1 + 2) = APoint(V_SIZE, 0.0f, V_SIZE) + offset;
		vba.Position<Float3>(startVertex1 + 3) = APoint(V_SIZE, V_SIZE, V_SIZE) + offset;

		if (vba.HasNormal())
		{
			vba.Normal<Float3>(startVertex1 + 0) = Float3(1.0f, 0.0f, 0.0f);
			vba.Normal<Float3>(startVertex1 + 1) = Float3(1.0f, 0.0f, 0.0f);
			vba.Normal<Float3>(startVertex1 + 2) = Float3(1.0f, 0.0f, 0.0f);
			vba.Normal<Float3>(startVertex1 + 3) = Float3(1.0f, 0.0f, 0.0f);
		}

		if (vba.HasColor(0))
		{
			vba.Color<Float4>(0, startVertex1 + 0) = Color;
			vba.Color<Float4>(0, startVertex1 + 1) = Color;
			vba.Color<Float4>(0, startVertex1 + 2) = Color;
			vba.Color<Float4>(0, startVertex1 + 3) = Color;
		}

		if (vba.HasTCoord(0))
		{
			VoxelBlockMtl *vbm = PX2_VOXELM.GetVoxelBlockMtlByType(MtlType);
			std::string faceName = vbm->Right;
			if (Top && 0 != Top->MtlType)
			{
				faceName = vbm->Bottom;
			}
			const TexPackElement &ele = PX2_VOXELM.GetTexPackEle(faceName);

			float u0 = 0.0f;
			float u1 = 0.0f;
			float v0 = 0.0f;
			float v1 = 0.0f;
			if (ele.IsValid())
			{
				u0 = (float)ele.X / (float)ele.TexWidth;
				u1 = (float)(ele.X + ele.W) / (float)ele.TexWidth;
				v0 = (float)(ele.TexHeight - ele.Y - ele.H) / (float)ele.TexHeight;
				v1 = (float)(ele.TexHeight - ele.Y) / (float)ele.TexHeight;

				// adjust
				u0 += 0.01f;
				u1 -= 0.01f;
				v0 += 0.01f;
				v1 -= 0.01f;
			}
			Float2 uvs[4];
			uvs[0] = Float2(u0, v0);
			uvs[1] = Float2(u1, v0);
			uvs[2] = Float2(u0, v1);
			uvs[3] = Float2(u1, v1);

			vba.TCoord<Float2>(0, startVertex1 + 0) = uvs[0];
			vba.TCoord<Float2>(0, startVertex1 + 1) = uvs[1];
			vba.TCoord<Float2>(0, startVertex1 + 2) = uvs[2];
			vba.TCoord<Float2>(0, startVertex1 + 3) = uvs[3];
		}

		indices[startIndex1 + 0] = (unsigned short)(startVertex1 + 0);
		indices[startIndex1 + 1] = (unsigned short)(startVertex1 + 1);
		indices[startIndex1 + 2] = (unsigned short)(startVertex1 + 3);
		indices[startIndex1 + 3] = (unsigned short)(startVertex1 + 0);
		indices[startIndex1 + 4] = (unsigned short)(startVertex1 + 3);
		indices[startIndex1 + 5] = (unsigned short)(startVertex1 + 2);

		startVertex1 += 4;
		startIndex1 += 6;

		++startFace;
	}

	if (!Front || (Front && 0 == Front->MtlType))
	{
		vba.Position<Float3>(startVertex1 + 0) = APoint(0.0f, 0.0f, 0.0f) + offset;
		vba.Position<Float3>(startVertex1 + 1) = APoint(V_SIZE, 0.0f, 0.0f) + offset;
		vba.Position<Float3>(startVertex1 + 2) = APoint(0.0f, 0.0f, V_SIZE) + offset;
		vba.Position<Float3>(startVertex1 + 3) = APoint(V_SIZE, 0.0f, V_SIZE) + offset;

		if (vba.HasNormal())
		{
			vba.Normal<Float3>(startVertex1 + 0) = Float3(0.0f, -1.0f, 0.0f);
			vba.Normal<Float3>(startVertex1 + 1) = Float3(0.0f, -1.0f, 0.0f);
			vba.Normal<Float3>(startVertex1 + 2) = Float3(0.0f, -1.0f, 0.0f);
			vba.Normal<Float3>(startVertex1 + 3) = Float3(0.0f, -1.0f, 0.0f);
		}

		if (vba.HasColor(0))
		{
			vba.Color<Float4>(0, startVertex1 + 0) = Color;
			vba.Color<Float4>(0, startVertex1 + 1) = Color;
			vba.Color<Float4>(0, startVertex1 + 2) = Color;
			vba.Color<Float4>(0, startVertex1 + 3) = Color;
		}

		if (vba.HasTCoord(0))
		{
			VoxelBlockMtl *vbm = PX2_VOXELM.GetVoxelBlockMtlByType(MtlType);
			std::string faceName = vbm->Front;
			if (Top && 0 != Top->MtlType)
			{
				faceName = vbm->Bottom;
			}
			const TexPackElement &ele = PX2_VOXELM.GetTexPackEle(faceName);

			float u0 = 0.0f;
			float u1 = 0.0f;
			float v0 = 0.0f;
			float v1 = 0.0f;
			if (ele.IsValid())
			{
				u0 = (float)ele.X / (float)ele.TexWidth;
				u1 = (float)(ele.X + ele.W) / (float)ele.TexWidth;
				v0 = (float)(ele.TexHeight - ele.Y - ele.H) / (float)ele.TexHeight;
				v1 = (float)(ele.TexHeight - ele.Y) / (float)ele.TexHeight;

				// adjust
				u0 += 0.01f;
				u1 -= 0.01f;
				v0 += 0.01f;
				v1 -= 0.01f;
			}
			Float2 uvs[4];
			uvs[0] = Float2(u0, v0);
			uvs[1] = Float2(u1, v0);
			uvs[2] = Float2(u0, v1);
			uvs[3] = Float2(u1, v1);

			vba.TCoord<Float2>(0, startVertex1 + 0) = uvs[0];
			vba.TCoord<Float2>(0, startVertex1 + 1) = uvs[1];
			vba.TCoord<Float2>(0, startVertex1 + 2) = uvs[2];
			vba.TCoord<Float2>(0, startVertex1 + 3) = uvs[3];
		}

		indices[startIndex1 + 0] = (unsigned short)(startVertex1 + 0);
		indices[startIndex1 + 1] = (unsigned short)(startVertex1 + 1);
		indices[startIndex1 + 2] = (unsigned short)(startVertex1 + 3);
		indices[startIndex1 + 3] = (unsigned short)(startVertex1 + 0);
		indices[startIndex1 + 4] = (unsigned short)(startVertex1 + 3);
		indices[startIndex1 + 5] = (unsigned short)(startVertex1 + 2);

		startVertex1 += 4;
		startIndex1 += 6;

		++startFace;
	}

	if (!Back || (Back && 0 == Back->MtlType))
	{
		vba.Position<Float3>(startVertex1 + 0) = APoint(V_SIZE, V_SIZE, 0.0f) + offset;
		vba.Position<Float3>(startVertex1 + 1) = APoint(0.0f, V_SIZE, 0.0f) + offset;
		vba.Position<Float3>(startVertex1 + 2) = APoint(V_SIZE, V_SIZE, V_SIZE) + offset;
		vba.Position<Float3>(startVertex1 + 3) = APoint(0.0f, V_SIZE, V_SIZE) + offset;

		if (vba.HasNormal())
		{
			vba.Normal<Float3>(startVertex1 + 0) = Float3(0.0f, 1.0f, 0.0f);
			vba.Normal<Float3>(startVertex1 + 1) = Float3(0.0f, 1.0f, 0.0f);
			vba.Normal<Float3>(startVertex1 + 2) = Float3(0.0f, 1.0f, 0.0f);
			vba.Normal<Float3>(startVertex1 + 3) = Float3(0.0f, 1.0f, 0.0f);
		}

		if (vba.HasColor(0))
		{
			vba.Color<Float4>(0, startVertex1 + 0) = Color;
			vba.Color<Float4>(0, startVertex1 + 1) = Color;
			vba.Color<Float4>(0, startVertex1 + 2) = Color;
			vba.Color<Float4>(0, startVertex1 + 3) = Color;
		}

		if (vba.HasTCoord(0))
		{
			VoxelBlockMtl *vbm = PX2_VOXELM.GetVoxelBlockMtlByType(MtlType);
			std::string faceName = vbm->Back;
			if (Top && 0 != Top->MtlType)
			{
				faceName = vbm->Bottom;
			}
			const TexPackElement &ele = PX2_VOXELM.GetTexPackEle(faceName);

			float u0 = 0.0f;
			float u1 = 0.0f;
			float v0 = 0.0f;
			float v1 = 0.0f;
			if (ele.IsValid())
			{
				u0 = (float)ele.X / (float)ele.TexWidth;
				u1 = (float)(ele.X + ele.W) / (float)ele.TexWidth;
				v0 = (float)(ele.TexHeight - ele.Y - ele.H) / (float)ele.TexHeight;
				v1 = (float)(ele.TexHeight - ele.Y) / (float)ele.TexHeight;

				// adjust
				u0 += 0.01f;
				u1 -= 0.01f;
				v0 += 0.01f;
				v1 -= 0.01f;
			}
			Float2 uvs[4];
			uvs[0] = Float2(u0, v0);
			uvs[1] = Float2(u1, v0);
			uvs[2] = Float2(u0, v1);
			uvs[3] = Float2(u1, v1);

			vba.TCoord<Float2>(0, startVertex1 + 0) = uvs[0];
			vba.TCoord<Float2>(0, startVertex1 + 1) = uvs[1];
			vba.TCoord<Float2>(0, startVertex1 + 2) = uvs[2];
			vba.TCoord<Float2>(0, startVertex1 + 3) = uvs[3];
		}

		indices[startIndex1 + 0] = (unsigned short)(startVertex1 + 0);
		indices[startIndex1 + 1] = (unsigned short)(startVertex1 + 1);
		indices[startIndex1 + 2] = (unsigned short)(startVertex1 + 3);
		indices[startIndex1 + 3] = (unsigned short)(startVertex1 + 0);
		indices[startIndex1 + 4] = (unsigned short)(startVertex1 + 3);
		indices[startIndex1 + 5] = (unsigned short)(startVertex1 + 2);

		startVertex1 += 4;
		startIndex1 += 6;

		++startFace;
	}

	if (!Bottom || (Bottom && 0 == Bottom->MtlType))
	{
		vba.Position<Float3>(startVertex1 + 0) = APoint(0.0f, V_SIZE, 0.0f) + offset;
		vba.Position<Float3>(startVertex1 + 1) = APoint(V_SIZE, V_SIZE, 0.0f) + offset;
		vba.Position<Float3>(startVertex1 + 2) = APoint(0.0f, 0.0f, 0.0f) + offset;
		vba.Position<Float3>(startVertex1 + 3) = APoint(V_SIZE, 0.0f, 0.0f) + offset;

		if (vba.HasNormal())
		{
			vba.Normal<Float3>(startVertex1 + 0) = Float3(0.0f, 0.0f, -1.0f);
			vba.Normal<Float3>(startVertex1 + 1) = Float3(0.0f, 0.0f, -1.0f);
			vba.Normal<Float3>(startVertex1 + 2) = Float3(0.0f, 0.0f, -1.0f);
			vba.Normal<Float3>(startVertex1 + 3) = Float3(0.0f, 0.0f, -1.0f);
		}

		if (vba.HasColor(0))
		{
			vba.Color<Float4>(0, startVertex1 + 0) = Color;
			vba.Color<Float4>(0, startVertex1 + 1) = Color;
			vba.Color<Float4>(0, startVertex1 + 2) = Color;
			vba.Color<Float4>(0, startVertex1 + 3) = Color;
		}

		if (vba.HasTCoord(0))
		{
			VoxelBlockMtl *vbm = PX2_VOXELM.GetVoxelBlockMtlByType(MtlType);
			const TexPackElement &ele = PX2_VOXELM.GetTexPackEle(vbm->Bottom);

			float u0 = 0.0f;
			float u1 = 0.0f;
			float v0 = 0.0f;
			float v1 = 0.0f;
			if (ele.IsValid())
			{
				u0 = (float)ele.X / (float)ele.TexWidth;
				u1 = (float)(ele.X + ele.W) / (float)ele.TexWidth;
				v0 = (float)(ele.TexHeight - ele.Y - ele.H) / (float)ele.TexHeight;
				v1 = (float)(ele.TexHeight - ele.Y) / (float)ele.TexHeight;

				// adjust
				u0 += 0.01f;
				u1 -= 0.01f;
				v0 += 0.01f;
				v1 -= 0.01f;
			}
			Float2 uvs[4];
			uvs[0] = Float2(u0, v0);
			uvs[1] = Float2(u1, v0);
			uvs[2] = Float2(u0, v1);
			uvs[3] = Float2(u1, v1);

			vba.TCoord<Float2>(0, startVertex1 + 0) = uvs[0];
			vba.TCoord<Float2>(0, startVertex1 + 1) = uvs[1];
			vba.TCoord<Float2>(0, startVertex1 + 2) = uvs[2];
			vba.TCoord<Float2>(0, startVertex1 + 3) = uvs[3];
		}

		indices[startIndex1 + 0] = (unsigned short)(startVertex1 + 0);
		indices[startIndex1 + 1] = (unsigned short)(startVertex1 + 1);
		indices[startIndex1 + 2] = (unsigned short)(startVertex1 + 3);
		indices[startIndex1 + 3] = (unsigned short)(startVertex1 + 0);
		indices[startIndex1 + 4] = (unsigned short)(startVertex1 + 3);
		indices[startIndex1 + 5] = (unsigned short)(startVertex1 + 2);

		startVertex1 += 4;
		startIndex1 += 6;

		++startFace;
	}

	if (!Top || (Top && 0 == Top->MtlType))
	{
		vba.Position<Float3>(startVertex1 + 0) = APoint(0.0f, 0.0f, V_SIZE) + offset;
		vba.Position<Float3>(startVertex1 + 1) = APoint(V_SIZE, 0.0f, V_SIZE) + offset;
		vba.Position<Float3>(startVertex1 + 2) = APoint(0.0f, V_SIZE, V_SIZE) + offset;
		vba.Position<Float3>(startVertex1 + 3) = APoint(V_SIZE, V_SIZE, V_SIZE) + offset;
		
		if (vba.HasNormal())
		{
			vba.Normal<Float3>(startVertex1 + 0) = Float3(0.0f, 0.0f, 1.0f);
			vba.Normal<Float3>(startVertex1 + 1) = Float3(0.0f, 0.0f, 1.0f);
			vba.Normal<Float3>(startVertex1 + 2) = Float3(0.0f, 0.0f, 1.0f);
			vba.Normal<Float3>(startVertex1 + 3) = Float3(0.0f, 0.0f, 1.0f);
		}

		if (vba.HasColor(0))
		{
			vba.Color<Float4>(0, startVertex1 + 0) = Color;
			vba.Color<Float4>(0, startVertex1 + 1) = Color;
			vba.Color<Float4>(0, startVertex1 + 2) = Color;
			vba.Color<Float4>(0, startVertex1 + 3) = Color;
		}

		if (vba.HasTCoord(0))
		{
			VoxelBlockMtl *vbm = PX2_VOXELM.GetVoxelBlockMtlByType(MtlType);
			const TexPackElement &ele = PX2_VOXELM.GetTexPackEle(vbm->Top);

			float u0 = 0.0f;
			float u1 = 0.0f;
			float v0 = 0.0f;
			float v1 = 0.0f;
			if (ele.IsValid())
			{
				u0 = (float)ele.X / (float)ele.TexWidth;
				u1 = (float)(ele.X + ele.W) / (float)ele.TexWidth;
				v0 = (float)(ele.TexHeight - ele.Y - ele.H) / (float)ele.TexHeight;
				v1 = (float)(ele.TexHeight - ele.Y) / (float)ele.TexHeight;

				// adjust
				u0 += 0.01f;
				u1 -= 0.01f;
				v0 += 0.01f;
				v1 -= 0.01f;
			}
			Float2 uvs[4];
			uvs[0] = Float2(u0, v0);
			uvs[1] = Float2(u1, v0);
			uvs[2] = Float2(u0, v1);
			uvs[3] = Float2(u1, v1);

			vba.TCoord<Float2>(0, startVertex1 + 0) = uvs[0];
			vba.TCoord<Float2>(0, startVertex1 + 1) = uvs[1];
			vba.TCoord<Float2>(0, startVertex1 + 2) = uvs[2];
			vba.TCoord<Float2>(0, startVertex1 + 3) = uvs[3];
		}

		indices[startIndex1 + 0] = (unsigned short)(startVertex1 + 0);
		indices[startIndex1 + 1] = (unsigned short)(startVertex1 + 1);
		indices[startIndex1 + 2] = (unsigned short)(startVertex1 + 3);
		indices[startIndex1 + 3] = (unsigned short)(startVertex1 + 0);
		indices[startIndex1 + 4] = (unsigned short)(startVertex1 + 3);
		indices[startIndex1 + 5] = (unsigned short)(startVertex1 + 2);

		startVertex1 += 4;
		startIndex1 += 6;

		++startFace;
	}
}
//----------------------------------------------------------------------------