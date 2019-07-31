-- FiniteStateTransition.lua
-- 转换器包装了要转换到的状态和用来确定转换是否执行的求值器

FiniteStateTransition = {};

function FiniteStateTransition.New(toStateName, evaluator)
    local transition = {};
    
    transition.evaluator_ = evaluator;
    transition.toStateName_ = toStateName;
    
    return transition;
end