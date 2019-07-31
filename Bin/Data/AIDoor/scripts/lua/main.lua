-- main.lua

AIDoor_MainUI = nil
AIDoor_VisitUI = nil
function ui_Main()
    local ui = PX2_PROJ:GetUI()
    
    local frameAll = UIFrame:New()
    ui:AttachChild(frameAll)
    AIDoor_MainUI = frameAll
    frameAll:SetAnchorHor(0.0, 1.0)
    frameAll:SetAnchorVer(0.0, 1.0)

    local butVisit = UIButton:New("BtnVisit")
    frameAll:AttachChild(butVisit)
    butVisit.LocalTransform:SetTranslateY(-2.0)
    butVisit:SetSize(300.0, 300.0)
    butVisit:SetAnchorHor(0.5,0.5)
    butVisit:SetAnchorVer(0.5,0.5)
    butVisit:SetAnchorParamHor(-200.0, -200.0)
    butVisit:SetAnchorParamVer(0, 0)
    butVisit:SetScriptHandler("aidoor_UICallback")
    butVisit:CreateAddText("拜访")

    local butCompany = UIButton:New("BtnCompany")
    frameAll:AttachChild(butCompany)
    butCompany.LocalTransform:SetTranslateY(-2.0)
    butCompany:SetSize(300.0, 300.0)
    butCompany:SetAnchorHor(0.5,0.5)
    butCompany:SetAnchorVer(0.5,0.5)
    butCompany:SetAnchorParamHor(200.0, 200.0)
    butCompany:SetAnchorParamVer(0, 0)
    butCompany:SetScriptHandler("aidoor_UICallback")
	butCompany:CreateAddText("企业宣传")

    ui_Visit()
end

function aidoor_UICallback(ptr,callType)
    local obj=Cast:ToO(ptr)
    local name=obj:GetName()
    if UICT_PRESSED==callType then
        PX2_GR:PlayScale(obj)
    elseif UICT_RELEASED ==callType then
        PX2_GR:PlayNormal(obj)
        if "BtnVisit" == name then
            AIDoor_VisitUI:Show(true)
        elseif "BtnCompany" == name then
            ui_Company()
        elseif "DlgVisitClose"==name then
            AIDoor_VisitUI:Show(false)
        end
    elseif UICT_RELEASED_NOTPICK == callType then
        PX2_GR:PlayNormal(obj)
    elseif UICT_CHECKED == callType then
    elseif UICT_DISCHECKED == callType then
    end
end

