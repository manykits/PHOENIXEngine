--play.lua

require("Data/AIDoor/scripts/lua/doorframe.lua")
require("Data/AIDoor/scripts/lua/settingframe.lua")

aidoor_clientconnsctor = nil
aidoor_connectip = ""
aidoor_connectport = 6601
aidoor_connectTime = 0.0
aidoor_isconnected = false

aidoor_statustext = nil
aidoor_editbox_address = nil
aidoor_editbox_username = nil
aidoor_editbox_key = nil

aidoor_address = ""
aidoor_username = ""
aidoor_key = ""

function engine_project_preplay() 
    PX2_APP:SetShowInfo(true)

    PX2_ENGINECANVAS:SetClearColor(Float4:MakeColor(63, 72, 204, 255))
    PX2_ENGINESCENECANVAS:SetClearColor(Float4:MakeColor(0, 0, 0, 255))
end

function engine_project_play() 
    aidoor_Play();
end

function engine_project_update(appseconds, elapsedseconds) 
    aidoor_connectTime = aidoor_connectTime + elapsedseconds
    if aidoor_connectTime > 4.0 then
        aidoor_isconnected = false
        aidoor_statustext:GetText():SetText("已和服务器断开连接")

        if aidoor_clientconnsctor:IsConnected() then
            aidoor_clientconnsctor:Disconnect()
        end
        if ""~=aidoor_connectip then
            aidoor_clientconnsctor:ConnectNB(aidoor_connectip, aidoor_connectport)
        end
        aidoor_connectTime = 0.0
    end
end

function engine_project_cmd(cmd, param0, param1, param2) 
end

function aidoor_Play()
    aidoor_connectip = PX2_PROJ:GetConfig("ip")
    aidoor_address = PX2_PROJ:GetConfig("address")
    aidoor_username = PX2_PROJ:GetConfig("username")
    aidoor_key = PX2_PROJ:GetConfig("key")

    PX2_LOGGER:LogInfo("ip", aidoor_connectip)
    PX2_LOGGER:LogInfo("address", aidoor_address)
    PX2_LOGGER:LogInfo("username", txtusername)
    PX2_LOGGER:LogInfo("key", txtkey)


    local ui = PX2_PROJ:GetUI()
    local frameAIDoor = UIFrame:New()
    ui:AttachChild(frameAIDoor)
    frameAIDoor:SetAnchorHor(0.0, 1.0)
    frameAIDoor:SetAnchorVer(0.0, 1.0)

    local tabFrame = UITabFrame:New()
    frameAIDoor:AttachChild(tabFrame)
    tabFrame:SetAnchorHor(0.0, 1.0)
    tabFrame:SetAnchorVer(0.0, 1.0)
    tabFrame:SetTabBarHeight(60.0)
    tabFrame:SetTabHeight(50.0)
    tabFrame:SetTabWidth(200.0)

    local frameDoor = ad_createDoorFrame()
    tabFrame:AddTab("Door", "电动门", frameDoor)
    frameDoor:SetAnchorHor(0.0, 1.0)
    frameDoor:SetAnchorVer(0.0, 1.0)

    local frameSetting = ad_createSettingFrame()
    tabFrame:AddTab("Setting", "设置", frameSetting)
    frameSetting:SetAnchorHor(0.0, 1.0)
    frameSetting:SetAnchorVer(0.0, 1.0)

    local btnDoor = tabFrame:GetTabButton("Door");
    btnDoor:GetText():SetFontSize(24)
    btnDoor:GetText():SetFontColor(Float3.WHITE)

    local btnSetting = tabFrame:GetTabButton("Setting");
    btnSetting:GetText():SetFontSize(24)
    btnSetting:GetText():SetFontColor(Float3.WHITE)

    tabFrame:SetActiveTab("Door")

    PX2_APP:ShutdownGeneralClientConnector("aidoor")
    aidoor_clientconnsctor = PX2_APP:CreateGetGeneralClientConnector("aidoor")
    
    if ""~=aidoor_address and ""==aidoor_connectip then
        local ip = Server:GetIP(""..aidoor_address)
        aidoor_connectip = ip
        if ""~=aidoor_connectip then
            PX2_PROJ:SetConfig("ip", aidoor_connectip);
        end
    end

    if ""~=aidoor_connectip then
        aidoor_clientconnsctor:ConnectNB(aidoor_connectip, aidoor_connectport)
    end
    aidoor_clientconnsctor:AddOnConnectCallback("aidoor_clientconnectOnConnect")
    aidoor_clientconnsctor:AddOnDisconnectCallback("aidoor_clientconnectOnDisConnect")
    aidoor_clientconnsctor:AddOnRecvCallback("aidoor_clientconnectOnReceive")
