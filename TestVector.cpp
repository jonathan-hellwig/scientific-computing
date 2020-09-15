#include <iostream>
#include <mpi.h>
#include "MpiVector.hpp"

int main(int argc, char* argv[])
{
  MPI::Init(argc, argv);
  int rank = MPI::COMM_WORLD.Get_rank();
  MpiVector A1(10);
  MpiVector A2(10);
  MpiVector B1{2.3,4.4,54.3,5453.54,454};
  B1.updateGlobal();
  for(int i=A1.getLower(); i<A1.getUpper(); i++)
  {
    A1.localData(i) = i;
    A2.localData(i) = i*i;
  }

  A1.updateGlobal();
  A2.updateGlobal();

  MpiVector A3(10);
  A3 = A1 + A2; 
  A3.updateGlobal();

  MpiVector A4(10);
  A4 = A1 - A2;
  A4.updateGlobal();

  MpiVector A5(10);
  A5 = A1*4.0;
  A5.updateGlobal();

  MpiVector A6(10);
  A6 = +A1;
  A6.updateGlobal();
  
  MpiVector A7(10);
  A7 = -A1;
  A7.updateGlobal();
  
  if(rank == 0)
  {
    std::cout << "A1: " << A1 << std::endl; 
    std::cout << "A2: " << A2 << std::endl; 
    std::cout << "A1+A2: " << A3 << std::endl; 
    std::cout << "A1-A2: " << A4 << std::endl; 
    std::cout << "A1*4.0: " << A5 << std::endl; 
    std::cout << "+A1: " << A6 << std::endl; 
    std::cout << "-A1: " << A7 << std::endl; 
    std::cout << "B1: " << B1 << std::endl;
  }


  MPI::Finalize();
}
