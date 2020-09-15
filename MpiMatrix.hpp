#ifndef MATRIXHEADERDEF
#define MATRIXHEADERDEF

#include <iostream>
#include <initializer_list>
#include "MpiVector.hpp"

class MpiMatrix 
{
  private:
   double** mData;
   double** mGlobalData;
   int mRows, mCols, mLower, mUpper;
  public:
   ~MpiMatrix();
   MpiMatrix(int rows, int cols);
   MpiMatrix(std::initializer_list<std::initializer_list<double>> list);
   int getLower() const;
   int getUpper() const;
   int getRows() const;
   int getCols() const;
   void updateGlobal();
   void assign(double value);
   double& localData(rowIndex, int colIndex);
   MpiMatrix& operator=(const MpiMatrix& otherMatrix);
   MpiMatrix operator+() const;
   MpiMatrix operator-() const;
   MpiMatrix operator+(const MpiMatrix& rhs) const;
   MpiMatrix operator-(const MpiMatrix& rhs) const;
   MpiMatrix operator*(double scalar) const;
   friend MpiVector operator*(const MpiMatrix& A,
                              const MpiVector& b);
   friend std::ostream& operator<<(std::ostream& output,
                                   MpiMatrix& v);
};
friend MpiVector operator*(const MpiMatrix& A,
                           const MpiVector& b);

#endif
