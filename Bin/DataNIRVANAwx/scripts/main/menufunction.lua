-- menufunction.lua

-- file
function n_NewProject()
	PX2EU_MAN:NewProject()
end

function n_OpenProject()
	PX2EU_MAN:OpenProject()
end

function n_SaveProject()
	PX2EU_MAN:SaveProject()
end

function n_CloseProject()
	PX2EU_MAN:CloseProject()
end

function n_NewScene()
	PX2EU_MAN:NewScene()
end

function n_OpenScene()
	PX2EU_MAN:OpenScene()
end

function n_SaveScene()
	PX2EU_MAN:SaveScene()
end

function n_SaveSceneAs()
	PX2EU_MAN:SaveSceneAs()
end

function n_CloseScene()
	PX2EU_MAN:CloseScene()
end

function n_Exit()
	PX2EU_MAN:Exit()
end

function n_UnDo()
	PX2_EDIT:Undo()
end

function n_ReDo()
	PX2_EDIT:Redo()
end

function n_Copy()
	PX2_EDIT:CopyObject()
end

function n_Paste()
	PX2_EDIT:PasteCopyedObject()
end

function n_Clone()
	PX2_EDIT:CloneSelectedObject()
end

function n_Delete()
	PX2_EDIT:CloneSelectedObject()
end

function n_Select()
	PX2_EDIT:SetEditMode(Edit.EM_SELECT)
end

function n_Translate()
	PX2_EDIT:SetEditMode(Edit.EM_TRANSLATE)
end

function n_Rotate()
	PX2_EDIT:SetEditMode(Edit.EM_ROTATE)
end

function n_Scale()
	PX2_EDIT:SetEditMode(Edit.EM_SCALE)
end

function n_Simulate()
	if Application.PT_NONE==PX2_APP:GetPlayType() then
		PX2_APP:Play(Application.PT_SIMULATE)
	else
		PX2_APP:Play(Application.PT_NONE)
		local projPath = PX2_APP:GetProjectFilePath()
		if ""~=projPath then
			PX2_APP:LoadProjectByPath(projPath)
		end
	end
end

function n_Play()
	if Application.PT_NONE==PX2_APP:GetPlayType() then	
		PX2_APP:Play(Application.PT_PLAY)
	else
		PX2_APP:Play(Application.PT_NONE)
		local projPath = PX2_APP:GetProjectFilePath()
		PX2_APP:LoadProjectByPath(projPath)
	end
end

function n_PlayInWindow()
	PX2_EDIT:BroadCastEditorEventPlayInWindow()
end

function n_UserLogin()

end

function n_About()
	NirMan:MessageBox(PX2_LM_EDITOR:V("n_About"), "http://wwww.manykit.com")
end