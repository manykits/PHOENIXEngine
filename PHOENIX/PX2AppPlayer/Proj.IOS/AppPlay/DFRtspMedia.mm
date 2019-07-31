//  DFRtspMedia.m

#import "DFRtspMedia.h"
#import "DVUISetting.h"
#import "DFFile.h"
#include <stdio.h>
#include <notify.h>
#define NotificationLock        CFSTR("com.apple.springboard.lockcomplete")
#define NotificationChange      CFSTR("com.apple.springboard.lockstate")

@interface DFRtspMedia()<DFLiveDelegate>{
    int mWidth;
    int mHeight;
}
@end

@implementation DFRtspMedia

-(instancetype)initwithWidth:(NSInteger)width Height:(NSInteger)height
{
    mWidth = width;
    mHeight =  height;
    
    [DFLive newOnDelegate:self];
    
    CFNotificationCenterAddObserver(CFNotificationCenterGetDarwinNotifyCenter(), NULL, screenLockStateChanged, NotificationLock, NULL, CFNotificationSuspensionBehaviorDeliverImmediately);
    CFNotificationCenterAddObserver(CFNotificationCenterGetDarwinNotifyCenter(), NULL, screenLockStateChanged, NotificationChange, NULL, CFNotificationSuspensionBehaviorDeliverImmediately);

    return self;
}

-(BOOL)isPlaying{
    return [DFLive isLiving];
}

-(void)play:(QUALITY)quality{

    _QL = quality;
    [DFLive didStartLiving:quality Mode:0 Type:TYPE_OF_FRONT];
}


-(void)stop{
    [DFLive didStopLiving];
}

#pragma mark --> 【DFLiveDelegate】
-(void)onDFLive:(DFLive *)objc Status:(BOOL)isRun{
    if (isRun)
    {
    }
    else
    {
    }
}

- (unsigned char *)pixelBRGABytesFromImage:(UIImage *)image {
    return [self pixelBRGABytesFromImageRef:image.CGImage];
}

- (unsigned char *)pixelBRGABytesFromImageRef:(CGImage *)cgImage {
    
    NSUInteger iWidth = CGImageGetWidth(cgImage);
    NSUInteger iHeight = CGImageGetHeight(cgImage);
    NSUInteger iBytesPerPixel = 4;
    NSUInteger iBytesPerRow = iBytesPerPixel * iWidth;
    NSUInteger iBitsPerComponent = 8;
    unsigned char *imageBytes = (unsigned char*)malloc(iWidth * iHeight * iBytesPerPixel);
    
    CGColorSpaceRef colorspace = CGColorSpaceCreateDeviceRGB();
    
    CGContextRef context = CGBitmapContextCreate(imageBytes,
                                                 iWidth,
                                                 iHeight,
                                                 iBitsPerComponent,
                                                 iBytesPerRow,
                                                 colorspace,
                                                 kCGBitmapByteOrder32Little | kCGImageAlphaPremultipliedFirst);
    
    CGRect rect = CGRectMake(0 , 0 , iWidth , iHeight);
    CGContextDrawImage(context , rect ,cgImage);
    CGColorSpaceRelease(colorspace);
    CGContextRelease(context);
    
    return imageBytes;
}

-(void)onDFLive:(DFLive *)objc ImageData:(NSData *)data{
    
    if (data)
    {
        UIImage *image = [UIImage imageWithData:data];
        int width = image.size.width;
        int height = image.size.height;
        int size = width*height*4;
      
        unsigned char *imageBytes = [self pixelBRGABytesFromImage:image];
        const char *bytes = (const char *)imageBytes;
        appplay::NativeCall::SetHardCameraFrame(width, height, bytes, size);
        free(imageBytes);
    }
     
}

-(void)onDFLive:(DFLive *)objc AudioData:(struct player_data *)data{
    //[DFLive playAudioBuffer:data];
    //free_player_data(data);
}


-(void)onDFLive:(DFLive *)objc AVIPath:(NSString *)path{
    //"4c0fc71dc72-70104064-20160311210112-REC00017.AVI",
    
    // .apv
    NSString *lastName = [path lastPathComponent];
    
    NSString *devName = @"JL0fc71dc72";
    
    NSString *aviLenth = [NSString stringWithFormat:@"%lld",[self fileSizeAtPath:path]];
    
    NSString *aviTime = [self nowDays];
    
    NSString *newName = [NSString stringWithFormat:@"%@-%@-%@-%@",devName,aviLenth,aviTime,lastName];
    
    
    NSString *libNsstr = [NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES) lastObject];
    NSString *tmpDoct = [NSString stringWithFormat:@"%@/DVMedias/%@",libNsstr,newName];
    NSString *newFilePath = [tmpDoct stringByReplacingOccurrencesOfString:@".avi" withString:@".AVI"];
    newFilePath = [newFilePath stringByReplacingOccurrencesOfString:@"_" withString:@""];
    
    [DFFile renameOldName:path NewName:newFilePath];
    
    NSLog(@"Saved rtsp avi:%@",newFilePath);
    
    [self createAPVFile:newFilePath];
}

#pragma mark -->> /*--- 锁屏通知回调 ---*/
static void screenLockStateChanged(CFNotificationCenterRef center,
                                   void * observer,
                                   CFStringRef name,
                                   const void * object,
                                   CFDictionaryRef userInfo)
{
    NSString * lockState = (__bridge NSString*)name;
    if ([lockState isEqualToString:(__bridge NSString*)NotificationLock])
    {
        [DFLive didStopLiving];
        //NSLog(@"Screen was Locked.");
    }else{
        //NSLog(@"Screen was Unlocked.");
    }
}

//单个文件的大小
-(long long) fileSizeAtPath:(NSString*) filePath{
    
    NSFileManager* manager = [NSFileManager defaultManager];
    
    if ([manager fileExistsAtPath:filePath]){
        return [manager attributesOfItemAtPath:filePath error:nil].fileSize;
    }
    return 0;
    
}

/**
 *  get当前时间
 *
 *  @return yyyyMMDDHHMMSS
 */
-(NSString *)nowDays{
    // 获取系统当前时间
    NSDate * date = [NSDate date];
    NSTimeInterval sec = [date timeIntervalSinceNow];
    NSDate * currentDate = [[NSDate alloc] initWithTimeIntervalSinceNow:sec];
    
    //设置时间输出格式：
    NSDateFormatter * df = [[NSDateFormatter alloc] init ];
    [df setDateFormat:@"yyyyMMddHHmmss"];
    NSString * na = [df stringFromDate:currentDate];
    return na;
}

/**
 *  创建预览图
 *
 *  @param path AVI文件路径
 */

-(void)createAPVFile:(NSString *)path{
    
    NSDictionary *dict = [DFFile previewAVI:path];
    NSFileManager *fm = [NSFileManager defaultManager];
    
    if (dict)
    {
        NSString *fileNameWith = [path lastPathComponent];
        NSString *fileName = [fileNameWith stringByDeletingPathExtension];
        
        NSString *aviTime = dict[@"TIME"];
        NSData *avpData = dict[@"IMG"];
        NSString *aviSize = dict[@"SIZE"];
        
        NSString *newPath = [NSString stringWithFormat:@"%@_%@_%@.apv",fileName,aviTime,aviSize];
        
        NSString *DocPath = [NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES) lastObject];
        
        NSString *newFilePath = [NSString stringWithFormat:@"%@/DVMedias/%@",DocPath,newPath];
        
        [fm createFileAtPath:newFilePath contents:avpData attributes:nil];
    }
    else
    {
        [fm removeItemAtPath:path error:nil];
    }
}

@end

