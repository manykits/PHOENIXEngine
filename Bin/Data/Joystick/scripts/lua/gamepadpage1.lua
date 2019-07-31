 -- gamepadpage1.lua

function rc_FramePadPage1()
    local frameAll = UIFrame:New()
    frameAll:SetAnchorHor(0.0, 1.0)
    frameAll:SetAnchorVer(0.0, 1.0)
    frameAll:SetAnchorParamHor(0.0, 0.0)
    frameAll:SetAnchorParamVer(0.0, 0.0)

    local frameSlider = UIFrame:New()
    frameAll:AttachChild(frameSlider)
    frameSlider:SetAnchorHor(0.0, 1.0)
    frameSlider:SetAnchorVer(1.0, 1.0)
    frameSlider:SetAnchorParamHor(0.0, 0.0)
    frameSlider:SetAnchorParamVer(-20.0, -20.0)
    frameSlider:SetPivot(0.5, 1.0)
    frameSlider:SetHeight(100)

    local txtPos = -45.0

    local sl0 = UISlider:New()
    rc_Slider0 = sl0
    frameSlider:AttachChild(sl0)
    sl0:GetButSlider():SetStateColorDefaultWhite()
    sl0:GetFPicBoxBack():GetUIPicBox():SetTexture("Data/Joystick/images/directctrl/slider0.png")
    sl0:GetFPicBoxBack():GetUIPicBox():SetPicBoxType(UIPicBox.PBT_NINE)
    sl0:GetFPicBoxBack():GetUIPicBox():SetTexCornerSize(8,30,8,30)        
    sl0:GetButSlider():GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/Joystick/images/directctrl/slider1.png")
    sl0.LocalTransform:SetTranslateY(-4.0)
    sl0:SetSliderLength(60)    
    sl0:SetHeight(60.0)
    sl0:SetAnchorHor(0.0, 0.25)
    sl0:SetAnchorParamHor(10.0, -10.0)
    sl0:SetAnchorVer(0.5, 0.5)
    local text = sl0:CreateAddText("slider1")
    text:GetText():SetFontColor(Float3.WHITE)
    text:SetAnchorParamVer(txtPos, txtPos)
    sl0:GetButSlider():SetSize(60,60)
    sl0:SetPercent(0.5)

    local sl1 = UISlider:New()
    rc_Slider1 = sl1
    frameSlider:AttachChild(sl1)
    sl1:GetButSlider():SetStateColorDefaultWhite()
    sl1:GetFPicBoxBack():GetUIPicBox():SetTexture("Data/Joystick/images/directctrl/slider0.png")
    sl1:GetFPicBoxBack():GetUIPicBox():SetPicBoxType(UIPicBox.PBT_NINE)
    sl1:GetFPicBoxBack():GetUIPicBox():SetTexCornerSize(8,30,8,30)        
    sl1:GetButSlider():GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/Joystick/images/directctrl/slider1.png")
    sl1.LocalTransform:SetTranslateY(-4.0)
    sl1:SetSliderLength(60)    
    sl1:SetHeight(60.0)
    sl1:SetAnchorHor(0.25, 0.5)
    sl1:SetAnchorParamHor(10.0, -10.0)
    sl1:SetAnchorVer(0.5, 0.5)
    local text = sl1:CreateAddText("slider2")
    text:GetText():SetFontColor(Float3.WHITE)
    text:SetAnchorParamVer(txtPos, txtPos)
    sl1:GetButSlider():SetSize(60,60)
    sl1:SetPercent(0.5)

    local sl2 = UISlider:New()
    rc_Slider2 = sl2
    frameSlider:AttachChild(sl2)
    sl2:GetButSlider():SetStateColorDefaultWhite()
    sl2:GetFPicBoxBack():GetUIPicBox():SetTexture("Data/Joystick/images/directctrl/slider0.png")
    sl2:GetFPicBoxBack():GetUIPicBox():SetPicBoxType(UIPicBox.PBT_NINE)
    sl2:GetFPicBoxBack():GetUIPicBox():SetTexCornerSize(8,30,8,30)        
    sl2:GetButSlider():GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/Joystick/images/directctrl/slider1.png")
    sl2.LocalTransform:SetTranslateY(-4.0)
    sl2:SetSliderLength(60)    
    sl2:SetHeight(60.0)
    sl2:SetAnchorHor(0.5, 0.75)
    sl2:SetAnchorParamHor(10.0, -10.0)
    sl2:SetAnchorVer(0.5, 0.5)
    local text = sl2:CreateAddText("slider3")
    text:GetText():SetFontColor(Float3.WHITE)
    text:SetAnchorParamVer(txtPos, txtPos)
    sl2:GetButSlider():SetSize(60,60)
    sl2:SetPercent(0.5)

    local sl3 = UISlider:New()
    rc_Slider3 = sl3
    frameSlider:AttachChild(sl3)
    sl3:GetButSlider():SetStateColorDefaultWhite()
    sl3:GetFPicBoxBack():GetUIPicBox():SetTexture("Data/Joystick/images/directctrl/slider0.png")
    sl3:GetFPicBoxBack():GetUIPicBox():SetPicBoxType(UIPicBox.PBT_NINE)
    sl3:GetFPicBoxBack():GetUIPicBox():SetTexCornerSize(8,30,8,30)        
    sl3:GetButSlider():GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/Joystick/images/directctrl/slider1.png")
    sl3.LocalTransform:SetTranslateY(-4.0)
    sl3:SetSliderLength(60)    
    sl3:SetHeight(60.0)
    sl3:SetAnchorHor(0.75, 1.0)
    sl3:SetAnchorParamHor(10.0, -10.0)
    sl3:SetAnchorVer(0.5, 0.5)
    local text = sl3:CreateAddText("slider4")
    text:GetText():SetFontColor(Float3.WHITE)
    text:SetAnchorParamVer(txtPos, txtPos)
    sl3:GetButSlider():SetSize(60,60)
    sl3:SetPercent(0.5)

    local round = UIRound:New("RoundCtrl")
    rc_RoundLeft = round
    frameAll:AttachChild(round)
    round.LocalTransform:SetTranslateY(-2.0)
    round:SetAnchorHor(0.0, 0.0)
    round:SetAnchorVer(0.0, 0.0)
    round:SetAnchorParamHor(450.0, 450.0)
    round:SetAnchorParamVer(130.0, 130.0)
    round:SetSize(120, 120)
    round:GetButton():SetStateColorDefaultWhite()
    round:GetButton():GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/Joystick/images/directctrl/round_in.png")
    round:GetButton():GetPicBoxAtState(UIButtonBase.BS_HOVERED):SetTexture("Data/Joystick/images/directctrl/round_in.png")
    round:GetButton():GetPicBoxAtState(UIButtonBase.BS_PRESSED):SetTexture("Data/Joystick/images/directctrl/round_in.png")
    round:GetBack():GetUIPicBox():SetTexture("Data/Joystick/images/directctrl/round_out.png")
    round:GetButton():SetSize(100, 100)
    round:RegistToScriptSystem()
    round:SetScriptHandler("rc_UIPadCtrlCallback")

    local round1 = UIRound:New("RoundCtrl1")
    rc_RoundRight = round1
    frameAll:AttachChild(round1)
    round1.LocalTransform:SetTranslateY(-2.0)
    round1:SetAnchorHor(1.0, 1.0)
    round1:SetAnchorVer(0.0, 0.0)
    round1:SetAnchorParamHor(-450.0, -450.0)
    round1:SetAnchorParamVer(130.0, 130.0)
    round1:SetSize(120, 120)
    round1:GetButton():SetStateColorDefaultWhite()
    round1:GetButton():GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/Joystick/images/directctrl/round_in.png")
    round1:GetButton():GetPicBoxAtState(UIButtonBase.BS_HOVERED):SetTexture("Data/Joystick/images/directctrl/round_in.png")
    round1:GetButton():GetPicBoxAtState(UIButtonBase.BS_PRESSED):SetTexture("Data/Joystick/images/directctrl/round_in.png")
    round1:GetBack():GetUIPicBox():SetTexture("Data/Joystick/images/directctrl/round_out.png")
    round1:GetButton():SetSize(100, 100)
    round1:RegistToScriptSystem()
    round1:SetScriptHandler("rc_UIPadCtrlCallback")

    local frameBtnsLeft = createFrameBtns(true)
    frameAll:AttachChild(frameBtnsLeft)
    frameBtnsLeft:SetAnchorHor(0.0, 0.0)
    frameBtnsLeft:SetAnchorVer(0.5, 0.5)
    frameBtnsLeft:SetSize(200.0, 200.0)
    frameBtnsLeft:SetAnchorParamHor(230.0, 230.0)

    local frameBtnsRight = createFrameBtns(false)
    frameAll:AttachChild(frameBtnsRight)
    frameBtnsRight:SetAnchorHor(1.0, 1.0)
    frameBtnsRight:SetAnchorVer(0.5, 0.5)
    frameBtnsRight:SetSize(200.0, 200.0)
    frameBtnsRight:SetAnchorParamHor(-230.0, -230.0)

    return frameAll
