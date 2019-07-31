-- voicepage.lua

function rc_VoicePage ()
    local uiFrame = UIFrame:New()

	uiFrame:SetAnchorHor(0.0, 1.0)
    uiFrame:SetAnchorVer(0.0, 1.0)

    local btnEyeLeft = UIButton:New("BtnEyeLeft")
    uiFrame:AttachChild(btnEyeLeft)
    btnEyeLeft.LocalTransform:SetTranslateY(-1.0)
    btnEyeLeft:SetAnchorHor(0.5, 0.5)
    btnEyeLeft:SetAnchorParamHor(-250.0, -250.0)
    btnEyeLeft:SetAnchorVer(0.5, 0.5)
    btnEyeLeft:SetAnchorParamVer(100.0, 100.0)
    btnEyeLeft:SetSize(220.0, 220.0)
    btnEyeLeft:SetScriptHandler("rc_UICallabck")
    btnEyeLeft:SetStateColorDefaultWhite()
    btnEyeLeft:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/RobotControl/images/directctrl/round.png")

    local btnEyeRight = UIButton:New("BtnEyeRight")
    uiFrame:AttachChild(btnEyeRight)
    btnEyeRight.LocalTransform:SetTranslateY(-1.0)
    btnEyeRight:SetAnchorHor(0.5, 0.5)
    btnEyeRight:SetAnchorParamHor(250.0, 250.0)
    btnEyeRight:SetAnchorVer(0.5, 0.5)
    btnEyeRight:SetAnchorParamVer(100.0, 100.0)
    btnEyeRight:SetSize(220.0, 220.0)
    btnEyeRight:SetScriptHandler("rc_UICallabck")
    btnEyeRight:SetStateColorDefaultWhite()
    btnEyeRight:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/RobotControl/images/directctrl/round.png")
    
    local btnVoice = UIButton:New("BtnVoiceRecording")
    uiFrame:AttachChild(btnVoice)
    btnVoice.LocalTransform:SetTranslateY(-1.0)
    btnVoice:SetAnchorHor(0.5, 0.5)
    btnVoice:SetAnchorVer(0.5, 0.5)
    btnVoice:SetAnchorParamVer(-130.0, -130.0)
    btnVoice:SetSize(150.0, 150.0)
    btnVoice:SetScriptHandler("rc_UICallabck")
    btnVoice:SetStateColorDefaultWhite()
    btnVoice:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/RobotControl/images/voice.png")

    local text = UIFText:New()
    rc_SpeakText = text
    uiFrame:AttachChild(text)
    text.LocalTransform:SetTranslateY(-5.0)
    text:GetText():SetFontColor(Float3.WHITE)
    text:GetText():SetFontSize(36)
    text:SetAnchorVer(0.0, 0.0)
    text:SetAnchorParamVer(70.0, 70.0)
    text:SetSize(rc_TextWidth, rc_TextHeight)
    text:GetText():SetAutoWarp(true)
    text:GetText():SetText("你可以按着按钮说话")
    text:SetPivot(0.5, 0.5)

    PX2_VOICESDK:EnableAutoSpeak(false)
    PX2_VOICESDK:EnableAutoSpeakTTS(true)    

    UnRegistAllEventFunctions("VoiceSDKSpace::RecognizeResults")
    RegistEventFunction("VoiceSDKSpace::RecognizeResults", function(txt, txtJson)
        PX2_LOGGER:LogInfo("RobotControl", txt)

        rc_SpeakText:GetText():SetText(txt)
        local fontWidth = rc_SpeakText:GetText():GetTextWidth()
        if fontWidth < (rc_TextWidth-20.0) then
            rc_SpeakText:GetText():SetRectUseage(RU_ALIGNS)
            rc_SpeakText:SetHeight(rc_TextHeight)
            rc_SpeakText:SetAnchorParamVer(rc_TextHeight*0.5, rc_TextHeight*0.5)
        else
            local numHeight = (fontWidth/rc_TextWidth) * 1.0 + 1
            local height = numHeight * 40
            rc_SpeakText:GetText():SetRectUseage(RU_CLIPWARP)
            rc_SpeakText:SetHeight(height)
            if height>rc_TextHeight then
                rc_SpeakText:SetAnchorParamVer(height*0.5, height*0.5)
            end
        end

        local strPlayMusic = "播放音乐"
        local strLen = string.len(strPlayMusic) 

        if string.find(txt, "播放最热音乐") then
            PX2_VOICESDK:PlayHotMusic()     
        elseif string.find(txt, strPlayMusic) then
            local subStr = ""..string.sub(txt, strLen +4, -4)  
            PX2_VOICESDK:PlayMusic(""..subStr)
        elseif string.find(txt, "停止音乐") or string.find(txt, "关掉音乐")  then
            PX2_SS:PlayMusic(0, nil, true, 0.0)
            PX2_SS:ClearAllSounds()
        elseif string.find(txt, "机器人前进") then
            rc_Arduino:Run(Arduino.SDT_FORWARD, 255.0 * rc_SpeedAdjustGo)
        elseif string.find(txt, "机器人后退") then
            rc_Arduino:Run(Arduino.SDT_BACKWARD, 255.0 * rc_SpeedAdjustGo)
        elseif string.find(txt, "机器人左转") then
            rc_Arduino:Run(Arduino.SDT_LEFT, 255.0 * rc_SpeedAdjustTurn)
        elseif string.find(txt, "机器人右转") then
            rc_Arduino:Run(Arduino.SDT_RIGHT, 255.0 * rc_SpeedAdjustTurn)
        elseif string.find(txt, "机器人停止") then
            rc_Arduino:Run(Arduino.SDT_NONE, 0)
        end

        PX2_VOICESDK:Speak(""..txt)
    end)

    UnRegistAllEventFunctions("VoiceSDKSpace::SpeakText")
    RegistEventFunction("VoiceSDKSpace::SpeakText", function(txt) 
    end)

    return uiFrame
end