//
//  QMRunningLocationConfiguration.m
//  VCRunningLocationLogger
//
//  Created by Weiming Wu on 2018/7/26.
//  Copyright © 2018 Weiming Wu. All rights reserved.
//

#import "QMRunningLocationConfiguration.h"
#import "QMRunningLocationConfiguration.h"

@interface QMRunningLocationConfiguration()

@end

@implementation QMRunningLocationConfiguration

+(instancetype) defaultConfiguration
{
    QMRunningLocationConfiguration *config = [[QMRunningLocationConfiguration alloc]
                                              initWithIsAlwaysAuthorization:YES
                                              andDesiredAccuracy:kCLLocationAccuracyBestForNavigation
                                              andDistanceFilter:kCLDistanceFilterNone
                                              andIsAllowBackground:YES
                                              andActivityType:CLActivityTypeFitness
                                              andIsPauseAutomatically:NO
                                              andAllowedHorizontalAccuracy:55
                                              andAllowedZoneDistanceRatio:0.8
                                              andAllowedVerticalAccuracy:40
                                              andAllowedMaxSpeed:100
                                              andAllowedMaxSpeed:60];
    return config;
}

-(instancetype) initWithIsAlwaysAuthorization: (BOOL)isAlwaysAuthorization
                           andDesiredAccuracy: (double) desiredAccuracy
                            andDistanceFilter: (double) distanceFilter
                         andIsAllowBackground: (BOOL)isAllowBackground
                              andActivityType: (CLActivityType) activityType
                      andIsPauseAutomatically: (BOOL) isPauseAutomatically
                 andAllowedHorizontalAccuracy: (double) allowedHorizontalAccuracy
                  andAllowedZoneDistanceRatio: (double) allowedZoneDistanceRatio
                   andAllowedVerticalAccuracy: (double) allowedVerticalAccuracy
                           andAllowedMaxSpeed: (double) allowedMaxGPSSpeed
                           andAllowedMaxSpeed: (double) allowedMaxCalculatedSpeed
{
    if(self == [super init])
    {
        self.isAlwaysAuthorization = isAlwaysAuthorization;
        self.desiredAccuracy       = desiredAccuracy;
        self.distanceFilter        = distanceFilter;
        self.isAlwaysAuthorization = isAlwaysAuthorization;
        self.activityType          = activityType;
        self.isPausesLocationUpdatesAutomatically = isPauseAutomatically;
        self.allowedHorizontalAccuracy = allowedHorizontalAccuracy;
        self.allowedZoneDistanceRatio  = allowedZoneDistanceRatio;
        self.allowedMaxGPSSpeed        = allowedMaxGPSSpeed;
        self.allowedVerticalAccuracy   = allowedVerticalAccuracy;
        self.allowedMaxCalculatedSpeed = allowedMaxCalculatedSpeed;
        
    }
    else
    {
        assert(0);
    }
    return self;
}

@end
