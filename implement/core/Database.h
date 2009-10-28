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
#ifdef HAVE_MYSQL
#include <mysql/mysql.h>
#endif

#ifndef DAL_H
#include "dal.h"
#endif

namespace DAL {
  
  /*!
    \class Database
  \ingroup DAL
  \ingroup core
    \brief Class to allow database connections
  */
  class Database {
    
    //! The name of the server to connect to
    std::string server_p;
    //! The user name
    std::string username;
    //! The user password
    std::string password;
    //! The port number to which to connect (if required)
    std::string port_p;
    //! The name of the database to work with
    std::string database;
    
#ifdef HAVE_MYSQL
    //! Connector to the database
    MYSQL *conn;
    //! Result from database query
    MYSQL_RES *res;
    MYSQL_ROW row;
#endif
    
  public:
    
    // Constructor
    Database( std::string const& server,
	      std::string const& username,
	      std::string const& password,
	      std::string const& database );
    //! Destructor
    ~Database();
    
    //! Query the database
    bool query(std::string const & querystr);
    
    //! Provide a summary of the internal status
    inline void summary () {
      summary (std::cout);
    }
    //! Provide a summary of the internal status
    void summary (std::ostream &os);    

  }; // end class
  
} // end namespace DAL

#endif // DATABASE_H
