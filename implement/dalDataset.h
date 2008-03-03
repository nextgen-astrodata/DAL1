/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2007 by Joseph Masters                                  *
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

#ifndef DALDATASET_H
#define DALDATASET_H

#ifndef DAL_H
#include "dal.h"
#endif

#ifndef DALARRAY_H
#include "dalArray.h"
#endif

#ifndef DALTABLE_H
#include "dalTable.h"
#endif

#ifndef DALGROUP_H
#include "dalGroup.h"
#endif

#ifndef DALATTRIBUTE_H
#include "dalAttribute.h"
#endif

/*!
  \class dalDataset

  \ingroup DAL

  \brief Represents the file containing all sub-structures
         (tables, arrays, etc.)

  \author Joseph Masters

  <h3>Synopsis</h3>

  The dalDataset is the highest level container for dalData.  It may
  consist of one or more files on disk, each of which contain multiple tables, 
  images and attributes.  These tables and images can be gruoped.
*/
class dalDataset{

	void * file;  //!< can be HDF5File, FITS, MS
	std::string type;  //!< "HDF5", "MSCASA" or "FITS"; for example
	std::string name;  //!< dataset name
	std::vector<std::string> files;  //!< list of files
	std::vector<dalTable> tables; //!< list of tables
	std::vector<dalGroup> groups; //!< list of groups
	std::vector<dalAttribute> attrs;  //!< list of attributes
	
	dalFilter * filter;	//!< dataset filter

	hid_t h5fh;   //!< hdf5 file handle
	herr_t status;

#ifdef WITH_CASA
	casa::MeasurementSet * ms; //!< CASA measurement set pointer
	casa::MSReader * ms_reader; //!< CASA measurement set reader pointer
	casa::Vector<casa::String> ms_tables; //!< vector of CASA MS tables
#endif

  public:
  	/*!
	  \brief The dataset object constructor
	 */
  	dalDataset();

	/*!
	  \brief The dataset object destructor
	 */
	~dalDataset();
	
	/*!
	  \brief Another constructor with two arguments.
      \param name The name of the dataset to open.
	  \param filetype Type of file to open ("HDF5", "MSCASA", etc.).
	*/
  	dalDataset( const char * name, std::string filetype );
	
	/*!
	  \brief Open the dataset.
	  \return Zero if successful.  Non-zero on failure.
     */
	int open( const char * datasetname );

	/*!
	  \brief Close the dataset.
	  \return Zero if successful.  Non-zero on failure.
     */
	int close();

        /*!
          \brief Print the group attributes.

          Print the group attributes.
        */
	void getAttributes();

        /*!
          \brief Print the value of a specified attribute.

          Print the value of a specified attribute.

          \param attrname The name of the attribute you want to print.
        */
	void printAttribute( std::string attrname );

        /*!
          \brief Get the value of an attribute.

          Get the value of an attribute.  This is different from printAttribute
          because the value of the attribute is returned into a structure
          instead of simply printing.

          \param attrname The name of the attribute you want to retrieve.

          \return void * A pointer to the data in the attribute. 
        */
	void * getAttribute( std::string attrname );

        /*!
          \brief Define a string attribute.

          Define a string attribute.

          \param attrname The name of the attribute you want to create.
          \param data The value of the attribute you want to create.
        */
	void setAttribute_string( std::string attrname, std::string data );

        /*!
          \brief Define a string attribute.

          Define a string attribute.

          \param attrname The name of the attribute you want to create.
          \param data The values of the attributes you want to create.
        */
	void setAttribute_string( std::string attrname, std::vector<std::string> data );

        /*!
          \brief Define a integer attribute.

          Define a integer attribute.

          \param attrname The name of the attribute you want to create.
          \param data The value of the attribute you want to create.
          \param size Optional parameter specifying the array size of the
                      attribute.  Default is scalar.
        */
	void setAttribute_int( std::string attrname, int * data, int size=1 );

        /*!
          \brief Define an unsigned integer attribute.

          Define an unsigned integer attribute.

          \param attrname The name of the attribute you want to create.
          \param data The value of the attribute you want to create.
          \param size Optional parameter specifying the array size of the
                      attribute.  Default is scalar.
        */
	void setAttribute_uint(std::string attrname, unsigned int * data,int size=1);

