-- BehaviorTree.lua
-- 行为树完全由不同的节点构成，更具节点类型不同，行为树将子节点看做动作或者求值器。
-- 节点类型包含：动作，条件，选择器，序列。
-- 动作：动作只会在智能体上执行一个行为，他不应该有任何子节点。他们是行为树的叶子节点。
-- 条件：条件节点和动作节点例似，也是行为树的叶子节点。条件节点执行赋值给它的求值器，
--     	然后返回结果给调用者来决定他们应该怎样被处理。
-- 选择器：选择器是一种拥有子节点的节点类型。选择器可以拥有任意数量的子节点，但只会执行
--		第一个可以执行的子节点。本质上如同if if...else else 结构。如果有至少一个子节点
--		可以运行返回真，否则返回假。相当于
-- 序列：序列节点是一个顺序执行动作，他会按照顺序执行它的每个子节点，直到有一个条件，选择器
-- 		或者子序列执行失败为止。如果所有子节点都运行成功，序列则返回真，如果任何一个子节点
--		返回假，则序列立即退出并返回假。本质上相当于if if...else else内部执行的条件判断加
--		执行内容

BehaviorTree = {};

local _EvaluateSelector;
local _EvaluateSequence;


_EvaluateSelector = function(self, node, deltaTimeInMillis)
    for index = 1, #node.children_ do
        local child = node:GetChild(index);
        
        if (child.type_ == BehaviorTreeNode.Type.ACTION) then
            return { node = child, result = true};
        elseif (child.type_ == BehaviorTreeNode.Type.CONDITION) then
            assert(false);
            return { result = false };
        elseif (child.type_ == BehaviorTreeNode.Type.SELECTOR) then
            local result = _EvaluateSelector(self, child, deltaTimeInMillis);
            if (result.result) then
                return result;
            end
        elseif (child.type_ == BehaviorTreeNode.Type.SEQUENCE) then
            local result = _EvaluateSequence(self, child, deltaTimeInMillis);
            if (result.result) then
                return result;
            end
        end
    end
    
    return { result = false };
end

_EvaluateSequence = function(self, node, deltaTimeInMillis, index)
    index = index or 1;

    for count=index, #node.children_ do
        local child = node:GetChild(count);
        
        if (child.type_ == BehaviorTreeNode.Type.ACTION) then
            return { node = child, result = true};
        elseif (child.type_ == BehaviorTreeNode.Type.CONDITION) then
            local result = child.evaluator_(self.userData_);
            if (not child.evaluator_(self.userData_)) then
                return { result = false };
            end
        elseif (child.type_ == BehaviorTreeNode.Type.SELECTOR) then
            local result = _EvaluateSelector(self, child, deltaTimeInMillis);
            
            if (not result.result) then
                return { result = false };
            elseif (result.result and result.node ~= nil) then
                return result;
            end
        elseif (child.type_ == BehaviorTreeNode.Type.SEQUENCE) then
            local result = _EvaluateSequence(self, child, deltaTimeInMillis);
            
            if (not result.result) then
                return { result = false };
            elseif (result.result and result.node ~= nil) then
                return result;
            end
        end
        
        -- Move to the next child to execute.
        count = count + 1;
    end
    
	-- 没有行为要执行，返回成功
    return { result = true };
end

local function _EvaluateNode(self, node, deltaTimeInMillis)
    if (node.type_ == BehaviorTreeNode.Type.ACTION) then
        return node;
    elseif (node.type_ == BehaviorTreeNode.Type.CONDITION) then
		-- 不会运行到这里，出错了
        assert(false);
    elseif (node.type_ == BehaviorTreeNode.Type.SELECTOR) then
        local result = _EvaluateSelector(self, node, deltaTimeInMillis);
        if (result.result) then
            return result.node;
        end
    elseif (node.type_ == BehaviorTreeNode.Type.SEQUENCE) then
        local result = _EvaluateSequence(self, node, deltaTimeInMillis);
        if (result.result) then
            return result.node;
        end
    end
end

local function _ContinueEvaluation(self, node, deltaTimeInMillis)
    local parentNode = node:GetParent();
    local childNode = node;

    while (parentNode ~= nil) do
        if (parentNode.type_ == BehaviorTreeNode.Type.SEQUENCE) then
		
            local childIndex = parentNode:ChildIndex(childNode);
            
			-- 执行队列子节点
            if (childIndex < parentNode:GetNumberOfChildren()) then
                return _EvaluateSequence(
                    self,
                    parentNode,
                    deltaTimeInMillis,
                    childIndex + 1);
            end
        end
        
        -- 返回队列父亲节点
        childNode = parentNode;
        parentNode = childNode:GetParent();
    end
end

function BehaviorTree.SetNode(self, node)
    self.node_ = node;
end

function BehaviorTree.Update(self, deltaTimeInMillis)
	-- 找到当前可执行动作
    if (self.currentNode_ == nil) then
        self.currentNode_ = _EvaluateNode(self, self.node_, deltaTimeInMillis);
    end

    if (self.currentNode_ ~= nil) then
        local status = self.currentNode_.action_.status_;
        
        if (status == Action.Status.UNINITIALIZED) then
            self.currentNode_.action_:Initialize();
        elseif (status == Action.Status.TERMINATED) then
            self.currentNode_.action_:CleanUp();

			-- 动作执行完，回溯父亲节点,获得需要执行的新的枝
            self.currentNode_ = _ContinueEvaluation(
                self, self.currentNode_, deltaTimeInMillis);
        elseif (status == Action.Status.RUNNING) then
            self.currentNode_.action_:Update(deltaTimeInMillis);
        end
    end
end

function BehaviorTree.New(userData)
    local tree = {};
    
    tree.currentNode_ = nil;
    tree.node_ = nil;
    tree.userData_ = userData;
    
    tree.SetNode = BehaviorTree.SetNode;
    tree.Update = BehaviorTree.Update;
    
    return tree;
end