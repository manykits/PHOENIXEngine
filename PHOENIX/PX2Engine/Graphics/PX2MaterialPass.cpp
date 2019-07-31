// PX2MaterialPass.cpp

#include "PX2MaterialPass.hpp"
#include "PX2Renderer.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Object, MaterialPass);
PX2_IMPLEMENT_STREAM(MaterialPass);
PX2_IMPLEMENT_FACTORY(MaterialPass);

//----------------------------------------------------------------------------
MaterialPass::MaterialPass ()
	:
PdrPointer(0)
{
}
//----------------------------------------------------------------------------
MaterialPass::~MaterialPass ()
{
	Renderer::UnbindAll(this);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Ãû³Æ
//----------------------------------------------------------------------------
Object* MaterialPass::GetObjectByName (const std::string& name)
{
    Object* found = Object::GetObjectByName(name);
    if (found)
    {
        return found;
    }

    PX2_GET_OBJECT_BY_NAME(mVShader, name, found);
    PX2_GET_OBJECT_BY_NAME(mPShader, name, found);
    PX2_GET_OBJECT_BY_NAME(mAlphaProperty, name, found);
    PX2_GET_OBJECT_BY_NAME(mCullProperty, name, found);
    PX2_GET_OBJECT_BY_NAME(mDepthProperty, name, found);
    PX2_GET_OBJECT_BY_NAME(mOffsetProperty, name, found);
    PX2_GET_OBJECT_BY_NAME(mStencilProperty, name, found);
    PX2_GET_OBJECT_BY_NAME(mWireProperty, name, found);

    return 0;
}
//----------------------------------------------------------------------------
void MaterialPass::GetAllObjectsByName (const std::string& name,
    std::vector<Object*>& objects)
{
    Object::GetAllObjectsByName(name, objects);

    PX2_GET_ALL_OBJECTS_BY_NAME(mVShader, name, objects);
    PX2_GET_ALL_OBJECTS_BY_NAME(mPShader, name, objects);
    PX2_GET_ALL_OBJECTS_BY_NAME(mAlphaProperty, name, objects);
    PX2_GET_ALL_OBJECTS_BY_NAME(mCullProperty, name, objects);
    PX2_GET_ALL_OBJECTS_BY_NAME(mDepthProperty, name, objects);
    PX2_GET_ALL_OBJECTS_BY_NAME(mOffsetProperty, name, objects);
    PX2_GET_ALL_OBJECTS_BY_NAME(mStencilProperty, name, objects);
    PX2_GET_ALL_OBJECTS_BY_NAME(mWireProperty, name, objects);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void MaterialPass::RegistProperties ()
{
	Object::RegistProperties();
	AddPropertyClass("MaterialPass");
}
//----------------------------------------------------------------------------
void MaterialPass::OnPropertyChanged (const PropertyObject &obj)
{
	Object::OnPropertyChanged(obj);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
MaterialPass::MaterialPass (LoadConstructor value)
    :
    Object(value),
	PdrPointer(0)
{
}
//----------------------------------------------------------------------------
void MaterialPass::Load (InStream& source)
{
    PX2_BEGIN_DEBUG_STREAM_LOAD(source);

    Object::Load(source);
	PX2_VERSION_LOAD(source);

    source.ReadPointer(mVShader);
    source.ReadPointer(mPShader);
    source.ReadPointer(mAlphaProperty);
    source.ReadPointer(mCullProperty);
    source.ReadPointer(mDepthProperty);
    source.ReadPointer(mOffsetProperty);
    source.ReadPointer(mStencilProperty);
    source.ReadPointer(mWireProperty);

    PX2_END_DEBUG_STREAM_LOAD(MaterialPass, source);
}
//----------------------------------------------------------------------------
void MaterialPass::Link (InStream& source)
{
    Object::Link(source);

    source.ResolveLink(mVShader);
    source.ResolveLink(mPShader);
    source.ResolveLink(mAlphaProperty);
    source.ResolveLink(mCullProperty);
    source.ResolveLink(mDepthProperty);
    source.ResolveLink(mOffsetProperty);
    source.ResolveLink(mStencilProperty);
    source.ResolveLink(mWireProperty);
}
//----------------------------------------------------------------------------
void MaterialPass::PostLink ()
{
    Object::PostLink();
}
//----------------------------------------------------------------------------
bool MaterialPass::Register (OutStream& target) const
{
    if (Object::Register(target))
    {
        target.Register(mVShader);
        target.Register(mPShader);
        target.Register(mAlphaProperty);
        target.Register(mCullProperty);
        target.Register(mDepthProperty);
        target.Register(mOffsetProperty);
        target.Register(mStencilProperty);
        target.Register(mWireProperty);
        return true;
    }
    return false;
}
//----------------------------------------------------------------------------
void MaterialPass::Save (OutStream& target) const
{
    PX2_BEGIN_DEBUG_STREAM_SAVE(target);

    Object::Save(target);
	PX2_VERSION_SAVE(target);

    target.WritePointer(mVShader);
    target.WritePointer(mPShader);
    target.WritePointer(mAlphaProperty);
    target.WritePointer(mCullProperty);
    target.WritePointer(mDepthProperty);
    target.WritePointer(mOffsetProperty);
    target.WritePointer(mStencilProperty);
    target.WritePointer(mWireProperty);

    PX2_END_DEBUG_STREAM_SAVE(MaterialPass, target);
}
//----------------------------------------------------------------------------
int MaterialPass::GetStreamingSize (Stream &stream) const
{
    int size = Object::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

    size += PX2_POINTERSIZE(mVShader);
    size += PX2_POINTERSIZE(mPShader);
    size += PX2_POINTERSIZE(mAlphaProperty);
    size += PX2_POINTERSIZE(mCullProperty);
    size += PX2_POINTERSIZE(mDepthProperty);
    size += PX2_POINTERSIZE(mOffsetProperty);
    size += PX2_POINTERSIZE(mStencilProperty);
    size += PX2_POINTERSIZE(mWireProperty);
    return size;
}
//----------------------------------------------------------------------------
