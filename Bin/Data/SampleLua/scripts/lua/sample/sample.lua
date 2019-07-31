-- sample.lua

-- 全局
g_s_t =
{
	Size_LeftBar = 50,
	Color_LeftBar = Float3:MakeColor(51, 51, 51),
	Size_LeftBarBut = 48,
}

g_s_UIFrame = nil
g_s_TableFrame = nil
g_s_MsgList = nil
g_s_AnimCtrl = nil

-- 创建首页
function sampleLua_Sample()
	PX2_ENGINECANVAS:SetClearColor(Float4.BLACK)
	PX2_ENGINESCENECANVAS:SetClearColor(Float4.BLACK)

	local musicName = "thelittleadventurer.mp3"
	PX2_RM:ReWriteFileToWriteablePath("Data/SampleLua/media/music/"..musicName, musicName)
	local dstPath = ResourceManager:GetWriteablePath()..musicName
	PX2_SS:PlayMusic(0, dstPath, true, 0.0)
	
	-- scene
	local scene = Scene:New()
	PX2_PROJ:SetScene(scene)
	
	local box = PX2_CREATER:CreateMovable_Box()
	scene:AttachChild(box)
	box.LocalTransform:SetTranslateZ(1.0)
	box.LocalTransform:SetUniformScale(2.0)

	-- model
	local model = PX2_CREATER:CreateNode_Model("Data/SampleLua/models/actors/swk/model.px2obj")
	scene:AttachChild(model)
	model.LocalTransform:SetUniformScale(0.1)
	model.LocalTransform:SetRotateDegree(0.0, 0.0, 180.0)
	model.LocalTransform:SetTranslate(2.5, 0.0, 0.0)
	g_s_AnimCtrl = model:GetControllerByName("ModelController")
	g_s_AnimCtrl:SetSkinAnimationConfig("Data/SampleLua/models/actors/swk/anim.xml")
	local anim = g_s_AnimCtrl:GetAnimByName("show0")	
	anim:SetPlayOnce(false)
	anim:SetFrequency(1.0)
	g_s_AnimCtrl:PlayAnim(anim)
	
	local ctrlSphere = ActorCtrl:New({val = 1000, _name="Sphere"})
	ctrlSphere._val = 1000
	model:AttachController(Cast:ToSC(ctrlSphere.__object))		
	
	-- model1
	local model1 = PX2_CREATER:CreateNode_Model("Data/SampleLua/models/actors/swk/model.px2obj")
	scene:AttachChild(model1)
	model1.LocalTransform:SetUniformScale(0.1)
	model1.LocalTransform:SetRotateDegree(0.0, 0.0, 0.0)
	model1.LocalTransform:SetTranslate(-5, 0.0, 0.0)
	model1:ResetPlay()	
	
	local ctrlBox = ActorCtrl:New({val = 1001, _name="Box"})
	ctrlBox._val = 1001	
	model1:AttachController(Cast:ToSC(ctrlBox.__object))

	PX2_LOGGER:LogInfo("script_lua", "end create models")
	
	for k, v in pairs(gScriptTable) do
		PX2_LOGGER:LogInfo("script_lua", "gScriptTable:"..k)
	end	
	--local scTable = gScriptTable[ctrlBox:SelfP()]
	--scTable._val = 1002	

	PX2_LOGGER:LogInfo("script_lua", "end gScriptTable")
	
	-- cam model
	local mainCameraNode = scene:GetMainCameraNode()
	local cameraPlayCtrl = PX2_CREATER:CreateCameraPlayController()
	model:AttachController(cameraPlayCtrl)
	cameraPlayCtrl:SetCameraNode(mainCameraNode)
	cameraPlayCtrl:SetCameraDistance(60.0)
	cameraPlayCtrl:SetCameraDist(30.0, 60.0)
	cameraPlayCtrl:ResetPlay()

	PX2_LOGGER:LogInfo("script_lua", "end camera node")
	
	if nil~=FBXImporter then
		-- fbx model
		local importer = FBXImporter:New()
		importer:Import("Data/SampleLua/models/actors/liuxing/model.FBX")		
		local modelFBX = importer:GetPX2Node()		
	
		local modelLiuxing = PX2_CREATER:CreateNode_Model(modelFBX)
		scene:AttachChild(modelLiuxing)
		modelLiuxing.LocalTransform:SetUniformScale(5)
		modelLiuxing.LocalTransform:SetTranslate(5, 0.0, 0.0)		
		FBXImporter:Delete(importer)
		
		local importer1 = FBXImporter:New()
		importer1:Import("Data/SampleLua/models/actors/liuxing/model.fbx")
		local modelAnim = importer1:GetPX2Node()
		
		local animCtrl = modelLiuxing:GetControllerByName("ModelController")
		animCtrl:SetSkinAnimationConfig("Data/SampleLua/models/actors/liuxing/anim.xml", modelAnim)
		local anim = animCtrl:GetAnimByName("show0")	
		anim:SetPlayOnce(false)
		anim:SetFrequency(1.0)
		animCtrl:PlayAnim(anim)		
		
		FBXImporter:Delete(importer1)		
	end

	-- panel
	local rectangle = PX2_CREATER:CreateMovable_Rectangle()
	scene:AttachChild(rectangle)
	rectangle.LocalTransform:SetUniformScale(10.0)

	PX2_LOGGER:LogInfo("script_lua", "end rectangle")

	-- sky
	local sky = PX2_CREATER:CreateSkyBox("Data/engine/sky/skybox/thickcloudswater/")
	scene:AttachChild(sky)

	PX2_LOGGER:LogInfo("script_lua", "end sky")

	-- ui
	local ui = PX2_PROJ:GetUI()
	
	local frame = UIFrame:New()
	ui:AttachChild(frame)
	frame.LocalTransform:SetTranslateY(-1.0)
	frame:SetAnchorHor(0.0, 1.0)
	frame:SetAnchorVer(0.0, 1.0)
	
	-- main but
	local but = UIButton:New()
	frame:AttachChild(but)
	but:SetName("ButHome")
	but:SetAnchorHor(0.0, 0.0)
	but:SetAnchorVer(1.0, 1.0)
	but:SetSize(50, 50)
	but:SetAnchorParamHor(50, 50)
	but:SetAnchorParamVer(-100, -100)
	but:SetScriptHandler("s_Callback")	
	local fPicBox = UIFPicBox:New()
	but:AttachChild(fPicBox)
	fPicBox.LocalTransform:SetTranslateY(-2.0)		
	fPicBox:GetUIPicBox():SetTexture("Data/SampleLua/images/icons/run.png")
	fPicBox:SetSize(30.0, 30.0)	
	
	-- uiFrame
	g_s_UIFrame = UIFrame:New()
	frame:AttachChild(g_s_UIFrame)
	g_s_UIFrame:SetAnchorHor(0.0, 1.0)
	g_s_UIFrame:SetAnchorVer(0.0, 1.0)
	g_s_UIFrame:SetAnchorParamHor(25.0, -25.0)
	g_s_UIFrame:SetAnchorParamVer(25.0, -100.0)
	
	local msgFrameWidth = 150.0
	
	g_s_TableFrame = UITabFrame:New()
	g_s_UIFrame:AttachChild(g_s_TableFrame)
	g_s_TableFrame.LocalTransform:SetTranslateY(-10.0)
	g_s_TableFrame:SetAnchorParamHor(0.0, -msgFrameWidth)	
	g_s_TableFrame:CreateAddBackgroundPicBox(true, Float3:MakeColor(200, 200, 200))
	g_s_TableFrame:SetTabBarHeight(40)
	g_s_TableFrame:SetTabHeight(35)
	g_s_TableFrame:SetTabWidth(40)
	
	g_s_TableFrame:AddTab("About", PX2_LM_APP:GetValue("About"), s_CreateUIFrameAbout())
	g_s_TableFrame:AddTab("UIFrame", PX2_LM_APP:GetValue("UIFrame"), s_CreateUIFrame())
	g_s_TableFrame:AddTab("UIFPicBox", PX2_LM_APP:GetValue("UIFPicBox"), s_CreateUIFPicBox())
	g_s_TableFrame:AddTab("UIFText", PX2_LM_APP:GetValue("UIFText"), s_CreateUIFText())
	g_s_TableFrame:AddTab("UIButton", PX2_LM_APP:GetValue("UIButton"), s_CreateUIButton())
	g_s_TableFrame:AddTab("UICheckBox", PX2_LM_APP:GetValue("UICheckBox"), s_CreateUICheckBox())
	g_s_TableFrame:AddTab("UIComboBox", PX2_LM_APP:GetValue("UIComboBox"), s_CreateUIComboBox())
	g_s_TableFrame:AddTab("UIEditBox", PX2_LM_APP:GetValue("UIEditBox"), s_CreateUIEditBox())
	g_s_TableFrame:AddTab("UIProgress", PX2_LM_APP:GetValue("UIProgress"), s_CreateUIProgress())
	g_s_TableFrame:AddTab("UIColorSelect", PX2_LM_APP:GetValue("UIColorSelect"), s_CreateUIColorSelect())
	g_s_TableFrame:AddTab("UIList", PX2_LM_APP:GetValue("UIList"), s_CreateUIList())
	g_s_TableFrame:AddTab("UITree", PX2_LM_APP:GetValue("UITree"), s_CreateUITree())
	g_s_TableFrame:AddTab("UISplitter", PX2_LM_APP:GetValue("UISplitter"), s_CreateUISplitter())
	g_s_TableFrame:AddTab("UISlider", PX2_LM_APP:GetValue("UISlider"), s_CreateUISlider())
	g_s_TableFrame:AddTab("UIRound", PX2_LM_APP:GetValue("UIRound"), s_CreateUIRound())
	--g_s_TableFrame:AddTab("UIWebFrame", PX2_LM_APP:GetValue("UIWebFrame"), s_CreateUIWebFrame())
	g_s_TableFrame:AddTab("Server", PX2_LM_APP:GetValue("Server"), s_CreateUIServer())
	g_s_TableFrame:SetActiveTab("About")
	
	local rightFrame = UIFrame:New()
	g_s_UIFrame:AttachChild(rightFrame)
	rightFrame:CreateAddBackgroundPicBox(true)
	rightFrame:SetSize(msgFrameWidth, 0.0)
	rightFrame:SetPivot(0.0, 0.5)
	rightFrame:SetAnchorHor(1.0, 1.0)
	rightFrame:SetAnchorParamHor(-msgFrameWidth, 0.0)
	rightFrame:SetAnchorVer(0.0, 1.0)
	
	g_s_MsgList = UIList:New()
	rightFrame:AttachChild(g_s_MsgList)
	g_s_MsgList:SetAnchorHor(0.0, 1.0)
	g_s_MsgList:SetAnchorVer(0.0, 1.0)
	g_s_MsgList:AddItem("msglist")
		
	g_s_UIFrame:Show(false)	
		
	-- anim buts
	local animBut0 = UIButton:New()
	frame:AttachChild(animBut0)
	animBut0:SetName("ButAnim0")
	animBut0:SetAnchorHor(0.0, 0.0)
	animBut0:SetAnchorVer(1.0, 1.0)
	animBut0:SetSize(50, 50)
	animBut0:SetAnchorParamHor(110, 110)
	animBut0:SetAnchorParamVer(-100.0, -100.0)
	animBut0:SetScriptHandler("s_Callback")	
	local fPicBox0 = UIFPicBox:New()
	animBut0:AttachChild(fPicBox0)
	fPicBox0.LocalTransform:SetTranslateY(-2.0)		
	fPicBox0:GetUIPicBox():SetTexture("Data/SampleLua/images/icons/anim.png")
	fPicBox0:SetSize(30.0, 30.0)	
	
	local animBut1 = UIButton:New()
	frame:AttachChild(animBut1)
	animBut1:SetName("ButAnim1")
	animBut1:SetAnchorHor(0.0, 0.0)
	animBut1:SetAnchorVer(1.0, 1.0)
	animBut1:SetSize(50, 50)
	animBut1:SetAnchorParamHor(170.0, 170.0)
	animBut1:SetAnchorParamVer(-100.0, -100.0)
	animBut1:SetScriptHandler("s_Callback")	
	local fPicBox1 = UIFPicBox:New()
	animBut1:AttachChild(fPicBox1)
	fPicBox1.LocalTransform:SetTranslateY(-2.0)		
	fPicBox1:GetUIPicBox():SetTexture("Data/SampleLua/images/icons/anim.png")
	fPicBox1:SetSize(30.0, 30.0)
	
	local logo = UIFPicBox:New()
	frame:AttachChild(logo)
	logo:SetName("Logo")
	logo:SetAnchorHor(0.0, 0.0)
	logo:SetAnchorParamHor(50.0, 50.0)
	logo:SetAnchorVer(0.0, 0.0)
	logo:SetAnchorParamVer(50.0, 50.0)
	logo:SetSize(50.0, 50.0)
	logo:GetUIPicBox():SetTexture("Data/engine/phoenix.png")
	
	local textInfo = UIFText:New()
	frame:AttachChild(textInfo)
	textInfo:GetText():SetText("使用PHOENIXEngine开发你的游戏，智能玩具，机器人应用")
	textInfo:SetAnchorHor(0.0, 0.0)
	textInfo:SetAnchorParamHor(100.0, 100.0)
	textInfo:SetPivot(0.0, 0.5)
	textInfo:SetAnchorVer(0.0, 0.0)
	textInfo:SetAnchorParamVer(50.0, 50.0)
	textInfo:SetWidth(700.0)
	textInfo:GetText():SetFontColor(Float3.WHITE)
	textInfo:GetText():SetAligns(TEXTALIGN_LEFT+TEXTALIGN_TOP)

	PX2_LOGGER:LogInfo("script_lua", "end sampleLua_Sample")
