-- ZERONEServer.lua

ZERONE_UDPServer = nil
function zo_CreateUDPServer()
    if nil~=ZERONE_UDPServer then
        return
    end
    
    local udpServer = UDPServer:New()
    ZERONE_UDPServer = udpServer
    PX2_PROJ:PoolSet("DUPServer", udpServer)
    udpServer:Bind(11139)   
    udpServer:AddOnRecvCallback("zo_UDPServerCallback")
    udpServer:Start()
end

function zo_UDPServerCallback(udpServer, recvStr)
    PX2_LOGGER:LogInfo("PHOENIXCode", recvStr)

    local stk = StringTokenizer(recvStr, "&")

    if stk:Count() >= 2 then
        local ip = stk:GetAt(0)
        local contentStr = stk:GetAt(1)

        local jsData = JSONData()
        if jsData:LoadBuffer(contentStr) then
            if jsData:IsHasMember("names") then
                local namesArray = jsData:GetMember("names")
                if namesArray:IsArray() then
                    local arraySize = namesArray:GetArraySize()
                    for i=1, arraySize do
                        local arrayEle = namesArray:GetArrayElement(i-1)
                        local str = arrayEle:ToString()

                        PX2_LOGGER:LogInfo("PHOENIXCode", str)

                        PX2_VOICESDK:Speak(str.."你好呀！")
                    end
                end
            end
        end
    end
end