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

#include "Database.h"

namespace DAL {

  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                     Database

  Database::Database ()
  {
    itsServerName   = "";
    itsUserName     = "";
    itsPassword     = "";
    itsDatabaseName = "";
    itsPortNumber   = 0;
  }
  
  //_____________________________________________________________________________
  //                                                                     Database
  
  /*!
    \param server   -- Name of the server to connect to.
    \param username -- The \c username parameter contains the user's MySQL login
           ID. If \c username is \c NULL or the empty string "", the current user
	   is assumed. Under Unix, this is the current login name. Under Windows
	   ODBC, the current user name must be specified explicitly.
    \param password -- The \c password parameter contains the password for user.
           If \c password is \c NULL, only entries in the user table for the user
	   that have a blank (empty) password field are checked for a match. This
	   enables the database administrator to set up the MySQL privilege
	   system in such a way that users get different privileges depending on
	   whether they have specified a password. 
    \param database -- Name of database.
    \param port     -- Port number over which to establish the connection.
  */
  Database::Database (std::string const& server,
                      std::string const& username,
                      std::string const& password,
                      std::string const& database,
		      unsigned int const &port)
  {
    connect (server,
	     username,
	     password,
	     database,
	     port);
  }
  
  //_____________________________________________________________________________
  //                                                                    ~Database
  
  Database::~Database()
  {
#ifdef DAL_WITH_MYSQL
    if (itsDatabaseConnector)
      {
        /* close connection */
        mysql_close(itsDatabaseConnector);
      }
#endif
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                      connect
  
  /*!
    \param server   -- Name of the server to connect to.
    \param username -- The \c username parameter contains the user's MySQL login
           ID. If \c username is \c NULL or the empty string "", the current user
	   is assumed. Under Unix, this is the current login name. Under Windows
	   ODBC, the current user name must be specified explicitly.
    \param password -- The \c password parameter contains the password for user.
           If \c password is \c NULL, only entries in the user table for the user
	   that have a blank (empty) password field are checked for a match. This
	   enables the database administrator to set up the MySQL privilege
	   system in such a way that users get different privileges depending on
	   whether they have specified a password. 
    \param database -- Name of database.
    \param port     -- Port number over which to establish the connection.
    \return status  -- Status of the operation; returns \e false in case an error
            was encountered.
  */
  bool Database::connect (std::string const& server,
			  std::string const& username,
			  std::string const& password,
			  std::string const& database,
			  unsigned int const &port)
  {
    bool status = true;
    
#ifdef DAL_WITH_MYSQL
    
    itsDatabaseConnector = mysql_init(NULL);
    
    /* Connect to database */
    if ( mysql_real_connect (itsDatabaseConnector,
			     itsServerName.c_str(),
			     itsUserName.c_str(),
			     itsPassword.c_str(),
			     itsDatabaseName.c_str(),
			     port,
			     NULL,
			     0))
      {
	// Store the parameters used for the conection
	itsServerName   = server;
	itsUserName     = username;
	itsPassword     = password;
	itsDatabaseName = database;
	itsPortNumber   = port;
      } else {
      fprintf(stderr, "%s\n", mysql_error(itsDatabaseConnector));
      status = false;
    }
#else
    // Store parameters (to avoid compiler warnings) ...
    itsServerName   = server;
    itsUserName     = username;
    itsPassword     = password;
    itsDatabaseName = database;
    itsPortNumber   = port;
    // ... and set return value.
    status          = false;
#endif
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                        query
  
  /*!
    \brief Query the database.
    \param querystr Query string to send to database.
  */
  bool Database::query (std::string const &querystr)
  {
    bool status = true;

#ifdef DAL_WITH_MYSQL

    /* send SQL query */
    if (mysql_query(itsDatabaseConnector, querystr.c_str())) {
      fprintf(stderr, "%s\n", mysql_error(itsDatabaseConnector));
      status = false;
    }
    
    res = mysql_use_result(itsDatabaseConnector);
    
    /* For now we print the result the screen, but in the future we most
       likely want to put the result into a data struct for use as input
       somewhere else
    */
    //while ((itsDatabaseRow = mysql_fetch_row(res)) != NULL)
    //  printf("%s \n", itsDatabaseRow[0]);
    
    mysql_free_result(res);
#else
    std::cerr << "[Database::query] Unable to perform query '"
	      << querystr
	      << "' - no connection to database!"
	      << std::endl;
    status = false;
#endif

    return status;
  }

  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void Database::summary (std::ostream &os)
  {
    os << "[Database] Summary of internal parameters." << std::endl;
    os << "-- The name of the server = " << itsServerName   << std::endl;
    os << "-- Database user name     = " << itsUserName     << std::endl;
    os << "-- Database user password = " << itsPassword     << std::endl;
    os << "-- Port number on server  = " << itsPortNumber   << std::endl;
    os << "-- Name of the database   = " << itsDatabaseName << std::endl;
  }
  
} // end namespace DAL
