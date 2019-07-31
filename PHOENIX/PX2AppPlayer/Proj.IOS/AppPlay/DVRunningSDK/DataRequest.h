//
//  DataRequest.h
//  WifiAudio
//
//  Created by zhjl on 15-6-17.
//  Copyright (c) 2015年 ZH-jl. All rights reserved.
//

#import <Foundation/Foundation.h>

/*
 *  注意:
 *      每次下载完，则回调通知。
 *      通知名：@"HTTP_JSON";
 *      参数名：NSString类型，文件的名字;
 */

/*
 *  注意:
 *      下载失败，则回调通知。
 *      通知名：@"HTTP_FAIL";
 *      参数名：NSString类型，文件的名字;
 */

@interface DataRequest : NSObject


/**
 *  (单例方法)从设备里下载文件
 *
 *  @param profile 文件名
 */
+(void)httpDown:(NSString*)profile;

/**
 *  (实例方法)从设备里下载文件
 *
 *  @param profile 文件名
 */
-(void)subHttpDown:(NSString*)profile;


@end
