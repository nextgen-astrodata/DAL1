
#include <iostream>
#include <H5public.h>

#ifdef TEST_HDF5_PARALLEL
#include <mpi.h>
#endif

int main ()
{
  /*________________________________________________________
    Library version
  */
  
  std::cout << "H5_VERS_MAJOR " << H5_VERS_MAJOR << std::endl;
  std::cout << "H5_VERS_MINOR " << H5_VERS_MINOR << std::endl;
  std::cout << "H5_VERS_RELEASE " << H5_VERS_RELEASE << std::endl;

  /*________________________________________________________
    Support for parallel I/O
  */
  
#ifdef H5_HAVE_PARALLEL
  std::cout << "H5_HAVE_PARALLEL 0" << std::endl;
#else
  std::cout << "H5_HAVE_PARALLEL 1" << std::endl;
#endif

#ifdef TEST_HDF5_PARALLEL
  /* Declare variables */
  int mpi_size = 0;
  int mpi_rank = 0;
  MPI_Comm comm  = MPI_COMM_WORLD;
  MPI_Info info  = MPI_INFO_NULL;
  /* Initialize MPI */
  MPI_Init(&argc, &argv);
  MPI_Comm_size(comm, &mpi_size);
  MPI_Comm_rank(comm, &mpi_rank);
#endif
  
  /*________________________________________________________
    Default API version
  */

#ifdef H5_USE_16_API_DEFAULT
  std::cout << "H5_USE_16_API_DEFAULT 1" << std::endl;
#else
  std::cout << "H5_USE_16_API_DEFAULT 0" << std::endl;
#endif
  
  return 1;
}
