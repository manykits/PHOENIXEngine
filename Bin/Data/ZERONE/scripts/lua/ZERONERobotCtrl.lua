-- ZERONERobotCtrl.lua

MoveType = 
{
	MT_NONE = 1,
	MT_FORWARD = 2,
	MT_BACKWARD = 3,
	MT_LEFT = 4,
	MT_RIGHT = 5,
}

ZERONERobotCtrl = class(LuaScriptController,
{
	_name = "ZERONERobotCtrl",
	_val = 10,
	_scriptControl = nil,
	_ctrlable = nil,
	_node = nil,
	_moveType = MoveType.MT_NONE,
	_IsLeftPressed = false,
	_IsRightPressed = false,
	_IsUpPressed = false,
	_IsDownPressed = false,
	_IsSpacePressed = false,
	_IsDirectionChanged = false,
})

function ZERONERobotCtrl:OnAttached()
	PX2_LOGGER:LogInfo("script_lua", ""..self._name.." OnAttached")

	g_ZERO3DCtrl = self
	
	self._scriptControl = Cast:ToSC(self.__object)
	self._ctrlable = self._scriptControl:GetControlledable()
	self._node = Cast:ToNode(self._ctrlable)
    
    -- model
	local obj = PX2_RM:BlockLoadCopy("Data/ZERONE/models/actors/robot/phoenixframe.px2obj")
    local model = Cast:ToMovable(obj)
    self._node:AttachChild(model)
end

function ZERONERobotCtrl:OnInitUpdate()
	PX2_LOGGER:LogInfo("script_lua", ""..self._name.." OnInitUpdate")
	PX2_LOGGER:LogInfo("script_lua", ""..self._name.." "..self._val)
end

function ZERONERobotCtrl:OnPPlay()
	PX2_LOGGER:LogInfo("script_lua", ""..self._name.." OnPPlay")
end

function ZERONERobotCtrl:OnPUpdate()
	local pos = rc_Robot:GetPosition()
	local newPos = APoint(pos:X(), pos:Y(), 0.0)
	
	local dir = rc_Robot:GetDirection()
    local up = rc_Robot:GetUp()
	local right = rc_Robot:GetRight()
	
	self._ctrlable.LocalTransform:SetTranslate(newPos)
	self._ctrlable.LocalTransform:SetRotate(right, dir, up)
	self._ctrlable.LocalTransform:SetUniformScale(0.1)
end
