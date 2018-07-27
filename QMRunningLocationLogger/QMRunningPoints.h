//
//  RunningPoints.h
//  VCRunningLocationLogger
//
//  Created by Weiming Wu on 2018/7/23.
//  Copyright © 2018 Weiming Wu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <MapKit/MapKit.h>

@interface QMRunningPoints : NSObject <MKAnnotation>

@property(nonatomic,assign) CLLocationCoordinate2D coordinate;
@property(nonatomic,copy) NSString *title;
@property(nonatomic,copy) NSString *subtitle;

-(instancetype) initWithLatitude: (CLLocationDegrees) latitude andLongitude: (CLLocationDegrees) longitude;

@end
