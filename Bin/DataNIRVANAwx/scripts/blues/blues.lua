-- blues.lua

--require("Data/BlueBlock/scripts/lua/start.lua")
--require("Data/BluePrint/scripts/lua/start.lua")

-- blueblock
function n_CreateFrame_BBs()
	local logicCanvas = PX2EU_MAN:GetLogicCanvas()
	if nil~= logicCanvas then
		local frame = UIFrame:New()
		logicCanvas:AttachChild(frame)
		frame.LocalTransform:SetTranslateY(-1.0)
		frame:SetAnchorHor(0.0, 1.0)
		frame:SetAnchorVer(0.0, 1.0)

		local tabFrame = UITabFrame:New()
		frame:AttachChild(tabFrame)
		tabFrame:SetLayoutPos(UITabFrame.LPT_BOTTOM)
		tabFrame:SetAnchorHor(0.0, 1.0)
		tabFrame:SetAnchorVer(0.0, 1.0)
		tabFrame:SetTabBarHeight(25)
		tabFrame:SetTabHeight(25)
		tabFrame:SetFontColor(Float3.WHITE)

		tabFrame:AddTab("Mission", PX2_LM_EDITOR:GetValue("Mission"), UIFrame:New())
		tabFrame:AddTab("Behaviour", PX2_LM_EDITOR:GetValue("Behaviour"), UIFrame:New())
		if nil~=bb_IsUseBlueBlock then
			tabFrame:AddTab("BlueBlock", PX2_LM_EDITOR:GetValue("BlueBlock"), n_CreateFrame_BlueBlock())
		end
		if nil~=bb_IsUseBluePrint then
			tabFrame:AddTab("BluePrint", PX2_LM_EDITOR:GetValue("BluePrint"), n_CreateFrame_BluePrint())
		end
		tabFrame:AddTab("Scratch", PX2_LM_EDITOR:GetValue("Scratch"), UIFrame:New())
		tabFrame:SetActiveTab("Mission")

		n_g_FrameLogicTableFrame = tabFrame
	end
end

n_g_FrameBlueBlockFrame = nil
n_g_FrameBlueBlockCodeEditor = nil

function n_CreateFrame_BlueBlock()
	b_Start()
	
	local frame = UIFrame:New()
	n_g_FrameBlueBlockFrame = frame
	frame:SetAnchorHor(0.0, 1.0)
	frame:SetAnchorVer(0.0, 1.0)

	-- block editor
	local frameBlockEditor = UIFrame:New()
	n_g_FrameBlueBlockCodeEditor = frameBlockEditor
	frame:AttachChild(frameBlockEditor)
	frameBlockEditor:SetAnchorHor(0.0, 1.0)
	frameBlockEditor:SetAnchorVer(0.0, 1.0)

	local leftSize = 320.0
	local leftCataheight = 160.0
	local cataFrme = UIBBlockCataFrame:New()
	frameBlockEditor:AttachChild(cataFrme)
	cataFrme:SetCataType(UIBBlockCataFrame.CT_NORMAL)
	cataFrme:CreateCataButs()
	cataFrme:SetPivot(0.0, 1.0)
	cataFrme:SetAnchorHor(0.0, 0.0)
	cataFrme:SetAnchorVer(1.0, 1.0)
	cataFrme:SetAnchorParamVer(0.0, 0.0)
	cataFrme:SetSize(leftSize, leftCataheight)

	local butShowCode = UIButton:New("ButShowCode")
	cataFrme:AttachChild(butShowCode)
	butShowCode.LocalTransform:SetTranslateY(-5.0)
	butShowCode:SetAnchorHor(0.0, 1.0)
	butShowCode:SetAnchorVer(0.0, 0.0)
	butShowCode:SetSize(0.0, 20.0)
	butShowCode:SetPivot(0.5, 0.0)
	local showCodeText = PX2_LM_EDITOR:GetValue("ShowCode")
	butShowCode:CreateAddText(showCodeText)
	butShowCode:SetScriptHandler("n_BlueBlock_Callback")

	local editFrame = UIBBlockEdit:New()
	frameBlockEditor:AttachChild(editFrame)
	editFrame:SetAnchorHor(0.0, 1.0)
	editFrame:SetAnchorVer(0.0, 1.0)
	editFrame:SetAnchorParamHor(leftSize, 0.0)

	local toolFrame = UIBBlockEditToolFrame:New()
	frameBlockEditor:AttachChild(toolFrame)
	toolFrame.LocalTransform:SetTranslateY(-15.0)
	toolFrame:SetAnchorHor(0.0, 1.0)
	toolFrame:SetAnchorVer(1.0, 1.0)
	toolFrame:SetSize(0.0, 40.0)
	toolFrame:SetPivot(0.5, 1.0)
	
	local blocksCanvas = Canvas:New()
	frameBlockEditor:AttachChild(blocksCanvas)
	blocksCanvas.LocalTransform:SetTranslateY(-2.0)
	blocksCanvas:CreateUICameraNode()
	blocksCanvas:SetWidth(leftSize)
	blocksCanvas:SetAnchorHor(0.0, 0.0)
	blocksCanvas:SetAnchorParamHor(0.0, 0.0)
	blocksCanvas:SetAnchorVer(0.0, 1.0)
	blocksCanvas:SetAnchorParamVer(0.0, -leftCataheight)
	blocksCanvas:SetPivot(0.0, 0.5)
	blocksCanvas:SetClearColor(Float4:MakeColor(80, 80, 80, 255))

	local blocksFrame = UIBBlockPanels:New()
	blocksCanvas:AttachChild(blocksFrame)
	blocksFrame.LocalTransform:SetTranslateY(-2.0)
	blocksFrame:SetAnchorHor(0.0, 1.0)
	blocksFrame:SetAnchorVer(0.0, 1.0)

	return frame
