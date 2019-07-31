-- ActorControllerSeek.lua

ActorControllerSeek = class(ActorController,
{
	_name = "ActorControllerSeek",
})

function ActorControllerSeek:OnAttached()
	ActorController.OnAttached(self)

	self._agent:SetMaxForce(200.0)
	self._agent:SetMass(0.0)
	self._agent:SetRadius(0.3)
	self._agent:SetPhysicsRadius(0.15)
	self._agent:SetHeight(0.5)
	self._agent:SetMaxSpeed(0.8)

	self._agent:SetPosition(APoint(0.0, 3.0, 0.0))

	self._agent:SetTarget(APoint(0.0, 3.0, 0.0))
	self._agent:SetTargetRadius(0.1);
    self._agent:ResetPlay()
end

function ActorControllerSeek:OnPUpdate()
	--PX2_LOGGER:LogInfo("script_lua", ""..self._name.." OnPUpdate")
end