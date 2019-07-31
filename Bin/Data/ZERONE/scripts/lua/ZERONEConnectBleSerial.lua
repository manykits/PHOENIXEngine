-- ZERONEConnectBleSerial.lua

ZERONE_IsRobotMotoWith298N = false
ZERONE_IsRobotMotoUseSpeed = true

function zo_ConnectBleSerial()
    local uiFrame = UIFrame:New()

	uiFrame:SetAnchorHor(0.0, 1.0)
	uiFrame:SetAnchorVer(0.0, 1.0)

    local list = UIList:New("ListBluetooth")
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
    ZERONE_BleList = list

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
    btnLeft:SetScriptHandler("zo_ButBluetoothSerialFrameCallabck")

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
	local fText = btnRight:CreateAddText(""..PX2_LM_APP:GetValue("Connect"))
    zo_UISetTextFont(fText:GetText(), 32)
    btnRight:SetScriptHandler("zo_ButBluetoothSerialFrameCallabck")
    ZERONE_BleConnect = btnRight

    local motoSpeedCheck = UICheckButton:New("MotoSpeedCheckButton")
    uiFrame:AttachChild(motoSpeedCheck)
	motoSpeedCheck.LocalTransform:SetTranslateY(-1.0)
	motoSpeedCheck:SetAnchorHor(1.0, 1.0)
	motoSpeedCheck:SetAnchorParamHor(-130.0, -130.0)
	motoSpeedCheck:SetAnchorVer(0.0, 0.0)
	motoSpeedCheck:SetAnchorParamVer(60.0, 60.0)
    motoSpeedCheck:SetSize(40, 40)
    local fText = motoSpeedCheck:CreateAddText(""..PX2_LM_APP:GetValue("encoder"))
    fText:GetText():SetFontColor(Float3.BLACK)
    fText:GetText():SetFontScale(0.75)
    motoSpeedCheck:SetScriptHandler("zo_ButBluetoothSerialFrameCallabck")
    motoSpeedCheck:Check(ZERONE_IsRobotMotoUseSpeed, false)

    local motoFireCheck = UICheckButton:New("FireCheckButton")
    uiFrame:AttachChild(motoFireCheck)
	motoFireCheck.LocalTransform:SetTranslateY(-1.0)
	motoFireCheck:SetAnchorHor(1.0, 1.0)
	motoFireCheck:SetAnchorParamHor(-80.0, -80.0)
	motoFireCheck:SetAnchorVer(0.0, 0.0)
	motoFireCheck:SetAnchorParamVer(60.0, 60.0)
    motoFireCheck:SetSize(40, 40)
    local fText = motoFireCheck:CreateAddText("Fire")
    fText:GetText():SetFontColor(Float3.BLACK)
    fText:GetText():SetFontScale(0.75)
    motoFireCheck:SetScriptHandler("zo_ButBluetoothSerialFrameCallabck")
    motoFireCheck:Check(false)
    motoFireCheck:Show(false)

    local motoInitCheck = UICheckButton:New("298NCheckButton")
    uiFrame:AttachChild(motoInitCheck)
	motoInitCheck.LocalTransform:SetTranslateY(-1.0)
	motoInitCheck:SetAnchorHor(1.0, 1.0)
	motoInitCheck:SetAnchorParamHor(-180.0, -180.0)
	motoInitCheck:SetAnchorVer(0.0, 0.0)
	motoInitCheck:SetAnchorParamVer(60.0, 60.0)
    motoInitCheck:SetSize(40, 40)
    local fText = motoInitCheck:CreateAddText(""..PX2_LM_APP:GetValue("298N"))
    fText:GetText():SetFontColor(Float3.BLACK)
    fText:GetText():SetFontScale(0.55)
    motoInitCheck:SetScriptHandler("zo_ButBluetoothSerialFrameCallabck")

    UnRegistAllEventFunctions("BluetoothES::OnDisocveryNewDevice")
    RegistEventFunction("BluetoothES::OnDisocveryNewDevice", function(deviceStr)
        local stk = StringTokenizer(deviceStr, "$")
        if stk:Count() >= 2 then
            local deviceItem = ZERONE_BleList:GetItemByUserDataString("NamePath", deviceStr)
            if nil==deviceItem then
                local strName = stk:GetAt(0)
                local strAddress = stk:GetAt(1)
                local rssi = stk:GetAt(2)
                local rssi100 = PX2_BLUETOOTH:Rssi100(rssi).."/100"

                local useStrName = strName.."  "..strAddress.." "..PX2_LM_APP:GetValue("SignDegree")..rssi100

                local uiItem = ZERONE_BleList:AddItem(useStrName)
                uiItem:SetUserDataString("NamePath", deviceStr)
            end
        end
    end)

    UnRegistAllEventFunctions("BluetoothES::OnConnected")
    RegistEventFunction("BluetoothES::OnConnected", function()
        if nil~=ZERONE_BleConnect then
            ZERONE_BleConnect:GetText():SetText(""..PX2_LM_APP:GetValue("DisConnect"))
          
            System:SleepSeconds(2.0)

            -- startup
            zo_OnStartUp(ZERONE_IsRobotMotoUseSpeed)
        end
    end)

    UnRegistAllEventFunctions("BluetoothES::OnDisConnected")
    RegistEventFunction("BluetoothES::OnDisConnected", function()
        if nil~=ZERONE_BleConnect then
            ZERONE_BleConnect:GetText():SetText(""..PX2_LM_APP:GetValue("Connect"))
        end
    end)

    RegistEventFunction("EngineNetES::OnEngineServerBeConnected", function(clientID, ip)
        zo_OnStartUp(ZERONE_IsRobotMotoUseSpeed)
    end)

    RegistEventFunction("SerialES::Open", function(tag)
        if 1==ZERONE_CurSerialOptType then
            if nil~=ZERONE_BleConnect then
                ZERONE_BleConnect:GetText():SetText(""..PX2_LM_APP:GetValue("DisConnect"))
                
                -- startup
                zo_OnStartUp(ZERONE_IsRobotMotoUseSpeed)
            end
        end
    end)

    RegistEventFunction("SerialES::Close", function(tag)
        if 1==ZERONE_CurSerialOptType then
            if nil~=ZERONE_BleConnect then
                ZERONE_BleConnect:GetText():SetText(""..PX2_LM_APP:GetValue("Connect"))
            end
        end
    end)

	return uiFrame
