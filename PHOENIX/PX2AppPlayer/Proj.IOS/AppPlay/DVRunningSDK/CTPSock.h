//
//  CTPSock.h
//  DVRunning
//
//  Created by Mac on 15/11/19.
//  Copyright (c) 2015年 DevinVon. All rights reserved.
//

#import <Foundation/Foundation.h>




/**
 * 【错误回调】
 *  通知名:@"CTP_ERR"
 *  返回参数:(NSString*)错误信息
 */


/**
 * 【动作回调】
 *  通知名:@"CTP_NOTE"
 *  返回参数:@{@"CMD":命令号,@"PARM":参数}
 *  (参数内容详细说明,请看工程内《特殊命令号说明_V0.1》)
 */


@interface CTPSock : NSObject

+(void)connectHost:(NSString*)address Port:(NSInteger)port;
+(void)cutOffLink;


#pragma mark -->> 获取当前的 WiFi
+(NSString*)currentSSID;

#pragma mark -->> 初始化CTP通道
+(void)openLink;

#pragma mark -->> 设置超时时间
+(void)setCtpTime:(int)time;

#pragma mark -->> CTP是否准备好
+(void)Enable;

#pragma mark -->> 获取模式
+(void)ctpMode;

#pragma mark -->> 连接状态
+(BOOL)ctpIsLinked;


#pragma mark -->> 选择模式
/**
 *  用途:小机模式切换。
 *  @param   index:（0：录像  1：拍照  2：回放）
 */
+(void)selectedMode:(int)index;


#pragma mark -->> 获取顶部状态
/**
 *  用途:获取功能项的状态。
 *  @param   array:每个功能项的命令号
 */
+(void)itemTopStateCmd:(NSArray*)array;


#pragma mark -->> 获取状态
/**
 *  用途:获取功能项的状态。
 *  @param   array:每个功能项的命令号
 */
+(void)itemStateCmd:(NSArray*)array;


#pragma mark -->> 选择状态
/**
 *  用途:选择功能项的状态。
 *  @param   cmd:命令号
 *  @param   state:对应的id
 */
+(void)selectedCmd:(NSString*)cmd State:(int)state;





#pragma mark -->> AP模式SSID设置
/**
 *  用途:AP模式SSID设置。
 *  @param   wifi:名字
 */
+(void)ApSSID:(NSString*)wifi;


#pragma mark -->> AP模式密码设置
/**
 *  用途:AP模式密码设置。
 *  @param   Password:密码
 */
+(void)ApPassword:(NSString*)psw;




#pragma mark -->> SP模式SSID设置
/**
 *  用途:SP模式SSID设置。
 *  @param   wifi:名字
 */
+(void)SpSSID:(NSString*)wifi;

#pragma mark -->> SP模式密码设置
/**
 *  用途:SP模式密码设置。
 *  @param   Password:密码
 */
+(void)SpPassword:(NSString*)psw;




#pragma mark -->> 恢复出厂设置
/**
 *  用途:恢复出厂设置。
 */
+(void)restore;


#pragma mark -->> 格式化
/**
 *  用途:格式化。
 */
+(void)format;




#pragma mark -->> SD卡状态
/**
 *  用途:请求SD卡状态。
 */
+(void)checkSDCard;

/**
 *  用途:查看SD卡状态。
 */
+(BOOL)SDCardStatus;



#pragma mark -->> 电池状态
/**
 *  用途:查看电池状态。
 */
+(void)checkBattery;


#pragma mark -->> 拍照
/**
 *  用途:拍照。
 */
+(void)takePicture;





#pragma mark -->> 是否处于拍照状态
/**
 *  用途:是否处于拍照状态。
 */
+(void)isTakingPhoto;


#pragma mark -->> 开始录像
/**
 *  用途:开始录像。
 */
+(void)startRecording;


#pragma mark -->> 停止录像
/**
 *  用途:停止录像。
 */
+(void)stopRecording;



//#pragma mark -->> 获取版本号
/**
 *  用途:获取版本号。（已移除于：2016-03-25 10:36:38 固件升级策略2.0）
 */
//+(void)version;




#pragma mark -->> 删除文件
/**
 *  用途:删除文件。
 */
+(void)deleteFile;


#pragma mark -->> 删除全部文件
/**
 *  用途:删除全部文件。
 */
+(void)deleteAllFile;


#pragma mark -->> 日期设置
/**
 *  用途:日期设置。
 *  @param   timeDic  @{@"YEAR" : @"2015",
 *                      @"MONTH": @"12",
 *                      @"DATE" : @"12",
 *                      @"HOUR" : @"11",
 *                      @"MIN"  : @"30",
 *                      @"SEC"  : @"55"}
 */
+(void)setTime:(NSDictionary*)timeDic;


#pragma mark -->> 设置录像尺寸
/**
 作用: 	设置录像尺寸
 参数: 	参数1:  分辨率ID 『0：720P(30FPS)  1：VGA(30FPS) 2:720P(60FPS) 3:VGA(60FPS) 4:1080P(30FPS)』
 返回: 	参数1:  分辨率ID, 参数2: 对应的通用命令号,参数3: 是否支持1080P(3FPS)
 **/