end

function n_BlueBlock_Callback(objPtr, callType)
	local obj = Cast:ToO(objPtr)
	local name = obj:GetName()
	if UICT_RELEASED==callType then
		if "ButShowCode" == name then

		end
	elseif UICT_SPLITTER_DRAGING==callType then
		local splitter = obj
		
		local spHalfWidth = splitter:GetWidth() * 0.5
		local blueBlockWidth = n_g_FrameBlueBlockFrame:GetWidth()

		local left = splitter:GetUserDataPointer("Left")
		local leftFrame = Cast:ToO(left)
		local right = splitter:GetUserDataPointer("Right")
		local rightFrame = Cast:ToO(right)
		local anchorParamHor = splitter:GetAnchorParamHor():X()
		
		leftFrame:SetAnchorHor(0.0, 0.0)
		leftFrame:SetPivot(0.0, 0.5)
		leftFrame:SetSize(anchorParamHor-spHalfWidth, 0.0)

		rightFrame:SetAnchorHor(0.0, 1.0)
		rightFrame:SetAnchorParamHor(anchorParamHor+spHalfWidth, 0.0)
	end
end

-- BlurPrint
n_g_FrameBluePrintFrame = nil
function n_CreateFrame_BluePrint()
	local frame = UIFrame:New()
	n_g_FrameBluePrintFrame = frame
	frame:SetAnchorHor(0.0, 1.0)
	frame:SetAnchorVer(0.0, 1.0)

	local printFrame = UIBluePrintFrame:New()
	frame:AttachChild(printFrame)
	printFrame.LocalTransform:SetTranslateY(-1.0)
	printFrame:SetAnchorHor(0.0, 1.0)
	printFrame:SetAnchorVer(0.0, 1.0)

	return frame
end

-- blueprint
function n_CreateBPPackage()
	PX2_EDIT:CreateBPPackage()
end

function n_CreateBPFile()
	PX2_EDIT:CreateBPFile()
end

function n_CreateBPModule(className, funName)
	PX2_EDIT:CreateBPModule(className, funName)
end

function n_CreateBPEvent(spaceName, eventName)
	PX2_EDIT:CreateBPEvent(spaceName, eventName)
end

function n_CreateBPOption(optionName)
	PX2_EDIT:CreateBPOption(optionName)
end

function n_CreateBPOperator(operatorName)
	PX2_EDIT:CreateBPOperator(operatorName)
end

function n_CreateBPParam(paramName)
	PX2_EDIT:CreateBPParam(paramName)
end

function n_BPFileCompile()
	PX2_EDIT:CompileBP()
end

function n_DisconnectParam()	
	PX2_EDIT:DisconnectParam()
end