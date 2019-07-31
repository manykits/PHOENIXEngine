//
//  FrimewareUpdate.h
//  DVRunning
//
//  Created by 陈冠杰 on 16/3/23.
//  Copyright © 2016年 DevinVon. All rights reserved.
//

#import <Foundation/Foundation.h>




@protocol FrimewareUpdateDelegate <NSObject>

@optional

/**
 *  固件下载百分比回调
 *
 *  @param progress 已下载了的
 */
-(void)downloadProgress:(NSString *)progress;

@end


@interface FrimewareUpdate : NSObject
@property (nonatomic,assign) id<FrimewareUpdateDelegate> delegate;
/**
 *  设置固件下载百分比回调Delegate
 *
 *  @param object view.self
 */
+(void)setFrimewareDelegate:(id)object;


/**
 *  检测版本
 *  发出的通知：@"MATCHVERSION"
 *   with object：（NSString）
 *  {
 *      @"match"  可匹配版本
 *      @"mustUpApp" 必须升级APP
 *      @"mustUpFrimware" 必须升级固件
 *      @"updateFirmware" 提示有可升级固件
 *      @"updateApp" 提示有可升级APP
 *
 *  }
 */
+(void)getTheJsonFromMachime;


/**
 *  进行本地校对升级
 *  (此操作是基于已下载好所需文件（固件版本文件和服务器文件）进行比较升级，
 *   执行后会直接下载固件，下载完毕后的回调还是和  DFTransfer.h 中的通知名一致：@"FTP_W_DL_PKG";)
 *  发出通知：@"SHOULDUPDATE"
 *  with Object：（NSString*）
 *  {
 *       @"APP"  APP有更新
 *       @"FIRMWARE" 升级固件（直接下载不再询问！！）
 *       @"NOTHING" 没有更新
 *  }
 */
+(void)compareUpdate;


/**
 *  本地已知文件校对
 *
 *  @return nsstring:NOTHING->无更新  APP->APP需要更新  FIRMWARE->固件需要更新 nil ->本地无校对文件
 */
+(NSString *)checkLocalVersion;


#pragma mark -->> 获取小机固件适配信息
/**
 *  获取小机的可兼容性固件版本，下载完后返回文件存放path；
 *  返回通知：MACHIMEVERSION
 *  对象:path
 */
+(void)getVersion;

#pragma mark -->> 获取服务器版本信息
/**
 *  获取对应产品类型在服务器的版本号信息
 *  返回通知：SERVICEVERSION 
 *  对象：path
 *  @param type 产品类型
 */
+(void)getServiceVersion:(NSString *)type;

/**
 *  新固件的更新信息
 *  @param product 对应产品类型
 *  @param version 对应最新版本号
 *  返回通知：UPDATENOTES
 *  对象：path
 *
 */
+(void)getMachineUpdateNotes:(NSString *)product version:(NSString *)version;


/**
 *  获取服务器上APP版本对应的更新信息
 *
 *  @param product 对应产品类型
 *  @param version 对应最新版本号
 *  返回通知：UPDATENOTES
 *  对象：path
 */

+(void)getAPPUpdateNotes:(NSString *)product version:(NSString *)version;


@end
