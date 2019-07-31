--play.lua

require("Data/Joystick/scripts/lua/language.lua")
require("Data/Joystick/scripts/lua/homepage.lua")
require("Data/Joystick/scripts/lua/gamepadpage1.lua")
require("Data/Joystick/scripts/lua/connect.lua")
require("Data/Joystick/scripts/lua/question.lua")
require("Data/Joystick/scripts/lua/bluetooth.lua")
require("Data/Joystick/scripts/lua/common.lua")

function engine_project_preplay() 
    PX2_APP:SetShowInfo(false)
    PX2_BLUETOOTH:SetDataProtocolHex(true)
    rc_AddLanguage()
end

function engine_project_play()
    rc_Play()
end

function _GetSendStr()
    local sendStr = ""

    PX2_BLUETOOTH:BeginHex("FF55");

    local dragDirLeft = rc_RoundLeft:GetDraggingDirAvector()
    local dragPercentLeft = rc_RoundLeft:GetDraggingPercent()
    local horValLeft = (dragDirLeft:X() * dragPercentLeft+ 1.0) * 0.5 * 255
    local verValLeft = (dragDirLeft:Y() * dragPercentLeft + 1.0) * 0.5 * 255
    PX2_LOGGER:LogInfo("RobotControl", "dragDirLeft:X():"..dragDirLeft:X()) 
    PX2_LOGGER:LogInfo("RobotControl", "dragDirLeft:Y():"..dragDirLeft:Y()) 
    PX2_LOGGER:LogInfo("RobotControl", "dragPercentLeft:"..dragPercentLeft)
    PX2_LOGGER:LogInfo("RobotControl", "horValLeft:"..horValLeft)
    PX2_LOGGER:LogInfo("RobotControl", "verValLeft:"..verValLeft)

    PX2_BLUETOOTH:AddHex(horValLeft);
    PX2_BLUETOOTH:AddHex(verValLeft);

    local dragDirRight = rc_RoundRight:GetDraggingDirAvector()
    local dragPercentRight = rc_RoundRight:GetDraggingPercent()
    local horValRight = (dragDirRight:X() * dragPercentRight + 1.0) * 0.5 * 255 
    local verValRight = (dragDirRight:Y() * dragPercentRight + 1.0) * 0.5 * 255 

    PX2_BLUETOOTH:AddHex(horValRight);
    PX2_BLUETOOTH:AddHex(verValRight);

    local bit0 = 0
    local bit1 = 0
    local bit2 = 0
    local bit3 = 0

    local bit4 = 0
    local bit5 = 0
    local bit6 = 0
    local bit7 = 0

    if rc_BtnLeft0:GetButtonState() == UIButtonBase.BS_PRESSED then
        bit0 = 1
    else
    end

    if rc_BtnLeft1:GetButtonState() == UIButtonBase.BS_PRESSED then
        bit1 = 1
    else
    end

    if rc_BtnLeft2:GetButtonState() == UIButtonBase.BS_PRESSED then
        bit2 = 1
    else
    end

    if rc_BtnLeft3:GetButtonState() == UIButtonBase.BS_PRESSED then
        bit3 = 1
    else
    end

    if rc_BtnRight0:GetButtonState() == UIButtonBase.BS_PRESSED then
        bit4 = 1
    else
    end

    if rc_BtnRight1:GetButtonState() == UIButtonBase.BS_PRESSED then
        bit5 = 1
    else
    end

    if rc_BtnRight2:GetButtonState() == UIButtonBase.BS_PRESSED then
        bit6 = 1
    else
    end

    if rc_BtnRight3:GetButtonState() == UIButtonBase.BS_PRESSED then
        bit7 = 1
    else
    end

    PX2_BLUETOOTH:AddHex(bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7);

    local sVal0 = rc_Slider0:GetPercent()
    sVal0 = sVal0 * 255
    local sVal1 = rc_Slider1:GetPercent()
    sVal1 = sVal1 * 255
    local sVal2 = rc_Slider2:GetPercent()
    sVal2 = sVal2 * 255
    local sVal3 = rc_Slider3:GetPercent()
    sVal3 = sVal3 * 255
    PX2_BLUETOOTH:AddHex(sVal0);
    PX2_BLUETOOTH:AddHex(sVal1);
    PX2_BLUETOOTH:AddHex(sVal2);
    PX2_BLUETOOTH:AddHex(sVal3);
    sendStr = PX2_BLUETOOTH:EndHexStr(true);

    return sendStr
