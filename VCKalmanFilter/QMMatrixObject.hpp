//
//  QMMatrixObject.hpp
//  QMKalmanFilter
//
//  Created by Weiming Wu on 2018/7/24.
//  Copyright © 2018 Weiming Wu. All rights reserved.
//

#ifndef QMMatrixObject_hpp
#define QMMatrixObject_hpp

#include <iostream>

typedef double MatrixElement;
typedef MatrixElement** Matrix;


class QMMatrixObject
{
public:
    Matrix matrix;
    
    
    static int matrixRow(Matrix matrix)
    {
        int row = sizeof(matrix) / sizeof(matrix[0]);
        return row;
    }
    
    static int matrixColumn(Matrix matrix)
    {
        int n = sizeof(matrix) / sizeof(matrix[0][0]);
        int row = sizeof(matrix) / sizeof(matrix[0]);
        int col = n / row;
        return col;
    }
    
    QMMatrixObject();
    QMMatrixObject(int N, int M);
    QMMatrixObject(QMMatrixObject *matrixObject);
    ~QMMatrixObject();
    
    //MARK: - HCMatrixObject functions
    
    /// getIdentityMatrix Function
    /// ==========================
    /// For some dimension dim, return identity matrix object
    ///
    /// - parameters:
    ///   - dim: dimension of desired identity matrix
    /// - returns: identity matrix object
    static std::unique_ptr<QMMatrixObject> getIdentityMatrix(int dim)
    {
        QMMatrixObject *identityMatrix = new QMMatrixObject(dim, dim);
        for(int i = 0; i < dim; ++i)
        {
            for(int j = 0; j < dim; ++j)
            {
                if(i == j)
                {
                    identityMatrix->matrix[i][j] = 1.0;
                }
            }
        }
        return std::unique_ptr<QMMatrixObject> (identityMatrix);
    }
    
    /// addElement Function
    /// ===================
    /// Add double value on (i,j) position in matrix
    ///
    /// - parameters:
    ///   - i: row of matrix
    ///   - j: column of matrix
    ///   - value: double value to add in matrix
    void addElement(int i, int j, double value);
    
    /// setMatrix Function
    /// ==================
    /// Set complete matrix
    ///
    /// - parameters:
    ///   - matrix: array of array of double values
    void setMatrix(Matrix matrix);
    
    /// getElement Function
    /// ===================
    /// Returns double value on specific position of matrix
    ///
    /// - parameters:
    ///   - i: row of matrix
    ///   - j: column of matrix
    double getElement(int i, int j);
    
    /// Transpose Matrix Function
    /// =========================
    /// Returns result HCMatrixObject of transpose operation
    ///
    /// - returns: transposed HCMatrixObject object
    QMMatrixObject* transpose();

    /// Matrix Determinant Helper Function
    /// ==================================
    /// Helper function for recursively search determinant value.
    /// Returns submatrix accumulative determinant value.
    ///
    /// - parameters:
    ///   - n: dimension of submatrix
    ///   - mat: submatrix object
    ///   - det: accumulative previous submatrix determinant value
    /// - returns: submatrix accumulative determinant value
    MatrixElement determinant(const int n, const QMMatrixObject *mat, const double det);
    
    /// Matrix Determinant Function
    /// ===========================
    /// Returns matrix determinant value
    ///
    /// - returns: matrix determinant value
    double determinant();
    
    /// Inverse Matrix Function
    /// =======================
    /// Returns inverse matrix object
    ///
    /// - returns: inverse matrix object
    std::unique_ptr<QMMatrixObject> inverseMatrix();
    
    /// Print Matrix Function
    /// =====================
    /// Printing the entire matrix
    void printMatrix();
    
    //MARK: - Predefined HCMatrixObject operators
    
    /// Predefined + operator
    /// =====================
    /// Returns result HCMatrixObject of addition operation
    ///
    /// - parameters:
    ///   - right: right addition HCMatrixObject operand
    /// - returns: result HCMatrixObject object of addition operation
    QMMatrixObject* operator+(const QMMatrixObject* right);
    std::unique_ptr<QMMatrixObject> plus(const QMMatrixObject* right);
    
    /// Predefined - operator
    /// =====================
    /// Returns result HCMatrixObject of subtraction operation
    ///
    /// - parameters:
    ///   - left: left subtraction HCMatrixObject operand
    ///   - right: right subtraction HCMatrixObject operand
    /// - returns: result HCMatrixObject object of subtraction operation
    QMMatrixObject* operator-(const QMMatrixObject* right);
    std::unique_ptr<QMMatrixObject> minus(const QMMatrixObject* right);
    
    /// Predefined * operator
    /// =====================
    /// Returns result HCMatrixObject of multiplication operation
    ///
    /// - parameters:
    ///   - left: left multiplication HCMatrixObject operand
    ///   - right: right multiplication HCMatrixObject operand
    /// - returns: result HCMatrixObject object of multiplication operation
    QMMatrixObject* operator*(const QMMatrixObject* right);
    std::unique_ptr<QMMatrixObject> multiply(const QMMatrixObject* right);
    
private:
    int M;
    int N;
    
    
};



#endif /* QMMatrixObject_hpp */
