
#include <iostream>
#include <dal_config.h>

int main ()
{
  /*________________________________________________________
    Library version
  */
  
  std::cout << "DAL_VERSION_MAJOR " << DAL_VERSION_MAJOR << std::endl;
  std::cout << "DAL_VERSION_MINOR " << DAL_VERSION_MINOR << std::endl;
  std::cout << "DAL_VERSION_PATCH " << DAL_VERSION_PATCH << std::endl;

  return 1;
}
