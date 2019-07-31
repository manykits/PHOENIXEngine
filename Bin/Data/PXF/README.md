# PHOENIXFrameFirewall

## 简述 ##
PHOENIXFrame个人DIY机器人(以下简称PXF)，是一个帮助你学习编程，从0到1创造机器人的硬件。

Arduino是全球使用范围最广的电子DIY单片机。此程序是一个和Arduino进行通信的程序。

PXF最大的特点是**结构简单**，使用**通用传感器**。致力于让更多的人能够学会编程，开发机器人。在AI时代，编程的重要性更加突出，互联网连接行业，人类通过编程和万物进行连接，PXF是为打开编程学习大门的好帮手。

## 链接 ##
官方网站：[http://www.manykit.com](http://www.manykit.com "http://www.manykit.com")

编程学习教程：[PHOENIXFrame编程之旅](PHOENIXFrame编程之旅 "https://manykit.gitbooks.io/phoenixframe-programming-journey/content/")


QQ群：336273110

## 内容 ##
**PXFArduinoFirewall**:PXF的Arduino主控芯片通信协议代码，将Arduino绝大多数功能进行导出，使用Arduino编辑环境上传到Arduino，通过串口或者蓝牙直接调用Arduino的相关功能

    if (!sOptTypeStr[OT_PM].compareTo(mCmdParams[0]))
    {
      int pin = _Str2Pin(mCmdParams[1]);
      int val = _Str2IO(mCmdParams[2]);
      pinMode(pin, val);
    }

**PHOENIXFrameFirewallMakeBlock**:与MakeBlock开发板相兼容的防火墙代码。

**PHOENIXFrameLineFollow**:寻线小车代码。通过寻线传感器，小车进行自动路径跟踪，沿着黑线行动。

**PHOENIXFrameUltrasonic**:超声波控制小车代码。当小车遇到障碍时进行左拐躲避障碍。（注意调整寻线传感器测试距离，以达到最好的测试效果）

**PHOENIXFrameUltrasonicMatrix**:超声波阵列。检测机器人6个方向的是否有障碍。

**PHOENIXFrameMPU6050**:陀螺仪，加速度仪检测代码。

