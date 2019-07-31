--play.lua

require("Data/RobotControl/scripts/lua/language.lua")
require("Data/RobotControl/scripts/lua/homepage.lua")
require("Data/RobotControl/scripts/lua/gamepadpage.lua")
require("Data/RobotControl/scripts/lua/acceleratorpage.lua")
require("Data/RobotControl/scripts/lua/voicepage.lua")
require("Data/RobotControl/scripts/lua/connect.lua")
require("Data/RobotControl/scripts/lua/question.lua")
require("Data/RobotControl/scripts/lua/bluetooth.lua")
require("Data/RobotControl/scripts/lua/common.lua")

function engine_project_preplay() 
    PX2_APP:SetShowInfo(false)
    PX2_BLUETOOTH:SetDataProtocolHex(false)
    rc_AddLanguage()
end

function engine_project_play()
    rc_Play()
end

function engine_project_update(appseconds, elapsedseconds) 
    rc_AcceleratorUpdate(appseconds, elapsedseconds)
end

function engine_project_cmd(cmd, param0, param1, param2) 
end

rc_Arduino = nil

rc_ReturnButton = nil
rc_ConnectFrame = nil
rc_QuestionFrame = nil
rc_FrameHome = nil
rc_FramePad = nil
rc_FrameAxis = nil
rc_FrameVoice = nil
rc_SpeakText = nil

rc_TextWidth = 660.0
rc_TextHeight = 200.0

rc_SpeedAdjustGo = 0.5
rc_SpeedAdjustTurn = 0.5

rc_IsRevertLeft = false
rc_IsRevertRight = false

PageType = {
    PT_HOME = 0,
    PT_CTRL = 1,
    PT_AXIS = 2,
    PT_VOICE = 3,
    PT_MAX_TYPE = 4,
}
rc_ThePageType = PageType.PT_HOME

function rc_UISetTextFont(text, fontSize)
	text:SetFont("Data/engine/font.ttf", fontSize, fontSize)
end

local cornorBtnPos = 70.0

function rc_Play()
    rc_Arduino = Arduino:New()
    PX2_PROJ:PoolSet("Arduino", rc_Arduino)

    PX2_ENGINECANVAS:SetClearColor(Float4:MakeColor(63, 72, 204, 255))
    PX2_ENGINESCENECANVAS:SetClearColor(Float4:MakeColor(63, 72, 204, 255))

    local ui = PX2_PROJ:GetUI()
    local frameRC = UIFrame:New()
    ui:AttachChild(frameRC)
    frameRC:SetAnchorHor(0.0, 1.0)
    frameRC:SetAnchorVer(0.0, 1.0)

    local btnConnect = UIButton:New("BtnConnect")
    frameRC:AttachChild(btnConnect)
    btnConnect.LocalTransform:SetTranslateY(-50.0)
    btnConnect:SetAnchorHor(1.0, 1.0)
    btnConnect:SetAnchorVer(1.0, 1.0)
    btnConnect:SetAnchorParamHor(-cornorBtnPos, -cornorBtnPos)
    btnConnect:SetAnchorParamVer(-cornorBtnPos, -cornorBtnPos)
    btnConnect:SetSize(60.0, 60.0)
    btnConnect:SetScriptHandler("rc_UICallabck")
    btnConnect:SetStateColorDefaultWhite()
    btnConnect:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/RobotControl/images/bluetooth.png")

    local btnQuestion = UIButton:New("BtnQuestion")
    frameRC:AttachChild(btnQuestion)
    btnQuestion.LocalTransform:SetTranslateY(-50.0)
    btnQuestion:SetAnchorHor(1.0, 1.0)
    btnQuestion:SetAnchorVer(1.0, 1.0)
    btnQuestion:SetAnchorParamHor(-cornorBtnPos*2.0, -cornorBtnPos*2.0)
    btnQuestion:SetAnchorParamVer(-cornorBtnPos, -cornorBtnPos)
    btnQuestion:SetSize(60.0, 60.0)
    btnQuestion:SetScriptHandler("rc_UICallabck")
    btnQuestion:SetStateColorDefaultWhite()
    btnQuestion:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/RobotControl/images/directctrl/slider1.png")
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
    btnReturn:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/RobotControl/images/return.png")
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

    local framePad = rc_FramePadPage()
    frameRC:AttachChild(framePad)
    rc_FramePad = framePad
    framePad.LocalTransform:SetTranslateY(-1.0)
    framePad:SetAnchorHor(0.0, 1.0)
    framePad:SetAnchorVer(0.0, 1.0)
    framePad:Show(false)

    local frameAcceler = rc_AcceleratorPage()
    frameRC:AttachChild(frameAcceler)
    rc_FrameAxis = frameAcceler
    frameAcceler.LocalTransform:SetTranslateY(-1.0)
    frameAcceler:SetAnchorHor(0.0, 1.0)
    frameAcceler:SetAnchorVer(0.0, 1.0)
    frameAcceler:Show(false)

    local frameVoice = rc_VoicePage()
    frameRC:AttachChild(frameVoice)
    rc_FrameVoice = frameVoice
    frameVoice.LocalTransform:SetTranslateY(-1.0)
    frameVoice:SetAnchorHor(0.0, 1.0)
    frameVoice:SetAnchorVer(0.0, 1.0)
    frameVoice:Show(false)

    PX2_PFSDK:StartAccelerator()
    PX2_PFSDK:RegistAccelerator()

    PX2_VOICESDK:Speak("Hi,我是零一机器人")
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
        elseif "BtnCtrl" == name then
            rc_ThePageType = PageType.PT_CTRL

            rc_FrameHome:Show(false)
            rc_FramePad:Show(false)
            rc_FrameAxis:Show(false)
            rc_FrameVoice:Show(false)
            rc_FramePad:Show(true)

            rc_ReturnButton:Show(true)
        elseif "BtnAxis"==name then
            rc_ThePageType = PageType.PT_AXIS

            rc_FrameHome:Show(false)
            rc_FramePad:Show(false)
            rc_FrameAxis:Show(false)
            rc_FrameVoice:Show(false)
            rc_FrameAxis:Show(true)

            rc_ReturnButton:Show(true)
        elseif "BtnVoice"==name then
            rc_ThePageType = PageType.PT_VOICE

            rc_FrameHome:Show(false)
            rc_FramePad:Show(false)
            rc_FrameAxis:Show(false)
            rc_FrameVoice:Show(false)
            rc_FrameVoice:Show(true)

            rc_ReturnButton:Show(true)

        elseif "BtnReturn"==name then
            if PageType.PT_HOME ~= rc_ThePageType then
                rc_FrameHome:Show(true)
                rc_FramePad:Show(false)
                rc_FrameAxis:Show(false)
                rc_FrameVoice:Show(false)

                rc_RobotCtrlLeave()

                rc_ReturnButton:Show(false)

                rc_ThePageType = PageType.PT_HOME
            end
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

function rc_RobotCtrlLeave()
    rc_Arduino:Run(Arduino.SDT_NONE, 0)
end