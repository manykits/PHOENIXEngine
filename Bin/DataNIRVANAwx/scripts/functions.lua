-- functions.lua

-- create objs
function n_Create_Actor_Actor()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetScene()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateActor()

		if EU_Manager.EMT_SCENE==PX2EU_MAN:GetEidtMenuType() then		
			mov.LocalTransform:SetTranslate(pickPos)
		end
		PX2_CREATER:AddObject(node, mov)
	else
		PX2EU_MAN:PlayTip(PX2_LM_EDITOR:GetValue("n_Notice"), PX2_LM_EDITOR:GetValue("n_TipNotHasParent"))
	end
end

function n_Create_Actor_InfinitePlane()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetScene()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateActor_InfinitePlane()

		if EU_Manager.EMT_SCENE==PX2EU_MAN:GetEidtMenuType() then		
			mov.LocalTransform:SetTranslate(pickPos)
		end
		PX2_CREATER:AddObject(node, mov)
	else
		PX2EU_MAN:PlayTip(PX2_LM_EDITOR:GetValue("n_Notice"), PX2_LM_EDITOR:GetValue("n_TipNotHasParent"))
	end
end

function n_Create_Actor_Box()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetScene()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateActorBox()

		if EU_Manager.EMT_SCENE==PX2EU_MAN:GetEidtMenuType() then		
			mov.LocalTransform:SetTranslate(pickPos)
		end
		PX2_CREATER:AddObject(node, mov)
	else
		PX2EU_MAN:PlayTip(PX2_LM_EDITOR:GetValue("n_Notice"), PX2_LM_EDITOR:GetValue("n_TipNotHasParent"))
	end
end

function n_Create_Geometry_Plane()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetScene()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateMovable_Rectangle()
		if EU_Manager.EMT_SCENE==PX2EU_MAN:GetEidtMenuType() then		
			mov.LocalTransform:SetTranslate(pickPos)
		end
		PX2_CREATER:AddObject(node, mov)
	else
		PX2EU_MAN:PlayTip(PX2_LM_EDITOR:GetValue("n_Notice"), PX2_LM_EDITOR:GetValue("n_TipNotHasParent"))
	end
end

function n_Create_Geometry_Box()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetScene()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateMovable_Box()
		if EU_Manager.EMT_SCENE==PX2EU_MAN:GetEidtMenuType() then	
			mov.LocalTransform:SetTranslate(pickPos)
		end
		PX2_CREATER:AddObject(node, mov)
	else
		PX2EU_MAN:PlayTip(PX2_LM_EDITOR:GetValue("n_Notice"), PX2_LM_EDITOR:GetValue("n_TipNotHasParent"))
	end
end

function n_Create_Geometry_Sphere()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetScene()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateMovable_Sphere()
		if EU_Manager.EMT_SCENE==PX2EU_MAN:GetEidtMenuType() then	
			mov.LocalTransform:SetTranslate(pickPos)
		end
		PX2_CREATER:AddObject(node, mov)
	else
		PX2EU_MAN:PlayTip(PX2_LM_EDITOR:GetValue("n_Notice"), PX2_LM_EDITOR:GetValue("n_TipNotHasParent"))
	end
end

function n_Create_Node()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetScene()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateNode()
		if EU_Manager.EMT_SCENE==PX2EU_MAN:GetEidtMenuType() then	
			mov.LocalTransform:SetTranslate(pickPos)
		end
		PX2_CREATER:AddObject(node, mov)
	else
		PX2EU_MAN:PlayTip(PX2_LM_EDITOR:GetValue("n_Notice"), PX2_LM_EDITOR:GetValue("n_TipNotHasParent"))
	end
end

function n_Create_Camera()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetScene()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateNode_Camera()
		if EU_Manager.EMT_SCENE==PX2EU_MAN:GetEidtMenuType() then	
			mov.LocalTransform:SetTranslate(pickPos)
		end
		PX2_CREATER:AddObject(node, mov)
	else
		PX2EU_MAN:PlayTip(PX2_LM_EDITOR:GetValue("n_Notice"), PX2_LM_EDITOR:GetValue("n_TipNotHasParent"))
	end
