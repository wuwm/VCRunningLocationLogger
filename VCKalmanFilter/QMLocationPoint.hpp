//
//  QMLocationPoint.hpp
//  QMKalmanFilter
//
//  Created by Weiming Wu on 2018/7/24.
//  Copyright © 2018 Weiming Wu. All rights reserved.
//

#ifndef QMLocationPoint_hpp
#define QMLocationPoint_hpp

#include <iostream>

class QMLocationPoint
{
public:
    double latitude;
    double longitude;
    double timestamp;
    double altitude;
    QMLocationPoint();
    ~QMLocationPoint();
    QMLocationPoint(double latitude, double longitude, double timestamp,double altitude);
    void print();
};

#endif /* QMLocationPoint_hpp */
