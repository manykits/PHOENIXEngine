// PX2BillboardNode.cpp

#include "PX2BillboardNode.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Node, BillboardNode);
PX2_IMPLEMENT_STREAM(BillboardNode);
PX2_IMPLEMENT_FACTORY(BillboardNode);

//----------------------------------------------------------------------------
BillboardNode::BillboardNode (Camera* camera)
	:
mCamera(camera),
mAlignType(BAT_DEFAULT)
{
}
//----------------------------------------------------------------------------
BillboardNode::~BillboardNode ()
{
}
//----------------------------------------------------------------------------
void BillboardNode::UpdateWorldData(double applicationTime, 
	double elapsedTime)
{
	Movable::UpdateWorldData(applicationTime, elapsedTime);

	if (mCamera)
	{
		// Inverse-transform the camera to the model space of the billboard.
		APoint modelPos = WorldTransform.Inverse()*mCamera->GetPosition();

		if (BAT_DEFAULT == mAlignType)
		{
			// To align the billboard, the projection of the camera to the
			// xz-plane of the billboard's model space determines the angle of
			// rotation about the billboard's model y-axis.  If the projected
			// camera is on the model axis (x = 0 and z = 0), ATan2 returns zero
			// (rather than NaN), so there is no need to trap this degenerate
			// case and handle it separately.
			float angle = Mathf::ATan2(modelPos[0], modelPos[2]);
			HMatrix orient(AVector::UNIT_Y, angle);
			WorldTransform.SetRotate(WorldTransform.GetRotate()*orient);
		}
		else if (BAT_Z == mAlignType)
		{
			float angle = Mathf::HALF_PI + Mathf::ATan2(modelPos[1], modelPos[0]);
			HMatrix orient(AVector::UNIT_Z, angle);

			float angle1 = Mathf::ATan2(modelPos[2], Mathf::Sqrt(modelPos[0]*modelPos[0] + modelPos[1]*modelPos[1]));
			HMatrix orient1(AVector::UNIT_X, -angle1);

			WorldTransform.SetRotate(WorldTransform.GetRotate()*orient*orient1);
		}
	}

	std::vector<MovablePtr>::iterator iter = mChild.begin();
	std::vector<MovablePtr>::iterator end = mChild.end();
	for (/**/; iter != end; ++iter)
	{
		Movable* child = *iter;
		if (child)
		{
			child->Update(applicationTime, false);
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Name support.
//----------------------------------------------------------------------------
Object* BillboardNode::GetObjectByName (const std::string& name)
{
	Object* found = Object::GetObjectByName(name);
	if (found)
	{
		return found;
	}

	PX2_GET_OBJECT_BY_NAME(mCamera, name, found);
	return 0;
}
//----------------------------------------------------------------------------
void BillboardNode::GetAllObjectsByName (const std::string& name,
	std::vector<Object*>& objects)
{
	Node::GetAllObjectsByName(name, objects);

	PX2_GET_ALL_OBJECTS_BY_NAME(mCamera, name, objects);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Streaming support.
//----------------------------------------------------------------------------
BillboardNode::BillboardNode (LoadConstructor value)
	:
Node(value),
	mCamera(0),
	mAlignType(BAT_DEFAULT)
{
}
//----------------------------------------------------------------------------
void BillboardNode::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Node::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadPointer(mCamera);

	PX2_END_DEBUG_STREAM_LOAD(BillboardNode, source);
}
//----------------------------------------------------------------------------
void BillboardNode::Link (InStream& source)
{
	Node::Link(source);

	source.ResolveLink(mCamera);
}
//----------------------------------------------------------------------------
void BillboardNode::PostLink ()
{
	Node::PostLink();
}
//----------------------------------------------------------------------------
bool BillboardNode::Register (OutStream& target) const
{
	if (Node::Register(target))
	{
		target.Register(mCamera);
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void BillboardNode::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Node::Save(target);
	PX2_VERSION_SAVE(target);

	target.WritePointer(mCamera);

	PX2_END_DEBUG_STREAM_SAVE(BillboardNode, target);
}
//----------------------------------------------------------------------------
int BillboardNode::GetStreamingSize (Stream &stream) const
{
	int size = Node::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_POINTERSIZE(mCamera);

	return size;
}
//----------------------------------------------------------------------------
