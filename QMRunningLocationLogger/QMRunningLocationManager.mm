//
//  VCLocationManager.m
//  VCRunningLocationLogger
//
//  Created by Weiming Wu on 2018/7/23.
//  Copyright © 2018 Weiming Wu. All rights reserved.
//

#import "QMRunningLocationManager.h"
#import "QMKalmanFilter.hpp"
#import "QMRunningPointZone.h"
#import "QMRunningLocationConfiguration.h"

@interface QMRunningLocationManager()

@property (strong, nonatomic) QMRunningLocationConfiguration *configuration;

/**
 C++中的 kalmanFilter指针
 */
@property (assign, nonatomic, nonnull) QMKalmanFilter *kalmanFilter;

@property (assign, nonatomic) CLLocationDistance realDistance;
@property (assign, nonatomic) CLLocationDistance optimizedDistance;

@property (strong, nonatomic) QMRunningPointZone *zone;

@end


@implementation QMRunningLocationManager


+(QMRunningLocationManager *) sharedInstance
{
    static QMRunningLocationManager *_instance = nil;
    @synchronized(self)
    {
        if(_instance == nil)
        {
            _instance = [[self alloc] init];
        }
    }
    return _instance;
}

-(instancetype) init
{
    if(self == [super init])
    {
        
    }
    else
    {
        assert(0);
    }
    return self;
}

-(void) configDefaultCLLocationManager
{
    self.locationManager = [[CLLocationManager alloc] init];
    [self.locationManager requestAlwaysAuthorization];
    self.locationManager.delegate = self;
    self.locationManager.desiredAccuracy = kCLLocationAccuracyBestForNavigation;
    self.locationManager.distanceFilter  = 5.0;
    self.locationManager.allowsBackgroundLocationUpdates = YES;
    self.locationManager.activityType = CLActivityTypeFitness;
    self.locationManager.pausesLocationUpdatesAutomatically = NO;
    self.realLocations      = [[NSMutableArray<CLLocation *> alloc] init];
    self.optimizedLocations = [[NSMutableArray<CLLocation *> alloc] init];
    self.realDistance       = 0.0;
    self.optimizedDistance  = 0.0;
    self.zone = [[QMRunningPointZone alloc] init];
}

-(void)configWithConfiguration:(QMRunningLocationConfiguration *)configuration
{
    self.configuration = configuration;
    self.locationManager = [[CLLocationManager alloc] init];
    self.locationManager.delegate = self;
    self.realLocations      = [[NSMutableArray<CLLocation *> alloc] init];
    self.optimizedLocations = [[NSMutableArray<CLLocation *> alloc] init];
    self.zone = [[QMRunningPointZone alloc] init];
    self.realDistance       = 0.0;
    self.optimizedDistance  = 0.0;
    if(configuration.isAlwaysAuthorization)
    {
        [self.locationManager requestAlwaysAuthorization];
    }
    else
    {
        [self.locationManager requestWhenInUseAuthorization];
    }
    self.locationManager.desiredAccuracy = configuration.desiredAccuracy;
    self.locationManager.distanceFilter  = configuration.distanceFilter;
    self.locationManager.activityType    = configuration.activityType;
    self.locationManager.pausesLocationUpdatesAutomatically = configuration.isPausesLocationUpdatesAutomatically;
}


/**
 当跑步开始的时候，调用这个函数。
 */
- (void)startUpdatingLocation
{
    [self.locationManager startUpdatingLocation];
}

/**
 当跑步结束的时候，调用这个函数。
 */
- (void)stopUpdatingLocation
{
    [self.locationManager stopUpdatingLocation];
}


#pragma CLLocation Delegate
/**
 每次更新地理位置之后会调用这个方法，方法调用的时候会给defaultCenter发送一个Notification，对应的ViewController需要Observe这个方法。

 @param manager 系统的LocationManager
 @param locations 返回的坐标数组（一次返回一组坐标）
 */
- (void)locationManager:(CLLocationManager *)manager didUpdateLocations:(NSArray<CLLocation *> *)locations
{
    for(int i = 0; i < locations.count; ++i)
    {
        CLLocation *loc = locations[i];
        if([self isValidLocationPoint:loc])
        {    
            if(self.realLocations.count < 1)
            {//第一个点
                [self processFirstRunningPoint:loc];
            }
            else
            {//之后的点
                [self processRunningPoint:loc];
            }
        }
    }
}


/**
 验证坐标点是否符合要求，只有符合要求的才会被收录。

 @param unfilteredLocation 原始的点
 @return 是否符合要求
 */
