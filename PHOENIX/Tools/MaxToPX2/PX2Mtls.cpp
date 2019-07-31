// PX2Mtls.cpp

#include "PX2Mtls.hpp"
using namespace std;

//----------------------------------------------------------------------------
Mtls::Mtls ()
{
}
//----------------------------------------------------------------------------
bool Mtls::Add (Mtl *mtl)
{
	if (mtl == 0)
		return false;

	for (int i=0; i<(int)mMaterial.size(); i++)
	{
		if (mMaterial[i] == mtl)
			return false;
	}

	mMaterial.push_back(mtl);
	
	return true;
}
//----------------------------------------------------------------------------
void Mtls::SetQuantity (int quantity)
{
	mMaterial.resize(quantity);
}
//----------------------------------------------------------------------------
int Mtls::GetQuantity () const
{
	return (int)mMaterial.size();
}
//----------------------------------------------------------------------------
int Mtls::GetIndex (Mtl *mtl) const
{
	for (int i = 0; i < (int)mMaterial.size(); i++)
	{
		if (mMaterial[i] == mtl)
			return i;
	}

	return -1;
}
//----------------------------------------------------------------------------
Mtl *Mtls::GetMtl (int mtlIndex) const
{
	if (0<=mtlIndex && mtlIndex<(int)mMaterial.size())
		return mMaterial[mtlIndex];

	return 0;
}
//----------------------------------------------------------------------------