        /*!
          \brief Define a double precision floating point attribute.

          Define a double precision floating point attribute.

          \param attrname The name of the attribute you want to create.
          \param data The value of the attribute you want to create.
          \param size Optional parameter specifying the array size of the
                      attribute.  Default is scalar.
        */
	void setAttribute_double( std::string attrname, double * data, int size=1 );

	/*!
	  \brief Create a new array in the root group.
	  \param arrayname The name of the array you want to create.
      \param data_object A pointer to a data object containing the intitial
						 data you want to include in the array.
	  \return dalArray * pointer to an array object.
     */
	dalArray * createArray(
				std::string arrayname,
				dalData * data_object);
				
	/*!
	  \brief Create a new static integer array in the root group.
	  \param arrayname The name of the array you want to create.
	  \param dims A vector containing the dimensions of the array.
	  \param data An array of integer values.
	  \return dalArray * pointer to an array object.
     */
	dalArray * createIntArray(
				std::string arrayname,
				std::vector<int> dims,
				int data[]);

	/*!
	  \brief Create a new extendible integer array in the root group.
	  \param arrayname The name of the array you want to create.
	  \param dims A vector containing the dimensions of the array.
	  \param data An array of integer values.
          \param cdims A vector a chunk dimensions (necessary for extending an
	               hdf5 dataset).
	  \return dalArray * pointer to an array object.
     */
	dalArray * createIntArray(
				std::string arrayname,
				std::vector<int> dims,
				int data[],
				std::vector<int>cdims);

	/*!
	  \brief Create a new extendible floating point array in the root group.
	  \param arrayname The name of the array you want to create.
	  \param dims A vector containing the dimensions of the array.
	  \param data An array of floating point values.
	  \param cdims A vector a chunk dimensions (necessary for extending an
	               hdf5 dataset).
	  \return dalArray * pointer to an array object.
     */
	dalArray * createFloatArray(
				std::string arrayname,
				std::vector<int> dims,
				float data[],
				std::vector<int>cdims);

	/*!
	  \brief Create a new extendible complex floating point array in the
	         root group.
	  \param arrayname The name of the array you want to create.
	  \param dims A vector containing the dimensions of the array.
	  \param data An array of complex floating point values.
	  \param cdims A vector a chunk dimensions (necessary for extending an
	               hdf5 dataset).
	  \return dalArray * pointer to an array object.
     */
	dalArray * createComplexArray(
				std::string arrayname,
				std::vector<int> dims,
				complex<float> data[],
				std::vector<int>cdims);

	/*!
	  \brief create a new table in the root group
	  \param tablename
	  \return dalTable
	*/
	dalTable * createTable( std::string tablename );

	/*!
	  \brief create a new table in a specified group
	  \param tablename 
	  \param groupname 
	  \return dalTable 
	*/
	dalTable * createTable( std::string tablename, std::string groupname );

	/*!
	  \brief Create a new group.

	  Create a new group.

	  \param groupname 
	  \return dalGroup 
	*/
	dalGroup * createGroup( char* groupname );

    /*!
	  \brief Open a table (that's not in a group) by name.

	  Open a table (that's not in a group) by name.

	  \param tablename The name of the table you want to open
	  \return dalTable * A pointer to a table object.
     */
	dalTable * openTable( std::string tablename );

    /*!
	  \brief Set table filter.

	  Set table filter.

	  \param columns A string containing a comma-separated list of columns to
	                 include in a filtered dataset.
	 */
	void setFilter( std::string columns );

    /*!
	  \brief Set table filter.

	  Set table filter.

	  \param columns A string containing a comma-separated list of columns to
	                 include in a filtered dataset.
	  \param conditions A string describing restraints on which rows are
	                 included in an opened dataset.  For example: "time>100".
	 */
	void setFilter( std::string columns, std::string conditions ); //!< set table filter
	
	/*!
	  \brief Open a table in a group.

	  Open a table in a group.

	  \param tablename The name of the table to open.
	  \param groupname The name of the group containing the table.
	  \return dalTable * A pointer to a table object.
	 */
	dalTable * openTable( std::string tablename, std::string groupname );

	/*!
	  \brief Open a array.

	  Open a array.

	  \param arrayname The name of the array to open.
	  \return dalArray * A pointer to a array object.
	 */
	dalArray * openArray( std::string arrayname );

