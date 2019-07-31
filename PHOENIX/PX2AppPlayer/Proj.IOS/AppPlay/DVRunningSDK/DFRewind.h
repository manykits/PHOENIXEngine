//
//  DFRewind.h
//  DVRunning
//
//  Created by DFung on 16/3/8.
//  Copyright © 2016年 DevinVon. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "player_head.h"
#import "DFAVISetting.h"



typedef enum : NSUInteger {
    DFREWIND_STATE_PLAY,
    DFREWIND_STATE_PAUSE,
    DFREWIND_STATE_STOP,
    DFREWIND_STATE_BLOCKING,
} DFREWIND_STATE;

typedef enum : NSUInteger {
    DFREWIND_SPEED_01 = 0,      //原速度
    DFREWIND_SPEED_02 = 1,      //2倍速度
    DFREWIND_SPEED_04 = 2,      //...
    DFREWIND_SPEED_08 = 3,
    DFREWIND_SPEED_16 = 4,
    DFREWIND_SPEED_32 = 5,
    DFREWIND_SPEED_64 = 6,
} DFREWIND_SPEED;


typedef enum : NSUInteger {
    
    DFREWIND_TYPE_REAR = 0, //后置摄像头
    DFREWIND_TYPE_FRONT = 1 //前置摄像头
    
    
}DFREWIND_TYPE;


@class DFRewind;
@protocol DFRewindDelegate <NSObject>
@optional

/**
 *  (返回当前回放的状态，用【DFREWIND_STATE】表明。)
 */
-(void)onDFRewind:(DFRewind *)objc OutState:(DFREWIND_STATE)state;

/**
 *  (返回音频数据。)
 *
 *  return YES (即可)
 */
-(BOOL)onDFRewind:(DFRewind *)objc OutAudio:(struct player_data1 *)p_data;

/**
 *  (返回视频数据。)
 *
 *  1.在回调方法内转换数据（并且释放原始数据）：
 *      NSData * tmpData = [[NSData alloc] initWithBytes:p_data->data length:p_data->size];
 *      UIImage * image = [[UIImage alloc] initWithData:tmpData];
 *      free(p_data);    //释放原始数据
 */
-(void)onDFRewind:(DFRewind *)objc OutVideo:(struct player_data1 *)p_data;

/**
 *  (返回多张预览图数据。)
 *
 *  imageDic = {@"DATA":图片数据
 *              @"TIME":时间}
 *  isOk  YES:已完成，NO:未完成
 */
-(void)onDFRewind:(DFRewind *)objc OutMorePreviews:(NSDictionary*)imageDic Finished:(BOOL)isOk;

/**
 *  (返回录制好后的AVI文件路径。)
 *
 *  rate：录制过程返回进度百分比。
 *  path：录制完成后才会返回的路径。
 */
-(void)onDFRewind:(DFRewind *)objc OutAVI:(NSString*)path Progress:(float)rate;

/**
 *  (返回当前播放速度。)
 */
-(void)onDFRewind:(DFRewind *)objc OutSpeed:(DFREWIND_SPEED)speed;

@end





@interface DFRewind : NSObject
@property(nonatomic,assign)id<DFRewindDelegate>delegate;
@property(nonatomic,strong)NSString *downFile;
@property(nonatomic,assign)DFREWIND_TYPE rewindType;


/**
 设置前置/后置摄像头的类型

 @param type 摄像头类型（0 ：后置 1：前置）
 */
+(void)setRewindType:(int) type ;


/**
 获取摄像头类型

 @return 摄像头类型
 */
+ (DFREWIND_TYPE)getRewindType;

/**
 *  创建DFRewind并且指定DFRewind的代理类。（必要的）
 */
+(void)newOnDelegate:(id)delegate;

/**
 *  用于播放音频数据。
 */
//+(void)playAudioBuffer:(struct player_data1*)p_data;

/**
 *  解析数据头。
 *
 *  @return @{@"SEQ" :@"1",
 *            @"SIZE":@"8142",
 *            @"DATE":@"20160806010203"
 *            @"FILE":@"B:/DCIMA/REC00000.AVI"};
 */
+(NSDictionary*)analyzeHead:(struct player_data1*)p_data;

/**
 *  注销一个DFRewind类。
 */
+(void)releaseMe;

/**
 *  在回放模式中，获取缩略图。
 *  @param      name        文件名                  --> B:/DCIMA/REC00001.AVI
 *  @param      time        偏移时间                --> 1s(秒)
 *  @param      counts      缩略图数量
 *  @param      gap         缩略图间隔(秒)
 *  @param      endSeek     文件名在vf_order.txt的偏移(取末偏移)
 */
+(void)didPreview:(NSString*)name Time:(int)time Counts:(int)count Gap:(int)gap EndSeek:(int)endSeek;

/**
 *  在回放模式中，选择时间点开始播放。
 *  @param      name        文件名                  --> B:/DCIMA/REC00001.AVI
 *  @param      time        偏移时间                --> 1s(秒)
 *  @param      type        (0：播放 1：下载)        默认填写“0”
 *  @param      endSeek     文件名在vf_order.txt的偏移(取末偏移)
 *  @param      utype       前后摄像头类型（0：后摄像头，1前摄像头）
 */
+(void)didPlay:(NSString*)name Time:(int)time Type:(int)type EndSeek:(int)endSeek;

/**
 *  在回放模式中，选择时间点开始播放。
 *  @param      timeA        开始时间               @"20160915203055"
 *  @param      timeB        结束时间               @"20160915203555"
 */
+(BOOL)didAviDownloadFrom:(NSString*)timeA To:(NSString*)timeB;


/**
 *  设置时间轴快速播放的速度。
 */
+(void)setPlaySpeed:(DFREWIND_SPEED)speed;

/**
 *  暂停回放。
 */
+(void)didPause;

/**
 *  继续回放。
 */
+(void)didContinue;

/**
 *  停止回放。
 */
+(void)didStop;

/**
 *  开始生成AVI文件。
 *
 *  @param setting
 *
 *   Default setting:
 *   {
 *      DFAVISetting * setting = [DFAVISetting new];
 *      setting.avi_Rate     = 8000;     //采样率
 *      setting.avi_Number   = 16;       //采样位数
 *      setting.avi_channels = 1;        //声道数
 *      setting.avi_fps      = 30;       //帧率（暂无需设置）
 *      setting.avi_width    = 1280;     //屏幕宽（暂无需设置）
 *      setting.avi_high     = 720;      //屏幕高（暂无需设置）
 *      setting.avi_isVoice  = YES;      //开启音效
 *      setting.avi_time     = 0;        //按固定的时间长度分开录制AVI。（单位：毫秒；置0失效）
 *      setting.avi_size     = 0;        //按固定的文件大小分开录制AVI。（单位：毫秒；置0失效）
 *   }
 *  @return 成功与否。
 */
+(BOOL)didSaveAVIWith:(DFAVISetting*)setting;

/**
 *  暂停录制AVI文件。
 */
+(void)didPauseAVI;

/**
 *  继续录制AVI文件。
 */
+(void)didContinueAVI;

/**
 *  完成生成AVI文件。
 */
+(void)didFinishAVI;



@end






