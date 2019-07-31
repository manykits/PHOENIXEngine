// PX2Mtls.hpp

#ifndef PX2MTLS_HPP
#define PX2MTLS_HPP

#include "PX2MaxToPX2PluginPre.hpp"

class Mtl;

class Mtls
{
public:
	Mtls ();

	bool Add (Mtl *mtl);
	void SetQuantity (int quantity);
	int GetQuantity () const;
	int GetIndex (Mtl *mtl) const;
	Mtl *GetMtl (int mtlIndex) const;

private:
	std::vector<Mtl*> mMaterial;
};

#endif