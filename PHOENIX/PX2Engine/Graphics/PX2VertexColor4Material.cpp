// PX2VertexColor4Material.cpp

#include "PX2VertexColor4Material.hpp"
#include "PX2PVWMatrixConstant.hpp"
#include "PX2ShaderKeys.hpp"

using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Material, VertexColor4Material);
PX2_IMPLEMENT_STREAM(VertexColor4Material);
PX2_IMPLEMENT_FACTORY(VertexColor4Material);
PX2_IMPLEMENT_DEFAULT_NAMES(Material, VertexColor4Material);
//----------------------------------------------------------------------------
VertexColor4Material::VertexColor4Material ()
{
	SetName("VertexColor4Material");

	VertexShader* vshader = new0 VertexShader("PX2.VertexColor4",
		2, 2, 1, 0, false);
	vshader->SetInput(0, "modelPosition", Shader::VT_FLOAT3,
		Shader::VS_POSITION);
	vshader->SetInput(1, "modelColor0", Shader::VT_FLOAT4,
		Shader::VS_COLOR0);
	vshader->SetOutput(0, "clipPosition", Shader::VT_FLOAT4,
		Shader::VS_POSITION);
	vshader->SetOutput(1, "vertexColor", Shader::VT_FLOAT3,
		Shader::VS_COLOR0);
	vshader->SetConstant(0, "PVWMatrix", 4);
	vshader->SetBaseRegisters(msVRegisters);
	vshader->SetPrograms(msVPrograms);

	PixelShader* pshader = new0 PixelShader("PX2.VertexColor4",
		1, 1, 0, 0, false);
	pshader->SetInput(0, "vertexColor", Shader::VT_FLOAT4,
		Shader::VS_COLOR0);
	pshader->SetOutput(0, "pixelColor", Shader::VT_FLOAT4,
		Shader::VS_COLOR0);
	pshader->SetPrograms(msPPrograms);

	MaterialPass* pass = new0 MaterialPass();
	pass->SetVertexShader(vshader);
	pass->SetPixelShader(pshader);
	pass->SetAlphaProperty(new0 AlphaProperty());
	pass->SetCullProperty(new0 CullProperty());
	pass->SetDepthProperty(new0 DepthProperty());
	pass->SetOffsetProperty(new0 OffsetProperty());
	pass->SetStencilProperty(new0 StencilProperty());
	pass->SetWireProperty(new0 WireProperty());

	MaterialTechnique* technique = new0 MaterialTechnique();
	technique->InsertPass(pass);
	InsertTechnique(technique);

	vshader->SetShaderKey(SKT_VERTEXCOLOR4);
	pshader->SetShaderKey(SKT_VERTEXCOLOR4);
}
//----------------------------------------------------------------------------
VertexColor4Material::~VertexColor4Material ()
{
}
//----------------------------------------------------------------------------
MaterialInstance* VertexColor4Material::CreateInstance () const
{
	MaterialInstance* instance = new0 MaterialInstance(this, 0);
	instance->SetVertexConstant(0, "PVWMatrix", new0 PVWMatrixConstant());
	return instance;
}
//----------------------------------------------------------------------------
MaterialInstance* VertexColor4Material::CreateUniqueInstance ()
{
	VertexColor4Material* instance = new0 VertexColor4Material();
	return instance->CreateInstance();
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
VertexColor4Material::VertexColor4Material (LoadConstructor value)
:
Material(value)
{
}
//----------------------------------------------------------------------------
void VertexColor4Material::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Material::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(VertexColor4Material, source);
}
//----------------------------------------------------------------------------
void VertexColor4Material::Link (InStream& source)
{
	Material::Link(source);
}
//----------------------------------------------------------------------------
void VertexColor4Material::PostLink ()
{
	Material::PostLink();

	MaterialPass* pass = mTechniques[0]->GetPass(0);
	VertexShader* vshader = pass->GetVertexShader();
	PixelShader* pshader = pass->GetPixelShader();
	vshader->SetBaseRegisters(msVRegisters);
	vshader->SetPrograms(msVPrograms);
	pshader->SetPrograms(msPPrograms);

	vshader->SetShaderKey(SKT_VERTEXCOLOR4);
	pshader->SetShaderKey(SKT_VERTEXCOLOR4);
}
//----------------------------------------------------------------------------
bool VertexColor4Material::Register (OutStream& target) const
{
	return Material::Register(target);
}
//----------------------------------------------------------------------------
void VertexColor4Material::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Material::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(VertexColor4Material, target);
}
//----------------------------------------------------------------------------
int VertexColor4Material::GetStreamingSize (Stream &stream) const
{
	int size = Material::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	
	return size;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// °æ±¾
//----------------------------------------------------------------------------
int VertexColor4Material::msDx9VRegisters[1] = { 0 };
int VertexColor4Material::msOglVRegisters[1] = { 1 };
int VertexColor4Material::msOpenGLES2VRegisters[1] = { 1 };
int* VertexColor4Material::msVRegisters[Shader::MAX_PROFILES] =
{
	0,
	msDx9VRegisters,
	msDx9VRegisters,
	msDx9VRegisters,
	msOglVRegisters,
	msOpenGLES2VRegisters
};

std::string VertexColor4Material::msVPrograms[Shader::MAX_PROFILES] =
{
	// VP_NONE
	"",

	// VP_VS_1_1
	"vs_1_1\n"
	"def c4, 1.00000000, 0, 0, 0\n"
	"dcl_position0 v0\n"
	"dcl_color0 v1\n"
	"mov r0.w, c4.x\n"
	"mov r0.xyz, v0\n"
	"dp4 oPos.w, r0, c3\n"
	"dp4 oPos.z, r0, c2\n"
	"dp4 oPos.y, r0, c1\n"
	"dp4 oPos.x, r0, c0\n"
	"mov oD0, v1\n",

	// VP_VS_2_0
	"vs_2_0\n"
	"def c4, 1.00000000, 0, 0, 0\n"
	"dcl_position v0\n"
	"dcl_color v1\n"
	"mov r0.w, c4.x\n"
	"mov r0.xyz, v0\n"
	"dp4 oPos.w, r0, c3\n"
	"dp4 oPos.z, r0, c2\n"
	"dp4 oPos.y, r0, c1\n"
	"dp4 oPos.x, r0, c0\n"
	"mov oD0, v1\n",

	// VP_VS_3_0
	"vs_3_0\n"
	"dcl_position o0\n"
	"dcl_color0 o1\n"
	"def c4, 1.00000000, 0, 0, 0\n"
	"dcl_position0 v0\n"
	"dcl_color0 v1\n"
	"mov r0.w, c4.x\n"
	"mov r0.xyz, v0\n"
	"dp4 o0.w, r0, c3\n"
	"dp4 o0.z, r0, c2\n"
	"dp4 o0.y, r0, c1\n"
	"dp4 o0.x, r0, c0\n"
	"mov o1, v1\n",

	// VP_ARBVP1
	"!!ARBvp1.0\n"
	"PARAM c[5] = { { 1 }, program.local[1..4] };\n"
	"TEMP R0;\n"
	"MOV R0.w, c[0].x;\n"
	"MOV R0.xyz, vertex.position;\n"
	"DP4 result.position.w, R0, c[4];\n"
	"DP4 result.position.z, R0, c[3];\n"
	"DP4 result.position.y, R0, c[2];\n"
	"DP4 result.position.x, R0, c[1];\n"
	"MOV result.color, vertex.color;\n"
	"END\n",

	// vp_gles2
	"uniform mat4 PVWMatrix;\n"
	"attribute vec3 modelPosition;\n"
	"attribute vec4 modelColor0;\n"
	"varying vec4 vertexColor;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = PVWMatrix*vec4(modelPosition, 1.0);\n"
	"	vertexColor = modelColor0;\n"
	"}\n"
};

std::string VertexColor4Material::msPPrograms[Shader::MAX_PROFILES] =
{
	// PP_NONE
	"",

	// PP_PS_1_1
	"ps.1.1\n"
	"mov r0, v0\n",

	// PP_PS_2_0
	"ps_2_0\n"
	"dcl v0\n"
	"mov oC0, v0\n",

	// PP_PS_3_0
	"ps_3_0\n"
	"dcl_color0 v0\n"
	"mov oC0, v0\n",

	// PP_ARBFP1
	"!!ARBfp1.0\n"
	"MOV result.color, fragment.color.primary;\n"
	"END\n",

	// fp_gles2
	"varying mediump vec4 vertexColor;\n"
	"void main()\n"
	"{\n"
	"	gl_FragColor = vertexColor;\n"
	"}\n"
};
//----------------------------------------------------------------------------
