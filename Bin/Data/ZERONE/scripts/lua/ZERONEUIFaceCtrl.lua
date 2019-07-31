-- ZERONEUIFaceCtrl.lua

ZERONE_SpeakText = nil
ZERONE_TextWidth = 660.0
ZERONE_TextHeight = 200.0

ZERONE_IsPlayingMusics = false
ZERONE_MusicsPlayIndex = 1

ZERONE_EditBoxName = nil
ZERONE_EditBoxID = nil

ZERONE_FrameSetting = nil
ZERONE_BleList = nil
ZERONE_BleConnect = nil

ZERONE_MAP_DrawType = 0 --0 none,1 obst,2 remove

zo_MT = 0.75
zo_LT = 2.0
zo_LastDelay = 0.8
zo_MapPickPos = {
    x = 0.0,
    y = 0.0
}

ZERONEUIFaceCtrl = class(LuaScriptController,
{
    _scriptControl = nil,
    _ctrlable = nil,
    _background = nil,
    _content = nil,
    _MapFrame = nil,
    _lidarFrame = nil,

    _name = "零一",
    _id = 1,

    _EyeLeft = nil,
    _EyeLeftCtrl = nil,

    _EyeRight = nil,
    _EyeRightCtrl = nil,

    _text = nil,
    _textPosition = nil,
    _textPositionCur = nil,

    _vlc = nil,

    _btnSetting = nil,
    _btnCamera = nil,
    _btnVoice = nil,
    _btn3D = nil,
    _btnShowUI = nil,
    _btnLiDar = nil,
    _btnLiDarCur = nil,
    _btnUIPad = nil,
    _btnMapDraw = nil,
    _btnChara = nil,
    _btnMotoType = nil,
    _isCameraOpen = false,

    _musics = {},
});

function ZERONEUIFaceCtrl:OnAttached()
    self._scriptControl = Cast:ToSC(self.__object)
	self._ctrlable = self._scriptControl:GetControlledable()

    ZERONE_UIFaceCtrlScript = self

    -- strlable
    self._ctrlable:SetScriptHandlerWidgetPicked("ZERONEWidgetPicked")
    self._ctrlable:SetScriptHandlerNodePicked("ZERONEFrameSizeNodePicked")
    self._ctrlable:SetScriptHandlerNodeNotPicked("ZERONEFrameSizeNodeNotPicked")

    -- background
    local frame = UIFrame:New()
    self._background = frame
    self._ctrlable:AttachChild(frame)
    frame:SetAnchorHor(0.0, 1.0)
    frame:SetAnchorVer(0.0, 1.0)

    -- text position
    local textPosition = UIFText:New()
    self._textPosition = textPosition
    frame:AttachChild(textPosition)
    textPosition.LocalTransform:SetTranslateY(-5.0)
    textPosition:GetText():SetFontColor(Float3.WHITE)
    textPosition:GetText():SetFontSize(24)
    textPosition:SetAnchorHor(0.0, 0.0)
    textPosition:SetAnchorVer(1.0, 1.0)
    textPosition:SetAnchorParamHor(40.0, 40.0)
    textPosition:SetAnchorParamVer(-140.0, -140.0)
    textPosition:SetSize(500.0, 100.0)
    textPosition:GetText():SetAutoWarp(true)
    textPosition:GetText():SetAligns(TEXTALIGN_LEFT+TEXTALIGN_VCENTER)
    textPosition:SetPivot(0.0, 0.5)
    textPosition:GetText():SetFontScale(0.7);
    textPosition:GetText():SetText("Position")

    local textPositionCur = UIFText:New()
    self._textPositionCur = textPositionCur
    frame:AttachChild(textPositionCur)
    textPositionCur.LocalTransform:SetTranslateY(-6.0)
    textPositionCur:GetText():SetFontColor(Float3.WHITE)
    textPositionCur:GetText():SetFontSize(24)
    textPositionCur:SetAnchorHor(0.0, 0.0)
    textPositionCur:SetAnchorVer(1.0, 1.0)
    textPositionCur:SetAnchorParamHor(40.0, 40.0)
    textPositionCur:SetAnchorParamVer(-160.0, -160.0)
    textPositionCur:SetSize(500.0, 100.0)
    textPositionCur:GetText():SetAutoWarp(true)
    textPositionCur:GetText():SetAligns(TEXTALIGN_LEFT+TEXTALIGN_VCENTER)
    textPositionCur:SetPivot(0.0, 0.5)
    textPositionCur:GetText():SetFontScale(0.7);
    textPositionCur:GetText():SetText("Position")

    -- cnt frame
    self:CreateContentFrame()

    -- btns frame
    self:CreateBtnsSetting()

    -- lidar frame
    self:CreateLidarFrame()

    -- musics
    --self:MusicAdd(1, "Data/ZERONE/media/music/canon.mp3")
    --self:MusicAdd(2, "Data/ZERONE/media/music/destination.mp3")
    --self:MusicAdd(3, "Data/ZERONE/media/music/roadtohappiness.mp3")
    --self:MusicAdd(4, "Data/ZERONE/media/music/stratus.mp3")
    --self:MusicReWrite()
    --self:MusicStop()

    -- say somthing
    PX2_VOICESDK:Speak("你好，我是零一")
    PX2_VOICESDK:EnableAutoSpeak(true)
    PX2_VOICESDK:EnableAutoSpeakTTS(true)     

    UnRegistAllEventFunctions("VoiceSDKSpace::SpeakText")
    RegistEventFunction("VoiceSDKSpace::SpeakText", function(txt)  
        --if not ZERONE_IsPlayingMusics then
            coroutine.wrap(function()     
                sleep(0.3)
                ZERONE_UIFaceCtrlScript:SetText(txt)
                local fontWidth = ZERONE_SpeakText:GetText():GetTextWidth()
                if fontWidth < (ZERONE_TextWidth-20.0) then
                    ZERONE_SpeakText:GetText():SetRectUseage(RU_ALIGNS)
                    ZERONE_SpeakText:SetHeight(ZERONE_TextHeight)
                    ZERONE_SpeakText:SetAnchorParamVer(ZERONE_TextHeight*0.5, ZERONE_TextHeight*0.5)
                else
                    local numHeight = (fontWidth/ZERONE_TextWidth) * 1.0 + 1
                    local height = numHeight * 40
                    ZERONE_SpeakText:GetText():SetRectUseage(RU_CLIPWARP)
                    ZERONE_SpeakText:SetHeight(height)
                    if height>ZERONE_TextHeight then
                        ZERONE_SpeakText:SetAnchorParamVer(height*0.5, height*0.5)
                    end
                end
            end)()
        --end
    end)  

    UnRegistAllEventFunctions("VoiceSDKSpace::RecognizeResults")
    RegistEventFunction("VoiceSDKSpace::RecognizeResults", function(txt, txtJson)  
        ZERONE_UIFaceCtrlScript:SetText(txt)
        local fontWidth = ZERONE_SpeakText:GetText():GetTextWidth()
        if fontWidth < (ZERONE_TextWidth-20.0) then
            ZERONE_SpeakText:GetText():SetRectUseage(RU_ALIGNS)
            ZERONE_SpeakText:SetHeight(ZERONE_TextHeight)
            ZERONE_SpeakText:SetAnchorParamVer(ZERONE_TextHeight*0.5, ZERONE_TextHeight*0.5)
        else
            local numHeight = (fontWidth/ZERONE_TextWidth) * 1.0 + 1
            local height = numHeight * 40
            ZERONE_SpeakText:GetText():SetRectUseage(RU_CLIPWARP)
            ZERONE_SpeakText:SetHeight(height)
            if height>ZERONE_TextHeight then
                ZERONE_SpeakText:SetAnchorParamVer(height*0.5, height*0.5)
            end
        end

        local strPlayMusic = "播放音乐"
        local strLen = string.len(strPlayMusic) 

        if string.find(txt, "播放最热音乐") then
            ZERONE_IsPlayingMusics = true
            
            PX2_VOICESDK:PlayHotMusic()
            PX2_VOICESDK:IngoreThisTimeAutoSpeech()
            
            --ZERONE_UIFaceCtrlScript:MusicPlayRandom()
            --PX2_VOICESDK:EnableAutoSpeakTTS(false)  
        elseif string.find(txt, strPlayMusic) then
            local subStr = ""..string.sub(txt, strLen +4, -4)  

            PX2_VOICESDK:PlayMusic(""..subStr)
            PX2_VOICESDK:IngoreThisTimeAutoSpeech()
        elseif string.find(txt, "停止音乐") ==txt then
            ZERONE_IsPlayingMusics = false
            ZERONE_UIFaceCtrlScript:MusicStop()
            --PX2_VOICESDK:EnableAutoSpeakTTS(true)            
        elseif "上一首"==txt or "上一手"==txt or "上一曲"==txt then
            if ZERONE_IsPlayingMusics then
                ZERONE_UIFaceCtrlScript:MusicBefore()
            end
        elseif "下一首"==txt  or "下一手"==txt or "下一曲"==txt then
            if ZERONE_IsPlayingMusics then
                ZERONE_UIFaceCtrlScript:MusicNext()
            end
        end
    end)  

    coroutine.wrap(function()     
        self:StartUp()
        sleep(zo_LT+zo_LastDelay)
        self:Idle()
    end)()