	/*!
	  \brief Open an array in a group.

	  Open an array in a group.

	  \param arrayname The name of the array to open.
	  \param groupname The name of the group containing the array.
	  \return dalArray * A pointer to a array object.
	 */
	dalArray * openArray( std::string arrayname, std::string groupname );

	/*!
	  \brief Get a list of groups in the dataset.

	  Get a list of groups in the dataset.

	  \return A vector of group names.
	 */
	std::vector<std::string> getGroupNames();

	/*!
	  \brief Open a group in a dataset.

	  Open a group in a dataset.

	  \param groupname The name of the group to open
	  \return dalGroup * A pointer to a group object.
	 */
	dalGroup * openGroup( std::string groupname );
	
	/*!
	  \brief List the tables in a dataset.

	  List the tables in a dataset; keep in mind, that this operation only is
	  supported for data set of appropriate internal structure, such e.g. 
	  MeasurementSets.
	 */
	void listTables();

	/*!
	  \brief Retrieve the dataset type ("HDF5", "MSCASA", etc.).

	  Retrieve the dataset type ("HDF5", "MSCASA", etc.).

	  \return A string describing the file format ("HDF5", "MSCASA", etc.)
	*/
	std::string getType();

	/*!
	  \brief Retrieve the name of the data set

	  \return name -- A string holding the name of the data set.
	 */
	inline std::string getName () const {
	  return name;
	}

	/*!
	  \brief Retrieve the list of files

	  \return files -- List of files
	*/
	inline std::vector<std::string> getFiles () const {
	  return files;
	}

	/*!
	  \brief Get the HDF5 file handle identifier.

	  \return h5fh -- File handle identifier to communicate with the HDF5
	          file through the HDF5 library.
	 */
	inline hid_t getFileHandle () const {
	  return h5fh;
	}
	
	/*!
	  \brief Read TBB data.
	  
	  Read the transient buffer board (TBB) data out of a file that is in
	  a predefined format.  This format is described in the Time Series
	  Interface Control Document.
	  
	  \param id A unique identifier for a particular antenna.  The identifier
		    is a string that is 9 characters long.  From left to right,
		    the first three characters represent the station id.  The next
		    three characters represent the rsp id.  The last three
		    characters represent the rcu id.  For example, if the station
		    number is 11, the rsp id is 5 and the rcu id is 6, the id
		    string will be "011005006".
	  \param start The starting position to read from within the tbb for a
	               certain antenna.
	  \param length The number of samples to read from the tbb antenna data,
	                starting at the specified start value.
	  \return A pointer to a structure containing the block of data.
	 */
	void read_tbb(std::string id, int start, int length, short data_out[]);

/************************************************************************
 *
 * The following functions are boost wrappers to allow some previously
 *   defined functions to be easily called from a python prompt.
 *
 ************************************************************************/
#ifdef PYTHON

	// create[]Array wrappers
	dalArray * cia_boost1(std::string arrayname, bpl::list dims, bpl::list data);
	dalArray * cia_boost2(std::string arrayname, bpl::list dims, bpl::list data,
				 bpl::list cdims );
	dalArray * cia_boost_numarray1( std::string arrayname, bpl::list dims,
			bpl::numeric::array data );
	dalArray * cia_boost_numarray2( std::string arrayname, bpl::list dims,
			bpl::numeric::array data, bpl::list cdims );
	dalArray * cfa_boost( std::string arrayname, bpl::list dims, bpl::list data,
				 bpl::list cdims );
	dalArray * cfa_boost_numarray( std::string arrayname, bpl::list dims,
			 bpl::numeric::array data, bpl::list cdims );

	bpl::numeric::array ria_boost( std::string arrayname );
	bpl::numeric::array rfa_boost( std::string arrayname );

	// createTable wrappers
	dalTable * ct1_boost( std::string tablename );
	dalTable * ct2_boost( std::string tablename, std::string groupname );

	// openTable wrappers
	dalTable * ot1_boost( std::string tablename );
	dalTable * ot2_boost( std::string tablename, std::string groupname );

	void setFilter_boost1(std::string);
	void setFilter_boost2(std::string,std::string);
#endif
};
#endif
