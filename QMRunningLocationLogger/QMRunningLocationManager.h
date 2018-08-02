//
//  VCLocationManager.h
//  VCRunningLocationLogger
//
//  Created by Weiming Wu on 2018/7/23.
//  Copyright © 2018 Weiming Wu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>
#import <CoreMotion/CoreMotion.h>
#import <UserNotifications/UserNotifications.h>
#import "QMKalmanFilter.hpp"
@class QMRunningLocationConfiguration;

//这个是每次位置更新时发出的通知的名称
static const NSString *kGetNewLocationPointNotification = @"GetNewLocationPointNotification";

@interface QMRunningLocationManager : NSObject <CLLocationManagerDelegate, UNUserNotificationCenterDelegate>

/**
 locationManager是系统的 CLLocationManager 的对象。
 */
@property (strong, nonatomic) CLLocationManager *locationManager;

/**
 运动传感器目前暂未用到。
 */
@property (strong, nonatomic, readonly) CMMotionActivityManager *motionActivityManager;

/**
 下面两个数组存储未经滤波的位置和经过滤波之后的位置
 */
@property (strong, nonatomic) NSMutableArray<CLLocation *> *realLocations;
@property (strong, nonatomic) NSMutableArray<CLLocation *> *optimizedLocations;

/**
 开始跑步的类方法，需要先导入config之后运行，否则会有奇怪的错误
 */
+ (void)startRunning;

/**
 暂停跑步的类方法，需要先导入config之后运行，否则会有奇怪的错误
 */
+ (void)pauseRunning;

/**
 停止跑步的类方法，相对于暂停，主要是对已经积累的数据进行归零操作。需要先导入config之后运行，否则会有奇怪的错误
 */
+ (void)stopRunning;

/**
 单例模式

 @return 返回LocationManager的单例对象
 */
+(QMRunningLocationManager *) sharedInstance;

/**
 初始化LocationManager， 生成单例之后必须要执行一次。
 */
-(void) configDefaultCLLocationManager;

/**
 使用参数设置manager

 @param configuration 配置参数，主要是GPS和Kalman算法相关的参数
 */
-(void) configWithConfiguration: (QMRunningLocationConfiguration *) configuration;

/**
 开始更新地理位置，每次更新会发送NSNotification
 */
-(void) startUpdatingLocation;


/**
 停止更新地理位置
 */
-(void) stopUpdatingLocation;

/**
 获取经过Kalman滤波之后的距离

 @return double类型的距离
 */
-(CLLocationDistance) getOptimizedDistance;

/**
 获取未经过Kalman滤波的距离

 @return double类型的距离
 */
-(CLLocationDistance) getRealDistance;

/**
 获取最后一次获取到的未经过滤波的位置

 @return 位置
 */
-(CLLocation *)   getCurrentRealLocation;

/**
 获取最后一次位置的时间戳

 @return 时间戳
 */
-(NSDate *)       getCurrentTimestamp;

/**
 获取倒数第二次获取到的未经过滤波的位置

 @return 位置
 */
-(CLLocation *)   getPreviousRealLocation;

/**
 获取倒数第二次获取到的位置的时间戳

 @return 时间戳
 */
-(NSDate *)       getPreviousTimestamp;

/**
 获取最后一次经过滤波的位置

 @return 最后一次经过滤波的位置
 */
-(CLLocation *)   getCurrentOptimizedLocation;

/**
 获取倒数第二次经过滤波之后的位置

 @return 倒数第二次经过滤波之后的位置
 */
-(CLLocation *)   getPreviousOptimizedLocation;

@end
