-- ZERONEUIPadCtrl.lua

local ZERONE_SpeedAdjustGo = 1.0
local ZERONE_SpeedAdjustTurn = 1.0
local ZERONE_LastLeftType = 0
local ZERONE_LastLeftSpeed = 0.0
local ZERONE_LastRightType = 0
local ZERONE_LastRightSpeed = 0.0

ZERONE_FrameUICtrl = nil
local ZERONE_SimpleCtr_UIMode = 0
local ZERONE_FrameCtrlMode0 = nil
local ZERONE_FrameCtrlMode1 = nil

local ZERONE_IsPressedLeft0 = false
local ZERONE_IsPressedLeft1 = false
local ZERONE_IsPressedRight0 = false
local ZERONE_IsPressedRight1 = false

local ZERONE_SpeedAdjustGo = 1.0
local ZERONE_SpeedAdjustTurn = 1.0
local ZERONE_LastLeftType = 0
local ZERONE_LastLeftSpeed = 0.0
local ZERONE_LastRightType = 0
local ZERONE_LastRightSpeed = 0.0

function zo_PadCtrl()
    -- padctrl
    local frameAll = UIFrame:New()
    frameAll:SetAnchorHor(0.0, 1.0)
    frameAll:SetAnchorVer(0.0, 1.0)
    frameAll:SetAnchorParamHor(25.0, -25.0)
    frameAll:SetAnchorParamVer(25.0, -25.0)
    local picBox = frameAll:CreateAddBackgroundPicBox()
    picBox:SetColor(Float3:MakeColor(50, 50, 50))
    ZERONE_FrameUICtrl = frameAll

    local fVal1 = 0.5
    local fVal2 = 0.5
    local fVal3 = 0.5

    local strlCfg1 = PX2_PROJ:GetConfig("Slider1")
    if ""~=strlCfg1 then
        fVal1 = 0.0 + strlCfg1
    end
    
    local strlCfg2 = PX2_PROJ:GetConfig("Slider2")
    if ""~=strlCfg2 then
        fVal2 = 0.0 + strlCfg2
    end

    local strlCfg3 = PX2_PROJ:GetConfig("Slider3")
    if ""~=strlCfg3 then
        fVal3 = 0.0 + strlCfg3
    end

    local posVer = 80.0
    for i=1, 3 do
        local name = "slider"..i
        local slider=UISlider:New(name)
        frameAll:AttachChild(slider)
        slider.LocalTransform:SetTranslateY(-2.0)
        slider:GetButSlider():SetStateColorDefaultWhite()
        slider:GetFPicBoxBack():GetUIPicBox():SetPicBoxType(UIPicBox.PBT_NINE)
        slider:GetFPicBoxBack():GetUIPicBox():SetTexCornerSize(12,12,12,12)        
        slider:GetFPicBoxBack():GetUIPicBox():SetTexture("Data/ZERONE/images/directctrl/slider0.png")
        slider:GetButSlider():GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/ZERONE/images/directctrl/slider1.png")
        slider:GetButSlider():SetSize(60,60)
        slider:SetSliderLength(60)    
        slider:SetAnchorHor(0.5, 0.5)
        slider:SetAnchorParamHor(0.0, 0.0)
        slider:SetAnchorVer(0.0, 0.0)
        slider:SetAnchorParamVer(posVer, posVer)
        slider:SetHeight(60.0)
        slider:SetWidth(300.0)
        slider:SetPercent(1.0)
        slider:SetScriptHandler("zo_UIPadCtrlCallback")
        slider:RegistToScriptSystem()

        if 1==i then
            slider:SetPercent(fVal1, true)
        elseif 2==i then
            slider:SetPercent(fVal2, true)
        elseif 3==i then
            slider:SetPercent(fVal3, true)
        end

        posVer = posVer + 80.0
    end

    local butClose = UIButton:New("BtnClose")
    frameAll:AttachChild(butClose)
    butClose.LocalTransform:SetTranslateY(-2.0)
    butClose:SetSize(50.0, 50.0)
    butClose:SetAnchorHor(1.0,1.0)
    butClose:SetAnchorVer(1.0,1.0)
    butClose:SetAnchorParamHor(-35, -35)
    butClose:SetAnchorParamVer(-35, 35)
    butClose:SetScriptHandler("zo_UIPadCtrlCallback")
    butClose:CreateAddText("X")

    local butUIMode = UIButton:New("ButUIMode")
	frameAll:AttachChild(butUIMode)
	butUIMode.LocalTransform:SetTranslateY(-1.0)
	butUIMode:SetAnchorHor(0.0, 0.0)
	butUIMode:SetAnchorParamHor(60.0, 30.0)
    butUIMode:SetAnchorVer(1.0, 1.0)
    butUIMode:SetAnchorParamVer(-30.0, -30.0)
    butUIMode:SetPivot(0.5, 0.5)
    butUIMode:CreateAddText(""..PX2_LM_APP:GetValue("UIMode"))
	butUIMode:SetSize(100, 40)
    butUIMode:SetScriptHandler("zo_UIPadCtrlCallback")

    local ledCheck_Sound = UICheckButton:New("CheckButton_Sound")
    frameAll:AttachChild(ledCheck_Sound)
	ledCheck_Sound.LocalTransform:SetTranslateY(-1.0)
	ledCheck_Sound:SetAnchorHor(0.0, 0.0)
	ledCheck_Sound:SetAnchorParamHor(120.0, 120.0)
    ledCheck_Sound:SetAnchorVer(1.0, 1.0)
    ledCheck_Sound:SetAnchorParamVer(-140.0, -140.0)
    ledCheck_Sound:SetPivot(0.5, 0.5)
    ledCheck_Sound:SetSize(80, 80)
    local fText = ledCheck_Sound:CreateAddText("Sound")
    fText:GetText():SetFontColor(Float3.BLACK)
    ledCheck_Sound:SetScriptHandler("zo_UIPadCtrlCallback")
    ledCheck_Sound:Check(false, false)

    local ledCheck = UICheckButton:New("CheckButton_LED_L")
    frameAll:AttachChild(ledCheck)
	ledCheck.LocalTransform:SetTranslateY(-1.0)
	ledCheck:SetAnchorHor(0.0, 0.0)
	ledCheck:SetAnchorParamHor(220.0, 220.0)
    ledCheck:SetAnchorVer(1.0, 1.0)
    ledCheck:SetAnchorParamVer(-140.0, -140.0)
    ledCheck:SetPivot(0.5, 0.5)
    ledCheck:SetSize(80, 80)
    local fText = ledCheck:CreateAddText("LED_L")
    fText:GetText():SetFontColor(Float3.BLACK)
    ledCheck:SetScriptHandler("zo_UIPadCtrlCallback")
    ledCheck:Check(false, false)

    local ledCheck_R = UICheckButton:New("CheckButton_LED_R")
    frameAll:AttachChild(ledCheck_R)
	ledCheck_R.LocalTransform:SetTranslateY(-1.0)
	ledCheck_R:SetAnchorHor(0.0, 0.0)
	ledCheck_R:SetAnchorParamHor(320.0, 320.0)
    ledCheck_R:SetAnchorVer(1.0, 1.0)
    ledCheck_R:SetAnchorParamVer(-140.0, -140.0)
    ledCheck_R:SetPivot(0.5, 0.5)
    ledCheck_R:SetSize(80, 80)
    local fText = ledCheck_R:CreateAddText("LED_R")
    fText:GetText():SetFontColor(Float3.BLACK)
    ledCheck_R:SetScriptHandler("zo_UIPadCtrlCallback")
    ledCheck_R:Check(false, false)
    
    zo_CreateFrameMode0()
    zo_CreateFrameMode1()
    zo_SetUIMode(1)

    return frameAll
