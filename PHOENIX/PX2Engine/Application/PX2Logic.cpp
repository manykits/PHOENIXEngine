// PX2Logic.cpp

#include "PX2Logic.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Node, Logic);
PX2_IMPLEMENT_STREAM(Logic);
PX2_IMPLEMENT_FACTORY(Logic);
PX2_IMPLEMENT_DEFAULT_NAMES(Node, Logic);

//----------------------------------------------------------------------------
Logic::Logic()
{
	SetName("UI");

	ComeInEventWorld();
}
//----------------------------------------------------------------------------
Logic::~Logic()
{
}
//----------------------------------------------------------------------------
void Logic::OnEvent(Event *ent)
{
	PX2_UNUSED(ent);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void Logic::RegistProperties()
{
	Node::RegistProperties();

	AddPropertyClass("UI");
}
//----------------------------------------------------------------------------
void Logic::OnPropertyChanged(const PropertyObject &obj)
{
	Node::OnPropertyChanged(obj);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
Logic::Logic(LoadConstructor value) :
Node(value)
{
	ComeInEventWorld();
}
//----------------------------------------------------------------------------
void Logic::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Node::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(Logic, source);
}
//----------------------------------------------------------------------------
void Logic::Link(InStream& source)
{
	Node::Link(source);
}
//----------------------------------------------------------------------------
void Logic::PostLink()
{
	Node::PostLink();

}
//----------------------------------------------------------------------------
bool Logic::Register(OutStream& target) const
{
	if (Node::Register(target))
	{

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void Logic::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Node::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(Logic, target);
}
//----------------------------------------------------------------------------
int Logic::GetStreamingSize(Stream &stream) const
{
	int size = Node::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------