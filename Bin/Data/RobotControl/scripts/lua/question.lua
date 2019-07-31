-- question.lua

function rc_Question()
    local uiFrameBack, uiFrame, btnClose, titleText = rc_UICreateDlgCommon(540.0, 320.0, "")

    uiFrame.LocalTransform:SetTranslateY(-5.0)
    uiFrame:SetAnchorHor(0.0, 1.0)
    uiFrame:SetAnchorVer(0.0, 1.0)
    uiFrame:Show(true)
    btnClose:SetScriptHandler("rc_UICallabck")

    uiFrame:SetAnchorParamHor(50.0, -50.0)
    uiFrame:SetAnchorParamVer(50.0, -50.0)

    local picBox = UIFPicBox:New("tex")
    uiFrame:AttachChild(picBox)
    picBox.LocalTransform:SetTranslateY(-1.0)
    picBox:GetUIPicBox():SetTexture("Data/RobotControl/images/question.png")
    picBox:AutoMakeSizeFixable()

    return uiFrameBack
end