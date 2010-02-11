/*-------------------------------------------------------------------------*
 | $Id:: NewClass.h 4027 2010-01-21 13:05:37Z baehren                    $ |
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

#ifndef HDF5HYPERSLAB_H
#define HDF5HYPERSLAB_H

// Standard library header files
#include <iostream>
#include <string>
#include <vector>
// DAL header files
#include <dalCommon.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class HDF5Hyperslab
    
    \ingroup DAL
    \ingroup data_common
    
    \brief A hyperslab region for selective access to a dataspace
    
    \author Lars B&aulm;hren

    \date 2010/02/09

    \test tHDF5Hyperslab.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>[start filling in your text here]
    </ul>
    
    <h3>Synopsis</h3>

    The function

    \code
    herr_t H5Sselect_hyperslab (hid_t space_id,
                                H5S_seloper_t op,
				const hsize_t *start,
				const hsize_t *stride,
				const hsize_t *count,
				const hsize_t *block  ) 
    \endcode

    selects a hyperslab region to add to the current selected region for the
    dataspace specified by \e space_id.
    
    The \e start, \e stride, \e count, and \e block arrays must be the same size
    as the rank of the dataspace. For example, if the dataspace is 4-dimensional,
    each of these parameters must be a 1-dimensional array of size 4. 
    
    The selection operator op determines how the new selection is to be combined
    with the already existing selection for the dataspace. The following
    operators are supported:

    - H5S_SELECT_SET -- Replaces the existing selection with the parameters from
    this call. Overlapping blocks are not supported with this operator.
    - H5S_SELECT_OR -- Adds the new selection to the existing selection. (Binary OR)
    - H5S_SELECT_AND -- Retains only the overlapping portions of the new
    selection and the existing selection. (Binary AND)
    - H5S_SELECT_XOR -- Retains only the elements that are members of the new
    selection or the existing selection, excluding elements that are members of
    both selections. (Binary exclusive-OR, XOR)
    - H5S_SELECT_NOTB -- Retains only elements of the existing selection that are
    not in the new selection.
    - H5S_SELECT_NOTA -- Retains only elements of the new selection that are not
    in the existing selection.
    
    The start array specifies the offset of the starting element of the specified
    hyperslab.
    
    The stride array chooses array locations from the dataspace with each value
    in the stride array determining how many elements to move in each dimension.
    Setting a value in the stride array to 1 moves to each element in that
    dimension of the dataspace; setting a value of 2 in allocation in the stride
    array moves to every other element in that dimension of the dataspace. In
    other words, the stride determines the number of elements to move from the
    start location in each dimension. Stride values of 0 are not allowed. If the
    stride parameter is NULL, a contiguous hyperslab is selected (as if each value
    in the stride array were set to 1). 
    
    <h3>Example(s)</h3>
    
    \code
    H5Sselect_hyperslab (dataspaceID,
                         H5S_SELECT_SET,
			 offset,
			 NULL,
			 dimensions,
			 NULL)
    \endcode
    
  */  
  class HDF5Hyperslab {

    //! Shape of the dataset
    std::vector<hsize_t> shape_p;
    //! Offset of the starting element of the specified hyperslab
    std::vector<int> start_p;
    //! Number of elements to separate each element or block to be selected
    std::vector<int> stride_p;
    //! The number of elements or blocks to select along each dimension
    std::vector<int> count_p;
    //! The size of the block selected from the dataspace
    std::vector<int> block_p;
    //! Selection operator
    H5S_seloper_t selection_p;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    HDF5Hyperslab ();

    //! Argumented constructor
    HDF5Hyperslab (std::vector<int> const &start,
		   std::vector<int> const &stride,
		   std::vector<int> const &count,
		   std::vector<int> const &block,
		   H5S_seloper_t const &selection=H5S_SELECT_SET);
    /*!
      \brief Copy constructor
      
      \param other -- Another HDF5Hyperslab object from which to create this new
             one.
    */
    HDF5Hyperslab (HDF5Hyperslab const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~HDF5Hyperslab ();
    
    // === Operators ============================================================
    
    /*!
      \brief Overloading of the copy operator
      
      \param other -- Another HDF5Hyperslab object from which to make a copy.
    */
    HDF5Hyperslab& operator= (HDF5Hyperslab const &other); 
    
    // === Parameter access =====================================================

    //! Get the offset of the starting element of the specified hyperslab
    inline std::vector<int> start () const {
      return start_p;
    }

    //! Set the offset of the starting element of the specified hyperslab
    bool setStart (std::vector<int> const &start);

    inline H5S_seloper_t selection () const {
      return selection_p;
    }

    bool setselection (H5S_seloper_t const &selection);

    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, HDF5Hyperslab.
    */
    inline std::string className () const {
      return "HDF5Hyperslab";
    }

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    /*!
      \brief Provide a summary of the internal status

      \param os -- Output stream to which the summary is written.
    */
    void summary (std::ostream &os);    

    // === Methods ==============================================================
    
    
    
  private:
    
    //! Unconditional copying
    void copy (HDF5Hyperslab const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class HDF5Hyperslab -- end
  
} // Namespace DAL -- end

#endif /* HDF5HYPERSLAB_H */
  