end

sendStr = "";
elapsedsecondsAll = 0.0
function engine_project_update(appseconds, elapsedseconds) 

    if nil~= rc_Arduino then
        rc_Arduino:Update(elapsedseconds)
    end

    --PX2_LOGGER:LogInfo("RobotControl", "ble sended")
    --PX2_BLUETOOTH:Send("120 130 140 150 160\n")
    --PX2_BLUETOOTH:SendHex("FF55 80 0F 80 0F 80 0F 80 2D");

    elapsedsecondsAll = elapsedsecondsAll + elapsedseconds;
    if elapsedsecondsAll > 0.2 then
        sendStr = _GetSendStr()
        elapsedsecondsAll = 0
        if rc_Arduino:IsInitlized() then
            PX2_BLUETOOTH:SendHex(sendStr);
        end
    end
end

function engine_project_cmd(cmd, param0, param1, param2) 
end

rc_ReturnButton = nil
rc_ConnectFrame = nil
rc_QuestionFrame = nil
rc_FrameHome = nil
rc_FramePad = nil

rc_Slider0 = nil
rc_Slider1 = nil
rc_Slider2 = nil
rc_Slider3 = nil

rc_RoundLeft = nil
rc_RoundRight = nil

rc_BtnLeft0 = nil
rc_BtnLeft1 = nil
rc_BtnLeft2 = nil
rc_BtnLeft3 = nil

rc_BtnRight0 = nil
rc_BtnRight1 = nil
rc_BtnRight2 = nil
rc_BtnRight3 = nil

function rc_UISetTextFont(text, fontSize)
	text:SetFont("Data/engine/font.ttf", fontSize, fontSize)
end

local cornorBtnPos = 100.0

function rc_Play()
    rc_Arduino = Arduino:New()
    PX2_PROJ:PoolSet("Arduino", rc_Arduino)

    PX2_ENGINECANVAS:SetClearColor(Float4:MakeColor(63, 72, 204, 255))
    PX2_ENGINESCENECANVAS:SetClearColor(Float4:MakeColor(0, 0, 0, 255))

    local ui = PX2_PROJ:GetUI()
    local frameRC = UIFrame:New()
    ui:AttachChild(frameRC)
    frameRC:SetAnchorHor(0.0, 1.0)
    frameRC:SetAnchorVer(0.0, 1.0)

    local btnConnect = UIButton:New("BtnConnect")
    frameRC:AttachChild(btnConnect)
    btnConnect.LocalTransform:SetTranslateY(-50.0)
    btnConnect:SetAnchorHor(0.5, 0.5)
    btnConnect:SetAnchorVer(0.5, 0.5)
    btnConnect:SetAnchorParamHor(-cornorBtnPos*0.5, -cornorBtnPos*0.5)
    btnConnect:SetSize(60.0, 60.0)
    btnConnect:SetScriptHandler("rc_UICallabck")
    btnConnect:SetStateColorDefaultWhite()
    btnConnect:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/Joystick/images/bluetooth.png")

    local btnQuestion = UIButton:New("BtnQuestion")
    frameRC:AttachChild(btnQuestion)
    btnQuestion.LocalTransform:SetTranslateY(-50.0)
    btnQuestion:SetAnchorHor(0.5, 0.5)
    btnQuestion:SetAnchorVer(0.5, 0.5)
    btnQuestion:SetAnchorParamHor(cornorBtnPos*0.5, cornorBtnPos*0.5)
    btnQuestion:SetSize(60.0, 60.0)
    btnQuestion:SetScriptHandler("rc_UICallabck")
    btnQuestion:SetStateColorDefaultWhite()
    btnQuestion:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/Joystick/images/directctrl/slider1.png")
    local text = btnQuestion:CreateAddText("?")
    text:GetText():SetFontColor(Float3.BLACK)
    text:GetText():SetFontSize(32, 32)

    local btnReturn = UIButton:New("BtnReturn")
    frameRC:AttachChild(btnReturn)
    rc_ReturnButton = btnReturn
    btnReturn.LocalTransform:SetTranslateY(-50.0)
    btnReturn:SetAnchorHor(0.0, 0.0)
    btnReturn:SetAnchorVer(1.0, 1.0)
    btnReturn:SetAnchorParamHor(cornorBtnPos, cornorBtnPos)
    btnReturn:SetAnchorParamVer(-cornorBtnPos, -cornorBtnPos)
    btnReturn:SetSize(60.0, 60.0)
    btnReturn:SetScriptHandler("rc_UICallabck")
    btnReturn:SetStateColorDefaultWhite()
    btnReturn:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/Joystick/images/return.png")
    btnReturn:Show(false)

    local cntFrame = rc_Connect()
    frameRC:AttachChild(cntFrame)
    rc_ConnectFrame = cntFrame
    cntFrame.LocalTransform:SetTranslateY(-100.0)
    cntFrame:SetAnchorHor(0.0, 1.0)
    cntFrame:SetAnchorVer(0.0, 1.0)
    cntFrame:Show(false)

    local questionFrame = rc_Question()
    frameRC:AttachChild(questionFrame)
    rc_QuestionFrame = questionFrame
    questionFrame.LocalTransform:SetTranslateY(-100.0)
    questionFrame:SetAnchorHor(0.0, 1.0)
    questionFrame:SetAnchorVer(0.0, 1.0)
    questionFrame:Show(false)

    local frameHome = rc_FrameHomePage()
    frameRC:AttachChild(frameHome)
    rc_FrameHome = frameHome
    rc_FrameHome.LocalTransform:SetTranslateY(-1.0)
    rc_FrameHome:SetAnchorHor(0.0, 1.0)
    rc_FrameHome:SetAnchorVer(0.0, 1.0)
    rc_FrameHome:Show(true)

    local framePad = rc_FramePadPage1()
    frameRC:AttachChild(framePad)
    rc_FramePad = framePad
    framePad.LocalTransform:SetTranslateY(-1.0)
    framePad:SetAnchorHor(0.0, 1.0)
    framePad:SetAnchorVer(0.0, 1.0)
    framePad:Show(true)
