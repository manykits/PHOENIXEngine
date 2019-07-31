// PX2KeyframeController.cpp

#include "PX2KeyframeController.hpp"
#include "PX2Movable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, TransformController, KeyframeController);
PX2_IMPLEMENT_STREAM(KeyframeController);
PX2_IMPLEMENT_FACTORY(KeyframeController);
PX2_IMPLEMENT_DEFAULT_NAMES(TransformController, KeyframeController);

//----------------------------------------------------------------------------
KeyframeController::KeyframeController (int numCommonTimes,
										int numTranslations, int numRotations, int numScales,
										const Transform& localTransform)
										:
TransformController(localTransform),
mTLastIndex(0),
mRLastIndex(0),
mSLastIndex(0),
mCLastIndex(0)
{
	if (numCommonTimes > 0)
	{
		mNumCommonTimes = numCommonTimes;
		mCommonTimes = new1<float>(mNumCommonTimes);

		if (numTranslations > 0)
		{
			mNumTranslations = numTranslations;
			mTranslationTimes = mCommonTimes;
			mTranslations = new1<APoint>(mNumTranslations);
		}
		else
		{
			mNumTranslations = 0;
			mTranslationTimes = 0;
			mTranslations = 0;
		}

		if (numRotations > 0)
		{
			mNumRotations = numRotations;
			mRotationTimes = mCommonTimes;
			mRotations = new1<HQuaternion>(mNumRotations);
		}
		else
		{
			mNumRotations = 0;
			mRotationTimes = 0;
			mRotations = 0;
		}

		if (numScales > 0)
		{
			mNumScales = numScales;
			mScaleTimes = mCommonTimes;
			mScales = new1<Float3>(mNumScales);
		}
		else
		{
			mNumScales = 0;
			mScaleTimes = 0;
			mScales = 0;
		}
	}
	else
	{
		mNumCommonTimes = 0;
		mCommonTimes = 0;

		if (numTranslations > 0)
		{
			mNumTranslations = numTranslations;
			mTranslationTimes = new1<float>(mNumTranslations);
			mTranslations = new1<APoint>(mNumTranslations);
		}
		else
		{
			mNumTranslations = 0;
			mTranslationTimes = 0;
			mTranslations = 0;
		}

		if (numRotations > 0)
		{
			mNumRotations = numRotations;
			mRotationTimes = new1<float>(mNumRotations);
			mRotations = new1<HQuaternion>(mNumRotations);
		}
		else
		{
			mNumRotations = 0;
			mRotationTimes = 0;
			mRotations = 0;
		}

		if (numScales > 0)
		{
			mNumScales = numScales;
			mScaleTimes = new1<float>(mNumScales);
			mScales = new1<Float3>(mNumScales);
		}
		else
		{
			mNumScales = 0;
			mScaleTimes = 0;
			mScales = 0;
		}
	}

	mTransScale = APoint(1.0f, 1.0f, 1.0f);

	SetName("KeyframeController");
}
//----------------------------------------------------------------------------
KeyframeController::~KeyframeController ()
{
	if (mNumCommonTimes > 0)
	{
		delete1(mCommonTimes);
	}
	else
	{
		delete1(mTranslationTimes);
		delete1(mRotationTimes);
		delete1(mScaleTimes);
	}

	delete1(mTranslations);
	delete1(mRotations);
	delete1(mScales);
}
//----------------------------------------------------------------------------
void KeyframeController::GetKeyInfo (float ctrlTime, int numTimes,
									 float* times, int& lastIndex, 
									 float& normTime, int& i0, int& i1)
{
	if (ctrlTime <= times[0])
	{
		normTime = 0.0f;
		lastIndex = 0;
		i0 = 0;
		i1 = 0;
		return;
	}

	if (ctrlTime >= times[numTimes-1])
	{
		normTime = 0.0f;
		lastIndex = numTimes - 1;
		i0 = lastIndex;
		i1 = lastIndex;
		return;
	}

	int nextIndex;
	if (ctrlTime > times[lastIndex])
	{
		nextIndex = lastIndex + 1;
		while (ctrlTime >= times[nextIndex])
		{
			lastIndex = nextIndex;
			++nextIndex;
		}

		i0 = lastIndex;
		i1 = nextIndex;
		normTime = (ctrlTime - times[i0])/(times[i1] - times[i0]);
	}
	else if (ctrlTime < times[lastIndex])
	{
		nextIndex = lastIndex - 1;
		while (ctrlTime <= times[nextIndex])
		{
			lastIndex = nextIndex;
			--nextIndex;
		}

		i0 = nextIndex;
		i1 = lastIndex;
		normTime = (ctrlTime - times[i0])/(times[i1] - times[i0]);
	}
	else
	{
		normTime = 0.0f;
		i0 = lastIndex;
		i1 = lastIndex;
	}
}
//----------------------------------------------------------------------------
APoint KeyframeController::GetTranslate (float normTime, int i0, int i1)
{
	APoint trn = mTranslations[i0] + normTime*(mTranslations[i1] -
		mTranslations[i0]);
	return trn;
}
//----------------------------------------------------------------------------
HMatrix KeyframeController::GetRotate (float normTime, int i0, int i1)
{
	HQuaternion q;
	q.Slerp(normTime, mRotations[i0], mRotations[i1]);

	HMatrix rot;
	q.ToRotationMatrix(rot);
	return rot;
}
//----------------------------------------------------------------------------
Float3 KeyframeController::GetScale(float normTime, int i0, int i1)
{
	float x = mScales[i0][0] + normTime*(mScales[i1][0] - mScales[i0][0]);
	float y = mScales[i0][1] + normTime*(mScales[i1][1] - mScales[i0][1]);
	float z = mScales[i0][2] + normTime*(mScales[i1][2] - mScales[i0][2]);
	return Float3(x, y, z);
}
//----------------------------------------------------------------------------
bool KeyframeController::Update(double applicationTime, double elapsedTime)
{
	if (!Controller::Update(applicationTime, elapsedTime))
	{
		return false;
	}

	float ctrlTime = (float)GetControlTimeByRangeTime(GetPlayedTimeMinusDelay());
	float normTime = 0.0f;
	int i0 = 0, i1 = 0;
	APoint trn;
	HMatrix rot;
	Float3 scale;

	// 如果有公共的时间，可以减少GetKeyInfo的调用，提高运行效率。
	if (mNumCommonTimes > 0)
	{
		GetKeyInfo(ctrlTime, mNumCommonTimes, mCommonTimes, mCLastIndex,
			normTime, i0, i1);

		if (mNumTranslations > 0)
		{
			trn = GetTranslate(normTime, i0, i1);
			trn[0] *= mTransScale[0];
			trn[1] *= mTransScale[1];
			trn[2] *= mTransScale[2];
			mTransform.SetTranslate(trn);
		}

		if (mNumRotations > 0)
		{
			rot = GetRotate(normTime, i0, i1);
			mTransform.SetRotate(rot);
		}

		if (mNumScales > 0)
		{
			scale = GetScale(normTime, i0, i1);
			mTransform.SetScale(scale);
		}
	}
	else
	{
		if (mNumTranslations > 0)
		{
			GetKeyInfo(ctrlTime, mNumTranslations, mTranslationTimes,
				mTLastIndex, normTime, i0, i1);
			trn = GetTranslate(normTime, i0, i1);
			trn[0] *= mTransScale[0];
			trn[1] *= mTransScale[1];
			trn[2] *= mTransScale[2];
			mTransform.SetTranslate(trn);
		}

		if (mNumRotations > 0)
		{
			GetKeyInfo(ctrlTime, mNumRotations, mRotationTimes, mRLastIndex,
				normTime, i0, i1);
			rot = GetRotate(normTime, i0, i1);
			mTransform.SetRotate(rot);
		}

		if (mNumScales > 0)
		{
			GetKeyInfo(ctrlTime, mNumScales, mScaleTimes, mSLastIndex,
				normTime, i0, i1);
			scale = GetScale(normTime, i0, i1);
			mTransform.SetScale(scale);
		}
	}

	Movable* movable = StaticCast<Movable>(mObject);
	movable->LocalTransform = mTransform;
	return true;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化
//----------------------------------------------------------------------------
KeyframeController::KeyframeController (LoadConstructor value)
:
TransformController(value),
mNumCommonTimes(0),
mCommonTimes(0),
mNumTranslations(0),
mTranslationTimes(0),
mTranslations(0),
mNumRotations(0),
mRotationTimes(0),
mRotations(0),
mNumScales(0),
mScaleTimes(0),
mScales(0),
mTLastIndex(0),
mRLastIndex(0),
mSLastIndex(0),
mCLastIndex(0)
{
	mTransScale = APoint(1.0f, 1.0f, 1.0f);
}
//----------------------------------------------------------------------------
void KeyframeController::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	TransformController::Load(source);
	PX2_VERSION_LOAD(source);

	source.Read(mNumCommonTimes);
	if (mNumCommonTimes > 0)
	{
		source.ReadVR(mNumCommonTimes, mCommonTimes);
		source.ReadAggregateRR(mNumTranslations, mTranslations);
		source.ReadAggregateRR(mNumRotations, mRotations);
		source.ReadAggregateRR(mNumScales, mScales);
	}
	else
	{
		source.ReadRR(mNumTranslations, mTranslationTimes);
		source.ReadAggregateVR(mNumTranslations, mTranslations);
		source.ReadRR(mNumRotations, mRotationTimes);
		source.ReadAggregateVR(mNumRotations, mRotations);
		source.ReadRR(mNumScales, mScaleTimes);
		source.ReadAggregateVR(mNumScales, mScales);
	}

	PX2_END_DEBUG_STREAM_LOAD(KeyframeController, source);
}
//----------------------------------------------------------------------------
void KeyframeController::Link (InStream& source)
{
	TransformController::Link(source);
}
//----------------------------------------------------------------------------
void KeyframeController::PostLink ()
{
	TransformController::PostLink();
}
//----------------------------------------------------------------------------
bool KeyframeController::Register (OutStream& target) const
{
	return TransformController::Register(target);
}
//----------------------------------------------------------------------------
void KeyframeController::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	TransformController::Save(target);
	PX2_VERSION_SAVE(target);

	target.Write(mNumCommonTimes);
	if (mNumCommonTimes > 0)
	{
		target.WriteN(mNumCommonTimes, mCommonTimes);
		target.WriteAggregateW(mNumTranslations, mTranslations);
		target.WriteAggregateW(mNumRotations, mRotations);
		target.WriteAggregateW(mNumScales, mScales);
	}
	else
	{
		target.WriteW(mNumTranslations, mTranslationTimes);
		target.WriteAggregateN(mNumTranslations, mTranslations);
		target.WriteW(mNumRotations, mRotationTimes);
		target.WriteAggregateN(mNumRotations, mRotations);
		target.WriteW(mNumScales, mScaleTimes);
		target.WriteAggregateN(mNumScales, mScales);
	}

	PX2_END_DEBUG_STREAM_SAVE(KeyframeController, target);
}
//----------------------------------------------------------------------------
int KeyframeController::GetStreamingSize (Stream &stream) const
{
	int size = TransformController::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += sizeof(mNumCommonTimes);

	if (mNumCommonTimes > 0)
	{
		size += mNumCommonTimes*sizeof(mCommonTimes[0]);
		size += sizeof(mNumTranslations);
		size += mNumTranslations*sizeof(mTranslations[0]);
		size += sizeof(mNumRotations);
		size += mNumRotations*sizeof(mRotations[0]);
		size += sizeof(mNumScales);
		size += mNumScales*sizeof(mScales[0]);
	}
	else
	{
		size += sizeof(mNumTranslations);
		size += mNumTranslations*sizeof(mTranslationTimes[0]);
		size += mNumTranslations*sizeof(mTranslations[0]);
		size += sizeof(mNumRotations);
		size += mNumRotations*sizeof(mRotationTimes[0]);
		size += mNumRotations*sizeof(mRotations[0]);
		size += sizeof(mNumScales);
		size += mNumScales*sizeof(mScaleTimes[0]);
		size += mNumScales*sizeof(mScales[0]);
	}

	return size;
}
//----------------------------------------------------------------------------
