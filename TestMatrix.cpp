#include <iostream>
#include "MpiMatrix.hpp"
#include "MpiVector.hpp"
#include <mpi.h>

int main(int argc, char* argv[])
{
  MPI::Init(argc, argv);
  MpiMatrix A1{{1.0,3.0},{5.4,64.5}};
  MpiVector b1{1.0,0.0};
  b1.updateGlobal();
  MpiVector b2{0.0,1.0};
  b2.updateGlobal();
  MpiVector c1(2);
  MpiVector c2(2);
  c1 = A1*b1;
  c2 = A1*b2;

  MPI::Finalize();
}