end

function n_Create_SkyBox()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetScene()
	end

	local selectedResDir = PX2_EDIT:GetSelectedResDir()
	
	if nil~=node and ""~=selectedResDir then
		local mov = PX2_CREATER:CreateSkyBox(selectedResDir)
		PX2_CREATER:AddObject(node, mov)
	else
		PX2EU_MAN:PlayTip(PX2_LM_EDITOR:GetValue("n_Notice"), PX2_LM_EDITOR:GetValue("n_TipNotHasParent"))
	end
end

function n_Create_Terrain()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetScene()
	end
	
	if nil~=node then
		local mov = PX2_CREATER:CreateTerrain()
		PX2_CREATER:AddObject(node, mov)
	else
		PX2EU_MAN:PlayTip(PX2_LM_EDITOR:GetValue("n_Notice"), PX2_LM_EDITOR:GetValue("n_TipNotHasParent"))
	end
end

-- create effect
function n_Create_Particle()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetScene()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateParticleEmitter()
		if EU_Manager.EMT_SCENE==PX2EU_MAN:GetEidtMenuType() then	
			mov.LocalTransform:SetTranslate(pickPos)
		end
		PX2_CREATER:AddObject(node, mov)
		mov:ResetPlay()
	else
		PX2EU_MAN:PlayTip(PX2_LM_EDITOR:GetValue("n_Notice"), PX2_LM_EDITOR:GetValue("n_TipNotHasParent"))
	end
end

function n_Create_Billboard()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetScene()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateBillboard()
		if EU_Manager.EMT_SCENE==PX2EU_MAN:GetEidtMenuType() then	
			mov.LocalTransform:SetTranslate(pickPos)
		end
		PX2_CREATER:AddObject(node, mov)
		mov:ResetPlay()
	else
		PX2EU_MAN:PlayTip(PX2_LM_EDITOR:GetValue("n_Notice"), PX2_LM_EDITOR:GetValue("n_TipNotHasParent"))
	end
end

function n_Create_Beam()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetScene()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateBeamEmitter()
		mov.LocalTransform:SetTranslate(pickPos)
		PX2_CREATER:AddObject(node, mov)
		mov:ResetPlay()
	else
		PX2EU_MAN:PlayTip(PX2_LM_EDITOR:GetValue("n_Notice"), PX2_LM_EDITOR:GetValue("n_TipNotHasParent"))
	end
end

function n_Create_Ribbon()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetScene()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateRibbonEmitter()
		mov.LocalTransform:SetTranslate(pickPos)
		PX2_CREATER:AddObject(node, mov)
		mov:ResetPlay()
	else
		PX2EU_MAN:PlayTip(PX2_LM_EDITOR:GetValue("n_Notice"), PX2_LM_EDITOR:GetValue("n_TipNotHasParent"))
	end
end

function n_Create_Sound()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetScene()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateSoundable()
		mov.LocalTransform:SetTranslate(pickPos)
		PX2_CREATER:AddObject(node, mov)
		mov:ResetPlay()
	else
		PX2EU_MAN:PlayTip(PX2_LM_EDITOR:GetValue("n_Notice"), PX2_LM_EDITOR:GetValue("n_TipNotHasParent"))
	end
end

-- create controller
function n_Create_Controller_ColorController()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local ctrlable = Cast:ToControlledable(selectObj)
	if nil~=ctrlable then
		local ctrl = PX2_CREATER:CreateICC_Color()
		PX2_CREATER:AddObject(ctrlable, ctrl)
		ctrl:ResetPlay()
	else
		NirMan:MessageBox(PX2_LM_EDITOR:GetValue("n_Notice"), PX2_LM_EDITOR:GetValue("n_TipNotHasParent"))
	end
end

