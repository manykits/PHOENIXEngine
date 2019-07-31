-- ZERONE.lua

ZERONE_UIFaceCtrlScript = nil
ZERONE_IsZERO3DSceneCreated = false
ZERONE_CurSerialOptType = 0 -- 1 car, 2 lidar, 3 axis
ZERONE_IsFireRobot = false
ZERONE_MotoType = 0 -- 0 encorderMoto 1 stepMoto
ZERONE_RobotType = 1 --0 wood 1 meta

function zo_ZERONE()  
    rc_Arduino = Arduino:New()
    PX2_PROJ:PoolSet("Arduino", rc_Arduino)
    
    -- on raspberry we use linux
    if nil==RobotExt then
        rc_Robot = Robot:New()
    else        
        rc_Robot = RobotExt:New()
    end
    rc_Robot:SetArduino(rc_Arduino)

    if 0 == ZERONE_RobotType then
        rc_Robot:SetMaxSpeed(0.1)
        rc_Robot:SetMaxAccelerate(0.4)
        rc_Robot:SetPredictTime(6)
        rc_Robot:SetRadius(0.1)
        rc_Robot:SetLidarOffset(0.0)
        rc_Robot:SetMotoRate(35)
        rc_Robot:SetWheelRadius(0.03)
    elseif 1 == ZERONE_RobotType then
        rc_Robot:SetMinSpeed(0.0)
        rc_Robot:SetMaxSpeed(0.4)
        rc_Robot:SetMaxAccelerate(0.4)
        rc_Robot:SetPredictTime(1.2)
        rc_Robot:SetRadius(0.22)
        rc_Robot:SetLidarOffset(0.225)
        rc_Robot:SetMotoRate(45)
        rc_Robot:SetWheelRadius(0.0465)
    end

    local ui = PX2_PROJ:GetUI()
    
    local frameZERONE = UIFrame:New()
    ui:AttachChild(frameZERONE)
    frameZERONE:SetAnchorHor(0.0, 1.0)
    frameZERONE:SetAnchorVer(0.0, 1.0)

    zo_CreateUDPServer()

    -- unregist events
    UnRegistAllEventFunctions("SerialES::Open")
    UnRegistAllEventFunctions("SerialES::Close")

    UnRegistAllEventFunctions("AIES::LiDarOpen")
    UnRegistAllEventFunctions("AIES::LiDarClose")

    UnRegistAllEventFunctions("AIES::AxisOpen")
    UnRegistAllEventFunctions("AIES::AxisClose")

    -- lidar
    rc_Robot:CreateLidar()

    -- ui
    local frameUIFface = UIFrame:New()
    frameZERONE:AttachChild(frameUIFface)
    frameUIFface.LocalTransform:SetTranslateY(-15.0)
    frameUIFface:SetAnchorHor(0.0, 1.0)
    frameUIFface:SetAnchorVer(0.0, 1.0)
    frameUIFface.LocalTransform:SetTranslateY(-10.0)

    local ctrlScript = ZERONEUIFaceCtrl:New()
    local ctrler = Cast:ToSC(ctrlScript.__object)
	frameUIFface:AttachController(ctrler)
    ctrler:ResetPlay()

    local framePad = zo_PadCtrl()
    frameZERONE:AttachChild(framePad)
    framePad.LocalTransform:SetTranslateY(-20.0)
    framePad:SetAnchorHor(0.0, 1.0)
    framePad:SetAnchorVer(0.0, 1.0)
    framePad:Show(false)

    zo_Scene()
    
    -- event
    UnRegistAllEventFunctions("InputEventSpace::KeyPressed")
    RegistEventFunction("InputEventSpace::KeyPressed", function(keyStr)
        if "KC_0" == keyStr then
            ZERONE_UIFaceCtrlScript:SoundPlay(0)
        elseif "KC_1" == keyStr then
            ZERONE_UIFaceCtrlScript:SoundPlay(1)
        elseif "KC_2" == keyStr then
            ZERONE_UIFaceCtrlScript:SoundPlay(2)
        elseif "KC_3" == keyStr then
            ZERONE_UIFaceCtrlScript:SoundPlay(3)
        elseif "KC_4" == keyStr then
            ZERONE_UIFaceCtrlScript:SoundPlay(4)
        elseif "KC_5" == keyStr then
            ZERONE_UIFaceCtrlScript:SoundPlay(5)
        elseif "KC_6" == keyStr then
            ZERONE_UIFaceCtrlScript:SoundPlay(6)
        elseif "KC_LEFT" == keyStr then
            if nil~=ZERONE_UIFaceCtrlScript then
                ZERONE_UIFaceCtrlScript:MoveControl(1, true)
            end
        elseif "KC_RIGHT" == keyStr then
            if nil~=ZERONE_UIFaceCtrlScript then
                ZERONE_UIFaceCtrlScript:MoveControl(2, true)
            end
        elseif "KC_UP" == keyStr then
            if nil~=ZERONE_UIFaceCtrlScript then
                ZERONE_UIFaceCtrlScript:MoveControl(3, true)
            end
        elseif "KC_DOWN" == keyStr then
            if nil~=ZERONE_UIFaceCtrlScript then
                ZERONE_UIFaceCtrlScript:MoveControl(4, true)
            end
        elseif "KC_SPACE" == keyStr then
            if nil~=ZERONE_UIFaceCtrlScript then
                ZERONE_UIFaceCtrlScript:MoveControl(0, true)
            end

            zo_SceneAddBox()
        end
    end)
    UnRegistAllEventFunctions("InputEventSpace::KeyReleased")
    RegistEventFunction("InputEventSpace::KeyReleased", function(keyStr)
        if "KC_LEFT" == keyStr then
            if nil~=ZERONE_UIFaceCtrlScript then
                ZERONE_UIFaceCtrlScript:MoveControl(1, false)
            end
        elseif "KC_RIGHT" == keyStr then
            if nil~=ZERONE_UIFaceCtrlScript then
                ZERONE_UIFaceCtrlScript:MoveControl(2, false)
            end
        elseif "KC_UP" == keyStr then
            if nil~=ZERONE_UIFaceCtrlScript then
                ZERONE_UIFaceCtrlScript:MoveControl(3, false)
            end
        elseif "KC_DOWN" == keyStr then
            if nil~=ZERONE_UIFaceCtrlScript then
                ZERONE_UIFaceCtrlScript:MoveControl(4, false)
            end
        elseif "KC_SPACE" == keyStr then
            if nil~=ZERONE_UIFaceCtrlScript then
                ZERONE_UIFaceCtrlScript:MoveControl(0, false)
            end
        end
    end)
