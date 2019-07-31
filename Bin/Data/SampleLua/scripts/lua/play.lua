-- play.lua

require("Data/SampleLua/scripts/lua/language.lua")
require("Data/SampleLua/scripts/lua/sample/sample.lua")
require("Data/SampleLua/scripts/lua/sample/ActorCtrl.lua")

function engine_project_preplay()
	sampleLua_AddLanguage()	
end

function engine_project_play()
	sampleLua_Sample()
end

function engine_project_update(appSeconds, elapsedSeconds)
	
end