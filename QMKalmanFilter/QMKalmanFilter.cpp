//
//  QMKalmanFilter.cpp
//  QMKalmanFilter
//
//  Created by Weiming Wu on 2018/7/24.
//  Copyright © 2018 Weiming Wu. All rights reserved.
//

#include "QMKalmanFilter.hpp"

QMKalmanFilter::QMKalmanFilter()
{
    
}

QMKalmanFilter::~QMKalmanFilter()
{
    delete xk1;
    delete Pk1;
    delete A;
    delete Qt;
    delete R;
    delete zt;
}

QMKalmanFilter::QMKalmanFilter(QMLocationPoint *location)
{
    this->xk1 = new QMMatrixObject(stateMDimension, stateNDimension);
    this->Pk1 = new QMMatrixObject(stateMDimension, stateMDimension);
    this->A = new QMMatrixObject(stateMDimension, stateMDimension);
    this->Qt = new QMMatrixObject(stateMDimension, stateMDimension);
    this->R = new QMMatrixObject(stateMDimension, stateMDimension);
    this->zt = new QMMatrixObject(stateMDimension, stateNDimension);
    
    this->initKalman(location);
}

void QMKalmanFilter::initKalman(QMLocationPoint *location)
{
    this->previousLocation = location;
    this->previousMeasureTime = location->timestamp;
    
    this->xk1->matrix[0][0] = location->latitude;
    this->xk1->matrix[1][0] = 0.0;
    this->xk1->matrix[2][0] = location->longitude;
    this->xk1->matrix[3][0] = 0.0;
    this->xk1->matrix[4][0] = location->altitude;
    this->xk1->matrix[5][0] = 0.0;
    
    for(int i = 0; i < stateMDimension; ++i)
    {
        for(int j = 0; j < stateMDimension; ++j)
        {
            this->Pk1->matrix[i][j] = 0.0;
            if(i == j)
            {
                this->A->matrix[i][j] = 1.0;
                this->R->matrix[i][j] = this->rValue;
            }
            else
            {
                this->A->matrix[i][j] = 0.0;
                this->R->matrix[i][j] = 0.0;
            }
        }
    }
}

void QMKalmanFilter::resetKalman(QMLocationPoint *location)
{
    this->initKalman(location);
}

QMLocationPoint* QMKalmanFilter::processState(QMLocationPoint *currentLocation)
{
    double newMeasureTime = currentLocation->timestamp;
    double lastMeasureTime = this->previousMeasureTime;
    
    double timeInterval = newMeasureTime - lastMeasureTime;
    
    for(int i = 0; i < stateMDimension; ++i)
    {
        for(int j = 0; j < stateMDimension; ++j)
        {
            if(i == j)
            {
                this->A->matrix[i][j] = 1.0;
            }
            else
            {
                this->A->matrix[i][j] = 0.0;
            }
        }
    }
    
    this->A->matrix[0][1] = timeInterval;
    this->A->matrix[2][3] = timeInterval;
    this->A->matrix[4][5] = timeInterval;
    
    double part1 = sigma * (timeInterval * timeInterval * timeInterval * timeInterval) / 4.0;
    double part2 = sigma * (timeInterval * timeInterval * timeInterval) / 2.0;
    double part3 = sigma * (timeInterval * timeInterval);
    
    for(int i = 0; i < stateMDimension; ++i)
    {
        for(int j = 0; j < stateMDimension; ++j)
        {
            this->Qt->matrix[i][j] = 0.0;
        }
    }
    this->Qt->matrix[0][0] = part1;
    this->Qt->matrix[0][1] = part2;
    this->Qt->matrix[1][0] = part2;
    this->Qt->matrix[1][1] = part3;
    this->Qt->matrix[2][2] = part1;
    this->Qt->matrix[2][3] = part2;
    this->Qt->matrix[3][2] = part2;
    this->Qt->matrix[3][3] = part3;
    this->Qt->matrix[4][4] = part1;
    this->Qt->matrix[4][5] = part2;
    this->Qt->matrix[5][4] = part2;
    this->Qt->matrix[5][5] = part3;
    
    double velocityXComponent = (previousLocation->latitude - currentLocation->latitude) / timeInterval;
    double velocityYComponent = (previousLocation->longitude - currentLocation->longitude) / timeInterval;
    double velocityZComponent = (previousLocation->altitude - currentLocation->altitude) / timeInterval;
    
    this->zt->matrix[0][0] = currentLocation->latitude;
    this->zt->matrix[1][0] = velocityXComponent;
    this->zt->matrix[2][0] = currentLocation->longitude;
    this->zt->matrix[3][0] = velocityYComponent;
    this->zt->matrix[4][0] = currentLocation->altitude;
    this->zt->matrix[5][0] = velocityZComponent;
    
    previousMeasureTime = newMeasureTime;
    previousLocation = currentLocation;
    
    return this->kalmanFilter();
}

QMLocationPoint* QMKalmanFilter::kalmanFilter()
{
    std::unique_ptr<QMMatrixObject> xk = this->A->multiply(this->xk1);
    std::unique_ptr<QMMatrixObject> Pk = this->A->multiply(this->Pk1)->multiply(this->A->transpose())->plus(this->Qt);
    std::unique_ptr<QMMatrixObject> temp = Pk->plus(this->R);
    
    std::unique_ptr<QMMatrixObject> Kt = Pk->multiply( &(*(temp->inverseMatrix())) );
    
    std::unique_ptr<QMMatrixObject> xt = xk->plus( &(*Kt->multiply( &(*this->zt->minus( &(*xk)) ) )) );
    std::unique_ptr<QMMatrixObject> Pt = QMMatrixObject::getIdentityMatrix(stateMDimension);
    Pt->printMatrix();
    Pt = Pt->minus(&(*Kt));
    Pt->printMatrix();
    Pt = Pt->multiply(&(*Pk));
    Pt->printMatrix();
    
    delete this->xk1;
    this->xk1 = new QMMatrixObject(&(*xt));
    delete this->Pk1;
    this->Pk1 = new QMMatrixObject(&(*Pt));
    
    double lat = this->xk1->matrix[0][0];
    double lon = this->xk1->matrix[2][0];
    double altitude = this->xk1->matrix[4][0];
    
    QMLocationPoint *newP = new QMLocationPoint(lat, lon,previousMeasureTime, altitude);
    
    return newP;
}

