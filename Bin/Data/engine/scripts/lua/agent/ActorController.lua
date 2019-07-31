-- ActorController.lua

ActorController = class(LuaScriptController,
{
	_name = "ActorController",
	_scriptControl = nil,
	_ctrlable = nil,
	_node = nil,

	_actor = nil,
	_agent = nil,
})

function ActorController:OnAttached()
	--PX2_LOGGER:LogInfo("script_lua", ""..self._name.." OnAttached")

	self._scriptControl = Cast:ToSC(self.__object)
	self._ctrlable = self._scriptControl:GetControlledable()
	self._node = Cast:ToNode(self._ctrlable)
	self._actor = Cast:ToActor(self._ctrlable)
	self._agent = self._actor:GetAIAgent()
end

function ActorController:OnInitUpdate()
	PX2_LOGGER:LogInfo("script_lua", ""..self._name.." OnInitUpdate")
end

function ActorController:OnPPlay()
	PX2_LOGGER:LogInfo("script_lua", ""..self._name.." OnPPlay")
end

function ActorController:OnPUpdate()
	--PX2_LOGGER:LogInfo("script_lua", ""..self._name.." OnPUpdate")

	local elapsedSeconds = PX2_APP:GetElapsedTime()
	
	-- self
    local position = self._agent:GetPosition()
	local radius = self._agent:GetRadius()
    
	-- target
	local destination = self._agent:GetTarget()
	local seekForce = self._agent:ForceToPosition(destination)
	local targetRadius = self._agent:GetTargetRadius()
	
	-- avoid
	local avoidAgentForce = self._agent:ForceToAvoidAgents(2.5)
    local avoidObjectForce = self._agent:ForceToAvoidObjects(2.5)
	local avoidanceMultiplier = 10
	local agentForce = avoidAgentForce:Dot(avoidanceMultiplier)
	local objForce = avoidObjectForce:Dot(avoidanceMultiplier)
	local force0 = seekForce:Add(agentForce)
	local steeringForces = force0:Add(objForce)
	
	Agent_ApplyPhysicsSteeringForce(self._agent, steeringForces, elapsedSeconds)
	Agent_ClampHorizontalSpeed(self._agent)
end