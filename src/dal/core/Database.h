/***************************************************************************
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

#include <iostream>
#include <core/dalCommon.h>

namespace DAL {
  
  /*!
    \class Database

    \ingroup DAL
    \ingroup core

    \brief Class to allow database connections

    \author Joseph Masters, Lars B&auml;hren
  */
  class Database {
    
    //! The name of the server to connect to
    std::string itsServerName;
    //! The user name
    std::string itsUserName;
    //! The user password
    std::string itsPassword;
    //! The name of the database to work with
    std::string itsDatabaseName;
    //! The port number to which to connect (if required)
    unsigned int itsPortNumber;
    
#ifdef DAL_WITH_MYSQL
    //! Connector to the database
    MYSQL *itsDatabaseConnector;
    //! Result from database query
    MYSQL_RES *res;
    MYSQL_ROW itsDatabaseRow;
#endif
    
  public:

    // === Construction =========================================================

    // Default constructor
    Database ();
    
    // Argumented constructor
    Database (std::string const &server,
	      std::string const &username,
	      std::string const &password,
	      std::string const &database,
	      unsigned int const &port=0);
    
    //! Destructor
    ~Database();
    
    // === Parameter access =====================================================
    
    //! The name of the server to connect to
    inline std::string serverName () const {
      return itsServerName;
    }
    
    //! The user name
    inline std::string userName () const {
      return itsUserName;
    }
    
    //! The user password
    inline std::string password () const {
      return itsPassword;
    }
    
    //! The name of the database to work with
    inline std::string database () const {
      return itsDatabaseName;
    }

    //! The port number to which to connect (if required)
    inline unsigned int portNumber () const {
      return itsPortNumber;
    }
    
    // === Public methods =======================================================

    //! Connect to the database
    bool connect (std::string const &server,
		  std::string const &username,
		  std::string const &password,
		  std::string const &database,
		  unsigned int const &port=0);
    
    //! Query the database
    bool query (std::string const &querystr);
    
    //! Provide a summary of the internal status
    inline void summary () {
      summary (std::cout);
    }
    //! Provide a summary of the internal status
    void summary (std::ostream &os);    

  }; // end class
  
} // end namespace DAL

#endif // DATABASE_H
