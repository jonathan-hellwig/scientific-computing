#ifndef VECTORHEADERDEF
#define VECTORHEADERDEF

#include "MpiMatrix.hpp"
#include <iostream>
#include <initializer_list>

class MpiVector 
{
  private:
   double* mData;
   double* mGlobalData;
   int mSize, mLower, mUpper;
  public:
   ~MpiVector();
   MpiVector(int size);
   MpiVector(std::initializer_list<double> list);
   int getLower() const;
   int getUpper() const;
   int getSize() const;
   double calculateNorm(int p) const;
   void updateGlobal();
   void assign(double value);
   double& localData(int index);
   double& globalData(int index);
   MpiVector& operator=(const MpiVector& otherVector);
   MpiVector operator+() const;
   MpiVector operator-() const;
   MpiVector operator+(const MpiVector& rhs) const;
   MpiVector operator-(const MpiVector& rhs) const;
   MpiVector operator*(double scalar) const;
   friend std::ostream& operator<<(std::ostream& output,
                                   MpiVector& v);
};

#endif
