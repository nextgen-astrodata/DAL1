
#include <iostream>
#include <dal1_config.h>

int main ()
{
  /*________________________________________________________
    Library version
  */
  
  std::cout << "DAL_VERSION "       << DAL1_VERSION       << std::endl;
  std::cout << "DAL_VERSION_MAJOR " << DAL1_VERSION_MAJOR << std::endl;
  std::cout << "DAL_VERSION_MINOR " << DAL1_VERSION_MINOR << std::endl;
  std::cout << "DAL_VERSION_PATCH " << DAL1_VERSION_PATCH << std::endl;

  /*________________________________________________________
    External packages
  */
  
  std::cout << "DAL_WITH_BOOST "    << DAL_WITH_BOOST    << std::endl;
  std::cout << "DAL_WITH_CFITSIO "  << DAL_WITH_CFITSIO  << std::endl;
  std::cout << "DAL_WITH_CASACORE " << DAL_WITH_CASACORE << std::endl;
  std::cout << "DAL_WITH_GSL "      << DAL_WITH_GSL      << std::endl;
  std::cout << "DAL_WITH_HDF5 "     << DAL_WITH_HDF5     << std::endl;
  std::cout << "DAL_WITH_LOFAR "    << DAL_WITH_LOFAR    << std::endl;
  std::cout << "DAL_WITH_MYSQL "    << DAL_WITH_MYSQL    << std::endl;
  std::cout << "DAL_WITH_OPENMP "   << DAL_WITH_OPENMP   << std::endl;
  std::cout << "DAL_WITH_WCSLIB "   << DAL_WITH_WCSLIB   << std::endl;

  return 1;
}
