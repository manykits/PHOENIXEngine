-- engine_agent.lua

function Agent_ApplyPhysicsSteeringForce(agent, steeringForce, deltaTimeInSeconds)
	-- 微小的力去除
	local forceLength = steeringForce:SquaredLength()
	if forceLength < 0.00001 then
        return
    end
	
	-- ingore mass
	-- 移除0质量物体
	if (agent:GetMass() <= 0) then
        return
    end

    -- cal force
	-- 计算力量
	steeringForce:SetZ(0)
	steeringForce:Normalize()
    steeringForce = steeringForce:Dot(agent:GetMaxForce())
    agent:ApplyForcing(steeringForce);

    local robot = agent:GetRobot()
    if nil==robot then
	    -- cal speed
	    -- 计算速度
        local acceleration = steeringForce:Over(agent:GetMass())
        local currentVelocity = agent:GetVelocity()
        local newVelocity = currentVelocity:Add(acceleration:Dot(deltaTimeInSeconds))
        newVelocity:SetZ(0)

        agent:SetForwarding(newVelocity)
    end
end

-- make speed in a fixed range
-- 将速度限制在一个固定的范围
function Agent_ClampHorizontalSpeed(agent)
    local robot = agent:GetRobot()
    if nil==robot then
        local velocity = agent:GetVelocity();
        local velocZTemp = velocity:Z();
        velocity:SetZ(0);

        local maxSpeed = agent:GetMaxSpeed();
        local squaredSpeed = maxSpeed * maxSpeed;
        
        -- to fix
        local sqLen = velocity:SquaredLength()
        if (sqLen > squaredSpeed) then
            local dir = velocity
            dir:Normalize()
            local newVelocity = dir:Dot(maxSpeed);
            newVelocity:SetZ(velocZTemp);
            agent:SetVelocity(newVelocity);
        end
    end
end

require("Data/engine/scripts/lua/agent/ActorController.lua")
require("Data/engine/scripts/lua/agent/ActorControllerSeek.lua")
require("Data/engine/scripts/lua/agent/ActorControllerPuresure.lua")
require("Data/engine/scripts/lua/agent/ActorControllerFollower.lua")
require("Data/engine/scripts/lua/agent/ActorControllerPath.lua")
require("Data/engine/scripts/lua/agent/BehaviorTreeNode.lua")
require("Data/engine/scripts/lua/agent/BehaviorTree.lua")
