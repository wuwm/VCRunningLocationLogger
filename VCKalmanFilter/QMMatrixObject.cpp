//
//  QMMatrixObject.cpp
//  QMKalmanFilter
//
//  Created by Weiming Wu on 2018/7/24.
//  Copyright © 2018 Weiming Wu. All rights reserved.
//

#include "QMMatrixObject.hpp"

QMMatrixObject::QMMatrixObject(void)
{
    
}

QMMatrixObject::QMMatrixObject(int N, int M)
{
    this->M = M;
    this->N = N;
    this->matrix = new MatrixElement*[N];
    for(int i = 0; i < N; ++i)
    {
        this->matrix[i] = new MatrixElement[M];
    }
    for(int i = 0; i < N; ++i)
    {
        for(int j = 0; j < M; ++j)
        {
            this->matrix[i][j] = 0.0;
        }
    }
}

QMMatrixObject::QMMatrixObject(QMMatrixObject *matrixObject)
{
    this->M = matrixObject->M;
    this->N = matrixObject->N;
    this->matrix = new MatrixElement*[N];
    for(int i = 0; i < N; ++i)
    {
        this->matrix[i] = new MatrixElement[M];
    }
    for(int i = 0; i < N; ++i)
    {
        for(int j = 0; j < M; ++j)
        {
            this->matrix[i][j] = matrixObject->matrix[i][j];
        }
    }
}

QMMatrixObject::~QMMatrixObject()
{
    delete [] this->matrix;
}

void QMMatrixObject::addElement(int i, int j, MatrixElement value)
{
    if(i < this->N && j < this->M)
    {
        this->matrix[i][j] = value;
    }
    else
    {
        //这里print(error);
    }
}

void QMMatrixObject::setMatrix(Matrix matrix)
{
    if(this->matrix != NULL && this->N > 0)
    {
        this->matrix = matrix;
    }
}

MatrixElement QMMatrixObject::getElement(int i, int j)
{
    if(i < N && j < M)
    {
        return this->matrix[i][j];
    }
    else
    {
        return -999999;
    }
}

QMMatrixObject* QMMatrixObject::transpose()
{
    QMMatrixObject *result = new QMMatrixObject(this->M, this->N);
    for(int i = 0; i < N; ++i)
    {
        for(int j = 0; j < M; ++j)
        {
            result->matrix[j][i] = this->matrix[i][j];
        }
    }
    return result;
}

MatrixElement QMMatrixObject::determinant(const int n, const QMMatrixObject *mat, const double det)
{
    QMMatrixObject *submat = new QMMatrixObject(n, n);
    double d = det;
    
    if(n == 2)
    {
        return ((mat->matrix[0][0] * mat->matrix[1][1]) - (mat->matrix[1][0] * mat->matrix[0][1]));
    }
    else
    {
        for(int c = 0; c < n; ++c)
        {
            int subi = 0;
            for(int i =1; i < n; ++i)
            {
                int subj = 0;
                for(int j = 0; j < n; ++j)
                {
                    if(j == c)
                    {
                        continue;
                    }
                    submat->matrix[subi][subj] = mat->matrix[i][j];
                    subj += 1;
                }
                subi += 1;
            }
            if(c % 2 == 0)
            {
                d = d + mat->matrix[0][c] * this->determinant(n-1, submat, d);
            }
            else
            {
                d = d - mat->matrix[0][c] * this->determinant(n-1, submat, d);
            }
        }
    }
    delete submat;
    return d;
}

MatrixElement QMMatrixObject::determinant()
{
    return this->determinant(this->N, this, 0.0);
}