function n_Create_Controller_BrightnessController()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local ctrlable = Cast:ToControlledable(selectObj)
	if nil~=ctrlable then
		local ctrl = PX2_CREATER:CreateICC_Brightness()
		PX2_CREATER:AddObject(ctrlable, ctrl)
		ctrl:ResetPlay()
	else
		NirMan:MessageBox(PX2_LM_EDITOR:GetValue("n_Notice"), PX2_LM_EDITOR:GetValue("n_TipNotHasParent"))
	end
end

function n_Create_Controller_AlphaController()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local ctrlable = Cast:ToControlledable(selectObj)
	if nil~=ctrlable then
		local ctrl = PX2_CREATER:CreateICC_Alpha()
		PX2_CREATER:AddObject(ctrlable, ctrl)
		ctrl:ResetPlay()
	else
		NirMan:MessageBox(PX2_LM_EDITOR:GetValue("n_Notice"), PX2_LM_EDITOR:GetValue("n_TipNotHasParent"))
	end
end

function n_Create_Controller_UniformScaleController()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local ctrlable = Cast:ToControlledable(selectObj)
	if nil~=ctrlable then
		local ctrl = PX2_CREATER:CreateICC_UniformScale()
		PX2_CREATER:AddObject(ctrlable, ctrl)
		ctrl:ResetPlay()
	else
		NirMan:MessageBox(PX2_LM_EDITOR:GetValue("n_Notice"), PX2_LM_EDITOR:GetValue("n_TipNotHasParent"))
	end
end

function n_Create_Controller_ScaleController()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local ctrlable = Cast:ToControlledable(selectObj)
	if nil~=ctrlable then
		local ctrl = PX2_CREATER:CreateICC_Scale()
		PX2_CREATER:AddObject(ctrlable, ctrl)
		ctrl:ResetPlay()
	else
		NirMan:MessageBox(PX2_LM_EDITOR:GetValue("n_Notice"), PX2_LM_EDITOR:GetValue("n_TipNotHasParent"))
	end
end

function n_Create_Controller_RotateController()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local ctrlable = Cast:ToControlledable(selectObj)
	if nil~=ctrlable then
		local ctrl = PX2_CREATER:CreateICC_Rotate()
		PX2_CREATER:AddObject(ctrlable, ctrl)
		ctrl:ResetPlay()
	else
		NirMan:MessageBox(PX2_LM_EDITOR:GetValue("n_Notice"), PX2_LM_EDITOR:GetValue("n_TipNotHasParent"))
	end
end

function n_Create_Controller_TranslateController()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local ctrlable = Cast:ToControlledable(selectObj)
	if nil~=ctrlable then
		local ctrl = PX2_CREATER:CreateICC_Translate()
		PX2_CREATER:AddObject(ctrlable, ctrl)
		ctrl:ResetPlay()
	else
		NirMan:MessageBox(PX2_LM_EDITOR:GetValue("n_Notice"), PX2_LM_EDITOR:GetValue("n_TipNotHasParent"))
	end
end

-- create ui
function n_Create_UIFrame()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetUI()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateUIFrame()
		if EU_Manager.EMT_SCENE==PX2EU_MAN:GetEidtMenuType() then	
			mov:SetAnchorParamHor(pickPos:X(), pickPos:X())
			mov:SetAnchorParamVer(pickPos:Z(), pickPos:Z())
		end
		PX2_CREATER:AddObject(node, mov)
		mov:ResetPlay()
	else
		PX2EU_MAN:PlayTip(PX2_LM_EDITOR:GetValue("n_Notice"), PX2_LM_EDITOR:GetValue("n_TipNotHasParent"))
	end
end

function n_Create_UIFPicBox()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetUI()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateUIFPicBox()
		if EU_Manager.EMT_SCENE==PX2EU_MAN:GetEidtMenuType() then
			mov:SetAnchorParamHor(pickPos:X(), pickPos:X())
			mov:SetAnchorParamVer(pickPos:Z(), pickPos:Z())
		end
		PX2_CREATER:AddObject(node, mov)
		mov:ResetPlay()
	else
		PX2EU_MAN:PlayTip(PX2_LM_EDITOR:GetValue("n_Notice"), PX2_LM_EDITOR:GetValue("n_TipNotHasParent"))
	end
