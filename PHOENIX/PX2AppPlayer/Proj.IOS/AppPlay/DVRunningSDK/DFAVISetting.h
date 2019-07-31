//
//  DFAVISetting.h
//  DVRunning
//
//  Created by DFung on 16/3/10.
//  Copyright © 2016年 DevinVon. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface DFAVISetting : NSObject

/**
 *  按固定的时间长度分开录制AVI。（单位：毫秒；置0失效）
 */
@property(nonatomic,assign)UInt32 avi_time;

/**
 *  按固定的文件大小分开录制AVI。（单位：毫秒；置0失效）
 */
@property(nonatomic,assign)UInt32 avi_size;

/**
 *  帧率
 */
@property(nonatomic,assign)UInt32 avi_fps;

/**
 *  屏幕宽
 */
@property(nonatomic,assign)UInt32 avi_width;

/**
 *  屏幕高
 */
@property(nonatomic,assign)UInt32 avi_high;

/**
 *  采样率
 */
@property(nonatomic,assign)UInt32 avi_Rate;

/**
 *  采样位数
 */
@property(nonatomic,assign)UInt32 avi_Number;

/**
 *  声道数
 */
@property(nonatomic,assign)UInt32 avi_channels;

/**
 *  音效开关
 */
@property(nonatomic,assign)BOOL avi_isVoice;


@end
