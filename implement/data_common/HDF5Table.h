/*-------------------------------------------------------------------------*
 | $Id:: NewClass.h 3888 2009-12-16 15:40:46Z baehren                    $ |
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

#ifndef HDF5TABLE_H
#define HDF5TABLE_H

// Standard library header files
#include <iostream>
#include <string>

#include <dalCommon.h>
#include <CommonInterface.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class HDF5Table
    
    \ingroup DAL
    \ingroup data_common
    
    \brief Brief description for class HDF5Table
    
    \author Lars B&auml;hren

    \date 2010/01/20

    \test tHDF5Table.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>HDF5 User's guide, Section 2.7,  Creating and defining compound
      datatypes
    </ul>

    A composite datatype is an aggregation of one or more datatypes. Each class
    of composite datatypes a properties that describe the organization of the 
    composite datatype. Composite datatypes include:
    - Compound datatypes: structured records
    - Array: a multidimensional array of a datatype.
    - Variable length: a one-dimensional array of a datatype
    
    An HDF5 compound datatype is similar to a C struct or a Fortran common block.
    Though not originally designed with databases in mind, HDF5 compound
    datatypes are sometimes used in a manner analogous to a database record.

    HDF5 defines a compound datatype as a collection of one or more data elements.
    Each elements is an atomic type, a small array, or another compound datatype.
    The provision for nested compound datatypes allows these structures become
    quite complex. Compound datatypes thus become either a powerful tool or a
    complex and difficult to debug construct; reasonable caution is advised.
    
    To create and use a compound datatype, you need to create a datatype with
    class compound (<tt>H5T_COMPOUND</tt>) and specify the total size of the
    data element in bytes. A compound datatype consists of zero or more uniquely
    named members. Members can be defined in any order but must occupy
    non-overlapping regions within the datum.

    - \b Index -- An index number between zero and N-1, where N is the number of
    members in the compound. The elements are indexed in the order of their
    location in the array of bytes.
    - \b Name -- A string that must be unique within the members of the same
    datatype.
    - \b Datatype -- An HDF5 datatype.
    - \b Offset -- A fixed byte offset, which defines the location of the first
    byte of that member in the compound datatype.

    Properties of the members of a compound datatype are defined when the member
    is added to the compound type and cannot be subsequently modified.
    
    <b>Defining compound datatypes.</b>

    Compound datatypes must be built out of other datatypes. First, one creates
    an empty compound datatype and specifies its total size. Members are then added
    to the compound datatype in any order. 
    
    Each member must have a descriptive name, which is the key used to uniquely
    identify the member within the compound datatype. A member name in an HDF5
    datatype does not necessarily have to be the same as the name of the
    corresponding member in the C struct in memory, although this is often the
    case. Nor does one need to define all members of the C struct in the HDF5
    compound datatype (or vice versa). 

    Usually a C struct will be defined to hold a data point in memory, and the
    offsets of the members in memory will be the offsets of the struct members
    from the beginning of an instance of the struct. The library defines the
    macro that computes the offset of member m within a struct variable
    <tt>s</tt>.: 
    \code
    HOFFSET(s,m) 
    \endcode

    <h3>Synopsis</h3>

    Encapsulated HDF5 library functions:
    
    <ol>
      <li>\b H5TBmake_table creates and writes a dataset named \e table_name
      attached to the object specified by the identifier \e loc_id. 
      \code
      herr_t H5TBmake_table (const char *table_title,
                             hid_t loc_id,
			     const char *dset_name,
			     hsize_t nfields,
			     const hsize_t nrecords,
			     size_t type_size,
			     const char *field_names [ ],
			     const size_t *field_offset,
			     const hid_t *field_types,
			     hsize_t chunk_size,
			     void *fill_data,
			     int compress,
			     const void *data) 
      \endcode

      <li>\b H5TBread_table reads a table named \e table_name attached to the
      object specified by the identifier \e loc_id.
      \code
      herr_t H5TBread_table (hid_t  loc_id,
                             const char *table_name,
			     size_t  dst_size,
			     const size_t *dst_offset,
			     const size_t  *dst_sizes,
			     void  *dst_buf)
      \endcode
      
      <li>\b H5TBread_records reads some records identified from a dataset named
      \e table_name attached to the object specified by the identifier \e loc_id.
      \code
      herr_t H5TBread_records (hid_t  loc_id,
                               const char *table_name,
			       hsize_t start,
			       hsize_t nrecords,
			       size_t type_size,
			       const size_t *field_offset,
			       const size_t *dst_sizes,
			       void  *data)
      \endcode

      <li>\b H5TBread_fields_name reads the fields identified by \e field_names
      from a dataset named \e table_name attached to the object specified by the
      identifier \e loc_id.
      \code
      herr_t H5TBread_fields_name (hid_t loc_id,
                                   const char *table_name,
				   const char * field_names,
				   hsize_t start,
				   hsize_t nrecords,
				   size_t type_size,
				   const size_t *field_offset,
				   const size_t *dst_sizes,
				   void *data)
      \endcode
      Parameters:
      - hid_t loc_id [IN] Identifier of the file or group to read the table within. 
      - const char *table_name [IN] The name of the dataset to read. 
      - const char * field_names [IN] An array containing the names of the fields
      to read. 
      - hsize_t  start [IN] The start record to read from. 
      - hsize_t nrecords [IN] The number of records to read. 
      - hsize_t type_size [IN] The size in bytes of the structure associated with
      the table. This value is obtained with sizeof. 
      - const size_t *field_offset [IN] An array containing the offsets of the fields.
      - const size_t *dst_sizes [IN] An array containing the size in bytes of the
      fields. 
      - void *data [OUT] Buffer with data. 

      <li>\b H5TBread_fields_index reads the fields identified by \e field_index
      from a dataset named \e table_name attached to the object specified by the
      identifier \e loc_id.

      <li>\b H5TBget_dimensions retrieves the table dimensions from a dataset
      named \e table_name attached to the object specified by the identifier
      \e loc_id.
      \code
      herr_t H5TBget_table_info (hid_t  loc_id,
                                 const char *table_name,
				 hsize_t *nfields,
				 hsize_t *nrecords)
      \endcode

      <li>\b H5TBget_field_info gets information about a dataset named
      \e table_name  attached to the object specified by the identifier
      \e loc_id.
      \code
      herr_t H5TBget_field_info (hid_t  loc_id,
                                 const char *table_name,
				 char *field_names[],
				 size_t  *field_sizes,
				 size_t *field_offsets,
				 size_t *type_size)
      \endcode
    </ol>

    <h3>Example(s)</h3>
    
  */  
  class HDF5Table {

    //! Names of the table columns
    std::vector<std::string> columnNames_p;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    HDF5Table ();
    
    //! Argumented constructor
    HDF5Table (hid_t const &location);
    
    // === Destruction ==========================================================

    //! Destructor
    ~HDF5Table ();
    
    // === Parameter access =====================================================

    //! Get the names of the table columns
    inline std::vector<std::string> columnNames () const {
      return columnNames_p;
    }

    //! Get the number of table columns
    inline unsigned int nofColumns () {
      return columnNames_p.size();
    }
    
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, HDF5Table.
    */
    inline std::string className () const {
      return "HDF5Table";
    }

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);    

    // === Methods ==============================================================
    
    
    
  private:

    //! Initialize the internal parameters/data
    void init ();
    
    //! Unconditional copying
    void copy (HDF5Table const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class HDF5Table -- end
  
} // Namespace DAL -- end

#endif /* HDF5TABLE_H */
  
