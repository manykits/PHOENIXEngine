// PX2DlodNode.cpp

#include "PX2DlodNode.hpp"
#include "PX2Camera.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, SwitchNode, DlodNode);
PX2_IMPLEMENT_STREAM(DlodNode);
PX2_IMPLEMENT_FACTORY(DlodNode);
PX2_IMPLEMENT_DEFAULT_NAMES(SwitchNode, DlodNode);

//----------------------------------------------------------------------------
DlodNode::DlodNode (int numLevelsOfDetail)
    :
    mNumLevelsOfDetail(numLevelsOfDetail)
{
    mModelMinDist = new1<float>(mNumLevelsOfDetail);
    mModelMaxDist = new1<float>(mNumLevelsOfDetail);
    mWorldMinDist = new1<float>(mNumLevelsOfDetail);
    mWorldMaxDist = new1<float>(mNumLevelsOfDetail);
}
//----------------------------------------------------------------------------
DlodNode::~DlodNode ()
{
    delete1(mModelMinDist);
    delete1(mModelMaxDist);
    delete1(mWorldMinDist);
    delete1(mWorldMaxDist);
}
//----------------------------------------------------------------------------
void DlodNode::SetModelDistance (int i, float minDist, float maxDist)
{
    if (0 <= i && i < mNumLevelsOfDetail)
    {
        mModelMinDist[i] = minDist;
        mModelMaxDist[i] = maxDist;
        mWorldMinDist[i] = minDist;
        mWorldMaxDist[i] = maxDist;
        return;
    }

    assertion(false, "Invalid index in SetModelDistance.\n");
}
//----------------------------------------------------------------------------
void DlodNode::SelectLevelOfDetail (const Camera* camera)
{
    mWorldLodCenter = WorldTransform*mModelLodCenter;

    int i;
    for (i = 0; i < mNumLevelsOfDetail; ++i)
    {
        mWorldMinDist[i] = WorldTransform.GetUniformScale()*mModelMinDist[i];
        mWorldMaxDist[i] = WorldTransform.GetUniformScale()*mModelMaxDist[i];
    }

    SetActiveChild(SN_INVALID_CHILD);
    AVector diff = mWorldLodCenter - camera->GetPosition();
    float dist = diff.Length();
    for (i = 0; i < mNumLevelsOfDetail; ++i) 
    {
        if (mWorldMinDist[i] <= dist && dist < mWorldMaxDist[i])
        {
            SetActiveChild(i);
            break;
        }
    }
}
//----------------------------------------------------------------------------
void DlodNode::OnGetVisibleSet (Culler& culler, bool noCull)
{
    SelectLevelOfDetail(culler.GetCamera());
    SwitchNode::OnGetVisibleSet(culler, noCull);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化
//----------------------------------------------------------------------------
DlodNode::DlodNode (LoadConstructor value)
    :
    SwitchNode(value),
    mNumLevelsOfDetail(0),
    mModelMinDist(0),
    mModelMaxDist(0),
    mWorldMinDist(0),
    mWorldMaxDist(0)
{
}
//----------------------------------------------------------------------------
void DlodNode::Load (InStream& source)
{
    PX2_BEGIN_DEBUG_STREAM_LOAD(source);

    SwitchNode::Load(source);
	PX2_VERSION_LOAD(source);

    source.ReadAggregate(mModelLodCenter);
    source.ReadRR(mNumLevelsOfDetail, mModelMinDist);
    source.ReadVR(mNumLevelsOfDetail, mModelMaxDist);
    source.ReadVR(mNumLevelsOfDetail, mWorldMinDist);
    source.ReadVR(mNumLevelsOfDetail, mWorldMaxDist);

    PX2_END_DEBUG_STREAM_LOAD(DlodNode, source);
}
//----------------------------------------------------------------------------
void DlodNode::Link (InStream& source)
{
    SwitchNode::Link(source);
}
//----------------------------------------------------------------------------
void DlodNode::PostLink ()
{
    SwitchNode::PostLink();
}
//----------------------------------------------------------------------------
bool DlodNode::Register (OutStream& target) const
{
    return SwitchNode::Register(target);
}
//----------------------------------------------------------------------------
void DlodNode::Save (OutStream& target) const
{
    PX2_BEGIN_DEBUG_STREAM_SAVE(target);

    SwitchNode::Save(target);
	PX2_VERSION_SAVE(target);

    target.WriteAggregate(mModelLodCenter);
    target.WriteW(mNumLevelsOfDetail, mModelMinDist);
    target.WriteN(mNumLevelsOfDetail, mModelMaxDist);
    target.WriteN(mNumLevelsOfDetail, mWorldMinDist);
    target.WriteN(mNumLevelsOfDetail, mWorldMaxDist);

	// 模型世界中心为实时计算，不需要存储。

    PX2_END_DEBUG_STREAM_SAVE(DlodNode, target);
}
//----------------------------------------------------------------------------
int DlodNode::GetStreamingSize (Stream &stream) const
{
    int size = SwitchNode::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
    size += sizeof(mModelLodCenter);
    size += sizeof(mNumLevelsOfDetail);
    size += mNumLevelsOfDetail*sizeof(float);
    size += mNumLevelsOfDetail*sizeof(float);
    size += mNumLevelsOfDetail*sizeof(float);
    size +=mNumLevelsOfDetail*sizeof(float);
    return size;
}
//----------------------------------------------------------------------------