function ui_Visit()
    --AIDoor_MainUI:DetachAllChildren()

    local uiFrameBack, uiFrame, btnClose, titleText = ad_UICreateDlgCommon(478.0, 324.0, "")
    uiFrame:SetAnchorHor(0.0, 1.0)
	uiFrame:SetAnchorParamHor(80.0, -80.0)
	uiFrame:SetAnchorVer(0.0, 1.0)
    uiFrame:SetAnchorParamVer(80.0, -80.0)
    btnClose:SetName("DlgVisitClose")
    btnClose:SetScriptHandler("aidoor_UICallback")

    -- begin content
    local posVer = -100.0
	local visitNameText = UIFText:New()
	uiFrame:AttachChild(visitNameText)
	visitNameText.LocalTransform:SetTranslateY(-1.0)
	visitNameText:SetAnchorHor(0.0, 0.0)
	visitNameText:SetAnchorParamHor(200.0, 200.0)
	visitNameText:SetAnchorVer(1.0, 1.0)
	visitNameText:SetAnchorParamVer(posVer, posVer)
	visitNameText:SetPivot(1.0, 0.5)
	visitNameText:SetSize(228, 80)
	visitNameText:GetText():SetText("拜访对象")
	visitNameText:GetText():SetAligns(TEXTALIGN_RIGHT + TEXTALIGN_VCENTER)
	ad_UISetTextFont(visitNameText:GetText(), 32)

    local editBoxPeopleNameToVisit = UIEditBox:New("PeopleNameToVisit")
	uiFrame:AttachChild(editBoxPeopleNameToVisit)
	editBoxPeopleNameToVisit.LocalTransform:SetTranslateY(-1.0)
	editBoxPeopleNameToVisit:SetAnchorHor(0.0, 1.0)
	editBoxPeopleNameToVisit:SetAnchorParamHor(230.0, -100.0)
	editBoxPeopleNameToVisit:SetAnchorVer(1.0, 1.0)
	editBoxPeopleNameToVisit:SetAnchorParamVer(posVer, posVer)
	editBoxPeopleNameToVisit:SetPivot(0.0, 0.5)
	editBoxPeopleNameToVisit:SetHeight(60.0)
	editBoxPeopleNameToVisit:GetBackPicBox():GetUIPicBox():SetTexture("Data/AIDoor/images/editorback.png")
	editBoxPeopleNameToVisit:GetBackPicBox():GetUIPicBox():SetPicBoxType(UIPicBox.PBT_NINE)
	editBoxPeopleNameToVisit:GetBackPicBox():GetUIPicBox():SetTexCornerSize(6.0, 6.0, 6.0, 6.0)

	posVer = posVer -100.0
	
	local textPeopleVisiting = UIFText:New()
	uiFrame:AttachChild(textPeopleVisiting)
	textPeopleVisiting.LocalTransform:SetTranslateY(-1.0)
	textPeopleVisiting:SetAnchorHor(0.0, 0.0)
	textPeopleVisiting:SetAnchorParamHor(200.0, 200.0)
	textPeopleVisiting:SetAnchorVer(1.0, 1.0)
	textPeopleVisiting:SetAnchorParamVer(posVer, posVer)
	textPeopleVisiting:SetPivot(1.0, 0.5)
	textPeopleVisiting:SetSize(128, 80)
	textPeopleVisiting:GetText():SetText("输入你的姓名")
	textPeopleVisiting:GetText():SetAligns(TEXTALIGN_RIGHT + TEXTALIGN_VCENTER)
	ad_UISetTextFont(textPeopleVisiting:GetText(), 32)

    local editBoxPeopleVisiting = UIEditBox:New("PeopleVisiting")
	uiFrame:AttachChild(editBoxPeopleVisiting)
	editBoxPeopleVisiting.LocalTransform:SetTranslateY(-1.0)
	editBoxPeopleVisiting:SetAnchorHor(0.0, 1.0)
	editBoxPeopleVisiting:SetAnchorParamHor(230.0, -100.0)
	editBoxPeopleVisiting:SetAnchorVer(1.0, 1.0)
	editBoxPeopleVisiting:SetAnchorParamVer(posVer, posVer)
	editBoxPeopleVisiting:SetPivot(0.0, 0.5)
	editBoxPeopleVisiting:SetHeight(60.0)
	editBoxPeopleVisiting:GetBackPicBox():GetUIPicBox():SetTexture("Data/AIDoor/images/editorback.png")
	editBoxPeopleVisiting:GetBackPicBox():GetUIPicBox():SetPicBoxType(UIPicBox.PBT_NINE)
    editBoxPeopleVisiting:GetBackPicBox():GetUIPicBox():SetTexCornerSize(6.0, 6.0, 6.0, 6.0)

    posVer = posVer -100.0
	
	local textReason = UIFText:New()
	uiFrame:AttachChild(textReason)
	textReason.LocalTransform:SetTranslateY(-1.0)
	textReason:SetAnchorHor(0.0, 0.0)
	textReason:SetAnchorParamHor(200.0, 200.0)
	textReason:SetAnchorVer(1.0, 1.0)
	textReason:SetAnchorParamVer(posVer, posVer)
	textReason:SetPivot(1.0, 0.5)
	textReason:SetSize(128, 80)
	textReason:GetText():SetText("输入拜访事由")
	textReason:GetText():SetAligns(TEXTALIGN_RIGHT + TEXTALIGN_VCENTER)
	ad_UISetTextFont(textReason:GetText(), 32)

    local peopleVisiting = UIEditBox:New("PeopleVisiting")
	uiFrame:AttachChild(peopleVisiting)
	peopleVisiting.LocalTransform:SetTranslateY(-1.0)
	peopleVisiting:SetAnchorHor(0.0, 1.0)
	peopleVisiting:SetAnchorParamHor(230.0, -100.0)
	peopleVisiting:SetAnchorVer(1.0, 1.0)
	peopleVisiting:SetAnchorParamVer(posVer, posVer)
	peopleVisiting:SetPivot(0.0, 0.5)
	peopleVisiting:SetHeight(60.0)
	peopleVisiting:SetText("普通拜访")
	peopleVisiting:GetBackPicBox():GetUIPicBox():SetTexture("Data/AIDoor/images/editorback.png")
	peopleVisiting:GetBackPicBox():GetUIPicBox():SetPicBoxType(UIPicBox.PBT_NINE)
    peopleVisiting:GetBackPicBox():GetUIPicBox():SetTexCornerSize(6.0, 6.0, 6.0, 6.0)
    
    -- call button
	local btnVisitCall = UIButton:New("BtnVisitCall")
	uiFrame:AttachChild(btnVisitCall)
	btnVisitCall.LocalTransform:SetTranslateY(-1.0)
	btnVisitCall:SetSize(150, 50)
	btnVisitCall:SetAnchorHor(0.5, 0.5)
	btnVisitCall:SetAnchorVer(0.0, 0.0)
	btnVisitCall:SetAnchorParamVer(80.0, 80.0)
	btnVisitCall:SetSize(200, 100)
	btnVisitCall:SetStateColor(UIButtonBase.BS_HOVERED, Float3:MakeColor(255, 255, 255))
	btnVisitCall:SetStateColor(UIButtonBase.BS_NORMAL, Float3:MakeColor(255, 255, 255))
	btnVisitCall:SetStateColor(UIButtonBase.BS_PRESSED, Float3:MakeColor(190, 190, 190))
	btnVisitCall:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/AIDoor/images/btnblue.png")
	btnVisitCall:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetPicBoxType(UIPicBox.PBT_NINE)
	local fText = btnVisitCall:CreateAddText("呼叫")
    ad_UISetTextFont(fText:GetText(), 32)
	btnVisitCall:SetScriptHandler("aidoor_UICallback")
	
	-- call phone text
	local phoneToCall = UIFText:New()
	uiFrame:AttachChild(phoneToCall)
	phoneToCall.LocalTransform:SetTranslateY(-1.0)
	phoneToCall:SetAnchorHor(0.0, 0.0)
	phoneToCall:SetAnchorParamHor(65.0, 65.0)
	phoneToCall:SetAnchorVer(1.0, 1.0)
	phoneToCall:SetAnchorParamVer(-40.0, -40.0)
	phoneToCall:SetPivot(0.0, 0.5)
	phoneToCall:SetSize(400, 80)
	phoneToCall:GetText():SetText("拜访呼叫号码:15361481809")
	phoneToCall:GetText():SetFontColor(Float3.RED)
	ad_UISetTextFont(phoneToCall:GetText(), 32)

    -- end content

    AIDoor_VisitUI = uiFrameBack
    AIDoor_VisitUI:Show(false)

    AIDoor_MainUI:AttachChild(uiFrameBack)
    uiFrameBack.LocalTransform:SetTranslateY(-5.0)
end

function  ui_Company()

end