+(void)setRecSize:(int)index;


#pragma mark -->> 获取录像状态
/**
 *  用途:获取录像状态。
 */
+(void)recStatus;





#pragma mark -->> 获取定时拍照状态
/**
 *  用途:获取定时拍照状态。
 */
+(void)pictureStatus;




//#pragma mark -->> 设置图像尺寸
/**
 *  用途:设置图像尺寸。
 *  @param   index   分辨率ID （0：1M  1：2M  2:3M)
 *
 */
//+(void)setPictureSize:(int)index;

//#pragma mark -->> 设置通用白平衡
/**
 *  用途:设置通用白平衡。
 *  @param   index   白平衡ID （0：自动  1：日光  2:阴天  3：钨丝灯 4：荧光灯)
 *
 */
//+(void)setBlance:(int)index;



//#pragma mark -->> 设置通用连拍模式
/**
 *  用途:设置通用连拍模式。
 *  @param   index   连拍张数ID （0：关  1：3张  2: 5张  3：10张)
 *
 */
//+(void)setContinuousShooting:(int)index;



//#pragma mark -->> 设置录像图片质量
/**
 *  用途:设置录像图片质量 。
 *  @param   index   质量ID （0：极好  1：好  2: 一般 )
 *
 */
//+(void)setVideoQuality:(int)index;



//#pragma mark -->> 定时拍照
/**
 *  用途:定时拍照。
 *  @param   index:（0：关 1：定时2秒 2：定时10秒)
 */
//+(void)takePictureInTime:(int)index;


//#pragma mark -->> 设置测光模式
/**
 *  用途:设置测光模式 。
 *  @param   state:开/关
 *
 */
//+(void)openPhotometry:(BOOL)state;







#pragma mark -->> 获取/设置小机UUID
/**
 *  用途:获取/设置小机UUID。
 *  @param   uuid     命令号     1.nil：表示获取UUID。
 */
+(void)deviceUUID:(NSString*)uuid;

#pragma mark -->>当前UUID
/**
 *  用途:当前UUID。
 */
+(NSString*)currentUUID;



#pragma mark -->> 请求进入FTP
/**
 *  用途:请求进入FTP。
 */
+(void)FtpEnter;

#pragma mark -->> 请求退出FTP
/**
 *  用途:请求退出FTP。
 */
+(void)FtpExit;




#pragma mark -->> 获取语言
/**
 *  用途:获取语言。
 */
+(void)getLanguage;


#pragma mark -->> 设置语言
/**
 *  用途:设置语言。
 *  @param   index   语言ID『0：简中  1：繁中  2：英语  3：德语  4：日语』
 */
+(void)setLanguage:(int)index;


#pragma mark -->> 固件升级
/**
 *  用途:固件升级。
 */
+(void)updateDevice;


#pragma mark -->> 重启设备
/**
 *  用途:重启设备。
 */
+(void)rebootDevice;


#pragma mark -->> 关闭设备
/**
 *  用途:关闭设备。
 */
+(void)shutDownDevice;



#pragma mark -->> 刷新顶栏状态
/**
 *  用途:刷新顶栏状态。
 */
+(void)refreshTopItems:(int)index;



#pragma mark -->> 获取SSID前缀
/**
 *  用途:获取SSID前缀。
 */
+(void)SsidPrefix;


#pragma mark -->> 自定义特殊命令
/**
 *  用途:用户自定义CTP数据。
 *  @param   cmd     命令号     -->> @"0001"
 *  @param   parm    参数组     -->> @[@"字符串1",@"字符串2"]
 */
+(void)specialCmd:(NSString*)cmd Parm:(NSArray*)parm;


#pragma mark -->> 用户自定义CTP
/**
 *  用途:用户自定义CTP数据。
 *  @param   cmd     命令号     -->> @"0001"
 *  @param   parm    参数组     -->> @[@"字符串1",@"字符串2"]
 *  (注:通知@"CTP_CUSTOM"回调,参数是字典。)
 */
+(void)customCmd:(NSString*)cmd Parm:(NSArray*)parm;




#pragma mark -->> 开启实时流
/**
 *  用途:开启实时流。
 *  @param      index     参数1:  WIFI串流的清晰度（1：流畅  5：普清 9：高清）
 *  @param      mode      参数2:（0：VGA  1：720P ）
 */
+(void)openRtsp:(int)index Mode:(int)mode;

#pragma mark -->> 关闭实时流
+(void)closeRtsp;

#pragma mark -->> 打开后视UVC实时流

/**
 用途：开启后方实时流

 @param index  参数1:  WIFI串流的清晰度（1：流畅  5：普清 9：高清）
 @param mode   参数2:（0：VGA  1：720P ）
 */
+(void)openUVCRtsp:(int)index Mode:(int)mode;

#pragma mark -->> 关闭后视UVC实时流

/**
 用途：关闭后方实时流
 */
+(void)closeUVCRtsp;


