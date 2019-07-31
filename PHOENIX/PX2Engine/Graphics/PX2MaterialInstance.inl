// PX2MaterialInstance.inl

//----------------------------------------------------------------------------
inline const std::string &MaterialInstance::GetMaterialFilename() const
{
	return mMaterialFilename;
}
//----------------------------------------------------------------------------
inline const std::string &MaterialInstance::GetTechniqueName() const
{
	return mInstanceTechName;
}
//----------------------------------------------------------------------------
inline Material* MaterialInstance::GetMaterial () const
{
	return mMaterial;
}
//----------------------------------------------------------------------------
inline int MaterialInstance::GetTechniqueIndex () const
{
	return mTechniqueIndex;
}
//----------------------------------------------------------------------------
inline int MaterialInstance::GetNumPasses () const
{
	return mMaterial->GetTechnique(mTechniqueIndex)->GetNumPasses();
}
//----------------------------------------------------------------------------
