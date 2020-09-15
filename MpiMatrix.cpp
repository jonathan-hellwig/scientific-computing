#include "MpiMatrix.hpp"
#include "MpiVector.hpp"
#include <cassert>
#include <initializer_list>
#include <mpi.h>

MpiMatrix::MpiMatrix(int rows, int cols)
{
  assert(rows >= 0 && cols >= 0);

  int num_process = MPI::COMM_WORLD.Get_size();
  int rank = MPI::COMM_WORLD.Get_rank();
  int local_size = rows/num_process;

  mLower = rank*local_size;
  mUpper = (rank+1)*local_size;
  mRows = rows;
  mCols = cols;

  if(rank == num_process-1)
    mUpper = mRows;

  mData = new double*[mUpper - mLower];
  for(int i=0; i<mUpper-mLower; i++)
    mData[i] = new double[mCols];
}

MpiMatrix::MpiMatrix(std::initializer_list<std::initializer_list<double>> list):
  MpiMatrix(static_cast<int>(list.size()),
      static_cast<int>((*list.begin()).size()))
{
  int rowIndex = 0;
  for(auto row: list)
  {
    assert(row.size() == mCols);
    if(mLower <= rowIndex && rowIndex < mUpper)
    {
      int colIndex = 0;
      for(auto element: row)
      {
        mData[rowIndex-mLower][colIndex] = element;
        ++colIndex;
      }
    }
    ++rowIndex;
  }
}

MpiMatrix::~MpiMatrix()
{
  delete[] mData;
}
int MpiMatrix::getLower() const
{
  return mLower;
}
int MpiMatrix::getUpper() const
{
  return mUpper;
}
int MpiMatrix::getRows() const
{
  return mRows;
}
int MpiMatrix::getCols() const
{
  return mCols;
}

double& MpiMatrix::localData(int rowIndex, int colIndex)
{
  assert(mLower <= rowIndex && rowIndex < mUpper);
  assert(0 <= colIndex && colIndex < mCols);
  return mData[rowIndex][colIndex];
}

MpiVector operator*(const MpiMatrix& A, const MpiVector& b)
{
  //b.updateGlobal();
  assert(A.getCols() == b.getSize());
  MpiVector product(b.getSize()); 
  for(int i=0; i<b.getUpper()-b.getLower(); i++)
  {
    product.localData(i) = 0.0;
    for(int j=0; j<b.getSize(); i++)
    {
      product.localData(i) += A.localData(i,j)*b.globalData(j);
    }
  }
  return product;
}

