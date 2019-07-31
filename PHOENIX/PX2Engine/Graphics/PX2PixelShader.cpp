// PX2PixelShader.cpp

#include "PX2PixelShader.hpp"
#include "PX2Renderer.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Shader, PixelShader);
PX2_IMPLEMENT_STREAM(PixelShader);
PX2_IMPLEMENT_FACTORY(PixelShader);
PX2_IMPLEMENT_DEFAULT_NAMES(Shader, PixelShader);
PX2_IMPLEMENT_DEFAULT_STREAM(Shader, PixelShader);

PixelShader::Profile PixelShader::msProfile = PixelShader::PP_NONE;

//----------------------------------------------------------------------------
PixelShader::PixelShader (const std::string& programName, int numInputs,
						  int numOutputs, int numConstants, int numSamplers,
						  bool profileOwner) :
Shader(programName, numInputs, numOutputs, numConstants, numSamplers,
	   profileOwner)
{
}
//----------------------------------------------------------------------------
PixelShader::~PixelShader ()
{
	Renderer::UnbindAll(this);
}
//----------------------------------------------------------------------------
void PixelShader::SetProfile (Profile profile)
{
	msProfile = profile;
}
//----------------------------------------------------------------------------
PixelShader::Profile PixelShader::GetProfile ()
{
	return msProfile;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void PixelShader::RegistProperties ()
{
	Shader::RegistProperties();
	AddPropertyClass("PixelShader");

	std::vector<std::string> profiles;
	profiles.push_back("PP_NONE");
	profiles.push_back("PP_VS_1_1");
	profiles.push_back("PP_VS_2_0");
	profiles.push_back("PP_VS_3_0");
	profiles.push_back("PP_ARBVP1");
	profiles.push_back("PP_OPENGLES2");

	AddPropertyEnum("Profile", (int)GetProfile(), profiles, false);
}
//----------------------------------------------------------------------------
void PixelShader::OnPropertyChanged (const PropertyObject &obj)
{
	Shader::OnPropertyChanged(obj);
}
//----------------------------------------------------------------------------