end

function zo_SetUIMode(mode)
    ZERONE_SimpleCtr_UIMode = mode

    ZERONE_FrameCtrlMode0:Show(false)
    ZERONE_FrameCtrlMode1:Show(false)

    if 0==mode then
        ZERONE_FrameCtrlMode0:Show(true)
    elseif 1==mode then
        ZERONE_FrameCtrlMode1:Show(true)
    end
end

function zo_CreateFrameMode0()
    local frame = UIFrame:New()
    ZERONE_FrameCtrlMode0 = frame
    ZERONE_FrameUICtrl:AttachChild(frame)
    frame.LocalTransform:SetTranslateY(-1.0)
    frame:SetAnchorHor(0.0, 1.0)
    frame:SetAnchorVer(0.0, 1.0)
    
    local round = UIRound:New("RoundCtrl")
    frame:AttachChild(round)
    round.LocalTransform:SetTranslateY(-2.0)
    round:SetAnchorHor(0.0, 0.0)
    round:SetAnchorVer(0.0, 0.0)
    round:SetAnchorParamHor(200.0, 200.0)
    round:SetAnchorParamVer(200.0, 200.0)
    round:SetSize(360, 360)
    round:GetButton():SetStateColorDefaultWhite()
    round:GetButton():GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/ZERONE/images/directctrl/round_in.png")
    round:GetButton():GetPicBoxAtState(UIButtonBase.BS_HOVERED):SetTexture("Data/ZERONE/images/directctrl/round_in.png")
    round:GetButton():GetPicBoxAtState(UIButtonBase.BS_PRESSED):SetTexture("Data/ZERONE/images/directctrl/round_in.png")
    round:GetBack():GetUIPicBox():SetTexture("Data/ZERONE/images/directctrl/round_out.png")
    round:GetButton():SetSize(200, 200)
    round:RegistToScriptSystem()
    round:SetScriptHandler("zo_UIPadCtrlCallback")
    ZERONE_SimpleCtrl_Round = round
    
    local btnFire = UIButton:New("BtnFire")
    frame:AttachChild(btnFire)
    btnFire.LocalTransform:SetTranslateY(-2.0)
    btnFire:SetAnchorHor(1.0, 1.0)
    btnFire:SetAnchorVer(0.0, 0.0)
    btnFire:SetAnchorParamHor(-200.0, -200.0)
    btnFire:SetAnchorParamVer(200.0, 200.0)
    btnFire:SetStateColorDefaultWhite()
    btnFire:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/ZERONE/images/directctrl/btn.png")
    btnFire:SetSize(200,200)
    btnFire:SetScriptHandler("zo_UIPadCtrlCallback")
