//
//  QMKalmanFilter.hpp
//  QMKalmanFilter
//
//  Created by Weiming Wu on 2018/7/24.
//  Copyright © 2018 Weiming Wu. All rights reserved.
//

#ifndef QMKalmanFilter_hpp
#define QMKalmanFilter_hpp

#include <iostream>
#include "QMMatrixObject.hpp"
#include "QMLocationPoint.hpp"

// 使用方法：
// QMLocationPoint *startPoint = new QMLocation(latitude, longitude, timestamp(单位秒，用来前后两个点算速度，所以只要相对于同一个时间系就行了，我用的是since1970), altitude);
// 如果没有高度，altitude可以都设为0
//
// QMKalmanFilter *filter = new QMKalmanFilter(startPoint);
// while(正在跑步)
// {
//      //下面这个是跑步中每次获取到的点
//      QMLocationPoint *runningPoint = new QMLocation(latitude, longitude, timestamp, altitude);
//      //下面这个是优化后的点
//      QMLocationPoint *OptimizedRunningPoint = filter->processState(runningPoint);
// }



//参数：主要是private里面的sigma，现在感觉不用修改，具体怎么改我也不太清楚，随便试出来的。

class QMKalmanFilter
{
public:
    QMKalmanFilter();
    ~QMKalmanFilter();
    QMKalmanFilter(QMLocationPoint *location);
    void resetKalman(QMLocationPoint *location);
    QMLocationPoint* processState(QMLocationPoint *currentLocation);
    
private:
    int stateMDimension = 6;
    int stateNDimension = 1;
    double sigma = 0.0625;
    double rValue = 29.0;
    QMMatrixObject *xk1;
    QMMatrixObject *Pk1;
    QMMatrixObject *A;
    QMMatrixObject *Qt;
    QMMatrixObject *R;
    QMMatrixObject *zt;
    QMLocationPoint *previousLocation;
    double previousMeasureTime;
    QMLocationPoint* kalmanFilter();
    void initKalman(QMLocationPoint *location);
    
};

#endif /* QMKalmanFilter_hpp */
