// FxCompiler.cpp

#include "FxCompiler.hpp"

const std::string FxCompiler::msSrcBlendMode[AlphaProperty::SBM_QUANTITY] =
{
	"SBM_ZERO",
	"SBM_ONE",
	"SBM_DST_COLOR",
	"SBM_ONE_MINUS_DST_COLOR",
	"SBM_SRC_ALPHA",
	"SBM_ONE_MINUS_SRC_ALPHA",
	"SBM_DST_ALPHA",
	"SBM_ONE_MINUS_DST_ALPHA",
	"SBM_SRC_ALPHA_SATURATE",
	"SBM_CONSTANT_COLOR",
	"SBM_ONE_MINUS_CONSTANT_COLOR",
	"SBM_CONSTANT_ALPHA",
	"SBM_ONE_MINUS_CONSTANT_ALPHA"
};

const std::string FxCompiler::msDstBlendMode[AlphaProperty::DBM_QUANTITY] =
{
	"DBM_ZERO",
	"DBM_ONE",
	"DBM_SRC_COLOR",
	"DBM_ONE_MINUS_SRC_COLOR",
	"DBM_SRC_ALPHA",
	"DBM_ONE_MINUS_SRC_ALPHA",
	"DBM_DST_ALPHA",
	"DBM_ONE_MINUS_DST_ALPHA",
	"DBM_CONSTANT_COLOR",
	"DBM_ONE_MINUS_CONSTANT_COLOR",
	"DBM_CONSTANT_ALPHA",
	"DBM_ONE_MINUS_CONSTANT_ALPHA"
};

const std::string FxCompiler::msAlphaCompareMode[AlphaProperty::CM_QUANTITY] =
{
	"CM_NEVER",
	"CM_LESS",
	"CM_EQUAL",
	"CM_LEQUAL",
	"CM_GREATER",
	"CM_NOTEQUAL",
	"CM_GEQUAL",
	"CM_ALWAYS"
};

const std::string FxCompiler::msDepthCompareMode[DepthProperty::CM_QUANTITY] =
{
	"CM_NEVER",
	"CM_LESS",
	"CM_EQUAL",
	"CM_LEQUAL",
	"CM_GREATER",
	"CM_NOTEQUAL",
	"CM_GEQUAL",
	"CM_ALWAYS"
};

const std::string FxCompiler::msStencilCompareMode[StencilProperty::CM_QUANTITY] =
{
	"CM_NEVER",
	"CM_LESS",
	"CM_EQUAL",
	"CM_LEQUAL",
	"CM_GREATER",
	"CM_NOTEQUAL",
	"CM_GEQUAL",
	"CM_ALWAYS"
};

const std::string FxCompiler::msStencilOperationType[StencilProperty::OT_QUANTITY] =
{
	"OT_KEEP",
	"OT_ZERO",
	"OT_REPLACE",
	"OT_INCREMENT",
	"OT_DECREMENT",
	"OT_INVERT"
};

const std::string FxCompiler::msVProfileName[Shader::MAX_PROFILES] =
{
	"",
	"vs_1_1",
	"vs_2_0",
	"vs_3_0",
	"arbvp1",
	"vp_gles2"
};

const std::string FxCompiler::msPProfileName[Shader::MAX_PROFILES] =
{
	"",
	"ps_1_1",
	"ps_2_0",
	"ps_3_0",
	"arbfp1",
	"fp_gles2"
};

const std::string FxCompiler::msVTName[Shader::VT_QUANTITY] =
{
	"VT_NONE",

	"VT_FLOAT",
	"VT_FLOAT1",   "VT_FLOAT2",   "VT_FLOAT3",   "VT_FLOAT4",
	"VT_FLOAT1X1", "VT_FLOAT1X2", "VT_FLOAT1X3", "VT_FLOAT1X4",
	"VT_FLOAT2X1", "VT_FLOAT2X2", "VT_FLOAT2X3", "VT_FLOAT2X4",
	"VT_FLOAT3X1", "VT_FLOAT3X2", "VT_FLOAT3X3", "VT_FLOAT3X4",
	"VT_FLOAT4X1", "VT_FLOAT4X2", "VT_FLOAT4X3", "VT_FLOAT4X4",

	"VT_HALF",
	"VT_HALF1",    "VT_HALF2",    "VT_HALF3",    "VT_HALF4",
	"VT_HALF1X1",  "VT_HALF1X2",  "VT_HALF1X3",  "VT_HALF1X4",
	"VT_HALF2X1",  "VT_HALF2X2",  "VT_HALF2X3",  "VT_HALF2X4",
	"VT_HALF3X1",  "VT_HALF3X2",  "VT_HALF3X3",  "VT_HALF3X4",
	"VT_HALF4X1",  "VT_HALF4X2",  "VT_HALF4X3",  "VT_HALF4X4",

	"VT_INT",
	"VT_INT1",     "VT_INT2",     "VT_INT3",     "VT_INT4",
	"VT_INT1X1",   "VT_INT1X2",   "VT_INT1X3",   "VT_INT1X4",
	"VT_INT2X1",   "VT_INT2X2",   "VT_INT2X3",   "VT_INT2X4",
	"VT_INT3X1",   "VT_INT3X2",   "VT_INT3X3",   "VT_INT3X4",
	"VT_INT4X1",   "VT_INT4X2",   "VT_INT4X3",   "VT_INT4X4",

	"VT_FIXED",
	"VT_FIXED1",   "VT_FIXED2",   "VT_FIXED3",   "VT_FIXED4",
	"VT_FIXED1X1", "VT_FIXED1X2", "VT_FIXED1X3", "VT_FIXED1X4",
	"VT_FIXED2X1", "VT_FIXED2X2", "VT_FIXED2X3", "VT_FIXED2X4",
	"VT_FIXED3X1", "VT_FIXED3X2", "VT_FIXED3X3", "VT_FIXED3X4",
	"VT_FIXED4X1", "VT_FIXED4X2", "VT_FIXED4X3", "VT_FIXED4X4",

	"VT_BOOL",
	"VT_BOOL1",    "VT_BOOL2",    "VT_BOOL3",    "VT_BOOL4",
	"VT_BOOL1X1",  "VT_BOOL1X2",  "VT_BOOL1X3",  "VT_BOOL1X4",
	"VT_BOOL2X1",  "VT_BOOL2X2",  "VT_BOOL2X3",  "VT_BOOL2X4",
	"VT_BOOL3X1",  "VT_BOOL3X2",  "VT_BOOL3X3",  "VT_BOOL3X4",
	"VT_BOOL4X1",  "VT_BOOL4X2",  "VT_BOOL4X3",  "VT_BOOL4X4"
};

const std::string FxCompiler::msVSName[Shader::VS_QUANTITY] =
{
	"VS_NONE",
	"VS_POSITION",
	"VS_BLENDWEIGHT",
	"VS_NORMAL",
	"VS_COLOR0",
	"VS_COLOR1", 
	"VS_FOGCOORD",
	"VS_PSIZE",
	"VS_BLENDINDICES",
	"VS_TEXCOORD0",
	"VS_TEXCOORD1",
	"VS_TEXCOORD2",
	"VS_TEXCOORD3",
	"VS_TEXCOORD4",
	"VS_TEXCOORD5",
	"VS_TEXCOORD6",
	"VS_TEXCOORD7",
	"VS_FOG",
	"VS_TANGENT",
	"VS_BINORMAL",
	"VS_COLOR2",
	"VS_COLOR3",
	"VS_DEPTH0"
};

const std::string FxCompiler::msSTName[Shader::ST_QUANTITY] =
{
	"ST_NONE",
	"ST_1D",
	"ST_2D",
	"ST_3D",
	"ST_CUBE"
};

