/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
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

#include <string>
#include <mysql/mysql.h>

#ifndef DAL_H
#include "dal.h"
#endif

namespace DAL
  {

  /*!
    \class Database
    \ingroup DAL
    \brief Class to allow database connections

    Class to allow database connections

   */
  class Database
    {
      std::string server;
      std::string username;
      std::string password;
      std::string port;  // if needed
      std::string database;

      MYSQL *conn;  // DB connection
      MYSQL_RES *res;  // result
      MYSQL_ROW row;

    public:

      // constructor
      Database( std::string const& server,
                std::string const& username,
                std::string const& password,
                std::string const& database );

      // destructor
      ~Database();

      bool query(std::string const & querystr);

    }; // end class

} // end namespace DAL

#endif // DATABASE_H