end
-- PrintMsg
function s_PrintMsg(str)
	g_s_MsgList:AddItem(str)
end
-- 按钮回调
function s_Callback(objPtr, callType)
	local obj = Cast:ToO(objPtr)
	
	local name = obj:GetName()
	if UICT_PRESSED==callType then
		if "But0"==name then
			s_PrintMsg("UICT_PRESSED But0")
		elseif "But1"==name then
			s_PrintMsg("UICT_PRESSED But1")
		elseif "But2"==name then		
			s_PrintMsg("UICT_PRESSED But2")
		end
	elseif UICT_RELEASED==callType then
		if "ButHome" == name then
			g_s_UIFrame:Show(not g_s_UIFrame:IsShow())
		elseif "ButAnim0" == name then
			g_s_AnimCtrl:PlayAnimByName("walk")
		elseif "ButAnim1" == name then
			g_s_AnimCtrl:PlayAnimByName("attack0")
		elseif "But0"==name then
			s_PrintMsg("UICT_RELEASED But0")
		elseif "But1"==name then
			s_PrintMsg("UICT_RELEASED But1")
		elseif "But2"==name then		
			s_PrintMsg("UICT_RELEASED But2")
		end
	elseif UICT_RELEASED_NOTPICK==callType then
		s_PrintMsg("UICT_RELEASED_NOTPICK")
	elseif UICT_CHECKED==callType then
		s_PrintMsg("UICT_CHECKED")
	elseif UICT_DISCHECKED==callType then
		s_PrintMsg("UICT_DISCHECKED")
	elseif UICT_EDITBOX_TEXTCHANGED==callType then
		s_PrintMsg("UICT_EDITBOX_TEXTCHANGED")
	elseif UICT_EDITBOX_ENTER==callType then 
		s_PrintMsg("UICT_EDITBOX_ENTER")
	elseif UICT_LIST_SELECTED==callType then
		s_PrintMsg("UICT_LIST_SELECTED")
	elseif UICT_COMBOBOX_CHOOSED==callType then
		s_PrintMsg("UICT_COMBOBOX_CHOOSED")
	elseif UICT_TREE_SELECTED==callType then
		s_PrintMsg("UICT_TREE_SELECTED")
	elseif UICT_COLOR_SELECT==callType then
		s_PrintMsg("UICT_COLOR_SELECT")
	elseif UICT_ROUND_DRAG==callType then
		local dirSimple = obj:GetDragingDirSimple()
		s_PrintMsg("UICT_ROUND_DRAG"..dirSimple)
	elseif UICT_SLIDERCHANGED==callType then
		local perc = obj:GetPercent()
		s_PrintMsg("UICT_SLIDERCHANGED"..perc)
	end
