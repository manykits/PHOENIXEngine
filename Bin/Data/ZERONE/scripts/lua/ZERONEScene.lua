-- ZERONEScene.lua

function zo_Scene()
    ZERONE_IsZERO3DSceneCreated = true
 
    -- start slam
    --rc_Robot:StartShareMemory()

    -- new scene
    PX2_APP:NewScene()
    local scene = PX2_PROJ:GetScene()
    scene:ResetPlay()

    -- new root node
    local nodeScene = Node:New("SceneFrame")
    scene:AttachChild(nodeScene)

    --[[
    -- for test not useful
    for k, v in pairs(gScriptTable) do
        PX2_LOGGER:LogInfo("script_lua", "gScriptTable:"..k)
    end	
    local actorScript1 = gScriptTable[actorController:SelfP()]
    actorScript1._val = 1002
    PX2_LOGGER:LogInfo("script_lua", "ActorCtrl._val:"..actorCtrlScript._val)
    ]]--

    -- cam model
    local mainCameraNode = scene:GetMainCameraNode()
    local cameraPlayCtrl = PX2_CREATER:CreateCameraPlayController()
    nodeScene:AttachController(cameraPlayCtrl)
    cameraPlayCtrl:SetCameraNode(mainCameraNode)
    cameraPlayCtrl:SetCameraDistance(40.0)
    cameraPlayCtrl:SetCameraDist(1.0, 60.0)
    cameraPlayCtrl:ResetPlay()
    local ui = PX2_PROJ:GetUI()
    local frameTouch = UIFrame:New()
    ui:AttachChild(frameTouch)
    frameTouch:SetAnchorHor(0.0, 1.0)
    frameTouch:SetAnchorVer(0.0, 1.0)
    frameTouch.LocalTransform:SetTranslateY(-2.0)
    cameraPlayCtrl:SetTouchSizeNode(frameTouch)
    
    -- actor controlSetLineValueAtPos
    -- local actorBox = PX2_CREATER:CreateActorBox()
    -- actorBox.LocalTransform:SetTranslateY(4.0)
    -- local actorBoxAgentBase = actorBox:GetAIAgentBase()
    -- actorBoxAgentBase:SetMass(0)
    -- actorBoxAgentBase:SetMassZeroAvoid(true)
    -- actorBoxAgentBase:SetRadius(0.1)
    -- actorBoxAgentBase:SetPosition(APoint(0.0, 1.0, 0.0))
    -- scene:AttachChild(actorBox)
    -- actorBoxAgentBase:ResetPlay()

    local actor = PX2_CREATER:CreateActor()
    scene:AttachChild(actor)
    local movBox = PX2_CREATER:CreateMovable_Box()
    actor:AttachChild(movBox)
    movBox.LocalTransform:SetUniformScale(0.2)
    local localAgent = actor:GetAIAgent()
    localAgent:SetMaxForce(2.0)
	localAgent:SetMass(2.0)
	localAgent:SetRadius(0.1)
	localAgent:SetPhysicsRadius(0.00001)
	localAgent:SetHeight(0.1)
	localAgent:SetMaxSpeed(0.2)
    localAgent:ResetPlay()    
    local robot = rc_Robot
    localAgent:SetRobot(robot)

    -- actor rect
    -- local actorRect = PX2_CREATER:CreateActor_InfinitePlane(10.0)
    -- actorRect.LocalTransform:SetTranslateZ(-0.01)
    -- scene:AttachChild(actorRect)
    -- actorRect:SetTexture("Data/ZERONE/images/floor.png", Float4(0.0, 0.0, 4.0, 4.0))

    -- voxel
    PX2_VOXELM:Initlize(VoxelManager.T_TEX)
    local vs = VoxelSection:New()
    vs:SetMinSize(-3, -3, 0)
    vs:SetMaxSize(3, 3, 0)
    vs:SetDistXY(10)
    vs:SetDistZ(10)
    vs:SetInitMtlType(0)
    scene:AttachChild(vs)
    rc_Robot:SetVoxelSection(vs)

    -- sky
    --local sky = PX2_CREATER:CreateSkyBox("Data/ZERONE/images/skynew/")
    --scene:AttachChild(sky)
end

function zo_SceneAddBox()
    local scene = PX2_PROJ:GetScene()

    local actor = PX2_CREATER:CreateActorBox()
    scene:AttachChild(actor)
    actor:GetAIAgentBase():SetPosition(APoint(0.5, 0.95, 5.0))
    actor:GetAIAgentBase():ResetPlay()
end