end

function rc_UICallabck(ptr, callType)
    local obj = Cast:ToO(ptr) 
	local name = obj:GetName()	

    if UICT_PRESSED==callType then
        playFrameScale(obj)
        
        if "BtnVoiceRecording"==name then
            PX2_VOICESDK:StartVoiceListening()
            PX2_LOGGER:LogInfo("RobotControl", "start voice")
        elseif "BtnEyeLeft"==name then
            local randomVal = math.random()
            if randomVal< 0.25 then
                PX2_VOICESDK:Speak("别戳我的眼睛")
            elseif randomVal< 0.6 then
                PX2_VOICESDK:Speak("再戳我的眼睛，我就不理你了")
            elseif randomVal< 0.75 then
                PX2_VOICESDK:Speak("小朋友，眼睛有什么好戳的")
            else
                PX2_VOICESDK:Speak("小朋友，你太调皮了，我要告诉你妈妈")
            end
        elseif "BtnEyeRight"==name then
            local randomVal = math.random()
            if randomVal< 0.25 then
                PX2_VOICESDK:Speak("别戳我的眼睛")
            elseif randomVal< 0.6 then
                PX2_VOICESDK:Speak("再戳我的眼睛，我就不理你了")
            elseif randomVal< 0.75 then
                PX2_VOICESDK:Speak("小朋友，眼睛有什么好戳的")
            else
                PX2_VOICESDK:Speak("小朋友，你太调皮了，我要告诉你妈妈")
            end
        end
	elseif UICT_RELEASED==callType then
        playFrameNormal(obj)

        if "BtnConnect" == name then
            rc_ConnectFrame:Show(true)
        elseif "BtnQuestion" == name then
            rc_QuestionFrame:Show(true)
        elseif "BtnDlgClose" == name then
            rc_ConnectFrame:Show(false)
            rc_QuestionFrame:Show(false)
        elseif "BtnVoiceRecording"==name then
            PX2_VOICESDK:EndVoiceListening()
            PX2_LOGGER:LogInfo("RobotControl", "end voice")
        end
    elseif UICT_CHECKED==callType then  
    elseif UICT_DISCHECKED==callType then
    elseif UICT_RELEASED_NOTPICK==callType then
        playFrameNormal(obj)
    elseif UICT_ROUND_RELEASEDNOTPICK==callType then
    elseif UICT_COMBOBOX_CHOOSED==callType then 
	end
end