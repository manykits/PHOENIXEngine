-- stop.lua

function engine_project_prestop()

end

function engine_project_stop()
    local udpServer = PX2_PROJ:PoolGet("DUPServer")
    if nil~=udpServer then
        udpServer:Stop()
    end
    ZERONE_UDPServer = nil

    PX2_PROJ:PoolSet("Arduino", nil)
    rc_Arduino = nil

    if nil==RobotExt then
        Robot:Delete(rc_Robot)
    else       
        RobotExt:Delete(rc_Robot)
    end
    rc_Robot = nil

    PX2_VOICESDK:EnableAutoSpeak(false)
    PX2_VOICESDK:EnableAutoSpeakTTS(false)
	
	Robot:TernimateSlam2D()
end