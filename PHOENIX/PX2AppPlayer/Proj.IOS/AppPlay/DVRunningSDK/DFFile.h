//
//  DFFile.h
//  DVRunning
//
//  Created by Mac on 15/11/13.
//  Copyright (c) 2015年 DevinVon. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface DFFile : NSObject


/**
 *  用途:【创建】一个文件，在你想要的路径下。
 *  @parm   sPath     系统路径
 *  @parm   mPath     中途路径    -->> @"test1/test2/test3" <<--
 *  @parm   file      文件名
 */
+(NSString*)createOn:(NSSearchPathDirectory)sPath MiddlePath:(NSString*)mPath File:(NSString*)file;


/**
 *  用途:【列举】一个文件，在你想要的路径下。
 *  @parm   sPath     系统路径
 *  @parm   mPath     中途路径    -->> @"test1/test2/test3" <<--
 *  @parm   file      文件名
 */
+(NSString*)listPath:(NSSearchPathDirectory)sPath MiddlePath:(NSString*)mPath File:(NSString*)file;



/**
 *  用途:Byte单位转换成KB,MB,GB
 *  @parm   byte   多少bytes？
 */
+(NSString*)byteSize:(long long)byte;



/**
 *  用途:重命名。
 *  @parm   oldName     旧的文件(路径)    -->> @"test1/test2/test3" <<--
 *  @parm   NewName     新的文件(路径)    -->> @"test1/test2/test3" <<--
 */
+(BOOL)renameOldName:(NSString*)oldName NewName:(NSString*)newName;



/**
 *  用途:删除文件。
 *  @parm   path      文件路径
 */
+(BOOL)removePath:(NSString*)path;


/**
 *  用途:把数据写进文件(覆盖)。
 *  @parm   data      数据字节
 *  @parm   path      文件路径
 */
+(BOOL)writeData:(NSData*)data fillFile:(NSString*)path;


/**
 *  用途:把数据写进文件(文件末尾)。
 *  @parm   data      数据字节
 *  @parm   path      文件路径
 */
+(void)writeData:(NSData*)data endFile:(NSString*)path;


/**
 *  用途:从bundle里寻找文件。
 *  @parm   bundle    @"*.bundle"
 *  @parm   file      文件名
 */
+(NSString*)find:(NSString*)bundle File:(NSString*)file;


/**
 *  用途:从Project资源里寻找文件。
 *  @parm   file      文件名
 *  @return 该文件的路径。
 */
+(NSString*)find:(NSString *)file;


/**
 *  用途:从Project资源里寻找图片文件。
 *  @parm   image     图片名     -->> @"test@2x.png" <<--
 */
+(UIImage*)loadImage:(NSString*)image;


/**
 *  用途:解析JSON文件。
 *  @parm   path      文件路径
 */
+(NSDictionary*)JsonPath:(NSString*)path;


/**
 *  用途:获得AVI视频的PreViewImage。
 *  @parm   path      文件路径
 *  @return @{@"IMG" : 图片Data,
 *            @"SIZE": 宽高,
 *            @"TIME": 时长}
 */
+(NSDictionary*)previewAVI:(NSString*)path;

/**
 *  用途:获得AVI视频的PreViewImage。
 *  @parm   data      文件数据
 *  @return @{@"IMG" : 图片Data,
 *            @"SIZE": 宽高,
 *            @"TIME": 时长}
 */
+(NSDictionary *)previewData:(NSData*)data;


/**
 *  用途:压缩图片。
 *  @parm   srcImage   待压缩的图片
 *  @parm   scale      压缩比
 */
+(NSData*)compressImage:(UIImage *)srcImage Scale:(double)scale;
























#pragma mark    -->> *【以下方法本工程适用】* <<--
/**
 *  用途:是否存在此预览图
 *  @parm   file      文件名
 */
+(BOOL)isExistPreview:(NSString*)file;


/**
 *  用途:是否存在此媒体文件
 *  @parm   file      文件名
 */
+(BOOL)isExistMedia:(NSString*)file;



/**
 *  用途:展示媒体文件。
 * 【适用:1.本工程；2.file后缀为:".AVI",".JPG"】
 *  @parm   file      文件名
 *  @parm   uuid      区分设备
 *  @return path      文件路径
 */
+(NSString*)displayMedia:(NSString*)file;

/**
 *  用途:读取文件路径。
 * 【适用:1.本工程；2.file后缀为:".AVI",".JPG"】
 *  @parm   file      文件名
 *  @parm   uuid      区分设备
 *  @return path      文件路径
 */
+(NSString*)loadMedia:(NSString*)file;

/**
 *  用途:在本地删除预览图和源文件。
 *  @parm   file      文件名
 */
+(void)deletePreviewAndSource:(NSString*)file;


/**
 *  用途:在本地删除源文件。
 *  @parm   file      文件名
 */
+(void)deleteSource:(NSString*)file;


/**
 *  用途:解析FTP(MLSD)数据。
 *  @parm   file      AVI文件名
 *  @return {
 *              NAME = "AVI00015";
 *              TIME = "00:01:05";
 *              DATE = "2015-08-07 15:13:21";
 *              SIZE = 105Mb;
 *          }
 */
+(NSDictionary*)AviInfo:(NSString*)file;

/**
 *  用途：解析FTP（MLSD）数据。
 *  @parm file JPG文件名
 *  @return{
 *         NAME = "AVI00015";
 *         DATE = "2015-08-07 15:13:21";
 *         SIZE = 105Mb;
 *      }
 */
+(NSDictionary *)JPGsInfo:(NSString *)file;


/**
 *  用途:解析FTP(MLSD)数据。
 *  @parm   ftp      字符串数据
 *  @return {
 *              FILE = "JPG00015.JPG";
 *              "UNIX.gid" = 30501;
 *              "UNIX.mode" = 0777;
 *              "UNIX.uid" = 30501;
 *              modify = 20910916194513;
 *              sizd = 62456;
 *              type = file;
 *              unique = 811gc100019;
 *          }
 */
+(NSDictionary *)ftpString:(NSString *)str;

/**
 * 用途：删除所有为下载完成的临时文件。
 *
 */
+(void)deleteAllTMPfile;


/**
 * 用途：列举已存在的AVIs。
 */
+(NSArray*)ExistedAVIs;

/**
 * 用途：列举已存在的JPGs。
 */
+(NSArray*)ExistedJPGs;



/**
 *  用途:找到固件文件路径。
 *  @return     file      路径
 */
+(NSString*)updatePkgPath;










@end
