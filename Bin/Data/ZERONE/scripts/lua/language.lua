-- language.lua

function zo_AddLanguage()
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
	PX2_LM_APP:AddItem1("face", "表情")
	PX2_LM_APP:AddItem1("lidar", "雷达")
	PX2_LM_APP:AddItem1("lidarcur", "当前雷达")
	PX2_LM_APP:AddItem1("uictrl", "控制器")
	PX2_LM_APP:AddItem1("Axis", "陀螺仪")
	PX2_LM_APP:AddItem1("Device", "设备")

	PX2_LM_APP:AddItem1("UIMode", "UI模式")

	PX2_LM_APP:AddItem1("SaveMap", "保存")
	PX2_LM_APP:AddItem1("LoadMap", "加载")
	PX2_LM_APP:AddItem1("UpdateObs", "更新障碍")

	PX2_LM_APP:AddItem1("master", "a1主机")
	PX2_LM_APP:AddItem1("connector", "a2连接器")
	PX2_LM_APP:AddItem1("master_sendlidar", "b1发送数据")
	PX2_LM_APP:AddItem1("master_connector_calculate", "b2接受数据并计算")

	PX2_LM_APP:AddItem1("EncorderMoto", "编码器电机")
	PX2_LM_APP:AddItem1("StepMoto", "步进电机")

	PX2_LM_APP:AddItem1("ClearObst", "清空障碍")
	PX2_LM_APP:AddItem1("AddObst", "增加障碍")
	PX2_LM_APP:AddItem1("RemoveObst", "移除障碍")

	PX2_LM_APP:AddItem1("encoder", "编码器")
	PX2_LM_APP:AddItem1("298N", "298N电驱")

	PX2_LM_APP:AddItem1("MapUpdate", "地图更新")

	PX2_LM_APP:AddItem1("Pos", "位置")
	PX2_LM_APP:AddItem1("PickPos", "点击位置")

	PX2_LM_APP:AddItem1("MapUpdate", "地图更新")
end