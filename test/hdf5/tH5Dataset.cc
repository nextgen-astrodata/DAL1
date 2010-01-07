/*-------------------------------------------------------------------------*
 | $Id:: tNewClass.cc 2286 2009-02-03 10:50:48Z baehren                  $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2009                                                    *
 *   Lars B"ahren (bahren@astron.nl)                                       *
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

/*!
  \file tHDF5_Dataset.cc

  \ingroup DAL

  \brief A collection of tests for working with a HDF5 Dataset object
 
  \author Lars B&auml;hren
 
  \date 2009/12/03
*/

#include <iostream>
#include <string>

#include <dalCommon.h>
#include "H5Dataset.h"

//_______________________________________________________________________________
//                                                                    test_create

/*!
  \brief Test the creation of a Dataset object within a file

  \param fileID -- File identifier
*/
int test_create (hid_t const &fileID)
{
  int nofFailedTests (0);
  std::string name;

  std::cout << "[1] Creating 2D dataset ..." << std::endl;
  try {
    std::vector<hsize_t> shape (2);

    name     = "Data2D";
    shape[0] = 1024;
    shape[1] = 4;
    
    //! Create the HDF5 Dataset
    DAL::H5Dataset dataset (fileID,
			    name,
			    shape);
    dataset.summary();
    
    dataset.setAttribute ("NOF_AXES", int(2)        );
    dataset.setAttribute ("NAXIS1",   int(shape[0]) );
    dataset.setAttribute ("NAXIS2",   int(shape[1]) );
  } catch (std::string message) {
    std::cerr << message << std::endl;
    ++nofFailedTests;
  }
  
  std::cout << "[2] Creating 3D dataset ..." << std::endl;
  try {
    std::vector<hsize_t> shape (3);
    
    name     = "Data3D";
    shape[0] = 1024;
    shape[1] = 1024;
    shape[2] = 3;
    
    //! Create the HDF5 Dataset
    DAL::H5Dataset dataset (fileID,
			    name,
			    shape);
    dataset.summary();
    
    dataset.setAttribute ("NOF_AXES", int(shape.size()) );
    dataset.setAttribute ("NAXIS1",   int(shape[0]) );
    dataset.setAttribute ("NAXIS2",   int(shape[1]) );
    dataset.setAttribute ("NAXIS3",   int(shape[2]) );
  } catch (std::string message) {
    std::cerr << message << std::endl;
    ++nofFailedTests;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

int main ()
{
  int nofFailedTests (0);
  std::string filename ("Dataset.h5");

  // open the file to work with
  hid_t fileID = H5Fcreate (filename.c_str(),
			    H5F_ACC_TRUNC,
			    H5P_DEFAULT,
			    H5P_DEFAULT);

  if (fileID > 0) {
    nofFailedTests += test_create (fileID);
  } else {
    std::cerr << "[tHDF5_Dataset] Failed to open file - skipping tests."
	      << std::endl;
  }
  
  // close the file
  H5Fclose(fileID);
  
  return nofFailedTests;
}
