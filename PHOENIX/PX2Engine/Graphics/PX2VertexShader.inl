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