end

function ZERONEUIFaceCtrl:OnInitUpdate()
end

function ZERONEUIFaceCtrl:OnUpdate()
end

function ZERONEUIFaceCtrl:OnPPlay()

end

function ZERONEUIFaceCtrl:OnPUpdate()
    local pos = rc_Robot:GetPosition()
    local dir = rc_Robot:GetDirection()
    local up = rc_Robot:GetUp()
    local right = rc_Robot:GetRight()
    local degree = rc_Robot:GetSlam2DDegree()

    local txt = PX2_LM_APP:GetValue("Pos")..":"..StringHelp:FloatToString(pos:X())..","..StringHelp:FloatToString(pos:Y())..","..StringHelp:FloatToString(pos:Z()).." D:"..string.format("%.2f", degree)
    self._textPosition:GetText():SetText(txt)

    local txtCur = PX2_LM_APP:GetValue("PickPos")..":"..string.format("%.2f", zo_MapPickPos.x) ..","..string.format("%.2f", zo_MapPickPos.y)
    self._textPositionCur:GetText():SetText(txtCur)

    if self._IsDirectionChanged then

		if not self._IsLeftPressed and not self._IsRightPressed and not self._IsUpPressed and not self._IsDownPressed then
			self:Move(MoveType.MT_NONE, 0)
		else
            if ZERONE_IsRobotMotoWith298N then
                if self._IsUpPressed then
                    self:Move(MoveType.MT_FORWARD, 160)
                end
                
                if self._IsDownPressed then
                    self:Move(MoveType.MT_BACKWARD, 160)
                end
            
                if self._IsLeftPressed then
                    self:Move(MoveType.MT_LEFT, 45)
                end
        
                if self._IsRightPressed then
                    self:Move(MoveType.MT_RIGHT, 45)
                end   
            else
                if self._IsUpPressed then
                    self:Move(MoveType.MT_FORWARD, 75)
                end
                
                if self._IsDownPressed then
                    self:Move(MoveType.MT_BACKWARD, 75)
                end

                if self._IsLeftPressed then
                    self:Move(MoveType.MT_LEFT, 25)
                end
        
                if self._IsRightPressed then
                    self:Move(MoveType.MT_RIGHT, 25)
                end     
            end
		end

		if self._IsSpacePressed then
			self:Move(MoveType.MT_NONE, 255)
		end

		self._IsDirectionChanged = false
	end
end

-- move
-- 0,1,2,3,4 none left right up down
function ZERONEUIFaceCtrl:MoveControl(type, isPresssed)
	if 0==type then
		if self._IsSpacePressed~=isPresssed then
			self._IsSpacePressed = isPresssed
			self._IsDirectionChanged = true
		end
	elseif 1==type then
		if self._IsLeftPressed~=isPresssed then
			self._IsLeftPressed = isPresssed
			self._IsDirectionChanged = true
		end
	elseif 2==type then
		if self._IsRightPressed~=isPresssed then
			self._IsRightPressed = isPresssed
			self._IsDirectionChanged = true
		end
	elseif 3==type then
		if self._IsUpPressed~=isPresssed then
			self._IsUpPressed = isPresssed
			self._IsDirectionChanged = true
		end
	elseif 4==type then
		if self._IsDownPressed~=isPresssed then
			self._IsDownPressed = isPresssed
			self._IsDirectionChanged = true
		end
	end
end

