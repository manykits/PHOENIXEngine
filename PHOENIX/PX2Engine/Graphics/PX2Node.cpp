// PX2Node.cpp

#include "PX2Node.hpp"
#include "PX2FunObject.hpp"
#include "PX2GraphicsEventType.hpp"
#include "PX2EventWorld.hpp"
#include "PX2GraphicsEventData.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Movable, Node);
PX2_IMPLEMENT_STREAM(Node);
PX2_IMPLEMENT_FACTORY(Node);

//----------------------------------------------------------------------------
Node::Node () :
mIsDoPickPriority(false),
mIsNeedCalUpdateChild(true),
mAnchorID(0)
{
	SetName("Node");
}
//----------------------------------------------------------------------------
Node::~Node ()
{
	for (int i = 0; i < (int)mChild.size(); i++)
	{
		if (mChild[i])
		{
			mChild[i]->SetParent(0);
			mChild[i] = 0;
		}
	}
}
//----------------------------------------------------------------------------
void Node::Play ()
{
	Movable::Play();

	for (int i=0; i<GetNumChildren(); i++)
	{
		if (mChild[i] && !mChild[i]->IsSelfCtrled())
		{
			mChild[i]->Play();
		}
	}
}
//----------------------------------------------------------------------------
bool Node::IsPlaying () const
{
	if (Movable::IsPlaying())
		return true;

	for (int i=0; i<GetNumChildren(); i++)
	{
		if (mChild[i] && !mChild[i]->IsSelfCtrled())
		{
			if (mChild[i]->IsPlaying())
				return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
void Node::Pause()
{
	Movable::Pause();

	for (int i=0; i<GetNumChildren(); i++)
	{
		if (mChild[i] && !mChild[i]->IsSelfCtrled())
		{
			mChild[i]->Pause();
		}
	}
}
//----------------------------------------------------------------------------
void Node::Reset ()
{
	Movable::Reset();

	for (int i=0; i<GetNumChildren(); i++)
	{
		if (mChild[i] && !mChild[i]->IsSelfCtrled())
		{
			mChild[i]->Reset();
		}
	}
}
//----------------------------------------------------------------------------
int Node::AttachChild (Movable* child)
{
    if (!child)
    {
        assertion(false, "You cannot attach null children to a node.\n");
        return -1;
    }

    if (child->GetParent())
    {
        assertion(false, "The child already has a parent.\n");
        return -1;
    }

    child->SetParent(this);

	// 将孩子插入到第一个可用位置
    std::vector<MovablePtr>::iterator iter = mChild.begin();
    std::vector<MovablePtr>::iterator end = mChild.end();
    for (int i = 0; iter != end; ++iter, ++i)
    {
        if (*iter == 0)
        {
            *iter = child;
			OnChildAttached(child);
			child->OnBeAttached();

            return i;
        }
    }

	// 所有空位都用完了，将孩子插入队列末尾
    const int numChildren = (int)mChild.size();
    mChild.push_back(child);
	OnChildAttached(child);
	child->OnBeAttached();

    return numChildren;
}
//----------------------------------------------------------------------------
void Node::InsertChild (Movable *before, Movable *child)
{
	if (!child)
	{
		assertion(false, "You cannot attach null children to a node.\n");
		return;
	}

	if (child->GetParent())
	{
		assertion(false, "The child already has a parent.\n");
		return;
	}

	child->SetParent(this);

	// 将孩子插入到第一个可用位置
	std::vector<MovablePtr>::iterator iter = mChild.begin();
	std::vector<MovablePtr>::iterator end = mChild.end();
	for (; iter!=end; iter++)
	{
		if (*iter == before)
		{
			std::vector<MovablePtr>::iterator itP1 = iter+1;
			if (itP1 != end)
			{
				mChild.insert(itP1, child);
				OnChildAttached(child);
				child->OnBeAttached();
			}
			else
			{
				mChild.push_back(child);
				OnChildAttached(child);
				child->OnBeAttached();
			}
			
			return;
		}
	}
}
//----------------------------------------------------------------------------
int Node::DetachChild (Movable* child)
{
    if (child)
    {
        std::vector<MovablePtr>::iterator iter = mChild.begin();
        std::vector<MovablePtr>::iterator end = mChild.end();
        for (int i = 0; iter != end; ++iter, ++i)
        {
            if (*iter == child)
            {
				child->OnBeDetach();
				OnChildDetach(child);
                (*iter)->SetParent(0);
                *iter = 0;

                return i;
            }
        }
    }
    return -1;
}
//----------------------------------------------------------------------------
bool Node::DetachChildByName (const std::string &name)
{
	for (int i=0; i<(int)mChild.size(); i++)
	{
		MovablePtr child = mChild[i];

		if (child != 0 && child->GetName() == name)
		{
			child->OnBeDetach();
			child->SetParent(0);
			OnChildDetach(child);
			mChild[i] = 0;

			return true;
		}

	}

	return false;
}
//----------------------------------------------------------------------------
MovablePtr Node::DetachChildAt (int i)
{
    if (0 <= i && i < (int)mChild.size())
    {
        MovablePtr child = mChild[i];
        if (child)
        {
			child->OnBeDetach();
            child->SetParent(0);
			OnChildDetach(child);
            mChild[i] = 0;
        }
        return child;
    }
    return 0;
}
//----------------------------------------------------------------------------
void Node::DetachAllChildren ()
{
	for (int i=0; i<(int)mChild.size(); i++)
	{
		 MovablePtr child = mChild[i];

		 if (child)
		 {
			 child->OnBeDetach();
			 child->SetParent(0);
			 OnChildDetach(child);
			 mChild[i] = 0;
		 }
	}
}
//----------------------------------------------------------------------------
int Node::GetNumValidChildren () const
{
	int num = 0;

	for (int i=0; i<(int)mChild.size(); i++)
	{
		if (mChild[i])
			num++;
	}

	return num;
}
//----------------------------------------------------------------------------
MovablePtr Node::SetChild (int i, Movable* child)
{
    if (child)
    {
        assertion(!child->GetParent(), "The child already has a parent.\n");
    }

    const int numChildren = (int)mChild.size();
    if (0 <= i && i < numChildren)
    {
		// 将旧的孩子移除
        MovablePtr previousChild = mChild[i];
        if (previousChild)
        {
			previousChild->OnBeDetach();
            previousChild->SetParent(0);
			OnChildDetach(previousChild);
        }

		// 插入新的孩子到空位
        if (child)
        {
            child->SetParent(this);
			OnChildAttached(child);
			child->OnBeAttached();
        }

        mChild[i] = child;
        return previousChild;
    }

	// 孩子超出范围，将其插入到孩子队列末尾
    if (child)
    {
        child->SetParent(this);
		OnChildAttached(child);
		child->OnBeAttached();
    }
    mChild.push_back(child);

    return 0;
}
//----------------------------------------------------------------------------
Movable *Node::GetChild (int i)
{
    if (0 <= i && i < (int)mChild.size())
    {
        return mChild[i];
    }
    return 0;
}
//----------------------------------------------------------------------------
std::vector<MovablePtr> &Node::GetChildren()
{
	return mChild;
}
//----------------------------------------------------------------------------
void Node::OnChildAttached(Movable *child)
{
	mIsNeedCalUpdateChild = true;

	AddObjectData data;
	data.ParentObj = this;
	data.Obj = child;

	if (EventWorld::GetSingletonPtr())
	{
		Event *ent = GraphicsES::CreateEventX(GraphicsES::AddObject, 
			"GraphicsES::AddObject");
		ent->SetDataPointer0(child);
		ent->SetDataPointer1(this);
		ent->SetData<AddObjectData>(data);
		PX2_EW.BroadcastingLocalEvent(ent);
	}
}
//----------------------------------------------------------------------------
void Node::OnChildDetach(Movable *child)
{
	mIsNeedCalUpdateChild = true;

	if (EventWorld::GetSingletonPtr())
	{
		Event *ent = GraphicsES::CreateEventX(GraphicsES::RemoveObject, 
			"GraphicsES::RemoveObject");
		ent->SetData<Object*>((Object*)child);
		ent->SetDataPointer0((Object*)child);
		PX2_EW.BroadcastingLocalEvent(ent);
	}
}
//----------------------------------------------------------------------------
Movable *Node::GetChildByName (const std::string &name)
{
	for (int i=0; i<(int)mChild.size(); i++)
	{
		MovablePtr child = mChild[i];

		if (child != 0 && child->GetName() == name)
		{
			return child;
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
bool Node::IsHasChild(const Movable *child) const
{
	if (child && ((Movable*)child)->GetParent() == this)
		return true;

	return false;
}
//----------------------------------------------------------------------------
void Node::ShowOnlyChild(const std::string &childName)
{
	for (int i = 0; i < GetNumChildren(); i++)
	{
		Movable *mov = GetChild(i);
		if (mov)
		{
			mov->Show(false);
		}
	}

	for (int i = 0; i < GetNumChildren(); i++)
	{
		Movable *mov = GetChild(i);
		if (mov && childName == mov->GetName())
		{
			mov->Show(true);
		}
	}
}
//----------------------------------------------------------------------------
void Node::Enable(bool enable)
{
	Movable::Enable(enable);

	for (int i = 0; i < GetNumChildren(); i++)
	{
		if (mChild[i] && !mChild[i]->IsEnableSelfCtrled())
		{
			mChild[i]->Enable(enable);
		}
	}
}
//----------------------------------------------------------------------------
void Node::SetActivate(bool act)
{
	Movable::SetActivate(act);

	for (int i = 0; i < GetNumChildren(); i++)
	{
		if (mChild[i] && !mChild[i]->IsActivateSelfCtrled())
		{
			mChild[i]->SetActivate(act);
		}
	}
}
//----------------------------------------------------------------------------
void Node::MarkAlphaColorBrightnessChanged()
{
	mIsAlphaColorBrightnessChanged = true;

	for (int i = 0; i < GetNumChildren(); i++)
	{
		Movable *child = GetChild(i);
		if (child)
		{
			if (!child->IsAlphaSelfCtrled() ||
				!child->IsColorSelfCtrled() ||
				!child->IsBrightnessSelfCtrled())
			{
				mChild[i]->MarkAlphaColorBrightnessChanged();
			}
		}
	}
}
//----------------------------------------------------------------------------
void Node::SetCastShadow(bool castShadow)
{
	Movable::SetCastShadow(castShadow);

	for (int i = 0; i < GetNumChildren(); i++)
	{
		if (mChild[i])
			mChild[i]->SetCastShadow(castShadow);
	}
}
//----------------------------------------------------------------------------
void Node::SetReceiveShadow(bool reciveShadow)
{
	Movable::SetReceiveShadow(reciveShadow);

	for (int i = 0; i < GetNumChildren(); i++)
	{
		if (mChild[i])
			mChild[i]->SetReceiveShadow(reciveShadow);
	}
}
//----------------------------------------------------------------------------
void Node::OnNotPicked (int pickInfo)
{
	for (int i=0; i<(int)mChild.size(); i++)
	{
		if (mChild[i])
		{
			mChild[i]->OnNotPicked(pickInfo);
		}
	}
}
//----------------------------------------------------------------------------
bool MovableUpdateLessThan (const Movable *mov0,
	const Movable *mov1)
{
	int priority0 = mov0->GetUpdatePriority();
	int priority1 = mov1->GetUpdatePriority();

	if (priority0 == priority1)
	{
		return mov0 < mov1;
	}

	return priority0 > priority1;
}
//----------------------------------------------------------------------------
void Node::UpdateWorldData(double applicationTime, double elapsedTime)
{
	Movable::UpdateWorldData(applicationTime, elapsedTime);

	_UpdateWorldDataChild(applicationTime, elapsedTime);
}
//----------------------------------------------------------------------------
void Node::_UpdateWorldDataChild(double applicationTime, double elapsedTime)
{
	if (mIsNeedCalUpdateChild)
	{
		mUpdateChild.clear();
		for (int i = 0; i < (int)mChild.size(); i++)
		{
			if (mChild[i])
				mUpdateChild.push_back(mChild[i]);
		}

		std::sort(mUpdateChild.begin(), mUpdateChild.end(), MovableUpdateLessThan);

		mIsNeedCalUpdateChild = false;
	}

	for (int i = 0; i < (int)mUpdateChild.size(); i++)
	{
		if (mUpdateChild[i])
			mUpdateChild[i]->Update(applicationTime, elapsedTime, false);
	}
}
//----------------------------------------------------------------------------
void Node::UpdateWorldBound ()
{
    if (!WorldBoundIsCurrent)
    {
        bool foundFirstBound = false;

		WorldBound.SetCenter(APoint::ORIGIN);
		WorldBound.SetRadius(0.0f);

		for (int i=0; i<(int)mChild.size(); i++)
		{
			Movable *child = mChild[i];

			if (child)
			{
				if (foundFirstBound)
				{
					// 将所有孩子的节点包围体融合到一起，作为当前节点的包围体
					WorldBound.GrowToContain(child->WorldBound);
				}
				else
				{
					// 首先将第一个孩子的包围盒作为节点的包围盒
					foundFirstBound = true;
					WorldBound = child->WorldBound;
				}
			}
		}
    }
}
//----------------------------------------------------------------------------
void Node::OnGetVisibleSet (Culler& culler, bool noCull)
{
	std::vector<MovablePtr>::iterator iter = mChild.begin();
    std::vector<MovablePtr>::iterator end = mChild.end();
    for (/**/; iter != end; ++iter)
    {
        Movable* child = *iter;
        if (child)
        {
			child->GetVisibleSet(culler, noCull);
        }
    }
}
//----------------------------------------------------------------------------
void Node::OnCulled(Culler& culler)
{
	for (int i = 0; i < GetNumChildren(); i++)
	{
		Movable *mov = GetChild(i);
		if (mov)
		{
			mov->OnCulled(culler);
		}
	}
}
//----------------------------------------------------------------------------
void Node::TravelExecute(Movable *mov, TravelExecuteFun fun, Any *data)
{
	bool goOn = true;

	(*fun)(mov, data, goOn);
	
	if (!goOn) return;

	Node *node = DynamicCast<Node>(mov);
	if (node)
	{
		for (int i = 0; i < node->GetNumChildren(); i++)
		{
			Movable *child = node->GetChild(i);

			if (child)
			{
				TravelExecute(child, fun, data);
			}
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Name support.
//----------------------------------------------------------------------------
Object* Node::GetObjectByName (const std::string& name)
{
    Object* found = Movable::GetObjectByName(name);
    if (found)
    {
        return found;
    }

    const int numChildren = (int)mChild.size();
    for (int i = 0; i < numChildren; ++i)
    {
        PX2_GET_OBJECT_BY_NAME(mChild[i], name, found);
    }

    return 0;
}
//----------------------------------------------------------------------------
void Node::GetAllObjectsByName (const std::string& name,
    std::vector<Object*>& objects)
{
    Movable::GetAllObjectsByName(name, objects);

    const int numChildren = (int)mChild.size();
    for (int i = 0; i < numChildren; ++i)
    {
        PX2_GET_ALL_OBJECTS_BY_NAME(mChild[i], name, objects);
    }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void Node::RegistProperties ()
{
	Movable::RegistProperties();

	AddPropertyClass("Node");

	AddProperty("NumChildren", Object::PT_INT, Any(GetNumChildren()), false);

	AddProperty("AnchorID", Object::PT_INT, GetAnchorID());
}
//----------------------------------------------------------------------------
void Node::OnPropertyChanged (const PropertyObject &obj)
{
	Movable::OnPropertyChanged(obj);

	if ("AnchorID" == obj.Name)
	{
		SetAnchorID(PX2_ANY_AS(obj.Data, int));
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
Node::Node(LoadConstructor value) :
Movable(value),
mIsDoPickPriority(false),
mIsNeedCalUpdateChild(true),
mAnchorID(0)
{
}
//----------------------------------------------------------------------------
void Node::Load (InStream& source)
{
    PX2_BEGIN_DEBUG_STREAM_LOAD(source);

    Movable::Load(source);
	PX2_VERSION_LOAD(source);

    int numChildren;
    source.Read(numChildren);
    if (numChildren > 0)
    {
        mChild.resize(numChildren);
        source.ReadPointerVV(numChildren, &mChild[0]);
    }

	source.Read(mAnchorID);

    PX2_END_DEBUG_STREAM_LOAD(Node, source);
}
//----------------------------------------------------------------------------
void Node::Link (InStream& source)
{
    Movable::Link(source);

    const int numChildren = (int)mChild.size();
    for (int i = 0; i < numChildren; ++i)
    {
        if (mChild[i])
        {
            source.ResolveLink(mChild[i]);
			mChild[i]->SetParent(this);
        }
    }
}
//----------------------------------------------------------------------------
void Node::PostLink ()
{
    Movable::PostLink();

	const int numChildren = (int)mChild.size();
	for (int i = 0; i < numChildren; ++i)
	{
		if (mChild[i])
		{
			OnChildAttached(mChild[i]);
			mChild[i]->OnBeAttached();
		}
	}
}
//----------------------------------------------------------------------------
bool Node::Register (OutStream& target) const
{
    if (Movable::Register(target))
    {
        const int numChildren = (int)mChild.size();
        for (int i = 0; i < numChildren; ++i)
        {
			if (mChild[i] && !mChild[i]->IsSaveWriteIngore())
            {
                target.Register(mChild[i]);
            }
        }
        return true;
    }
    return false;
}
//----------------------------------------------------------------------------
void Node::Save (OutStream& target) const
{
    PX2_BEGIN_DEBUG_STREAM_SAVE(target);

    Movable::Save(target);
	PX2_VERSION_SAVE(target);

    const int numChildren = (int)mChild.size();
    target.Write(numChildren);
    for (int i = 0; i < numChildren; ++i)
    {
        if (mChild[i])
        {
			if (mChild[i]->IsSaveWriteIngore())
			{
				target.WritePointer((Movable*)0);
			}
			else
			{
				target.WritePointer(mChild[i]);
			}
        }
        else
        {
            target.WritePointer((Movable*)0);
        }
    }

	target.Write(mAnchorID);

    PX2_END_DEBUG_STREAM_SAVE(Node, target);
}
//----------------------------------------------------------------------------
int Node::GetStreamingSize (Stream &stream) const
{
    int size = Movable::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
    int numChildren = (int)mChild.size();
    size += sizeof(numChildren);
	if (numChildren > 0) size += numChildren*PX2_POINTERSIZE(mChild[0]);

	size += sizeof(mAnchorID);

    return size;
}
//----------------------------------------------------------------------------
