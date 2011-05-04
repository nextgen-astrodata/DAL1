/***************************************************************************
 *   Copyright (C) 2011                                                    *
 *   Lars B"ahren <bahren@astron.nl>                                       *
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

#include <iostream>
#include <hdf5.h>

/*!
  \file h5_attribute_string

  \ingroup DAL

  \brief Test creating/writing/reading an attribute of type \e string

  \author Lars B&auml;hren

  \date 2011/05/03
*/

int main (int argc, char *argv[])
{
  int nofFailedTests   = 0;
  hid_t fileID         = 0;
  std::string filename = "h5_attribute_string.h5";

  //________________________________________________________
  // Process command line parameters

  if (argc>1) {
    filename = argv[1];
  }

  //________________________________________________________
  // Open/create HDF5 file to work with
  
  fileID = H5Fcreate (filename.c_str(),
		      H5F_ACC_TRUNC,
		      H5P_DEFAULT,
		      H5P_DEFAULT);
  
  //________________________________________________________
  // Run the tests
  

    
  //________________________________________________________
  // Release HDF5 objects
  
  if (H5Iis_valid(fileID)) {
    H5Fclose(fileID);
  }
  
  return nofFailedTests;
}
