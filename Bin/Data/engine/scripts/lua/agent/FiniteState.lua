-- FiniteState.lua
-- 状态
-- 状态用于吧一个动作关联到状态上，包含一个动作和一个名称

FiniteState = {};

function FiniteState.New(name, action)
    local state = {};
    
    state.name_ = name;
    state.action_ = action;
    
    return state;
end