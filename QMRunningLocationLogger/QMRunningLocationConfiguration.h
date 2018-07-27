//
//  QMRunningLocationConfiguration.h
//  VCRunningLocationLogger
//
//  Created by Weiming Wu on 2018/7/26.
//  Copyright © 2018 Weiming Wu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>

@interface QMRunningLocationConfiguration : NSObject

@property (assign, nonatomic) BOOL   isAlwaysAuthorization;
@property (assign, nonatomic) double desiredAccuracy;
@property (assign, nonatomic) double distanceFilter;
@property (assign, nonatomic) BOOL   isAllowBackgroundLocaitonUpdates;
@property (assign, nonatomic) CLActivityType activityType;
@property (assign, nonatomic) BOOL   isPausesLocationUpdatesAutomatically;
@property (assign, nonatomic) double allowedHorizontalAccuracy;
@property (assign, nonatomic) double allowedZoneDistanceRatio;

+(instancetype) defaultConfiguration;

-(instancetype) initWithIsAlwaysAuthorization: (BOOL)isAlwaysAuthorization
                           andDesiredAccuracy: (double) desiredAccuracy
                            andDistanceFilter: (double) distanceFilter
                         andIsAllowBackground: (BOOL)isAllowBackground
                              andActivityType: (CLActivityType) activityType
                      andIsPauseAutomatically: (BOOL) isPauseAutomatically
                 andAllowedHorizontalAccuracy: (double) allowedHorizontalAccuracy
                  andAllowedZoneDistanceRatio: (double) allowedZoneDistanceRatio;

@end
