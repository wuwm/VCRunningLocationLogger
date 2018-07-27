//
//  RunningPoints.m
//  VCRunningLocationLogger
//
//  Created by Weiming Wu on 2018/7/23.
//  Copyright © 2018 Weiming Wu. All rights reserved.
//

#import "QMRunningPoints.h"

@implementation QMRunningPoints

-(instancetype) initWithLatitude: (CLLocationDegrees) latitude andLongitude: (CLLocationDegrees) longitude
{
    _coordinate.latitude = latitude;
    _coordinate.longitude = longitude;
    return self;
}

@end
