-- ZERONESetting.lua

function zo_SettingFrame()
    -- setting frame
    local frame = UIFrame:New()
    frame:SetAnchorHor(0.0, 1.0)
	frame:SetAnchorVer(0.0, 1.0)

	local btnRight = UIButton:New("BtnOK")
	frame:AttachChild(btnRight)
	btnRight.LocalTransform:SetTranslateY(-1.0)
	btnRight:SetSize(150, 50)
	btnRight:SetAnchorHor(0.5, 0.5)
	btnRight:SetAnchorVer(0.0, 0.0)
	btnRight:SetAnchorParamVer(60.0, 60.0)
	btnRight:SetSize(180, 60)
	btnRight:SetStateColor(UIButtonBase.BS_HOVERED, Float3:MakeColor(255, 255, 255))
	btnRight:SetStateColor(UIButtonBase.BS_NORMAL, Float3:MakeColor(255, 255, 255))
	btnRight:SetStateColor(UIButtonBase.BS_PRESSED, Float3:MakeColor(190, 190, 190))
	btnRight:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/ZERONE/images/btnblue.png")
	btnRight:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetPicBoxType(UIPicBox.PBT_NINE)
	local fText = btnRight:CreateAddText(""..PX2_LM_APP:GetValue("OK"))
    zo_UISetTextFont(fText:GetText(), 32)
    btnRight:SetScriptHandler("zo_SettingCallabck")

    local posVer = -100.0
	local textRobotName = UIFText:New()
	frame:AttachChild(textRobotName)
	textRobotName.LocalTransform:SetTranslateY(-1.0)
	textRobotName:SetAnchorHor(0.0, 0.0)
	textRobotName:SetAnchorParamHor(200.0, 200.0)
	textRobotName:SetAnchorVer(1.0, 1.0)
	textRobotName:SetAnchorParamVer(posVer, posVer)
	textRobotName:SetPivot(1.0, 0.5)
	textRobotName:SetSize(128, 80)
	textRobotName:GetText():SetText(""..PX2_LM_APP:GetValue("Name"))
	textRobotName:GetText():SetAligns(TEXTALIGN_RIGHT + TEXTALIGN_VCENTER)
	zo_UISetTextFont(textRobotName:GetText(), 32)

    local uiEditBoxName = UIEditBox:New("EditBoxRobotName")
    ZERONE_EditBoxName = uiEditBoxName
	frame:AttachChild(uiEditBoxName)
	uiEditBoxName.LocalTransform:SetTranslateY(-1.0)
	uiEditBoxName:SetAnchorHor(0.0, 1.0)
	uiEditBoxName:SetAnchorParamHor(230.0, -100.0)
	uiEditBoxName:SetAnchorVer(1.0, 1.0)
	uiEditBoxName:SetAnchorParamVer(posVer, posVer)
	uiEditBoxName:SetPivot(0.0, 0.5)
	uiEditBoxName:SetHeight(60.0)
	uiEditBoxName:GetBackPicBox():GetUIPicBox():SetTexture("Data/ZERONE/images/editorback.png")
	uiEditBoxName:GetBackPicBox():GetUIPicBox():SetPicBoxType(UIPicBox.PBT_NINE)
	uiEditBoxName:GetBackPicBox():GetUIPicBox():SetTexCornerSize(6.0, 6.0, 6.0, 6.0)

	posVer = posVer -100.0
	
	local textID = UIFText:New()
	frame:AttachChild(textID)
	textID.LocalTransform:SetTranslateY(-1.0)
	textID:SetAnchorHor(0.0, 0.0)
	textID:SetAnchorParamHor(200.0, 200.0)
	textID:SetAnchorVer(1.0, 1.0)
	textID:SetAnchorParamVer(posVer, posVer)
	textID:SetPivot(1.0, 0.5)
	textID:SetSize(128, 80)
	textID:GetText():SetText("ID")
	textID:GetText():SetAligns(TEXTALIGN_RIGHT + TEXTALIGN_VCENTER)
	zo_UISetTextFont(textID:GetText(), 32)

    local uiEditBoxID = UIEditBox:New("EditBoxRobotID")
    ZERONE_EditBoxID = uiEditBoxID
	frame:AttachChild(uiEditBoxID)
	uiEditBoxID.LocalTransform:SetTranslateY(-1.0)
	uiEditBoxID:SetAnchorHor(0.0, 1.0)
	uiEditBoxID:SetAnchorParamHor(230.0, -100.0)
	uiEditBoxID:SetAnchorVer(1.0, 1.0)
	uiEditBoxID:SetAnchorParamVer(posVer, posVer)
	uiEditBoxID:SetPivot(0.0, 0.5)
	uiEditBoxID:SetHeight(60.0)
	uiEditBoxID:GetBackPicBox():GetUIPicBox():SetTexture("Data/ZERONE/images/editorback.png")
	uiEditBoxID:GetBackPicBox():GetUIPicBox():SetPicBoxType(UIPicBox.PBT_NINE)
	uiEditBoxID:GetBackPicBox():GetUIPicBox():SetTexCornerSize(6.0, 6.0, 6.0, 6.0)

	ZERONE_EditBoxName:SetText("".. ZERONE_UIFaceCtrlScript:GetName())
	ZERONE_EditBoxID:SetText("".. ZERONE_UIFaceCtrlScript:GetID())

    return frame
end

function zo_SettingCallabck(ptr, callType)
    local obj = Cast:ToO(ptr) 
	local name = obj:GetName()	

    if UICT_PRESSED==callType then
        playFrameScale(obj)
	elseif UICT_RELEASED==callType then
        playFrameNormal(obj)
        
        if "BtnOK"==name then
			ZERONE_UIFaceCtrlScript:SetName(ZERONE_EditBoxName:GetText())
			ZERONE_UIFaceCtrlScript:SetID(ZERONE_EditBoxID:GetText())
		end
    elseif UICT_RELEASED_NOTPICK==callType then
        playFrameNormal(obj)
    elseif UICT_ROUND_RELEASEDNOTPICK==callType then
    elseif UICT_CHECKED==callType then
    elseif UICT_DISCHECKED==callType then
	end
end