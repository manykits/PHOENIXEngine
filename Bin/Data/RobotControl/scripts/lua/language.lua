-- language.lua

function rc_AddLanguage()
	PX2_LM_APP:Clear()
	PX2_LM_APP:AddItem1("General", "通用")
	PX2_LM_APP:AddItem1("UI", "界面")
	PX2_LM_APP:AddItem1("About", "关于")
	
	PX2_LM_APP:AddItem1("Cancel", "取消")
	PX2_LM_APP:AddItem1("OK", "确定")

	PX2_LM_APP:AddItem1("Setting", "设置")

	PX2_LM_APP:AddItem1("Name", "名字")

	PX2_LM_APP:AddItem1("Connect", "连接")
	PX2_LM_APP:AddItem1("Bluetooth", "蓝牙")
	PX2_LM_APP:AddItem1("BleSerConnect", "蓝牙串口连接")
	PX2_LM_APP:AddItem1("DisConnect", "断开")
	PX2_LM_APP:AddItem1("Scan", "扫描")
	PX2_LM_APP:AddItem1("SignDegree", "信号强度")
	PX2_LM_APP:AddItem1("Lidar", "雷达")
	PX2_LM_APP:AddItem1("face", "面")
	PX2_LM_APP:AddItem1("lidar", "雷达")
	PX2_LM_APP:AddItem1("lidarcur", "当前雷达")
	PX2_LM_APP:AddItem1("uictrl", "控制器")
	PX2_LM_APP:AddItem1("Axis", "陀螺仪")
	PX2_LM_APP:AddItem1("Device", "设备")

	PX2_LM_APP:AddItem1("UIMode", "UI模式")

	PX2_LM_APP:AddItem1("SaveMap", "保存")
	PX2_LM_APP:AddItem1("LoadMap", "加载")

	PX2_LM_APP:AddItem1("Control", "控制")
	PX2_LM_APP:AddItem1("Axis", "陀螺仪")
	PX2_LM_APP:AddItem1("Voice", "语音")

	PX2_LM_APP:AddItem1("Left", "左")
	PX2_LM_APP:AddItem1("Right", "右")
	PX2_LM_APP:AddItem1("LED_L", "左灯")
	PX2_LM_APP:AddItem1("LED_R", "右灯")
	PX2_LM_APP:AddItem1("Sound", "声音")
	PX2_LM_APP:AddItem1("RevertLeft", "左反")
	PX2_LM_APP:AddItem1("RevertRight", "右反")

	PX2_LM_APP:AddItem1("Hor", "水平")
	PX2_LM_APP:AddItem1("Ver", "垂直")
end