#pragma mark -->> 获取视频列表
/**
 *  用途:获取视频列表。(JSON格式)
 */
+(void)movieJson;

#pragma mark -->> 获取抓拍半径
+(void)movieGap;

#pragma mark -->> 获取略缩图命令
/**
 *  用途:获取略缩图命令。
 *  @param      name     文件名
 *  @param      time     文件偏移时间
 *  @param      counts   缩略图数量
 *  @param      gap      缩略图间隔(秒)
 *  @param      endSeek  文件名在vf_order.txt的偏移(取末偏移)
 */
+(void)screenshot:(NSString*)name Time:(int)time Counts:(int)count Gap:(int)gap EndSeek:(int)endSeek;

#pragma mark -->> 发送播放时间轴命令
/**
 *  用途:发送播放时间轴命令。
 *  @param      name     起始播放文件名     --> B:/DCIMA/REC00001.AVI
 *  @param      time     文件偏移时间       --> 1s(秒)
 *  @param      type     (0：播放 1：下载)
 *  @param      endSeek  文件名在vf_order.txt的偏移(取末偏移)
 */
+(void)timeAxisPlay:(NSString*)name Time:(int)time Type:(int)type EndSeek:(int)endSeek;

#pragma mark -->> 调整时间轴快速播放等级,默认为 X1
/**
 *  用途:调整时间轴快速播放等级,默认为 X1。
 *  @param   index  『0：原速 , 1：二倍速 ,   2：4倍速 ,   3：8倍速 ,   4：16倍速,   5:32倍速』
 */
+(void)timeAxisPlayLevel:(int)index;

//#pragma mark -->> 发送暂停时间轴命令
//+(void)timeAxisPause;

//#pragma mark -->> 发送继续时间轴命令
//+(void)timeAxisContinue;

#pragma mark -->> 发送停止时间轴命令
+(void)timeAxisStop;

//#pragma mark -->> 请求设备端减慢数据量发送
//+(void)timeAxisDecelerate;

//#pragma mark -->> 请求设备端加快数据量发送
//+(void)timeAxisAccelerate;


#pragma mark -->> 实时流是否静音
/**
 *  实时流是否静音
 *
 *  @param type 『NO：否  YES：是 』
 */
+(void)openVoice:(BOOL)type;

#pragma mark --->> 调节数字变焦
/**
 *  调节数字焦距
 *
 *  @param type 『YES：缩小  NO：放大 』
 */

+(void)focalSetting:(BOOL)type;



#pragma mark --->> 获取小机摄像头当前外部光线，返回值0~16
/**
 *  获取小机摄像头当前外部光线，返回值为0-16
 *  小机返回命令号：0057
 *  参数：0-16
 */
+(void)machineExternalLight;




#pragma mark --->> 获取小机剩余录像时间
/**
 *  获取小机剩余录像时间
 *  小机返回命令号：0059
 *
 */
+(void)remainingRecording;


#pragma mark --->> 获取剩余拍照数量
/**
 *  获取小机剩余拍照数量
 *  小机返回命令号:0060
 *
 */

+(void)remainingPhoto;


#pragma mark -->> 设置发送心跳包时间

/**
 *  设置发送心跳包时间
 *
 *  @param time    心跳包间隔时间
 *  @param timeout 检测超时时间
 */
+(void)setheartTime:(int)time TimeOut:(int)timeout;


#pragma mark <后视摄像头相关接口>
/**
 获取 后视视频文件信息汇总文件名
 */
+ (void)movieJsonUVC;

/**
 获取 后视缩略图命令
 
 @param name 文件名
 @param time 起始文件偏移时间
 @param count 缩略图个数
 @param gap 缩略图间隔
 @param endSeek 文件名在vf_order.txt中的偏移
 */
+(void)screenUVCshot:(NSString*)name Time:(int)time Counts:(int)count Gap:(int)gap EndSeek:(int)endSeek;
/**
 发送播放后市时间轴命令
 
 @param name 起始播放文件名
 @param time 起始播放文件偏移时间（秒为单位）
 @param type 『0：播放 1：下载』
 @param endSeek 文件名在vf_order.txt的偏移(取后偏移)
 */
+(void)timeAxisUVCPlay:(NSString*)name Time:(int)time Type:(int)type EndSeek:(int)endSeek;
/**
 调整后事时间轴快速播放等级，默认为 X1
 
 @param index 『0：原速 , 1：二倍速 ,   2：4倍速 ,   3：8倍速 ,   4：16倍速,   5:32倍速,  6:64倍速』
 */
+(void)timeAxisUVCPlayLevel:(int)index;

/**
 发送后视停止时间轴命令
 */
+(void)timeAxisUVCStop;
/**
 设置、获取设备名称（最大允许超过250字节）
 
 @param name 设备名
 */
+(void)deviceNameSet:(NSString *)name;
/**
 检测设备是否在倒车
 “CTP_NOTE”
 通知返回 参数1：『0：不在倒车  1：正在倒车 』
 */
+(void)deviceReversing;


@end