function ZERONEUIFaceCtrl:Move(moveType, speed)
	if self._moveType==moveType then
		return
	end

    self._moveType = moveType
    
    local runSpeed = rc_Robot:GetMaxSpeed()
    local turnSpeed = runSpeed * 0.15

    local radius = rc_Robot:GetWheelRadius()
    local rat = rc_Robot:GetMotoRate()

    if 0==ZERONE_MotoType then
        if moveType == MoveType.MT_NONE then
            rc_Arduino:RunSpeed(0, 0.0, radius, rat)
            rc_Arduino:RunSpeed(1, 0.0, radius, rat)
        elseif moveType == MoveType.MT_FORWARD then
            rc_Arduino:RunSpeed(0, runSpeed, radius, rat)
            rc_Arduino:RunSpeed(1, runSpeed, radius, rat)
        elseif moveType == MoveType.MT_BACKWARD then
            rc_Arduino:RunSpeed(0, -runSpeed, radius, rat)
            rc_Arduino:RunSpeed(1, -runSpeed, radius, rat)
        elseif moveType == MoveType.MT_LEFT then
            rc_Arduino:RunSpeed(0, -turnSpeed, radius, rat)
            rc_Arduino:RunSpeed(1, turnSpeed, radius, rat)
        elseif moveType == MoveType.MT_RIGHT then
            rc_Arduino:RunSpeed(0, turnSpeed, radius, rat)
            rc_Arduino:RunSpeed(1, -turnSpeed, radius, rat)
        end
    elseif 1==ZERONE_MotoType then
        if moveType == MoveType.MT_NONE then
            rc_Arduino:StepMotoStep(0, 255)
            rc_Arduino:StepMotoStep(1, 255)

            rc_Arduino:StepMotoEnable(0, false)
            rc_Arduino:StepMotoEnable(1, false)
            
            rc_Arduino:StepMotoDir(0, true)
            rc_Arduino:StepMotoDir(1, true)
        elseif moveType == MoveType.MT_FORWARD then
            rc_Arduino:StepMotoStep(0, 2)
            rc_Arduino:StepMotoStep(1, 2)

            rc_Arduino:StepMotoEnable(0, true)
            rc_Arduino:StepMotoEnable(1, true)

            rc_Arduino:StepMotoDir(0, false)
            rc_Arduino:StepMotoDir(1, true)
        elseif moveType == MoveType.MT_BACKWARD then
            rc_Arduino:StepMotoStep(0, 2)
            rc_Arduino:StepMotoStep(1, 2)

            rc_Arduino:StepMotoEnable(0, true)
            rc_Arduino:StepMotoEnable(1, true)

            rc_Arduino:StepMotoDir(0, true)
            rc_Arduino:StepMotoDir(1, false)
        elseif moveType == MoveType.MT_LEFT then
            rc_Arduino:StepMotoStep(0, 2)
            rc_Arduino:StepMotoStep(1, 2)

            rc_Arduino:StepMotoEnable(0, true)
            rc_Arduino:StepMotoEnable(1, true)

            rc_Arduino:StepMotoDir(0, true)
            rc_Arduino:StepMotoDir(1, true)
        elseif moveType == MoveType.MT_RIGHT then
            rc_Arduino:StepMotoStep(0, 2)
            rc_Arduino:StepMotoStep(1, 2)

            rc_Arduino:StepMotoEnable(0, true)
            rc_Arduino:StepMotoEnable(1, true)

            rc_Arduino:StepMotoDir(0, false)
            rc_Arduino:StepMotoDir(1, false)
        end
    end
end

-- lidar frame
function ZERONEUIFaceCtrl:CreateLidarFrame()
    local lidarFrame = UIFrame:New()
    self._MapFrame = lidarFrame
    self._background:AttachChild(lidarFrame)
    lidarFrame.LocalTransform:SetTranslateY(-2.0)
    lidarFrame:SetAnchorHor(0.0, 1.0)
    lidarFrame:SetAnchorVer(0.0, 1.0)

    local picBox = rc_Robot:GetUIFPicBoxMap()
    lidarFrame:AttachChild(picBox)
    picBox.LocalTransform:SetTranslateY(-8.0)
    picBox:SetAnchorHor(0.5, 0.5)
    picBox:SetAnchorVer(0.5, 0.5)
    picBox:SetAlpha(0.8)
    picBox:SetSize(640.0, 640.0)
    picBox:SetScriptHandlerWidgetPicked("ZERO_MapPicBoxCallback")
    picBox:SetWidget(true)
    picBox:RegistToScriptSystem()

    lidarFrame:Show(false)

    -- cur
    local lidarFrameCur = UIFrame:New()
    self._lidarFrame = lidarFrameCur
    self._background:AttachChild(lidarFrameCur)
    lidarFrameCur.LocalTransform:SetTranslateY(-2.0)
    lidarFrameCur:SetAnchorHor(0.0, 1.0)
    lidarFrameCur:SetAnchorVer(0.0, 1.0)

    local picBoxCur = rc_Robot:GetLidar():GetUIFPicBoxLidar()
    lidarFrameCur:AttachChild(picBoxCur)
    picBoxCur.LocalTransform:SetTranslateY(-7.0)
    picBoxCur:SetAnchorHor(0.5, 0.5)
    picBoxCur:SetAnchorVer(0.5, 0.5)
    picBoxCur:SetAlpha(0.5)
    picBoxCur:SetSize(640.0, 640.0)

    lidarFrameCur:Show(false)
end

local _IsPressed = false
local _IsMoved = false
function ZERO_MapPicBoxCallback(ptr)
    local obj = Cast:ToO(ptr)
	if nil~=obj then
        local lastPickData = obj:GetLastPickData()
        local logicPos = lastPickData.LogicPos

        if UIPT_PRESSED == lastPickData.PickType then
            ZERO_SetLogicPos(obj, logicPos)
            _IsPressed = true
            _IsMoved = false
        elseif UIPT_RELEASED == lastPickData.PickType then
            _IsPressed = false
            if not _IsMoved then
                local pos = APoint(zo_MapPickPos.x, zo_MapPickPos.y, 0.0)
                rc_Robot:GoTarget(pos, Robot.PT_A)
            end
        elseif UIPT_MOVED == lastPickData.PickType then
            _IsMoved = true
            if _IsPressed then
                ZERO_SetObst(obj, logicPos)
            end
		end
	end
end