end

function n_Create_UIFText()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetUI()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateUIFText()
		if EU_Manager.EMT_SCENE==PX2EU_MAN:GetEidtMenuType() then
			mov:SetAnchorParamHor(pickPos:X(), pickPos:X())
			mov:SetAnchorParamVer(pickPos:Z(), pickPos:Z())
		end
		PX2_CREATER:AddObject(node, mov)
		mov:ResetPlay()
	else
		PX2EU_MAN:PlayTip(PX2_LM_EDITOR:GetValue("n_Notice"), PX2_LM_EDITOR:GetValue("n_TipNotHasParent"))
	end

end

function n_Create_UIButton()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetUI()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateUIButton()
		if EU_Manager.EMT_SCENE==PX2EU_MAN:GetEidtMenuType() then
			mov:SetAnchorParamHor(pickPos:X(), pickPos:X())
			mov:SetAnchorParamVer(pickPos:Z(), pickPos:Z())
		end
		PX2_CREATER:AddObject(node, mov)
		mov:ResetPlay()
	else
		PX2EU_MAN:PlayTip(PX2_LM_EDITOR:GetValue("n_Notice"), PX2_LM_EDITOR:GetValue("n_TipNotHasParent"))
	end
end

function n_Create_UICheckButton()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetUI()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateUICheckButton()
		if EU_Manager.EMT_SCENE==PX2EU_MAN:GetEidtMenuType() then
			mov:SetAnchorParamHor(pickPos:X(), pickPos:X())
			mov:SetAnchorParamVer(pickPos:Z(), pickPos:Z())
		end
		PX2_CREATER:AddObject(node, mov)
		mov:ResetPlay()
	else
		PX2EU_MAN:PlayTip(PX2_LM_EDITOR:GetValue("n_Notice"), PX2_LM_EDITOR:GetValue("n_TipNotHasParent"))
	end
end

function n_Create_UIComboBox()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetUI()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateUIComboBox()
		if EU_Manager.EMT_SCENE==PX2EU_MAN:GetEidtMenuType() then
			mov:SetAnchorParamHor(pickPos:X(), pickPos:X())
			mov:SetAnchorParamVer(pickPos:Z(), pickPos:Z())
		end
		PX2_CREATER:AddObject(node, mov)
		mov:ResetPlay()
	else
		PX2EU_MAN:PlayTip(PX2_LM_EDITOR:GetValue("n_Notice"), PX2_LM_EDITOR:GetValue("n_TipNotHasParent"))
	end
end

function n_Create_UIEditBox()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)
	
	if nil==node then
		node = PX2_PROJ:GetUI()
	end
	
	if nil~=node then
		local pickPos = PX2_EDIT:GetPickLocalPos(node)
		local mov = PX2_CREATER:CreateUIEditBox()
		if EU_Manager.EMT_SCENE==PX2EU_MAN:GetEidtMenuType() then
			mov:SetAnchorParamHor(pickPos:X(), pickPos:X())
			mov:SetAnchorParamVer(pickPos:Z(), pickPos:Z())
		end
		PX2_CREATER:AddObject(node, mov)
		mov:ResetPlay()
	else
		PX2EU_MAN:PlayTip(PX2_LM_EDITOR:GetValue("n_Notice"), PX2_LM_EDITOR:GetValue("n_TipNotHasParent"))
	end
end

function n_CreateEffectableControllerModule(moduleName)
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local effectController = Cast:ToEffectController(selectObj)
	if nil~= effectController then
		local module = PX2_CREATER:CreateEffectModule(moduleName)
		PX2_CREATER:AddObject(effectController, module)
	else
		PX2EU_MAN:PlayTip(PX2_LM_EDITOR:GetValue("n_Notice"), PX2_LM_EDITOR:GetValue("n_TipNotHasParent"))
	end
