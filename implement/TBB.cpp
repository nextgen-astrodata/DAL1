/*-------------------------------------------------------------------------*
 | $Id:: dal.h 1126 2007-12-10 17:14:20Z masters                         $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008 by Joseph Masters                                  *
 *   J.S.Masters@uva.nl                                               *
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
#ifndef TBB_H
#include "TBB.h"
#endif

namespace DAL {

  TBB::TBB( string const& filename )
  {

    name = filename;

    dataset = new dalDataset;

    if ( DAL::FAIL == dataset->open( filename.c_str() ) )
    {
      cerr << "Creating new dataset " << filename << endl;
      delete dataset;
      dataset = new dalDataset( filename.c_str(), "HDF5" );
    }
    else
    {
       cerr << "Dataset " << filename << " exists." << endl;

       vector<string> stations = dataset->getGroupNames();
       for( unsigned int ss = 0; ss < stations.size(); ss++ )
       {
          cerr << stations[ss] << endl;
       }

    }

  }

  // ------------------------------------------------------------- 
  //
  //  Set up the socket connection to the server
  //
  // ------------------------------------------------------------- 

  int TBB::connectsocket( char* ipaddress, char* portnumber )
  {
    int port_number = atol( portnumber );
    const char * remote = ipaddress;
    int remote_port = atol( portnumber );

    // Step 1 Look up server to get numeric IP address
    hostent * record = gethostbyname(remote);
    if (record==NULL) { herror("gethostbyname failed"); exit(1); }
    in_addr * addressptr = (in_addr *) record->h_addr;

    // Step 2 Create a socket
    int main_socket = socket(PF_INET, SOCK_DGRAM, 0);
    if (main_socket<0) { perror("socket creation"); exit(1); }

    // Step 3 Create a sockaddr_in to describe the local port
    sockaddr_in local_info;
    local_info.sin_family = AF_INET;
    local_info.sin_addr.s_addr = htonl(INADDR_ANY);
    local_info.sin_port = htons(port_number);

    // Step 4 Bind the socket to the port
    int rr = bind(main_socket, (sockaddr *) &local_info, sizeof(local_info));
    if (rr<0) { perror("bind"); exit(1); }
    printf("ready\n");

    // Step 5 Create a sockaddr_in to describe the remote application
    sockaddr_in remote_info;
    remote_info.sin_family = AF_INET;
    remote_info.sin_addr = *addressptr;
    remote_info.sin_port = htons(remote_port);
    return main_socket;
  }

} // end namespace DAL
