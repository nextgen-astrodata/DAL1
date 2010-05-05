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

#define H5S_HYPERSLAB_MAX 4294967296

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class HDF5Hyperslab
    
    \ingroup DAL
    \ingroup data_common
    
    \brief A hyperslab region for selective access to a dataspace
    
    \author Lars B&auml;hren

    \date 2010/02/09

    \test tHDF5Hyperslab.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li><a href="http://www.hdfgroup.org/HDF5/doc/UG/UG_frame12Dataspaces.html">HDF5
      Dataspaces and Partial I/O</a>
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
    dataspace specified by \b space_id.
    
    The \b start, \b stride, \b count, and \b block arrays must be the same size
    as the rank of the dataspace. For example, if the dataspace is 4-dimensional,
    each of these parameters must be a 1-dimensional array of size 4. 
    
    The selection operator \b op determines how the new selection is to be
    combined with the already existing selection for the dataspace. The following
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
    
    The \b start array specifies the offset of the starting element of the specified
    hyperslab.
    
    The \b stride array chooses array locations from the dataspace with each value
    in the \b stride array determining how many elements to move in each dimension.
    Setting a value in the \b stride array to 1 moves to each element in that
    dimension of the dataspace; setting a value of 2 in allocation in the stride
    array moves to every other element in that dimension of the dataspace. In
    other words, the stride determines the number of elements to move from the
    start location in each dimension. Stride values of 0 are not allowed. If the
    stride parameter is NULL, a contiguous hyperslab is selected (as if each value
    in the stride array were set to 1).

    The \b count array determines how many blocks to select from the dataspace,
    in each dimension.

    The \b block array determines the size of the element block selected from the
    dataspace. If the block parameter is set to NULL, the block size defaults to
    a single element in each dimension (as if each value in the block array were
    set to 1).

    For example, consider a 2-dimensional dataspace with hyperslab selection
    settings as follows:

    - the start offset is specified as [1,1],
    - stride is [4,4],
    - count is [3,7],
    - and block is [2,2].

    In C, these settings will specify a hyperslab consisting of 21 2x2 blocks of
    array elements starting with location (1,1) with the selected blocks at
    locations (1,1), (5,1), (9,1), (1,5), (5,5), etc.; in Fortran, they will
    specify a hyperslab consisting of 21 2x2 blocks of array elements starting
    with location (2,2) with the selected blocks at locations (2,2), (6,2),
    (10,2), (2,6), (6,6), etc. 

    <b>Rules for Defining Selections</b>

    A selection must have the same number of dimensions (rank) as the dataspace
    it is applied to, although it may select from only a small region, e.g., a
    plane from a 3D dataspace. Selections do not affect the extent of the
    dataspace, the selection may be larger than the dataspace. The boundaries of
    selections are reconciled with the extent at the time of the data transfer.

    <b>Data Transfer with Selections</b>
    
    A data transfer (read or write) with selections is the same as any read or
    write, except the source and destination dataspace have compatible selections.
    
    During the data transfer, the following steps are executed by the library.
    
    <ol>
      <li>The source and destination dataspaces are checked to assure that the
      selections are compatible.
      <ol>
        <li>Each selection must be within the current extent of the dataspace.
	A selection may be defined to extend outside the current extent of the
	dataspace, but the dataspace cannot be accessed if the selection is not
	valid at the time of the access.
	<li>The total number of points selected in the source and destination
	must be the same. Note that the dimensionality of the source and
	destination can be different (e.g., the source could be 2D, the destination
	1D or 3D), and the shape can be different, but the number of elements
	selected must be the same.
      </ol>
      <li>The data is transferred, element by element.
    </ol>
    
    Selections have an iteration order for the points selected, which can be any
    permutation of the dimensions involved (defaulting to 'C' array order) or a
    specific order for the selected points, for selections composed of single
    array elements with H5Sselect_elements.
    
    The elements of the selections are transferred in row-major, or C order. That
    is, it is assumed that the first dimension varies slowest, the second next
    slowest, and so forth. For hyperslab selections, the order can be any
    permutation of the dimensions involved (defaulting to 'C' array order). When
    multiple hyperslabs are combined, the hyperslabs are coalesced into contiguous
    reads and writes
    
    In the case of point selections, the points are read and written in the order
    specified.

    <table border=0>
      <tr align=center>
        <td width=20%><img src="images/Hyperslab_2D_row.png"></td>
        <td width=20%><img src="images/Hyperslab_2D_column.png"></td>
        <td width=20%><img src="images/Hyperslab_2D_sub-array.png"></td>
        <td width=20%><img src="images/Hyperslab_2D_sub-arrays.png"></td>
      </tr>
      <tr valign=top>
        <td><b>Fig 1.</b><br> Read/write by row.</td>
        <td><b>Fig 2.</b><br> Read/write by column</td>
        <td><b>Fig 3.</b><br> Read/write sub-array of same rank.</td>
        <td><b>Fig 4.</b><br> Read/write group of sub-arrays of same rank.</td>
      </tr>
    </table>

    <h3>Example(s)</h3>

    <ol>
      <li>Simplest Hyperslab: consider all of the dataspace to be selected:
      \code
      std::vector<hsize_t> shape(2);

      // Set the shape of the dataset, i.e. the number of elements along the axes
      shape[0] = 1024;
      shape[1] = 200;

      // create Hyperslab object
      HDF5Hyperslab slab (shape);
      \endcode

      <li>Create a Hyperslab which selects all the entries along the first axis:
      \code
      int rank (2);
      std::vector<hsize_t> shape(rank);
      std::vector<int> start (rank);
      std::vector<int> block (rank);

      // Set the shape of the dataset, i.e. the number of elements along the axes
      shape[0] = 1024;
      shape[1] = 200;
      // Set the starting point
      start[0] = 0;
      start[1] = 0;
      // Set the shape of the elements block read from the data
      block[0] = shape[0];
      block[1] = 1;

      // create Hyperslab object
      HDF5Hyperslab slab (shape,start,block);
      \endcode

      <li>Access the \e row of a two-dimension dataset (Fig. 1):
      \code
      // Row of the array to access
      unsigned int row (10);
      // Get shape of the array
      std::vector<hsize_t> shape = slab.shape();

      start[0] = row;
      start[1] = 0;
      count[0] = 1;
      count[1] = 1;
      block[0] = 1;
      block[1] = shape[1];

      slab.setStart (start);
      slab.setCount (count);
      slab.setBlock (block);
      \endcode

      <li>Access the \e column of a two-dimension dataset (Fig. 2):
      \code
      // Column of the array to access
      unsigned int col (10);
      // Get shape of the array
      std::vector<hsize_t> shape = slab.shape();

      start[0] = 0;
      start[1] = col;
      count[0] = 1;
      count[1] = 1;
      block[0] = shape[0];
      block[1] = 1;

      slab.setStart (start);
      slab.setCount (count);
      slab.setBlock (block);
      \endcode
    </ol>
    
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
    HDF5Hyperslab (std::vector<hsize_t> const &shape);

    //! Argumented constructor
    HDF5Hyperslab (std::vector<hsize_t> const &shape,
		   std::vector<int> const &start,
		   std::vector<int> const &block,
		   H5S_seloper_t const &selection=H5S_SELECT_SET);

    //! Argumented constructor
    HDF5Hyperslab (std::vector<hsize_t> const &shape,
		   std::vector<int> const &start,
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

    //! Get the shape of the array to which the hyperslab is applied
    inline std::vector<hsize_t> shape () const {
      return shape_p;
    }

    //! Get the offset of the starting element of the specified hyperslab
    inline std::vector<int> start () const {
      return start_p;
    }

    //! Set the offset of the starting element of the specified hyperslab
    bool setStart (std::vector<int> const &start,
		   bool const &ignoreShape=false);

    //! Get the number of elements to separate each element or block to be selected
    inline std::vector<int> stride () const {
      return stride_p;
    }

    //! Set the number of elements to separate each element or block to be selected
    bool setStride (std::vector<int> const &stride);
    
    //! Get the number of elements or blocks to select along each dimension
    inline std::vector<int> count () const {
      return count_p;
    }

    //! Set the number of elements or blocks to select along each dimension
    bool setCount (std::vector<int> const &count);
    
    //! Get the size of the element block selected from the dataspace
    inline std::vector<int> block () const {
      return block_p;
    }

    //! Set the size of the element block selected from the dataspace
    bool setBlock (std::vector<int> const &block,
		   bool const &ignoreShape=false);

    //! Get the size of the gap between two subsequent blocks
    std::vector<int> gap ();

    //! Set the size of the gap between two subsequent blocks
    bool setGap (std::vector<int> const &gap);

    //! Get the selection operator
    inline H5S_seloper_t selection () const {
      return selection_p;
    }
    
    //! Set the selection operator
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
    
    //! Get the number of data points returned for the given Hyperslab
    unsigned int nofDatapoints ();

    //! Get the number of data points returned for a given Hyperslab
    static unsigned int nofDatapoints (std::vector<int> const &count,
				       std::vector<int> const &block);

    //! Get the offset of the last element of the specified hyperslab.
    std::vector<hsize_t> end ();
    
    //! Get the offset of the last element of the specified hyperslab.
    static std::vector<hsize_t> end (std::vector<int> const &start,
				     std::vector<int> const &stride,
				     std::vector<int> const &count,
				     std::vector<int> const &block);
    
    //! Set the Hyperslab for the dataspace attached to a dataset
    bool setHyperslab (hid_t const &location,
		       bool const &resizeDataset);

    //! Set the Hyperslab for the dataspace attached to a dataset
    static bool setHyperslab (hid_t const &location,
			      H5S_seloper_t const &selection,
			      std::vector<int> const &start,
			      std::vector<int> const &block,
			      bool const &resizeDataset);
    
    //! Set the Hyperslab for the dataspace attached to a dataset
    static bool setHyperslab (hid_t const &location,
			      H5S_seloper_t const &selection,
			      std::vector<int> const &start,
			      std::vector<int> const &stride,
			      std::vector<int> const &count,
			      std::vector<int> const &block,
			      bool const &resizeDataset);

  private:
    
    //! Initialize the internal parameters
    void init ();
    
    //! Unconditional copying
    void copy (HDF5Hyperslab const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class HDF5Hyperslab -- end
  
} // Namespace DAL -- end

#endif /* HDF5HYPERSLAB_H */
  
