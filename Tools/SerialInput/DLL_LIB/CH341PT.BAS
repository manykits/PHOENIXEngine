Attribute VB_Name = "CH341PTDLL"
' 2005.07.28
'****************************************
'**  Copyright  (C)  W.ch  1999-2005   **
'**  Web:  http:'www.winchiphead.com  **
'****************************************
'**  DLL for USB interface chip CH341  **
'**  C, VC5.0                          **
'****************************************
'
' USB总线接口芯片CH341端口应用层接口库 V1.0
' 南京沁恒电子有限公司  作者: W.ch 2005.07
' CH341-Port-DLL  V1.0
' 运行环境: Windows 98/ME, Windows 2000/XP
' support USB chip: CH341, CH341A
' USB => Serial, Parallel
'



Public Const MAX_DEVICE_PATH_SIZE = 128               ' 设备名称的最大字符数
Public Const MAX_DEVICE_ID_SIZE = 64                  ' 设备ID的最大字符数

Declare Function CH341PtGetVersion Lib "CH341PT.DLL" () As Long   ' 获得DLL版本号,返回版本号

Declare Function CH341PtHandleIsCH341 Lib "CH341PT.DLL" (ByVal iPortHandle As Long) As Boolean   ' 指定已经打开的端口句柄
'    CH341PtHandleIsCH341   检查已经打开的端口是否为CH341端口
'    iPortHandle   '        指定已经打开的端口句柄

Declare Function CH341PtNameIsCH341 Lib "CH341PT.DLL" (ByVal iPortName As String) As Boolean
'    CH341PtNameIsCH341    检查指定名称的端口是否为CH341端口
'    iPortName             指定端口名称,例如"COM3","COM15"等,该端口必须未被占用(指未被其它程序打开)

' 端口设备事件通知回调程序
' sub mPCH341PT_NOTIFY_ROUTINE  (byval iDevIndexAndEvent as long)  ' 端口设备序号和事件及当前状态(参考下行说明)
' iDevIndexAndEvent: 正数说明是设备插入事件/已经插入, 负数说明是设备拔出事件/已经拔出, 其绝对值是设备序号(1到255)

Declare Function CH341PtSetDevNotify Lib "CH341PT.DLL" (ByVal iDeviceID As String, ByVal iNotifyRoutine As Long) As Boolean       ' 指定端口设备事件回调程序,为NULL则取消事件通知,否则在检测到事件时调用该程序
'    CH341PtSetDevNotify      设定端口设备事件通知程序
'    iDeviceID,               可选参数,指向以\0终止的字符串,指定被监控的设备的ID或者不含序号的主名称,对于串口该参数必须为NULL或者"COM"
'    iNotifyRoutine           指定端口设备事件回调程序,为NULL则取消事件通知,否则在检测到事件时调用该程序

'/* 即插即用设备的应用程序编程参考
'1. 主程序启动，默认是禁止数据传输的，只有在确定有CH341端口可用并且打开端口后才进行数据传输
'2. 调用CH341PtSetDevNotify设置插拔监视，如果将来发生CH341端口的插拔事件DLL将会自动调用iNotifyRoutine所指定的子程序或者函数
'3. 从端口1到端口255（正常到20即可）逐个调用CH341PtNameIsCH341确定是否为CH341端口，如果返回是则记忆其端口号并打开后开始传输，如果返回否则休息
'4. 如果iNotifyRoutine收到事件通知，那么可以在保存事件参数后通知主程序处理，也可以在该子程序中处理，
'   分析事件参数，如果是正数则说明有一个端口插入，需要打开时应该首先用CH341PtNameIsCH341判断是否为CH341端口，是则记忆其端口号并打开后开始传输，
'                 如果是负数则说明有一个端口拔出，判断其端口号（用0减去该负数得端口号）是否与记忆的已打开的端口号相同，是则应该及时关闭
'5. 当前已经打开的端口号应该保存为全局变量, 如果端口未打开或者被关闭，那么应用程序应该停止数据传输
'6. 如果事先未用CH341PtNameIsCH341确定是否为CH341端口，那么在端口已经打开后，也可以调用CH341PtHandleIsCH341判断是否为CH341端口
'*/