function ZERO_SetLogicPos(obj, logicPos)
    local worldRect = obj:GetWorldRect()

    local width = worldRect:Width()
    local height = worldRect:Height()

    local picX = logicPos:X() - worldRect.Left
    local picY = logicPos:Z() - worldRect.Bottom

    local percX = (picX*1.0)/(width*1.0)
    local percY = (picY*1.0)/(height*1.0)

    local mapSize = rc_Robot:GetMapSize()
    local mapReso = rc_Robot:GetResolution()
    local mapAllSize = mapSize * mapReso

    zo_MapPickPos.x = (percX-0.5) * mapAllSize
    zo_MapPickPos.y = (percY-0.5) * mapAllSize
end

function ZERO_SetObst(obj, logicPos)
    local worldRect = obj:GetWorldRect()

    local width = worldRect:Width()
    local height = worldRect:Height()

    local picX = logicPos:X() - worldRect.Left
    local picY = logicPos:Z() - worldRect.Bottom

    local percX = (picX*1.0)/(width*1.0)
    local percY = (picY*1.0)/(height*1.0)

    local mapSize = rc_Robot:GetMapSize()
    local mapReso = rc_Robot:GetResolution()
    local mapAllSize = mapSize * mapReso

    local x = (percX-0.5) * mapAllSize
    local y = (percY-0.5) * mapAllSize

    local pos = APoint(x, y, 0.0)

    if 0 == ZERONE_MAP_DrawType then -- clear
        rc_Robot:SetObstMapValueAtPos(pos, 0.25, 10)    
    elseif 1 == ZERONE_MAP_DrawType then -- obst
        rc_Robot:SetObstMapValueAtPos(pos, 0.1, 0)
    elseif 2 == ZERONE_MAP_DrawType then -- can go
        rc_Robot:SetObstMapValueAtPos(pos, 0.1, 200) -- space can go
    end
end

-- cnt frame
function ZERONEUIFaceCtrl:CreateContentFrame()
    local frameContent = UIFrame:New()
    self._content = frameContent
    self._background:AttachChild(frameContent)
    frameContent:SetAnchorHor(0.0, 1.0)
    frameContent:SetAnchorVer(0.0, 1.0)
    frameContent:SetWidget(true)

    -- circle out
    local fPicEyeLeft = UIFPicBox:New()
    self._EyeLeft = fPicEyeLeft
    frameContent:AttachChild(fPicEyeLeft)
    fPicEyeLeft.LocalTransform:SetTranslateY(-2.0)
    fPicEyeLeft:GetUIPicBox():SetTexture("Data/ZERONE/images/zerone.png")
    fPicEyeLeft:SetAnchorHor(0.3, 0.3)
    self:SetEyeLeftSize(260.0)

    -- circle in
    local fPicRightEye = UIFPicBox:New()
    self._EyeRight = fPicRightEye
    frameContent:AttachChild(fPicRightEye)
    fPicRightEye.LocalTransform:SetTranslateY(-2.0)
    fPicRightEye:GetUIPicBox():SetTexture("Data/ZERONE/images/zerone.png")
    fPicRightEye:SetAnchorHor(0.7, 0.7)
    self:SetEyeRightSize(260.0)

    -- text
    local text = UIFText:New()
    self._text = text
    ZERONE_SpeakText = text
    frameContent:AttachChild(text)
    text.LocalTransform:SetTranslateY(-5.0)
    text:GetText():SetFontColor(Float3.WHITE)
    text:GetText():SetFontSize(36)
    text:SetAnchorVer(0.0, 0.0)
    text:SetAnchorParamVer(100.0, 100.0)
    text:SetSize(ZERONE_TextWidth, ZERONE_TextHeight)
    text:GetText():SetAutoWarp(true)
    self:SetText("你好，我是"..self._name)
    text:SetPivot(0.5, 0.5)

    -- ctrls
    local ctrlOut = InterpCurveUniformScaleController:New()
    self._EyeLeftCtrl = ctrlOut
    fPicEyeLeft:AttachController(ctrlOut)

    local ctrlIn = InterpCurveUniformScaleController:New()
    self._EyeRightCtrl = ctrlIn
    fPicRightEye:AttachController(ctrlIn)

    -- vlc
    --[[
    _vlc = UIVlc:New()
    frameContent:AttachChild(_vlc)
    _vlc.LocalTransform:SetTranslateY(-7.0)
    _vlc:SetAnchorHor(0.35, 0.65)
    _vlc:SetAnchorVer(0.75, 1.0)
    _vlc:SetType(UIVlc.T_HARDCAMERA)
    ]]--

    --self._content:Show(false)

    self:CloseCamera()
end


