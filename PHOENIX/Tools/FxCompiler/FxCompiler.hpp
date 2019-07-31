// FxCompiler.hpp

#ifndef FXCOMPILER_HPP
#define FXCOMPILER_HPP

#include "PX2Material.hpp"
#include "PX2XMLData.hpp"
#include "PX2XMLNode.hpp"
using namespace PX2;
using namespace std;

class FxCompiler
{
public:
	FxCompiler ();
	~FxCompiler ();

	bool Compile (const std::string &XMLName);

	std::vector<std::string> Messages;

private:
	typedef std::vector<std::string> TokenArray;
	typedef std::vector<TokenArray> TokenArrays;

	class Program
	{
	public:
		Program ();

		std::string Name;
		std::string Text;
		TokenArrays Variables;
	};

	class Input
	{
	public:
		std::string Name;
		Shader::VariableType Type;
		Shader::VariableSemantic Semantic;
	};
	typedef std::vector<Input> InputArray;

	class Output
	{
	public:
		std::string Name;
		Shader::VariableType Type;
		Shader::VariableSemantic  Semantic;
	};
	typedef std::vector<Output> OutputArray;

	class Constant
	{
	public:
		std::string Name;
		Shader::VariableType Type;
		int BaseRegister;
		int NumRegistersUsed;
	};
	typedef std::vector<Constant> ConstantArray;

	class Sampler
	{
	public:
		std::string Name;
		Shader::SamplerType Type;
		int Unit;
		Shader::SamplerFilter Filter;
		Shader::SamplerCoordinate Coordinate[3];
		float LodBias;
		float Anisotropy;
		Float4 BorderColor;
	};
	typedef std::vector<Sampler> SamplerArray;

private:
	bool ProcessMaterialNode (XMLNode materialNode);
	bool ProcessTechniqueNode (XMLNode techniqueNode);
	bool ProcessPassNode (XMLNode passNode, MaterialTechnique *technique);
	bool ProcessShaderNode(XMLNode shaderNode, Shader *shader);
	bool ProcessShaderNode_Inputs(XMLNode inputsNode, Shader *shader);
	bool ProcessShaderNode_Outputs(XMLNode outputsNode, Shader *shader);
	bool ProcessShaderNode_Constants(XMLNode constantsNode, Shader *shader);
	bool ProcessShaderNode_Samples(XMLNode samplesNode, Shader *shader);
	bool ProcessShaderNode_Program(XMLNode programsNode, Shader *shader);
	bool CompileShader (bool v, string filename, string shaderName);
	bool ProcessRenderProperty(XMLNode renderPropertyNode, MaterialPass *pass);

	//bool CheakShaderPairs ();
	//bool UpdateMaterialPass (string vertexShaderName, string pixelShaderName,
	//	MaterialPass *pass);
	Float4 StringToFloat4 (std::string value);

	XMLData mXMLData;
	XMLNode mRootNode;
	MaterialPtr mCurMaterial;
	bool mCompileSucceeded;
	string mCurVertexShaderName; //< 当前正在处理的顶点着色器名称
	string mCurPixelShaderName; //< 当前正在处理的像素着色器名称
	int mCgVStatus[Shader::MAX_PROFILES];
	int mCgPStatus[Shader::MAX_PROFILES];
	int mActiveProfile;

	static const std::string msSrcBlendMode[AlphaProperty::SBM_QUANTITY];
	static const std::string msDstBlendMode[AlphaProperty::DBM_QUANTITY];
	static const std::string msAlphaCompareMode[AlphaProperty::CM_QUANTITY];
	static const std::string msDepthCompareMode[DepthProperty::CM_QUANTITY];
	static const std::string msStencilCompareMode[StencilProperty::CM_QUANTITY];
	static const std::string msStencilOperationType[StencilProperty::OT_QUANTITY];

	void InitializeMaps ();

	bool Parse (const std::string &fileName, const std::string &profileName,
		Program &program);
	void GetTokens (const std::string &line, std::string::size_type begin,
		std::vector<std::string> &tokens);

	// 更新材质
	bool CreateShaders (const Program &vProgram, const Program &pProgram,
		MaterialPass *pass);
	bool UpdateShaders (const Program &vProgram, const Program &pProgram,
		VertexShader *vShader, PixelShader *pShader);
	bool UpdateShader (Shader* shader, const Program& program,
		InputArray& inputs, OutputArray& outputs, ConstantArray& constants,
		SamplerArray& samplers);
	bool Process (const Program& program, InputArray& inputs,
		OutputArray& outputs, ConstantArray& constants,
		SamplerArray& samplers);

	// shader
	bool GetInput (const TokenArray& tokens, const std::string& name,
		Shader::VariableType type, InputArray& inputs);
	bool GetOutput (const TokenArray& tokens, const std::string& name,
		Shader::VariableType type, OutputArray& outputs);
	bool GetConstant (const TokenArray& tokens, const std::string& name,
		Shader::VariableType type, ConstantArray& constants);
	bool GetSampler (const TokenArray& tokens, const std::string& name,
		Shader::SamplerType type, SamplerArray& samplers);
	Shader *CreateShader (bool isVShader, const Program &program,
		InputArray &inputs, OutputArray &outputs, ConstantArray &constants,
		SamplerArray &samplers);

	// 出错
	void ReportError (const std::string &message, 
		const TokenArray *tokens = 0);

	typedef std::map<std::string, Shader::VariableType> VariableTypeMap;
	VariableTypeMap mVariableTypes;

	typedef std::map<std::string, Shader::VariableSemantic> SemanticMap;
	SemanticMap mSemantics;

	typedef std::map<std::string, Shader::SamplerType> SamplerTypeMap;
	SamplerTypeMap mSamplerTypes;

	typedef std::map<std::string, Shader::SamplerFilter> SamplerFilterMap;
	SamplerFilterMap mSamplerFilterTypes;

	typedef std::map<std::string, Shader::SamplerCoordinate> SamplerCoordinateMap;
	SamplerCoordinateMap mSamplerCoordinates;

	static const std::string msVProfileName[Shader::MAX_PROFILES];	// shader编译版本名称
	static const std::string msPProfileName[Shader::MAX_PROFILES];	// 同上
	static const std::string msVTName[Shader::VT_QUANTITY];			// 变量数据类型名称
	static const std::string msVSName[Shader::VS_QUANTITY];			// 变量语义名称
	static const std::string msSTName[Shader::ST_QUANTITY];			// 纹理采样器类型名称
};

#endif