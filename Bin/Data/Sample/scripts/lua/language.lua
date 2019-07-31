-- language.lua

function g_AddLanguage()
	PX2_LM_APP:Clear()
	PX2_LM_APP:AddItem1("General", "通用")
	PX2_LM_APP:AddItem1("UI", "界面")
	PX2_LM_APP:AddItem1("About", "关于")
	
	PX2_LM_APP:AddItem1("UIFrame", "框架")
	PX2_LM_APP:AddItem1("UIFPicBox", "图片")
	PX2_LM_APP:AddItem1("UIFText", "文字")
	PX2_LM_APP:AddItem1("UIButton", "按钮")
	PX2_LM_APP:AddItem1("UICheckBox", "判断框")
	PX2_LM_APP:AddItem1("UIComboBox", "选择框")
	PX2_LM_APP:AddItem1("UIEditBox", "编辑框")
	PX2_LM_APP:AddItem1("UIProgress", "进度条")
	PX2_LM_APP:AddItem1("UIColorSelect", "颜色选择")
	PX2_LM_APP:AddItem1("UIList", "列表")
	PX2_LM_APP:AddItem1("UITree", "树")
	PX2_LM_APP:AddItem1("UISplitter", "分隔线")
	PX2_LM_APP:AddItem1("UISlider", "滑动条")
	PX2_LM_APP:AddItem1("UIRound", "方向盘")
	PX2_LM_APP:AddItem1("UIWebFrame", "浏览器框")

	PX2_LM_APP:AddItem1("Server", "服务器")
end