end

function n_CreateScriptControllerFromRes()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local controlledable = Cast:ToControlledable(selectObj)	
		
	local selResData = PX2_EDIT:GetSelectedResource()
	
	if nil~=controlledable then
	
		local controller = PX2_CREATER:CreateScriptController_FromRes(selResData.ResPathname)
		if nil~= controller then
			PX2_CREATER:AddObject(controlledable, controller)
		end
	else
		PX2EU_MAN:PlayTip(PX2_LM_EDITOR:GetValue("n_Notice"), PX2_LM_EDITOR:GetValue("n_TipNotHasParent"))
	end
end

function n_ImportSelectRes()
	local selectObj = PX2_SELECTM_E:GetFirstObject()
	local node = Cast:ToNode(selectObj)	
		
	local selResData = PX2_EDIT:GetSelectedResource()
	
	if nil==node then
		node = PX2_PROJ:GetUI()
	end
	
	if nil~=node then		
		local mov = PX2_CREATER:CreateMovable_FromRes(selResData.ResPathname)
		if nil~= mov then
			PX2_CREATER:AddObject(node, mov)
			mov:ResetPlay()		
		end
	else
		PX2EU_MAN:PlayTip(PX2_LM_EDITOR:GetValue("n_Notice"), PX2_LM_EDITOR:GetValue("n_TipNotHasParent"))
	end
end

function n_Import()
	PX2EU_MAN:Import()
end

function n_Export()
	PX2EU_MAN:Export()
end

-- other functions
function n_Copy()
	PX2_EDIT:SetCopyObject(PX2_SELECTM_E:GetFirstObject())
end

function n_Paste()
	PX2_EDIT:PasteCopyedObject()
end

function n_DeleteSelection()
	PX2_EDIT:DeleteSelection()
end

-- anim play
function n_AnimResetPlay()
	PX2_EDIT:AnimResetPlay()
end

function n_AnimPlayStop()
	PX2_EDIT:AnimPlayStop()
end

function n_AnimPlay()
	PX2_EDIT:AnimPlay()
end

function n_AnimStop()
	PX2_EDIT:AnimStop()
end

function n_AnimReset()
	PX2_EDIT:AnimReset()
end

function n_MakeCurve()
	PX2_EDIT:MakeSelectTimeLineEdit();
end

-- time line
function n_TimeLine_Delete()
	PX2_EDIT:TimeLine_UIGroup_Delete();
end

function n_TimeLine_DeleteAll()
	PX2_EDIT:TimeLine_UIGroup_DeleteAll();
end

function n_TimeLine_SetInValue()
	PX2_EDIT:TimeLine_SetInValue()
end

function n_TimeLine_SetOutValue()
	PX2_EDIT:TimeLine_SetOutValue()
end

function n_TimeLine_AddPoint()
	PX2_EDIT:TimeLine_AddPoint()
end

function n_TimeLine_DeletePoint()
	PX2_EDIT:TimeLine_DeletePoint()
end

-- project
function n_FindInProjectTree()
	PX2_EDIT:FindSelectInProjTree()
end

function n_FindInResTree()
	PX2_EDIT:FindSelectInResTree()
end

function n_Project_Children()
	PX2EU_MAN:SetProjectTreeLevel(0)
end

function n_Project_Control()
	PX2EU_MAN:SetProjectTreeLevel(1)
end

function n_Project_Material()
	PX2EU_MAN:SetProjectTreeLevel(2)
end

function n_Project_Detail()
	PX2EU_MAN:SetProjectTreeLevel(3)
end

function n_Project_Synchronize()
end

-- inspector
function n_InspView_Property()
	PX2EU_MAN:SetInspectorView(1)
end

function n_InspView_Make()
	PX2EU_MAN:SetInspectorView(2)
end

-- text
function n_Text_Save()
end