-- btns setting
function ZERONEUIFaceCtrl:CreateBtnsSetting()
    -- setting btn
    local btnSetting = UIButton:New("BtnSetting")
    self._btnSetting = btnSetting
    self._background:AttachChild(btnSetting)
    btnSetting.LocalTransform:SetTranslateY(-5.0)
    btnSetting:SetAnchorHor(0.0, 0.0)
    btnSetting:SetAnchorVer(1.0, 1.0)
    btnSetting:SetAnchorParamHor(60.0, 60.0)
    btnSetting:SetAnchorParamVer(-60.0, -60.0)
    btnSetting:SetSize(40.0, 40.0)
    btnSetting:SetStateColorDefaultWhite()
    btnSetting:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/engine/white.png")
    btnSetting:SetStateColor(UIButtonBase.BS_NORMAL, Float3:MakeColor(0, 121, 216))
    btnSetting:SetStateColor(UIButtonBase.BS_PRESSED, Float3:MakeColor(0, 121, 216))
    btnSetting:SetStateColor(UIButtonBase.BS_HOVERED, Float3:MakeColor(0, 121, 216))
    btnSetting:SetScriptHandler("zo_UICallabck")
    btnSetting:CreateAddText("S")

    -- text testing
    local textTesting = UIFText:New()
    self._background:AttachChild(textTesting)
    textTesting.LocalTransform:SetTranslateY(-5.0)
    textTesting:GetText():SetFontColor(Float3.WHITE)
    textTesting:GetText():SetFontSize(20)
    textTesting:SetWidth(200)
    textTesting:SetAnchorHor(0.0, 0.0)
    textTesting:SetAnchorVer(1.0, 1.0)
    textTesting:SetPivot(0.0, 0.5)
    textTesting:SetAnchorParamHor(60.0, 60.0)
    textTesting:SetAnchorParamVer(-60.0, -60.0)
    textTesting:GetText():SetText("测试版 v 1.0.0")

    -- cam btn
    local btnCamera = UIButton:New("BtnCamera")
    self._btnCamera = btnCamera
    self._background:AttachChild(btnCamera)
    btnCamera.LocalTransform:SetTranslateY(-5.0)
    btnCamera:SetAnchorHor(1.0, 1.0)
    btnCamera:SetAnchorVer(1.0, 1.0)
    btnCamera:SetAnchorParamHor(-60.0, -60.0)
    btnCamera:SetAnchorParamVer(-60.0, -60.0)
    btnCamera:SetSize(40.0, 40.0)
    btnCamera:SetStateColorDefaultWhite()
    btnCamera:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/engine/white.png")
    btnCamera:SetStateColor(UIButtonBase.BS_NORMAL, Float3:MakeColor(0, 121, 216))
    btnCamera:SetStateColor(UIButtonBase.BS_PRESSED, Float3:MakeColor(0, 121, 216))
    btnCamera:SetStateColor(UIButtonBase.BS_HOVERED, Float3:MakeColor(0, 121, 216))
    btnCamera:SetScriptHandler("zo_UICallabck")
    btnCamera:CreateAddText("C")

    -- btn camera
    local btnVoice = UIButton:New("BtnVoice")
    self._btnVoice = btnVoice
    self._background:AttachChild(btnVoice)
    btnVoice.LocalTransform:SetTranslateY(-5.0)
    btnVoice:SetAnchorHor(0.0, 0.0)
    btnVoice:SetAnchorVer(0.0, 0.0)
    btnVoice:SetAnchorParamHor(60.0, 60.0)
    btnVoice:SetAnchorParamVer(60.0, 60.0)
    btnVoice:SetSize(40.0, 40.0)
    btnVoice:SetStateColorDefaultWhite()
    btnVoice:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/engine/white.png")
    btnVoice:SetStateColor(UIButtonBase.BS_NORMAL, Float3:MakeColor(0, 121, 216))
    btnVoice:SetStateColor(UIButtonBase.BS_PRESSED, Float3:MakeColor(0, 121, 216))
    btnVoice:SetStateColor(UIButtonBase.BS_HOVERED, Float3:MakeColor(0, 121, 216))
    btnVoice:SetScriptHandler("zo_UICallabck")
    btnVoice:CreateAddText("V")

    -- btn 3d
    -- local btn3D = UIButton:New("Btn3D")
    -- self._btn3D = btn3D
    -- self._background:AttachChild(btn3D)
    -- btn3D.LocalTransform:SetTranslateY(-5.0)
    -- btn3D:SetAnchorHor(1.0, 1.0)
    -- btn3D:SetAnchorVer(0.0, 0.0)
    -- btn3D:SetAnchorParamHor(-60.0, -60.0)
    -- btn3D:SetAnchorParamVer(60.0, 60.0)
    -- btn3D:SetSize(40.0, 40.0)
    -- btn3D:SetStateColorDefaultWhite()
    -- btn3D:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/engine/white.png")
    -- btn3D:SetStateColor(UIButtonBase.BS_NORMAL, Float3:MakeColor(0, 121, 216))
    -- btn3D:SetStateColor(UIButtonBase.BS_PRESSED, Float3:MakeColor(0, 121, 216))
    -- btn3D:SetStateColor(UIButtonBase.BS_HOVERED, Float3:MakeColor(0, 121, 216))
    -- btn3D:SetScriptHandler("zo_UICallabck")
    -- btn3D:CreateAddText("3D")

    -- RoboType
    local comBoxMotoType = UIComboBox:New("BtnComboxBoxMotoType")
    self._btnMotoType = comBoxMotoType
    self._background:AttachChild(comBoxMotoType)
    comBoxMotoType.LocalTransform:SetTranslateY(-5.0)
    comBoxMotoType:AddChooseStr(""..PX2_LM_APP:GetValue("EncorderMoto"))
    comBoxMotoType:AddChooseStr(""..PX2_LM_APP:GetValue("StepMoto"))
    comBoxMotoType:SetChooseListHeightSameWithChooses()
    comBoxMotoType:SetAnchorHor(0.0, 0.0)
    comBoxMotoType:SetAnchorVer(1.0, 1.0)
    comBoxMotoType:SetAnchorParamHor(40.0, 40.0)
    comBoxMotoType:SetAnchorParamVer(-100.0, -100.0)
    comBoxMotoType:SetSize(140.0, 30.0)
    comBoxMotoType:SetPivot(0.0, 0.5)
    comBoxMotoType:SetScriptHandler("zo_UICallabck")
    comBoxMotoType:Choose(0)

    local comBoxChara = UIComboBox:New("BtnComboxBoxChara")
    self._btnChara = comBoxChara
    self._background:AttachChild(comBoxChara)
    comBoxChara.LocalTransform:SetTranslateY(-5.0)
    comBoxChara:AddChooseStr(""..PX2_LM_APP:GetValue("master"))
    comBoxChara:AddChooseStr(""..PX2_LM_APP:GetValue("connector"))
    comBoxChara:AddChooseStr(""..PX2_LM_APP:GetValue("master_sendlidar"))
    comBoxChara:AddChooseStr(""..PX2_LM_APP:GetValue("master_connector_calculate"))
    comBoxChara:SetChooseListHeightSameWithChooses()
    comBoxChara:SetAnchorHor(0.0, 0.0)
    comBoxChara:SetAnchorVer(1.0, 1.0)
    comBoxChara:SetAnchorParamHor(190.0, 190.0)
    comBoxChara:SetAnchorParamVer(-100.0, -100.0)
    comBoxChara:SetSize(140.0, 30.0)
    comBoxChara:SetPivot(0.0, 0.5)
    comBoxChara:SetScriptHandler("zo_UICallabck")
    comBoxChara:Choose(0)

    -- combox
    local comBoxEdit = UIComboBox:New("BtnComboxBoxEdit")
    self._btnMapDraw = comBoxEdit
    self._background:AttachChild(comBoxEdit)
    comBoxEdit.LocalTransform:SetTranslateY(-5.0)
    comBoxEdit:AddChooseStr(""..PX2_LM_APP:GetValue("ClearObst"))
    comBoxEdit:AddChooseStr(""..PX2_LM_APP:GetValue("AddObst"))
    comBoxEdit:AddChooseStr(""..PX2_LM_APP:GetValue("RemoveObst"))
    comBoxEdit:SetChooseListHeightSameWithChooses()
    comBoxEdit:SetAnchorHor(1.0, 1.0)
    comBoxEdit:SetAnchorVer(1.0, 1.0)
    comBoxEdit:SetAnchorParamHor(-40.0, -40.0)
    comBoxEdit:SetAnchorParamVer(-100.0, -100.0)
    comBoxEdit:SetSize(140.0, 30.0)
    comBoxEdit:SetPivot(1.0, 0.5)
    comBoxEdit:SetScriptHandler("zo_UICallabck")
    comBoxEdit:Choose(0)

    -- check show face
    local checkShowFace = UICheckButton:New("BtnShowFace")
    self._btnShowUI = checkShowFace
    self._background:AttachChild(checkShowFace)
    checkShowFace.LocalTransform:SetTranslateY(-5.0)
    checkShowFace:SetAnchorHor(1.0, 1.0)
    checkShowFace:SetAnchorVer(0.0, 0.0)
    checkShowFace:SetAnchorParamHor(-110.0, -110.0)
    checkShowFace:SetAnchorParamVer(60.0, 60.0)
    checkShowFace:SetSize(40.0, 40.0)
    checkShowFace:SetScriptHandler("zo_UICallabck")
    checkShowFace:Check(true, false)
	local fTextFace = checkShowFace:CreateAddText(""..PX2_LM_APP:GetValue("face"))
    zo_UISetTextFont(fTextFace:GetText(), 16)

    local checkLidar = UICheckButton:New("BtnLidar")
    self._btnLiDar = checkLidar
    self._background:AttachChild(checkLidar)
    checkLidar.LocalTransform:SetTranslateY(-5.0)
    checkLidar:SetAnchorHor(1.0, 1.0)
    checkLidar:SetAnchorVer(0.0, 0.0)
    checkLidar:SetAnchorParamHor(-160.0, -110.0)
    checkLidar:SetAnchorParamVer(60.0, 60.0)
    checkLidar:SetSize(40.0, 40.0)
    checkLidar:SetScriptHandler("zo_UICallabck")
    checkLidar:Check(false, false)
	local fTextLiDar = checkLidar:CreateAddText(""..PX2_LM_APP:GetValue("lidar"))
    zo_UISetTextFont(fTextLiDar:GetText(), 16)

    local checkLidarCur = UICheckButton:New("BtnLidarCur")
    self._btnLiDarCur = checkLidarCur
    self._background:AttachChild(checkLidarCur)
    checkLidarCur.LocalTransform:SetTranslateY(-5.0)
    checkLidarCur:SetAnchorHor(1.0, 1.0)
    checkLidarCur:SetAnchorVer(0.0, 0.0)
    checkLidarCur:SetAnchorParamHor(-210.0, -210.0)
    checkLidarCur:SetAnchorParamVer(60.0, 60.0)
    checkLidarCur:SetSize(40.0, 40.0)
    checkLidarCur:SetScriptHandler("zo_UICallabck")
    checkLidarCur:Check(false, false)
	local fTextLiDarCur = checkLidarCur:CreateAddText(""..PX2_LM_APP:GetValue("lidarcur"))
    zo_UISetTextFont(fTextLiDarCur:GetText(), 16)
    fTextLiDarCur:GetText():SetFontScale(0.65)

    local checkUICtrl = UICheckButton:New("BtnUICtrl")
    self._btnUIPad = checkUICtrl
    self._background:AttachChild(checkUICtrl)
    checkUICtrl.LocalTransform:SetTranslateY(-5.0)
    checkUICtrl:SetAnchorHor(1.0, 1.0)
    checkUICtrl:SetAnchorVer(0.0, 0.0)
    checkUICtrl:SetAnchorParamHor(-260.0, -260.0)
    checkUICtrl:SetAnchorParamVer(60.0, 60.0)
    checkUICtrl:SetSize(40.0, 40.0)
    checkUICtrl:SetScriptHandler("zo_UICallabck")
    checkUICtrl:Check(false, false)
	local fTextUICtrlCur = checkUICtrl:CreateAddText(""..PX2_LM_APP:GetValue("uictrl"))
    zo_UISetTextFont(fTextUICtrlCur:GetText(), 16)
    fTextUICtrlCur:GetText():SetFontScale(0.65)

    -- frame
	local uiFrameBack, uiFrame, btnClose, titleText = zo_UICreateDlgCommon(478.0, 324.0, "")
    uiFrame:SetAnchorHor(0.0, 1.0)
	uiFrame:SetAnchorParamHor(80.0, -80.0)
	uiFrame:SetAnchorVer(0.0, 1.0)
    uiFrame:SetAnchorParamVer(80.0, -80.0)
    btnClose:SetScriptHandler("zo_UICallabck")

    -- tab frame
    local frameTab = UITabFrame:New()
    uiFrame:AttachChild(frameTab)
    frameTab.LocalTransform:SetTranslateY(-1.0)
    frameTab:SetAnchorHor(0.0, 1.0)
    frameTab:SetAnchorVer(0.0, 1.0)
    frameTab:SetAnchorParamVer(0.0, -60.0)
    frameTab:SetTabBarHeight(42.0)
    frameTab:SetTabHeight(42.0)
    frameTab:SetFontColor(Float3.WHITE)

    -- local frame = zo_SettingFrame()
    -- frameTab:AddTab("Setting", PX2_LM_APP:GetValue("Setting"), frame)
    -- local txt = frameTab:GetTabButton("Setting"):GetText()
    -- zo_UISetTextFont(txt, 16)

    local frameDevice = zo_DeviceFrame()
    frameTab:AddTab("Device", PX2_LM_APP:GetValue("Device"), frameDevice)
    local txt = frameTab:GetTabButton("Device"):GetText()
    zo_UISetTextFont(txt, 16)

    local frameBle = zo_ConnectBleSerial()
    frameTab:AddTab("BleSerConnect", PX2_LM_APP:GetValue("BleSerConnect"), frameBle)
    local txt = frameTab:GetTabButton("BleSerConnect"):GetText()
    zo_UISetTextFont(txt, 16)

    local frameLidar = zo_ConnectLidar()
    frameTab:AddTab("Lidar", PX2_LM_APP:GetValue("Lidar"), frameLidar)
    local txt = frameTab:GetTabButton("Lidar"):GetText()
    zo_UISetTextFont(txt, 16)

    frameTab:SetActiveTab("Device")

    -- setting frame
    ZERONE_FrameSetting = uiFrameBack
    self._background:AttachChild(uiFrameBack)
    uiFrameBack.LocalTransform:SetTranslateY(-10.0)
    ZERONE_FrameSetting:Show(false)
