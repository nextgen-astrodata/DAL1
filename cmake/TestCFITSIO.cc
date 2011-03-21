
#include <iostream>
#include <fitsio.h>

int main () 
{

  /*________________________________________________________
    Display version information
  */
  
  std::cout << "CFITSIO_VERSION " << CFITSIO_VERSION << std::endl;
  std::cout << "CFITSIO_MAJOR "   << CFITSIO_MAJOR   << std::endl;
  std::cout << "CFITSIO_MINOR "   << CFITSIO_MINOR   << std::endl;

  /*________________________________________________________
    Global variables
  */
  
  std::cout << "NIOBUF "        << NIOBUF        << std::endl;
  std::cout << "IOBUFLEN "      << IOBUFLEN      << std::endl;
  std::cout << "FLEN_FILENAME " << FLEN_FILENAME << std::endl;
  std::cout << "FLEN_KEYWORD "  << FLEN_KEYWORD  << std::endl;
  std::cout << "FLEN_CARD "     << FLEN_CARD     << std::endl;
  std::cout << "FLEN_VALUE "    << FLEN_VALUE    << std::endl;
  std::cout << "FLEN_COMMENT "  << FLEN_COMMENT  << std::endl;

  /* return value to indicate success to CMake */
  return 1;
}
