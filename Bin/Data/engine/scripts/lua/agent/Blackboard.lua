-- Blackboard.lua

Blackboard = {};

local function EvaluateSources(self, sources)
    local bestConfidence = 0;
    local bestResult = nil;
    
    for index = 1, #sources do
        local eval = sources[index]:Evaluate(self.userData_);
        
        if (eval.confidence > bestConfidence) then
            bestConfidence = eval.confidence;
            bestResult = eval.evaluation;
        end
    end
    
    return bestResult;
end

function Blackboard.AddSource(self, attribute, source)
    if (not self.sources_[attribute]) then
        self.sources_[attribute] = {};
    end
    
    table.insert(self.sources_[attribute], source);
end

function Blackboard.Get(self, attribute)
    if (self.sources_[attribute]) then
        return EvaluateSources(self, self.sources_[attribute]);
    end

    return self.attributes_[attribute];
end

function Blackboard.RemoveSource(self, attribute, source)
    if (self.sources_[attribute]) then
        for index = 1, #self.sources_[attribute] do
            if (self.sources_[attribute][index] == source) then
                table.remove(self.sources_[attribute], index);
                return true;
            end
        end
    end
    
    return false;
end

function Blackboard.Set(self, attribute, value)
    self.attributes_[attribute] = value;
end

function Blackboard.new(userData)
    local blackboard = {};
    
    blackboard.attributes_ = {};
    blackboard.sources_ = {};
    blackboard.userData_ = userData;
    
    blackboard.AddSource = Blackboard.AddSource;
    blackboard.Get = Blackboard.Get;
    blackboard.RemoveSource = Blackboard.RemoveSource;
    blackboard.Set = Blackboard.Set;
    
    return blackboard;
end