--stop.lua

function engine_project_prestop() 
end

function engine_project_stop() 
    PX2_APP:ShutdownGeneralClientConnector("aidoor")
end

