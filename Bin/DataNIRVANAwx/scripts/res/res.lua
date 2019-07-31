-- res.lua

function n_ResButCallback(objPtr, callType)
	local obj = Cast:ToO(objPtr)
	local name = obj:GetName()

	local selectResData = PX2_EDIT:GetSelectedResource()
	local resPathname = selectResData.ResPathname

	if UICT_RELEASED == callType then
		if "ButOpenIn" == name then
			PX2_EDIT:OpenResIn(resPathname)
		elseif "ButOpenOut" == name then
			PX2_EDIT:OpenResOut(resPathname)
		elseif "ButCopyPath" == name then
			n_ResView_CopyResPath()
		elseif "ButRefresh" == name then
			PX2_EDIT:BroadCastEditorEventRefreshRes()
		elseif "ButUp" == name then
			PX2_EDIT:BroadCastEditorEventUp()
		elseif "ButDown" == name then
			PX2_EDIT:BroadCastEditorEventDown()
		end
	end
end

function n_ResView_Refresh()
	PX2_EDIT:BroadCastEditorEventRefreshRes()
end

function n_ResView_Clear()
	PX2_EDIT:BraodCastEditorEventClearRes()
end