end

-- About
function s_CreateUIFrameAbout()
	local frame = UIFrame:New()
	frame.LocalTransform:SetTranslateY(-1.0)
	frame:SetAnchorHor(0.0, 1.0)
	frame:SetAnchorVer(0.0, 1.0)
	
	local fText = UIFText:New()
	frame:AttachChild(fText)
	fText:SetAnchorHor(0.0, 1.0)
	fText:SetAnchorVer(0.0, 1.0)
	fText:GetText():SetText("欢迎使用#RPHOENIXEngine#K进行开发! :-D ");
	
	return frame
end

-- UIFrame
function s_CreateUIFrame()
	local frame = UIFrame:New()
	frame.LocalTransform:SetTranslateY(-1.0)
	frame:SetAnchorHor(0.0, 1.0)
	frame:SetAnchorVer(0.0, 1.0)	
	
	local frame0 = UIFrame:New()
	frame:AttachChild(frame0)
	frame0:CreateAddBackgroundPicBox(true, Float3.RED)
	frame0:SetAnchorHor(0.0, 1.0)
	frame0:SetAnchorVer(1.0, 1.0)
	frame0:SetSize(0.0, 100.0)
	frame0:SetAnchorParamHor(50.0, -50.0)
	frame0:SetAnchorParamVer(-50.0, -5.0)
	
	local frame1 = UIFrame:New()
	frame:AttachChild(frame1)
	frame1:CreateAddBackgroundPicBox(true, Float3.GREEN)
	frame1:SetPivot(0.5, 1.0)
	frame1:SetAnchorHor(0.0, 1.0)
	frame1:SetAnchorVer(1.0, 1.0)
	frame1:SetSize(0.0, 100.0)
	frame1:SetAnchorParamHor(50.0, -50.0)
	frame1:SetAnchorParamVer(-100.0, -100.0)
	
	local frame2 = UIFrame:New()
	frame:AttachChild(frame2)
	frame2:CreateAddBackgroundPicBox(false, Float3.WHITE)
	frame2:SetPivot(0.5, 1.0)
	frame2:SetAnchorHor(0.0, 0.0)
	frame2:SetAnchorVer(1.0, 1.0)
	frame2:SetSize(200.0, 200.0)
	frame2:SetAnchorParamHor(150.0, 0.0)
	frame2:SetAnchorParamVer(-200.0, 0.0)
	
	return frame