end

-- functions

-- 设置名称
function ZERONEUIFaceCtrl:SetName(name)
    self._name = name
end

function ZERONEUIFaceCtrl:GetName()
    return self._name
end

function ZERONEUIFaceCtrl:SetID(id)
    self._id = id
end

function ZERONEUIFaceCtrl:GetID()
    return self._id
end

-- 设置背景色
function ZERONEUIFaceCtrl:SetBackColor(color)
    self._background:GetBackgroundPicBox():SetColor(color)
end

-- 设置CircleOut颜色
function ZERONEUIFaceCtrl:SetEyeLeftColor(color)
    self._EyeLeft:SetColor(color)
end
-- 设置CircleOut大小
function ZERONEUIFaceCtrl:SetEyeLeftSize(size)
    self._EyeLeft:SetSize(size, size)
end

-- 设置CircleIn颜色
function ZERONEUIFaceCtrl:SetEyeRightColor(color)
    self._EyeRight:SetColor(color)
end
-- 设置CircleIn大小
function ZERONEUIFaceCtrl:SetEyeRightSize(size)
    self._EyeRight:SetSize(size, size)
end

-- 设置Face颜色
function ZERONEUIFaceCtrl:SetFaceColor(color)

end
-- 设置Face大小
function ZERONEUIFaceCtrl:SetFaceSize(size)

