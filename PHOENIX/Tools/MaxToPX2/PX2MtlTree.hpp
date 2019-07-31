// PX2MtlTree.hpp

#ifndef PX2MTLTREE_HPP
#define PX2MTLTREE_HPP

#include "PX2Shine.hpp"
#include "PX2MaterialInstance.hpp"

class MtlTree
{
public:
	MtlTree (PX2::Shine *shine=0);

	// ²ÄÖÊÊ÷
	void SetMChildQuantity (int quantity);
	int GetMChildQuantity () const;
	void SetShine (PX2::Shine *shine);
	PX2::Shine *GetShine ();
	MtlTree &GetMChild (int i);

	// ÌùÍ¼Ê÷
	void SetMaterialInstance (PX2::MaterialInstance *mi);
	PX2::MaterialInstance *GetMaterialInstance ();

private:
	PX2::ShinePtr mShine;
	PX2::MaterialInstancePtr mMaterialInstance;
	std::vector<MtlTree> mMChild;
};

#endif