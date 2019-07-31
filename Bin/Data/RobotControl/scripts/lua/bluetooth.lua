-- bluetooth.lua

rc_BleList = nil

function rc_BluetoothSerial()
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
    rc_BleList = list

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
    btnLeft:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/RobotControl/images/btnblue.png")
    btnLeft:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetPicBoxType(UIPicBox.PBT_NINE)
    local fText = btnLeft:CreateAddText(""..PX2_LM_APP:GetValue("Scan"))
    rc_UISetTextFont(fText:GetText(), 32)
    btnLeft:SetScriptHandler("rc_ButBluetoothSerialFrameCallabck")

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
	btnRight:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/RobotControl/images/btnblue.png")
	btnRight:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetPicBoxType(UIPicBox.PBT_NINE)
	local fText = btnRight:CreateAddText(""..PX2_LM_APP:GetValue("Connect"))
    rc_UISetTextFont(fText:GetText(), 32)
    btnRight:SetScriptHandler("rc_ButBluetoothSerialFrameCallabck")
    ZERONE_BleConnect = btnRight

    UnRegistAllEventFunctions("BluetoothES::OnDisocveryNewDevice")
    RegistEventFunction("BluetoothES::OnDisocveryNewDevice", function(deviceStr)
        local stk = StringTokenizer(deviceStr, "$")
        if stk:Count() >= 2 then
            local deviceItem = rc_BleList:GetItemByUserDataString("NamePath", deviceStr)
            if nil==deviceItem then
                local strName = stk:GetAt(0)
                local strAddress = stk:GetAt(1)
                local rssi = stk:GetAt(2)
                local rssi100 = PX2_BLUETOOTH:Rssi100(rssi).."/100"

                local useStrName = strName.."  "..strAddress.." "..PX2_LM_APP:GetValue("SignDegree")..rssi100

                local uiItem = rc_BleList:AddItem(useStrName)
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
            rc_OnStartUp()
        end
    end)

    UnRegistAllEventFunctions("BluetoothES::OnDisConnected")
    RegistEventFunction("BluetoothES::OnDisConnected", function()
        if nil~=ZERONE_BleConnect then
            ZERONE_BleConnect:GetText():SetText(""..PX2_LM_APP:GetValue("Connect"))
        end
    end)

    RegistEventFunction("SerialES::Open", function(tag) 
        if nil~=ZERONE_BleConnect then
            ZERONE_BleConnect:GetText():SetText(""..PX2_LM_APP:GetValue("DisConnect"))
            
            -- startup
            rc_OnStartUp()
        end
    end)

    RegistEventFunction("SerialES::Close", function(tag)
        if nil~=ZERONE_BleConnect then
            ZERONE_BleConnect:GetText():SetText(""..PX2_LM_APP:GetValue("Connect"))
        end
    end)

	return uiFrame
end

function rc_ButBluetoothSerialFrameCallabck(ptr, callType)
    local obj = Cast:ToO(ptr) 
	local name = obj:GetName()	
    
    local platType = PX2_APP:GetPlatformType()

    if UICT_PRESSED==callType then
        playFrameScale(obj)
    elseif UICT_RELEASED==callType then
        playFrameNormal(obj)

        if "BtnDlgLeft"==name then
            if Application.PLT_WINDOWS==platType or Application.PLT_LINUX==platType then
                rc_ScanSerialDevices()
            else
                rc_ScanBleDevices()
            end        
        elseif "BtnDlgRight"==name then
            if Application.PLT_WINDOWS==platType or Application.PLT_LINUX==platType then
                if not rc_Arduino:IsInitlized() then
                    rc_SerialTryToConnect()
                else
                    rc_Arduino:Terminate()
                end
            else
                if not PX2_BLUETOOTH:IsConnected() then
                    rc_BluetoothTryToConnect()
                else
                    PX2_BLUETOOTH:DisConnect()
                end
            end
        end
    elseif UICT_CHECKED == callType then
     
	end
end

function rc_ScanBleDevices()
    if nil~=rc_BleList then

        rc_BleList:RemoveAllItems()
        
        PX2_BLUETOOTH:GetPairedDevices()
        local numPairedDevices = PX2_BLUETOOTH:GetNumPairedDevices()
        for i=1, numPairedDevices, 1 do
            local deviceStr = PX2_BLUETOOTH:GetPairedDevice(i-1)
            local stk = StringTokenizer(deviceStr, "$")

            if stk:Count() >= 2 then
                local strName = stk:GetAt(0)
                local strAddress = stk:GetAt(1)

                local useStrName = strName

                local uiItem = rc_BleList:AddItem(useStrName)
                
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

function rc_BluetoothTryToConnect()
    local item = rc_BleList:GetSelectedItem()

    if nil~=item then
        local namePath = item:GetUserDataString("NamePath")
        PX2_LOGGER:LogInfo("RobotControl", "NamePath:"..namePath)
        if ""~=namePath then
            local stk = StringTokenizer(namePath, "$")
            if stk:Count() >= 2 then
                local strName = stk:GetAt(0)
                local strAddress = stk:GetAt(1)
                local rssi = stk:GetAt(2)

                if ""~=strAddress then
                    rc_BlueToConnect(strAddress)
                end
            end
        end
    end
end

function rc_BlueToConnect(strAddress)
    coroutine.wrap(function()
        PX2_LOGGER:LogInfo("RobotControl", "connect to"..strAddress)
        PX2_BLUETOOTH:Connect(strAddress)
    end)()
end

function rc_ScanSerialDevices()
    rc_BleList:RemoveAllItems()

    local serial = Serial()
    serial:GetPortList()
    local numPorts = serial:GetNumPorts()
    for i=0, numPorts-1 do
        local portStr = serial:GetPort(i)
        local item = rc_BleList:AddItem(portStr)
        item:SetUserDataString("NamePath", portStr)
    end  
end

function rc_SerialTryToConnect()
    local item = rc_BleList:GetSelectedItem()

    if nil~=item then
        local namePath = item:GetUserDataString("NamePath")
        PX2_LOGGER:LogInfo("RobotControl", "NamePath:"..namePath)        
        if ""~=namePath then
            rc_Arduino:Initlize(Arduino.M_SERIAL, namePath, 9600)
        end
    end
end

function rc_OnStartUp()
    -- Arduino
    rc_Arduino:VehicleInitMotoBoard4567()
    --rc_Arduino:VehicleSpeedInit(Arduino.P_2, Arduino.P_8, Arduino.P_3, Arduino.P_9)

    rc_Arduino:PinMode(Arduino.P_3, Arduino.PM_OUTPUT)
    rc_Arduino:PinMode(Arduino.P_8, Arduino.PM_OUTPUT)
    rc_Arduino:PinMode(Arduino.P_9, Arduino.PM_OUTPUT)
    rc_Arduino:PinMode(Arduino.P_13, Arduino.PM_OUTPUT)
    rc_Arduino:ServoInit(0, Arduino.P_A0)
    rc_Arduino:ServoInit(1, Arduino.P_A1)
end