end

-- 设置文字
function ZERONEUIFaceCtrl:SetText(text)
    self._text:GetText():SetText(text)
end

-- 待机
function ZERONEUIFaceCtrl:StartUp()
    self._EyeLeftCtrl:Clear()
    self._EyeLeftCtrl:AddPoint(0.0, 0.0001, ICM_LINEAR) --ICM_LINEAR ICM_CURVE_AUTO
    self._EyeLeftCtrl:AddPoint(zo_MT, 1.05, ICM_LINEAR)   
    self._EyeLeftCtrl:AddPoint(zo_LT, 1.0, ICM_LINEAR)   
    self._EyeLeftCtrl.MaxTime = zo_LT
    self._EyeLeftCtrl.Repeat = Controller.RT_CLAMP
    self._EyeLeftCtrl:SetDelayPlayTime(0.0)
    self._EyeLeftCtrl:ResetPlay()

    self._EyeRightCtrl:Clear()
    self._EyeRightCtrl:AddPoint(0.0, 0.0001, ICM_LINEAR) --ICM_LINEAR ICM_CURVE_AUTO
    self._EyeRightCtrl:AddPoint(zo_MT, 1.2, ICM_LINEAR)   
    self._EyeRightCtrl:AddPoint(zo_LT, 1.0, ICM_LINEAR)   
    self._EyeRightCtrl.MaxTime = zo_LT
    self._EyeRightCtrl.Repeat = Controller.RT_CLAMP
    self._EyeRightCtrl:SetDelayPlayTime(0.4)
    self._EyeRightCtrl:ResetPlay()
end

-- 冰冻
function ZERONEUIFaceCtrl:Freeze()
    self._EyeLeftCtrl:Clear()
    self._EyeLeftCtrl:AddPoint(0.0, 1.0, ICM_CURVE_AUTO) --ICM_LINEAR ICM_CURVE_AUTO
    self._EyeLeftCtrl.MaxTime = 1.0
    self._EyeLeftCtrl.Repeat = Controller.RT_WRAP
    self._EyeLeftCtrl:ResetPlay()

    self._EyeRightCtrl:Clear()
    self._EyeRightCtrl:AddPoint(0.0, 1.0, ICM_CURVE_AUTO) --ICM_LINEAR ICM_CURVE_AUTO
    self._EyeRightCtrl.MaxTime = 1.0
    self._EyeRightCtrl.Repeat = Controller.RT_WRAP
    self._EyeRightCtrl:ResetPlay()
end

-- 待机
function ZERONEUIFaceCtrl:Idle()
    self._EyeLeftCtrl:Clear()
    self._EyeLeftCtrl:AddPoint(0.0, 1.0, ICM_CURVE_AUTO) --ICM_LINEAR ICM_CURVE_AUTO
    self._EyeLeftCtrl:AddPoint(1.5, 1.05, ICM_CURVE_AUTO)   
    self._EyeLeftCtrl:AddPoint(3.0, 1.0, ICM_CURVE_AUTO)   
    self._EyeLeftCtrl.MaxTime = 3.0
    self._EyeLeftCtrl.Repeat = Controller.RT_WRAP
    self._EyeLeftCtrl:ResetPlay()

    self._EyeRightCtrl:Clear()
    self._EyeRightCtrl:AddPoint(0.0, 1.0, ICM_CURVE_AUTO) --ICM_LINEAR ICM_CURVE_AUTO
    self._EyeRightCtrl:AddPoint(1.5, 0.9, ICM_CURVE_AUTO)   
    self._EyeRightCtrl:AddPoint(3.0, 1.0, ICM_CURVE_AUTO)   
    self._EyeRightCtrl.MaxTime = 3.0
    self._EyeRightCtrl.Repeat = Controller.RT_WRAP
    self._EyeRightCtrl:ResetPlay()
end

-- 兴奋
function ZERONEUIFaceCtrl:Excite()
end

-- 音乐
function ZERONEUIFaceCtrl:MusicAdd(index, musicPath)
    self._musics[index] = musicPath
end

function ZERONEUIFaceCtrl:MusicReWrite()
    for index,value in ipairs(self._musics) do
        local pathStr = value
        local musicName = index..".mp3"

        local dstPath = ResourceManager:GetWriteablePath()..musicName
        if not PX2_RM:IsFileFloderExist(dstPath) then
            PX2_RM:ReWriteFileToWriteablePath(pathStr, musicName)
        end      
    end
end

function ZERONEUIFaceCtrl:MusicPlayRandom()
    local tabSize = table.getn(self._musics)
    ZERONE_MusicsPlayIndex = Mathf:IntRandom(1, tabSize+1)   
    self:MusicPlay(ZERONE_MusicsPlayIndex)
end

function ZERONEUIFaceCtrl:MusicPlay(index)
    local tabSize = table.getn(self._musics)

    if index>=1 and index<=tabSize then
        local dstPath = ResourceManager:GetWriteablePath()..index..".mp3"
        PX2_SS:PlayMusic(0, dstPath, true, 0.0)
    end
end

function ZERONEUIFaceCtrl:MusicStop()
    ZERONE_MusicsPlayIndex = 1
	PX2_SS:PlayMusic(0, nil, true, 0.0)
end

