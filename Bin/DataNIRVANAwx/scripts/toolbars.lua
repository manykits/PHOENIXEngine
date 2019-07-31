-- toolbars.lua

-- file tool bar
function n_CreateToolBarMain()
	PX2EU_MAN:AddTool("Main", "DataNIRVANAwx/images/icons/main/proj_new.png", "n_NewProject")
	PX2EU_MAN:AddTool("Main", "DataNIRVANAwx/images/icons/main/proj_open.png", "n_OpenProject")
	PX2EU_MAN:AddTool("Main", "DataNIRVANAwx/images/icons/main/proj_save.png", "n_SaveProject")
	PX2EU_MAN:AddToolSeparater("Main")
	PX2EU_MAN:AddTool("Main", "DataNIRVANAwx/images/icons/main/scene_new.png", "n_NewScene")
	PX2EU_MAN:AddTool("Main", "DataNIRVANAwx/images/icons/main/scene_open.png", "n_OpenScene")
	PX2EU_MAN:AddTool("Main", "DataNIRVANAwx/images/icons/main/scene_save.png", "n_SaveScene")
	PX2EU_MAN:AddTool("Main", "DataNIRVANAwx/images/icons/main/scene_saveas.png", "n_SaveSceneAs")
	PX2EU_MAN:AddToolSeparater("Main")
	PX2EU_MAN:AddTool("Main", "DataNIRVANAwx/images/icons/main/play.png", "n_Play")
	PX2EU_MAN:AddTool("Main", "DataNIRVANAwx/images/icons/main/playinwindow.png", "n_PlayInWindow")
	PX2EU_MAN:AddToolStretch("Main")
	PX2EU_MAN:AddTool("Main", "DataNIRVANAwx/images/icons/main/user.png", "n_UserLogin")
end