// PX2PropertyGrid.cpp

#include "PX2UIPropertyGrid.hpp"
#include "PX2Transform.hpp"
#include "PX2UIComboBox.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2UIPropertyItem.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UITree, UIPropertyGrid);
PX2_IMPLEMENT_STREAM(UIPropertyGrid);
PX2_IMPLEMENT_FACTORY(UIPropertyGrid);
PX2_IMPLEMENT_DEFAULT_NAMES(UITree, UIPropertyGrid);

//----------------------------------------------------------------------------
UIPropertyGrid::UIPropertyGrid() 
{
	mSpliterFrame = new0 UISplitterFrame(false);
	mMaskFrame->AttachChild(mSpliterFrame);
	mSpliterFrame->SetSize(3.0f, 0.0f);
	mSpliterFrame->LocalTransform.SetTranslateY(-5.0f);
	mSpliterFrame->SetAnchorHor(0.5f, 0.5f);
	mSpliterFrame->SetAnchorParamHor(0.0f, 0.0f);
	mSpliterFrame->SetAnchorVer(0.0f, 1.0f);
	mSpliterFrame->SetAnchorParamVer(0.0f, 0.0f);
	mSpliterFrame->SetDragType(UISplitterFrame::DT_PERCENT);
	mSpliterFrame->Show(false);

	mSpliterFrame->SetMemUICallback(this, 
		(UIFrame::MemUICallback)(&UIPropertyGrid::_SpliterDragingCallback));

	NotFreeObjectPool<UIPropertyItem> *classPool = 
		new0 NotFreeObjectPool<UIPropertyItem>();
	Any initDataClass = UIPropertyItem::IT_CLASS;
	classPool->AllocAllObjects(10, initDataClass);
	mItemPools[UIPropertyItem::IT_CLASS] = classPool;

	NotFreeObjectPool<UIPropertyItem> *colorButtonPool =
		new0 NotFreeObjectPool<UIPropertyItem>();
	Any initDataColorButton = UIPropertyItem::IT_COLORBUTTON;
	colorButtonPool->AllocAllObjects(10, initDataColorButton);
	mItemPools[UIPropertyItem::IT_COLORBUTTON] = colorButtonPool;

	NotFreeObjectPool<UIPropertyItem> *comboxPool =
		new0 NotFreeObjectPool<UIPropertyItem>();
	Any initDataComboBox = UIPropertyItem::IT_COMBOX;
	comboxPool->AllocAllObjects(45, initDataComboBox);
	mItemPools[UIPropertyItem::IT_COMBOX] = comboxPool;

	NotFreeObjectPool<UIPropertyItem> *editBoxPool =
		new0 NotFreeObjectPool<UIPropertyItem>();
	Any initDataEditBox = UIPropertyItem::IT_EDITBOX;
	editBoxPool->AllocAllObjects(60, initDataEditBox);
	mItemPools[UIPropertyItem::IT_EDITBOX] = editBoxPool;

	NotFreeObjectPool<UIPropertyItem> *editBox2Pool =
		new0 NotFreeObjectPool<UIPropertyItem>();
	Any initDataEditBox2 = UIPropertyItem::IT_EDITBOX2;
	editBox2Pool->AllocAllObjects(10, initDataEditBox2);
	mItemPools[UIPropertyItem::IT_EDITBOX2] = editBox2Pool;

	NotFreeObjectPool<UIPropertyItem> *editBox3Pool =
		new0 NotFreeObjectPool<UIPropertyItem>();
	Any initDataEditBox3 = UIPropertyItem::IT_EDITBOX3;
	editBox3Pool->AllocAllObjects(35, initDataEditBox3);
	mItemPools[UIPropertyItem::IT_EDITBOX3] = editBox3Pool;

	NotFreeObjectPool<UIPropertyItem> *editBoxBut =
		new0 NotFreeObjectPool<UIPropertyItem>();
	Any initDataEditBoxBut = UIPropertyItem::IT_EDITBOXBUT;
	editBoxBut->AllocAllObjects(10, initDataEditBoxBut);
	mItemPools[UIPropertyItem::IT_EDITBOXBUT] = editBoxBut;

	NotFreeObjectPool<UIPropertyItem> *transformPool =
		new0 NotFreeObjectPool<UIPropertyItem>();
	Any initDataTransform = UIPropertyItem::IT_TRANSFORM;
	transformPool->AllocAllObjects(5, initDataTransform);
	mItemPools[UIPropertyItem::IT_TRANSFORM] = transformPool;

	NotFreeObjectPool<UIPropertyItem> *editSliderformPool =
		new0 NotFreeObjectPool<UIPropertyItem>();
	Any initDataEditSlider = UIPropertyItem::IT_EDITBOXSLIDER;
	editSliderformPool->AllocAllObjects(10, initDataEditSlider);
	mItemPools[UIPropertyItem::IT_EDITBOXSLIDER] = editSliderformPool;
}
//----------------------------------------------------------------------------
UIPropertyGrid::~UIPropertyGrid()
{
	std::map<UIPropertyItem::ItemType, NotFreeObjectPool<UIPropertyItem> *>
		::iterator it = mItemPools.begin();
	for (; it != mItemPools.end(); it++)
	{
		delete0(it->second);
	}
	mItemPools.clear();
}
//----------------------------------------------------------------------------
void UIPropertyGrid::_SpliterDragingCallback(UIFrame *frame, UICallType type)
{
	if (UICT_SPLITTER_DRAGING == type)
	{
		UISplitterFrame *spFrame = DynamicCast<UISplitterFrame>(frame);
		if (spFrame)
		{	
			for (int i = 0; i < (int)mEditObjs.size(); i++)
			{
				_SetEditObjPos(mEditObjs[i]);
			}

			for (int i = 0; i < (int)mAllItems.size(); i++)
			{
				_SetItemPos(mAllItems[i]);
			}
		}
	}
}
//----------------------------------------------------------------------------
void UIPropertyGrid::_AddEditObj(UIFrame *obj)
{
	mEditObjs.push_back(obj);

	_SetEditObjPos(obj);
}
//----------------------------------------------------------------------------
void UIPropertyGrid::_SetItemPos(UIPropertyItem *item)
{
	float anchorHor = mSpliterFrame->GetAnchorHor()[0];

	UIPropertyItem::ItemType it = item->GetItemType();
	if (it != UIPropertyItem::IT_CLASS)
	{
		item->GetFText()->SetAnchorHor(0.0f, anchorHor);
		item->GetFText()->SetAnchorParamHor(0.0f, -mSpliterFrame->GetSize().Width / 2.0f);
		item->GetFText()->SetAnchorVer(0.0f, 1.0f);
		item->GetFText()->SetAnchorParamVer(0.0f, 0.0f);
	}
}
//----------------------------------------------------------------------------
void UIPropertyGrid::_SetEditObjPos(UIFrame *frame)
{
	float anchorHor = mSpliterFrame->GetAnchorHor()[0];

	frame->SetAnchorHor(anchorHor, 1.0f);
	frame->SetAnchorParamHor(mSpliterFrame->GetSize().Width/2.0f, 0.0f);
	frame->SetAnchorVer(0.0f, 1.0f);
	frame->SetAnchorParamVer(0.5f, -0.5f);
}
//----------------------------------------------------------------------------
UIPropertyItem::ItemType UIPropertyGrid::_GetPropertyItemType(
	Object::PropertyType pt)
{
	if (Object::PT_CLASS == pt)
	{
		return UIPropertyItem::IT_CLASS;
	}
	else if (Object::PT_INT == pt || PT_FLOAT == pt)
	{
		return UIPropertyItem::IT_EDITBOX;
	}
	else if (Object::PT_FLOATSLIDER == pt)
	{
		return UIPropertyItem::IT_EDITBOXSLIDER;
	}
	else if (Object::PT_BOOL == pt || PT_BOOLCHECK==pt || Object::PT_ENUM == pt)
	{
		return UIPropertyItem::IT_COMBOX;
	}
	else if (Object::PT_FLOAT2 == pt || PT_SIZE == pt)
	{
		return UIPropertyItem::IT_EDITBOX2;
	}
	else if (Object::PT_COLOR3FLOAT3 == pt)
	{
		return UIPropertyItem::IT_COLORBUTTON;
	}
	else if (Object::PT_FLOAT3 == pt || Object::PT_FLOAT4 == pt ||
		Object::PT_APOINT3 == pt || Object::PT_APOINT4 == pt ||
		Object::PT_AVECTOR3 == pt || Object::PT_AVECTOR4 == pt)
	{
		return UIPropertyItem::IT_EDITBOX3;
	}
	else if (Object::PT_RECT == pt)
	{
		return UIPropertyItem::IT_EDITBOX4;
	}
	else if (Object::PT_STRING == pt)
	{
		return UIPropertyItem::IT_EDITBOX;
	}
	else if (Object::PT_STRINGBUTTON == pt)
	{
		return UIPropertyItem::IT_EDITBOXBUT;
	}
	else if (Object::PT_TRANSFORM == pt)
	{
		return UIPropertyItem::IT_TRANSFORM;
	}
	else if (Object::PT_EMFLOAT == pt)
	{
		return UIPropertyItem::IT_EDITBOX;
	}
	else if (Object::PT_EMFLOATRANGE == pt)
	{
		return UIPropertyItem::IT_EDITBOX;
	}
	else if (Object::PT_EMFLOAT3 == pt)
	{
		return UIPropertyItem::IT_EDITBOX;
	}
	else if (Object::PT_EMFLOAT3RANGE == pt)
	{
		return UIPropertyItem::IT_EDITBOX;
	}

	return UIPropertyItem::IT_NONE;
}
//----------------------------------------------------------------------------
NotFreeObjectPool<UIPropertyItem> *UIPropertyGrid::_GetPool(
	UIPropertyItem::ItemType itemType)
{
	std::map<UIPropertyItem::ItemType, NotFreeObjectPool<UIPropertyItem> *>
		::iterator it = mItemPools.find(itemType);
	if (it == mItemPools.end())
		return 0;

	return it->second;
}
//----------------------------------------------------------------------------
UIPropertyItem *UIPropertyGrid::AddItemType(UIItem *parentItem,
	const std::string &label, Object::PropertyType pt,
	const std::string &name, const Any &data, const Any &data1, 
	float sliderMin, float sliderMax,
	bool isEnable, Object *obj)
{
	mSpliterFrame->Show(true);

	// get pool
	UIPropertyItem::ItemType itemType = _GetPropertyItemType(pt);
	NotFreeObjectPool<UIPropertyItem> *pool = _GetPool(itemType);
	if (!pool) return 0;

	// new item
	UIPropertyItem *item = pool->AllocObject();

	parentItem->AttachChild(item);

	item->SetName(name);
	item->SetSize(parentItem->GetSize());
	item->GetFText()->GetText()->SetText(label);
	item->SetItemObject(obj);
	item->GetFText()->GetText()->SetFontColor(PX2_UISM.Color_ContentFont);
	item->SetItemType(itemType, pt, data, data1, sliderMin, sliderMax, isEnable);

	UIFrame *editCtrl = item->GetEditCtrl();
	if (editCtrl)
	{
		_AddEditObj(editCtrl);
	}

	mAllItems.push_back(item);
	_SetItemPos(item);

	// but back
	UIButton *butBack = item->GetButBack();
	if (butBack)
	{
		butBack->SetMemUICallback(this,
			(UIFrame::MemUICallback)(&UITree::SelectCallback));
	}

	if (!name.empty())
	{
		mTagItems[name] = item;
	}

	mMaskFrame->SetNeedAdjustChildrenMask(true);

	return item;
}
//----------------------------------------------------------------------------
void UIPropertyGrid::RemoveAllProperties()
{
	mSpliterFrame->Show(false);

	for (int i = 0; i < (int)mAllItems.size(); i++)
	{
		UIPropertyItem *propertyItem = mAllItems[i];
		if (propertyItem != mRootItem)
		{
			NotFreeObjectPool<UIPropertyItem> *pool = _GetPool(
				propertyItem->GetItemType());
			if (pool)
			{
				pool->FreeObject(propertyItem->GetObjectID());
			}
		}
	}
	mAllItems.clear();

	mRootItem->RemoveAllChildItems();
	mLastClassItem = 0;

	mTagItems.clear();
	mEditObjs.clear();
}
//----------------------------------------------------------------------------
UIItem *UIPropertyGrid::AddProperty(const std::string &name,
	const std::string &label, PropertyType type, const Any &data, 
	const Any &data1, float sliderMin, float sliderMax, bool isEnable)
{
	UIPropertyItem *item = 0;
	if (PT_CLASS == type)
	{
		item = AddItemType(mRootItem, label, type, name, data, data1, sliderMin, sliderMax, isEnable);
		mLastClassItem = item;
	}
	else
	{
		if (PT_INT == type || PT_BOOL == type || PT_FLOAT == type ||
			PT_INTSLIDER==type || PT_FLOATSLIDER==type ||
			PT_FLOAT2 == type || PT_FLOAT3 == type || PT_COLOR3FLOAT3 == type ||
			PT_AVECTOR3 == type || PT_APOINT3 == type || PT_ENUM == type
			|| PT_SIZE==type || PT_STRING == type || PT_STRINGBUTTON == type)
		{
			item = AddItemType(mLastClassItem, label, type, name, data, 
				data1, sliderMin, sliderMax, isEnable);
		}
		else if (PT_TRANSFORM == type)
		{
			UIItem *transformItem = AddItemType(mLastClassItem, label, type,
				name, data, data1, sliderMin, sliderMax, isEnable);

			Transform trans = PX2_ANY_AS(data, Transform);
			const APoint &pos = trans.GetTranslate();
			float rotX = 0.0f; float rotY = 0.0f; float rotZ = 0.0f;
			trans.GetRotate(rotX, rotY, rotZ);
			APoint rot(rotX*Mathf::RAD_TO_DEG, rotY*Mathf::RAD_TO_DEG, rotZ*Mathf::RAD_TO_DEG);
			APoint scale = trans.GetScale();

			UIPropertyItem *item0 = AddItemType(transformItem, "Pos", Object::PT_APOINT3, name + "_Sub0", pos, data1, sliderMin, sliderMax, isEnable);
			item0->SetUserData("sub_tag", std::string("pos"));
			UIPropertyItem *item1 = AddItemType(transformItem, "Rotate", Object::PT_APOINT3, name + "_Sub1", rot, data1, sliderMin, sliderMax, isEnable);
			item1->SetUserData("sub_tag", std::string("rotate"));
			UIPropertyItem *item2 = AddItemType(transformItem, "Scale", Object::PT_APOINT3, name + "_Sub2", scale, data1, sliderMin, sliderMax, isEnable);
			item2->SetUserData("sub_tag", std::string("scale"));
		}
		else if (PT_EMFLOAT == type)
		{
			InterpCurvePointFloat point = PX2_ANY_AS(data, InterpCurvePointFloat);
			float inVal = point.InVal;
			float outVal = point.OutVal;
			float arriveTagent = point.ArriveTangent;
			float leaveTarget = point.LeaveTangent;

			UIPropertyItem *itemEMFloat = AddItemType(mLastClassItem, label, type, name, data, data1, sliderMin, sliderMax, isEnable);
			AddItemType(itemEMFloat, "In", Object::PT_FLOAT, name + "InVal", inVal, data1, sliderMin, sliderMax, isEnable);
			AddItemType(itemEMFloat, "Out", Object::PT_FLOAT, name + "OutVal", outVal, data1, sliderMin, sliderMax, isEnable);
			AddItemType(itemEMFloat, "ArriveTangent", Object::PT_FLOAT, name + "ArriveTangent", arriveTagent, data1, sliderMin, sliderMax, isEnable);
			AddItemType(itemEMFloat, "LeaveTangent", Object::PT_FLOAT, name + "LeaveTangent", leaveTarget, data1, sliderMin, sliderMax, isEnable);
		}
		else if (PT_EMFLOATRANGE == type)
		{
			InterpCurvePointFloat pointMin = PX2_ANY_AS(data, InterpCurvePointFloat);
			float inValMin = pointMin.InVal;
			float outValMin = pointMin.OutVal;
			float arriveTagentMin = pointMin.ArriveTangent;
			float leaveTargetMin = pointMin.LeaveTangent;

			InterpCurvePointFloat pointMax = PX2_ANY_AS(data1, InterpCurvePointFloat);
			float inValMax = pointMax.InVal;
			float outValMax = pointMax.OutVal;
			float arriveTagentMax = pointMax.ArriveTangent;
			float leaveTargetMax = pointMax.LeaveTangent;

			UIItem *itemEMFloatRangeMin = AddItemType(mLastClassItem, label + "Min", type, name, data, data1, sliderMin, sliderMax, isEnable);
			AddItemType(itemEMFloatRangeMin, "In", Object::PT_FLOAT, name + "InValMin", inValMin, data1, sliderMin, sliderMax, isEnable);
			AddItemType(itemEMFloatRangeMin, "Out", Object::PT_FLOAT, name + "OutValMin", outValMin, data1, sliderMin, sliderMax, isEnable);
			AddItemType(itemEMFloatRangeMin, "ArriveTangent", Object::PT_FLOAT, name + "ArriveTangentMin", arriveTagentMin, data1, sliderMin, sliderMax, isEnable);
			AddItemType(itemEMFloatRangeMin, "LeaveTangent", Object::PT_FLOAT, name + "LeaveTangentMin", leaveTargetMin, data1, sliderMin, sliderMax, isEnable);

			UIItem *itemEMFloatRangeMax = AddItemType(mLastClassItem, label + "Max", type, name, data, data1, sliderMin, sliderMax, isEnable);
			AddItemType(itemEMFloatRangeMax, "In", Object::PT_FLOAT, name + "InValMax", inValMax, data1, sliderMin, sliderMax, isEnable);
			AddItemType(itemEMFloatRangeMax, "Out", Object::PT_FLOAT, name + "OutValMax", outValMax, data1, sliderMin, sliderMax, isEnable);
			AddItemType(itemEMFloatRangeMax, "ArriveTangent", Object::PT_FLOAT, name + "ArriveTangentMax", arriveTagentMax, data1, sliderMin, sliderMax, isEnable);
			AddItemType(itemEMFloatRangeMax, "LeaveTangent", Object::PT_FLOAT, name + "LeaveTangentMax", leaveTargetMax, data1, sliderMin, sliderMax, isEnable);
		}
		else if (PT_EMFLOAT3 == type)
		{
			InterpCurvePointFloat3 point = PX2_ANY_AS(data, InterpCurvePointFloat3);
			float inVal = point.InVal;
			Float3 outVal = point.OutVal;
			Float3 arriveTagent = point.ArriveTangent;
			Float3 leaveTarget = point.LeaveTangent;

			UIPropertyItem *itemEMFloat = AddItemType(mLastClassItem, label, type, name, data, data1, sliderMin, sliderMax, isEnable);
			AddItemType(itemEMFloat, "In", Object::PT_FLOAT, name + "InVal", inVal, data1, sliderMin, sliderMax, isEnable);
			AddItemType(itemEMFloat, "Out", Object::PT_FLOAT3, name + "OutVal", outVal, data1, sliderMin, sliderMax, isEnable);
			AddItemType(itemEMFloat, "ArriveTangent", Object::PT_FLOAT3, name + "ArriveTangent", arriveTagent, data1, sliderMin, sliderMax, isEnable);
			AddItemType(itemEMFloat, "LeaveTangent", Object::PT_FLOAT3, name + "LeaveTangent", leaveTarget, data1, sliderMin, sliderMax, isEnable);
		}
		else if (PT_EMFLOAT3RANGE == type)
		{
			InterpCurvePointFloat3 pointMin = PX2_ANY_AS(data, InterpCurvePointFloat3);
			float inValMin = pointMin.InVal;
			Float3 outValMin = pointMin.OutVal;
			Float3 arriveTagentMin = pointMin.ArriveTangent;
			Float3 leaveTargetMin = pointMin.LeaveTangent;

			InterpCurvePointFloat3 pointMax = PX2_ANY_AS(data1, InterpCurvePointFloat3);
			float inValMax = pointMax.InVal;
			Float3 outValMax = pointMax.OutVal;
			Float3 arriveTagentMax = pointMax.ArriveTangent;
			Float3 leaveTargetMax = pointMax.LeaveTangent;

			UIItem *itemEMFloat3RangeMin = AddItemType(mLastClassItem, label + "Min", type, name, data, data1, sliderMin, sliderMax, isEnable);
			AddItemType(itemEMFloat3RangeMin, "In", Object::PT_FLOAT, name + "InValMin", inValMin, data1, sliderMin, sliderMax, isEnable);
			AddItemType(itemEMFloat3RangeMin, "Out", Object::PT_FLOAT3, name + "OutValMin", outValMin, data1, sliderMin, sliderMax, isEnable);
			AddItemType(itemEMFloat3RangeMin, "ArriveTangent", Object::PT_FLOAT3, name + "ArriveTangentMin", arriveTagentMin, data1, sliderMin, sliderMax, isEnable);
			AddItemType(itemEMFloat3RangeMin, "LeaveTangent", Object::PT_FLOAT3, name + "LeaveTangentMin", leaveTargetMin, data1, sliderMin, sliderMax, isEnable);

			UIItem *itemEMFloat3RangeMax = AddItemType(mLastClassItem, label + "Max", type, name, data, data1, sliderMin, sliderMax, isEnable);
			AddItemType(itemEMFloat3RangeMax, "In", Object::PT_FLOAT, name + "InValMax", inValMax, data1, sliderMin, sliderMax, isEnable);
			AddItemType(itemEMFloat3RangeMax, "Out", Object::PT_FLOAT3, name + "OutValMax", outValMax, data1, sliderMin, sliderMax, isEnable);
			AddItemType(itemEMFloat3RangeMax, "ArriveTangent", Object::PT_FLOAT3, name + "ArriveTangentMax", arriveTagentMax, data1, sliderMin, sliderMax, isEnable);
			AddItemType(itemEMFloat3RangeMax, "LeaveTangent", Object::PT_FLOAT3, name + "LeaveTangentMax", leaveTargetMax, data1, sliderMin, sliderMax, isEnable);
		}
		else if (PT_EMFLOAT3COLOR == type)
		{
			UIItem *itemEMFloat3Color = AddItemType(mLastClassItem, label, type,
				name, data, data1, sliderMin, sliderMax, isEnable);
		}
		else if (PT_EMFLOAT3COLORRANGE == type)
		{
			UIItem *itemEMFloat3ColorRange = AddItemType(mLastClassItem, label, type,
				name, data, data1, sliderMin, sliderMax, isEnable);
		}
		else
		{
			item = AddItemType(mLastClassItem, "Not_" + label,
				Object::PT_STRING, name, data, data1, sliderMin, sliderMax, isEnable);
		}
	}

	return item;
}
//----------------------------------------------------------------------------
void UIPropertyGrid::OnItemChanged(UIItem *item)
{
	PX2_UNUSED(item);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIPropertyGrid::UIPropertyGrid(LoadConstructor value) :
UITree(value)
{
}
//----------------------------------------------------------------------------
void UIPropertyGrid::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UITree::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UIPropertyGrid, source);
}
//----------------------------------------------------------------------------
void UIPropertyGrid::Link(InStream& source)
{
	UITree::Link(source);
}
//----------------------------------------------------------------------------
void UIPropertyGrid::PostLink()
{
	UITree::PostLink();
}
//----------------------------------------------------------------------------
bool UIPropertyGrid::Register(OutStream& target) const
{
	if (UITree::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIPropertyGrid::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UITree::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UIPropertyGrid, target);
}
//----------------------------------------------------------------------------
int UIPropertyGrid::GetStreamingSize(Stream &stream) const
{
	int size = UITree::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------