//
//  ViewController.m
//  VCRunningLocationLogger
//
//  Created by Weiming Wu on 2018/7/23.
//  Copyright © 2018 Weiming Wu. All rights reserved.
//

#import "ViewController.h"
#import "QMRunningLocationManager.h"
#import "QMRunningPoints.h"
#import "QMRunningLocationConfiguration.h"

@interface ViewController ()
@property (weak, nonatomic) IBOutlet MKMapView *mapView;
@property (strong, nonatomic) QMRunningLocationManager *locationManager;
@property (weak, nonatomic) IBOutlet UITextView *logView;
@property (assign, nonatomic) unsigned long count;
@property (strong, nonatomic) MKPolyline *routeLine;
@property (strong, nonatomic) MKPolylineRenderer *lineView;
@property (strong, nonatomic) MKPolyline *routeLine2;
@property (weak, nonatomic) IBOutlet UITextField *preciousTextField;
@property (strong, nonatomic) MKPolylineRenderer *lineView2;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    _mapView.delegate = self;
    MKPointAnnotation *point = [[MKPointAnnotation alloc] init];
    point.coordinate = CLLocationCoordinate2DMake(51.49795, -0.174056);
//    [_mapView addAnnotation:point];
//    runningPoints.title = @"111";
    _locationManager = [QMRunningLocationManager sharedInstance];
    [_locationManager configDefaultCLLocationManager];
//    [_locationManager configWithConfiguration:[QMRunningLocationConfiguration defaultConfiguration]];
    _mapView.showsUserLocation = YES;
    _count = 0;
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(didUpdateLocation:)
                                                 name:@"GetNewLocationPointNotification"
                                               object:nil];
    // Do any additional setup after loading the view, typically from a nib.
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma Map Annotation related

-(void) addAnnotationWithLatitude: (double) latitude andLongitude: (double) longitude
{
    MKPointAnnotation *point = [[MKPointAnnotation alloc] init];
    point.coordinate = CLLocationCoordinate2DMake(latitude, longitude);
    [_mapView addAnnotation:point];
}

-(void) addAnnotationWithLatitude: (double)latitude andLongitude: (double)longitude andTitle: (NSString *)title
{
    MKPointAnnotation *point = [[MKPointAnnotation alloc] init];
    point.coordinate = CLLocationCoordinate2DMake(latitude, longitude);
    point.title = title;
    [_mapView addAnnotation:point];
}

#pragma MapKit related

//-(void) drawLineWithPoint1: (CLLocationCoordinate2D) p1 andPoint2: (CLLocationCoordinate2D) p2
//{
//    MKPolyline *routeLine;
//    CLLocationCoordinate2D points[2];
//    points[0] = p1;
//    points[1] = p2;
//    routeLine = [MKPolyline polylineWithCoordinates: points count:2];
//    [self.mapView setVisibleMapRect:[routeLine boundingMapRect]];
//    [self.mapView addOverlay:routeLine];
//    MKPolylineView *lineView = [[MKPolylineView alloc]initWithPolyline:routeLine];
//    lineView.strokeColor = [UIColor redColor];
//    lineView.lineWidth = 5;
//}

-(void) drawPolyLine: (NSArray<CLLocation *> *)locations
{
    [_mapView removeOverlays:_mapView.overlays];
//    CLLocationCoordinate2D points[locations.count];
    CLLocationCoordinate2D *points = (CLLocationCoordinate2D *)malloc(locations.count * sizeof(CLLocationCoordinate2D));
    for(int i = 0; i < locations.count; ++i)
    {
        points[i] = locations[i].coordinate;
    }
    self.routeLine = [MKPolyline polylineWithCoordinates: points count:locations.count];
    [self.mapView addOverlay:_routeLine];
    self.lineView = [[MKPolylineRenderer alloc] initWithPolyline:self.routeLine];
    self.lineView.strokeColor = [UIColor redColor];
    self.lineView.lineWidth = 2;
}

-(void) drawPolyLine2: (NSArray<CLLocation *> *)locations
{
    [_mapView removeOverlays:_mapView.overlays];
    //    CLLocationCoordinate2D points[locations.count];
    CLLocationCoordinate2D *points = (CLLocationCoordinate2D *)malloc(locations.count * sizeof(CLLocationCoordinate2D));
    for(int i = 0; i < locations.count; ++i)
    {
        points[i] = locations[i].coordinate;
    }
    self.routeLine2 = [MKPolyline polylineWithCoordinates: points count:locations.count];
    [self.mapView addOverlay:self.routeLine2];
    self.lineView2 = [[MKPolylineRenderer alloc] initWithPolyline:self.routeLine2];
    self.lineView2.strokeColor = [UIColor greenColor];
    self.lineView2.lineWidth = 1;
}

- (MKOverlayRenderer *)mapView:(MKMapView *)mapView rendererForOverlay:(id<MKOverlay>)overlay
{
    if(overlay == self.routeLine)
    {
        return self.lineView;
    }
    else
    {
        return self.lineView2;
    }
    
}

#pragma NSNotification

-(void) didUpdateLocation: (NSNotification *)notification
{
    QMRunningLocationManager *locationManager = (QMRunningLocationManager *) notification.object;
    _count += 1;
    CLLocation *currentLocation = [locationManager getCurrentRealLocation];
    NSTimeInterval currentTimestamp = [[locationManager getCurrentTimestamp] timeIntervalSince1970];
    CLLocation *currentOptimizedLocation = [locationManager getCurrentOptimizedLocation];
    
    [self addAnnotationWithLatitude: currentLocation.coordinate.latitude
                       andLongitude: currentLocation.coordinate.longitude
                           andTitle: [NSString stringWithFormat:@"%lu", (unsigned long)_count]];
    
    self.logView.text = [self.logView.text stringByAppendingString:
                         [NSString stringWithFormat:@"%lu. Latitude: %f, Longitude: %f, AccH: %f, AccV: %f， Speed: %f, Distance: %f \n\n",
                          self.count,
                          currentOptimizedLocation.coordinate.latitude,
                          currentOptimizedLocation.coordinate.longitude,
                          currentOptimizedLocation.horizontalAccuracy,
                          currentOptimizedLocation.verticalAccuracy,
                          currentOptimizedLocation.speed,
                          [locationManager getOptimizedDistance]]];
    if(_count > 1)
    {
//        [self drawPolyLine:locationManager.realLocations];
        [self drawPolyLine2:locationManager.optimizedLocations];
    }

}

#pragma Button events

- (IBAction)onClickedAddAnnotationBtn:(UIButton *)sender {
    [self addAnnotationWithLatitude:30 andLongitude:30];
}
- (IBAction)onClickedStart:(UIButton *)sender {
    kPrecise =  [self.preciousTextField.text doubleValue];
    [_locationManager startUpdatingLocation];
}
- (IBAction)onClickedEnd:(UIButton *)sender {
    [_locationManager stopUpdatingLocation];
}
- (IBAction)onClickedClear:(UIButton *)sender {
    [_mapView removeAnnotations:_mapView.annotations];
}
@end