end

function zo_CreateFrameMode1()
    local frame = UIFrame:New()
    ZERONE_FrameCtrlMode1 = frame
    ZERONE_FrameUICtrl:AttachChild(frame)
    frame.LocalTransform:SetTranslateY(-1.0)
    frame:SetAnchorHor(0.0, 1.0)
    frame:SetAnchorVer(0.0, 1.0)

    local uiFrameLeft = UIFrame:New("FrameLeft")
    frame:AttachChild(uiFrameLeft)
    uiFrameLeft.LocalTransform:SetTranslateY(-1.0)
    uiFrameLeft:SetAnchorHor(0.0, 0.0)
    uiFrameLeft:SetAnchorVer(0.0, 0.0)
    uiFrameLeft:SetAnchorParamHor(200.0, 200.0)
    uiFrameLeft:SetAnchorParamVer(200.0, 200.0)
    uiFrameLeft:SetSize(360, 360)
    local picBack = uiFrameLeft:CreateAddBackgroundPicBox(true, Float3:MakeColor(255, 255, 255))
    picBack:SetTexture("Data/ZERONE/images/directctrl/ctrlbackleftright.png")
	picBack:UseAlphaBlend(true)
    picBack:SetFakeTransparent(false)
    
    local btnLeft0 = UIButton:New("BtnLeft0")
    uiFrameLeft:AttachChild(btnLeft0)
    btnLeft0.LocalTransform:SetTranslateY(-2.0)
    btnLeft0:SetAnchorHor(0.0, 0.5)
    btnLeft0:SetAnchorVer(0.0, 1.0)
    btnLeft0:SetButType(UIButtonBase.BT_PICBOXSWAP)
    btnLeft0:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/ZERONE/images/directctrl/btnleft.png")
    btnLeft0:GetPicBoxAtState(UIButtonBase.BS_NORMAL):UseAlphaBlendAdd(true)
    btnLeft0:GetPicBoxAtState(UIButtonBase.BS_HOVERED):SetTexture("Data/ZERONE/images/directctrl/btnleft.png")
    btnLeft0:GetPicBoxAtState(UIButtonBase.BS_HOVERED):UseAlphaBlendAdd(true)
    btnLeft0:GetPicBoxAtState(UIButtonBase.BS_PRESSED):SetTexture("Data/ZERONE/images/directctrl/btnleftpress.png")
    btnLeft0:GetPicBoxAtState(UIButtonBase.BS_PRESSED):UseAlphaBlendAdd(true)
    btnLeft0:SetScriptHandler("zo_UIPadCtrlCallback")
    local btnLeft1 = UIButton:New("BtnLeft1")
    uiFrameLeft:AttachChild(btnLeft1)
    btnLeft1.LocalTransform:SetTranslateY(-2.0)
    btnLeft1:SetAnchorHor(0.5, 1.0)
    btnLeft1:SetAnchorVer(0.0, 1.0)
    btnLeft1:SetButType(UIButtonBase.BT_PICBOXSWAP)
    btnLeft1:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/ZERONE/images/directctrl/btnright.png")
    btnLeft1:GetPicBoxAtState(UIButtonBase.BS_NORMAL):UseAlphaBlendAdd(true)
    btnLeft1:GetPicBoxAtState(UIButtonBase.BS_HOVERED):SetTexture("Data/ZERONE/images/directctrl/btnright.png")
    btnLeft1:GetPicBoxAtState(UIButtonBase.BS_HOVERED):UseAlphaBlendAdd(true)
    btnLeft1:GetPicBoxAtState(UIButtonBase.BS_PRESSED):SetTexture("Data/ZERONE/images/directctrl/btnrightpress.png")
    btnLeft1:GetPicBoxAtState(UIButtonBase.BS_PRESSED):UseAlphaBlendAdd(true)
    btnLeft1:SetScriptHandler("zo_UIPadCtrlCallback")

    local uiFrameRight = UIFrame:New("FrameRight")
    frame:AttachChild(uiFrameRight)
    uiFrameRight.LocalTransform:SetTranslateY(-1.0)
    uiFrameRight:SetAnchorHor(1.0, 1.0)
    uiFrameRight:SetAnchorVer(0.0, 0.0)
    uiFrameRight:SetAnchorParamHor(-200.0, -200.0)
    uiFrameRight:SetAnchorParamVer(200.0, 200.0)
    uiFrameRight:SetSize(360, 360)
    local picBack = uiFrameRight:CreateAddBackgroundPicBox(true, Float3:MakeColor(255, 255, 255))
    picBack:SetTexture("Data/ZERONE/images/directctrl/ctrlbackupdown.png")
	picBack:UseAlphaBlend(true)
    picBack:SetFakeTransparent(false)
    
    local btnRight0 = UIButton:New("BtnRight0")
    uiFrameRight:AttachChild(btnRight0)
    btnRight0.LocalTransform:SetTranslateY(-2.0)
    btnRight0:SetAnchorHor(0.0, 1.0)
    btnRight0:SetAnchorVer(0.5, 1.0)
    btnRight0:SetStateColorDefaultWhite()
    btnRight0:SetButType(UIButtonBase.BT_PICBOXSWAP)
    btnRight0:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/ZERONE/images/directctrl/btnup.png")
    btnRight0:GetPicBoxAtState(UIButtonBase.BS_NORMAL):UseAlphaBlendAdd(true)
    btnRight0:GetPicBoxAtState(UIButtonBase.BS_HOVERED):SetTexture("Data/ZERONE/images/directctrl/btnup.png")
    btnRight0:GetPicBoxAtState(UIButtonBase.BS_HOVERED):UseAlphaBlendAdd(true)
    btnRight0:GetPicBoxAtState(UIButtonBase.BS_PRESSED):SetTexture("Data/ZERONE/images/directctrl/btnuppress.png")
    btnRight0:GetPicBoxAtState(UIButtonBase.BS_PRESSED):UseAlphaBlendAdd(true)
    btnRight0:SetScriptHandler("zo_UIPadCtrlCallback")
    local btnRight1 = UIButton:New("BtnRight1")
    uiFrameRight:AttachChild(btnRight1)
    btnRight1.LocalTransform:SetTranslateY(-2.0)
    btnRight1:SetAnchorHor(0.0, 1.0)
    btnRight1:SetAnchorVer(0.0, 0.5)
    btnRight1:SetStateColorDefaultWhite()
    btnRight1:SetButType(UIButtonBase.BT_PICBOXSWAP)
    btnRight1:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/ZERONE/images/directctrl/btndown.png")
    btnRight1:GetPicBoxAtState(UIButtonBase.BS_NORMAL):UseAlphaBlendAdd(true)
    btnRight1:GetPicBoxAtState(UIButtonBase.BS_HOVERED):SetTexture("Data/ZERONE/images/directctrl/btndown.png")
    btnRight1:GetPicBoxAtState(UIButtonBase.BS_HOVERED):UseAlphaBlendAdd(true)
    btnRight1:GetPicBoxAtState(UIButtonBase.BS_PRESSED):SetTexture("Data/ZERONE/images/directctrl/btndownpress.png")
    btnRight1:GetPicBoxAtState(UIButtonBase.BS_PRESSED):UseAlphaBlendAdd(true)
    btnRight1:SetScriptHandler("zo_UIPadCtrlCallback")

    local btnFire = UIButton:New("BtnFire")
    frame:AttachChild(btnFire)
    btnFire.LocalTransform:SetTranslateY(-2.0)
    btnFire:SetAnchorHor(1.0, 1.0)
    btnFire:SetAnchorVer(1.0, 1.0)
    btnFire:SetAnchorParamHor(-200.0, -200.0)
    btnFire:SetAnchorParamVer(-100.0, -100.0)
    btnFire:SetStateColorDefaultWhite()
    btnFire:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/ZERONE/images/directctrl/btn.png")
    btnFire:SetSize(150,150)
    btnFire:SetScriptHandler("zo_UIPadCtrlCallback")
