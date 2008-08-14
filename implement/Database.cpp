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

namespace DAL
  {

// ----------------------------------------------------- Database

  /*!
    \brief Constructor
    \param server Name of the server to connect to.
    \param username Database username.
    \param password Database password.
    \param database Name of database.
   */
  Database::Database( std::string const& lcl_server,
                      std::string const& lcl_username,
                      std::string const& lcl_password,
                      std::string const& lcl_database )
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
      }

  }

// ----------------------------------------------------- ~Database

  /*!
    \brief Destructor
   */
  Database::~Database()
  {
    if (conn)
      {
        /* close connection */
        mysql_close(conn);
      }
  }

// ----------------------------------------------------- query

  /*!
    \brief Query the database.
    \param querystr Query string to send to database.
   */
  void Database::query( std::string const& querystr )
  {
    /* send SQL query */
    if (mysql_query(conn, querystr.c_str()))
      {
        fprintf(stderr, "%s\n", mysql_error(conn));
      }

    res = mysql_use_result(conn);

    /* For now we print the result the screen, but in the future we most
       likely want to put the result into a data struct for use as input
       somewhere else
     */
    while ((row = mysql_fetch_row(res)) != NULL)
      printf("%s \n", row[0]);

    mysql_free_result(res);
  }

} // end namespace DAL

// #endif // WITH_MYSQL
