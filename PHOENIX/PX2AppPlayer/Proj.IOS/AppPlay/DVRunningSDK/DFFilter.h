//
//  DFFilter.h
//  DVRunning
//
//  Created by DFung on 16/2/14.
//  Copyright © 2016年 DevinVon. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface DFFilter : NSObject

/**
 *  【本工程适用】 用途:筛选时间列表，并降序排列。
 *  不用则填写-1;
 */
+(NSArray *)filter:(NSArray*)array Year:(int)y Month:(int)mo Day:(int)d Hour:(int)h Min:(int)mi Sec:(int)s;

/**
 *  【本工程适用】 用途:仅仅筛选时间列表。(用于检测是否某个时刻是否存在视频帧)
 *  不用则填写-1;
 */
+(NSArray*)isExistTime:(NSArray*)array Year:(int)y Month:(int)mo Day:(int)d Hour:(int)h Min:(int)mi Sec:(int)s;

/**
 *  【本工程适用】用途:筛选时间列表，设置升降序排列。 => 针对->原始数据类型 NSDictionary
 */
+(NSArray *)filter:(NSArray*)array IsAscending:(BOOL)isOn;

/**
 *  【本工程适用】用途:筛选时间列表，设置升降序排列。 => 针对->VideoInfo数据类型 (@class VideoInfo)
 */
+(NSArray *)filterInfos:(NSArray*)array IsAscending:(BOOL)isOn;

/**
 *  【本工程适用】用途:降序排列时间轴。
 */
+(NSArray*)listDate:(NSArray*)array;

/**
 *  【本工程适用】用途:最近文件末尾时间。
 */
+(NSString*)lastFileDate;

/**
 *  【本工程适用】用途:读取视频描述文件。
 */
+(NSArray*)readVideoList;

/**
 *  【本工程适用】用途:读取视频描述文件Info。(@class VideoInfo)
 */
+(NSArray*)readVideoInfo;

/**
 *  【本工程适用】用途:选择否时刻。
 *  @param          date     @"2016_02_18_11_28_31"
 *  @return         @"B:/DCIMA/REC00000.AVI_31_201602181128"    @“寻找到文件_偏移量_日期“
 */
+(NSString*)selectDate:(NSString*)date;

/**
 *  【本工程适用】用途:查询当前播放那个文件。
 *  @return         @"B:/DCIMA/REC00000.AVI"
 */
+(NSString *)whichFile:(NSArray*)array Year:(int)y Month:(int)mo Day:(int)d Hour:(int)h Min:(int)mi Sec:(int)s;

/**
 *  【本工程适用】用途:时间轴信息描述文本。
 *  @return         文本路径
 */
+(NSString*)produceTimeLineTxT:(NSArray*)array;

/**
 *  【本工程适用】用途:选择时间信息。
 *  @param          date    @"20160411110020"
 *                          @"201604111100"
 *                          @"2016041111"
 *                          @"20160411"
 *                          @"201604"
 *                          @"2016"
 *
 *  @return         相关点的信息：
 *                  ["0<@#@>B:/DCIMA/REC00000.AVI<@#@>20160411110020<@#@>0",
 *                   "1<@#@>B:/DCIMA/REC00000.AVI<@#@>20160411110021<@#@>1",
 *                   "2<@#@>B:/DCIMA/REC00000.AVI<@#@>20160411110022<@#@>2",
 *                   "3<@#@>B:/DCIMA/REC00000.AVI<@#@>20160411110023<@#@>3"]
 */
+(NSArray*)pickWithDate:(NSString*)date;

/**
 *  【本工程适用】用途:文件名在vf_order.txt的偏移(取末偏移)。
 *  @param      @"B:/DCIMA/REC00000.AVI"
 */
+(NSInteger)endSeekOfFile:(NSString*)file;

@end




@interface VideoInfo : NSObject

@property(nonatomic,assign) int type;
@property(nonatomic,strong) NSString *path;
@property(nonatomic,assign) long long date;
@property(nonatomic,assign) int time;

@end