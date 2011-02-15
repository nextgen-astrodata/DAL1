
#include <iostream>
#include <boost/version.hpp>

int main ()
{

  /*________________________________________________________
    Library version
    BOOST_VERSION % 100 is the patch level
    BOOST_VERSION / 100 % 1000 is the minor version
    BOOST_VERSION / 100000 is the major version  
  */
  
  std::cout << "BOOST_VERSION "       << BOOST_VERSION          << std::endl;
  std::cout << "BOOST_VERSION_MAJOR " << BOOST_VERSION/100000   << std::endl;
  std::cout << "BOOST_VERSION_MINOR " << BOOST_VERSION/100%1000 << std::endl;
  std::cout << "BOOST_VERSION_PATCH " << BOOST_VERSION%100      << std::endl;
  std::cout << "BOOST_LIB_VERSION "   << BOOST_LIB_VERSION      << std::endl;

  return 1;
}
