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

static const NSString *kGetNewLocationPointNotification = @"GetNewLocationPointNotification";


@interface QMRunningLocationManager : NSObject <CLLocationManagerDelegate, UNUserNotificationCenterDelegate>

@property (strong, nonatomic) CLLocationManager *locationManager;
@property (strong, nonatomic, readonly) CMMotionActivityManager *motionActivityManager;

@property (strong, nonatomic) NSMutableArray<CLLocation *> *realLocations;
@property (strong, nonatomic) NSMutableArray<CLLocation *> *optimizedLocations;

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
 为跑步电台的功能进行配置，这里不要改，可以增加不同的配置方法
 */
-(void) configForGPSRunningTracker;


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

-(CLLocation *)   getCurrentRealLocation;

-(NSDate *)       getCurrentTimestamp;

-(CLLocation *)   getPreviousRealLocation;

-(NSDate *)       getPreviousTimestamp;

-(CLLocation *)   getCurrentOptimizedLocation;

-(CLLocation *)   getPreviousOptimizedLocation;

@end