end

function createFrameBtns(isLeft)
    local frame = UIFrame:New()
    frame:SetChildPickOnlyInSizeRange(false)

    local btnSize = 100.0

    local btn0 = UIButton:New("btn0")
    frame:AttachChild(btn0)
    btn0.LocalTransform:SetTranslateY(-2.0)
    btn0:SetAnchorHor(0.0, 0.0)
    btn0:SetAnchorVer(0.5, 0.5)
    btn0:SetStateColorDefaultWhite()
    btn0:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/Joystick/images/directctrl/round.png")
    btn0:SetSize(btnSize, btnSize)
    btn0:SetScriptHandler("rc_UIPadCtrlCallback")
    if isLeft then
        rc_BtnLeft0 = btn0
        btn0:CreateAddText(""..PX2_LM_APP:GetValue("Left"))
    else
        rc_BtnRight0 = btn0
        btn0:CreateAddText("3")
    end

    local btn1 = UIButton:New("btn1")
    frame:AttachChild(btn1)
    btn1.LocalTransform:SetTranslateY(-2.0)
    btn1:SetAnchorHor(1.0, 1.0)
    btn1:SetAnchorVer(0.5, 0.5)
    btn1:SetStateColorDefaultWhite()
    btn1:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/Joystick/images/directctrl/round.png")
    btn1:SetSize(btnSize, btnSize)
    btn1:SetScriptHandler("rc_UIPadCtrlCallback")
    if isLeft then
        rc_BtnLeft1 = btn1
        btn1:CreateAddText(""..PX2_LM_APP:GetValue("Right"))
    else
        rc_BtnRight1 = btn1
        btn1:CreateAddText("4")
    end

    local btn2 = UIButton:New("btn2")
    frame:AttachChild(btn2)
    btn2.LocalTransform:SetTranslateY(-2.0)
    btn2:SetAnchorHor(0.5, 0.5)
    btn2:SetAnchorVer(0.0, 0.0)
    btn2:SetStateColorDefaultWhite()
    btn2:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/Joystick/images/directctrl/round.png")
    btn2:SetSize(btnSize, btnSize)
    btn2:SetScriptHandler("rc_UIPadCtrlCallback")
    if isLeft then
        rc_BtnLeft2 = btn2
        btn2:CreateAddText(""..PX2_LM_APP:GetValue("Down"))
    else
        rc_BtnRight2 = btn2
        btn2:CreateAddText("2")
    end

    local btn3 = UIButton:New("btn3")
    frame:AttachChild(btn3)
    btn3.LocalTransform:SetTranslateY(-2.0)
    btn3:SetAnchorHor(0.5, 0.5)
    btn3:SetAnchorVer(1.0, 1.0)
    btn3:SetStateColorDefaultWhite()
    btn3:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/Joystick/images/directctrl/round.png")
    btn3:SetSize(btnSize, btnSize)
    btn3:SetScriptHandler("rc_UIPadCtrlCallback")
    if isLeft then
        rc_BtnLeft3 = btn3
        btn3:CreateAddText(""..PX2_LM_APP:GetValue("Up"))
    else
        rc_BtnRight3 = btn3
        btn3:CreateAddText("1")
    end

    return frame
end

function rc_UIPadCtrlCallback(ptr,callType)
    local obj=Cast:ToO(ptr)
    local name=obj:GetName()
    if UICT_PRESSED==callType then
        PX2_GR:PlayScale(obj)

    elseif UICT_RELEASED ==callType then
        PX2_GR:PlayNormal(obj)        

    elseif UICT_RELEASED_NOTPICK == callType then
        PX2_GR:PlayNormal(obj)

    elseif UICT_ROUND_DRAG == callType then
        if "RoundCtrl" == name then      
        end
    elseif UICT_ROUND_RELEASED == callType then
        if "RoundCtrl" == name then      

        end
    elseif UICT_ROUND_RELEASEDNOTPICK == callType then
        if "RoundCtrl" == name then      

        end
    elseif UICT_SLIDERCHANGED == callType then

    elseif UICT_CHECKED == callType then

    elseif UICT_DISCHECKED == callType then

    end
end
