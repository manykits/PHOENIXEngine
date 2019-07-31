// PX2SceneBuilderAnimation.cpp

#include "PX2SceneBuilder.hpp"

//----------------------------------------------------------------------------
bool SceneBuilder::AreEqual (const Point3 &point1, const Point3 &point2)
{
	return fabsf(point1.x - point2.x) <= MIN_DIFFERENCE
		&& fabsf(point1.y - point2.y) <= MIN_DIFFERENCE
		&& fabsf(point1.z - point2.z) <= MIN_DIFFERENCE;
}
//----------------------------------------------------------------------------
bool SceneBuilder::AreEqual (const Quat &quat1, const Quat &quat2)
{
	return fabsf(quat1.x - quat2.x) <= MIN_DIFFERENCE
		&& fabsf(quat1.y - quat2.y) <= MIN_DIFFERENCE
		&& fabsf(quat1.z - quat2.z) <= MIN_DIFFERENCE
		&& fabsf(quat1.w - quat2.w) <= MIN_DIFFERENCE;
}
//----------------------------------------------------------------------------
bool SceneBuilder::CompareKeyTimes (KeyInfo* pkFirst, KeyInfo* pkSecond)
{
	return pkFirst->Time < pkSecond->Time;
}
//----------------------------------------------------------------------------
void SceneBuilder::BuildKeyFrameController(INode *node, PX2::Movable *movable)
{
	assertion(node!=0, "node must exist.");
	assertion(movable!=0, "movable must exist.");

	// 检测当前节点是否是动画
	Control *tmCtl = node->GetTMController();
	assertion(tmCtl!=0, "must contains time.");

	if (!tmCtl->IsKeyable())
	{
		// 节点不是keyframe，但是他仍然是动画的。举例来说，它可以是一个IK节
		// 点。在那样的情况下，导出节点的所有frames。
		BuildFrameController(node, movable);
		return;
	}

	// 用来优化，检测节点的平移，旋转和缩放是否真的随着时间的变化而变化。
	AnimationTiming trnTiming, rotTiming, scaTiming;
	if (!GetAnimationTiming(node, trnTiming, rotTiming, scaTiming)) 
	{
		return;
	}

	// 关键帧信息集
	std::vector<KeyInfo*> keyInfos;

	// 收集平移变换键信息
	int numTrnKeys = 0;
	if (trnTiming.Active)
	{
		Control* ctl = tmCtl->GetPositionController();
		if (ctl)
		{
			IKeyControl* keyCtl = GetKeyControlInterface(ctl);
			if (keyCtl)
			{
				numTrnKeys = keyCtl->GetNumKeys();
				GetTrnKeyInfo(numTrnKeys, ctl->ClassID(), keyCtl, trnTiming,
					keyInfos);
			}
		}
	}

	// 收集旋转变换信息
	int numRotKeys = 0;
	if (rotTiming.Active)
	{
		Control *ctl = tmCtl->GetRotationController();
		if (ctl)
		{
			IKeyControl *keyCtl = GetKeyControlInterface(ctl);
			if (keyCtl)
			{
				numRotKeys = keyCtl->GetNumKeys();
				GetRotKeyInfo(numRotKeys, ctl->ClassID(), keyCtl, rotTiming,
					keyInfos);
			}
		}
	}

	// 收集缩放变换信息
	int numScaKeys = 0;
	if (scaTiming.Active)
	{
		Control* ctl = tmCtl->GetScaleController();
		if (ctl)
		{
			IKeyControl* keyCtl = GetKeyControlInterface(ctl);
			if (keyCtl)
			{
				numScaKeys = keyCtl->GetNumKeys();
				GetScaleKeyInfo(numScaKeys, ctl->ClassID(), keyCtl, scaTiming,
					keyInfos);
			}
		}
	}

	// 如果物体使用程序控制器而不是使用一般的控制器，以上所有
	// GetKeyControlInterface(...)函数的调用会返回null，因为在程序控制器里没有
	// "keys"。
	if ( 0==numTrnKeys && 0==numRotKeys && 0==numScaKeys)
	{
		BuildFrameController(node, movable);
		return;
	}

	// 对KeyInfo物体按照时间排序
	std::sort(keyInfos.begin(), keyInfos.end(), CompareKeyTimes);

	// 初始化关键中控制器数组
	PX2::KeyframeController *kfc = new0 PX2::KeyframeController
		(0, numTrnKeys, numRotKeys, numScaKeys, movable->LocalTransform);
	kfc->Repeat = PX2::Controller::RT_NONE;
	PX2::APoint *tKey = kfc->GetTranslations();
	float *tTime = kfc->GetTranslationTimes();

	PX2::HQuaternion *rKey = kfc->GetRotations();
	float *rTime = kfc->GetRotationTimes();

	PX2::Float3 *sKey = kfc->GetScales();
	float *sTime = kfc->GetScaleTimes();

	TimeValue timeNow = -1;
	PX2::Transform trans;
	for (int i=0; i<(int)keyInfos.size(); i++)
	{
		KeyInfo *info = keyInfos[i];

		if (timeNow != info->Time)
		{
			timeNow = info->Time;
			trans = GetLocalTransform(node, timeNow);
		}

		switch (info->Type)
		{
		case KT_TRANSLATION:
			*tKey++ = trans.GetTranslate();
			*tTime++ = TicksToSec(timeNow - mTimeOffset);
			break;
		case KT_ROTATION:
			if (trans.IsRSMatrix())
			{
				rKey++->FromRotationMatrix(trans.GetRotate());
				*rTime++ = TicksToSec(timeNow - mTimeOffset);
			}
			break;
		case KT_SCALE:
			// 当不是同一缩放或者为负缩放是会发出警告
			PX2::Float3 scale3;
			if (trans.IsUniformScale())
			{
				float scale = trans.GetUniformScale();
				scale3 = PX2::Float3(scale, scale, scale);
			}
			else
			{
				scale3 = PX2::Float3(
					PX2::Mathf::FAbs(trans.GetScale().X()),
					PX2::Mathf::FAbs(trans.GetScale().Y()),
					PX2::Mathf::FAbs(trans.GetScale().Z()));
			}

			*sKey++ = scale3;
			*sTime++ = TicksToSec(timeNow - mTimeOffset);
			break;
		}

		delete0(info);
	}

	kfc->MinTime = TicksToSec(mTimeStart - mTimeOffset);
	kfc->MaxTime = TicksToSec(mTimeEnd - mTimeOffset);
	
	movable->AttachController(kfc);
}
//----------------------------------------------------------------------------
void SceneBuilder::BuildFrameController (INode *node, PX2::Movable *movable)
{
	// 关联一个"frame controller"到一个动画节点。这个函数导出所有帧，而不
	// 是关键帧。在场景中，Phoenix2会在各个独立的帧之间进行插值。然后在某些
	// 情况下动画使用IK动画，导出插件当前不支持。
	// node:
	//		在Max场景节点里的动画节点。
	// movable:
	//		在Phoenix2场景里的动画节点。

	std::vector<int> ttTime, rrTime, ssTime;
	std::vector<Point3> ttData, ssData;
	std::vector<Quat> rrData;

	int time = mTimeStart;
	Matrix3 local = node->GetNodeTM(time) * 
		Inverse(node->GetParentTM(time));
	AffineParts affParts;
	decomp_affine(local, &affParts);
	ttTime.push_back(time);
	rrTime.push_back(time);
	ssTime.push_back(time);
	ttData.push_back(affParts.t);
	rrData.push_back(affParts.q);
	ssData.push_back(affParts.k);

	// 对每一帧生成一个样本
	for (time+=mTicksPerFrame; time<=mTimeEnd; 
		time+=mTicksPerFrame)
	{
		local = node->GetNodeTM(time) * 
			Inverse(node->GetParentTM(time));
		decomp_affine(local, &affParts);

		if (!AreEqual(affParts.t, ttData.back()))
		{
			ttTime.push_back(time);
			ttData.push_back(affParts.t);
		}

		if (!AreEqual(affParts.q, rrData.back()))
		{
			// q和-q代表相同的旋转
			if (!AreEqual(-affParts.q, rrData.back()))
			{
				rrTime.push_back(time);

				// 为旋转的四元数存储连贯的角度?
				float dot = affParts.q % rrData.back();
				if (dot > 0.0f)
				{
					rrData.push_back(affParts.q);
				}
				else
				{
					rrData.push_back(-affParts.q);
				}
			}
		}

		if (!AreEqual(affParts.k, ssData.back()))
		{
			ssTime.push_back(time);
			ssData.push_back(affParts.f * affParts.k);
		}

		// 确定缩放是否是同一缩放
		bool scaleIsUniform =
			fabsf(affParts.k.x - affParts.k.y) < MIN_DIFFERENCE &&
			fabsf(affParts.k.y - affParts.k.z) < MIN_DIFFERENCE;

		// 确定是否是单位缩放
		bool scaleIsUnity = scaleIsUniform &&
			fabsf(affParts.k.x - 1.0f) < MIN_DIFFERENCE;

		// 确定缩放轴向是否是单位
		if (affParts.u.w < 0.0f)
		{
			affParts.u = -affParts.u;
		}

		bool orientIsIdentity = scaleIsUniform || (
			fabsf(affParts.u.w - 1.0f) < MIN_DIFFERENCE &&
			fabsf(affParts.u.x) < MIN_DIFFERENCE &&
			fabsf(affParts.u.y) < MIN_DIFFERENCE &&
			fabsf(affParts.u.z) < MIN_DIFFERENCE);

		PX2_UNUSED(scaleIsUnity);
		PX2_UNUSED(orientIsIdentity);
	}

	if (ttData.size()==1 && rrData.size()==1 && ssData.size()==1)
	{
		return;
	}

	int numTKeys = (int)ttData.size();
	int numRKeys = (int)rrData.size();
	int numSKeys = (int)ssData.size();

	PX2::KeyframeController *kfc = new0 PX2::KeyframeController(0, numTKeys,
		numRKeys, numSKeys, movable->LocalTransform);
	kfc->MinTime = TicksToSec(mTimeStart - mTimeOffset);
	kfc->MaxTime = TicksToSec(mTimeEnd - mTimeOffset);
	kfc->Repeat = PX2::Controller::RT_NONE;
	PX2::APoint *tKey = kfc->GetTranslations();
	float *tTime = kfc->GetTranslationTimes();
	PX2::HQuaternion *rKey = kfc->GetRotations();
	float *rTime = kfc->GetRotationTimes();
	PX2::Float3 *sKey = kfc->GetScales();
	float *sTime = kfc->GetScaleTimes();

	// t
	for (int i=0; i<numTKeys; i++)
	{
		tTime[i] = TicksToSec(ttTime[i] - mTimeOffset);
		tKey[i].X() = ttData[i][0];
		tKey[i].Y() = ttData[i][1];
		tKey[i].Z() = ttData[i][2];
	}

	// r
	for (int i=0; i<numRKeys; i++)
	{
		rTime[i] = TicksToSec(rrTime[i] - mTimeOffset);
		rKey[i].W() = rrData[i].w;
		rKey[i].X() = -rrData[i].x;
		rKey[i].Y() = -rrData[i].y;
		rKey[i].Z() = -rrData[i].z;
	}

	// s
	for (int i=0; i<numSKeys; i++)
	{
		sTime[i] = TicksToSec(ssTime[i] - mTimeOffset);
		sKey[i] = PX2::Float3(ssData[i].x, ssData[i].y, ssData[i].z);
	}

	movable->AttachController(kfc);
}
//----------------------------------------------------------------------------
bool SceneBuilder::GetAnimationTiming(INode *node,
									  SceneBuilder::AnimationTiming &tTiming, 
									  SceneBuilder::AnimationTiming &rTiming, 
									  SceneBuilder::AnimationTiming &sTiming)
{
	// 检测节点的方位是否真的随着时间的变化而变化。

	int ticksPerFrame = GetTicksPerFrame();
	Matrix3 nodeTM;
	AffineParts affParts;
	Point3 prevTrans, rotAxis, prevRotAxis, prevScaleFactor;
	float rotAngle, prevRotAngle;
	tTiming.Active = FALSE;
	rTiming.Active = FALSE;
	sTiming.Active = FALSE;

	// 获得节点最初变换，将变换进行分解
	nodeTM = node->GetNodeTM(mTimeStart) *
		Inverse(node->GetParentTM(mTimeStart));
	decomp_affine(nodeTM, &affParts);
	AngAxisFromQ(affParts.q, &prevRotAngle, prevRotAxis);
	prevTrans = affParts.t;
	prevScaleFactor = affParts.k;

	// 遍历所有帧，检测变换
	TimeValue timeNow = mTimeStart + ticksPerFrame;
	for (; timeNow<=mTimeEnd; timeNow+=ticksPerFrame)
	{
		nodeTM = node->GetNodeTM(timeNow)*
			Inverse(node->GetParentTM(timeNow));
		decomp_affine(nodeTM, &affParts);
		AngAxisFromQ(affParts.q, &rotAngle, rotAxis);

		// t
		if (!tTiming.Active)
		{
			if (!AreEqual(affParts.t, prevTrans))
			{
				tTiming.Active = TRUE;
				tTiming.Start = timeNow - ticksPerFrame;
				tTiming.End = timeNow;
			}
		}
		else
		{
			if (!AreEqual(affParts.t, prevTrans))
			{
				tTiming.End = timeNow;
			}
		}

		// r
		if (!rTiming.Active)
		{
			if (fabsf(rotAngle - prevRotAngle) > MIN_DIFFERENCE
				|| !AreEqual(rotAxis, prevRotAxis))
			{
				rTiming.Active = TRUE;
				rTiming.Start = timeNow - ticksPerFrame;
				rTiming.End = timeNow;
			}
		}
		else
		{
			if (fabsf(rotAngle - prevRotAngle) > MIN_DIFFERENCE
				|| !AreEqual(rotAxis, prevRotAxis))
			{
				rTiming.End = timeNow;
			}
		}

		// s
		if (!sTiming.Active)
		{
			if (!AreEqual(affParts.k, prevScaleFactor))
			{
				sTiming.Active = TRUE;
				sTiming.Start = timeNow - ticksPerFrame;
				sTiming.End = timeNow;
			}
		}
		else
		{
			if (!AreEqual(affParts.k, prevScaleFactor))
			{
				sTiming.End = timeNow;
			}
		}

		prevRotAngle = rotAngle;
		prevRotAxis = rotAxis;
		prevTrans = affParts.t;
		prevScaleFactor = affParts.k;
	}

	return tTiming.Active || rTiming.Active || sTiming.Active;
}
//----------------------------------------------------------------------------
void SceneBuilder::GetTrnKeyInfo(int &numKeys, Class_ID classID,
								 IKeyControl *keyCtl, AnimationTiming &tTiming, 
								 std::vector<KeyInfo*> &keyInfos)
{
	// 提取Max节点位置变换控制器的位移变换关键帧
	//
	// numKeys:
	//		作为输入参数，表示要处理的关键帧数量。作为输出参数，表示与之相
	// 匹配的Trudant中的关键帧的数量。
	// classID:
	//		Max“位移动画变换”关键帧控制器的Class_ID。
	// keyOn:
	//		Max节点的“位移动画变换”关键帧控制器接口。
	// tTiming:
	//		保存着节点“位移动画变换”时间细节。
	// keyInfos:
	//		关于KeyInfo的向量，里面存储了关键帧的类型和时间信息。

	int i, tempNumKeys = numKeys;
	numKeys = 0;

	if (classID == Class_ID(TCBINTERP_POSITION_CLASS_ID, 0))
	{
		// 处理Tension/Continuity/Bias控制器
		ITCBPoint3Key key;
		for (i=0; i<tempNumKeys; i++)
		{
			keyCtl->GetKey(i, &key);
			if (key.time < tTiming.Start)
			{
				continue;
			}
			else if (key.time > tTiming.End)
			{
				break;
			}

			keyInfos.push_back(new0 KeyInfo(key.time, KT_ROTATION));
			numKeys++;
		}
	}
	else if (classID == Class_ID(HYBRIDINTERP_POSITION_CLASS_ID, 0))
	{
		// 处理Bezier控制器
		IBezPoint3Key key;
		for (i=0; i<tempNumKeys; i++)
		{
			keyCtl->GetKey(i, &key);
			if (key.time < tTiming.Start)
				continue;
			else if (key.time > tTiming.End)
				break;

			keyInfos.push_back(new0 KeyInfo(key.time, KT_TRANSLATION));
			numKeys++;
		}
	}
	else if (classID == Class_ID(LININTERP_POSITION_CLASS_ID, 0))
	{
		// 处理 Linear 控制器

		ILinPoint3Key key;
		for (int i=0; i<tempNumKeys; i++)
		{
			keyCtl->GetKey(i, &key);
			if (key.time < tTiming.Start)
				continue;
			else if (key.time > tTiming.End)
				break;

			keyInfos.push_back(new0 KeyInfo(key.time, KT_TRANSLATION));
			numKeys++;
		}
	}
}
//----------------------------------------------------------------------------
void SceneBuilder::GetRotKeyInfo (int &numKeys, Class_ID classID,
								  IKeyControl *keyCtl,
								  AnimationTiming &rTiming,
								  std::vector<KeyInfo*> &keyInfos)
{
	// 提取Max节点位置旋转控制器的位移变换关键帧
	//
	// numKeys:
	//		作为输入参数，表示要处理的关键帧数量。作为输出参数，表示与之相
	// 匹配的Trudant中关键帧的数量。
	// classID:
	//		Max“旋转动画变换”关键帧控制器的class ID。
	// keyCtl:
	//		Max节点的“旋转动画变换”关键帧控制器接口。
	// rTiming:
	//		保存着节点“旋转动画变换”时间细节。
	// keyInfos:
	//		关于KeyInfo的向量，里面存储了关键帧的类型和时间信息。

	int i, tempNumKeys = numKeys;
	numKeys = 0;

	if (classID == Class_ID(TCBINTERP_ROTATION_CLASS_ID, 0))
	{
		// 处理 Tension/Continuity/Bias 控制器

		ITCBRotKey key;
		for (i=0; i<tempNumKeys; i++)
		{
			keyCtl->GetKey(i, &key);
			if (key.time < rTiming.Start)
				continue;
			else if (key.time > rTiming.End)
				break;

			keyInfos.push_back(new0 KeyInfo(key.time, KT_ROTATION));
			numKeys++;
		}
	}
	else if (classID == Class_ID(HYBRIDINTERP_ROTATION_CLASS_ID, 0))
	{
		// 处理 Bezier 控制器

		IBezQuatKey key;
		for (i=0; i<tempNumKeys; i++)
		{
			keyCtl->GetKey(i, &key);
			if (key.time < rTiming.Start)
				continue;
			else if (key.time > rTiming.End)
				break;

			keyInfos.push_back(new0 KeyInfo(key.time, KT_ROTATION));
			numKeys++;
		}
	}
	else if (classID == Class_ID(LININTERP_ROTATION_CLASS_ID, 0))
	{
		// 处理 Linear 控制器

		ILinRotKey key;
		for (i=0; i<numKeys; i++)
		{
			keyCtl->GetKey(i, &key);
			if (key.time <rTiming.Start)
				continue;
			else if (key.time > rTiming.End)
				break;

			keyInfos.push_back(new0 KeyInfo(key.time, KT_ROTATION));
			numKeys++;
		}
	}
}
//----------------------------------------------------------------------------
void SceneBuilder::GetScaleKeyInfo(int &numKeys, Class_ID classID, 
								   IKeyControl *keyCtl, 
								   AnimationTiming &sTiming,
								   std::vector<KeyInfo*> &keyInfos)
{
	// 提取Max节点缩放变换控制器的缩放变换关键帧
	//
	// numKeys:
	//		作为输入参数，表示要处理的关键帧数量。作为输出参数，表示与之相
	// 匹配的Trudant中关键帧的数量。
	// classID:
	//		Max“缩放动画变换”关键帧控制器的class ID。
	// keyCtl:
	//		Max节点的“缩放动画变换”关键帧控制器接口。
	// tTiming:
	//		保存着节点“缩放动画变换”时间细节。
	// keyInfos:
	//		关于KeyInfo的向量，里面存储了关键帧的类型和时间信息。

	int i, tempNumKeys = numKeys;
	numKeys = 0;

	if (classID == Class_ID(TCBINTERP_SCALE_CLASS_ID, 0))
	{
		// 处理 Tension/Continuity/Bias 控制器
		ITCBRotKey key;
		for (i=0; i<tempNumKeys; i++)
		{
			keyCtl->GetKey(i, &key);
			if (key.time < sTiming.Start)
				continue;
			else if (key.time > sTiming.End)
				break;

			keyInfos.push_back(new0 KeyInfo(key.time, KT_SCALE));
			numKeys++;
		}
	}
	else if (classID == Class_ID(HYBRIDINTERP_SCALE_CLASS_ID, 0))
	{
		// 处理 Bezier 控制器
		IBezScaleKey key;
		for (i=0; i<tempNumKeys; i++)
		{
			keyCtl->GetKey(i, &key);
			if (key.time < sTiming.Start)
				continue;
			else if (key.time > sTiming.End)
				break;

			keyInfos.push_back(new0 KeyInfo(key.time, KT_SCALE));
			numKeys++;
		}
	}
	else if (classID == Class_ID(LININTERP_SCALE_CLASS_ID, 0))
	{
		// 处理 Linear 控制器
		ILinScaleKey key;
		for (i=0; i<tempNumKeys; i++)
		{
			keyCtl->GetKey(i, &key);
			if (key.time < sTiming.Start)
				continue;
			else if (key.time > sTiming.End)
				break;

			keyInfos.push_back(new0 KeyInfo(key.time, KT_SCALE));
			numKeys++;
		}
	}
}
//----------------------------------------------------------------------------