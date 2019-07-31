-- connect.lua

function rc_Connect()
    local uiFrameBack, uiFrame, btnClose, titleText = rc_UICreateDlgCommon(540.0, 320.0, "")

    uiFrame.LocalTransform:SetTranslateY(-5.0)
    uiFrame:SetAnchorHor(0.0, 1.0)
    uiFrame:SetAnchorVer(0.0, 1.0)
    uiFrame:Show(true)
    btnClose:SetScriptHandler("rc_UICallabck")

    uiFrame:SetAnchorParamHor(50.0, -50.0)
    uiFrame:SetAnchorParamVer(50.0, -50.0)

    -- tab frame
    local frameTab = UITabFrame:New()
    uiFrame:AttachChild(frameTab)
    frameTab.LocalTransform:SetTranslateY(-1.0)
    frameTab:SetAnchorHor(0.0, 1.0)
    frameTab:SetAnchorVer(0.0, 1.0)
    frameTab:SetAnchorParamHor(20.0, -20.0)
    frameTab:SetAnchorParamVer(20.0, -60.0)
    frameTab:SetTabBarHeight(42.0)
    frameTab:SetTabHeight(42.0)
    frameTab:SetFontColor(Float3.WHITE)

    local frameBle = rc_BluetoothSerial()
    frameTab:AddTab("BleSerConnect", PX2_LM_APP:GetValue("BleSerConnect"), frameBle)

    local txt = frameTab:GetTabButton("BleSerConnect"):GetText()
    rc_UISetTextFont(txt, 16)

    frameTab:SetActiveTab("BleSerConnect")

    return uiFrameBack
end