-- Action.lua

Action = {};

Action.Status =
{
    RUNNING = "RUNNING",
    TERMINATED = "TERMINATED",
    UNINITIALIZED = "UNINITIALIZED"
};

Action.Type = "Action";

function Action.CleanUp(self)
    if (self.status_ == Action.Status.TERMINATED) then
        if (self.cleanUpFunction_) then
            self.cleanUpFunction_(self.userData_);
        end
    end
    
    self.status_ = Action.Status.UNINITIALIZED;
end

function Action.Initialize(self)
    if (self.status_ == Action.Status.UNINITIALIZED) then
        if (self.initializeFunction_) then
            self.initializeFunction_(self.userData_);
        end
    end
    
    self.status_ = Action.Status.RUNNING;
end

function Action.Update(self, deltaTimeInMillis)
    if (self.status_ == Action.Status.TERMINATED) then
        return Action.Status.TERMINATED;
    elseif (self.status_ == Action.Status.RUNNING) then
        if (self.updateFunction_) then
            self.status_ = self.updateFunction_(deltaTimeInMillis, self.userData_);

            -- Ensure that a status was returned by the update function.
            -- 确保行为的执行函数，有一个返回值
			assert(self.status_);
        else
            self.status_ = Action.Status.TERMINATED;
        end
    end

    return self.status_;
end

function Action.New(name, initializeFunction, updateFunction, cleanUpFunction, userData)
    local action = {};
    
    action.cleanUpFunction_ = cleanUpFunction;
    action.initializeFunction_ = initializeFunction;
    action.updateFunction_ = updateFunction;
    action.name_ = name or "";
    action.status_ = Action.Status.UNINITIALIZED;
    action.type_ = Action.Type;
    action.userData_ = userData;
    
    action.CleanUp = Action.CleanUp;
    action.Initialize = Action.Initialize;
    action.Update = Action.Update;
    
    return action;
end