function ZERONEUIFaceCtrl:MusicBefore()
    local tabSize = table.getn(self._musics)
    ZERONE_MusicsPlayIndex = ZERONE_MusicsPlayIndex - 1
    if ZERONE_MusicsPlayIndex < 1 then
        ZERONE_MusicsPlayIndex = tabSize
    end
    if ZERONE_MusicsPlayIndex > tabSize then
        ZERONE_MusicsPlayIndex = 1
    end    
    self:MusicPlay(ZERONE_MusicsPlayIndex)
end

function ZERONEUIFaceCtrl:MusicNext()
    local tabSize = table.getn(self._musics)
    ZERONE_MusicsPlayIndex = ZERONE_MusicsPlayIndex + 1
    if ZERONE_MusicsPlayIndex < 1 then
        ZERONE_MusicsPlayIndex = tabSize
    end
    if ZERONE_MusicsPlayIndex > tabSize then
        ZERONE_MusicsPlayIndex = 1
    end    
    self:MusicPlay(ZERONE_MusicsPlayIndex)
end

function ZERONEUIFaceCtrl:SoundPlay(index)
    if 0<=index and index<=6 then
        local dstPath = "Data/ZERONE/media/sounds/not"..index..".wav"
        PX2_SS:PlayASound(dstPath, 1.0, 5.0)
    end
end

-- camera
function ZERONEUIFaceCtrl:OpenCamera(isFront)
    -- body
    if isFront then
        PX2_HARDCAMERA:OpenCamera(1)
        _vlc:SetHorSpaw(true)
    else
        PX2_HARDCAMERA:OpenCamera(0)
        _vlc:SetHorSpaw(false)
    end

    _vlc:Show(true)

    self._isCameraOpen = true
end

function ZERONEUIFaceCtrl:CloseCamera()
    if nil~=_vlc then
        PX2_HARDCAMERA:CloseCamera()
        _vlc:Show(false)
    end

    self._isCameraOpen = false
end

function ZERONEUIFaceCtrl:IsCameraOpened()
    return self._isCameraOpen
end

-- touch
function ZERONEWidgetPicked(ptr)
    local obj = Cast:ToO(ptr)    
end

function ZERONEFrameSizeNodePicked(ptr)
    local obj = Cast:ToO(ptr)    
end

function ZERONEFrameSizeNodeNotPicked(ptr)
    local obj = Cast:ToO(ptr)    
end

function zo_UICallabck(ptr, callType)
    local obj = Cast:ToO(ptr) 
	local name = obj:GetName()	

    if UICT_PRESSED==callType then
        playFrameScale(obj)

        if "BtnVoice"==name then
            PX2_VOICESDK:StartVoiceListening()
        end
	elseif UICT_RELEASED==callType then
        playFrameNormal(obj)

        if "BtnSetting"==name then
            ZERONE_FrameSetting:Show(true)
            -- ZERONE_EditBoxName:SetText("".. ZERONE_UIFaceCtrlScript:GetName())
            -- ZERONE_EditBoxID:SetText("".. ZERONE_UIFaceCtrlScript:GetID())
        elseif "BtnCamera"==name then
            local platform = PX2_APP:GetPlatformType()
            if Application.PLT_ANDROID == platform then
                if not ZERONE_UIFaceCtrlScript:IsCameraOpened() then
                    ZERONE_UIFaceCtrlScript:OpenCamera(0)
                else
                    ZERONE_UIFaceCtrlScript:CloseCamera()
                end
            elseif Application.PLT_LINUX == platform then
                System:CallGetStatus("sh ../PHOENIXFrame/Raspberry/MJPG-Streamer/mjpg-streamer-experimental/start.sh")
            end
        elseif "BtnVoice"==name then
            PX2_VOICESDK:EndVoiceListening()
        elseif "BtnDlgClose"==name then
            ZERONE_FrameSetting:Show(false)
        elseif "Btn3D"==name then
            if not ZERONE_IsZERO3DSceneCreated then
                zo_Scene()
            end
        end
    elseif UICT_CHECKED==callType then
        if "BtnShowFace"==name then
            ZERONE_UIFaceCtrlScript._content:Show(true)
        elseif "BtnLidar"==name then
            ZERONE_UIFaceCtrlScript._MapFrame:Show(true)
        elseif "BtnLidarCur"==name then
            ZERONE_UIFaceCtrlScript._lidarFrame:Show(true)
        elseif "BtnUICtrl"==name then
            ZERONE_FrameUICtrl:Show(true)
        end    
    elseif UICT_DISCHECKED==callType then
        if "BtnShowFace"==name then
            ZERONE_UIFaceCtrlScript._content:Show(false)
        elseif "BtnLidar"==name then
            ZERONE_UIFaceCtrlScript._MapFrame:Show(false)
        elseif "BtnLidarCur"==name then
            ZERONE_UIFaceCtrlScript._lidarFrame:Show(false)
        elseif "BtnUICtrl"==name then
            ZERONE_FrameUICtrl:Show(false)
        end
    elseif UICT_RELEASED_NOTPICK==callType then
        playFrameNormal(obj)
    elseif UICT_ROUND_RELEASEDNOTPICK==callType then
    elseif UICT_COMBOBOX_CHOOSED==callType then
        if "BtnComboxBoxEdit"==name then    
            local select = ZERONE_UIFaceCtrlScript._btnMapDraw:GetChoose()
            if 0==select then
                ZERONE_MAP_DrawType = 0
            elseif 1==select then
                ZERONE_MAP_DrawType = 1
            elseif 2==select then
                ZERONE_MAP_DrawType = 2
            end
        elseif "BtnComboxBoxChara"==name then
            local select = ZERONE_UIFaceCtrlScript._btnChara:GetChoose()
            if 0==select then
                rc_Robot:SetRoleType(Robot.RT_MASTER)
            elseif 1==select then
                rc_Robot:SetRoleType(Robot.RT_CONNECTOR)
            elseif 2==select then
                rc_Robot:SetRoleType(Robot.RT_MASTER_ONLY_SENDLIDAR)
            elseif 3==select then
                rc_Robot:SetRoleType(Robot.RT_CONNECTOR_CALCULATE)
            end
        elseif "BtnComboxBoxMotoType"==name then
            local select = ZERONE_UIFaceCtrlScript._btnMotoType:GetChoose()
            if 0==select then
                ZERONE_MotoType = 0
            elseif 1==select then
                ZERONE_MotoType = 1
            end
        end  
	end
end