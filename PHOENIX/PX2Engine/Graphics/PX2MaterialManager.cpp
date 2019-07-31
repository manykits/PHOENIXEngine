// PX2MaterialManager.cpp

#include "PX2MaterialManager.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2PVMatrixConstant.hpp"
#include "PX2PVWMatrixConstant.hpp"
#include "PX2VMatrixConstant.hpp"
#include "PX2WMatrixConstant.hpp"
#include "PX2VWMatrixConstant.hpp"
#include "PX2WInverseMatrixConstant.hpp"
#include "PX2ProjectorMatrixConstant.hpp"
#include "PX2ShineEmissiveConstant.hpp"
#include "PX2ShineAmbientConstant.hpp"
#include "PX2ShineDiffuseConstant.hpp"
#include "PX2ShineSpecularConstant.hpp"
#include "PX2CameraWorldPositionConstant.hpp"
#include "PX2CameraModelPositionConstant.hpp"
#include "PX2CameraWorldDVectorConstant.hpp"
#include "PX2LightWorldDVectorConstant.hpp"
#include "PX2LightAmbientConstant.hpp"
#include "PX2LightDiffuseConstant.hpp"
#include "PX2LightSpecularConstant.hpp"
#include "PX2LightGroupConstant.hpp"
#include "PX2FogParamConstant.hpp"
#include "PX2FogColorHeightConstant.hpp"
#include "PX2FogColorDistConstant.hpp"
#include "PX2SkinMatrixConstant.hpp"
#include "PX2Log.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
MaterialManager::MaterialManager() :
mNextShaderKey(1000)
{
}
//----------------------------------------------------------------------------
MaterialManager::~MaterialManager()
{
}
//----------------------------------------------------------------------------
bool MaterialManager::Initlize()
{
	mNextShaderKey = 1000;

	return true;
}
//----------------------------------------------------------------------------
bool MaterialManager::Terminate()
{
	mMaterialMap.clear();

	return true;
}
//----------------------------------------------------------------------------
Material *MaterialManager::GetMaterial(const FString &filename, 
	const FString &techName, bool share)
{
	std::string allName = std::string(filename) + std::string(techName);

	if (share)
	{
		Material *mtl = 0;
		std::map<FString, _MtlObject>::iterator it = mMaterialMap.find(allName.c_str());
		if (it != mMaterialMap.end())
		{
			mtl = it->second.TheMaterial;
		}
		else
		{
			int shaderKey = QueryShaderKey();

			mtl = DynamicCast<Material>(GraphicsRoot::msUserLoadFun(filename));
			mMaterialMap[allName.c_str()].TheMaterial = mtl;
			mMaterialMap[allName.c_str()].ShaderKey = shaderKey;

			_SetMaterialShaderKey(mtl, shaderKey);
		}

		return mtl;
	}
	else
	{		
		Material *mtl = 0;
		Object *mtlObj = 0;

		if (GraphicsRoot::msUserLoadFun)
		{
			mtlObj = GraphicsRoot::msUserLoadFun(filename);
		}

		if (mtlObj)
		{
			mtl = DynamicCast<Material>(mtlObj->Copy(""));
			if (mtl)
			{
				std::map<FString, _MtlObject>::iterator it = mMaterialMap.find(allName.c_str());
				if (it != mMaterialMap.end())
				{
					_SetMaterialShaderKey(mtl, it->second.ShaderKey);
				}
				else
				{
					int shaderKey = QueryShaderKey();

					mMaterialMap[filename].TheMaterial = mtl;
					mMaterialMap[filename].ShaderKey = shaderKey;

					_SetMaterialShaderKey(mtl, shaderKey);
				}
			}
		}

		return mtl;
	}
}
//----------------------------------------------------------------------------
void MaterialManager::AddMaterial(const FString &filename, 
	const FString &techName, Material *mtl)
{
	std::string allName = std::string(filename) + std::string(techName);

	int shaderKey = 99999;
	std::map<FString, _MtlObject>::iterator it = mMaterialMap.find(allName.c_str());
	if (it == mMaterialMap.end())
	{
		shaderKey = QueryShaderKey();

		mMaterialMap[allName.c_str()].TheMaterial = mtl;
		mMaterialMap[allName.c_str()].ShaderKey = shaderKey;
	}
	else
	{
		shaderKey = it->second.ShaderKey;
	}

	_SetMaterialShaderKey(mtl, shaderKey);
}
//----------------------------------------------------------------------------
int MaterialManager::QueryShaderKey()
{
	return mNextShaderKey++;
}
//----------------------------------------------------------------------------
void MaterialManager::_SetMaterialShaderKey(Material *mtl, int shaderKey)
{
	int numTechniques = mtl->GetNumTechniques();
	for (int i = 0; i < numTechniques; i++)
	{
		MaterialTechnique *tech = mtl->GetTechnique(i);
		int numPass = tech->GetNumPasses();
		for (int i = 0; i < numPass; i++)
		{
			MaterialPass *mtlPass = tech->GetPass(i);
			VertexShader *vs = mtlPass->GetVertexShader();
			PixelShader *ps = mtlPass->GetPixelShader();

			vs->SetShaderKey(shaderKey);
			ps->SetShaderKey(shaderKey);
		}
	}
}
//----------------------------------------------------------------------------
ShaderFloat *MaterialManager::CreateShaderFloat(const FString &name, int numRegister)
{
	ShaderFloat *shaderFloat = 0;

	if (FString("PVWMatrix") == name)
	{
		shaderFloat = new0 PVWMatrixConstant();
	}
	else if (FString("WMatrix") == name)
	{
		shaderFloat = new0 WMatrixConstant();
	}
	else if (FString("VMatrix") == name)
	{
		shaderFloat = new0 VMatrixConstant();
	}
	else if (FString("VWMatrix") == name)
	{
		shaderFloat = new0 VWMatrixConstant();
	}
	else if (FString("WIMatrix") == name)
	{
		shaderFloat = new0 WInverseMatrixConstant();
	}
	else if (FString("ProjectPVBSMatrix_Dir") == name)
	{
		shaderFloat = new0 ProjectorMatrixConstant(0, true);
	}
	else if (FString("ShineEmissive") == name)
	{
		shaderFloat = new0 ShineEmissiveConstant();
	}
	else if (FString("ShineAmbient") == name)
	{
		shaderFloat = new0 ShineAmbientConstant();
	}
	else if (FString("ShineDiffuse") == name)
	{
		shaderFloat = new0 ShineDiffuseConstant();
	}
	else if (FString("ShineSpecular") == name)
	{
		shaderFloat = new0 ShineSpecularConstant();
	}
	else if (FString("CameraWorldPosition") == name)
	{
		shaderFloat = new0 CameraWorldPositionConstant();
	}
	else if (FString("CameraModelPosition") == name)
	{
		shaderFloat = new0 CameraModelPositionConstant();
	}
	else if (FString("CameraWorldDVector") == name)
	{
		shaderFloat = new0 CameraWorldDVectorConstant();
	}
	else if (FString("LightWorldDVector_Dir") == name)
	{
		shaderFloat = new0 LightWorldDVectorConstant(0);
		((LightConstant*)shaderFloat)->SetDefaultDir(true);
	}
	else if (FString("LightAmbient_Dir") == name)
	{
		shaderFloat = new0 LightAmbientConstant(0);
		((LightConstant*)shaderFloat)->SetDefaultDir(true);
	}
	else if (FString("LightDiffuse_Dir") == name)
	{
		shaderFloat = new0 LightDiffuseConstant(0);
		((LightConstant*)shaderFloat)->SetDefaultDir(true);
	}
	else if (FString("LightSpecular_Dir") == name)
	{
		shaderFloat = new0 LightSpecularConstant(0);
		((LightConstant*)shaderFloat)->SetDefaultDir(true);
	}
	else if (FString("LightGroup") == name)
	{
		shaderFloat = new0 LightGroupConstant();
	}
	else if (FString("FogParam") == name)
	{
		shaderFloat = new0 FogParamConstant();
	}
	else if (FString("FogColorDist") == name)
	{
		shaderFloat = new0 FogColorDistConstant();
	}
	else if (FString("FogColorHeight") == name)
	{
		shaderFloat = new0 FogColorHeightConstant();
	}
	else if (FString("BoneTM") == name)
	{
		shaderFloat = new0 SkinMatrixConstant();
	}
	else
	{
		shaderFloat = new0 ShaderFloat(numRegister);

		if ("UVOffset" == std::string(name))
		{
			Float4 vl(0.0f, 0.0f, 1.0f, 1.0f);
			shaderFloat->SetRegister(0, vl);
		}
	}

	return shaderFloat;
}
//----------------------------------------------------------------------------