
#include <iostream>
#include <mysql.h>

//_____________________________________________________________________________
//                                                                         main

int main ()
{
  // MYSQL mysql;
  // MYSQL *connection;
  // MYSQL_RES *result;
  // MYSQL_ROW row;
  int status;

  /*__________________________________________________________________
    This function should be called to initialize the MySQL library
    before you call any other MySQL function, whether your application
    is a regular client program or uses the embedded server. If the
    application uses the embedded server, this call starts the server
    and initializes any subsystems (mysys, InnoDB, and so forth) that
    the server uses.
  */
  
  status = mysql_library_init (0, NULL, NULL);

  if (status) {
    std::cerr << "[MySQL] ERROR - could not initialize MySQL library!"
	      << std::endl;
    return 0;
  } else {
    std::cout << "[MySQL] Successfully initialize MySQl library." << std::endl;
  }  

  return 1;
}
