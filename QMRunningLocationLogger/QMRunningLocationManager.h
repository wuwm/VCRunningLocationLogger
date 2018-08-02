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

//This is the name of NSNotification when get a new updated location. You can config it.
static const NSString *kGetNewLocationPointNotification = @"GetNewLocationPointNotification";

@interface QMRunningLocationManager : NSObject <CLLocationManagerDelegate, UNUserNotificationCenterDelegate>

/**
 locationManager is the instance of CLLocationManager.
 */
@property (strong, nonatomic) CLLocationManager *locationManager;

/**
 Not used in this version.
 */
@property (strong, nonatomic, readonly) CMMotionActivityManager *motionActivityManager;

/**
 The following two arrays stores the unfiltered and filtered locations.
 */
@property (strong, nonatomic) NSMutableArray<CLLocation *> *realLocations;
@property (strong, nonatomic) NSMutableArray<CLLocation *> *optimizedLocations;

/**
 Invoke when start to run. You must use the config function first.
 */
+ (void)startRunning;

/**
 Invoke when pause.
 */
+ (void)pauseRunning;

/**
 Invoke when stop running. You must use the config function first.
 */
+ (void)stopRunning;

/**
 Singleton.

 @return The singleton instance of QMRunningLocationManager.
 */
+(QMRunningLocationManager *) sharedInstance;

/**
 Config our location manager with default parameter. You must choose from 2 following config functions before start to run.
 */
-(void) configDefaultCLLocationManager;

/**
 Config our location manager with customized parameters.

 @param configuration Config instance. Mainly GPS and Kalman filter related.
 */
-(void) configWithConfiguration: (QMRunningLocationConfiguration *) configuration;

/**
 Start to update location, will send NSNotification when getting new location.
 */
-(void) startUpdatingLocation;


/**
 Stop updating location.
 */
-(void) stopUpdatingLocation;

/**
 Filtered distance.

 @return Filtered distance.
 */
-(CLLocationDistance) getOptimizedDistance;

/**
 Unfiltered distance.

 @return Unfiltered distance.
 */
-(CLLocationDistance) getRealDistance;

/**
 Get last known unfiltered location.

 @return last known unfiltered location
 */
-(CLLocation *)   getCurrentRealLocation;

/**
 Get last known timestamp;

 @return last known timestamp
 */
-(NSDate *)       getCurrentTimestamp;

-(CLLocation *)   getPreviousRealLocation;

-(NSDate *)       getPreviousTimestamp;

-(CLLocation *)   getCurrentOptimizedLocation;

-(CLLocation *)   getPreviousOptimizedLocation;

@end
