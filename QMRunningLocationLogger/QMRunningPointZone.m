//
//  QMRunningPointZone.m
//  VCRunningLocationLogger
//
//  Created by Weiming Wu on 2018/7/26.
//  Copyright © 2018 Weiming Wu. All rights reserved.
//

#import "QMRunningPointZone.h"
#import <CoreLocation/CoreLocation.h>

@interface QMRunningPointZone()

@property (strong, nonatomic) NSMutableArray<CLLocation *> *pointsInZone;
@property (assign, nonatomic) CLLocationDistance avgLat;
@property (assign, nonatomic) CLLocationDistance avgLon;
@property (assign, nonatomic) CLLocationDistance avgAlti;
@property (assign, nonatomic) NSUInteger maxZoneSize;
@property (assign, nonatomic) NSUInteger outOfZoneCount;
@property (assign, nonatomic) double zoneRatio;

@end

@implementation QMRunningPointZone

-(instancetype)init
{
    if (self == [super init])
    {
        self.pointsInZone = [[NSMutableArray alloc] init];
        self.avgLat       = 0;
        self.avgLon       = 0;
        self.maxZoneSize  = 20;
        self.zoneRatio    = 1.0;
        self.outOfZoneCount = 0;
    }
    return self;
}

-(instancetype)initWithZoneSize: (NSUInteger)zoneSize andZoneRatio: (double)zoneRatio
{
    if (self == [super init])
    {
        self.pointsInZone = [[NSMutableArray alloc] init];
        self.avgLat       = 0;
        self.avgLon       = 0;
        self.maxZoneSize  = zoneSize;
        self.zoneRatio    = zoneRatio;
        self.outOfZoneCount = 0;
    }
    return self;
}

-(instancetype)initWithFirstPoint:(CLLocation *)firstPoint
{
    if (self == [super init])
    {
        self.pointsInZone  = [[NSMutableArray alloc] init];
        [self.pointsInZone addObject:firstPoint];
        self.avgLat = firstPoint.coordinate.latitude;
        self.avgLon = firstPoint.coordinate.longitude;
    }
    return self;
}

-(void)addPoint:(CLLocation *)newPoint
{
    if([self shouldInZone:newPoint])
    {
        if(self.pointsInZone.count > self.maxZoneSize)
        {
            [self.pointsInZone removeObjectAtIndex: 0];
        }
        self.avgLat = (newPoint.coordinate.latitude + self.avgLat * self.pointsInZone.count) / (self.pointsInZone.count + 1);
        self.avgLon = (newPoint.coordinate.latitude + self.avgLon * self.pointsInZone.count) / (self.pointsInZone.count + 1);
        [self.pointsInZone addObject:newPoint];
    }
    else{
        NSLog(@"Should not in Zone.");
    }
}

-(BOOL)shouldInZone:(CLLocation *)newPoint
{
    if(self.pointsInZone.count == 0)
    {
        return YES;
    }
    if([self getHorizentalDistanceBetween:newPoint and: [self getAverageLocation]] < newPoint.horizontalAccuracy * self.zoneRatio)
    {
        return YES;
    }
    else
    {
        self.outOfZoneCount += 1;
        if(self.outOfZoneCount > 3)
        {
            return NO;
        }
        else
        {
            return YES;
        }
    }
}

-(CLLocation *)getAverageLocation
{
    if(self.pointsInZone.count < 1)
    {
        assert(0);
        CLLocation *newLocation = [[CLLocation alloc] initWithLatitude:0
                                                             longitude:0];
        return newLocation;
    }
    else
    {
        CLLocation *newLocation = [[CLLocation alloc] initWithLatitude:self.avgLat
                                                             longitude:self.avgLon];
        return newLocation;
    }
}

-(CLLocationDistance) getHorizentalDistanceBetween: (CLLocation *) p1 and: (CLLocation *) p2
{
    CLLocation *p11 = [[CLLocation alloc] initWithLatitude:p1.coordinate.latitude longitude:p1.coordinate.longitude];
    CLLocation *p22 = [[CLLocation alloc] initWithLatitude:p2.coordinate.latitude longitude:p2.coordinate.longitude];
    return [p11 distanceFromLocation:p22];
}

-(void) clearPointsInZoneArray
{
    self.pointsInZone = [[NSMutableArray alloc] init];
}

@end
