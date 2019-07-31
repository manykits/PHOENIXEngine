// PX2SwitchNode.cpp

#include "PX2SwitchNode.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Node, SwitchNode);
PX2_IMPLEMENT_STREAM(SwitchNode);
PX2_IMPLEMENT_FACTORY(SwitchNode);
PX2_IMPLEMENT_DEFAULT_NAMES(Node, SwitchNode);

//----------------------------------------------------------------------------
SwitchNode::SwitchNode ()
:
mActiveChild(SN_INVALID_CHILD)
{
}
//----------------------------------------------------------------------------
SwitchNode::~SwitchNode ()
{
}
//----------------------------------------------------------------------------
void SwitchNode::Play ()
{
	Node::Play();

	//Movable *actChild = GetActiveChildPointer();
	//if (actChild && !actChild->IsSelfCtrled())
	//{
	//	actChild->Play();
	//}
}
//----------------------------------------------------------------------------
bool SwitchNode::IsPlaying () const
{
	if (Node::IsPlaying())
		return true;

	//if (mActiveChild>=0 && mActiveChild<(int)mChild.size())
	//{
	//	if (mChild[mActiveChild] && mChild[mActiveChild]->IsPlaying()
	//		&& !mChild[mActiveChild]->IsSelfCtrled())
	//		return true;
	//}

	return false;
}
//----------------------------------------------------------------------------
void SwitchNode::Pause()
{
	Node::Pause();

	//Movable *actChild = GetActiveChildPointer();
	//if (actChild && !actChild->IsSelfCtrled())
	//{
	//	actChild->Stop();
	//}
}
//----------------------------------------------------------------------------
void SwitchNode::Reset ()
{
	Node::Reset();

	//Movable *actChild = GetActiveChildPointer();
	//if (actChild && !actChild->IsSelfCtrled())
	//{
	//	actChild->Reset();
	//}
}
//----------------------------------------------------------------------------
void SwitchNode::OnGetVisibleSet (Culler& culler, bool noCull)
{
	if (mActiveChild == SN_INVALID_CHILD)
	{
		return;
	}

	Movable* child = mChild[mActiveChild];
	if (child)
	{
		child->GetVisibleSet(culler, noCull);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void SwitchNode::RegistProperties ()
{
	Node::RegistProperties();

	AddPropertyClass("SwitchNode");
	AddProperty("ActiveChildIndex", PT_INT, GetActiveChild());
}
//----------------------------------------------------------------------------
void SwitchNode::OnPropertyChanged (const PropertyObject &obj)
{
	Node::OnPropertyChanged(obj);

	if ("ActiveChildIndex" == obj.Name)
	{
		SetActiveChild(PX2_ANY_AS(obj.Data, int));
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
SwitchNode::SwitchNode (LoadConstructor value)
:
Node(value),
mActiveChild(SN_INVALID_CHILD)
{
}
//----------------------------------------------------------------------------
void SwitchNode::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Node::Load(source);
	PX2_VERSION_LOAD(source);

	source.Read(mActiveChild);

	PX2_END_DEBUG_STREAM_LOAD(SwitchNode, source);
}
//----------------------------------------------------------------------------
void SwitchNode::Link (InStream& source)
{
	Node::Link(source);
}
//----------------------------------------------------------------------------
void SwitchNode::PostLink ()
{
	Node::PostLink();
}
//----------------------------------------------------------------------------
bool SwitchNode::Register (OutStream& target) const
{
	return Node::Register(target);
}
//----------------------------------------------------------------------------
void SwitchNode::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Node::Save(target);
	PX2_VERSION_SAVE(target);

	target.Write(mActiveChild);

	PX2_END_DEBUG_STREAM_SAVE(SwitchNode, target);
}
//----------------------------------------------------------------------------
int SwitchNode::GetStreamingSize (Stream &stream) const
{
	int size = Node::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	size += sizeof(mActiveChild);
	return size;
}
//----------------------------------------------------------------------------
