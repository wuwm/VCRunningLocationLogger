//
//  QMLocationPoint.cpp
//  QMKalmanFilter
//
//  Created by Weiming Wu on 2018/7/24.
//  Copyright © 2018 Weiming Wu. All rights reserved.
//

#include "QMLocationPoint.hpp"

QMLocationPoint::QMLocationPoint()
{
    
}

QMLocationPoint::~QMLocationPoint()
{
    
}

QMLocationPoint::QMLocationPoint(double latitude, double longitude, double timestamp, double altitude)
{
    this->latitude = latitude;
    this->longitude = longitude;
    this->timestamp = timestamp;
    this->altitude = altitude;
}

void QMLocationPoint::print()
{
    printf("%f, %f, %f, %f", latitude, longitude, timestamp, altitude);
}