end

function zo_ScanBleDevices()
    if nil~=ZERONE_BleList then

        ZERONE_BleList:RemoveAllItems()
        
        PX2_BLUETOOTH:GetPairedDevices()
        local numPairedDevices = PX2_BLUETOOTH:GetNumPairedDevices()
        for i=1, numPairedDevices, 1 do
            local deviceStr = PX2_BLUETOOTH:GetPairedDevice(i-1)
            local stk = StringTokenizer(deviceStr, "$")

            if stk:Count() >= 2 then
                local strName = stk:GetAt(0)
                local strAddress = stk:GetAt(1)

                local useStrName = strName

                local uiItem = ZERONE_BleList:AddItem(useStrName)
                
                local text = UIFText:New()
                uiItem:AttachChild(text)
                text.LocalTransform:SetTranslateY(-1.0)
                text:SetAnchorHor(0.5, 1.0)
                text:SetAnchorVer(0.0, 1.0)
                text:GetText():SetFontColor(Float3.WHITE)
                text:GetText():SetText(""..PX2_LM_APP:GetValue("IsPaired"))

                uiItem:SetUserDataString("NamePath", deviceStr)
            end
        end
    end
    PX2_BLUETOOTH:DoDiscovery()
end

function zo_bluetoothTryToConnect()
    local item = ZERONE_BleList:GetSelectedItem()

    if nil~=item then
        local namePath = item:GetUserDataString("NamePath")
        PX2_LOGGER:LogInfo("ZERONE", "NamePath:"..namePath)
        if ""~=namePath then
            local stk = StringTokenizer(namePath, "$")
            if stk:Count() >= 2 then
                local strName = stk:GetAt(0)
                local strAddress = stk:GetAt(1)
                local rssi = stk:GetAt(2)

                if ""~=strAddress then
                    zo_blueToConnect(strAddress)
                end
            end
        end
    end
end

function zo_blueToConnect(strAddress)
    coroutine.wrap(function()
        PX2_LOGGER:LogInfo("ZERONE", "connect to"..strAddress)
        PX2_BLUETOOTH:Connect(strAddress)
    end)()
end

function zo_ScanSerialDevices()
    ZERONE_BleList:RemoveAllItems()

    local serial = Serial()
    serial:UpdatePortList()
    local numPorts = serial:GetNumPorts()
    for i=0, numPorts-1 do
        local portStr = serial:GetPort(i)
        local portDesc = serial:GetPortDesc(i)
        local hardID = serial:GetPortHardID(i)
        local item = ZERONE_BleList:AddItem(portStr.."-"..portDesc.."-"..hardID)
        item:SetUserDataString("NamePath", portStr)
    end  
