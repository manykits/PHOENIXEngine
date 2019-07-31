-- DecisionTree.lua
-- 决策树
-- 优点
-- 决策树遵循一种二叉树的设计，每个分支只有一个动作和另外一个分支，
-- 树顶的子节点将包含两个不同的动作，相当于嵌套的一系列的ifelse过程
-- 任何可能情况都可以在决策树中进行建模，智能体能支持的每一个动作都会
-- 作为树的叶子节点出现
-- 缺点
-- 在复杂的逻辑条件下，必须考虑每一种可能性，当拥有大量的分支可能性时，
-- 决策树也需要平衡，否则树的某些部分最终可能会重复，增加树的复杂性

DecisionTree = {};

function DecisionTree.SetBranch(self, branch)
    self.branch_ = branch;
end

function DecisionTree.Update(self, deltaTimeInMillis)
    if (self.branch_ == nil) then
        return;
    end
	
    if (self.currentAction_ == nil) then
        self.currentAction_ = self.branch_:Evaluate();

        self.currentAction_:Initialize();
    end
    
    local status = self.currentAction_:Update(deltaTimeInMillis);
    
    -- Clean up the Action once it has terminated.
    if (status == Action.Status.TERMINATED) then
        self.currentAction_:CleanUp();
        self.currentAction_ = nil;
    end
end

function DecisionTree.New()
    local decisionTree = {};
    
    decisionTree.branch_ = nil;
    decisionTree.currentAction_ = nil;
    
    decisionTree.SetBranch = DecisionTree.SetBranch;
    decisionTree.Update = DecisionTree.Update;
    
    return decisionTree;
end