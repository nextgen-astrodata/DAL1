/*-------------------------------------------------------------------------*
 | $Id:: dal.h 1126 2007-12-10 17:14:20Z masters                         $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008 by Joseph Masters                                  *
 *   jmasters@science.uva.nl                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef DATABASE_H
#include "Database.h"
#endif

// #ifdef WITH_MYSQL

namespace DAL
  {

  // constructor
  Database::Database( string const& lcl_server,
                      string const& lcl_username,
                      string const& lcl_password,
                      string const& lcl_database )
  {
    server = lcl_server;
    username = lcl_username;
    password = lcl_password;
    database = lcl_database;
    port = "";

    conn = mysql_init(NULL);

    /* Connect to database */
    if ( !mysql_real_connect(conn, server.c_str(),
                             username.c_str(), password.c_str(),
                             database.c_str(), 0, NULL, 0))
      {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
      }

  }

  // destructor
  Database::~Database()
  {
    if (conn)
      {
        /* close connection */
        mysql_close(conn);
      }
  }

  void Database::query( string const& querystr )
  {
    /* send SQL query */
    if (mysql_query(conn, querystr.c_str()))
      {
        fprintf(stderr, "%s\n", mysql_error(conn));
      }
    res = mysql_use_result(conn);
    /* output table names */
    while ((row = mysql_fetch_row(res)) != NULL)
      printf("%s \n", row[0]);
    mysql_free_result(res);
  }

} // end namespace DAL

// #endif // WITH_MYSQL