FxCompiler::FxCompiler ()
{
	mCompileSucceeded = false;
	mActiveProfile = 0;

	for (int i=0; i<(int)Shader::MAX_PROFILES; ++i)
	{
		mCgVStatus[i] = -1;
		mCgPStatus[i] = -1;
	}

	InitializeMaps(); 
}
//----------------------------------------------------------------------------
FxCompiler::~FxCompiler ()
{
	mCurMaterial = 0;
}
//----------------------------------------------------------------------------
bool FxCompiler::Compile (const std::string &XMLName)
{
	bool success = mXMLData.LoadFile(XMLName);
	if (success)
	{
		mRootNode = mXMLData.GetRootNode();
	}
	else
	{
		assertion(false, "Failed to load %s.", XMLName);
	}

	bool compileSuccess = false;

	// material
	XMLNode materialNode = mRootNode;
	if (!materialNode.IsNull())
	{
		if (!ProcessMaterialNode(materialNode))
		{
			assertion(false, "Process material node error.");

			compileSuccess = false;
		}
	}
	else
	{
		assertion(false, "Material node error.");

		compileSuccess = false;

		return false;
	}

	const std::string &mtlName = mCurMaterial->GetName();
	string mtlFilename = mtlName + string(".px2obj");

	std::string resourcePath = "Data/engine_mtls/" + mtlName + "/" + mtlFilename;
	cout << mtlName << endl;

	mCurMaterial->SetResourcePath(resourcePath);

	OutStream outStream;
	outStream.Insert(mCurMaterial);
	outStream.Save(mtlFilename);

	return true;
}
//----------------------------------------------------------------------------
bool FxCompiler::ProcessMaterialNode (XMLNode materialNode)
{
	string materialName = materialNode.AttributeToString("name");

	mCurMaterial = new0 Material();
	mCurMaterial->SetName(materialName);

	// Technique
	XMLNode childNode = materialNode.IterateChild();
	while (!childNode.IsNull())
	{
		std::string childName = childNode.GetName();
		if ("technique" == childName)
		{
			if (!ProcessTechniqueNode(childNode))
			{
				assertion(false, "Process technique %s node failed.",
					childNode.AttributeToString("name"));

				return false;
			}
		}

		childNode = materialNode.IterateChild(childNode);
	}

	return true;
}
//----------------------------------------------------------------------------
bool FxCompiler::ProcessTechniqueNode (XMLNode techniqueNode)
{
	string techniqueName = techniqueNode.AttributeToString("name");

	MaterialTechniquePtr materialTechnique = new0 MaterialTechnique();
	materialTechnique->SetName(techniqueName);

	mCurMaterial->InsertTechnique(materialTechnique);

	// pass
	XMLNode passNode = techniqueNode.IterateChild();
	while (!passNode.IsNull())
	{
		if (!ProcessPassNode(passNode, materialTechnique))
		{
			assertion(false, "Process pass %s node failed.",
				passNode.AttributeToString("name"));

			return false;
		}

		passNode = techniqueNode.IterateChild(passNode);
	}

	return true;
}
//----------------------------------------------------------------------------
bool FxCompiler::ProcessPassNode (XMLNode passNode, MaterialTechnique* technique)
{
	string passName = passNode.AttributeToString("name");

	MaterialPassPtr materialPass = new0 MaterialPass();
	materialPass->SetName(passName);
	technique->InsertPass(materialPass);

	XMLNode shaderNode = passNode.IterateChild();
	while (!shaderNode.IsNull())
	{
		std::string shaderNodeName = shaderNode.GetName();
		if ("shader" == shaderNodeName)
		{
			std::string type = shaderNode.AttributeToString("type");
			std::string name = shaderNode.AttributeToString("name");
			int numInputs = shaderNode.AttributeToInt("ni");
			int numOutputs = shaderNode.AttributeToInt("no");
			int numConstants = shaderNode.AttributeToInt("nc");
			int numSamplers = shaderNode.AttributeToInt("ns");

			Shader *shader = 0;
			if ("vs" == type)
			{
				VertexShader *vs = new0 VertexShader(name, numInputs, numOutputs,
					numConstants, numSamplers, true);
				materialPass->SetVertexShader(vs);

				shader = vs;
			}
			else if ("ps" == type)
			{
				PixelShader *ps = new0 PixelShader(name, numInputs, numOutputs,
					numConstants, numSamplers, true);
				materialPass->SetPixelShader(ps);

				shader = ps;
			}

			shader->SetName(name);

			ProcessShaderNode(shaderNode, shader);
		}

		shaderNode = passNode.IterateChild(shaderNode);
	}

	// renderproperty
	XMLNode renderPropertyNode = passNode.GetChild("renderproperty");
	if (!renderPropertyNode.IsNull())
	{
		XMLNode propertyNode = renderPropertyNode.IterateChild();
		while (!propertyNode.IsNull())
		{
			ProcessRenderProperty(propertyNode, materialPass);

			propertyNode = renderPropertyNode.IterateChild(propertyNode);
		}
	}

	if (!materialPass->GetAlphaProperty())
		materialPass->SetAlphaProperty(new0 AlphaProperty());

	if (!materialPass->GetCullProperty())
		materialPass->SetCullProperty(new0 CullProperty());

	if (!materialPass->GetDepthProperty())
		materialPass->SetDepthProperty(new0 DepthProperty());

	if (!materialPass->GetOffsetProperty())
		materialPass->SetOffsetProperty(new0 OffsetProperty());

	if (!materialPass->GetStencilProperty())
		materialPass->SetStencilProperty(new0 StencilProperty());

	if (!materialPass->GetWireProperty())
		materialPass->SetWireProperty(new0 WireProperty());

	return true;
}
//----------------------------------------------------------------------------
bool FxCompiler::ProcessShaderNode(XMLNode shaderNode, Shader *shader)
{
	XMLNode childNode = shaderNode.IterateChild();
	while (!childNode.IsNull())
	{
		std::string name = childNode.GetName();
		if ("i" == name)
		{
			ProcessShaderNode_Inputs(childNode, shader);
		}
		else if ("o" == name)
		{
			ProcessShaderNode_Outputs(childNode, shader);
		}
		else if ("c" == name)
		{
			ProcessShaderNode_Constants(childNode, shader);
		}
		else if ("s" == name)
		{
			ProcessShaderNode_Samples(childNode, shader);
		}
		else if ("p" == name)
		{
			ProcessShaderNode_Program(childNode, shader);
		}

		childNode = shaderNode.IterateChild(childNode);
	}

	return true;
}
//----------------------------------------------------------------------------
bool FxCompiler::ProcessShaderNode_Inputs(XMLNode inputsNode, Shader *shader)
{
	XMLNode childNode = inputsNode.IterateChild();
	while (!childNode.IsNull())
	{
		std::string name = childNode.GetName();

		if ("var" == name)
		{
			int index = childNode.AttributeToInt("index");
			std::string paramName = childNode.AttributeToString("name");
			Shader::VariableType vt = mVariableTypes[childNode.AttributeToString("vt")];
			Shader::VariableSemantic vs = mSemantics[childNode.AttributeToString("vs")];

			shader->SetInput(index, paramName, vt, vs);
		}

		childNode = inputsNode.IterateChild(childNode);
	}

	return true;
}
//----------------------------------------------------------------------------
bool FxCompiler::ProcessShaderNode_Outputs(XMLNode outputsNode, Shader *shader)
{
	XMLNode childNode = outputsNode.IterateChild();
	while (!childNode.IsNull())
	{
		std::string name = childNode.GetName();

		if ("var" == name)
		{
			int index = childNode.AttributeToInt("index");
			std::string paramName = childNode.AttributeToString("name");
			Shader::VariableType vt = mVariableTypes[childNode.AttributeToString("vt")];
			Shader::VariableSemantic vs = mSemantics[childNode.AttributeToString("vs")];

			shader->SetOutput(index, paramName, vt, vs);
		}

		childNode = outputsNode.IterateChild(childNode);
	}

	return true;
}
//----------------------------------------------------------------------------
bool FxCompiler::ProcessShaderNode_Constants(XMLNode constantsNode, Shader *shader)
{
	VertexShader *vs = DynamicCast<VertexShader>(shader);
	bool iVS = 0 != vs;

	int numAllRegist = 0;

	XMLNode childNode = constantsNode.IterateChild();
	while (!childNode.IsNull())
	{
		std::string name = childNode.GetName();

		if ("var" == name)
		{
			int index = childNode.AttributeToInt("index");
			std::string paramName = childNode.AttributeToString("name");
			int numRegist = childNode.AttributeToInt("nr");

			shader->SetConstant(index, paramName, numRegist);

			for (int i = 1; i < Shader::MAX_PROFILES; ++i)
			{
				if (i <= (int)VertexShader::P_3_0)
				{
					shader->SetBaseRegister(i, index, numAllRegist);
				}
				else if (i == (int)VertexShader::P_ARBVP1)
				{
					int startReg = 0;
					if (iVS) startReg = 1;
					shader->SetBaseRegister(i, index, startReg + numAllRegist);
				}
				else if (i == (int)VertexShader::P_OPENGLES2)
				{
					int startReg = 0;
					if (iVS) startReg = 1;
					shader->SetBaseRegister(i, index, startReg + numAllRegist);
				}
			}

			numAllRegist += numRegist;
		}

		childNode = constantsNode.IterateChild(childNode);
	}

	return true;
}
//----------------------------------------------------------------------------
bool FxCompiler::ProcessShaderNode_Samples(XMLNode samplesNode, Shader *shader)
{
	XMLNode childNode = samplesNode.IterateChild();
	while (!childNode.IsNull())
	{
		std::string name = childNode.GetName();

		if ("var" == name)
		{
			int index = childNode.AttributeToInt("index");
			std::string sampleName = childNode.AttributeToString("name");

			std::string toFileST = childNode.AttributeToString("st");
			Shader::SamplerType samtype;
			SamplerTypeMap::iterator iter = mSamplerTypes.find(toFileST);
			if (iter == mSamplerTypes.end())
			{
				assertion(false, "Invalid sampler type: %s", toFileST.c_str());
				return false;
			}
			samtype = iter->second;
			shader->SetSampler(index, sampleName, samtype);

			std::string sf = childNode.AttributeToString("sf");
			Shader::SamplerFilter samfliter;
			SamplerFilterMap::iterator iterSF = mSamplerFilterTypes.find(sf);
			if (iterSF == mSamplerFilterTypes.end())
			{
				assertion(false, "Invalid SamplerFilter type: %s", toFileST.c_str());
				return false;
			}
			samfliter = iterSF->second;
			shader->SetFilter(index, samfliter);

			std::string sc_0 = childNode.AttributeToString("sc_0");
			std::string sc_1 = childNode.AttributeToString("sc_1");

			Shader::SamplerCoordinate sc0;
			SamplerCoordinateMap::iterator sc0_iter = mSamplerCoordinates.find(sc_0);
			if (sc0_iter == mSamplerCoordinates.end())
			{
				assertion(false, "Invalid SamplerCoordinate type: %s", sc_0.c_str());
				return false;
			}
			sc0 = sc0_iter->second;
			shader->SetCoordinate(index, 0, mSamplerCoordinates[sc_0]);

			Shader::SamplerCoordinate sc1;
			SamplerCoordinateMap::iterator sc1_iter = mSamplerCoordinates.find(sc_1);
			if (sc1_iter == mSamplerCoordinates.end())
			{
				assertion(false, "Invalid SamplerCoordinate type: %s", sc_1.c_str());
				return false;
			}
			sc1 = sc1_iter->second;
			shader->SetCoordinate(index, 1, mSamplerCoordinates[sc_1]);

			for (int i = 1; i < Shader::MAX_PROFILES; ++i)
			{
				shader->SetTextureUnit(i, index, index);
			}
		}

		childNode = samplesNode.IterateChild(childNode);
	}

	return true;
}
//----------------------------------------------------------------------------
bool FxCompiler::ProcessShaderNode_Program(XMLNode programNode, Shader *shader)
{
	const std::string &mtlName = mCurMaterial->GetName();

	VertexShader *vs = DynamicCast<VertexShader>(shader);
	bool isVertexShader = (0 != vs);

	XMLNode childNode = programNode.IterateChild();
	while (!childNode.IsNull())
	{
		std::string name = childNode.GetName();

		if ("var" == name)
		{
			std::string type = childNode.AttributeToString("type");
			std::string filename = childNode.AttributeToString("file");

			if ("cg" == type)
			{
				const std::string &entryname = shader->GetName();

				if (!CompileShader(isVertexShader, filename, entryname))
				{
					assertion(false, "Process pixelshader failed.");

					return false;
				}

				for (int i = 1; i <= Shader::P_ARBVP1; ++i)
				{
					Program program;

					std::string compiledFilename;
					std::string profileName = msVProfileName[i];
					if (!isVertexShader)
					{
						profileName = msPProfileName[i];
					}

					compiledFilename = entryname + "." + profileName + ".txt";

					shader->SetProgramFilename(i, "Data/engine_mtls/" + mtlName + "/" + compiledFilename);
				}
			}
			else if ("gles2" == type)
			{
				int profile = (int)Shader::P_OPENGLES2;
				shader->SetProgramFilename(profile, "Data/engine_mtls/" + mtlName + "/" + filename);
			}
		}

		childNode = programNode.IterateChild(childNode);
	}

	return true;
}
//----------------------------------------------------------------------------
bool FxCompiler::CompileShader (bool v, string filename, string shaderName)
{
	std::string command;

	for (int i=1; i<Shader::MAX_PROFILES; ++i)
	{
		if (i==5)
			continue;

		if (v)
		{
			mCurVertexShaderName = shaderName;

			// 如果存在旧的顶点着色器程序，将其删除
			//command = "del " + shaderName + "." + msVProfileName[i] + ".txt";
			//system(command.c_str());

			// 编译顶点着色器
			command = "cgc -profile " + msVProfileName[i];
			if (i == 1)
			{
				// VS_1_1 需要版本选项信息
				command += " -profileopts dcls";
			}
			command += " -entry v_" + shaderName + " -o " + shaderName + ".";
			command += msVProfileName[i] + ".txt " + filename;
			mCgVStatus[i] = system(command.c_str());
		}
		else
		{
			mCurPixelShaderName = shaderName;

			// 如果存在旧的像素着色器程序，将其删除
			//command = "del " + shaderName + "." + msPProfileName[1] + ".txt";
			//system(command.c_str());

			// 编译像素着色器
			command = "cgc -profile " + msPProfileName[i];
			command += " -entry p_" + shaderName + " -o " + shaderName + ".";
			command += msPProfileName[i] + ".txt " + filename;
			mCgPStatus[i] = system(command.c_str()); 
		}

	} // end for (i=1; i<Shader::MAX_PROFILES; ++i)

	return true;
}
//----------------------------------------------------------------------------
bool FxCompiler::ProcessRenderProperty (XMLNode nodeProperty, MaterialPass *pass)
{
	string propertyName = nodeProperty.GetName();

	if (propertyName == "Alpha")
	{
		AlphaPropertyPtr alphaProperty = new0 AlphaProperty();
		pass->SetAlphaProperty(alphaProperty);

		XMLNode childNode = nodeProperty.IterateChild();
		while (!childNode.IsNull())
		{
			if (string(childNode.AttributeToString("param")) == "BlendEnabled")
			{
				alphaProperty->BlendEnabled = childNode.AttributeToBool("value");
			}
			else if (string(childNode.AttributeToString("param")) == "SrcBlend")
			{
				for (int i = 0; i < (int)AlphaProperty::SBM_QUANTITY; i++)
				{
					if (childNode.AttributeToString("value") == msSrcBlendMode[i])
						alphaProperty->SrcBlend = (AlphaProperty::SrcBlendMode)i;
				}
			}
			else if (string(childNode.AttributeToString("param")) == "DstBlend")
			{
				for (int i = 0; i < (int)AlphaProperty::DBM_QUANTITY; i++)
				{
					if (childNode.AttributeToString("value") == msDstBlendMode[i])
						alphaProperty->DstBlend = (AlphaProperty::DstBlendMode)i;
				}
			}
			else if (string(childNode.AttributeToString("param")) == "CompareEnabled")
			{
				alphaProperty->CompareEnabled = childNode.AttributeToBool("value");
			}
			else if (string(childNode.AttributeToString("param")) == "Compare")
			{
				for (int i = 0; i < (int)AlphaProperty::CM_QUANTITY; i++)
				{
					if (string(childNode.AttributeToString("value")) == msAlphaCompareMode[i])
						alphaProperty->Compare = (AlphaProperty::CompareMode)i;
				}
			}
			else if (string(childNode.AttributeToString("param")) == "Reference")
			{
				alphaProperty->Reference = childNode.AttributeToFloat("value");
			}

			childNode = nodeProperty.IterateChild(childNode);
		}
	}
	else if (propertyName == "Cull")
	{
		CullPropertyPtr cullProperty = new0 CullProperty();
		pass->SetCullProperty(cullProperty);

		XMLNode childNode = nodeProperty.IterateChild();
		while (!childNode.IsNull())
		{
			if (string(childNode.AttributeToString("param")) == "Enabled")
			{
				cullProperty->Enabled = childNode.AttributeToBool("value");
			}
			else if (string(childNode.AttributeToString("param")) == "CCWOrder")
			{
				cullProperty->CCWOrder = childNode.AttributeToBool("value");
			}

			childNode = nodeProperty.IterateChild(childNode);
		}
	}
	else if (propertyName == "Depth")
	{
		DepthPropertyPtr depthProperty = new0 DepthProperty();
		pass->SetDepthProperty(depthProperty);

		XMLNode childNode = nodeProperty.IterateChild();
		while (!childNode.IsNull())
		{
			if (string(childNode.AttributeToString("param")) == "Enabled")
			{
				depthProperty->Enabled = childNode.AttributeToBool("value");
			}
			else if (string(childNode.AttributeToString("param")) == "Writable")
			{
				depthProperty->Writable = childNode.AttributeToBool("value");
			}
			else if (string(childNode.AttributeToString("param")) == "Compare")
			{
				for (int i = 0; i < (int)DepthProperty::CM_QUANTITY; i++)
				{
					if (string(childNode.AttributeToString("value")) == msDepthCompareMode[i])
					{
						depthProperty->Compare = DepthProperty::CompareMode(i);
					}
				}
			}

			childNode = nodeProperty.IterateChild(childNode);
		}
	}
	else if (propertyName == "Offset")
	{
		OffsetPropertyPtr offsetProperty = new0 OffsetProperty();
		pass->SetOffsetProperty(offsetProperty);

		XMLNode childNode = nodeProperty.IterateChild();
		while (!childNode.IsNull())
		{
			if (string(childNode.AttributeToString("param")) == "FillEnabled")
			{
				offsetProperty->FillEnabled = childNode.AttributeToBool("value");
			}
			else if (string(childNode.AttributeToString("param")) == "LineEnabled")
			{
				offsetProperty->LineEnabled = childNode.AttributeToBool("LineEnabled");
			}
			else if (string(childNode.AttributeToString("param")) == "PointEnabled")
			{
				offsetProperty->Scale = childNode.AttributeToFloat("Scale");
			}
			else if (string(childNode.AttributeToString("param")) == "Bias")
			{
				offsetProperty->Bias = childNode.AttributeToFloat("Bias");
			}

			childNode = nodeProperty.IterateChild(childNode);
		}
	}
	else if (propertyName == "Stencil")
	{
		StencilPropertyPtr stencilProperty = new0 StencilProperty();
		pass->SetStencilProperty(stencilProperty);

		XMLNode childNode = nodeProperty.IterateChild();
		while (!childNode.IsNull())
		{
			if (string(childNode.AttributeToString("param")) == "Enabled")
			{
				stencilProperty->Enabled = childNode.AttributeToBool("value");
			}
			else if (string(childNode.AttributeToString("param")) == "Compare")
			{
				for (int i = 0; i < (int)StencilProperty::CM_QUANTITY; i++)
				{
					if (childNode.AttributeToString("value") == msStencilCompareMode[i])
					{
						stencilProperty->Compare = (StencilProperty::CompareMode)i;
					}
				}
			}
			else if (string(childNode.AttributeToString("param")) == "Reference")
			{
				stencilProperty->Reference = childNode.AttributeToInt("Reference");
			}
			else if (string(childNode.AttributeToString("param")) == "Mask")
			{
				stencilProperty->Mask = childNode.AttributeToInt("value");
			}
			else if (string(childNode.AttributeToString("param")) == "WriteMask")
			{
				stencilProperty->WriteMask = childNode.AttributeToInt("value");
			}
			else if (string(childNode.AttributeToString("param")) == "OnFail")
			{
				for (int i = 0; i < (int)StencilProperty::OT_QUANTITY; i++)
				{
					if (childNode.AttributeToString("value") == msStencilOperationType[i])
						stencilProperty->OnFail = (StencilProperty::OperationType)i;
				}
			}
			else if (string(childNode.AttributeToString("param")) == "OnZFail")
			{
				for (int i = 0; i < (int)StencilProperty::OT_QUANTITY; i++)
				{
					if (string(childNode.AttributeToString("value")) == msStencilOperationType[i])
						stencilProperty->OnZFail = (StencilProperty::OperationType)i;
				}
			}
			else if (string(childNode.AttributeToString("param")) == "OnZPass")
			{
				for (int i = 0; i < (int)StencilProperty::OT_QUANTITY; i++)
				{
					if (string(childNode.AttributeToString("value")) == msStencilOperationType[i])
						stencilProperty->OnZPass = (StencilProperty::OperationType)i;
				}
			}

			childNode = nodeProperty.IterateChild(childNode);
		}
	}
	else if (propertyName == "Wire")
	{
		WirePropertyPtr wireProperty = new0 WireProperty();
		pass->SetWireProperty(wireProperty);

		XMLNode childNode = nodeProperty.IterateChild();
		while (!childNode.IsNull())
		{
			if (string(childNode.AttributeToString("param")) == "Enabled")
				wireProperty->Enabled = childNode.AttributeToBool("value");

			childNode = nodeProperty.IterateChild(childNode);
		}
	}

	return true;
}
//----------------------------------------------------------------------------
Float4 FxCompiler::StringToFloat4 (std::string value)
{
	Float4 color;
	//	char r = value.
	return color;
}
//----------------------------------------------------------------------------
void FxCompiler::InitializeMaps ()
{
	mVariableTypes.insert(std::make_pair("float", Shader::VT_FLOAT));
	mVariableTypes.insert(std::make_pair("float1", Shader::VT_FLOAT1));
	mVariableTypes.insert(std::make_pair("float2", Shader::VT_FLOAT2));
	mVariableTypes.insert(std::make_pair("float3", Shader::VT_FLOAT3));
	mVariableTypes.insert(std::make_pair("float4", Shader::VT_FLOAT4));
	mVariableTypes.insert(std::make_pair("float1x1", Shader::VT_FLOAT1X1));
	mVariableTypes.insert(std::make_pair("float1x2", Shader::VT_FLOAT1X2));
	mVariableTypes.insert(std::make_pair("float1x3", Shader::VT_FLOAT1X3));
	mVariableTypes.insert(std::make_pair("float1x4", Shader::VT_FLOAT1X4));
	mVariableTypes.insert(std::make_pair("float2x1", Shader::VT_FLOAT2X1));
	mVariableTypes.insert(std::make_pair("float2x2", Shader::VT_FLOAT2X2));
	mVariableTypes.insert(std::make_pair("float2x3", Shader::VT_FLOAT2X3));
	mVariableTypes.insert(std::make_pair("float2x4", Shader::VT_FLOAT2X4));
	mVariableTypes.insert(std::make_pair("float3x1", Shader::VT_FLOAT3X1));
	mVariableTypes.insert(std::make_pair("float3x2", Shader::VT_FLOAT3X2));
	mVariableTypes.insert(std::make_pair("float3x3", Shader::VT_FLOAT3X3));
	mVariableTypes.insert(std::make_pair("float3x4", Shader::VT_FLOAT3X4));
	mVariableTypes.insert(std::make_pair("float4x1", Shader::VT_FLOAT4X1));
	mVariableTypes.insert(std::make_pair("float4x2", Shader::VT_FLOAT4X2));
	mVariableTypes.insert(std::make_pair("float4x3", Shader::VT_FLOAT4X3));
	mVariableTypes.insert(std::make_pair("float4x4", Shader::VT_FLOAT4X4));

	mVariableTypes.insert(std::make_pair("half", Shader::VT_HALF));
	mVariableTypes.insert(std::make_pair("half1", Shader::VT_HALF1));
	mVariableTypes.insert(std::make_pair("half2", Shader::VT_HALF2));
	mVariableTypes.insert(std::make_pair("half3", Shader::VT_HALF3));
	mVariableTypes.insert(std::make_pair("half4", Shader::VT_HALF4));
	mVariableTypes.insert(std::make_pair("half1x1", Shader::VT_HALF1X1));
	mVariableTypes.insert(std::make_pair("half1x2", Shader::VT_HALF1X2));
	mVariableTypes.insert(std::make_pair("half1x3", Shader::VT_HALF1X3));
	mVariableTypes.insert(std::make_pair("half1x4", Shader::VT_HALF1X4));
	mVariableTypes.insert(std::make_pair("half2x1", Shader::VT_HALF2X1));
	mVariableTypes.insert(std::make_pair("half2x2", Shader::VT_HALF2X2));
	mVariableTypes.insert(std::make_pair("half2x3", Shader::VT_HALF2X3));
	mVariableTypes.insert(std::make_pair("half2x4", Shader::VT_HALF2X4));
	mVariableTypes.insert(std::make_pair("half3x1", Shader::VT_HALF3X1));
	mVariableTypes.insert(std::make_pair("half3x2", Shader::VT_HALF3X2));
	mVariableTypes.insert(std::make_pair("half3x3", Shader::VT_HALF3X3));
	mVariableTypes.insert(std::make_pair("half3x4", Shader::VT_HALF3X4));
	mVariableTypes.insert(std::make_pair("half4x1", Shader::VT_HALF4X1));
	mVariableTypes.insert(std::make_pair("half4x2", Shader::VT_HALF4X2));
	mVariableTypes.insert(std::make_pair("half4x3", Shader::VT_HALF4X3));
	mVariableTypes.insert(std::make_pair("half4x4", Shader::VT_HALF4X4));

	mVariableTypes.insert(std::make_pair("int", Shader::VT_INT));
	mVariableTypes.insert(std::make_pair("int1", Shader::VT_INT1));
	mVariableTypes.insert(std::make_pair("int2", Shader::VT_INT2));
	mVariableTypes.insert(std::make_pair("int3", Shader::VT_INT3));
	mVariableTypes.insert(std::make_pair("int4", Shader::VT_INT4));
	mVariableTypes.insert(std::make_pair("int1x1", Shader::VT_INT1X1));
	mVariableTypes.insert(std::make_pair("int1x2", Shader::VT_INT1X2));
	mVariableTypes.insert(std::make_pair("int1x3", Shader::VT_INT1X3));
	mVariableTypes.insert(std::make_pair("int1x4", Shader::VT_INT1X4));
	mVariableTypes.insert(std::make_pair("int2x1", Shader::VT_INT2X1));
	mVariableTypes.insert(std::make_pair("int2x2", Shader::VT_INT2X2));
	mVariableTypes.insert(std::make_pair("int2x3", Shader::VT_INT2X3));
	mVariableTypes.insert(std::make_pair("int2x4", Shader::VT_INT2X4));
	mVariableTypes.insert(std::make_pair("int3x1", Shader::VT_INT3X1));
	mVariableTypes.insert(std::make_pair("int3x2", Shader::VT_INT3X2));
	mVariableTypes.insert(std::make_pair("int3x3", Shader::VT_INT3X3));
	mVariableTypes.insert(std::make_pair("int3x4", Shader::VT_INT3X4));
	mVariableTypes.insert(std::make_pair("int4x1", Shader::VT_INT4X1));
	mVariableTypes.insert(std::make_pair("int4x2", Shader::VT_INT4X2));
	mVariableTypes.insert(std::make_pair("int4x3", Shader::VT_INT4X3));
	mVariableTypes.insert(std::make_pair("int4x4", Shader::VT_INT4X4));

	mVariableTypes.insert(std::make_pair("fixed", Shader::VT_FIXED));
	mVariableTypes.insert(std::make_pair("fixed1", Shader::VT_FIXED1));
	mVariableTypes.insert(std::make_pair("fixed2", Shader::VT_FIXED2));
	mVariableTypes.insert(std::make_pair("fixed3", Shader::VT_FIXED3));
	mVariableTypes.insert(std::make_pair("fixed4", Shader::VT_FIXED4));
	mVariableTypes.insert(std::make_pair("fixed1x1", Shader::VT_FIXED1X1));
	mVariableTypes.insert(std::make_pair("fixed1x2", Shader::VT_FIXED1X2));
	mVariableTypes.insert(std::make_pair("fixed1x3", Shader::VT_FIXED1X3));
	mVariableTypes.insert(std::make_pair("fixed1x4", Shader::VT_FIXED1X4));
	mVariableTypes.insert(std::make_pair("fixed2x1", Shader::VT_FIXED2X1));
	mVariableTypes.insert(std::make_pair("fixed2x2", Shader::VT_FIXED2X2));
	mVariableTypes.insert(std::make_pair("fixed2x3", Shader::VT_FIXED2X3));
	mVariableTypes.insert(std::make_pair("fixed2x4", Shader::VT_FIXED2X4));
	mVariableTypes.insert(std::make_pair("fixed3x1", Shader::VT_FIXED3X1));
	mVariableTypes.insert(std::make_pair("fixed3x2", Shader::VT_FIXED3X2));
	mVariableTypes.insert(std::make_pair("fixed3x3", Shader::VT_FIXED3X3));
	mVariableTypes.insert(std::make_pair("fixed3x4", Shader::VT_FIXED3X4));
	mVariableTypes.insert(std::make_pair("fixed4x1", Shader::VT_FIXED4X1));
	mVariableTypes.insert(std::make_pair("fixed4x2", Shader::VT_FIXED4X2));
	mVariableTypes.insert(std::make_pair("fixed4x3", Shader::VT_FIXED4X3));
	mVariableTypes.insert(std::make_pair("fixed4x4", Shader::VT_FIXED4X4));

	mVariableTypes.insert(std::make_pair("bool", Shader::VT_BOOL));
	mVariableTypes.insert(std::make_pair("bool1", Shader::VT_BOOL1));
	mVariableTypes.insert(std::make_pair("bool2", Shader::VT_BOOL2));
	mVariableTypes.insert(std::make_pair("bool3", Shader::VT_BOOL3));
	mVariableTypes.insert(std::make_pair("bool4", Shader::VT_BOOL4));
	mVariableTypes.insert(std::make_pair("bool1x1", Shader::VT_BOOL1X1));
	mVariableTypes.insert(std::make_pair("bool1x2", Shader::VT_BOOL1X2));
	mVariableTypes.insert(std::make_pair("bool1x3", Shader::VT_BOOL1X3));
	mVariableTypes.insert(std::make_pair("bool1x4", Shader::VT_BOOL1X4));
	mVariableTypes.insert(std::make_pair("bool2x1", Shader::VT_BOOL2X1));
	mVariableTypes.insert(std::make_pair("bool2x2", Shader::VT_BOOL2X2));
	mVariableTypes.insert(std::make_pair("bool2x3", Shader::VT_BOOL2X3));
	mVariableTypes.insert(std::make_pair("bool2x4", Shader::VT_BOOL2X4));
	mVariableTypes.insert(std::make_pair("bool3x1", Shader::VT_BOOL3X1));
	mVariableTypes.insert(std::make_pair("bool3x2", Shader::VT_BOOL3X2));
	mVariableTypes.insert(std::make_pair("bool3x3", Shader::VT_BOOL3X3));
	mVariableTypes.insert(std::make_pair("bool3x4", Shader::VT_BOOL3X4));
	mVariableTypes.insert(std::make_pair("bool4x1", Shader::VT_BOOL4X1));
	mVariableTypes.insert(std::make_pair("bool4x2", Shader::VT_BOOL4X2));
	mVariableTypes.insert(std::make_pair("bool4x3", Shader::VT_BOOL4X3));
	mVariableTypes.insert(std::make_pair("bool4x4", Shader::VT_BOOL4X4));

	mSemantics.insert(std::make_pair("POSITION", Shader::VS_POSITION));
	mSemantics.insert(std::make_pair("POSITION0", Shader::VS_POSITION));
	mSemantics.insert(std::make_pair("BLENDWEIGHT", Shader::VS_BLENDWEIGHT));
	mSemantics.insert(std::make_pair("NORMAL", Shader::VS_NORMAL));
	mSemantics.insert(std::make_pair("NORMAL0", Shader::VS_NORMAL));
	mSemantics.insert(std::make_pair("COLOR", Shader::VS_COLOR0));
	mSemantics.insert(std::make_pair("COLOR0", Shader::VS_COLOR0));
	mSemantics.insert(std::make_pair("COLOR1", Shader::VS_COLOR1));
	mSemantics.insert(std::make_pair("FOGCOORD", Shader::VS_FOGCOORD));
	mSemantics.insert(std::make_pair("PSIZE", Shader::VS_PSIZE));
	mSemantics.insert(std::make_pair("BLENDINDICES", Shader::VS_BLENDINDICES));
	mSemantics.insert(std::make_pair("TEXCOORD0", Shader::VS_TEXCOORD0));
	mSemantics.insert(std::make_pair("TEXCOORD1", Shader::VS_TEXCOORD1));
	mSemantics.insert(std::make_pair("TEXCOORD2", Shader::VS_TEXCOORD2));
	mSemantics.insert(std::make_pair("TEXCOORD3", Shader::VS_TEXCOORD3));
	mSemantics.insert(std::make_pair("TEXCOORD4", Shader::VS_TEXCOORD4));
	mSemantics.insert(std::make_pair("TEXCOORD5", Shader::VS_TEXCOORD5));
	mSemantics.insert(std::make_pair("TEXCOORD6", Shader::VS_TEXCOORD6));
	mSemantics.insert(std::make_pair("TEXCOORD7", Shader::VS_TEXCOORD7));
	mSemantics.insert(std::make_pair("FOG", Shader::VS_FOG));
	mSemantics.insert(std::make_pair("TANGENT", Shader::VS_TANGENT));
	mSemantics.insert(std::make_pair("TANGENT0", Shader::VS_TANGENT));
	mSemantics.insert(std::make_pair("BINORMAL", Shader::VS_BINORMAL));
	mSemantics.insert(std::make_pair("BINORMAL0", Shader::VS_BINORMAL));
	mSemantics.insert(std::make_pair("COLOR2", Shader::VS_COLOR2));
	mSemantics.insert(std::make_pair("COLOR3", Shader::VS_COLOR3));
	mSemantics.insert(std::make_pair("DEPTH0", Shader::VS_DEPTH0));

	mSamplerTypes.insert(std::make_pair("sampler1D", Shader::ST_1D));
	mSamplerTypes.insert(std::make_pair("sampler2D", Shader::ST_2D));
	mSamplerTypes.insert(std::make_pair("samplerSHADOW", Shader::ST_2D));
	mSamplerTypes.insert(std::make_pair("sampler3D", Shader::ST_3D));
	mSamplerTypes.insert(std::make_pair("samplerCUBE", Shader::ST_CUBE));

	mSamplerFilterTypes.insert(std::make_pair("NEAREST", Shader::SF_NEAREST));
	mSamplerFilterTypes.insert(std::make_pair("LINEAR", Shader::SF_LINEAR));
	mSamplerFilterTypes.insert(std::make_pair("NEAREST_NEAREST", Shader::SF_NEAREST_NEAREST));
	mSamplerFilterTypes.insert(std::make_pair("NEAREST_LINEAR", Shader::SF_NEAREST_LINEAR));
	mSamplerFilterTypes.insert(std::make_pair("LINEAR_NEAREST", Shader::SF_LINEAR_NEAREST));
	mSamplerFilterTypes.insert(std::make_pair("LINEAR_LINEAR", Shader::SF_LINEAR_LINEAR));

	mSamplerCoordinates.insert(std::make_pair("NONE", Shader::SC_NONE));
	mSamplerCoordinates.insert(std::make_pair("CLAMP", Shader::SC_CLAMP));
	mSamplerCoordinates.insert(std::make_pair("REPEAT", Shader::SC_REPEAT));
	mSamplerCoordinates.insert(std::make_pair("MIRRORED_REPEAT", Shader::SC_MIRRORED_REPEAT));
	mSamplerCoordinates.insert(std::make_pair("CLAMP_BORDER", Shader::SC_CLAMP_BORDER));
	mSamplerCoordinates.insert(std::make_pair("CLAMP_EDGE", Shader::SC_CLAMP_EDGE));
}
//----------------------------------------------------------------------------
bool FxCompiler::Parse (const std::string& fileName,
	const std::string& profileName, Program& program)
{
	std::ifstream inFile(fileName.c_str());
	if (!inFile)
	{
		// 文件不存在，表明这个版本的着色器编译失败。
		Messages.push_back("Profile " + profileName +
			" not supported Filename:" + fileName + "\n");
		return false;
	}

	program.Text = "";

	while (!inFile.eof())
	{
		// 获得一行
		std::string line;
		getline(inFile, line);

		if (line.empty())
			continue;

		// 任何非注释内容都是着色器程序的一部分。
		if (line[0] != '/' && line[0] != '#')
		{
			program.Text += line + "\n";
			continue;
		}

		std::vector<std::string> tokens;
		std::string::size_type begin;

		// 变量
		begin = line.find("var", 0);
		if (begin != std::string::npos)
		{
			GetTokens(line, begin, tokens);
			if (tokens.size() >= 2 && tokens[0] == "var")
			{
				std::string used = tokens.back();
				if (used == "0" || used == "1")
				{
					if (used == "1")
					{
						program.Variables.push_back(tokens);
					}
					continue;
				}
			}
			inFile.close();
			ReportError("Invalid variable line", &tokens);
			return false;
		}

		// 版本
		begin = line.find("profile", 0);
		if (begin != std::string::npos)
		{
			GetTokens(line, begin, tokens);
			if (tokens.size() >= 2 && tokens[0] == "profile")
			{
				// When the user has already compiled the programs, it is
				// because a profile is a special one.  The "!!ARBfp1.0"
				// string and the last token of "#profile specialProfile"
				// most likely do not match, so do not compare them.
				if (tokens[1] == profileName)
				{
					continue;
				}
			}
			inFile.close();
			ReportError("Invalid profile line", &tokens);
			return false;
		}

		// 着色器名称（着色器程序入口点）
		begin = line.find("program", 0);
		if (begin != std::string::npos)
		{
			GetTokens(line, begin, tokens);
			if (tokens.size() >= 2 && tokens[0] == "program")
			{
				program.Name = tokens[1];
				continue;
			}
			inFile.close();
			ReportError("Invalid program line", &tokens);
			return false;
		}
	}

	inFile.close();
	return true;
}
//----------------------------------------------------------------------------
void FxCompiler::GetTokens (const std::string& line,
	std::string::size_type begin, std::vector<std::string>& tokens)
{
	while (begin != std::string::npos)
	{
		// 跳过一个token，到下一个token
		std::string::size_type end = line.find(" ", begin);

		// Extract the token.
		std::string token = line.substr(begin, end - begin);
		tokens.push_back(token);

		// 跳过空格
		begin = line.find_first_not_of(" ", end);
	}
}
//----------------------------------------------------------------------------
bool FxCompiler::CreateShaders (const Program &vProgram, 
	const Program &pProgram,
	MaterialPass *pass)
{
	InputArray vInputs, pInputs;
	OutputArray vOutputs, pOutputs;
	ConstantArray vConstants, pConstants;
	SamplerArray vSamplers, pSamplers;

	if (!Process(vProgram, vInputs, vOutputs, vConstants, vSamplers))
	{
		return false;
	}

	if (!Process(pProgram, pInputs, pOutputs, pConstants, pSamplers))
	{
		return false;
	}

	VertexShaderPtr vertexShader = 
		(VertexShader*)CreateShader(true, vProgram, vInputs, vOutputs,
		vConstants, vSamplers);

	PixelShaderPtr pixelShader =
		(PixelShader*)CreateShader(false, pProgram, pInputs, pOutputs,
		pConstants, pSamplers);

	pass->SetVertexShader(vertexShader);
	pass->SetPixelShader(pixelShader);

	return true;
}
//----------------------------------------------------------------------------
bool FxCompiler::UpdateShaders (const Program &vProgram, 
	const Program &pProgram, VertexShader *vShader,
	PixelShader *pShader)
{
	InputArray vInputs, pInputs;
	OutputArray vOutputs, pOutputs;
	ConstantArray vConstants, pConstants;
	SamplerArray vSamplers, pSamplers;

	if (!Process(vProgram, vInputs, vOutputs, vConstants, vSamplers))
	{
		return false;
	}

	if (!Process(pProgram, pInputs, pOutputs, pConstants, pSamplers))
	{
		return false;
	}

	if (!UpdateShader(vShader, vProgram, vInputs, vOutputs, vConstants,
		vSamplers))
	{
		return false;
	}

	if (!UpdateShader(pShader, pProgram, pInputs, pOutputs, pConstants,
		pSamplers))
	{
		return false;
	}

	return true;
}
//----------------------------------------------------------------------------
bool FxCompiler::UpdateShader (Shader* shader, const Program& program,
	InputArray& inputs, OutputArray& outputs, 
	ConstantArray& constants, SamplerArray& samplers)
{
	int numInputs = (int)inputs.size();
	if (numInputs != shader->GetNumInputs())
	{
		ReportError("Mismatch in number of inputs.\n");
		return false;
	}

	int numOutputs = (int)outputs.size();
	if (numOutputs != shader->GetNumOutputs())
	{
		ReportError("Mismatch in number of outputs.\n");
		return false;
	}

	int numConstants = (int)constants.size();
	if (numConstants != shader->GetNumConstants())
	{
		ReportError("Mismatch in number of constants.\n");
		return false;
	}

	int numSamplers = (int)samplers.size();
	if (numSamplers != shader->GetNumSamplers())
	{
		ReportError("Mismatch in number of samplers.\n");
		return false;
	}

	std::string message;
	int i;
	for (i = 0; i < numInputs; ++i)
	{
		Input& input = inputs[i];
		if (input.Name != shader->GetInputName(i))
		{
			message =  "Mismatch in input names '" +
				input.Name +
				"' and '" +
				shader->GetInputName(i);

			ReportError(message);
			return false;
		}
		if (input.Type != shader->GetInputType(i))
		{
			message =  "Mismatch in input types '" +
				msVTName[input.Type] +
				"' and '" +
				msVTName[shader->GetInputType(i)];

			ReportError(message);
			return false;
		}
		if (input.Semantic != shader->GetInputSemantic(i))
		{
			message =  "Mismatch in input semantics '" +
				msVSName[input.Semantic] +
				"' and '" +
				msVSName[shader->GetInputSemantic(i)];

			ReportError(message);
			return false;
		}
	}

	for (i = 0; i < numOutputs; ++i)
	{
		Output& output = outputs[i];
		if (output.Name != shader->GetOutputName(i))
		{
			message =  "Mismatch in output names '" +
				output.Name +
				"' and '" +
				shader->GetOutputName(i);

			ReportError(message);
			return false;
		}
		if (output.Type != shader->GetOutputType(i))
		{
			message =  "Mismatch in output types '" +
				msVTName[output.Type] +
				"' and '" +
				msVTName[shader->GetOutputType(i)];

			ReportError(message);
			return false;
		}
		if (output.Semantic != shader->GetOutputSemantic(i))
		{
			message =  "Mismatch in output semantics '" +
				msVSName[output.Semantic] +
				"' and '" +
				msVSName[shader->GetOutputSemantic(i)];

			ReportError(message);
			return false;
		}
	}

	for (i = 0; i < numConstants; ++i)
	{
		Constant& constant = constants[i];
		if (constant.Name != shader->GetConstantName(i))
		{
			message =  "Mismatch in constant names '" +
				constant.Name +
				"' and '" +
				shader->GetConstantName(i);

			ReportError(message);
			return false;
		}
		if (constant.NumRegistersUsed != shader->GetNumRegistersUsed(i))
		{
			char number0[8], number1[8];
			sprintf(number0, "%d", constant.NumRegistersUsed);
			sprintf(number1, "%d", shader->GetNumRegistersUsed(i));
			message =  "Mismatch in constant registers used '" +
				std::string(number0) +
				"' and '" +
				std::string(number1);

			ReportError(message);
			return false;
		}
		shader->SetBaseRegister(mActiveProfile, i, constant.BaseRegister);
	}

	for (i = 0; i < numSamplers; ++i)
	{
		Sampler& sampler = samplers[i];
		if (sampler.Name != shader->GetSamplerName(i))
		{
			message =  "Mismatch in sampler names '" +
				sampler.Name +
				"' and '" +
				shader->GetSamplerName(i);

			ReportError(message);
			return false;
		}
		if (sampler.Type != shader->GetSamplerType(i))
		{
			message =  "Mismatch in sampler types '" +
				msSTName[sampler.Type] +
				"' and '" +
				msSTName[shader->GetSamplerType(i)];

			ReportError(message);
			return false;
		}
		shader->SetTextureUnit(mActiveProfile, i, sampler.Unit);
	}

	shader->SetProgram(mActiveProfile, program.Text);

	return true;
}
//----------------------------------------------------------------------------
bool FxCompiler::Process (const Program& program, InputArray& inputs,
	OutputArray& outputs, ConstantArray& constants,
	SamplerArray& samplers)
{
	// 变量行是下面形式之一：
	//   var TYPE NAME : $vin.SEMANTIC  : inputType           : index : 1
	//   var TYPE NAME : $vout.SEMANTIC : outputType          : index : 1
	//   var TYPE NAME :                : c[REGISTER]         : index : 1
	//   var TYPE NAME :                : c[REGISTER], NUMREG : index : 1
	//   var TYPE NAME :                : texunit UNITNUMBER  : -1    : 1
	//   var TYPE NAME :                :		  UNITNUMBER  : -1    : 1	（// ps_1_1中）
	// 最后一个值为“0”或者“1”。表示这个变量在这个着色器程序中是否被使用。

	TokenArrays::const_iterator iter = program.Variables.begin();
	TokenArrays::const_iterator end = program.Variables.end();
	for (/**/; iter != end; ++iter)
	{
		const TokenArray& tokens = *iter;

		// token数组拥有10或者11个token。
		if (tokens.size() < 10 || tokens.size() > 11)
		{
			ReportError("Invalid number of tokens", &tokens);
			return false;
		}

		// 获得变量类型
		Shader::VariableType vartype = Shader::VT_NONE;
		Shader::SamplerType samtype = Shader::ST_NONE;
		std::string::size_type begin = tokens[1].find("sampler", 0);
		if (begin != std::string::npos)
		{
			SamplerTypeMap::iterator iter = mSamplerTypes.find(tokens[1]);
			if (iter == mSamplerTypes.end())
			{
				ReportError("Invalid sampler type", &tokens);
				return false;
			}
			samtype = iter->second;
		}
		else
		{
			VariableTypeMap::iterator iter = mVariableTypes.find(tokens[1]);
			if (iter == mVariableTypes.end())
			{
				ReportError("Invalid variable type", &tokens);
				return false;
			}
			vartype = iter->second;
		}

		// 获得变量名称
		std::string name = tokens[2];

		// 检测变量时一个单一变量还是一个数组。如果是一个数组，我们需要确定使
		// 用多少寄存器。
		bool varArray;
		begin = name.find("[", 0);
		if (begin != std::string::npos)
		{
			varArray = true;
			name = name.substr(0, begin);  // 去掉 "[register]"
		}
		else
		{
			varArray = false;
		}

		// 分隔符
		if (tokens[3] != ":")
		{
			ReportError("Expecting separator character at index 3", &tokens);
			return false;
		}

		// 确定变量是输入变量还是输出变量。
		begin = tokens[4].find("$vin.", 0);
		if (begin != std::string::npos)
		{
			// 是输入变量
			if (!GetInput(tokens, name, vartype, inputs))
			{
				return false;
			}
			continue;
		}

		begin = tokens[4].find("$vout.", 0);
		if (begin != std::string::npos)
		{
			// 是输出变量
			if (!GetOutput(tokens, name, vartype, outputs))
			{
				return false;
			}
			continue;
		}

		if (tokens[4] == ":")
		{
			begin = tokens[1].find("sampler", 0);
			if (begin != std::string::npos)
			{
				// 变量是着色器纹理采样器
				if (!GetSampler(tokens, name, samtype, samplers))
				{
					return false;
				}
			}
			else
			{
				// 变量是着色器常量
				if (varArray)
				{
					if (constants.size() > 0 && name == constants.back().Name)
					{
						// 着色器常量数组的一个元素，只要紧接着上一个变量的末尾，
						// 增加寄存器数量
						++constants.back().NumRegistersUsed;
					}
					else
					{
						// 为第一次遇到常量创建着色器常量
						if (!GetConstant(tokens, name, vartype, constants))
						{
							return false;
						}
					}
				}
				else
				{
					if (!GetConstant(tokens, name, vartype, constants))
					{
						return false;
					}
				}
			}
			continue;
		}

		ReportError("Failed to find classifier", &tokens);
		return false;
	}

	return true;
}
//----------------------------------------------------------------------------
bool FxCompiler::GetInput (const TokenArray& tokens, const std::string& name,
	Shader::VariableType type, InputArray& inputs)
{
	std::string::size_type begin = 5;  // 跳过"$vin."
	std::string semanticName = tokens[4].substr(begin, std::string::npos);
	SemanticMap::iterator siter = mSemantics.find(semanticName);
	if (siter == mSemantics.end())
	{
		ReportError("Invalid $vin classifier", &tokens);
		return false;
	}

	Input input;
	input.Name = name;
	input.Type = type;
	input.Semantic = siter->second;
	inputs.push_back(input);
	return true;
}
//----------------------------------------------------------------------------
bool FxCompiler::GetOutput (const TokenArray& tokens, const std::string& name,
	Shader::VariableType type, OutputArray& outputs)
{
	std::string::size_type begin = 6;  // skip over "$vout."
	std::string semanticName = tokens[4].substr(begin, std::string::npos);
	SemanticMap::iterator siter = mSemantics.find(semanticName);
	if (siter == mSemantics.end())
	{
		ReportError("Invalid $vout classifier", &tokens);
		return false;
	}

	Output output;
	output.Name = name;
	output.Type = type;
	output.Semantic = siter->second;
	outputs.push_back(output);
	return true;
}
//----------------------------------------------------------------------------
bool FxCompiler::GetConstant (const TokenArray& tokens,
	const std::string& name, Shader::VariableType type,
	ConstantArray& constants)
{
	std::string::size_type begin, end;

	if (tokens[5].size() < 4
		||  tokens[5][0] != 'c'
		||  tokens[5][1] != '['
		||  (end = tokens[5].find("]", 0)) == std::string::npos)
	{
		ReportError("Expecting 'c[register]' token", &tokens);
		return false;
	}

	// 获得常量寄存器索引
	begin = 2; // '['后面一个字符索引
	std::string number = tokens[5].substr(begin, end - begin);
	int baseRegister = atoi(number.c_str());
	if (baseRegister == 0 && number != "0")
	{
		ReportError("Invalid base register", &tokens);
		return false;
	}

	// 常量寄存器单元的个数（一个寄存器单元为一个float4）
	int numRegistersUsed;
	if (tokens[5].find(",", 0) == std::string::npos)
	{
		// 这个常量使用一个float4
		numRegistersUsed = 1;
	}
	else
	{
		// 常量使用多个寄存器单元
		numRegistersUsed = atoi(tokens[6].c_str());
		if (numRegistersUsed == 0)
		{
			ReportError("Invalid number of registers", &tokens);
			return false;
		}
	}

	Constant constant;
	constant.Name = name;
	constant.Type = type;
	constant.BaseRegister = baseRegister;
	constant.NumRegistersUsed = numRegistersUsed;
	constants.push_back(constant);
	return true;
}
//----------------------------------------------------------------------------
bool FxCompiler::GetSampler (const TokenArray& tokens,
	const std::string& name, Shader::SamplerType type,
	SamplerArray& samplers)
{
	int unit;

	if (mActiveProfile != 1)
	{
		if (tokens[5] != "texunit")
		{
			ReportError("Expecting 'texunit' token", &tokens);
			return false;
		}

		// 获得采样器的贴图的unit
		unit = atoi(tokens[6].c_str());
		if (unit == 0 && tokens[6] != "0")
		{
			ReportError("Invalid texture unit", &tokens);
			return false;
		}
	}
	else  // ps_1_1 (vs_1_1 不支持纹理采样)
	{
		// 获得采样器的贴图的unit
		unit = atoi(tokens[5].c_str());
		if (unit == 0 && tokens[5] != "0")
		{
			ReportError("Invalid texture unit", &tokens);
			return false;
		}
	}

	Sampler sampler;
	sampler.Name = name;
	sampler.Type = type;
	sampler.Unit = unit;
	sampler.Filter = Shader::SF_NEAREST;
	sampler.Coordinate[0] = Shader::SC_CLAMP_EDGE;
	sampler.Coordinate[1] = Shader::SC_CLAMP_EDGE;
	sampler.Coordinate[2] = Shader::SC_CLAMP_EDGE;
	sampler.LodBias = 0.0f;
	sampler.Anisotropy = 1.0f;
	sampler.BorderColor = Float4(0.0f, 0.0f, 0.0f, 0.0f);
	samplers.push_back(sampler);
	return true;
}
//----------------------------------------------------------------------------
Shader* FxCompiler::CreateShader (bool isVShader, const Program& program,
	InputArray& inputs, OutputArray& outputs, ConstantArray& constants,
	SamplerArray& samplers)
{
	int numInputs = (int)inputs.size();
	int numOutputs = (int)outputs.size();
	int numConstants = (int)constants.size();
	int numSamplers = (int)samplers.size();

	Shader* shader;
	if (isVShader)
	{
		shader = new0 VertexShader(program.Name, numInputs, numOutputs,
			numConstants, numSamplers, true);
	}
	else
	{
		shader = new0 PixelShader(program.Name, numInputs, numOutputs,
			numConstants, numSamplers, true);
	}

	int i;
	for (i = 0; i < numInputs; ++i)
	{
		Input& input = inputs[i];
		shader->SetInput(i, input.Name, input.Type, input.Semantic);
	}

	for (i = 0; i < numOutputs; ++i)
	{
		Output& output = outputs[i];
		shader->SetOutput(i, output.Name, output.Type, output.Semantic);
	}

	for (i = 0; i < numConstants; ++i)
	{
		Constant& constant = constants[i];
		shader->SetConstant(i, constant.Name, constant.NumRegistersUsed);
		shader->SetBaseRegister(mActiveProfile, i, constant.BaseRegister);
	}

	for (i = 0; i < numSamplers; ++i)
	{
		Sampler& sampler = samplers[i];
		shader->SetSampler(i, sampler.Name, sampler.Type);
		shader->SetFilter(i, sampler.Filter);
		shader->SetCoordinate(i, 0, sampler.Coordinate[0]);
		shader->SetCoordinate(i, 1, sampler.Coordinate[1]);
		shader->SetCoordinate(i, 2, sampler.Coordinate[2]);
		shader->SetLodBias(i, sampler.LodBias);
		shader->SetAnisotropy(i, sampler.Anisotropy);
		shader->SetBorderColor(i, sampler.BorderColor);
		shader->SetTextureUnit(mActiveProfile, i, sampler.Unit);
	}

	shader->SetProgram(mActiveProfile, program.Text);
	return shader;
}
//----------------------------------------------------------------------------
void FxCompiler::ReportError (const std::string& message,
	const TokenArray* tokens)
{
	Messages.push_back(message);
	if (tokens)
	{
		Messages.push_back(":");
		for (int i = 0; i < (int)tokens->size(); ++i)
		{
			Messages.push_back(" " + (*tokens)[i]);
		}
	}
	Messages.push_back("\n");

	assertion(false, "FxCompiler error.\n");
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FxCompiler::Program
//----------------------------------------------------------------------------
FxCompiler::Program::Program () :
Name(""),
Text("")
{
}
//----------------------------------------------------------------------------