end

-- UIFPicBox
function s_CreateUIFPicBox()
	local frame = UIFrame:New()
	frame.LocalTransform:SetTranslateY(-1.0)
	frame:SetAnchorHor(0.0, 1.0)
	frame:SetAnchorVer(0.0, 1.0)
	
	local size = 120
	
	local gridAlignCtrl = frame:CreateAddGridAlignCtrl()
	gridAlignCtrl:SetChildAlignment(UIFrameGridAlignControl.CAT_MIDDLE_CENTER)
	gridAlignCtrl:SetConstraintType(UIFrameGridAlignControl.CT_FIXED_COLUMN_COUNT)
	gridAlignCtrl:SetConstraintValue(4)
	gridAlignCtrl:SetBorder(10.0, 10.0, 10.0, 10.0);
	gridAlignCtrl:SetCellSize(Sizef(size, size))
	
	local fPicBox = UIFPicBox:New()
	frame:AttachChild(fPicBox)
	fPicBox:SetSize(size, size)
	fPicBox:GetUIPicBox():SetTexture("Data/SampleLua/images/pics/arduino.png")
	fPicBox:RegistToScriptSystem()
	
	local fPicBox1 = UIFPicBox:New()
	frame:AttachChild(fPicBox1)
	fPicBox1:SetSize(size, size)
	fPicBox1:GetUIPicBox():SetUVRepeat(3, 3)
	fPicBox1:GetUIPicBox():SetTexture("Data/SampleLua/images/pics/raspberry.png")
	fPicBox1:RegistToScriptSystem()
	
	local fPicBox2 = UIFPicBox:New()
	frame:AttachChild(fPicBox2)
	fPicBox2:SetSize(size, size)
	fPicBox2:GetUIPicBox():SetPicBoxType(UIPicBox.PBT_NINE)
	fPicBox2:GetUIPicBox():SetTexCornerSize(8, 8, 8, 8)
	fPicBox2:GetUIPicBox():SetTexture("Data/SampleLua/images/pics/but.png")
	fPicBox2:RegistToScriptSystem()
	
	local fPicBox3 = UIFPicBox:New()
	frame:AttachChild(fPicBox3)
	fPicBox3:SetSize(size, size)
	fPicBox3:GetUIPicBox():SetPicBoxType(UIPicBox.PBT_NINE)
	fPicBox3:GetUIPicBox():SetTexCornerSize(8, 8, 8, 8)
	fPicBox3:GetUIPicBox():SetTexture("Data/engine/common.xml", "but_normal")
	fPicBox3:RegistToScriptSystem()
	
	return frame
