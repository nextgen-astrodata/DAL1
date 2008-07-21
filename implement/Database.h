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
#define DATABASE_H

#ifndef DAL_H
#include "dal.h"
#endif

// #ifdef WITH_MYSQL
#include "mysql/mysql.h"

namespace DAL
  {

  class Database
    {
      string server;
      string username;
      string password;
      string port;  // if needed
      string database;

      MYSQL *conn;  // DB connection
      MYSQL_RES *res;  // result
      MYSQL_ROW row;

    public:

      // constructor
      Database( string const& server,
                string const& username,
                string const& password,
                string const& database );

      // destructor
      ~Database();

      void query(string const& querystr);

    }; // end class

} // end namespace DAL

// #endif // WITH_MYSQL

#endif // DATABASE_H
