-- start.lua

PX2_SC_LUA:CallFile("DataNIRVANAwx/scripts/globals.lua")
PX2_SC_LUA:CallFile("DataNIRVANAwx/scripts/language.lua")
PX2_SC_LUA:CallFile("DataNIRVANAwx/scripts/languagePre.lua")
PX2_SC_LUA:CallFile("DataNIRVANAwx/scripts/functions.lua")
PX2_SC_LUA:CallFile("DataNIRVANAwx/scripts/toolbars.lua")
PX2_SC_LUA:CallFile("DataNIRVANAwx/scripts/menus.lua")
PX2_SC_LUA:CallFile("DataNIRVANAwx/scripts/res/res.lua")
PX2_SC_LUA:CallFile("DataNIRVANAwx/scripts/text/text.lua")
PX2_SC_LUA:CallFile("DataNIRVANAwx/scripts/console/console.lua")
PX2_SC_LUA:CallFile("DataNIRVANAwx/scripts/main/menu.lua")
PX2_SC_LUA:CallFile("DataNIRVANAwx/scripts/main/menufunction.lua")
PX2_SC_LUA:CallFile("DataNIRVANAwx/scripts/blues/blues.lua")

function n_EditOnEvent(entType)
	n_project:onEvent(entType)
end

function naPreStart()
	n_AddLanguagePre()
	n_AddLanguage()
end

n_g_FrameBluesTableFrame = nil
function naStart()
	n_CreateMainMenu()
	n_CreateToolBarMain()
	n_CreateFrame_BBs()
end

function naPreEnd()
	if nil~=n_g_FrameLogicTableFrame then
		n_g_FrameLogicTableFrame:RemoveAllTabs()
	end
end

function naEnd()
end