-(BOOL) isValidLocationPoint:(CLLocation *)unfilteredLocation
{
    
    if(unfilteredLocation.horizontalAccuracy <= self.configuration.allowedHorizontalAccuracy &&
       unfilteredLocation.verticalAccuracy   <= self.configuration.allowedVerticalAccuracy   &&
       unfilteredLocation.speed              <  self.configuration.allowedMaxGPSSpeed        &&
       unfilteredLocation.speed > 0)
    {
        if(self.realLocations.count == 0)
        {
            return YES;
        }
        else
        {
            double calculatedSpeed = [unfilteredLocation distanceFromLocation: [self.realLocations lastObject]]
                                        /
                                    ([unfilteredLocation.timestamp timeIntervalSince1970] - [[self.realLocations lastObject].timestamp timeIntervalSince1970]);
            if(calculatedSpeed < self.configuration.allowedMaxCalculatedSpeed)
            {
                return YES;
            }
            else
            {
                return NO;
            }
        }
    }
    else
    {
        return NO;
    }
}

/**
 处理第一个获取到的坐标点

 @param runningPoint 第一个获取到的坐标点
 */
-(void) processFirstRunningPoint: (CLLocation *)runningPoint
{
    [self.realLocations       addObject: runningPoint];
    [self.optimizedLocations  addObject: runningPoint];
    [self.zone addPoint:runningPoint];
    QMLocationPoint *startPoint = new QMLocationPoint(runningPoint.coordinate.latitude,
                                                      runningPoint.coordinate.longitude,
                                                      [runningPoint.timestamp timeIntervalSince1970],
                                                      runningPoint.altitude);
    self.kalmanFilter = new QMKalmanFilter(startPoint);
    [[NSNotificationCenter defaultCenter] postNotificationName:@"GetNewLocationPointNotification" object:self];
}


/**
 处理第二、第三、第四……个点

 @param runningPoint 获取到的坐标点
 */
-(void) processRunningPoint: (CLLocation *)runningPoint
{
    if([self.zone shouldInZone:runningPoint])
    {
        [self.zone addPoint:runningPoint];
    }
    else
    {
        [self.zone clearPointsInZoneArray];
        [self.zone addPoint:runningPoint];
        [self.realLocations addObject:runningPoint];
        QMLocationPoint *runningLPoint = new QMLocationPoint(runningPoint.coordinate.latitude,
                                                             runningPoint.coordinate.longitude,
                                                             [runningPoint.timestamp timeIntervalSince1970],
                                                             runningPoint.altitude);
        QMLocationPoint *optimizedPoint = self.kalmanFilter->processState(runningLPoint);
        CLLocation *optimizedLocation = [[CLLocation alloc]
                                         initWithCoordinate:(CLLocationCoordinate2DMake(optimizedPoint->latitude, optimizedPoint->longitude))
                                         altitude:optimizedPoint->altitude
                                         horizontalAccuracy:runningPoint.horizontalAccuracy
                                         verticalAccuracy:runningPoint.verticalAccuracy
                                         course:runningPoint.course
                                         speed:runningPoint.speed
                                         timestamp:runningPoint.timestamp];
        [self.optimizedLocations addObject:optimizedLocation];
        
        self.realDistance      += [[self getCurrentRealLocation]      distanceFromLocation: [self getPreviousRealLocation]];
        self.optimizedDistance += [[self getCurrentOptimizedLocation] distanceFromLocation: [self getPreviousOptimizedLocation]];
        [[NSNotificationCenter defaultCenter] postNotificationName:@"GetNewLocationPointNotification" object:self];
    }
    
}

-(CLLocationDistance)getOptimizedDistance
{
    return self.optimizedDistance;
}

-(CLLocationDistance)getRealDistance
{
    return self.realDistance;
}

-(NSDate *)getCurrentTimestamp
{
    return [self.realLocations lastObject].timestamp;
}

-(CLLocation *)getCurrentRealLocation
{
    return [self.realLocations lastObject];
}

-(CLLocation *)getCurrentOptimizedLocation
{
    return [self.optimizedLocations lastObject];
}

-(NSDate *)getPreviousTimestamp
{
    return [self.realLocations objectAtIndex: self.realLocations.count-2].timestamp;
}

-(CLLocation *)getPreviousRealLocation
{
    return [self.realLocations objectAtIndex: self.realLocations.count-2];
}

-(CLLocation *)getPreviousOptimizedLocation
{
    return [self.optimizedLocations objectAtIndex: self.optimizedLocations.count-2];
}
-(void)dealloc
{
    delete self.kalmanFilter;
}
@end