end

-- UIFText
function s_CreateUIFText()
	local frame = UIFrame:New()
	frame.LocalTransform:SetTranslateY(-1.0)
	frame:SetAnchorHor(0.0, 1.0)
	frame:SetAnchorVer(0.0, 1.0)
	
	local size = 200
	
	local gridAlignCtrl = frame:CreateAddGridAlignCtrl()
	gridAlignCtrl:SetChildAlignment(UIFrameGridAlignControl.CAT_MIDDLE_CENTER)
	gridAlignCtrl:SetConstraintType(UIFrameGridAlignControl.CT_FIXED_COLUMN_COUNT)
	gridAlignCtrl:SetConstraintValue(3)
	gridAlignCtrl:SetBorder(10.0, 10.0, 10.0, 10.0);
	gridAlignCtrl:SetCellSize(Sizef(size, size))
	
	local fText = UIFText:New()
	frame:AttachChild(fText)
	fText:SetSize(size, size)
	fText:GetText():SetFontColor(Float3:MakeColor(55, 0, 0))
	fText:GetText():SetFontScale(1.5)
	fText:RegistToScriptSystem()
	
	local fText1 = UIFText:New()
	frame:AttachChild(fText1)
	fText1:SetSize(size, size)
	fText1:CreateAddBackgroundPicBox(true)
	fText1:GetText():SetFont("Data/SampleLua/fonts/hyswlongfangsong.ttf", 24, 24, FES_UNDERLINE)
	fText1:GetText():SetText("人工智能")
	fText1:GetText():SetBorderShadowColor(Float3(100, 0.0, 0.0))
	fText1:GetText():SetDrawStyle(FD_SHADOW)
	fText1:GetText():SetAligns(TEXTALIGN_LEFT+TEXTALIGN_TOP)
	fText1:RegistToScriptSystem()
	
	local fText2 = UIFText:New()
	frame:AttachChild(fText2)
	fText2:SetSize(size, size)
	fText2:CreateAddBackgroundPicBox(true)
	fText2:GetText():SetText("智能娱乐智能娱乐智能娱乐智能娱乐智能娱乐智能娱乐智能娱乐")
	fText2:GetText():SetRectUseage(UIText.RU_CLIPWARP)
	fText2:GetText():SetAutoWarp(true)
	fText2:RegistToScriptSystem()
	
	return frame
