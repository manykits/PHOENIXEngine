-- language.lua

function n_AddLanguage()
	-- MainFrame
	PX2_LM_EDITOR:AddItem1("n_File", "文件")
	PX2_LM_EDITOR:AddItem1("n_NewProject", "新建工程")
	PX2_LM_EDITOR:AddItem1("n_NewScene", "新建场景")
	PX2_LM_EDITOR:AddItem1("n_Open", "打开")
	PX2_LM_EDITOR:AddItem1("n_Save", "保存")
	PX2_LM_EDITOR:AddItem1("n_SaveAll", "全部保存")
	PX2_LM_EDITOR:AddItem1("n_SaveAs", "另存为")
	PX2_LM_EDITOR:AddItem1("n_Close", "关闭")
	PX2_LM_EDITOR:AddItem1("n_Exit", "退出")
	PX2_LM_EDITOR:AddItem1("n_Edit", "编辑")
	PX2_LM_EDITOR:AddItem1("n_View", "视图")
	
	PX2_LM_EDITOR:AddItem1("n_Edit", "编辑")
	PX2_LM_EDITOR:AddItem1("n_UnDo", "撤销")
	PX2_LM_EDITOR:AddItem1("n_ReDo", "重复")
	PX2_LM_EDITOR:AddItem1("n_Copy", "复制")
	PX2_LM_EDITOR:AddItem1("n_Paste", "粘贴")
	PX2_LM_EDITOR:AddItem1("n_Clone", "克隆")
	PX2_LM_EDITOR:AddItem1("n_Delete", "删除")
	PX2_LM_EDITOR:AddItem1("n_DeleteAll", "删除所有")
	PX2_LM_EDITOR:AddItem1("n_Import", "导入")
	PX2_LM_EDITOR:AddItem1("n_Export", "导出")
	PX2_LM_EDITOR:AddItem1("n_Select", "选择")
	PX2_LM_EDITOR:AddItem1("n_Translate", "位移")
	PX2_LM_EDITOR:AddItem1("n_Rotate", "旋转")
	PX2_LM_EDITOR:AddItem1("n_Scale", "缩放")
	PX2_LM_EDITOR:AddItem1("n_PlayAndStop", "播放/停止")
	PX2_LM_EDITOR:AddItem1("n_ResetPlay", "重置播放")
	
	PX2_LM_EDITOR:AddItem1("n_Local", "本地")
	PX2_LM_EDITOR:AddItem1("n_World", "世界")
	PX2_LM_EDITOR:AddItem1("n_Parent", "父对象")
	
	PX2_LM_EDITOR:AddItem1("n_Window", "窗口")
	PX2_LM_EDITOR:AddItem1("n_Project", "工程")
	PX2_LM_EDITOR:AddItem1("n_Stage", "编辑")
	PX2_LM_EDITOR:AddItem1("n_Res", "资源")
	PX2_LM_EDITOR:AddItem1("n_Inspector", "属性")
	PX2_LM_EDITOR:AddItem1("n_Console", "控制台")
	PX2_LM_EDITOR:AddItem1("n_Text", "文本")
	PX2_LM_EDITOR:AddItem1("n_Connect", "连接")
	PX2_LM_EDITOR:AddItem1("n_PreView", "预览")
	PX2_LM_EDITOR:AddItem1("n_Logic", "逻辑")
	PX2_LM_EDITOR:AddItem1("n_Play0", "运行")
	PX2_LM_EDITOR:AddItem1("n_TimeLine", "时间线")
	
	PX2_LM_EDITOR:AddItem1("n_Welcome", "欢迎!")
	
	PX2_LM_EDITOR:AddItem1("n_Debug", "调试")
	
	PX2_LM_EDITOR:AddItem1("n_Tool", "工具")
	
	PX2_LM_EDITOR:AddItem1("n_Help", "帮助")	
	PX2_LM_EDITOR:AddItem1("n_About", "关于")
	
	-- General
	PX2_LM_EDITOR:AddItem1("n_Create", "创建")
	
	-- Connect
	PX2_LM_EDITOR:AddItem1("n_Name", "名称")
	PX2_LM_EDITOR:AddItem1("n_IP", "IP")
	PX2_LM_EDITOR:AddItem1("n_State", "状态")
	PX2_LM_EDITOR:AddItem1("n_Property", "属性")
	
	-- BluePrint
	PX2_LM_EDITOR:AddItem1("n_BluePrint", "蓝图")
	PX2_LM_EDITOR:AddItem1("n_Compile", "编译")
	PX2_LM_EDITOR:AddItem1("n_Disconnect", "断开")
	
	PX2_LM_EDITOR:AddItem1("n_Play0", "运行")
	
	-- Scene
	PX2_LM_EDITOR:AddItem1("n_Scene", "场景")
	
	-- UI
	PX2_LM_EDITOR:AddItem1("n_UI", "界面")
	PX2_LM_EDITOR:AddItem1("n_UIFrame", "层")
	PX2_LM_EDITOR:AddItem1("n_UIFPicBox", "层图片")
	PX2_LM_EDITOR:AddItem1("n_UIFText", "层文本")
	PX2_LM_EDITOR:AddItem1("n_UIButton", "按钮")
	PX2_LM_EDITOR:AddItem1("n_UICheckButton", "选择按钮")
	PX2_LM_EDITOR:AddItem1("n_UIComboBox", "组合框")
	PX2_LM_EDITOR:AddItem1("n_UIEditBox", "编辑框")
	PX2_LM_EDITOR:AddItem1("n_UIPicBox", "图片")
	PX2_LM_EDITOR:AddItem1("n_UIText", "文本")	
	
	PX2_LM_EDITOR:AddItem1("n_CreateScriptControllerFromRes", "从资源创建脚本控制器")
	
	PX2_LM_EDITOR:AddItem1("n_CreateFromRes", "从资源创建")
	
	-- Event
	PX2_LM_EDITOR:AddItem1("n_Event", "事件")
	PX2_LM_EDITOR:AddItem1("n_SimuStart", "仿真开始")
	
	-- Edit
	PX2_LM_EDITOR:AddItem1("n_Geometry", "几何体")
	PX2_LM_EDITOR:AddItem1("n_Plane", "平面")
	PX2_LM_EDITOR:AddItem1("n_Box", "正方体")
	PX2_LM_EDITOR:AddItem1("n_Sphere", "球")
	PX2_LM_EDITOR:AddItem1("n_Camera", "像机")
	PX2_LM_EDITOR:AddItem1("n_Light", "灯光")
	PX2_LM_EDITOR:AddItem1("n_Trigger", "触发器")
	PX2_LM_EDITOR:AddItem1("n_Terrain", "地形")
	PX2_LM_EDITOR:AddItem1("n_Sky", "天空")
	PX2_LM_EDITOR:AddItem1("n_ImportSelectedRes", "导入选择资源")
	PX2_LM_EDITOR:AddItem1("n_Import", "导入")
	PX2_LM_EDITOR:AddItem1("n_Export", "导出")
	PX2_LM_EDITOR:AddItem1("n_SceneActor", "场景角色")
	PX2_LM_EDITOR:AddItem1("n_Effect", "特效")
	PX2_LM_EDITOR:AddItem1("n_Billboard", "公告板")
	PX2_LM_EDITOR:AddItem1("n_Particle", "粒子")
	PX2_LM_EDITOR:AddItem1("n_Beam", "条带")
	PX2_LM_EDITOR:AddItem1("n_Ribbon", "波带")
	PX2_LM_EDITOR:AddItem1("n_Sound", "声音")
	PX2_LM_EDITOR:AddItem1("n_Actor", "角色")
	PX2_LM_EDITOR:AddItem1("n_InfinitePlane", "无限平面")
	
	PX2_LM_EDITOR:AddItem1("n_Node", "节点")
	PX2_LM_EDITOR:AddItem1("n_SkyBox", "天空盒")
	PX2_LM_EDITOR:AddItem1("n_Controller", "控制器")
	PX2_LM_EDITOR:AddItem1("n_ColorController", "颜色控制器")
	PX2_LM_EDITOR:AddItem1("n_BrightnessController", "亮度控制器")
	PX2_LM_EDITOR:AddItem1("n_AlphaController", "透明控制器")
	PX2_LM_EDITOR:AddItem1("n_UniformScaleController", "统一缩放控制器")
	PX2_LM_EDITOR:AddItem1("n_ScaleController", "缩放控制器")
	PX2_LM_EDITOR:AddItem1("n_RotateController", "旋转控制器")
	PX2_LM_EDITOR:AddItem1("n_TranslateController", "位移控制器")		
	PX2_LM_EDITOR:AddItem1("n_ScriptController", "脚本控制器")
	PX2_LM_EDITOR:AddItem1("n_EffectMoudle", "特效模组")
	
	PX2_LM_EDITOR:AddItem1("n_Copy", "复制")
	PX2_LM_EDITOR:AddItem1("n_Paste", "粘贴")
	PX2_LM_EDITOR:AddItem1("n_Delete", "删除")
	PX2_LM_EDITOR:AddItem1("n_DeleteAll", "删除全部")
	
	PX2_LM_EDITOR:AddItem1("n_ResetPlay", "重置播放")
	PX2_LM_EDITOR:AddItem1("n_Play", "播放")
	PX2_LM_EDITOR:AddItem1("n_Pause", "暂停")
	PX2_LM_EDITOR:AddItem1("n_Reset", "重置")
	
	PX2_LM_EDITOR:AddItem1("n_FindInProjectTree", "在工程树中查找")
	PX2_LM_EDITOR:AddItem1("n_FindInResTree", "在资源树中查找")
	
	PX2_LM_EDITOR:AddItem1("n_MakeCurve", "曲线化")
	
	-- timeline
	PX2_LM_EDITOR:AddItem1("n_SetInValue", "设置InVal")
	PX2_LM_EDITOR:AddItem1("n_NewInValue", "新InVal")
	PX2_LM_EDITOR:AddItem1("n_SetOutValue", "设置OutVal")
	PX2_LM_EDITOR:AddItem1("n_NewOutValue", "新OutVal")
	PX2_LM_EDITOR:AddItem1("n_AddPoint", "增加点")
	PX2_LM_EDITOR:AddItem1("n_DeletePoint", "删除")	
	
	-- tips
	PX2_LM_EDITOR:AddItem("n_Notice", "注意！", "Notice!")
	PX2_LM_EDITOR:AddItem("n_Tip0", "退出？", "Do you want to exist?")
	PX2_LM_EDITOR:AddItem("n_TipCreateProject", "工程已经存在，无法创建！", "Project is already exist, can't be created!")
	PX2_LM_EDITOR:AddItem("n_TipNotHasParent", "必须存在父节点！", "There must has a parent node!")
	PX2_LM_EDITOR:AddItem("n_TipNotSelectAObject", "必须选择一个对象！", "You must select a object!")
	PX2_LM_EDITOR:AddItem("n_OpenProject", "打开项目", "Open Project")
	PX2_LM_EDITOR:AddItem("n_OpenScene", "打开场景", "Open Scene")
	PX2_LM_EDITOR:AddItem("n_SaveScene", "保存场景", "Save Scene")
	PX2_LM_EDITOR:AddItem("n_SaveSceneAs", "场景另存为", "Save Scene as")
	PX2_LM_EDITOR:AddItem("n_Open", "打开", "Open")
	PX2_LM_EDITOR:AddItem("n_CopyResPath", "拷贝路径", "CopyResPath")
	PX2_LM_EDITOR:AddItem("n_About", "关于", "About..")
end