end

function zo_OnStartUp(useSpeed)
    if 0 == ZERONE_MotoType then
        PX2_LOGGER:LogInfo("ZERONE", "zo_OnStartUp".."0")
        -- Arduino
        if not ZERONE_IsRobotMotoWith298N then
            rc_Arduino:VehicleInitMotoBoard4567()
            if useSpeed then
                rc_Arduino:VehicleSpeedInit(Arduino.P_2, Arduino.P_8, Arduino.P_3, Arduino.P_9)
            end
        else
            rc_Arduino:VehicleInitMotoBoard298N(Arduino.P_A0, Arduino.P_A1, Arduino.P_10, Arduino.P_A2, Arduino.P_A3, Arduino.P_11)
            if useSpeed then
                rc_Arduino:VehicleSpeedInit(Arduino.P_2, Arduino.P_8, Arduino.P_3, Arduino.P_9)
            end
        end

        rc_Arduino:PinMode(Arduino.P_10, Arduino.PM_OUTPUT)
        rc_Arduino:PinMode(Arduino.P_11, Arduino.PM_OUTPUT)
        rc_Arduino:PinMode(Arduino.P_12, Arduino.PM_OUTPUT)
        rc_Arduino:PinMode(Arduino.P_13, Arduino.PM_OUTPUT)
        rc_Arduino:ServoInit(0, Arduino.P_A0)
        
        if ZERONE_IsFireRobot then
            rc_Arduino:ServoInit(1, Arduino.P_A1)
        end
    elseif 1==ZERONE_MotoType then
        PX2_LOGGER:LogInfo("ZERONE", "zo_OnStartUp".."1")

        rc_Arduino:StepMotoInit(0, Arduino.P_2, Arduino.P_3, Arduino.P_4, Arduino.P_5)
        rc_Arduino:StepMotoInit(1, Arduino.P_7, Arduino.P_6, Arduino.P_8, Arduino.P_9)

        rc_Arduino:StepMotoEnable(0, false)
        rc_Arduino:StepMotoEnable(1, false)
    end

    PX2_VOICESDK:Speak("零一机器人已经启动！")
end

function zo_AppUpdateCallback(appSecondsStr, elapsedSecondsStr)
    if nil~=rc_Robot then
        rc_Robot:Update(appSecondsStr, elapsedSecondsStr)
    end

    if nil~=ZERONE_UDPServer then
        ZERONE_UDPServer:Update(elapsedSecondsStr)
    end
end