// PX2UIComboBox.inl

//----------------------------------------------------------------------------
inline const std::vector<std::string> &UIComboBox::GetChooseStrs() const
{
	return mChooses;
}
//----------------------------------------------------------------------------
inline UIButton *UIComboBox::GetSelectButton()
{
	return mSelectButton;
}
//----------------------------------------------------------------------------
inline UIList *UIComboBox::GetChooseList()
{
	return mChooseList;
}
//----------------------------------------------------------------------------
inline int UIComboBox::GetChoose() const
{
	return mChoose;
}
//----------------------------------------------------------------------------