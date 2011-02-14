
#include <mysql/mysql.h>

//_____________________________________________________________________________
//                                                                 test_connect

// int test_connect (std::string const &server,
// 		  std::string const &user,
// 		  std::string const &password,
// 		  std::string const &database)
// {
//   int status = 0;

//   mysql_real_connect (itsDatabaseConnector,
// 		      server.c_str(),
// 		      user.c_str(),
// 		      password.c_str(),
// 		      database.c_str(),
// 		      0,
// 		      NULL,
// 		      0);
  
//     return status;
// }

//_____________________________________________________________________________
//                                                                         main

int main ()
{
  int nofFailedTests = 0;

  MYSQL mysql;
  // MYSQL *connection;
  // MYSQL_RES *result;
  // MYSQL_ROW row;
  // int query_state;

  mysql_init(&mysql);

  return nofFailedTests;
}
