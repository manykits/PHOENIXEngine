// PX2Movable.cpp

#include "PX2LightNode.hpp"
#include "PX2GraphicsRoot.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Node, LightNode);
PX2_IMPLEMENT_STREAM(LightNode);
PX2_IMPLEMENT_FACTORY(LightNode);

//----------------------------------------------------------------------------
LightNode::LightNode (Light* light):
mLight(light),
mIsNeedUpdate(false)
{
	if (mLight)
	{
		LocalTransform.SetTranslate(mLight->Position);

		HMatrix rotate(mLight->DVector, mLight->UVector, mLight->RVector,
			APoint::ORIGIN, true);

		LocalTransform.SetRotate(rotate);
	}
}
//----------------------------------------------------------------------------
LightNode::~LightNode ()
{
}
//----------------------------------------------------------------------------
void LightNode::SetLight (Light* light)
{
	mLight = light;

	if (mLight)
	{
		LocalTransform.SetTranslate(mLight->Position);

		HMatrix rotate(mLight->DVector, mLight->UVector, mLight->RVector,
			APoint::ORIGIN, true);

		LocalTransform.SetRotate(rotate);

		Update();
	}
}
//----------------------------------------------------------------------------
void LightNode::UpdateWorldData(double applicationTime, double elapsedTime)
{
	Node::UpdateWorldData(applicationTime, elapsedTime);

	if (mLight)
	{
		mLight->Position = WorldTransform.GetTranslate();

		if (WorldTransform.IsRSMatrix())
		{
			WorldTransform.GetRotate().GetColumn(0, mLight->DVector);
			WorldTransform.GetRotate().GetColumn(1, mLight->UVector);
			WorldTransform.GetRotate().GetColumn(2, mLight->RVector);
		}
	}

	if (mIsNeedUpdate)
	{
		mIsNeedUpdate = false;
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Name support.
//----------------------------------------------------------------------------
Object* LightNode::GetObjectByName (const std::string& name)
{
	Object* found = Node::GetObjectByName(name);
	if (found)
	{
		return found;
	}

	PX2_GET_OBJECT_BY_NAME(mLight, name, found);
	return 0;
}
//----------------------------------------------------------------------------
void LightNode::GetAllObjectsByName (const std::string& name,
									 std::vector<Object*>& objects)
{
	Node::GetAllObjectsByName(name, objects);

	PX2_GET_ALL_OBJECTS_BY_NAME(mLight, name, objects);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
LightNode::LightNode (LoadConstructor value) :
Node(value),
mIsNeedUpdate(true)
{
}
//----------------------------------------------------------------------------
void LightNode::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Node::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadPointer(mLight);

	PX2_END_DEBUG_STREAM_LOAD(LightNode, source);
}
//----------------------------------------------------------------------------
void LightNode::Link (InStream& source)
{
	Node::Link(source);

	source.ResolveLink(mLight);
}
//----------------------------------------------------------------------------
void LightNode::PostLink ()
{
	Node::PostLink();
}
//----------------------------------------------------------------------------
bool LightNode::Register (OutStream& target) const
{
	if (Node::Register(target))
	{
		target.Register(mLight);
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void LightNode::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Node::Save(target);
	PX2_VERSION_SAVE(target);

	target.WritePointer(mLight);

	PX2_END_DEBUG_STREAM_SAVE(LightNode, target);
}
//----------------------------------------------------------------------------
int LightNode::GetStreamingSize (Stream &stream) const
{
	int size = Node::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	size += PX2_POINTERSIZE(mLight);
	return size;
}
//----------------------------------------------------------------------------