end

-- UIButton
function s_CreateUIButton()
	local frame = UIFrame:New()
	frame.LocalTransform:SetTranslateY(-1.0)
	frame:SetAnchorHor(0.0, 1.0)
	frame:SetAnchorVer(0.0, 1.0)
	
	local size = 200
	
	local gridAlignCtrl = frame:CreateAddGridAlignCtrl()
	gridAlignCtrl:SetChildAlignment(UIFrameGridAlignControl.CAT_MIDDLE_CENTER)
	gridAlignCtrl:SetConstraintType(UIFrameGridAlignControl.CT_FIXED_COLUMN_COUNT)
	gridAlignCtrl:SetConstraintValue(3)
	gridAlignCtrl:SetBorder(10.0, 10.0, 10.0, 10.0);
	gridAlignCtrl:SetCellSize(Sizef(size, size))
	
	local but = UIButton:New()
	frame:AttachChild(but)
	but:SetName("But0")
	but:SetSize(size, size)
	but:CreateAddText("按钮0")
	but:SetScriptHandler("s_Callback")
	but:RegistToScriptSystem()
	
	local but1 = UIButton:New()
	frame:AttachChild(but1)
	but1:SetName("But1")
	but1:SetButType(UIButtonBase.BT_PICBOXSWAP)	
	but1:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetPicBoxType(UIPicBox.PBT_NINE)
	but1:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexCornerSize(8, 8, 8, 8)
	but1:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/engine/common.xml", "but_normal")	
	but1:GetPicBoxAtState(UIButtonBase.BS_HOVERED):SetPicBoxType(UIPicBox.PBT_NINE)
	but1:GetPicBoxAtState(UIButtonBase.BS_HOVERED):SetTexCornerSize(8, 8, 8, 8)
	but1:GetPicBoxAtState(UIButtonBase.BS_HOVERED):SetTexture("Data/engine/common.xml", "but_over")	
	but1:GetPicBoxAtState(UIButtonBase.BS_PRESSED):SetPicBoxType(UIPicBox.PBT_NINE)
	but1:GetPicBoxAtState(UIButtonBase.BS_PRESSED):SetTexCornerSize(8, 8, 8, 8)
	but1:GetPicBoxAtState(UIButtonBase.BS_PRESSED):SetTexture("Data/engine/common.xml", "but_down")	
	but1:SetSize(size, size)
	but1:CreateAddText("按钮1")
	but1:SetScriptHandler("s_Callback")
	but1:RegistToScriptSystem()
	
	local but2 = UIButton:New()
	frame:AttachChild(but2)
	but2:SetName("But2")
	but2:SetSize(size, size)
	but2:CreateAddText("按钮3")
	but2:SetScriptHandler("s_Callback")
	but2:RegistToScriptSystem()
	
	return frame
end

-- UICheckBox
function s_CreateUICheckBox()
	local frame = UIFrame:New()
	frame.LocalTransform:SetTranslateY(-1.0)
	frame:SetAnchorHor(0.0, 1.0)
	frame:SetAnchorVer(0.0, 1.0)	
	
	local checkButton = UICheckButton:New()
	frame:AttachChild(checkButton)
	checkButton:SetName("CheckButton")
	checkButton:SetSize(40.0, 40.0)
	checkButton:SetScriptHandler("s_Callback")
	checkButton:RegistToScriptSystem()
	
	return frame
end

-- UIComboBox
function s_CreateUIComboBox()
	local frame = UIFrame:New()
	frame.LocalTransform:SetTranslateY(-1.0)
	frame:SetAnchorHor(0.0, 1.0)
	frame:SetAnchorVer(0.0, 1.0)
	
	local comboBox = UIComboBox:New()
	frame:AttachChild(comboBox)
	comboBox:AddChooseStr("Choose0")
	comboBox:AddChooseStr("Choose1")
	comboBox:ChooseStr("Choose0")
	comboBox:SetScriptHandler("s_Callback")
	comboBox:RegistToScriptSystem()
	
	return frame
