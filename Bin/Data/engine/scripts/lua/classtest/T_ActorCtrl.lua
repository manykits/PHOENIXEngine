-- ActorCtrl.lua

T_ActorCtrl = class(nil,
{
	_name = "unnamed",
	_val0 = 10,
	_val1 = 11,
	_isLoggedPUpdate = false
});

function T_ActorCtrl:OnAttached()
	PX2_LOGGER:LogInfo("script_lua", "T_ActorCtrl:OnAttached")
end

function T_ActorCtrl:OnInitUpdate()
	PX2_LOGGER:LogInfo("script_lua", "T_ActorCtrl:OnInitUpdate")
end

function T_ActorCtrl:OnPPlay()
	PX2_LOGGER:LogInfo("script_lua", "T_ActorCtrl:OnPPlay")
end

function T_ActorCtrl:OnPUpdate()
	if false==_isLoggedPUpdate then
		PX2_LOGGER:LogInfo("script_lua", "T_ActorCtrl:OnPUpdate")
		_isLoggedPUpdate = true
	end
end