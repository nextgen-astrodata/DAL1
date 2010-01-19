/*-------------------------------------------------------------------------*
 | $Id:: NewClass.h 2286 2009-02-03 10:50:48Z baehren                    $ |
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

#ifndef HDF5HYPERSLAB_H
#define HDF5HYPERSLAB_H

// Standard library header files
#include <iostream>
#include <string>
#include <vector>

#include <dalCommon.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class HDF5Hyperslab
    
    \ingroup data_common
    
    \brief Brief description for class HDF5Hyperslab
    
    \author Lars B&auml;hren

    \date 2009/11/25

    \test tHDF5Hyperslab.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>[start filling in your text here]
    </ul>
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */  
  class HDF5Hyperslab {
    
    //! Identifier for this dataset within the HDF5 file
    hid_t datasetID_p;
    //! Rank of the dataspace addressed by the hyperslab
    int rank_p;
    //! Starting location for the hyperslab
    std::vector<int> start_p;
    //! Number of elements to separate each element or block to be selected
    std::vector<int> stride_p;
    //! The number of elements or blocks to select along each dimension
    std::vector<int> count_p;
    //! The size of the block selected from the dataspace
    std::vector<int> block_p;
    
  public:
    
    // ------------------------------------------------------------- Construction
    
    //! Default constructor
    HDF5Hyperslab ();
    
    //! Argumented constructor
    HDF5Hyperslab (hid_t const &location);
    
    //! Argumented constructor
    HDF5Hyperslab (hid_t const &location,
		      std::vector<int> const &start,
		      std::vector<int> const &stride,
		      std::vector<int> const &count,
		      std::vector<int> const &block);
    
    /*!
      \brief Copy constructor
      
      \param other -- Another HDF5Hyperslab object from which to create this new
             one.
    */
    HDF5Hyperslab (HDF5Hyperslab const &other);
    
    // -------------------------------------------------------------- Destruction

    //! Destructor
    ~HDF5Hyperslab ();
    
    // ---------------------------------------------------------------- Operators
    
    /*!
      \brief Overloading of the copy operator
      
      \param other -- Another HDF5Hyperslab object from which to make a copy.
    */
    HDF5Hyperslab& operator= (HDF5Hyperslab const &other); 
    
    // --------------------------------------------------------------- Parameters
    
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, HDF5Hyperslab.
    */
    inline std::string className () const {
      return "HDF5Hyperslab";
    }

    /*!
      \brief Provide a summary of the internal status
    */
    inline void summary () {
      summary (std::cout);
    }

    /*!
      \brief Provide a summary of the internal status

      \param os -- Output stream to which the summary is written.
    */
    void summary (std::ostream &os);    

    // ------------------------------------------------------------------ Methods

    //! Open dataset for which the hyperslab is created
    bool openDataset (hid_t const &location,
		      std::string const &name);
    
  private:
    
    //! Unconditional copying
    void copy (HDF5Hyperslab const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class HDF5Hyperslab -- end
  
} // Namespace DAL -- end

#endif /* HDF5HYPERSLAB_H */
  
