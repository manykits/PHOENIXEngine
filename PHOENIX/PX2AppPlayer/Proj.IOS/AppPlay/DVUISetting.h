//  DVUISetting.h

#import <Foundation/Foundation.h>

@interface DVUISetting : NSObject


@property (nonatomic, assign)int UIIndex;
//@property (nonatomic, assign)BOOL isLock;        //RTSP锁
@property (nonatomic, assign)BOOL isTimingPhoto;

@property (nonatomic,assign) float sW_Width;
@property (nonatomic,assign) float sH_Height;

+(id)defaultMe;
/**
 *  判断具体机型
 *
 *  @return 机型名称
 */
+ (NSString*)deviceModelName;
@end