end

function aidoor_clientconnectOnConnect(ptr)
    local cnt = Cast:ToGCC(ptr)

    aidoor_isconnected = true

    local sendStr = "app connect "..aidoor_username.." "..aidoor_key
    cnt:SendString(sendStr)
end

function aidoor_clientconnectOnDisConnect(ptr)
    local cnt = Cast:ToGCC(ptr)

    aidoor_isconnected = false
end


function aidoor_clientconnectOnReceive(ptr, dataStr)
    local cnt = Cast:ToGCC(ptr)

    if "aiotheart" == dataStr then
        aidoor_connectTime = 0.0
        aidoor_isconnected = true
    else
        local param0 = "";
        local param1 = "";
        local param2 = "";
        local param3 = "";
        local param4 = "";
        local param5 = "";

        local stk = StringTokenizer(dataStr, " ")
        if stk:Count() > 0 then
            param0 = stk:GetAt(0)
        end
        if stk:Count() > 1 then
            param1 = stk:GetAt(1)
        end
        if stk:Count() > 2 then
            param2 = stk:GetAt(2)
        end
        if stk:Count() > 3 then
            param3 = stk:GetAt(3)
        end
        if stk:Count() > 4 then
            param4 = stk:GetAt(4)
        end
        if stk:Count() > 5 then
            param5 = stk:GetAt(5)
        end

        if "status" == param0 then
            local statutext = ""
            if "true" == param1 then
                statutext = statutext.."有遮挡".."  "
            else
                statutext = statutext.."无遮挡".."  "
            end

            local param22 = StringHelp:FloatToString(0.0 + param2)
            statutext = statutext.."开门比:"..param22.." "

            if "0"==param3 then
                statutext = statutext.."系统已关闭"
            elseif "1"==param3 then
                if "0" == param4 then
                    statutext = statutext.."关闭"
                elseif "1" ==param4 then
                    statutext = statutext.."开门中"
                elseif "2" ==param4 then
                    statutext = statutext.."等待关门"
                elseif "3" ==param4 then
                    statutext = statutext.."关门中"
                end
            elseif "2"==param3 then
                statutext = statutext.."系统出错"
                statutext = statutext.."错误码"..param5
            end

            aidoor_statustext:GetText():SetText(statutext)
        end
    end
end

function aidoor_UICallabck(ptr, callType)
    local obj = Cast:ToO(ptr) 
	local name = obj:GetName()	

    if UICT_PRESSED==callType then
        playFrameScale(obj)
        
	elseif UICT_RELEASED==callType then
        playFrameNormal(obj)

        if "systemon" == name then
            aidoor_clientconnsctor:SendString("app systemon");
        elseif "systemoff" == name then
            aidoor_clientconnsctor:SendString("app systemoff");
        elseif "sidedooropen" == name then
            aidoor_clientconnsctor:SendString("app sidedooropen");
        elseif "dooropen" == name then
            aidoor_clientconnsctor:SendString("app dooropen");
        elseif "doorstop" == name then
            aidoor_clientconnsctor:SendString("app doorstop");
        elseif "doorclose" == name then
            aidoor_clientconnsctor:SendString("app doorclose");
        elseif "doorOpenLiense" == name then
            aidoor_clientconnsctor:SendString("app doorOpenLiense");
        elseif "btnsave" == name then
            aidoor_saveConfig()
        end
    elseif UICT_CHECKED==callType then  
    elseif UICT_DISCHECKED==callType then
    elseif UICT_RELEASED_NOTPICK==callType then
        playFrameNormal(obj)
    elseif UICT_ROUND_RELEASEDNOTPICK==callType then
    elseif UICT_COMBOBOX_CHOOSED==callType then 
	end
end