end

-- UIEditBox
function s_CreateUIEditBox()
	local frame = UIFrame:New()
	frame.LocalTransform:SetTranslateY(-1.0)
	frame:SetAnchorHor(0.0, 1.0)
	frame:SetAnchorVer(0.0, 1.0)	
	
	local editbox = UIEditBox:New()
	frame:AttachChild(editbox)
	editbox:SetScriptHandler("s_Callback")
	editbox:RegistToScriptSystem()
	
	return frame
end

-- UIProgress
function s_CreateUIProgress()
	local frame = UIFrame:New()
	frame.LocalTransform:SetTranslateY(-1.0)
	frame:SetAnchorHor(0.0, 1.0)
	frame:SetAnchorVer(0.0, 1.0)	
	
	local progressBar = UIProgressBar:New()
	frame:AttachChild(progressBar)
	progressBar:SetProgress(0.5)
	progressBar:SetScriptHandler("s_Callback")
	progressBar:RegistToScriptSystem()
	
	return frame
end

-- UIColorSelect
function s_CreateUIColorSelect()
	local frame = UIFrame:New()
	frame.LocalTransform:SetTranslateY(-1.0)
	frame:SetAnchorHor(0.0, 1.0)
	frame:SetAnchorVer(0.0, 1.0)	
	
	local colorSelect = UIColorSelector:New()
	frame:AttachChild(colorSelect)
	colorSelect:SetScriptHandler("s_Callback")
	colorSelect:SetSize(300, 300)
	colorSelect:SetAnchorHor(0.5, 0.5)
	colorSelect:SetAnchorVer(0.5, 0.5)
	colorSelect:RegistToScriptSystem()
	
	return frame
end

-- UIList
function s_CreateUIList()
	local frame = UIFrame:New()
	frame.LocalTransform:SetTranslateY(-1.0)
	frame:SetAnchorHor(0.0, 1.0)
	frame:SetAnchorVer(0.0, 1.0)	
	
	local list = UIList:New()
	frame:AttachChild(list)
	list:SetScriptHandler("s_Callback")
	list:AddItem("a")
	list:AddItem("b")
	list:AddItem("c")
	list:AddItem("d")
	list:RegistToScriptSystem()
	
	return frame
end

-- UITree
function s_CreateUITree()
	local frame = UIFrame:New()
	frame.LocalTransform:SetTranslateY(-1.0)
	frame:SetAnchorHor(0.0, 1.0)
	frame:SetAnchorVer(0.0, 1.0)	
	
	local tree = UITree:New()
	frame:AttachChild(tree)
	tree:SetScriptHandler("s_Callback")
	local itemA = tree:AddItem(tree:GetRootItem(), "a", "a")
		tree:AddItem(itemA, "aa", "aa")
	local itemB = tree:AddItem(tree:GetRootItem(), "b", "b")
		tree:AddItem(itemB, "bb", "bb")
	local itemC = tree:AddItem(tree:GetRootItem(), "c", "c")
		tree:AddItem(itemC, "cc", "cc")
	tree:SetSize(200, 200)
	tree:RegistToScriptSystem()
	
	return frame
end

-- UISplitter
function s_CreateUISplitter()
	local frame = UIFrame:New()
	frame.LocalTransform:SetTranslateY(-1.0)
	frame:SetAnchorHor(0.0, 1.0)
	frame:SetAnchorVer(0.0, 1.0)
	
	local splitter = UISplitterFrame:New()
	frame:AttachChild(splitter)
	splitter.LocalTransform:SetTranslateY(-1.0)
	splitter:SetScriptHandler("s_Callback")
	splitter:SetSize(0.0, 4.0)
	splitter:SetAnchorHor(0.0, 1.0)
	splitter:SetAnchorParamHor(0.0, 0.0)
	splitter:SetAnchorVer(0.0, 0.0)
	splitter:SetAnchorParamVer(400.0, 400.0)
	splitter:RegistToScriptSystem()
	
	return frame
end

-- UISlider
function s_CreateUISlider()
	local frame = UIFrame:New()
	frame.LocalTransform:SetTranslateY(-1.0)
	frame:SetAnchorHor(0.0, 1.0)
	frame:SetAnchorVer(0.0, 1.0)	
	
	local slider = UISlider:New()
	frame:AttachChild(slider)
	slider:SetName("Slider")
	slider.LocalTransform:SetTranslateY(-1.0)
	slider:SetScriptHandler("s_Callback")
	slider:RegistToScriptSystem()
	slider:CreateAddBackgroundPicBox()
	
	return frame
