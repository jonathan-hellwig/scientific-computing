#include <iostream>
#include <cmath>
#include <cstdlib>
#include <mpi.h>
#include <unistd.h>

double func(double x)
{
  return 1.0/(1.0+x*x);
}
int main(int argc, char* argv[])
{
  srand(getpid());
  long total_num_points = 10000000000;
  double local_sum = 0.0;
  
  MPI::Init(argc, argv);

  int num_procs = MPI::COMM_WORLD.Get_size();
  int rank = MPI::COMM_WORLD.Get_rank();
  long local_num_points = total_num_points / num_procs;

  if(rank == num_procs-1)
    local_num_points += total_num_points%num_procs;

  for(long i=0; i<local_num_points; i++)
  {
    double x = rand()/((double)(RAND_MAX));
    local_sum += func(x);
  }
  double global_sum;
  MPI::COMM_WORLD.Reduce(&local_sum, &global_sum, 1,
              MPI::DOUBLE, MPI::SUM, 0);
  if(rank == 0)
  {
    double pi = 4.0*(global_sum/(double)(total_num_points));
    std::cout << "Pi is about " << pi << std::endl;
  }
  MPI::Finalize();
  return 0;
}

