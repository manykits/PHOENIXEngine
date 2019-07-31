// PX2Projector.cpp

#include "PX2Projector.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Camera, Projector);
PX2_IMPLEMENT_STREAM(Projector);
PX2_IMPLEMENT_FACTORY(Projector);
PX2_IMPLEMENT_DEFAULT_NAMES(Camera, Projector);

const HMatrix Projector::BiasScaleMatrix[2] =
{
	HMatrix(
	0.5f, 0.0f, 0.0f, 0.5f,
	0.0f, -0.5f, 0.0f, 0.5f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f),

	HMatrix(
	0.5f, 0.0f, 0.0f, 0.5f,
	0.0f, +0.5f, 0.0f, 0.5f,
	0.0f, 0.0f, 0.5, 0.5f,
	0.0f, 0.0f, 0.0f, 1.0f)
};

//----------------------------------------------------------------------------
Projector::Projector (bool isPerspective) :
Camera(isPerspective)
{
	SetName("Projector");
}
//----------------------------------------------------------------------------
Projector::~Projector ()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化
//----------------------------------------------------------------------------
Projector::Projector (LoadConstructor value)
:
Camera(value)
{
}
//----------------------------------------------------------------------------
void Projector::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Camera::Load(source);
	PX2_VERSION_LOAD(source);

	// 改变了mDepthType，需要重新计算相机投影矩阵。
	OnFrustumChange();

	PX2_END_DEBUG_STREAM_LOAD(Projector, source);
}
//----------------------------------------------------------------------------
void Projector::Link (InStream& source)
{
	Camera::Link(source);
}
//----------------------------------------------------------------------------
void Projector::PostLink ()
{
	Camera::PostLink();
}
//----------------------------------------------------------------------------
bool Projector::Register (OutStream& target) const
{
	return Camera::Register(target);
}
//----------------------------------------------------------------------------
void Projector::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Camera::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(Projector, target);
}
//----------------------------------------------------------------------------
int Projector::GetStreamingSize (Stream &stream) const
{
	int size = Camera::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------