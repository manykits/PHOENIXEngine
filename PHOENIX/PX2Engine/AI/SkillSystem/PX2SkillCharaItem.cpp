// PX2SkillCharaItem.cpp

#include "PX2SkillChara.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
bool SkillChara::AddItem(Item *item)
{
	assertion(0 != item, "item should not be 0.");

	if (!item)
		return false;

	if (HasItem(item))
		return false;

	mItems.push_back(item);

	item->SetChara(this);
	item->OnAdded();

	return true;
}
//----------------------------------------------------------------------------
bool SkillChara::HasItem(Item *item)
{
	for (int i = 0; i < (int)mItems.size(); i++)
	{
		if (item == mItems[i])
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
Item *SkillChara::GetItem(int i)
{
	assertion(0 <= i&&i < (int)mItems.size(), "i should be in right range.\n");

	if (0 <= i && i < (int)mItems.size())
	{
		return mItems[i];
	}

	return 0;
}
//----------------------------------------------------------------------------
Item *SkillChara::GetItemByTypeID(int typeID)
{
	for (int i = 0; i < (int)mItems.size(); i++)
	{
		Item *item = mItems[i];
		if (item && item->GetTypeID()==typeID)
		{
			return item;
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
int SkillChara::GetNumItems() const
{
	return (int)mItems.size();
}
//----------------------------------------------------------------------------
void SkillChara::RemoveAllItems()
{
	std::vector<ItemPtr>::iterator it = mItems.begin();
	if (it != mItems.end())
	{
		(*it)->OnRemove();
		(*it)->SetChara(0);
	}

	mItems.clear();
}
//----------------------------------------------------------------------------
bool SkillChara::RemoveItem(Item *item)
{
	assertion(0 != item, "item should not be 0.");

	if (!item)
		return false;

	std::vector<ItemPtr>::iterator it = mItems.begin();
	if (it != mItems.end())
	{
		(*it)->OnRemove();
		(*it)->SetChara(0);

		mItems.erase(it);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void SkillChara::EquipItem(Item *item)
{
	if (IsItemEquipped(item))
		return;

	mEquippedItems.push_back(item);
	item->SetEquipped(true);
}
//----------------------------------------------------------------------------
bool SkillChara::IsItemEquipped(Item *item)
{
	for (int i = 0; i < (int)mEquippedItems.size(); i++)
	{
		if (mEquippedItems[i] == item)
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void SkillChara::UnEquipItem(Item *item)
{
	auto it = mEquippedItems.begin();
	for (; it != mEquippedItems.end(); it++)
	{
		if (item == *it)
		{
			mEquippedItems.erase(it);
			item->SetEquipped(false);
		}
	}
}
//----------------------------------------------------------------------------
void SkillChara::UnEquipAllItems()
{
	for (int i = 0; i < (int)mEquippedItems.size(); i++)
	{
		mEquippedItems[i]->SetEquipped(false);
	}

	mEquippedItems.clear();
}
//----------------------------------------------------------------------------
int SkillChara::GetNumEquippedItem() const
{
	return (int)mEquippedItems.size();
}
//----------------------------------------------------------------------------
Item *SkillChara::GetEquippedItem(int i)
{
	if (0 <= i && i < (int)mEquippedItems.size())
	{
		return mEquippedItems[i];
	}

	return 0;
}
//----------------------------------------------------------------------------