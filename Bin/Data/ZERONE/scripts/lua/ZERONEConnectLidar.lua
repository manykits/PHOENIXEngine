-- ZERONEConnectLidar.lua

ZERONEText_SerialLidarConnect = nil
ZERONESerialList_Lidar = nil
ZERONEComboBox_LidarType = nil
function zo_ConnectLidar()
    local uiFrame = UIFrame:New()

    uiFrame:SetAnchorHor(0.0, 1.0)
    uiFrame:SetAnchorVer(0.0, 1.0)
    
    local comboxBoxLidarType = UIComboBox:New("BtnComboxBoxLidarType")
    uiFrame:AttachChild(comboxBoxLidarType)
    ZERONEComboBox_LidarType = comboxBoxLidarType
    comboxBoxLidarType.LocalTransform:SetTranslateY(-8.0)
    comboxBoxLidarType:AddChooseStr("3i_old")
    comboxBoxLidarType:AddChooseStr("3i")
    comboxBoxLidarType:AddChooseStr("RP")
    comboxBoxLidarType:AddChooseStr("WR")
    comboxBoxLidarType:SetChooseListHeightSameWithChooses()
    comboxBoxLidarType:SetAnchorHor(0.0, 0.0)
    comboxBoxLidarType:SetAnchorVer(1.0, 1.0)
    comboxBoxLidarType:SetAnchorParamHor(90.0, 90.0)
    comboxBoxLidarType:SetAnchorParamVer(-20.0, -20.0)
    comboxBoxLidarType:SetSize(120.0, 30.0)
    comboxBoxLidarType:SetPivot(0.0, 0.5)
    comboxBoxLidarType:SetScriptHandler("zo_ButFrameCallabck_Lidar")
    comboxBoxLidarType:Choose(0)

    local list = UIList:New("ListLidar")
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
    ZERONESerialList_Lidar = list

    local btnSaveMap = UIButton:New("BtnSaveMap")
    uiFrame:AttachChild(btnSaveMap)
    btnSaveMap.LocalTransform:SetTranslateY(-1.0)
    btnSaveMap:SetSize(150, 50)
    btnSaveMap:SetAnchorHor(0.5, 0.5)
    btnSaveMap:SetAnchorParamHor(-420.0, -420.0)
    btnSaveMap:SetAnchorVer(0.0, 0.0)
    btnSaveMap:SetAnchorParamVer(60.0, 60.0)
    btnSaveMap:SetSize(60, 60)
    btnSaveMap:SetStateColor(UIButtonBase.BS_HOVERED, Float3:MakeColor(255, 255, 255))
    btnSaveMap:SetStateColor(UIButtonBase.BS_NORMAL, Float3:MakeColor(255, 255, 255))
    btnSaveMap:SetStateColor(UIButtonBase.BS_PRESSED, Float3:MakeColor(190, 190, 190))
    btnSaveMap:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/ZERONE/images/btnblue.png")
    btnSaveMap:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetPicBoxType(UIPicBox.PBT_NINE)
    local fText = btnSaveMap:CreateAddText(""..PX2_LM_APP:GetValue("SaveMap"))
    zo_UISetTextFont(fText:GetText(), 32)
    btnSaveMap:SetScriptHandler("zo_ButFrameCallabck_Lidar")

    local btnLoadMap = UIButton:New("BtnLoadMap")
    uiFrame:AttachChild(btnLoadMap)
    btnLoadMap.LocalTransform:SetTranslateY(-1.0)
    btnLoadMap:SetSize(150, 50)
    btnLoadMap:SetAnchorHor(0.5, 0.5)
    btnLoadMap:SetAnchorParamHor(-340.0, -340.0)
    btnLoadMap:SetAnchorVer(0.0, 0.0)
    btnLoadMap:SetAnchorParamVer(60.0, 60.0)
    btnLoadMap:SetSize(60, 60)
    btnLoadMap:SetStateColor(UIButtonBase.BS_HOVERED, Float3:MakeColor(255, 255, 255))
    btnLoadMap:SetStateColor(UIButtonBase.BS_NORMAL, Float3:MakeColor(255, 255, 255))
    btnLoadMap:SetStateColor(UIButtonBase.BS_PRESSED, Float3:MakeColor(190, 190, 190))
    btnLoadMap:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/ZERONE/images/btnblue.png")
    btnLoadMap:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetPicBoxType(UIPicBox.PBT_NINE)
    local fText = btnLoadMap:CreateAddText(""..PX2_LM_APP:GetValue("LoadMap"))
    zo_UISetTextFont(fText:GetText(), 32)
    btnLoadMap:SetScriptHandler("zo_ButFrameCallabck_Lidar")

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
    btnLeft:SetScriptHandler("zo_ButFrameCallabck_Lidar")

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
    btnRight:SetScriptHandler("zo_ButFrameCallabck_Lidar")
    ZERONEText_SerialLidarConnect = fText

    local slamCheck = UICheckButton:New("SlamCheckButton")
    uiFrame:AttachChild(slamCheck)
	slamCheck.LocalTransform:SetTranslateY(-1.0)
	slamCheck:SetAnchorHor(1.0, 1.0)
	slamCheck:SetAnchorParamHor(-180.0, -180.0)
	slamCheck:SetAnchorVer(0.0, 0.0)
	slamCheck:SetAnchorParamVer(60.0, 60.0)
    slamCheck:SetSize(40, 40)
    local fText = slamCheck:CreateAddText("" .. PX2_LM_APP:GetValue("MapUpdate"))
    fText:GetText():SetFontColor(Float3.BLACK)
    fText:GetText():SetFontScale(0.50)
    slamCheck:SetScriptHandler("zo_ButFrameCallabck_Lidar")
    slamCheck:Check(true, false)

    RegistEventFunction("AIES::LiDarOpen", function(tag)
        if 2==ZERONE_CurSerialOptType then
            ZERONEText_SerialLidarConnect:GetText():SetText(""..PX2_LM_APP:GetValue("DisConnect"))
        end 
    end)

    RegistEventFunction("AIES::LiDarClose", function(tag)
        if 2==ZERONE_CurSerialOptType then
            ZERONEText_SerialLidarConnect:GetText():SetText(""..PX2_LM_APP:GetValue("Connect"))
        end
    end)

    return uiFrame