std::unique_ptr<QMMatrixObject> QMMatrixObject::inverseMatrix()
{
    QMMatrixObject *a = new QMMatrixObject(2*N, 2*N);
    QMMatrixObject *result = new QMMatrixObject(N, N);
    MatrixElement d;
    
    for(int i = 0; i < N; ++i)
    {
        for(int j = 0; j < N; ++j)
        {
            a->matrix[i][j] = matrix[i][j];
        }
    }
    
    for(int i = 0; i < N; ++i)
    {
        for(int j = 0; j < 2*N; ++j)
        {
            if(j == (i+N))
            {
                a->matrix[i][j] = 1.0;
            }
        }
    }
    
    for(int i = N-1; i > 1; --i)
    {
        if(a->matrix[i-1][1] < a->matrix[i][1])
        {
            for(int j = 0; j < 2*N; ++j)
            {
                d = a->matrix[i][j];
                a->matrix[i][j] = a->matrix[i-1][j];
                a->matrix[i-1][j] = d;
            }
        }
    }
    
    for(int i = 0; i < N; ++i)
    {
        for(int j = 0; j < 2*N; ++j)
        {
            if(j != i)
            {
                d = (a->matrix[j][i]) / (a->matrix[i][i]);
                for(int k = 0; k < 2*N; ++k)
                {
                    a->matrix[j][k] = a->matrix[j][k] - (a->matrix[i][k] * d);
                }
            }
        }
    }
    
    for(int i = 0; i < N; ++i)
    {
        d = a->matrix[i][i];
        for(int j = 0; j < 2*N; ++j)
        {
            a->matrix[i][j] = a->matrix[i][j] / d;
        }
    }
    
    for(int i = 0; i < N; ++i)
    {
        for(int j = N; j < 2*N; ++j)
        {
            result->matrix[i][j-N] = a->matrix[i][j];
        }
    }
    delete a;
    return std::unique_ptr<QMMatrixObject>(result);
}

void QMMatrixObject::printMatrix()
{
    for(int i = 0; i < this->N; ++i)
    {
        for(int j = 0; j < this->M; ++j)
        {
            printf("%lf ",this->matrix[i][j]);
        }
        std::cout << "---\n";
    }
}

QMMatrixObject* QMMatrixObject::operator+(const QMMatrixObject *right)
{
    QMMatrixObject *result = new QMMatrixObject(this->N, this->M);
    if(this->M == right->M && this->N == right->N)
    {
        for(int i = 0; i < this->N; ++i)
        {
            for(int j = 0; j < this->M; ++j)
            {
                result->matrix[i][j] = this->matrix[i][j] + right->matrix[i][j];
            }
        }
    }
    return result;
}

std::unique_ptr<QMMatrixObject> QMMatrixObject::plus(const QMMatrixObject *right)
{
    QMMatrixObject *result = new QMMatrixObject(this->N, this->M);
    if(this->M == right->M && this->N == right->N)
    {
        for(int i = 0; i < this->N; ++i)
        {
            for(int j = 0; j < this->M; ++j)
            {
                result->matrix[i][j] = this->matrix[i][j] + right->matrix[i][j];
            }
        }
    }
    return std::unique_ptr<QMMatrixObject>(result);
}

QMMatrixObject* QMMatrixObject::operator-(const QMMatrixObject *right)
{
    QMMatrixObject *result = new QMMatrixObject(this->N, this->M);
    if(this->M == right->M && this->N == right->N)
    {
        for(int i = 0; i < this->N; ++i)
        {
            for(int j = 0; j < this->M; ++j)
            {
                result->matrix[i][j] = this->matrix[i][j] - right->matrix[i][j];
            }
        }
    }
    return result;
}

std::unique_ptr<QMMatrixObject> QMMatrixObject::minus(const QMMatrixObject *right)
{
    QMMatrixObject *result = new QMMatrixObject(this->N, this->M);
    if(this->M == right->M && this->N == right->N)
    {
        for(int i = 0; i < this->N; ++i)
        {
            for(int j = 0; j < this->M; ++j)
            {
                result->matrix[i][j] = this->matrix[i][j] - right->matrix[i][j];
            }
        }
    }
    return std::unique_ptr<QMMatrixObject>(result);
}

QMMatrixObject* QMMatrixObject::operator*(const QMMatrixObject *right)
{
    QMMatrixObject *result = new QMMatrixObject(this->N, right->M);
    if(this->M == right->N)
    {
        for(int i = 0; i < this->N; ++i)
        {
            for(int j = 0; j < right->M; ++j)
            {
                for(int k = 0; k < this->M; ++k)
                {
                    result->matrix[i][j] += this->matrix[i][k] * right->matrix[k][j];
                }
            }
        }
    }
    return result;
}

std::unique_ptr<QMMatrixObject> QMMatrixObject::multiply(const QMMatrixObject *right)
{
    QMMatrixObject *result = new QMMatrixObject(this->N, right->M);
    if(this->M == right->N)
    {
        for(int i = 0; i < this->N; ++i)
        {
            for(int j = 0; j < right->M; ++j)
            {
                for(int k = 0; k < this->M; ++k)
                {
                    result->matrix[i][j] += this->matrix[i][k] * right->matrix[k][j];
                }
            }
        }
    }
    return std::unique_ptr<QMMatrixObject>(result) ;
}








