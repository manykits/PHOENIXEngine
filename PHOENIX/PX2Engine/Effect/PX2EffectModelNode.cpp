// PX2EffectModelNode.cpp

#include "PX2EffectModelNode.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2Renderer.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Node, EffectModelNode);
PX2_IMPLEMENT_STREAM(EffectModelNode);
PX2_IMPLEMENT_FACTORY(EffectModelNode);
PX2_IMPLEMENT_DEFAULT_NAMES(Node, EffectModelNode);

//----------------------------------------------------------------------------
EffectModelNode::EffectModelNode ()
	:
mResetFilename(true)
{
	mEMNCtrl = new0 EffectModelNodeController();
	mEMNCtrl->SetName("EffectModelNodeController");
	AttachController(mEMNCtrl);

	mUVSpeed = Float2::ZERO;
	mUpdatedUV = Float2::ZERO;

	mCT0 = CT_CLAMP;
	mCT1 = CT_CLAMP;
}
//----------------------------------------------------------------------------
EffectModelNode::~EffectModelNode ()
{
}
//----------------------------------------------------------------------------
void EffectModelNode::SetModelFilename (const std::string &filename)
{
	mModelFilename = filename;

	if (!mModelFilename.empty())
	{
		DetachAllChildren();

		mMeshs.clear();
		mInitEMNObjs.clear();

		mMovable = DynamicCast<Movable>(PX2_RM.BlockLoadCopy(filename));

		if (mMovable)
		{
			CollectMeshs(mMovable);
		}

		AttachChild(mMovable);
	}
}
//----------------------------------------------------------------------------
void EffectModelNode::SetTextureFilename (const std::string &filename)
{
	if (filename.empty())
		return;

	Texture2D *tex2D = DynamicCast<Texture2D>(PX2_RM.BlockLoad(filename));
	
	if (tex2D)
	{
		mTextureFileName = filename;

		std::map<TriMesh*, std::vector<EMNObj> >::iterator it = mInitEMNObjs.begin();
		for (; it!=mInitEMNObjs.end(); it++)
		{
			TriMesh *mesh = it->first;

			mesh->GetMaterialInstance()->SetPixelTexture(0, 0, tex2D);
		}
	}
}
//----------------------------------------------------------------------------
void EffectModelNode::Reset ()
{
	Node::Reset();

	const Float3 &color = GetColor();
	float alpha = GetAlpha();

	mUpdatedUV = Float2::ZERO;

	std::map<TriMesh*, std::vector<EMNObj> >::iterator it = mInitEMNObjs.begin();
	for (; it!=mInitEMNObjs.end(); it++)
	{
		TriMesh *mesh = it->first;
		std::vector<EMNObj> &objs = it->second;

		VertexBufferAccessor vba(mesh->GetVertexFormat(), mesh->GetVertexBuffer());
		for (int i=0; i<vba.GetNumVertices(); i++)
		{
			EMNObj &obj = objs[i];
			float initAlpha = obj.InitAlpha;
			Float3 &initColor = obj.InitColor;
			Float2 &initUV = obj.InitUV;

			if (vba.HasColor(0))
			{
				if (3 == vba.GetColorChannels(0))
				{
					vba.Color<Float3>(0, i) = Float3(initColor[0]*color[0], initColor[1]*color[1], initColor[2]*color[2]);
				}
				else if (4 == vba.GetColorChannels(0))
				{
					vba.Color<Float4>(0, i) = 
						Float4(initColor[0]*color[0], initColor[1]*color[1], initColor[2]*color[2], initAlpha*alpha);
				}
			}

			if (vba.HasTCoord(0))
			{
				vba.TCoord<Float2>(0, i) = initUV;
			}
		}

		Renderer::UpdateAll(mesh->GetVertexBuffer());
	}
}
//----------------------------------------------------------------------------
void EffectModelNode::CollectMeshs (Movable *movToCollect)
{
	TriMesh *mesh = DynamicCast<TriMesh>(movToCollect);
	Node *node = DynamicCast<Node>(movToCollect);
	if (mesh)
	{
		mMeshs.push_back(mesh);

		if (CT_CLAMP == mCT0)
			mesh->GetMaterialInstance()->GetMaterial()->GetPass(0, 0)->GetPixelShader()->SetCoordinate(0, 0, Shader::SC_CLAMP);
		else
			mesh->GetMaterialInstance()->GetMaterial()->GetPass(0, 0)->GetPixelShader()->SetCoordinate(0, 0, Shader::SC_REPEAT);

		if (CT_CLAMP == mCT1)
			mesh->GetMaterialInstance()->GetMaterial()->GetPass(0, 0)->GetPixelShader()->SetCoordinate(0, 1, Shader::SC_CLAMP);
		else
			mesh->GetMaterialInstance()->GetMaterial()->GetPass(0, 0)->GetPixelShader()->SetCoordinate(0, 1, Shader::SC_REPEAT);

		std::vector<EMNObj> emnObjs;
		VertexBufferAccessor vba(mesh->GetVertexFormat(), mesh->GetVertexBuffer());
		for (int i=0; i<vba.GetNumVertices(); i++)
		{
			EMNObj obj;

			if (vba.HasColor(0))
			{
				if (3 == vba.GetColorChannels(0))
				{
					obj.InitColor = vba.Color<Float3>(0, i);
				}
				else if (4 == vba.GetColorChannels(0))
				{
					Float4 color = vba.Color<Float4>(0, i);

					obj.InitColor = Float3(color[0], color[1], color[2]);
					obj.InitAlpha = color[3];
				}
			}

			if (vba.HasTCoord(0))
			{
				Float2 uv = vba.TCoord<Float2>(0, i);
				obj.InitUV = uv;
			}

			emnObjs.push_back(obj);
		}

		mInitEMNObjs[mesh] = emnObjs;
	}
	else if (node)
	{
		for (int i=0; i<node->GetNumChildren(); i++)
		{
			Movable *mov = node->GetChild(i);
			CollectMeshs(mov);
		}
	}
}
//----------------------------------------------------------------------------
void EffectModelNode::UpdateModelNode (float applicationTime, float elapsedTime)
{
	PX2_UNUSED(applicationTime);

	if (mResetFilename)
	{
		SetModelFilename(mModelFilename);
		SetTextureFilename(mTextureFileName);

		mResetFilename = false;
	}

	const Float3 &color = GetColor();
	float alpha = GetAlpha();

	if (IsPlaying())
	{
		mUpdatedUV[0] += mUVSpeed[0] * elapsedTime;
		mUpdatedUV[1] += mUVSpeed[1] * elapsedTime;

		std::map<TriMesh*, std::vector<EMNObj> >::iterator it = mInitEMNObjs.begin();
		for (; it!=mInitEMNObjs.end(); it++)
		{
			TriMesh *mesh = it->first;
			std::vector<EMNObj> &objs = it->second;

			VertexBufferAccessor vba(mesh->GetVertexFormat(), mesh->GetVertexBuffer());
			for (int i=0; i<vba.GetNumVertices(); i++)
			{
				EMNObj &obj = objs[i];
				float initAlpha = obj.InitAlpha;
				Float3 &initColor = obj.InitColor;
				Float2 &initUV = obj.InitUV;

				if (vba.HasColor(0))
				{
					if (3 == vba.GetColorChannels(0))
					{
						vba.Color<Float3>(0, i) = Float3(initColor[0]*color[0], initColor[1]*color[1], initColor[2]*color[2]);
					}
					else if (4 == vba.GetColorChannels(0))
					{
						vba.Color<Float4>(0, i) = 
							Float4(initColor[0]*color[0], initColor[1]*color[1], initColor[2]*color[2], initAlpha*alpha);
					}
				}

				if (vba.HasTCoord(0))
				{
					vba.TCoord<Float2>(0, i)[0] = initUV[0] + mUpdatedUV[0];
					vba.TCoord<Float2>(0, i)[1] = initUV[1] + mUpdatedUV[1];
				}
			}

			Renderer::UpdateAll(mesh->GetVertexBuffer());
		}
	}
}
//----------------------------------------------------------------------------
void EffectModelNode::SetCoordinateType0 (CoordinateType type)
{
	mCT0 = type;

	std::map<TriMesh*, std::vector<EMNObj> >::iterator it = mInitEMNObjs.begin();
	for (; it!=mInitEMNObjs.end(); it++)
	{
		TriMesh *mesh = it->first;

		if (CT_CLAMP == mCT0)
			mesh->GetMaterialInstance()->GetMaterial()->GetPass(0, 0)->GetPixelShader()->SetCoordinate(0, 0, Shader::SC_CLAMP);
		else
			mesh->GetMaterialInstance()->GetMaterial()->GetPass(0, 0)->GetPixelShader()->SetCoordinate(0, 0, Shader::SC_REPEAT);

		if (CT_CLAMP == mCT1)
			mesh->GetMaterialInstance()->GetMaterial()->GetPass(0, 0)->GetPixelShader()->SetCoordinate(0, 1, Shader::SC_CLAMP);
		else
			mesh->GetMaterialInstance()->GetMaterial()->GetPass(0, 0)->GetPixelShader()->SetCoordinate(0, 1, Shader::SC_REPEAT);
	}
}
//----------------------------------------------------------------------------
void EffectModelNode::SetCoordinateType1 (CoordinateType type)
{
	mCT1 = type;

	std::map<TriMesh*, std::vector<EMNObj> >::iterator it = mInitEMNObjs.begin();
	for (; it!=mInitEMNObjs.end(); it++)
	{
		TriMesh *mesh = it->first;

		if (CT_CLAMP == mCT0)
			mesh->GetMaterialInstance()->GetMaterial()->GetPass(0, 0)->GetPixelShader()->SetCoordinate(0, 0, Shader::SC_CLAMP);
		else
			mesh->GetMaterialInstance()->GetMaterial()->GetPass(0, 0)->GetPixelShader()->SetCoordinate(0, 0, Shader::SC_REPEAT);

		if (CT_CLAMP == mCT1)
			mesh->GetMaterialInstance()->GetMaterial()->GetPass(0, 0)->GetPixelShader()->SetCoordinate(0, 1, Shader::SC_CLAMP);
		else
			mesh->GetMaterialInstance()->GetMaterial()->GetPass(0, 0)->GetPixelShader()->SetCoordinate(0, 1, Shader::SC_REPEAT);
	}
}
//----------------------------------------------------------------------------
void EffectModelNode::RegistProperties ()
{
	Node::RegistProperties();

	AddPropertyClass("EffectModelNode");
	AddProperty("ModelFilename", PT_STRINGBUTTON, GetModelFilename());
	AddProperty("TextureFilename", PT_STRINGBUTTON, GetTextureFilename());
	
	std::vector<std::string> ctTypes;
	ctTypes.push_back("SC_CLAMP");
	ctTypes.push_back("SC_REPEAT");

	AddPropertyEnum("CoordType0", GetCoordinateType0(), ctTypes);
	AddPropertyEnum("CoordType1", GetCoordinateType1(), ctTypes);

	AddProperty("UVSpeed", PT_FLOAT2, GetUVSpeed());
}
//----------------------------------------------------------------------------
void EffectModelNode::OnPropertyChanged (const PropertyObject &obj)
{
	Node::OnPropertyChanged(obj);

	if ("ModelFilename" == obj.Name)
	{
		SetModelFilename(PX2_ANY_AS(obj.Data, std::string));
	}
	else if ("TextureFilename" == obj.Name)
	{
		SetTextureFilename(PX2_ANY_AS(obj.Data, std::string));
	}
	else if ("UVSpeed" == obj.Name)
	{
		SetUVSpeed(PX2_ANY_AS(obj.Data, Float2));
	}
	else if ("CoordType0" == obj.Name)
	{
		SetCoordinateType0((CoordinateType)PX2_ANY_AS(obj.Data, int));
	}
	else if ("CoordType1" == obj.Name)
	{
		SetCoordinateType1((CoordinateType)PX2_ANY_AS(obj.Data, int));
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
EffectModelNode::EffectModelNode (LoadConstructor value)
	:
Node(value),
mResetFilename(true)
{
	mUVSpeed = Float2::ZERO;
	mUpdatedUV = Float2::ZERO;

	mCT0 = CT_CLAMP;
	mCT1 = CT_CLAMP;
}
//----------------------------------------------------------------------------
void EffectModelNode::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Node::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadString(mModelFilename);
	source.ReadAggregate(mUVSpeed);

	source.ReadPointer(mEMNCtrl);

	source.ReadString(mTextureFileName);
	source.ReadEnum(mCT0);
	source.ReadEnum(mCT1);

	PX2_END_DEBUG_STREAM_LOAD(EffectModelNode, source);
}
//----------------------------------------------------------------------------
void EffectModelNode::Link (InStream& source)
{
	Node::Link(source);

	source.ResolveLink(mEMNCtrl);
}
//----------------------------------------------------------------------------
void EffectModelNode::PostLink ()
{
	Node::PostLink();

	if (mEMNCtrl)
	{
		mEMNCtrl->PostLink();
	}
}
//----------------------------------------------------------------------------
bool EffectModelNode::Register (OutStream& target) const
{
	if (Node::Register(target))
	{
		mEMNCtrl->Register(target);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EffectModelNode::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Node::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteString(mModelFilename);
	target.WriteAggregate(mUVSpeed);

	target.WritePointer(mEMNCtrl);

	target.WriteString(mTextureFileName);
	target.WriteEnum(mCT0);
	target.WriteEnum(mCT1);

	PX2_END_DEBUG_STREAM_SAVE(EffectModelNode, target);
}
//----------------------------------------------------------------------------
int EffectModelNode::GetStreamingSize (Stream &stream) const
{
	int size = Node::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_STRINGSIZE(mModelFilename);
	size += sizeof(mUVSpeed);
	size += PX2_POINTERSIZE(mEMNCtrl);

	size += PX2_STRINGSIZE(mTextureFileName);
	size += PX2_ENUMSIZE(mCT0);
	size += PX2_ENUMSIZE(mCT1);

	return size;
}
//----------------------------------------------------------------------------