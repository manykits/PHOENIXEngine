// PX2ProjectorMatrixConstant.cpp

#include "PX2ProjectorMatrixConstant.hpp"
#include "PX2Camera.hpp"
#include "PX2Renderable.hpp"
#include "PX2GraphicsRoot.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, ShaderFloat, ProjectorMatrixConstant);
PX2_IMPLEMENT_STREAM(ProjectorMatrixConstant);
PX2_IMPLEMENT_FACTORY(ProjectorMatrixConstant);

//----------------------------------------------------------------------------
ProjectorMatrixConstant::ProjectorMatrixConstant (Projector* projector,
												  bool biased)
												  :
ShaderFloat(4),
mProjector(projector),
mBiased(biased)
{
	mAllowUpdater = true;
}
//----------------------------------------------------------------------------
ProjectorMatrixConstant::~ProjectorMatrixConstant ()
{
}
//----------------------------------------------------------------------------
Projector* ProjectorMatrixConstant::GetProjector ()
{
	return mProjector;
}
//----------------------------------------------------------------------------
void ProjectorMatrixConstant::Update(const ShaderStruct *struc)
{
	mProjector = PX2_GR.GetCurEnvirParamController()->GetLight_Dir_Projector();
	if (!mProjector) return;

	const Renderable *renderable = struc->TheRenderable;

	const HMatrix& PVMatrix = mProjector->GetProjectionViewMatrix();
	const HMatrix& WMatrix = renderable->WorldTransform.Matrix();
	HMatrix PVWMatrix = PVMatrix*WMatrix;

	if (mBiased)
	{
		int baseMatIndex = (int)Camera::GetDefaultDepthType();
		PVWMatrix = Projector::BiasScaleMatrix[baseMatIndex] * PVWMatrix;
	}

#ifdef PX2_USE_OPENGLES
	// you must trans on 0penGLES2
	HMatrix matTrans = PVWMatrix.Transpose();
	const float *source = (const float*)matTrans;
#else
	const float* source = (const float*)PVWMatrix;
#endif

	float* target = mData;
	for (int i = 0; i < 16; ++i)
	{
		*target++ = *source++;
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Name support.
//----------------------------------------------------------------------------
Object* ProjectorMatrixConstant::GetObjectByName (const std::string& name)
{
	Object* found = ShaderFloat::GetObjectByName(name);
	if (found)
	{
		return found;
	}

	PX2_GET_OBJECT_BY_NAME(mProjector, name, found);
	return 0;
}
//----------------------------------------------------------------------------
void ProjectorMatrixConstant::GetAllObjectsByName (const std::string& name,
												   std::vector<Object*>& objects)
{
	ShaderFloat::GetAllObjectsByName(name, objects);

	PX2_GET_ALL_OBJECTS_BY_NAME(mProjector, name, objects);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
ProjectorMatrixConstant::ProjectorMatrixConstant (LoadConstructor value)
:
ShaderFloat(value)
{
}
//----------------------------------------------------------------------------
void ProjectorMatrixConstant::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	ShaderFloat::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadPointer(mProjector);
	source.ReadBool(mBiased);

	PX2_END_DEBUG_STREAM_LOAD(ProjectorMatrixConstant, source);
}
//----------------------------------------------------------------------------
void ProjectorMatrixConstant::Link (InStream& source)
{
	ShaderFloat::Link(source);

	source.ResolveLink(mProjector);
}
//----------------------------------------------------------------------------
void ProjectorMatrixConstant::PostLink ()
{
	ShaderFloat::PostLink();
}
//----------------------------------------------------------------------------
bool ProjectorMatrixConstant::Register (OutStream& target) const
{
	if (ShaderFloat::Register(target))
	{
		target.Register(mProjector);
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void ProjectorMatrixConstant::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	ShaderFloat::Save(target);
	PX2_VERSION_SAVE(target);

	target.WritePointer(mProjector);
	target.WriteBool(mBiased);

	PX2_END_DEBUG_STREAM_SAVE(ProjectorMatrixConstant, target);
}
//----------------------------------------------------------------------------
int ProjectorMatrixConstant::GetStreamingSize (Stream &stream) const
{
	int size = ShaderFloat::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_POINTERSIZE(mProjector);
	size += PX2_BOOLSIZE(mBiased);

	return size;
}
//----------------------------------------------------------------------------
