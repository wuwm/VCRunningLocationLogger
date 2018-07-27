//
//  QMRunningPointZone.h
//  VCRunningLocationLogger
//
//  Created by Weiming Wu on 2018/7/26.
//  Copyright © 2018 Weiming Wu. All rights reserved.
//

#import <Foundation/Foundation.h>
@class CLLocation;

@interface QMRunningPointZone : NSObject

-(CLLocation *) getAverageLocation;

-(void) addPoint: (CLLocation *) newPoint;

-(BOOL) shouldInZone: (CLLocation *) newPoint;

-(instancetype) initWithFirstPoint: (CLLocation *) firstPoint;

-(void) clearPointsInZoneArray;

@end
