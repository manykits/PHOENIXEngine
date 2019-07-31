// PX2AnimationObject.inl

//----------------------------------------------------------------------------
inline bool AnimationObject::IsBlendOver()
{
	return BlendTime > ANIMATION_BLENDTIME;
}
//----------------------------------------------------------------------------