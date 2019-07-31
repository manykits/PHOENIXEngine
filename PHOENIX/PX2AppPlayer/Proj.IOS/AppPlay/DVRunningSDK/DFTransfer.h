//
//  DFTransfer.h
//  TalkToToy
//
//  Created by Mac on 15/10/16.
//  Copyright (c) 2015年 JL. All rights reserved.
//

#import <Foundation/Foundation.h>



@interface DFTransfer : NSObject

#pragma mark --> 获取设备文件目录
/**
*   通知名：@"FTP_OPEN_FILE";
*   参数名：NSArray类型，所有文件的信息;
*/
+(void)openFiles:(NSString*)filePath;

#pragma mark --> 从设备中下载媒体文件

/**
 批量下载
 通知名：@"FTP_DL_FILE";
 @param array 已下载的文件
 @param sourePath 文件所在的文件夹名称（DCIMA/DCIMB）
 */
+(void)downloadOjects:(NSArray *)array Path:(NSString *)sourePath;

#pragma mark --> 取消下载媒体文件
/**
 *  取消下载媒体文件
 */
+(void)stopMission;

#pragma mark --> 从设备中下载预览图


/**
 从设备中下载预览图
 通知名：@"FTP_DL_PV";
 @param array 已下载的预览图名称数组
 @param sourePath 文件所在的文件夹名称（DCIMA/DCIMB）
 */
+(void)downloadPreview:(NSArray *)array Path:(NSString *)sourePath;

#pragma mark --> 删除设备和手机本地相对应的预览图和媒体文件
/**
 *  通知名：@"FTP_DEL";
 *  参数名：NSArray类型，已删除的文件;
 *  @param sourePath 文件所在的文件夹名称（DCIMA/DCIMB）
 */
+(void)deleteFile:(NSArray *)array Path:(NSString *)sourePath;



//#pragma mark --> 向服务器请求固件文件列表
///**
// *  通知名：@"FTP_W_UPDATE_LIST";
// *  参数名：NSArray类型，服务器存在的固件列表;
// */
//+(void)updateFiles:(NSString*)folder;

#pragma mark --> 从服务器下载固件
/**
 *  从服务器存下载固件
 *  @param doc 固件目录的路径--》路径：@"/%@/firmware/%@"，产品类型，固件型号
 *  通知名：@"FTP_W_DL_PKG";
 *  参数名：BOOL类型，从服务器存下载固件;
 *  (存储路径:APP中的Library/UPDATE/JL_AC51.bfu)
 */
+(void)downPkgOn:(NSString*)doc;

#pragma mark --> 取消固件下载
/**
 *  取消固件下载
 */
+(void)stopDownPkg;

/*--- 上传固件给设备 ---*/
//+(void)upPkg:(NSString*)path;

#pragma mark --> 上传文件(固件)给设备
/**
 *  通知名：@"FTP_UP_PKG";
 *  参数名：BOOL类型，上传文件(固件)给设备;
 */
+(void)upPkgToPath:(NSString*)tPath FromPath:(NSString*)fPath;

#pragma mark --> 取消上传固件给设备
/**
 *  取消上传固件给设备
 */
-(void)stopUpPkg;



#pragma mark --> 更改小机SD卡上的文件名字
/**
 *  更改ftp上的文件名字
 *
 *  @param oldName 旧名字
 *  @param newName 新名字
 *  @param sourePath 文件所在的文件夹名称（DCIMA/DCIMB）
 */
+(void)renameFile:(NSString *)oldName newName:(NSString*)newName Path:(NSString *)sourePath;



@end
