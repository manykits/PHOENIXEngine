// PX2SceneBuilderMtl.cpp

#include "PX2SceneBuilder.hpp"
#include "PX2ExportSettings.hpp"
#include "PX2AlphaProperty.hpp"
#include "PX2Shader.hpp"
#include "PX2MaterialManager.hpp"
#include "PX2MaterialInstance.hpp"
#include "PX2VertexColor4Material.hpp"
#include "PX2StringHelp.hpp"

#define PX2_DEFAULT_TEXTURE "Data/engine/default.png"

#define DIRECTX_9_SHADER_CLASS_ID Class_ID(0x0ed995e4 ,0x6133daf2)
//----------------------------------------------------------------------------
const TCHAR* SceneBuilder::msMapName[NTEXMAPS] =
{
	_T("map_ambient"),             // ID_AM 环境光
	_T("map_diffuse"),             // ID_DI 漫反射
	_T("map_specular"),            // ID_SP 高光颜色
	_T("map_shininess"),           // ID_SH 高光级别
	_T("map_shininess_strength"),  // ID_SS 光泽度
	_T("map_selfillumination"),    // ID_SI 自发光
	_T("map_opacity"),             // ID_OP 不透明度
	_T("map_filter_color"),        // ID_FI 过滤色
	_T("map_bump"),                // ID_BU 凹凸贴图
	_T("map_reflection"),          // ID_RL 反射
	_T("map_refraction"),          // ID_RR 折射
	_T("map_displacment")          // ID_DP 置换
};
//----------------------------------------------------------------------------
const char* SceneBuilder::msEnvName[5] =
{
	"env_explicit",
	"env_sphere",
	"env_cylinder",
	"env_shrink",
	"env_screen"
};
//----------------------------------------------------------------------------
const TCHAR* SceneBuilder::msMapEnvironment = _T("map_environment");
const TCHAR* SceneBuilder::msMapGeneric = _T("map_generic");
//----------------------------------------------------------------------------
void SceneBuilder::CollectMaterials(INode *node)
{
	Mtl *mtl = node->GetMtl();
	bool hasModifiers = IsNodeHasModifierSkin(node);
	PX2_UNUSED(hasModifiers);

	if (mtl)
	{
		mMtls.Add(mtl);
	}

	for (int i=0; i<node->NumberOfChildren(); i++)
	{
		CollectMaterials(node->GetChildNode(i));
	}
}
//----------------------------------------------------------------------------
void SceneBuilder::ConvertMaterials ()
{
	int quantity = mMtls.GetQuantity();

	mMtlTreeList.resize(quantity);
	for (int i=0; i<quantity; i++)
	{
		Mtl *mtl = mMtls.GetMtl(i);
		if (mtl)
		{
			ConvertMaterial(*mtl, mMtlTreeList[i]);
		}
	}
}
//----------------------------------------------------------------------------
void SceneBuilder::ConvertMaterial (Mtl &mtl, MtlTree &mtlTree)
{
	// 光照属性
	PX2::Shine *shine = new0 PX2::Shine;
	Color color = mtl.GetAmbient();
	float alpha = 1.0f - mtl.GetXParency();
	shine->Emissive = PX2::Float4(0.0f, 0.0f, 0.0f, alpha);
	shine->Ambient = PX2::Float4(color.r, color.g, color.b, 1.0f);
	color = mtl.GetDiffuse();
	shine->Diffuse = PX2::Float4(color.r, color.g, color.b, 1.0f);
	color = mtl.GetSpecular();
	float shininess = mtl.GetShininess()*2.0f;
	shine->Specular = PX2::Float4(color.r, color.g, color.b, shininess);

	const char *name = (const char*)mtl.GetName();
	shine->SetName(name);

	mtlTree.SetShine(shine);

	bool IsDirect9Shader = false;

	if (mtl.ClassID() == Class_ID(CMTL_CLASS_ID, 0)
		|| mtl.ClassID() == Class_ID(DMTL_CLASS_ID, 0))
	{
		bool isEnableDI = false;
		bool isEnableOP = false;
		bool isEnableNORMAL = false;
		float opacity = 1.0f;

		StdMat2 *stdMat2 = (StdMat2*)(&mtl);
		Interval valid = FOREVER;
		stdMat2->Update(mTimeStart, valid);

		std::string strName(stdMat2->GetName());
		bool doubleSide = (stdMat2->GetTwoSided()==1);

		char strBitMapName[256];
		memset(strBitMapName, 0, 256*sizeof(char));
		std::string resourcePath_Diffuse;

		char strBitMapNameNormal[256];
		memset(strBitMapNameNormal, 0, 256 * sizeof(char));
		std::string resourcePath_Normal;

		PX2::Shader::SamplerFilter filter = PX2::Shader::SF_LINEAR_LINEAR;
		PX2::Shader::SamplerCoordinate uvCoord0 = PX2::Shader::SC_CLAMP;
		PX2::Shader::SamplerCoordinate uvCoord1 = PX2::Shader::SC_CLAMP;

		if (stdMat2->MapEnabled(ID_DI))
		{
			isEnableDI = true;

			BitmapTex *tex = (BitmapTex*)stdMat2->GetSubTexmap(ID_DI);
			int tt = tex->GetTextureTiling();
			if (tt & U_WRAP)
			{
				uvCoord0 = PX2::Shader::SC_REPEAT;
			}
			if (tt & V_WRAP)
			{
				uvCoord1 = PX2::Shader::SC_REPEAT;
			}

			BitmapInfo bI;
			const char *mapName = tex->GetMapName();
			TheManager->GetImageInfo(&bI, mapName);
			strcpy(strBitMapName, bI.Name());
			int width = bI.Width();
			int height = bI.Height();
			const char *biFilename = bI.Filename();

			std::string fullName = std::string(strBitMapName);
			std::string srcRootDirStr = std::string(mSettings->SrcRootDir);
			std::string::size_type sizeT = srcRootDirStr.length();

			if (srcRootDirStr.length()>0 && ('\\'!=srcRootDirStr[srcRootDirStr.length()-1] &&
				'/'!=srcRootDirStr[srcRootDirStr.length()-1]))
			{
				sizeT += 1;
			}


			resourcePath_Diffuse = std::string(strBitMapName).substr(sizeT);			

			StdUVGen* uvGen = tex->GetUVGen();
			PX2_UNUSED(uvGen);
			int filType = tex->GetFilterType();
			switch (filType)
			{
				case FILTER_PYR:
					filter = PX2::Shader::SF_LINEAR_LINEAR;
					break;
				case FILTER_SAT:
					filter = PX2::Shader::SF_NEAREST;
					break;
				default:
					break;
			}
		}
		else
		{
			sprintf(strBitMapName, "%s/%s", mSettings->SrcRootDir, PX2_DEFAULT_TEXTURE);
			resourcePath_Diffuse = PX2_DEFAULT_TEXTURE;
		}

		if (stdMat2->MapEnabled(ID_OP))
		{
			isEnableOP = true;
		}
		opacity = stdMat2->GetOpacity(mTimeStart);

		if (stdMat2->MapEnabled(ID_BU))
		{
			isEnableNORMAL = true;

			BitmapTex *tex = (BitmapTex*)stdMat2->GetSubTexmap(ID_BU);
			int tt = tex->GetTextureTiling();
			if (tt & U_WRAP)
			{
				uvCoord0 = PX2::Shader::SC_REPEAT;
			}
			if (tt & V_WRAP)
			{
				uvCoord1 = PX2::Shader::SC_REPEAT;
			}

			BitmapInfo bI;
			const char *mapName = tex->GetMapName();
			TheManager->GetImageInfo(&bI, mapName);
			strcpy(strBitMapNameNormal, bI.Name());
			int width = bI.Width();
			int height = bI.Height();
			const char *biFilename = bI.Filename();

			std::string fullName = std::string(strBitMapNameNormal);
			std::string srcRootDirStr = std::string(mSettings->SrcRootDir);
			std::string::size_type sizeT = srcRootDirStr.length();

			if (srcRootDirStr.length() > 0 && ('\\' != srcRootDirStr[srcRootDirStr.length() - 1] &&
				'/' != srcRootDirStr[srcRootDirStr.length() - 1]))
			{
				sizeT += 1;
			}

			resourcePath_Normal = std::string(strBitMapNameNormal).substr(sizeT);

			StdUVGen* uvGen = tex->GetUVGen();
			PX2_UNUSED(uvGen);
			int filType = tex->GetFilterType();
			switch (filType)
			{
			case FILTER_PYR:
				filter = PX2::Shader::SF_LINEAR_LINEAR;
				break;
			case FILTER_SAT:
				filter = PX2::Shader::SF_NEAREST;
				break;
			default:
				break;
			}
		}
		else
		{
			sprintf(strBitMapNameNormal, "%s/%s", mSettings->SrcRootDir, PX2_DEFAULT_TEXTURE);
			resourcePath_Normal = PX2_DEFAULT_TEXTURE;
		}

		PX2::Texture2D *tex2d_Diffuse = PX2::DynamicCast<PX2::Texture2D>(PX2::ResourceManager::GetSingleton().BlockLoad(strBitMapName));
		PX2::Texture2D *tex2d_Normal = PX2::DynamicCast<PX2::Texture2D>(PX2::ResourceManager::GetSingleton().BlockLoad(strBitMapNameNormal));
		if (tex2d_Diffuse)
		{
			resourcePath_Diffuse = PX2::StringHelp::StandardisePath(resourcePath_Diffuse);
			resourcePath_Diffuse = resourcePath_Diffuse.substr(0, resourcePath_Diffuse.length() - 1);
			tex2d_Diffuse->SetResourcePath(resourcePath_Diffuse);
			if (tex2d_Normal)
			{
				resourcePath_Normal = PX2::StringHelp::StandardisePath(resourcePath_Normal);
				resourcePath_Normal = resourcePath_Normal.substr(0, resourcePath_Normal.length() - 1);
				tex2d_Normal->SetResourcePath(resourcePath_Normal);
			}

			std::string mtlFileName;
			std::string allMtlFileName;
			std::string dstRootDirStr = std::string(mSettings->DstRootDir);
			dstRootDirStr = PX2::StringHelp::StandardisePath(dstRootDirStr);

			std::string dstFilePathDiffuse = dstRootDirStr + resourcePath_Diffuse;

			// Copy
			PX2::PX2_RM.CopyTheFile(strBitMapName, dstFilePathDiffuse);

			PX2::MaterialInstancePtr px2MtlInst;
			PX2::MaterialPtr px2mlt;

			const std::string &mtlStr = mSettings->GetMtlTypeStr();

			if ("std_light" == mtlStr)
			{
				mtlFileName = "Data/engine_mtls/std/std.px2obj";
				allMtlFileName = dstRootDirStr + mtlFileName;

				px2MtlInst = new0 PX2::MaterialInstance(allMtlFileName, "std_light", false);
			}
			else if ("materialcolor" == mtlStr)
			{
				mtlFileName = "Data/engine_mtls/materialcolor/materialcolor.px2obj";
				allMtlFileName = dstRootDirStr + mtlFileName;

				px2MtlInst = new0 PX2::MaterialInstance(allMtlFileName, "materialcolor", false);
			}
			else if ("vertexcolor_light" == mtlStr)
			{
				mtlFileName = "Data/engine_mtls/vertexcolor/vertexcolor.px2obj";
				allMtlFileName = dstRootDirStr + mtlFileName;

				px2MtlInst = new0 PX2::MaterialInstance(allMtlFileName, "vertexcolor_light", false);
			}
			else if ("std_lightnormal" == mtlStr)
			{
				mtlFileName = "Data/engine_mtls/std/std.px2obj";
				allMtlFileName = dstRootDirStr + mtlFileName;

				px2MtlInst = new0 PX2::MaterialInstance(allMtlFileName, "std_lightnormal", false);
			}
			else if ("skinskeleton" == mtlStr)
			{
				mtlFileName = "Data/engine_mtls/skinskeleton/skinskeleton.px2obj";
				allMtlFileName = dstRootDirStr + mtlFileName;

				px2MtlInst = new0 PX2::MaterialInstance(allMtlFileName, "skinskeleton", false);
			}
			else if ("tex2d" == mtlStr)
			{
				mtlFileName = "Data/engine_mtls/tex2d/tex2d.px2obj";
				allMtlFileName = dstRootDirStr + mtlFileName;

				px2MtlInst = new0 PX2::MaterialInstance(allMtlFileName, "tex2d", false);
			}

			px2MtlInst->_SetMaterialFilename(mtlFileName);
			px2MtlInst->SetPixelTexture(0, "SampleBase", tex2d_Diffuse);

			if ("std_lightnormal" == mtlStr)
			{
				std::string dstFilePathNormal = dstRootDirStr + resourcePath_Normal;
				PX2::PX2_RM.CopyTheFile(strBitMapNameNormal, dstFilePathNormal);
				px2MtlInst->SetPixelTexture(0, "SampleNormal", tex2d_Normal);
			}

			px2mlt = px2MtlInst->GetMaterial();

			// tex
			if ("materialcolor" != mtlStr)
			{
				px2mlt->GetPixelShader(0, 0)->SetFilter(0, PX2::Shader::SF_LINEAR_LINEAR);
				px2mlt->GetPixelShader(0, 0)->SetCoordinate(0, 0, uvCoord0);
				px2mlt->GetPixelShader(0, 0)->SetCoordinate(0, 1, uvCoord1);
			}

			px2mlt->GetCullProperty(0, 0)->Enabled = doubleSide;

			px2mlt->GetAlphaProperty(0, 0)->BlendEnabled = false;
			px2mlt->GetAlphaProperty(0, 0)->CompareEnabled = false;

			px2mlt->GetAlphaProperty(0, 0)->BlendEnabled = (opacity < 1.0f);

			px2mlt->GetAlphaProperty(0, 0)->CompareEnabled = isEnableOP;
			px2mlt->GetAlphaProperty(0, 0)->Compare = PX2::AlphaProperty::CM_GREATER;
			px2mlt->GetAlphaProperty(0, 0)->Reference = 0.25f;
	
			mtlTree.SetMaterialInstance(px2MtlInst);
		}
		else
		{
			const std::string &mtlStr = mSettings->GetMtlTypeStr();
			if ("materialcolor" == mtlStr)
			{
				std::string mtlFileName;
				std::string allMtlFileName;
				std::string dstRootDirStr = std::string(mSettings->DstRootDir);
				dstRootDirStr = PX2::StringHelp::StandardisePath(dstRootDirStr);

				PX2::MaterialInstancePtr px2MtlInst;
				PX2::MaterialPtr px2mlt;

				const std::string &mtlStr = mSettings->GetMtlTypeStr();
				mtlFileName = "Data/engine_mtls/materialcolor/materialcolor.px2obj";
				allMtlFileName = dstRootDirStr + mtlFileName;

				px2MtlInst = new0 PX2::MaterialInstance(allMtlFileName, "materialcolor", false);
				px2MtlInst->_SetMaterialFilename(mtlFileName);

				px2mlt = px2MtlInst->GetMaterial();


				px2mlt->GetCullProperty(0, 0)->Enabled = doubleSide;

				px2mlt->GetAlphaProperty(0, 0)->BlendEnabled = false;
				px2mlt->GetAlphaProperty(0, 0)->CompareEnabled = false;

				px2mlt->GetAlphaProperty(0, 0)->BlendEnabled = (opacity < 1.0f);

				px2mlt->GetAlphaProperty(0, 0)->CompareEnabled = isEnableOP;
				px2mlt->GetAlphaProperty(0, 0)->Compare = PX2::AlphaProperty::CM_GREATER;
				px2mlt->GetAlphaProperty(0, 0)->Reference = 0.25f;

				mtlTree.SetMaterialInstance(px2MtlInst);
			}
			else
			{
				PX2::VertexColor4Material *vcMtl = new0 PX2::VertexColor4Material();
				PX2::MaterialInstance *instance = vcMtl->CreateInstance();
				instance->SetName("MI_VertexColor4");
				mtlTree.SetMaterialInstance(instance);
			}
		}
	}
	else if (mtl.ClassID() == Class_ID(MULTI_CLASS_ID, 0))
	{
	}
	else if (mtl.ClassID() == DIRECTX_9_SHADER_CLASS_ID)
	{
		/*_*/;
		//we do not use it now.
	}
	else
	{
		PX2::VertexColor4Material *vcMtl = new0 PX2::VertexColor4Material();
		PX2::MaterialInstance *instance = vcMtl->CreateInstance();
		instance->SetName("MI_VertexColor4");
		mtlTree.SetMaterialInstance(instance);
	}

	// 对子材质进行处理
	if (IsDirect9Shader)
		return;

	int mQuantity = mtl.NumSubMtls(); // Class_ID(MULTI_CLASS_ID, 0)
	if (mQuantity > 0)
	{
		mtlTree.SetMChildQuantity(mQuantity);
		for (int i=0; i<mQuantity; i++)
		{
			Mtl *subMtl = 0;
			subMtl = mtl.GetSubMtl(i);
			if (subMtl)
			{
				ConvertMaterial(*subMtl, mtlTree.GetMChild(i));
			}
		}
	}
}
//----------------------------------------------------------------------------
bool SceneBuilder::IsValidName (const char *str)
{
	int length = (int)strlen(str);
	if ( length == 0) 
		return false;

	for (int i = 0; i < length; i++)
	{
		if (str[i] >= 'a' && str[i] <= 'z')
			continue;
		else if (str[i] >= 'A' && str[i] <= 'Z')
			continue;
		else if (str[i] >= '1' && str[i] <= '9')
			continue;
		else if (str[i] == '0')
			continue;
		else if (str[i] == ' ')
			continue;
		else if (str[i] == '\t')
			continue;
		else if (str[i] == '_')
			continue;
		else if (str[i] == '-')
			continue;
		else if (str[i] == '/')
			continue;
		else if (str[i] == '\\')
			continue; 
		else
			return false;
	}
	return true;
}
//----------------------------------------------------------------------------