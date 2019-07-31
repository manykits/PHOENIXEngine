// PX2RibbonEmitterController.cpp

#include "PX2RibbonEmitterController.hpp"
#include "PX2RibbonEmitter.hpp"
#include "PX2Renderer.hpp"
#include "PX2GraphicsRoot.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, EffectableController, RibbonEmitterController);
PX2_IMPLEMENT_STREAM(RibbonEmitterController);
PX2_IMPLEMENT_FACTORY(RibbonEmitterController);
PX2_IMPLEMENT_DEFAULT_NAMES(EffectableController, RibbonEmitterController);

//----------------------------------------------------------------------------
RibbonEmitterController::RibbonEmitterController () :
StartRandomIndex(0),
mSmoother(0)
{
	mSmoother = new0 Smoother<AVector>(20, AVector::ZERO);
}
//----------------------------------------------------------------------------
RibbonEmitterController::~RibbonEmitterController ()
{
	delete0(mSmoother);
}
//----------------------------------------------------------------------------
void RibbonEmitterController::Reset ()
{
	EffectableController::Reset();

	mRibbonObjects.clear();
}
//----------------------------------------------------------------------------
bool RibbonEmitterController::Update(double applicationTime, 
	double elapsedTime1)
{
	if (!EffectableController::Update(applicationTime, elapsedTime1))
	{
		return false;
	}

	float elapsedTime = (float)elapsedTime1;
	RibbonEmitter *rb = (RibbonEmitter*)GetControlledable();
	float ribbonLength = rb->GetLength();
	float ribbonSegLength = rb->GetSegLength();

	Camera *camera = PX2_GR.GetCurUpdateCamera();
	if (!camera)
		return true;

	Movable *parent = rb->GetParent();

//	if (IsPlaying())
	{
		AVector up = camera->GetUVector();

		AVector minusDepthVector = -camera->GetDVector();
		AVector minusDepthVectorLast = rb->WorldTransform.GetRotate() * minusDepthVector;

		APoint localPos = rb->LocalTransform.GetTranslate();
		APoint curPos = parent->WorldTransform * localPos;

		if (mRibbonObjects.empty())
		{
			RibbonObject newObject;
			newObject.Pos = curPos;
			newObject.MinusDepthVector = minusDepthVectorLast;
			newObject.Length = 0.0f;
			newObject.Color = rb->GetEmitColor();
			newObject.Alpha = rb->GetEmitAlpha();
			newObject.SizeX = rb->GetEmitSizeX();
			newObject.SizeXInit = newObject.SizeX;

			newObject.Update(rb, 0.0f);

			ModulesUpdateEffectObject(&newObject);

			mRibbonObjects.push_front(newObject); // push two front
			mRibbonObjects.push_front(newObject);

			mLastPos = curPos;

			if (rb->IsAnimStartDoRandom())
				StartRandomIndex = rb->GetAnimStartRandomIndex();
		}
		else
		{
			AVector moveDir = curPos - mLastPos;
			float dirLength = moveDir.Normalize();

			mLastPos = curPos;
			AVector dir = mSmoother->Update(moveDir, elapsedTime1);
			dir.Normalize();

			if (dirLength > 0.0f)
			{
				RibbonObject &first = *mRibbonObjects.begin();
				if (first.Length >= ribbonSegLength)
				{
					first.LengthOnCompleteOneSegment = first.Length;

					RibbonObject newObject;
					newObject.Pos = curPos;
					newObject.MinusDepthVector = minusDepthVectorLast;
					newObject.DVector = dir;
					newObject.Length = dirLength;
					newObject.Color = rb->GetEmitColor();
					newObject.Alpha = rb->GetEmitAlpha();
					newObject.SizeX = rb->GetEmitSizeX();
					newObject.SizeXInit = newObject.SizeX;

					newObject.Update(rb, 0.0f);

					ModulesUpdateEffectObject(&newObject);

					mRibbonObjects.push_front(newObject);
				}
				else
				{
					if (2 == (int)mRibbonObjects.size())
					{
						std::list<RibbonObject>::iterator beg = mRibbonObjects.begin();
						++beg;
						beg->DVector = dir;
					}

					first.Pos = curPos;
					first.MinusDepthVector = minusDepthVectorLast;
					first.DVector = dir;
					first.Length += dirLength;
				}
			}

			float lengthTemp = 0.0f;
			bool eraseMode = false;
			std::list<RibbonObject>::iterator it = mRibbonObjects.begin();
			for (; it!=mRibbonObjects.end();)
			{
				if (!eraseMode)
				{
					lengthTemp += it->Length;

					if (lengthTemp > ribbonLength)
					{
						it->Length = lengthTemp - ribbonLength;

						eraseMode = true;
					}

					++it;
				}
				else
				{
					mRibbonObjects.erase(it++);
				}
			}

			it = mRibbonObjects.begin();
			for (; it!=mRibbonObjects.end(); it++)
			{
				ModulesUpdateEffectObject(&(*it));
			}

			it = mRibbonObjects.begin();
			for (; it!=mRibbonObjects.end(); it++)
			{
				(*it).Update(rb, elapsedTime);
			}
		}
	}

	rb->GenBuffers();

	return true;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
RibbonEmitterController::RibbonEmitterController (LoadConstructor value) :
EffectableController(value),
StartRandomIndex(0)
{
	mSmoother = new0 Smoother<AVector>(20, AVector::ZERO);
}
//----------------------------------------------------------------------------
void RibbonEmitterController::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	EffectableController::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(RibbonEmitterController, source);
}
//----------------------------------------------------------------------------
void RibbonEmitterController::Link (InStream& source)
{
	EffectableController::Link(source);
}
//----------------------------------------------------------------------------
void RibbonEmitterController::PostLink ()
{
	EffectableController::PostLink();
}
//----------------------------------------------------------------------------
bool RibbonEmitterController::Register (OutStream& target) const
{
	if (EffectableController::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void RibbonEmitterController::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	EffectableController::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(RibbonEmitterController, target);
}
//----------------------------------------------------------------------------
int RibbonEmitterController::GetStreamingSize (Stream &stream) const
{
	int size = EffectableController::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------