end

-- UIRound
function s_CreateUIRound()
	local frame = UIFrame:New()
	frame.LocalTransform:SetTranslateY(-1.0)
	frame:SetAnchorHor(0.0, 1.0)
	frame:SetAnchorVer(0.0, 1.0)

	local round = UIRound:New()
	frame:AttachChild(round)
	round.LocalTransform:SetTranslateY(-1.0)
	round:SetScriptHandler("s_Callback")
	round:RegistToScriptSystem()
	
	return frame
end

-- UIWebFrame
function s_CreateUIWebFrame()
	local frame = UIFrame:New()
	frame.LocalTransform:SetTranslateY(-1.0)
	frame:SetAnchorHor(0.0, 1.0)
	frame:SetAnchorVer(0.0, 1.0)
	
	local webFrame = UIWebFrame:New()
	frame:AttachChild(webFrame)
	webFrame:SetAnchorHor(0.0, 1.0)
	webFrame:SetAnchorVer(0.0, 1.0)
	webFrame:SetUpdateToTex(true)
	webFrame:LoadURL("http://www.manykit.com")
	
	return frame
end

-- Server
g_s_ClientConnector = nil
g_s_ClientMsgEditBox = nil
g_s_ButConnect = nil
g_s_ButSend = nil

function s_ServerCallback(objPtr, callType)
	local obj = Cast:ToO(objPtr)
	
	local name = obj:GetName()
	if UICT_PRESSED==callType then
	elseif UICT_RELEASED==callType then
		if nil ~= g_s_ClientConnector then
			if "ButConnect"==name then
				if g_s_ClientConnector:IsConnected() then
					g_s_ClientConnector:Disconnect()
				else
					g_s_ClientConnector:ConnectNB("127.0.0.1", 18180)
				end
			elseif "ButSend"==name then
				if g_s_ClientConnector:IsConnected() then
					local text = g_s_ClientMsgEditBox:GetText()
					g_s_ClientConnector:SendString(text)
				end
			end
		end
	end
end

-- server
function OnClientConnectorConnect(generalClientConnector)
    PX2_LOGGER:LogInfo("ClientConnector", "OnClientConnectorConnect ")
	g_s_ButConnect:CreateAddText("Connected!")
	g_s_ClientMsgEditBox:Enable(true)
	g_s_ButSend:Enable(true)
end

function OnClientConnectorDisConnect(generalClientConnector)
    PX2_LOGGER:LogInfo("ClientConnector", "OnClientConnectorDisConnect ")
	g_s_ButConnect:CreateAddText("ConnectToServer 127.0.0.1:18180")
	g_s_ClientMsgEditBox:Enable(false)
	g_s_ButSend:Enable(false)
end

function OnClientConnectorRecv(generalClientConnector, str)
    PX2_LOGGER:LogInfo("ClientConnector", "OnClientConnectorRecv "..":"..str)
	g_s_MsgList:AddItem(str)
end

function s_CreateUIServer()
	local frame = UIFrame:New()
	frame.LocalTransform:SetTranslateY(-1.0)
	frame:SetAnchorHor(0.0, 1.0)
	frame:SetAnchorVer(0.0, 1.0)

	local editbox = UIEditBox:New()
	frame:AttachChild(editbox)
	g_s_ClientMsgEditBox = editbox
	editbox:SetScriptHandler("s_ServerCallback")
	editbox:RegistToScriptSystem()
	editbox:Enable(false)

	local butConnect = UIButton:New()
	frame:AttachChild(butConnect)
	g_s_ButConnect = butConnect
	butConnect:SetName("ButConnect")
	butConnect:SetAnchorParamVer(100.0, 100.0)
	butConnect:SetSize(300, 40)
	butConnect:SetScriptHandler("s_ServerCallback")
	butConnect:CreateAddText("ConnectToServer 127.0.0.1:18180")

	local butSend = UIButton:New()
	frame:AttachChild(butSend)
	g_s_ButSend = butSend
	butSend:SetName("ButSend")
	butSend:SetAnchorParamVer(-100.0, -100.0)
	butSend:SetSize(300, 40)
	butSend:SetScriptHandler("s_ServerCallback")
	butSend:CreateAddText("SendMsg")
	butSend:Enable(false)

	-- connector
	local clientConnector = PX2_APP:CreateGetGeneralClientConnector("Default")
	g_s_ClientConnector = clientConnector
	clientConnector:AddOnConnectCallback("OnClientConnectorConnect")
	clientConnector:AddOnDisconnectCallback("OnClientConnectorDisConnect")
	clientConnector:AddOnRecvCallback("OnClientConnectorRecv")
	
	return frame
end