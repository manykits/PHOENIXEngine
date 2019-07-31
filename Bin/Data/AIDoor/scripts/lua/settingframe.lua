-- settingframe.lua

function ad_createSettingFrame()
    local frameDoor = UIFrame:New()
    frameDoor:SetAnchorHor(0.0, 1.0)
    frameDoor:SetAnchorVer(0.0, 1.0)

    local editboxAddress = UIEditBox:New()
    aidoor_editbox_address = editboxAddress
    frameDoor:AttachChild(editboxAddress)
    editboxAddress:SetAnchorHor(0.0, 1.0)
    editboxAddress:SetAnchorVer(1.0, 1.0)
    editboxAddress:SetHeight(80.0)
    editboxAddress:SetAnchorParamVer(-80.0, -80.0)
    editboxAddress:SetAnchorParamHor(200.0, -20.0)
    editboxAddress:SetText(aidoor_address)

    local fText = UIFText:New()
    frameDoor:AttachChild(fText)
    fText.LocalTransform:SetTranslateY(-1.0)
    fText:SetAnchorHor(0.0, 0.0)
    fText:SetAnchorVer(1.0, 1.0)
    fText:SetAnchorParamVer(-80.0, -80.0)
    fText:SetAnchorParamHor(0.0, 0.0)
    fText:SetPivot(0.0, 0.5)
    fText:SetWidth(200.0)
    fText:GetText():SetFontColor(Float3.WHITE)
    fText:GetText():SetFontSize(24)
    fText:GetText():SetText("地址")

    local editboxName = UIEditBox:New()
    aidoor_editbox_username = editboxName
    frameDoor:AttachChild(editboxName)
    editboxName:SetAnchorHor(0.0, 1.0)
    editboxName:SetAnchorVer(1.0, 1.0)
    editboxName:SetHeight(80.0)
    editboxName:SetAnchorParamVer(-200.0, -200.0)
    editboxName:SetAnchorParamHor(200.0, -20.0)
    editboxName:SetText(aidoor_username)

    local fText = UIFText:New()
    frameDoor:AttachChild(fText)
    fText.LocalTransform:SetTranslateY(-1.0)
    fText:SetAnchorHor(0.0, 0.0)
    fText:SetAnchorVer(1.0, 1.0)
    fText:SetAnchorParamVer(-200.0, -200.0)
    fText:SetAnchorParamHor(0.0, 0.0)
    fText:SetPivot(0.0, 0.5)
    fText:SetWidth(200.0)
    fText:GetText():SetFontColor(Float3.WHITE)
    fText:GetText():SetFontSize(24)
    fText:GetText():SetText("账号")

    local editboxKey = UIEditBox:New()
    aidoor_editbox_key = editboxKey
    frameDoor:AttachChild(editboxKey)
    editboxKey.LocalTransform:SetTranslateY(-1.0)
    editboxKey:SetAnchorHor(0.0, 1.0)
    editboxKey:SetAnchorVer(1.0, 1.0)
    editboxKey:SetHeight(80.0)
    editboxKey:SetAnchorParamVer(-320.0, -320.0)
    editboxKey:SetAnchorParamHor(200.0, -20.0)
    editboxKey:SetPassword(true)
    editboxKey:SetText(aidoor_key)

    local fText1 = UIFText:New()
    frameDoor:AttachChild(fText1)
    fText1.LocalTransform:SetTranslateY(-1.0)
    fText1:SetAnchorHor(0.0, 0.0)
    fText1:SetAnchorVer(1.0, 1.0)
    fText1:SetAnchorParamVer(-320.0, -320.0)
    fText1:SetAnchorParamHor(0.0, 0.0)
    fText1:SetPivot(0.0, 0.5)
    fText1:SetWidth(200.0)
    fText1:GetText():SetFontColor(Float3.WHITE)
    fText1:GetText():SetFontSize(24)
    fText1:GetText():SetText("密钥")

    local btnSave = UIButton:New("btnsave")
    frameDoor:AttachChild(btnSave)
    btnSave:SetAnchorHor(0.0, 1.0)
    btnSave:SetAnchorVer(1.0, 1.0)
    btnSave:SetAnchorParamHor(20.0, -20.0)
    btnSave:SetAnchorParamVer(-440.0, -440.0)
    btnSave:SetHeight(80.0)
    local txt = btnSave:CreateAddText("保存")
    txt:GetText():SetFontSize(24)
    btnSave:SetScriptHandler("aidoor_UICallabck")
    btnSave:SetScriptHandler("aidoor_UICallabck")

    return frameDoor
end

function aidoor_saveConfig()
    aidoor_address = aidoor_editbox_address:GetText()
    aidoor_username = aidoor_editbox_username:GetText()
    aidoor_key = aidoor_editbox_key:GetRealText()

    PX2_PROJ:SetConfig("address", aidoor_address);
    PX2_PROJ:SetConfig("username", aidoor_username);
    PX2_PROJ:SetConfig("key", aidoor_key);

    PX2_LOGGER:LogInfo("address", aidoor_address)
    PX2_LOGGER:LogInfo("username", txtusername)
    PX2_LOGGER:LogInfo("key", txtkey)

    aidoor_connectTime = 0.0
    if aidoor_clientconnsctor:IsConnected() then
        aidoor_clientconnsctor:Disconnect()
    end

    if ""~=aidoor_address then
        local ip = Server:GetIP(""..aidoor_address)
        aidoor_connectip = ip
        PX2_PROJ:SetConfig("ip", aidoor_connectip);
    end
    if ""~=aidoor_connectip then
        aidoor_clientconnsctor:ConnectNB(aidoor_connectip, aidoor_connectport)
    end
end