/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2010                                                    *
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

#include <BF_StokesDataset.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                             BF_StokesDataset
  
  BF_StokesDataset::BF_StokesDataset ()
    : HDF5Dataset()
  {;}
  
  //_____________________________________________________________________________
  //                                                             BF_StokesDataset
  
  BF_StokesDataset::BF_StokesDataset (hid_t const &location,
				      std::string const &name)
    : HDF5Dataset(location,
		  name)
  {;}
  

  //_____________________________________________________________________________
  //                                                             BF_StokesDataset
  
  BF_StokesDataset::BF_StokesDataset (hid_t const &location,
				      std::string const &name,
				      std::vector<hsize_t> const &shape,
				      hid_t const &datatype)
    : HDF5Dataset (location,
		   name,
		   shape,
		   datatype)
  {}

  /*!
    \param other -- Another HDF5Property object from which to create this new
           one.
  */
  BF_StokesDataset::BF_StokesDataset (BF_StokesDataset const &other)
    : HDF5Dataset (other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  BF_StokesDataset::~BF_StokesDataset ()
  {
    destroy();
  }
  
  void BF_StokesDataset::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  /*!
    \param other -- Another BF_StokesDataset object from which to make a copy.
  */
  BF_StokesDataset& BF_StokesDataset::operator= (BF_StokesDataset const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void BF_StokesDataset::copy (BF_StokesDataset const &other)
  {;}

  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void BF_StokesDataset::summary (std::ostream &os)
  {
    os << "[BF_StokesDataset] Summary of internal parameters." << std::endl;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  

} // Namespace DAL -- end
