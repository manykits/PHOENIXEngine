-- ActorCtrl.lua

ActorCtrl = class(LuaScriptController,
{
	_name = "ActorCtrl",
	_val = 10,
	_scriptControl = nil,
	_ctrlable = nil,
});

function ActorCtrl:OnAttached()
	PX2_LOGGER:LogInfo("script_lua", ""..self._name.." OnAttached")
	
	self._scriptControl = Cast:ToSC(self.__object)
	self._ctrlable = self._scriptControl:GetControlledable()
end

function ActorCtrl:OnInitUpdate()
	PX2_LOGGER:LogInfo("script_lua", ""..self._name.." OnInitUpdate")
	PX2_LOGGER:LogInfo("script_lua", ""..self._name.." "..self._val)

	if nil~= self._ctrlable and self._name=="ActorCtrl" then
		PX2_LOGGER:LogInfo("script_lua", ""..self._name.." ResetPlay")
		PX2_LOGGER:LogInfo("script_lua", "_ctrlable name:"..self._ctrlable:GetName())
		
		self._ctrlable:ResetPlay()
	end
end

function ActorCtrl:OnPPlay()
	PX2_LOGGER:LogInfo("script_lua", ""..self._name.." OnPPlay")
end