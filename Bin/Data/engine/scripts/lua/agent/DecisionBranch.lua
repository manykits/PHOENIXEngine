-- DecisionBranch.lua

DecisionBranch = {}

DecisionBranch.Type = "DecisionBranch";

function DecisionBranch.AddChild(self, child, index)
    index = index or (#self.children_ + 1);
    table.insert(self.children_, index, child);
end

function DecisionBranch.Evaluate(self)
    assert(self.evaluator_);

    local eval = self.evaluator_();

    assert(type(eval) == "number", "evaluator must return a number");
    assert(eval <= #self.children_, "evaluator must choose a child to execute");

    local choice = self.children_[eval];

    if (choice.type_ == DecisionBranch.Type) then
        return choice:Evaluate();
    else
		-- returned the action
		-- 这会是个执行的行为
        return choice;
    end
end

function DecisionBranch.SetEvaluator(self, evaluator)
    assert(type(evaluator) == "function", "evaluator must be a function");
    self.evaluator_ = evaluator;
end

function DecisionBranch.New()
    local branch = {};

    branch.children_ = {};
    branch.evaluator_ = nil;
    branch.type_ = DecisionBranch.Type;

    branch.AddChild = DecisionBranch.AddChild;
    branch.Evaluate = DecisionBranch.Evaluate;
    branch.SetEvaluator = DecisionBranch.SetEvaluator;

    return branch;
end