end

function zo_UIPadCtrlCallback(ptr,callType)
    local obj=Cast:ToO(ptr)
    local name=obj:GetName()
    if UICT_PRESSED==callType then
        PX2_GR:PlayScale(obj)
        if "BtnLeft0" == name then
            ZERONE_IsPressedLeft0 = true
            zo_UpdateButtonsPress()
        end
        if "BtnLeft1" == name then
            ZERONE_IsPressedLeft1 = true
            zo_UpdateButtonsPress()
        end
        if "BtnRight0" == name then
            ZERONE_IsPressedRight0 = true
            zo_UpdateButtonsPress()
        end
        if "BtnRight1" == name then
            ZERONE_IsPressedRight1 = true
            zo_UpdateButtonsPress()
        end
        if "BtnFire" == name then
            --if ZERONE_IsFireRobot then
            rc_Arduino:DigitalWrite(Arduino.P_13, true)
            --end
        end
    elseif UICT_RELEASED ==callType then
        PX2_GR:PlayNormal(obj)        
        if "BtnClose" == name then
            ZERONE_FrameUICtrl:Show(false)
            ZERONE_UIFaceCtrlScript._btnUIPad:Check(false, false)

        elseif "ButUIMode" == name then
            if 0==ZERONE_SimpleCtr_UIMode then
                zo_SetUIMode(1)
            elseif 1==ZERONE_SimpleCtr_UIMode then
                zo_SetUIMode(0)
            end
        end
        
        if "BtnLeft0" == name then
            ZERONE_IsPressedLeft0 = false
            zo_UpdateButtonsPress()
        end
        if "BtnLeft1" == name then
            ZERONE_IsPressedLeft1 = false
            zo_UpdateButtonsPress()
        end
        if "BtnRight0" == name then
            ZERONE_IsPressedRight0 = false
            zo_UpdateButtonsPress()
        end
        if "BtnRight1" == name then
            ZERONE_IsPressedRight1 = false
            zo_UpdateButtonsPress()
        end

        if "BtnFire" == name then
            --if ZERONE_IsFireRobot then
            rc_Arduino:DigitalWrite(Arduino.P_13, false)
            --end
        end
    elseif UICT_RELEASED_NOTPICK == callType then
        PX2_GR:PlayNormal(obj)

        if "BtnLeft0" == name then
            ZERONE_IsPressedLeft0 = false
            zo_UpdateButtonsPress()
        end
        if "BtnLeft1" == name then
            ZERONE_IsPressedLeft1 = false
            zo_UpdateButtonsPress()
        end
        if "BtnRight0" == name then
            ZERONE_IsPressedRight0 = false
            zo_UpdateButtonsPress()
        end
        if "BtnRight1" == name then
            ZERONE_IsPressedRight1 = false
            zo_UpdateButtonsPress()
        end
    elseif UICT_ROUND_DRAG == callType then
        if "RoundCtrl" == name then      
            local leftMoveType = obj:GetDragingLeftMoveType()
            local leftMoveSpeed = obj:GetDragingLeftMoveSpeed()
            local rightMoveType = obj:GetDragingRightMoveType()
            local rightMoveSpeed = obj:GetDragingRightMoveSpeed()
            if leftMoveType == rightMoveType then
                leftMoveSpeed = leftMoveSpeed * ZERONE_SpeedAdjustGo
                rightMoveSpeed = rightMoveSpeed * ZERONE_SpeedAdjustGo
            else
                leftMoveSpeed = leftMoveSpeed * ZERONE_SpeedAdjustTurn
                rightMoveSpeed = rightMoveSpeed * ZERONE_SpeedAdjustTurn
            end

            if ZERONE_LastLeftType~=leftMoveType or ZERONE_LastLeftSpeed ~=leftMoveSpeed then
                rc_Arduino:Run(0, leftMoveType, leftMoveSpeed)

                ZERONE_LastLeftType = leftMoveType
                ZERONE_LastLeftSpeed = leftMoveSpeed
            end
            if ZERONE_LastRightType~=rightMoveType or ZERONE_LastRightSpeed ~=rightMoveSpeed then
                rc_Arduino:Run(1, rightMoveType, rightMoveSpeed)

                ZERONE_LastRightType = rightMoveType
                ZERONE_LastRightSpeed = rightMoveSpeed
            end
        end
    elseif UICT_ROUND_RELEASED == callType then
        if "RoundCtrl" == name then      
            rc_Arduino:Run(0, 0, 0)
            rc_Arduino:Run(1, 0, 0)

            ZERONE_LastLeftType = 0
            ZERONE_LastLeftSpeed = 0.0
            ZERONE_LastRightType = 0
            ZERONE_LastRightSpeed = 0.0
        end
    elseif UICT_ROUND_RELEASEDNOTPICK == callType then
        if "RoundCtrl" == name then      
            rc_Arduino:Run(0, 0, 0)
            rc_Arduino:Run(1, 0, 0)

            ZERONE_LastLeftType = 0
            ZERONE_LastLeftSpeed = 0.0
            ZERONE_LastRightType = 0
            ZERONE_LastRightSpeed = 0.0
        end
    elseif UICT_SLIDERCHANGED == callType then
        local percent = obj:GetPercent()
        local pos = 180.0*percent
        if not ZERONE_IsFireRobot then
            if "slider3" == name then
                ZERONE_SpeedAdjustTurn = percent
                PX2_PROJ:SetConfig("Slider3", ""..percent)
            elseif "slider2"==name then
                ZERONE_SpeedAdjustGo = percent
                PX2_PROJ:SetConfig("Slider2", ""..percent)
            elseif "slider1"==name then
                rc_Arduino:ServoWrite(0, pos)
                PX2_PROJ:SetConfig("Slider1", ""..percent)
            end
        else
            local pos = 180.0*percent
            if "slider3" == name then
                ZERONE_SpeedAdjustTurn = percent
                ZERONE_SpeedAdjustGo = percent
                PX2_PROJ:SetConfig("Slider3", ""..percent)
            elseif "slider2"==name then
                rc_Arduino:ServoWrite(0, pos)
                PX2_PROJ:SetConfig("Slider2", ""..percent)
            elseif "slider1"==name then
                rc_Arduino:ServoWrite(1, pos)
                PX2_PROJ:SetConfig("Slider1", ""..percent)
            end
        end
    elseif UICT_CHECKED == callType then
        if "CheckButton_LED_L"==name then
            rc_Arduino:DigitalWrite(Arduino.P_11, true)
        elseif "CheckButton_LED_R"==name then
            rc_Arduino:DigitalWrite(Arduino.P_12, true)
        elseif "CheckButton_Sound"==name then
            rc_Arduino:DigitalWrite(Arduino.P_10, true)
        end
    elseif UICT_DISCHECKED == callType then
        if "CheckButton_LED_L"==name then
            rc_Arduino:DigitalWrite(Arduino.P_11, false)
        elseif "CheckButton_LED_R"==name then
            rc_Arduino:DigitalWrite(Arduino.P_12, false)
        elseif "CheckButton_Sound"==name then
            rc_Arduino:DigitalWrite(Arduino.P_10, false)
        end
    end
