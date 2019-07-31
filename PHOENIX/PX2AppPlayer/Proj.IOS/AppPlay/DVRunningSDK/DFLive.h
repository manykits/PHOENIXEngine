//
//  DFLive.h
//  DVRunning
//
//  Created by DFung on 16/3/8.
//  Copyright © 2016年 DevinVon. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "DFAVISetting.h"
#import "player_head.h"



typedef enum : NSUInteger
{
    QUALITY_LOW     = 1,    //流畅
    QUALITY_MEDIUM  = 5,    //普清
    QUALITY_HIGH    = 9,    //高清
    
} QUALITY;

typedef enum : NSUInteger
{
//    参数2:（0：VGA  1：720P ）
    STREAM_HD       = 1,
    STREAM_VGA      = 0,
    
}STREAMMODE;

typedef NS_ENUM(NSUInteger)
{
    TYPE_OF_REAR = 0, //后置
    TYPE_OF_FRONT = 1 //前置

}TYPEVIDEO;


@class DFLive;
@protocol DFLiveDelegate <NSObject>
@optional

/**
 *  (返回实时流的状态。)
 *  
 *  @param isRun = YES  ==>>  正在播放。
 *  @param isRun = NO   ==>>  已经停止。
 */
-(void)onDFLive:(DFLive *)objc Status:(BOOL)isRun;

/**
 *  (返回实时流的JPEG数据。)
 *
 *  @param data  ==> JPEG数据。
 */
-(void)onDFLive:(DFLive *)objc ImageData:(NSData*)data;

/**
 *  (返回实时流的音频数据。)
 *
 *  @param data  ==> 音频数据。
 *
 *  1.播放音频时,在该回调方法内(需释放原始数据)：
 *      [DFLive playAudioBuffer:p_data];
 *      freeMediaData(p_data):    //释放原始数据
 *
 */
-(void)onDFLive:(DFLive *)objc AudioData:(struct player_data*)data;


/**
 *  (返回录制好后的AVI文件路径。)
 */
-(void)onDFLive:(DFLive *)objc AVIPath:(NSString*)path;

@end



@interface DFLive : NSObject
@property(nonatomic,assign)id<DFLiveDelegate>delegate;

/**
 *  创建一个DFLive类，并指定一个代理。
 */
+(void)newOnDelegate:(id)delegate;

/**
 *  当前实时流状态。
 */
+(BOOL)isLiving;

/**
 *  开始实时流。
 *  @param  quality     (可选：流畅、普清、高清.)
 *  @param  mode        (0：VGA  1：720P )
 *  @param  type        (TYPE_OF_REAR=后置，TYPE_OF_FRONT=前置)
 */
+(void)didStartLiving:(QUALITY)quality Mode:(int)mode Type:(TYPEVIDEO)type;

/**
 *  停止实时流。
 */
+(void)didStopLiving;

/**
 *  用于播放音频数据。
 */
+(void)playAudioBuffer:(struct player_data*)p_data;

/**
 *  开始录制AVI文件。
 *
 *  @param setting
 *
 *   Default setting:
 *   {
 *      DFAVISetting * setting = [DFAVISetting new];
 *      setting.avi_Rate     = 8000;     //采样率
 *      setting.avi_Number   = 16;       //采样位数
 *      setting.avi_channels = 1;        //声道数
 *      setting.avi_fps      = 30;       //帧率
 *      setting.avi_width    = 640;      //屏幕宽（RTSP中默认640）
 *      setting.avi_high     = 480;      //屏幕高（RTSP中默认480）
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
 *  完成录制AVI文件。
 */
+(void)didFinishAVI;


@end

