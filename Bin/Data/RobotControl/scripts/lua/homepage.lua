-- homepage.lua

function rc_FrameHomePage()
    local frameAll = UIFrame:New()
    frameAll:SetAnchorHor(0.0, 1.0)
    frameAll:SetAnchorVer(0.0, 1.0)
    frameAll:SetAnchorParamHor(0.0, 0.0)
    frameAll:SetAnchorParamVer(0.0, 0.0)

    local btnCtrl = UIButton:New("BtnCtrl")
    frameAll:AttachChild(btnCtrl)
    btnCtrl.LocalTransform:SetTranslateY(-1.0)
    btnCtrl:SetAnchorHor(0.25, 0.25)
    btnCtrl:SetSize(180.0, 180.0)
    btnCtrl:SetStateColorDefaultWhite()
    local text = btnCtrl:CreateAddText(""..PX2_LM_APP:GetValue("Control"))
    text:SetAnchorParamVer(-120.0, -120.0)
    btnCtrl:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/RobotControl/images/control.png")
    btnCtrl:SetScriptHandler("rc_UICallabck")

    local btnShake = UIButton:New("BtnAxis")
    frameAll:AttachChild(btnShake)
    btnShake.LocalTransform:SetTranslateY(-1.0)
    btnShake:SetAnchorHor(0.5, 0.5)
    btnShake:SetSize(180.0, 180.0)
    btnShake:SetStateColorDefaultWhite()
    local text = btnShake:CreateAddText(""..PX2_LM_APP:GetValue("Axis"))
    text:SetAnchorParamVer(-120.0, -120.0)
    btnShake:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/RobotControl/images/accel.png")
    btnShake:SetScriptHandler("rc_UICallabck")

    local btnVoice = UIButton:New("BtnVoice")
    frameAll:AttachChild(btnVoice)
    btnVoice.LocalTransform:SetTranslateY(-1.0)
    btnVoice:SetAnchorHor(0.75, 0.75)
    btnVoice:SetSize(180.0, 180.0)
    btnVoice:SetStateColorDefaultWhite()
    local text = btnVoice:CreateAddText(""..PX2_LM_APP:GetValue("Voice"))
    text:SetAnchorParamVer(-120.0, -120.0)
    btnVoice:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/RobotControl/images/voice1.png")
    btnVoice:SetScriptHandler("rc_UICallabck")

    return frameAll
end