end

function zo_UpdateButtonsPress()
    if ZERONE_IsPressedLeft0 then
        rc_Arduino:Run(0, 2, 255.0 * ZERONE_SpeedAdjustTurn)
        rc_Arduino:Run(1, 1, 255.0 * ZERONE_SpeedAdjustTurn)
    elseif ZERONE_IsPressedLeft1 then
        rc_Arduino:Run(0, 1, 255.0 * ZERONE_SpeedAdjustTurn)
        rc_Arduino:Run(1, 2, 255.0 * ZERONE_SpeedAdjustTurn)
    else
        if ZERONE_IsPressedRight0 then
            rc_Arduino:Run(0, 1, 255.0 * ZERONE_SpeedAdjustGo)
            rc_Arduino:Run(1, 1, 255.0 * ZERONE_SpeedAdjustGo)
        elseif ZERONE_IsPressedRight1 then
            rc_Arduino:Run(0, 2, 255.0 * ZERONE_SpeedAdjustGo)
            rc_Arduino:Run(1, 2, 255.0 * ZERONE_SpeedAdjustGo)
        else
            rc_Arduino:Run(0, 0, 255.0 * ZERONE_SpeedAdjustGo)
            rc_Arduino:Run(1, 0, 255.0 * ZERONE_SpeedAdjustGo)
        end
    end
end