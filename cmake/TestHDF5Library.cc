
#include <iostream>
#include <H5public.h>

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
