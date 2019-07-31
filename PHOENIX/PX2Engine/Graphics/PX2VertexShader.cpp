// PX2VertexShader.cpp

#include "PX2VertexShader.hpp"
#include "PX2Renderer.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Shader, VertexShader);
PX2_IMPLEMENT_STREAM(VertexShader);
PX2_IMPLEMENT_FACTORY(VertexShader);
PX2_IMPLEMENT_DEFAULT_NAMES(Shader, VertexShader);
PX2_IMPLEMENT_DEFAULT_STREAM(Shader, VertexShader);

VertexShader::Profile VertexShader::msProfile = VertexShader::VP_NONE;

//----------------------------------------------------------------------------
VertexShader::VertexShader (const std::string& programName, int numInputs,
							int numOutputs, int numConstants, int numSamplers, bool profileOwner)
							:
Shader(programName, numInputs, numOutputs, numConstants, numSamplers,
	   profileOwner)
{
}
//----------------------------------------------------------------------------
VertexShader::~VertexShader ()
{
	Renderer::UnbindAll(this);
}
//----------------------------------------------------------------------------
void VertexShader::SetProfile (Profile profile)
{
	msProfile = profile;
}
//----------------------------------------------------------------------------
VertexShader::Profile VertexShader::GetProfile ()
{
	return msProfile;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void VertexShader::RegistProperties ()
{
	Shader::RegistProperties();
	AddPropertyClass("VertexShader");

	std::vector<std::string> profiles;
	profiles.push_back("VP_NONE");
	profiles.push_back("VP_VS_1_1");
	profiles.push_back("VP_VS_2_0");
	profiles.push_back("VP_VS_3_0");
	profiles.push_back("VP_ARBVP1");
	profiles.push_back("VP_OPENGLES2");

	AddPropertyEnum("Profile", (int)GetProfile(), profiles, false);
}
//----------------------------------------------------------------------------
void VertexShader::OnPropertyChanged (const PropertyObject &obj)
{
	Shader::OnPropertyChanged(obj);
}
//----------------------------------------------------------------------------