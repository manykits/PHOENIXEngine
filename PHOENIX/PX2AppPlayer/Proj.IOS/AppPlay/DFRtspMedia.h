//  DFRtspMedia.h

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "DFLive.h"
#include "AppPlay.hpp"

@interface DFRtspMedia : NSObject
@property(assign,nonatomic) QUALITY QL;

-(instancetype)initwithWidth:(NSInteger)width Height:(NSInteger)height;

/*--- 播放RTSP流 ---*/
-(void)play:(QUALITY)quality;

/*--- 停止RTSP流 ---*/
-(void)stop;

/*--- 是否在播放 ---*/
-(BOOL)isPlaying;

@end

