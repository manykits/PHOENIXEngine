-- DecisionEvaluator.lua
-- 求值器/判定器

Evaluator = {};

Evaluator.Type = "Evaluator";

function Evaluator.Evaluate(self)
    return self.function_(self.userData_);
end

function Evaluator.New(name, evalFunction, userData)
    local evaluator = {};
    
    evaluator.function_ = evalFunction;
    evaluator.name_ = name or "";
    evaluator.type_ = Evaluator.Type;
    evaluator.userData_ = userData;
    
    evaluator.evaluate_ = Evaluate;
    
    return evaluator;
end