end

function zo_ScanSerialDevices_Lidar()
    ZERONESerialList_Lidar:RemoveAllItems()

    local serial = Serial()
    serial:UpdatePortList()
    local numPorts = serial:GetNumPorts()
    for i=0, numPorts-1 do
        local portStr = serial:GetPort(i)
        local portDesc = serial:GetPortDesc(i)
        local hardID = serial:GetPortHardID(i)
        local item = ZERONESerialList_Lidar:AddItem(portStr.."—"..portDesc.."-"..hardID)
        item:SetUserDataString("NamePath", portStr)
    end  
end

function zo_SerialTryToConnect_Lidar()
    local lidarType = rc_Robot:GetLidar():GetLiDarType()

    if LiDar.LT_SICK ==lidarType then
        rc_Robot:LidarOpen("169.254.177.161", 2111)
    elseif LiDar.LT_WR == lidarType then
        rc_Robot:LidarOpen("192.168.1.100", 2112)
    else
        local item = ZERONESerialList_Lidar:GetSelectedItem()
                    
        if nil~=item then
            local namePath = item:GetUserDataString("NamePath")
            PX2_LOGGER:LogInfo("PHOENIXCode", "NamePath:"..namePath)
            if ""~=namePath then
                if LiDar.LT_III==lidarType or LiDar.LT_RP==lidarType then
                    if LiDar.LT_III ==lidarType then
                        local chooseStr = ZERONEComboBox_LidarType:GetChooseStr()
                        if "3i_old" == chooseStr then
                            rc_Robot:LidarOpen(namePath, 115200)
                        else
                            rc_Robot:LidarOpen(namePath, 230400)
                        end
                    else
                        rc_Robot:LidarOpen(namePath, 115200)
                    end
                end 
            end
        end
    end
end

function zo_ButFrameCallabck_Lidar(ptr, callType)
    local obj = Cast:ToO(ptr) 
	local name = obj:GetName()	
    
    local platType = PX2_APP:GetPlatformType()

    if UICT_PRESSED==callType then
        playFrameScale(obj)
    elseif UICT_RELEASED==callType then
        playFrameNormal(obj)

        if "BtnDlgLeft"==name then
            if Application.PLT_WINDOWS==platType or Application.PLT_LINUX==platType then
                zo_ScanSerialDevices_Lidar()
            else
            end        
        elseif "BtnDlgRight"==name then
            ZERONE_CurSerialOptType = 2
            if Application.PLT_WINDOWS==platType or Application.PLT_LINUX==platType then
                if nil~=rc_Robot:GetLidar() then
                    if rc_Robot:GetLidar():IsOpened() then
                        rc_Robot:GetLidar():Close()
                    else
                        zo_SerialTryToConnect_Lidar()
                    end
                end
            else
            end
        elseif "BtnSaveMap"==name then
            rc_Robot:SaveMap("map.px2obj")
        elseif "BtnLoadMap"==name then
            rc_Robot:LoadMap("map.px2obj", APoint(0.0, 0.0, 0.001), 0.001)
        end
    elseif UICT_CHECKED == callType then
        if "SlamCheckButton"==name then
            rc_Robot:SetSlamMapUpdate(true)
        end
    elseif UICT_DISCHECKED == callType then
        if "SlamCheckButton"==name then
            rc_Robot:SetSlamMapUpdate(false)
        end
    elseif UICT_COMBOBOX_CHOOSED == callType then
        if "BtnComboxBoxLidarType"==name then
            local chooseStr = ZERONEComboBox_LidarType:GetChooseStr()
            if "3i_old" == chooseStr then
                rc_Robot:GetLidar():SetLiDarType(LiDar.LT_III)
            elseif "3i" == chooseStr then
                rc_Robot:GetLidar():SetLiDarType(LiDar.LT_III)
            elseif "RP"==chooseStr then
                rc_Robot:GetLidar():SetLiDarType(LiDar.LT_RP)    
            elseif "WR"==chooseStr then
                rc_Robot:GetLidar():SetLiDarType(LiDar.LT_WR)
            end
        end
	end
end