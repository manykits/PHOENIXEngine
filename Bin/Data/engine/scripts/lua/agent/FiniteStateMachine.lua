-- FiniteStateMachine.lua
-- 有限状态机
-- 状态转换使用求值器自动处理，一旦求值器返回真，状态机就会转换到新的状态，
-- 并调用相关的动作
-- 优点
-- 状态机的直观性依赖于大量的数据，这种结构的有点在于，智能体的状态是他的逻辑结构
-- 的固有属性。与决策树对比，有限的状态限制了从一个动作能转换到的其他可能动作的数量。
-- 如果想要在决策树中创建例似的动作转换比较困难，可能需要嵌入某种用户数据来维护
-- 智能体的状态而，状态机本身就有这样的功能。
-- 缺点
-- 随着新状态数量的增加，可能的连接数量会成指数级增长。为了延迟或者非预想的动作链，
-- 需要仔细考虑状态机中的连接，这样才能保证智能体款速地选择到最佳动作。

FiniteStateMachine = {};

local function EvaluateTransitions(self, transitions)
    for index = 1, #transitions do
        if (transitions[index].evaluator_(self.userData_)) then
            return transitions[index].toStateName_;
        end
    end
end

function FiniteStateMachine.AddState(self, name, action)
    self.states_[name] = FiniteState.New(name, action);
end

function FiniteStateMachine.AddTransition(
    self, fromStateName, toStateName, evaluator)
    -- Ensure both states exist within the FSM.
    if (self:ContainsState(fromStateName) and self:ContainsState(toStateName)) then

        if (self.transitions_[fromStateName] == nil) then
            self.transitions_[fromStateName] = {};
        end

        table.insert(
            self.transitions_[fromStateName],
            FiniteStateTransition.New(toStateName, evaluator));
    end
end

function FiniteStateMachine.ContainsState(self, stateName)
    return self.states_[stateName] ~= nil;
end

function FiniteStateMachine.ContainsTransition(self, fromStateName, toStateName)
    return self.transitions_[fromStateName] ~= nil and
        self.transitions_[fromStateName][toStateName] ~= nil;
end

function FiniteStateMachine.GetCurrentStateName(self)
    if (self.currentState_) then
        return self.currentState_.name_;
    end
end

function FiniteStateMachine.GetCurrentStateStatus(self)
    if (self.currentState_) then
        return self.currentState_.action_.status_;
    end
end

function FiniteStateMachine.SetState(self, stateName)
    if (self:ContainsState(stateName)) then
        if (self.currentState_) then
            self.currentState_.action_:CleanUp();
        end
        
        self.currentState_ = self.states_[stateName];
        self.currentState_.action_:Initialize();
    end
end

function FiniteStateMachine.Update(self, deltaTimeInMillis)
    if (self.currentState_) then
        local status = self:GetCurrentStateStatus();
        
        if (status == Action.Status.RUNNING) then
            self.currentState_.action_:Update(deltaTimeInMillis);
        elseif (status == Action.Status.TERMINATED) then
            local toStateName =
                EvaluateTransitions(self, self.transitions_[self.currentState_.name_]);
            if (self.states_[toStateName] ~= nil) then
                self.currentState_.action_:CleanUp();
                self.currentState_ = self.states_[toStateName];
                self.currentState_.action_:Initialize();
            end
        end
    end
end

function FiniteStateMachine.New(userData)
    local fsm = {};

    fsm.currentState_ = nil;
    fsm.states_ = {};
    fsm.transitions_ = {};
    fsm.userData_ = userData;

    fsm.AddState = FiniteStateMachine.AddState;
    fsm.AddTransition = FiniteStateMachine.AddTransition;
    fsm.ContainsState = FiniteStateMachine.ContainsState;
    fsm.ContainsTransition = FiniteStateMachine.ContainsTransition;
    fsm.GetCurrentStateName = FiniteStateMachine.GetCurrentStateName;
    fsm.GetCurrentStateStatus = FiniteStateMachine.GetCurrentStateStatus;
    fsm.SetState = FiniteStateMachine.SetState;
    fsm.Update = FiniteStateMachine.Update;

    return fsm;
end