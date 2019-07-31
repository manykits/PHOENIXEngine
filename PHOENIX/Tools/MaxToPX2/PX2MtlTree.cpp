// PX2MtlTree.cpp

#include "PX2MtlTree.hpp"

//----------------------------------------------------------------------------
MtlTree::MtlTree (PX2::Shine *shine)
{
	mShine = shine;
}
//----------------------------------------------------------------------------
void MtlTree::SetMChildQuantity (int quantity)
{
	mMChild.resize(quantity);
}
//----------------------------------------------------------------------------
int MtlTree::GetMChildQuantity () const
{
	return (int)mMChild.size();
}
//----------------------------------------------------------------------------
void MtlTree::SetShine (PX2::Shine *shine)
{
	mShine = shine;
}
//----------------------------------------------------------------------------
PX2::Shine *MtlTree::GetShine ()
{
	return mShine;
}
//----------------------------------------------------------------------------
MtlTree &MtlTree::GetMChild (int i)
{
	return mMChild[i];
}
//----------------------------------------------------------------------------
void MtlTree::SetMaterialInstance (PX2::MaterialInstance *mi)
{
	mMaterialInstance = mi;
}
//----------------------------------------------------------------------------
PX2::MaterialInstance *MtlTree::GetMaterialInstance ()
{
	return mMaterialInstance;
}
//----------------------------------------------------------------------------