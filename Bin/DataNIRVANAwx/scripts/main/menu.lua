-- menu.lua

-- create main menu
function n_CreateMainMenu()
	-- File
	PX2_APP:Menu_Main_AddMainItem("File", (PX2_LM_EDITOR:V("n_File")))
	PX2_APP:Menu_Main_AddItem("File", "NewProject", PX2_LM_EDITOR:V("n_NewProject").."\tCtrl-N", "n_NewProject", (""), "Proj_NewProject")
	PX2_APP:Menu_Main_AddItem("File", "Open", PX2_LM_EDITOR:V("n_Open").."\tCtrl-O", "n_OpenProject", "", "Proj_Open")
	PX2_APP:Menu_Main_AddItem("File", "Save", PX2_LM_EDITOR:V("n_Save").."\tCtrl-S", "n_SaveProject", "", "Proj_Save")
	PX2_APP:Menu_Main_AddItem("File", "Close", PX2_LM_EDITOR:V("n_Close"), "n_CloseProject", "", "Proj_Close")
	PX2_APP:Menu_Main_AddItemSeparater("File")
	PX2_APP:Menu_Main_AddItem("File", "NewScene", PX2_LM_EDITOR:V("n_NewScene").."\tCtrl-N", "n_NewScene", "", "Proj_Scene_NewScene")
	PX2_APP:Menu_Main_AddItem("File", "Open", PX2_LM_EDITOR:V("n_Open"), "n_OpenScene", "", "Proj_Scene_Open")
	PX2_APP:Menu_Main_AddItem("File", "Save", PX2_LM_EDITOR:V("n_Save"), "n_SaveScene", "", "Proj_Scene_Save")
	PX2_APP:Menu_Main_AddItem("File", "SaveAs", PX2_LM_EDITOR:V("n_SaveAs"), "n_SaveSceneAs", "", "Proj_Scene_SaveAs")
	PX2_APP:Menu_Main_AddItem("File", "Close", PX2_LM_EDITOR:V("n_Close"), "n_CloseScene", "", "Proj_Scene_Close")
	PX2_APP:Menu_Main_AddItemSeparater("File")
	PX2_APP:Menu_Main_AddItem("File", "Exit", PX2_LM_EDITOR:V("n_Exit"), "n_Exit")
	
	-- Edit
	PX2_APP:Menu_Main_AddMainItem("Edit", (PX2_LM_EDITOR:V("n_Edit")))
	PX2_APP:Menu_Main_AddItem("Edit", "UnDo", PX2_LM_EDITOR:V("n_UnDo").."\tCtrl-Z", "n_UnDo", "", "Edit")
	PX2_APP:Menu_Main_AddItem("Edit", "ReDo", PX2_LM_EDITOR:V("n_ReDo").."\tCtrl-R", "n_ReDo", "", "Edit")
	PX2_APP:Menu_Main_AddItemSeparater("Edit")
	PX2_APP:Menu_Main_AddItem("Edit", "Copy", PX2_LM_EDITOR:V("n_Copy").."\tCtrl-C", "n_Copy", "", "Edit")
	PX2_APP:Menu_Main_AddItem("Edit", "Paste", PX2_LM_EDITOR:V("n_Paste").."\tCtrl-V", "n_Paste", "", "Edit")
	PX2_APP:Menu_Main_AddItem("Edit", "Clone", PX2_LM_EDITOR:V("n_Clone").."\tCtrl-D", "n_Clone", "", "Edit")
	PX2_APP:Menu_Main_AddItem("Edit", "Delete", PX2_LM_EDITOR:V("n_Delete").."\tDelete", "n_DeleteSelection", "", "Edit")
	PX2_APP:Menu_Main_AddItemSeparater("Edit")
	PX2_APP:Menu_Main_AddItem("Edit", "Import", PX2_LM_EDITOR:V("n_Import").."\tCtrl-I", "n_Import", "", "Edit")
	PX2_APP:Menu_Main_AddItem("Edit", "Export", PX2_LM_EDITOR:V("n_Export").."\tCtrl-E", "n_Export", "", "Edit")
	PX2_APP:Menu_Main_AddItemSeparater("Edit")
	PX2_APP:Menu_Main_AddItem("Edit", "Select", PX2_LM_EDITOR:V("n_Select"), "n_Select".."\tCtrl-Q", "", "Edit")
	PX2_APP:Menu_Main_AddItem("Edit", "Translate", PX2_LM_EDITOR:V("n_Translate").."\tCtrl-W", "n_Translate", "", "Edit")
	PX2_APP:Menu_Main_AddItem("Edit", "Rotate", PX2_LM_EDITOR:V("n_Rotate").."\tCtrl-E", "n_Rotate", "", "Edit")
	PX2_APP:Menu_Main_AddItem("Edit", "Scale", PX2_LM_EDITOR:V("n_Scale").."\tCtrl-R", "n_Scale", "", "Edit")
	PX2_APP:Menu_Main_AddItemSeparater("Edit")
	PX2_APP:Menu_Main_AddItem("Edit", "PlayAndStop", PX2_LM_EDITOR:V("n_PlayAndStop"), "", "", "Edit")
	PX2_APP:Menu_Main_AddItem("Edit", "ResetPlay", PX2_LM_EDITOR:V("n_ResetPlay"), "", "", "Edit")
	
	-- Views
	--PX2_APP:Menu_Main_AddMainItem("View", (PX2_LM_EDITOR:V("n_View")))
	--PX2_APP:Menu_Main_AddItem("View", "Project", "Project", "", "")
	--PX2_APP:Menu_Main_AddItem("View", "Property", "Property", "", "")
	--PX2_APP:Menu_Main_AddItem("View", "Resource", "Resource", "", "")
	--PX2_APP:Menu_Main_AddItem("View", "TimeLine", "TimeLine", "", "")
	--PX2_APP:Menu_Main_AddItem("View", "Console", "Console", "", "")
	--PX2_APP:Menu_Main_AddItem("View", "Text", "Text", "", "")
	
	-- about
	PX2_APP:Menu_Main_AddMainItem("About", (PX2_LM_EDITOR:V("n_About")))
	PX2_APP:Menu_Main_AddItem("About", "About", (PX2_LM_EDITOR:V("n_About")), "n_About")	
end