end

function zo_SerialTryToConnect()
    local item = ZERONE_BleList:GetSelectedItem()

    if nil~=item then
        local namePath = item:GetUserDataString("NamePath")
        PX2_LOGGER:LogInfo("ZERONE", "NamePath:"..namePath)        
        if ""~=namePath then
            ZERONE_CurSerialOptType = 1
            rc_Arduino:Initlize(Arduino.M_SERIAL, namePath, 9600)
        end
    end
end

function zo_ButBluetoothSerialFrameCallabck(ptr, callType)
    local obj = Cast:ToO(ptr) 
	local name = obj:GetName()	
    
    local platType = PX2_APP:GetPlatformType()

    if UICT_PRESSED==callType then
        playFrameScale(obj)
    elseif UICT_RELEASED==callType then
        playFrameNormal(obj)

        if "BtnDlgLeft"==name then
            if Application.PLT_WINDOWS==platType or Application.PLT_LINUX==platType then
                zo_ScanSerialDevices()
            else
                zo_ScanBleDevices()
            end        
        elseif "BtnDlgRight"==name then
            ZERONE_CurSerialOptType = 1
            if Application.PLT_WINDOWS==platType or Application.PLT_LINUX==platType then
                if not rc_Arduino:IsInitlized() then
                    zo_SerialTryToConnect()
                else
                    rc_Arduino:Terminate()
                end
            else
                if not PX2_BLUETOOTH:IsConnected() then
                    zo_bluetoothTryToConnect()
                else
                    PX2_BLUETOOTH:DisConnect()
                end
            end
        end
    elseif UICT_CHECKED == callType then
        if "298NCheckButton"==name then
            ZERONE_IsRobotMotoWith298N = true
            if rc_Arduino:IsInitlized() then
                rc_Arduino:VehicleInitMotoBoard298N(Arduino.P_A0, Arduino.P_A1, Arduino.P_10, Arduino.P_A2, Arduino.P_A3, Arduino.P_11)
                if ZERONE_IsRobotMotoUseSpeed then
                    rc_Arduino:VehicleSpeedInit(Arduino.P_2, Arduino.P_8, Arduino.P_3, Arduino.P_9)
                end
            end
        elseif "MotoSpeedCheckButton"==name then
            ZERONE_IsRobotMotoUseSpeed = true
            if rc_Arduino:IsInitlized() then
                if ZERONE_IsRobotMotoWith298N then
                    rc_Arduino:VehicleSpeedInit(Arduino.P_2, Arduino.P_8, Arduino.P_3, Arduino.P_9)
                else
                    rc_Arduino:VehicleSpeedInit(Arduino.P_2, Arduino.P_8, Arduino.P_3, Arduino.P_9)
                end
            end
        elseif "FireCheckButton"==name then
            ZERONE_IsFireRobot = true
            rc_Arduino:PinMode(Arduino.P_10, Arduino.PM_OUTPUT)
            rc_Arduino:PinMode(Arduino.P_11, Arduino.PM_OUTPUT)
            rc_Arduino:PinMode(Arduino.P_12, Arduino.PM_OUTPUT)
            rc_Arduino:PinMode(Arduino.P_13, Arduino.PM_OUTPUT)
            rc_Arduino:ServoInit(0, Arduino.P_A0)
            rc_Arduino:ServoInit(1, Arduino.P_A1)
        end
    elseif UICT_DISCHECKED == callType then
        if "298NCheckButton"==name then
            ZERONE_IsRobotMotoWith298N = false
            if rc_Arduino:IsInitlized() then
                rc_Arduino:VehicleInitMotoBoard4567()
                if ZERONE_IsRobotMotoUseSpeed then
                    rc_Arduino:VehicleSpeedInit(Arduino.P_2, Arduino.P_8, Arduino.P_3, Arduino.P_9)
                end
            end
        elseif "MotoSpeedCheckButton"==name then
            ZERONE_IsRobotMotoUseSpeed = false
            rc_Arduino:VehicleSpeedInit(Arduino.P_0, Arduino.P_0, Arduino.P_0, Arduino.P_0) -- ternimate speed
        elseif "FireCheckButton"==name then
            ZERONE_IsFireRobot = false
        end
	end
end