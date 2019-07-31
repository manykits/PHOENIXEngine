//  DVUISetting.m

#import "DVUISetting.h"
#import <sys/utsname.h>
#import <UIKit/UIKit.h>

static DVUISetting * ME;
@implementation DVUISetting

+(id)defaultMe{
    @synchronized ([DVUISetting class]){
        if (ME == nil) {
            ME = [[DVUISetting alloc] init];
        }
    }
    return ME;
}

-(instancetype)init{
    self = [super init];
    if (self) {
        int width = [UIScreen mainScreen].bounds.size.width;
        int height = [UIScreen mainScreen].bounds.size.height;
        _sW_Width = width;
        _sH_Height = height;
    }
    return  self;
}


/**
 *  判断具体机型
 *
 *  @return 机型名称
 */
+ (NSString*)deviceModelName
{
    struct utsname systemInfo;
    uname(&systemInfo);
    NSString *deviceModel = [NSString stringWithCString:systemInfo.machine encoding:NSUTF8StringEncoding];
    //iPhone 系列
    if ([deviceModel isEqualToString:@"iPhone1,1"])    return @"iPhone1G";
    if ([deviceModel isEqualToString:@"iPhone1,2"])    return @"iPhone3G";
    if ([deviceModel isEqualToString:@"iPhone2,1"])    return @"iPhone3GS";
    if ([deviceModel isEqualToString:@"iPhone3,1"])    return @"iPhone4";
    if ([deviceModel isEqualToString:@"iPhone3,2"])    return @"VerizoniPhone4";
    if ([deviceModel isEqualToString:@"iPhone4,1"])    return @"iPhone4S";
    if ([deviceModel isEqualToString:@"iPhone5,1"])    return @"iPhone5";
    if ([deviceModel isEqualToString:@"iPhone5,2"])    return @"iPhone5";
    if ([deviceModel isEqualToString:@"iPhone5,3"])    return @"iPhone5C";
    if ([deviceModel isEqualToString:@"iPhone5,4"])    return @"iPhone5C";
    if ([deviceModel isEqualToString:@"iPhone6,1"])    return @"iPhone5S";
    if ([deviceModel isEqualToString:@"iPhone6,2"])    return @"iPhone5S";
    if ([deviceModel isEqualToString:@"iPhone7,1"])    return @"iPhone6Plus";
    if ([deviceModel isEqualToString:@"iPhone7,2"])    return @"iPhone6";
    if ([deviceModel isEqualToString:@"iPhone8,1"])    return @"iPhone6s";
    if ([deviceModel isEqualToString:@"iPhone8,2"])    return @"iPhone6sPlus";
    return deviceModel;
}

@end

