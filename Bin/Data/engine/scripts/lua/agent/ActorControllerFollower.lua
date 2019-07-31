-- ActorControllerFollower.lua

ActorControllerFollower = class(ActorController,
{
	_name = "ActorControllerFollower",
	_leader = nil
})

function ActorControllerFollower:OnAttached()
	ActorController.OnAttached(self)
    self._agent:ResetPlay()
end


function ActorControllerFollower:OnPUpdate()
	if nil== self._leader then
		return
	end

	local elapsedSeconds = PX2_APP:GetElapsedTime()
	
	-- self
    local position = self._agent:GetPosition()
	local radius = self._agent:GetRadius()
    
	-- combine
	local forceToCombine = self._agent:ForceToCombine(100, 180, self._leader);
	
	-- sparate
	local forceToSeparate = self._agent:ForceToSeparateAgents(5, 180);

	-- align
    local forceToAlign = self._agent:ForceToSeparate(6, 45, self._leader);
	
	-- avoid
	local avoidAgentForce = self._agent:ForceToAvoidAgents(3.5)
    local avoidObjectForce = self._agent:ForceToAvoidObjects(3.5)
	local avoidanceMultiplier = 1.5
	local agentForce = avoidAgentForce:Dot(avoidanceMultiplier)
	local objForce = avoidObjectForce:Dot(avoidanceMultiplier)
	
	-- all force
	local steeringForces = forceToCombine
	steeringForces = steeringForces:Add(forceToSeparate)
	steeringForces = steeringForces:Add(forceToAlign)
	steeringForces = steeringForces:Add(agentForce)
	steeringForces = steeringForces:Add(objForce)
	
	Agent_ApplyPhysicsSteeringForce(self._agent, steeringForces, elapsedSeconds)
	Agent_ClampHorizontalSpeed(self._agent)
end