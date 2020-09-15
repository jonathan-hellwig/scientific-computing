#include "MpiVector.hpp"
#include <iostream>
#include <cmath>
#include <mpi.h>
#include <cassert>
#include <initializer_list>

MpiVector::MpiVector(int size)
{
  assert(size > -1);

  int num_process = MPI::COMM_WORLD.Get_size();
  int rank = MPI::COMM_WORLD.Get_rank();
  int local_size = size/num_process;

  mLower = rank*local_size;
  mUpper = (rank+1)*local_size;
  mSize = size;

  if(rank == num_process-1)
    mUpper = mSize;

  mData = new double[mUpper - mLower];
  mGlobalData = new double[mSize];

}
MpiVector::MpiVector(std::initializer_list<double> list):
  MpiVector(static_cast<int>(list.size()))
{
  int count = 0;
  for(auto element: list)
  {
    if(mLower <= count && count < mUpper)
      mData[count-mLower] = element;
    ++count;
  }
}

MpiVector::~MpiVector()
{
  delete[] mData;
  delete[] mGlobalData;
}
  
void MpiVector::assign(double value)
{
  for(int i=0; i<mUpper-mLower; i++)
  {
    mData[i] = value;
  }
}
int MpiVector::getSize() const
{
  return mSize;
}
double& MpiVector::localData(int globalIndex)
{
  assert(mLower <= globalIndex  && globalIndex < mUpper);
  return mData[globalIndex - mLower];
}
double& MpiVector::globalData(int globalIndex)
{
  assert(0 <= globalIndex  && globalIndex < mSize);
  return mGlobalData[globalIndex];
}

int MpiVector::getLower() const
{
  return mLower;
}

int MpiVector::getUpper() const
{
  return mUpper;
}

double MpiVector::calculateNorm(int p) const
{
  double localSum = 0.0;
  for(int i=0; i<mUpper-mLower; i++)
    localSum += pow(fabs(mData[i]), p);

  double globalSum;
  MPI::COMM_WORLD.Allreduce(&localSum, &globalSum, 1,
            MPI::DOUBLE, MPI::SUM);

  double normVal = pow(globalSum, 1.0/((double)p));
  return normVal;
}

MpiVector MpiVector::operator-(const MpiVector& rhs) const
{ 
  assert(mSize == rhs.mSize);
  MpiVector diff(mSize);
  for(int i=0; i<mUpper-mLower; i++)
    diff.mData[i] = mData[i] - rhs.mData[i];
  return diff;
}
MpiVector MpiVector::operator-() const
{ 
  MpiVector neg(mSize);
  for(int i=0; i<mUpper-mLower; i++)
    neg.mData[i] = -mData[i];
  return neg;
}
MpiVector& MpiVector::operator=(const MpiVector& otherVector)
{
  for(int i=0; i<mUpper-mLower; i++)
    mData[i] = otherVector.mData[i];
  return *this;
}
MpiVector MpiVector::operator+() const
{ 
  MpiVector pos(mSize);
  for(int i=0; i<mUpper-mLower; i++)
    pos.mData[i] = mData[i];
  return pos;
}
MpiVector MpiVector::operator+(const MpiVector& rhs) const
{ 
  assert(mSize == rhs.mSize);
  MpiVector sum(mSize);
  for(int i=0; i<mUpper-mLower; i++)
    sum.mData[i] = mData[i] + rhs.mData[i];
  return sum;
}
MpiVector MpiVector::operator*(double scalar) const
{ 
  MpiVector scaledVector(mSize);
  for(int i=0; i<mUpper-mLower; i++)
    scaledVector.mData[i] = scalar*mData[i];
  return scaledVector;
}
void MpiVector::updateGlobal()
{
  int num_process = MPI::COMM_WORLD.Get_size();
  int rank = MPI::COMM_WORLD.Get_rank();

  int local_size = mSize / num_process;
  int* num_entries_received = new int[num_process];
  int* displacements = new int[num_process];

  // Calculate the number of entries in each process' local data 
  // and their displacements
  for(int i=0; i<num_process; i++)
  {
    num_entries_received[i] = local_size;
    displacements[i] = i*local_size;
  }
  // Add excess values to top process
  num_entries_received[num_process-1] += mSize%num_process;
  // Load local data into global data of every process
  MPI::COMM_WORLD.Allgatherv(mData, mUpper-mLower, MPI::DOUBLE,
                        mGlobalData, num_entries_received,
                        displacements, MPI::DOUBLE);

  delete[] num_entries_received;
  delete[] displacements;
}

std::ostream& operator<<(std::ostream& output, MpiVector& v)
{
  int rank = MPI::COMM_WORLD.Get_rank();

  output << "[";
  for(int i=0; i<v.mSize-1; i++)
    output << v.mGlobalData[i] << ",";
  output << v.mGlobalData[v.mSize-1] << "]";

  return output;
}
