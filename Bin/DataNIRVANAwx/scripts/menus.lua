-- menus.lua

function n_CreateResMenu()
	local editMenu = NirMan:GetCurMenu()
	if nil~=editMenu then
		NirMan:AddMenuItem(editMenu, ""..PX2_LM_EDITOR:GetValue("n_Open"), "n_ResView_Open", "")
		NirMan:AddMenuItem(editMenu, ""..PX2_LM_EDITOR:GetValue("n_CopyResPath"), "n_ResView_CopyResPath", "")
	end
end

function n_ResView_Open()
	local selectResData = PX2_EDIT:GetSelectedResource()
	local resPathname = selectResData.ResPathname
	PX2_EDIT:OpenResOut(resPathname)
end

function n_ResView_CopyResPath()
	PX2_EDIT:CopySelectResourcePath()
end