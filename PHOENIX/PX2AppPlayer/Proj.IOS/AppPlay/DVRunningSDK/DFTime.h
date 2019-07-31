//
//  DFTime.h
//  
//
//  Created by Mac on 15/12/10.
//
//

#import <Foundation/Foundation.h>


typedef void(^Action)();

@interface DFTime : NSObject


/**
 *  延时执行代码block。
 *  @param      sec     延时时间
 *  @param      block   代码行
 */
+(void)delaySec:(double)sec perform:(Action)block;


/**
 *  获取本地当前时间。
 *  @return @"2016" @"2" @"18" @"11" @"26" @"00"
 */
+(NSArray*)dateMembers;


/**
 *  获取本地当前时间。
 *  @return @"20160218112600"
 */
+ (NSString*)dateString;

/**
 *  分解时间成员，@"20160218112600" => @"2016" @"2" @"18" @"11" @"26" @"00"
 *  @param time     @"20160218112600"
 *  @return @"2016" @"2" @"18" @"11" @"26" @"00"
 */
+ (NSArray*)divideDateMembers:(NSString*)time;


/**
 *  时间值加“秒”值。
 *  @param   sec     秒数
 *  @param   date    @"20160218112600"
 */
+ (NSString*)addSec:(NSTimeInterval)sec OnDate:(NSString *)date;

/**
 *  求两个时间点差值。(timeB-timeA)
 *  @param   timeA    @"20160218112600"
 *  @param   timeB    @"20160218112601"
 */
+(long)differenceOfDateA:(NSString*)timeA AndDateB:(NSString*)timeB;



@end
