
#include <iostream>
#include <boost/version.hpp>

int main ()
{

  /*________________________________________________________
    Library version
  */
  
  std::cout << "BOOST_VERSION "       << BOOST_VERSION        << std::endl;
  std::cout << "BOOST_VERSION_MAJOR " << BOOST_VERSION/100000 << std::endl;
  std::cout << "BOOST_LIB_VERSION "   << BOOST_LIB_VERSION    << std::endl;

  return 1;
}
