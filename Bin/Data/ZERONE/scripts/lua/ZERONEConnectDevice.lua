-- ZERONEConnectDevice.lua

g_ZERONEList_Device = nil
g_ZERONE_DeviceConnect = nil
function zo_DeviceFrame()
    local uiFrame = UIFrame:New()

	uiFrame:SetAnchorHor(0.0, 1.0)
    uiFrame:SetAnchorVer(0.0, 1.0)
    
    local list = UIList:New("ListDevice")
    uiFrame:AttachChild(list)
    list.LocalTransform:SetTranslateY(-2.0)
    list:SetAnchorHor(0.0, 1.0)
    list:SetAnchorParamHor(50.0, -50.0)
    list:SetAnchorVer(0.0, 1.0)
    list:SetAnchorParamVer(100.0, -50.0)
    list:SetItemHeight(60.0)
    list:SetSliderSize(30.0)
    list:SetItemBackColor(Float3(0.7, 0.7, 0.7))
    list:SetReleasedDoSelect(true)
    list:SetFontSize(24)
    g_ZERONEList_Device = list

    local btnLeft = UIButton:New("BtnDlgLeft")
    uiFrame:AttachChild(btnLeft)
    btnLeft.LocalTransform:SetTranslateY(-1.0)
    btnLeft:SetSize(150, 50)
    btnLeft:SetAnchorHor(0.5, 0.5)
    btnLeft:SetAnchorParamHor(-120.0, -120.0)
    btnLeft:SetAnchorVer(0.0, 0.0)
    btnLeft:SetAnchorParamVer(60.0, 60.0)
    btnLeft:SetSize(180, 60)
    btnLeft:SetStateColor(UIButtonBase.BS_HOVERED, Float3:MakeColor(255, 255, 255))
    btnLeft:SetStateColor(UIButtonBase.BS_NORMAL, Float3:MakeColor(255, 255, 255))
    btnLeft:SetStateColor(UIButtonBase.BS_PRESSED, Float3:MakeColor(190, 190, 190))
    btnLeft:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/ZERONE/images/btnblue.png")
    btnLeft:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetPicBoxType(UIPicBox.PBT_NINE)
    local fText = btnLeft:CreateAddText(""..PX2_LM_APP:GetValue("Scan"))
    zo_UISetTextFont(fText:GetText(), 32)
    btnLeft:SetScriptHandler("zo_ButDeviceFrameCallabck")

	local btnRight = UIButton:New("BtnDlgRight")
	uiFrame:AttachChild(btnRight)
	btnRight.LocalTransform:SetTranslateY(-1.0)
	btnRight:SetSize(150, 50)
	btnRight:SetAnchorHor(0.5, 0.5)
	btnRight:SetAnchorParamHor(120.0, 120.0)
	btnRight:SetAnchorVer(0.0, 0.0)
	btnRight:SetAnchorParamVer(60.0, 60.0)
	btnRight:SetSize(180, 60)
	btnRight:SetStateColor(UIButtonBase.BS_HOVERED, Float3:MakeColor(255, 255, 255))
	btnRight:SetStateColor(UIButtonBase.BS_NORMAL, Float3:MakeColor(255, 255, 255))
	btnRight:SetStateColor(UIButtonBase.BS_PRESSED, Float3:MakeColor(190, 190, 190))
	btnRight:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/ZERONE/images/btnblue.png")
	btnRight:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetPicBoxType(UIPicBox.PBT_NINE)
    g_ZERONE_DeviceConnect = btnRight
    local fText = btnRight:CreateAddText(""..PX2_LM_APP:GetValue("Connect"))
    zo_UISetTextFont(fText:GetText(), 32)
    btnRight:SetScriptHandler("zo_ButDeviceFrameCallabck")

    return uiFrame
end


function zo_ButDeviceFrameCallabck(ptr, callType)
    local obj = Cast:ToO(ptr) 
	local name = obj:GetName()	
    
    local platType = PX2_APP:GetPlatformType()

    if UICT_PRESSED==callType then
        playFrameScale(obj)
    elseif UICT_RELEASED==callType then
        playFrameNormal(obj)

        if "BtnDlgLeft"==name then
            zo_RefreshDevices()
        elseif "BtnDlgRight"==name then
            local clientConnector = PX2_APP:GetEngineClientConnector()
            local isconnected = clientConnector:IsConnected()
            if isconnected then
                PX2_LOGGER:LogInfo("ZERONE", "isconnected:true")
                zo_DisconnectDevice()                
            else
                PX2_LOGGER:LogInfo("ZERONE", "isconnected:false")
                zo_ConnectDevice()
            end
        end
	end
end

UnRegistAllEventFunctions("EngineNetES::EngineClientUDPInfoChanged")
RegistEventFunction("EngineNetES::EngineClientUDPInfoChanged", function()
    zo_RefreshDevices()
end)

function zo_RefreshDevices()
    g_ZERONEList_Device:RemoveAllItems()

    local numInfos = PX2_APP:GetNumUDPNetInfo()
    for i=1,numInfos,1 do
        local netInfo = PX2_APP:GetUDPNetInfo(i-1)
        if nil~=netInfo then
            local name = netInfo:GetName()
            local ip = netInfo:GetIP()
            local allStr = name..":"..ip

            local uiItem = g_ZERONEList_Device:AddItem(allStr)
                
            uiItem:SetUserDataString("IP", ip)
        end
    end
end

function zo_OnDeviceConnect(connector)
    g_ZERONE_DeviceConnect:GetText():SetText(""..PX2_LM_APP:GetValue("DisConnect"))
end

function zo_OnDeviceDisconnect(connector)
    g_ZERONE_DeviceConnect:GetText():SetText(""..PX2_LM_APP:GetValue("Connect"))
end

function zo_ConnectDevice()
    local item = g_ZERONEList_Device:GetSelectedItem()

    if nil~=item then
        local ip = item:GetUserDataString("IP")

        local clientConnector = PX2_APP:GetEngineClientConnector()
        if nil~=clientConnector then
            clientConnector:AddOnConnectCallback("zo_OnDeviceConnect")
            clientConnector:AddOnDisconnectCallback("zo_OnDeviceDisconnect")
            clientConnector:ConnectB(ip, 9907)
            clientConnector:SetAutoConnect(false)
            clientConnector:SetAutoConnectIP(ip)
            clientConnector:SetAutoConnectPort(9907)

            rc_Arduino:InitlizeSocketTCP_Connector(clientConnector)
        end
    end
end

function  zo_DisconnectDevice()
    local clientConnector = PX2_APP:GetEngineClientConnector()
    if nil~=clientConnector then
        clientConnector:Disconnect()
    end
end
