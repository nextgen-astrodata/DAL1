/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2006 by Joseph Masters                                  *
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

#ifdef PYTHON
#define PY_ARRAY_UNIQUE_SYMBOL PyArrayHandle
#define NO_IMPORT_ARRAY
#endif

#ifndef DALDATASET_H
#include "dalDataset.h"
#endif

namespace DAL {
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  // ----------------------------------------------------------------  dalDataset
  
  dalDataset::dalDataset()
  {
    init();
  }

  // ----------------------------------------------------------------  dalDataset

  /*!
    \param filename  -- The name of the dataset/file to open.
    \param filetype  -- Type of file to open ("HDF5", "MSCASA", etc.).
    \param overwrite -- Overwrite existing file if one already exists for name
           \e filename. By default an already existing file is kept and only
	   opened -- if you want to overwrite use <tt>overwrite=true</tt>
  */
  dalDataset::dalDataset( const char * filename,
                          std::string filetype,
                          const bool &overwrite)
  {
    init (filename,
	  filetype,
	  overwrite);
    
    if ( filetype == H5TYPE ) {
      /*
       * Check if the provided name belongs to an already existing dataset;
       * if this is the case, open the dataset instead of blindly creating
       * it (and thereby potentially overwriting the existing one).
       */
      if (overwrite_p)
	{
	  /* Directly try to create the dataset */
	  if ( ( h5fh_p = H5Fcreate( filename,
				     H5F_ACC_TRUNC,
				     H5P_DEFAULT,
				     H5P_DEFAULT ) ) < 0 )
	    {
	      std::cerr << "ERROR: Could not create file '" << filename << "'."
			<< std::endl;
	    }
	}
      else
	{
	  FILE * pFile;
	  pFile = fopen ( filename, "r" );
	  if ( pFile == NULL )  /* check to see if the file exists */
	    {
	      /* if not, create it */
	      if ( ( h5fh_p = H5Fcreate( filename,
					 H5F_ACC_TRUNC,
					 H5P_DEFAULT,
					 H5P_DEFAULT ) ) < 0 )
		std::cerr << "ERROR: Could not create file '" << filename << "'.\n";
	    }
	  else  /* if it does exist, try to reopen it as a hdf5 file */
	    {
	      if ( 0 != fclose( pFile ) )
		std::cerr << "ERROR: Could not close file '" << filename << "'.\n";
	      
	      /* if it does reopen it as an hdf5 file */
	      if ( ( h5fh_p = H5Fopen(filename, H5F_ACC_RDWR, H5P_DEFAULT ) ) > 0 )
		std::cerr << "SUCCESS: Opened file '" << filename << "'.\n";
	      else
		{
		  std::cerr << "ERROR: There was a problem opening the file '"
			    << filename << "'.\n";
		  exit(9);
		}
	    }
	  
	  file = &h5fh_p;
	}
    }
    else if ( filetype == FITSTYPE )
      {
#ifdef HAVE_CFITSIO
        fitsfile *fptr; /* pointer to the FITS file; defined in fitsio.h */
        int status;
        fits_create_file(&fptr, filename, &status); /*create new file*/
#else
        std::cerr << "ERROR: CFITSIO libraries not found.\n";
#endif
      }
    else
      {
        std::cerr << "(dalDataset::dalDataset) Data format \'" << type
                  << "\' not supported for this operation." << endl;
      }
  }
  
  //_____________________________________________________________________________
  //                                                                         init
  
  void dalDataset::init()
  {
    init ("UNDEFINED",
	  "UNDEFINED",
	  false);
  }

  //_____________________________________________________________________________
  //                                                                         init
  
  /*!
    \param filename  -- The name of the dataset/file to open.
    \param filetype  -- Type of file to open ("HDF5", "MSCASA", etc.).
    \param overwrite -- Overwrite existing file if one already exists for name
           \e filename. By default an already existing file is kept and only
	   opened -- if you want to overwrite use <tt>overwrite=true</tt>
  */
  void dalDataset::init (const char * filename,
			 std::string filetype,
			 const bool &overwrite)
  {
#ifdef PYTHON
    Py_Initialize();
#endif

#ifdef HAVE_CASA
    ms        = NULL;
    ms_reader = NULL;
#endif

    file        = NULL;
    name        = stringify( filename );
    type        = filetype;
    overwrite_p = overwrite;
    h5fh_p      = 0;
  }

  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void dalDataset::summary (std::ostream &os)
  {
    os << "[dalDataset] Summary of object properties" << std::endl;

    os << "-- Dataset type     = " << getType()       << std::endl;
    os << "-- Dataset name     = " << getName()       << std::endl;
    os << "-- HDF5 file handle = " << getFileHandle() << std::endl;
    os << "-- HDF5 group ID    = " << getId()         << std::endl;

    /* Further properties only can be requested if the object is connected
     * to a dataset.
     */

    if (getName() != "UNDEFINED") {
      std::vector<std::string> groupNames = getGroupNames();
      
      os << "-- Group names      = [";
      for (unsigned int n(0); n<groupNames.size(); n++) {
	os << " " << groupNames[n];
      }
      os << " ]" << std::endl;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                     openFITS

  /*!
    \param filename  -- The name of the dataset/file to open.
  */  
  bool dalDataset::openFITS ( const char * fname )
  {
    
#ifdef HAVE_CFITSIO
    
    fitsfile * fptr = NULL;
    int status      = 0;  /* MUST initialize status */
    int nkeys       = 0;
    int ii          = 0;
    char card [500 ];
    memset( card, '\0', 500);
    
    fits_open_file(&fptr, fname, READONLY, &status);
    
    /* print any error messages */
    if (status) {
      return DAL::FAIL;
    }
    else {
      fits_get_hdrspace(fptr, &nkeys, NULL, &status);
      
      for (ii = 1; ii <= nkeys; ii++) {
	fits_read_record(fptr, ii, card, &status); /* read keyword */
	printf("%s\n", card);
      }
      
      fits_close_file(fptr, &status);
      
      return DAL::SUCCESS;
    }
#else
    
    fname = fname; // to get rid of unused var compiler warning
    return DAL::FAIL;

#endif

  }

  //_____________________________________________________________________________
  //                                                                     openHDF5
  
  /*!
    \param filename  -- The name of the dataset/file to open.
  */  
  hid_t dalDataset::openHDF5 (const char * fname)
  {
    // the following returns an integer file handle

    hid_t fh = 0;  // file handle

    // Turn off error reporting since we expect failure in cases
    //   where the file is not hdf5
    H5Eset_auto1(NULL, NULL);

    /* return -1 if the file is not recognized as hdf5 */
    if ( H5Fis_hdf5( fname ) < 1 )  // hdf5 file returns a positive int
      {
        return -1;
      }

    // the following returns an integer file handle
    if ( ( fh = H5Fopen(fname, H5F_ACC_RDWR, H5P_DEFAULT ) ) < 0 )
      {
        std::cerr << "Could not open " << fname
                  << " as read-write.  Trying as read-only." << endl;
        if ( ( fh = H5Fopen(fname, H5F_ACC_RDONLY, H5P_DEFAULT ) ) < 0 )
          return -1;
      }
    else
      {
        return fh;
      }

    return fh;
  }

  //_____________________________________________________________________________
  //                                                                         open

  /*!
    \param filename The name of the file to open.
    \return bool -- DAL::FAIL or DAL::SUCCESS
  */
  bool dalDataset::open (const char * fname)
  {
    std::string lcltype;
    
    if ( DAL::SUCCESS == openFITS( fname ) ) {
      // store the type of the dataset
      lcltype = FITSTYPE;
      type    = lcltype;
      // report successful opening of file
      std::cerr << lcltype << " file opened, but other FITS operations are not "
		<< "yet supported.  Sorry." << endl;
      return DAL::SUCCESS;
    }
    else if ( (h5fh_p = openHDF5( fname )) >= 0 )
      {
        file = &h5fh_p;
        lcltype = H5TYPE;
        type = lcltype;
        name = fname;
        return DAL::SUCCESS;
      }
    else
      {
#ifdef HAVE_CASA
        try
          {
            lcltype = MSCASATYPE;
            type = lcltype;
            casa::File msfile( fname );
            // first treat it as a symbolic link
            if ( msfile.isSymLink() )
              {
                casa::SymLink link( msfile );
                casa::Path realFileName = link.followSymLink();
                ms = new casa::MeasurementSet( realFileName.absoluteName() );
                file = &ms;
                ms_reader = new casa::MSReader( *ms );
                name = fname;
                return DAL::SUCCESS;
              }
            else // treat it as a regular file
              {
                ms = new casa::MeasurementSet( fname );
                file = &ms;
                ms_reader = new casa::MSReader( *ms );
                name = fname;
                return DAL::SUCCESS;
              }
          }
        catch (casa::AipsError x)
          {
            return DAL::FAIL;
          }
#else
        return DAL::FAIL;
#endif
      }
  }

  //_____________________________________________________________________________
  //                                                                        close

  /*!
    \return bool -- DAL::FAIL or DAL::SUCCESS
   */
  bool dalDataset::close()
  {
    if ( type == MSCASATYPE )
      {
#ifdef HAVE_CASA
        delete ms;
        return DAL::SUCCESS;
#else
        std::cerr << "ERROR: CASA support not enabled.\n";
        return DAL::FAIL;
#endif
      }
    else if ( type == H5TYPE )
      {
        if ( H5Fclose( h5fh_p ) < 0 )
          return DAL::FAIL;
        else
          return DAL::SUCCESS;
      }
    return DAL::FAIL;
  }

  //_____________________________________________________________________________
  //                                                                getAttributes

  /*!
    \brief Print the group attributes.

    Print the group attributes.

    \return bool -- DAL::FAIL or DAL::SUCCESS
  */
  bool dalDataset::getAttributes()
  {
    if ( H5Aiterate1( h5fh_p, NULL, attr_info, NULL ) < 0 )
      {
        std::cerr << "ERROR: Could not iterate over dataset attributes.\n";
        return DAL::FAIL;
      }
    else {
      return DAL::SUCCESS;
    }

  }
  
  //_____________________________________________________________________________
  //                                                                 setAttribute

  /*!
    \param attrname The name of the attribute you want to create.
    \param data The value of the attribute you want to create.
    \param size Optional parameter specifying the array size of the
                attribute.  Default is scalar.
    \return bool -- DAL::FAIL or DAL::SUCCESS
  */
  bool dalDataset::setAttribute( std::string attrname,
                                 const char * data,
                                 int size )
  {
    return h5set_attribute( H5T_NATIVE_CHAR, h5fh_p, attrname, data, size );
  }

  //_____________________________________________________________________________
  //                                                                 setAttribute

  /*!
    \param attrname The name of the attribute you want to create.
    \param data The value of the attribute you want to create.
    \param size Optional parameter specifying the array size of the
                attribute.  Default is scalar.
    \return bool -- DAL::FAIL or DAL::SUCCESS
  */
  bool dalDataset::setAttribute( std::string attrname,
                                 const short * data,
                                 int size )
  {
    return h5set_attribute( H5T_NATIVE_SHORT, h5fh_p, attrname, data, size );
  }

  //_____________________________________________________________________________
  //                                                                 setAttribute

  /*!
    \param attrname The name of the attribute you want to create.
    \param data The value of the attribute you want to create.
    \param size Optional parameter specifying the array size of the
                attribute.  Default is scalar.
    \return bool -- DAL::FAIL or DAL::SUCCESS
  */
  bool dalDataset::setAttribute( std::string attrname,
                                 const int * data,
                                 int size )
  {
    return h5set_attribute( H5T_NATIVE_INT, h5fh_p, attrname, data, size );
  }

  //_____________________________________________________________________________
  //                                                                 setAttribute

  /*!
    \param attrname The name of the attribute you want to create.
    \param data The value of the attribute you want to create.
    \param size Optional parameter specifying the array size of the
                attribute.  Default is scalar.
    \return bool -- DAL::FAIL or DAL::SUCCESS
  */
#ifndef WORDSIZE_IS_64
  bool dalDataset::setAttribute( std::string attrname,
                                 const int64_t * data,
                                 int size )
  {
#ifdef HAVE_LONG_LONG
    return h5set_attribute( H5T_NATIVE_LLONG, h5fh_p, attrname, data, size );
#else
    return h5set_attribute( H5T_NATIVE_LONG, h5fh_p, attrname, data, size );
#endif
  }
#else
  bool dalDataset::setAttribute( std::string attrname,
                                 const long * data,
                                 int size )
  {
    return h5set_attribute( H5T_NATIVE_LONG, h5fh_p, attrname, data, size );
  }
#endif

  //_____________________________________________________________________________
  //                                                                 setAttribute

  /*!
    \param attrname The name of the attribute you want to create.
    \param data The value of the attribute you want to create.
    \param size Optional parameter specifying the array size of the
                attribute.  Default is scalar.
    \return bool -- DAL::FAIL or DAL::SUCCESS
  */
  bool dalDataset::setAttribute( std::string attrname,
                                 const uint * data,
                                 int size )
  {
    return h5set_attribute( H5T_NATIVE_UINT, h5fh_p, attrname, data, size );
  }

  //_____________________________________________________________________________
  //                                                                 setAttribute

  /*!
    \param attrname The name of the attribute you want to create.
    \param data The value of the attribute you want to create.
    \param size Optional parameter specifying the array size of the
                attribute.  Default is scalar.
    \return bool -- DAL::FAIL or DAL::SUCCESS
  */
  bool dalDataset::setAttribute( std::string attrname,
                                 const float * data,
                                 int size )
  {
    return h5set_attribute( H5T_NATIVE_FLOAT, h5fh_p, attrname, data, size );
  }

  //_____________________________________________________________________________
  //                                                                 setAttribute

  /*!
    \param attrname -- The name of the attribute you want to create.
    \param data     -- The value of the attribute you want to create.
    \param size     -- Optional parameter specifying the array size of the
           attribute. Default is scalar.
    \return bool    -- DAL::FAIL or DAL::SUCCESS
  */
  bool dalDataset::setAttribute( std::string attrname,
                                 const double * data,
                                 int size )
  {
    return h5set_attribute( H5T_NATIVE_DOUBLE, h5fh_p, attrname, data, size );
  }

  //_____________________________________________________________________________
  //                                                                 setAttribute

  /*!
    \param attrname -- The name of the attribute you want to create.
    \param data     -- The value of the attribute you want to create.
    \return bool    -- DAL::FAIL or DAL::SUCCESS
  */
  bool dalDataset::setAttribute( std::string attrname,
                                 std::string data )
  {
    return h5setAttribute_string( h5fh_p, attrname, &data, 1 );
  }

  //_____________________________________________________________________________
  //                                                                 setAttribute

  /*!
    \param attrname -- The name of the attribute you want to create.
    \param data     -- The value of the attribute you want to create.
    \return bool    -- DAL::FAIL or DAL::SUCCESS
  */
  bool dalDataset::setAttribute( std::string attrname,
                                 const std::string * data,
                                 int size )
  {
    return h5setAttribute_string ( h5fh_p, attrname, data, size );
  }

  //_____________________________________________________________________________
  //                                                          setAttribute_string

  /*!
    \brief Define a string attribute.

    Define a string attribute.

    \param attrname The name of the attribute you want to create.
    \param data The values of the attributes you want to create.
    \return bool -- DAL::FAIL or DAL::SUCCESS
  */
  bool dalDataset::setAttribute_string( std::string attrname,
                                        std::vector<std::string> data )
  {
    hid_t aid, atype, att;
    int size = data.size();

    const char *string_att[ size ];
    for (int ii=0; ii<size; ii++)
      string_att[ii] = (data[ii]).c_str();

    hsize_t dims[] = { size };

    if ( ( aid  = H5Screate_simple (1, dims, NULL) ) < 0 )
      {
        std::cerr << "ERROR: Could not create dataspace.\n";
        return DAL::FAIL;
      }

    if ( ( atype = H5Tcopy(H5T_C_S1) ) < 0 )
      {
        std::cerr << "ERROR: Could not copy attribute datatype.\n";
        return DAL::FAIL;
      }

    if ( H5Tset_size( atype, H5T_VARIABLE ) < 0 )
      {
        std::cerr << "ERROR: Could not set attribute size.\n";
        return DAL::FAIL;
      }

    hid_t root;
    if ( ( root = H5Gopen1( h5fh_p, "/") ) < 0 )
      {
        std::cerr << "ERROR: Could not open group.\n";
        return DAL::FAIL;
      }

    if ( ( att = H5Acreate1(root, attrname.c_str(), atype, aid, H5P_DEFAULT) )
         < 0 )
      {
        std::cerr << "ERROR: Could not create attribute '" << attrname << ".\n";
        return DAL::FAIL;
      }

    if ( H5Awrite(att, atype, string_att ) < 0 )
      {
        std::cerr << "ERROR: could not set attribute " << attrname << endl;
        return DAL::FAIL;
      }

    return DAL::SUCCESS;

  }

  //_____________________________________________________________________________
  //                                                                  createGroup

  /*!
    \brief Create a new group.

    \param groupname -- The name of the group to be created
    \return dalGroup -- Pointer to a dalGroup object
  */
  dalGroup * dalDataset::createGroup( const char * gname )
  {
    if ( type == H5TYPE ) {
      dalGroup * lg = NULL;
      lg = new dalGroup( gname, &h5fh_p );
      return lg;
    }
    else {
      std::cerr << "ERROR: Operation not supported for filetype ("
		<< type << ").\n";
      return NULL;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                   listTables

  /*!
    List the tables in a dataset; keep in mind, that this operation only is
    supported for data set of appropriate internal structure, such e.g.
    MeasurementSets.

    \return String vector of table names.
   */
  std::vector<std::string> dalDataset::listTables()
  {
    std::vector<std::string> tabs;
    if ( type == MSCASATYPE )
      {
#ifdef HAVE_CASA
        ms_tables = ms_reader->tables();
        unsigned int nelem (ms_tables.nelements());

        // list the names of the tables
        for (unsigned int table(0); table<nelem; table++)
          {
            tabs.push_back( ms_tables(table) );
          }
        return tabs;
#else
        std::cerr << "CASA support not enabled." << endl;
        return tabs;
#endif
      }
    else
      {
        std::cerr << "This operation is not supported for filetype "
                  << type << endl;
        return tabs;
      }
  }

  //_____________________________________________________________________________
  //                                                                    setFilter
  
  /*!
    \param columns A string containing a comma-separated list of columns to
           include in a filtered dataset.
  */
  void dalDataset::setFilter ( std::string columns )
  {
    filter.setFiletype( type );
    filter.set(columns);
  }

  //_____________________________________________________________________________
  //                                                                    setFilter

  /*!
    \param columns A string containing a comma-separated list of columns to
           include in a filtered dataset.
    \param conditions A string describing restraints on which rows are
           included in an opened dataset.  For example: "time>100".
  */
  void dalDataset::setFilter (std::string columns,
			      std::string conditions)
  {
    filter.setFiletype( type );
    filter.set(columns,conditions);
  }


  // ---------------------------------------------------------- createArray

  /*!
    \param arrayname The name of the array you want to create.
    \param data_object A pointer to a data object containing the intitial
                       data you want to include in the array.
    \return dalArray * pointer to an array object.
   */
  dalArray * dalDataset::createArray( std::string arrayname,
                                      dalData * data_object )
  {
    std::vector<int> cdims;
    complex<float> * cdata;
    long indx   = 0;
    long mysize = 1;

    for (unsigned int ll=0; ll<data_object->shape.size(); ll++)
      mysize *= data_object->shape[ll];

    cdata = new complex<float>[ mysize ];
    for (int xx=0; xx<data_object->shape[0]; xx++)
      for (int yy=0; yy<data_object->shape[1]; yy++)
        for (int zz=0; zz<data_object->shape[2]; zz++)
          {
            indx = data_object->c_index(xx,yy,zz);
            cdata[ indx ] = *( (complex<float>*)data_object->get(xx,yy,zz) );
          }

    if ( dal_COMPLEX == data_object->datatype() )
      return createComplexFloatArray( arrayname, data_object->shape,
                                      cdata, cdims );
    else
      {
// ADD MORE DATATYPES HERE!
        std::cerr << "ERROR: " << data_object->datatype()
                  << "is not yet supported.\n";
        return NULL;
      }
  }


  // -------------------------------------------------------- createIntArray

  /****************************************************************
   *  Create an Integer array of N dimensions
   *
   *****************************************************************/
  /*!
    \param arrayname The name of the array you want to create.
    \param dims A vector containing the dimensions of the array.
    \param data An array of integer values.
          \param cdims A vector a chunk dimensions (necessary for extending an
                 hdf5 dataset).
    \return dalArray * pointer to an array object.
  */
  dalArray * dalDataset::createIntArray( std::string arrayname,
                                         std::vector<int> dims,
                                         int data[],
                                         std::vector<int> cdims )
  {
    if ( type == H5TYPE )
      {
        dalIntArray * la = new dalIntArray( h5fh_p, arrayname, dims,
                                            data, cdims );
        return la;
      }
    else if ( type == FITSTYPE )
      {
        std::cerr <<
                  "dalDataset::createIntArray FITS Type not yet supported.\n";
        return NULL;
      }
    else
      {
        std::cerr << "(dalDataset::createIntArray) Data format \'" << type
                  << "\' not supported.\n";
        return NULL;
      }
  }

  // ----------------------------------------------------- createFloatArray

  /****************************************************************
   *  Create an Float array of N dimensions
   *
   *****************************************************************/
  /*!
    \param arrayname The name of the array you want to create.
    \param dims A vector containing the dimensions of the array.
    \param data An array of floating point values.
    \param cdims A vector a chunk dimensions (necessary for extending an
                 hdf5 dataset).
    \return dalArray * pointer to an array object.
     */
  dalArray *
  dalDataset::createFloatArray( std::string arrayname, std::vector<int> dims,
                                float data[], std::vector<int> cdims )
  {
    if ( type == H5TYPE )
      {
        dalFloatArray * la =
          new dalFloatArray( h5fh_p, arrayname, dims, data, cdims );
        return la;
      }
    else
      {
        std::cerr << "(dalDataset::createFloatArray) Filetype \'" << type
                  << "\' not yet supported.\n";
        return NULL;
      }
  }

  // ----------------------------------------------- createComplexFloatArray

  /****************************************************************
   *  Create an Copmlex array of N dimensions
   *
   *****************************************************************/

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
  dalArray *
  dalDataset::createComplexFloatArray( std::string arrayname,
                                       std::vector<int> dims,
                                       complex<float> data[],
                                       std::vector<int> cdims )
  {
    if ( type == H5TYPE )
      {
        dalComplexArray_float32 * la =
          new dalComplexArray_float32( h5fh_p, arrayname, dims, data, cdims );
        return la;
      }
    else
      {
        std::cerr << "(dalDataset::createComplexArray) Filetype \'" << type
                  << "\' not yet supported.\n";
        return NULL;
      }
  }

  // ---------------------------------------------------------- createTable

  /****************************************************************
   *  Creates a table at the highest level of a dataset
   *
   *****************************************************************/

  /*!
    \brief create a new table in the root group
    \param tablename
    \return dalTable
  */
  dalTable * dalDataset::createTable( std::string tablename )
  {
    if ( type == H5TYPE )
      {
        dalTable * lt = new dalTable( H5TYPE );
        lt->createTable( file, tablename, "/" );
        return lt;
      }
    else
      {
        std::cerr << "(dalDataset::createTable) Filetype \'" << type
                  << "\' not yet supported.\n";
        return NULL;
      }
  }

  //_____________________________________________________________________________
  //                                                                  createTable

  /*!
    Creates a table in a group (mainly for hdf5)

    \brief create a new table in a specified group
    \param tablename -- Name of the table to be created
    \param groupname -- Name of the group within which the table is to be
           created.
    \return dalTable
  */
  dalTable * dalDataset::createTable( std::string tablename,
                                      std::string groupname )
  {
    if ( type == H5TYPE )
      {
        dalTable * lt = new dalTable( H5TYPE );
        lt->createTable( file, tablename, groupname );
        return lt;
      }
    else
      {
        std::cerr << "(dalDataset::createTable) Filetype \'" << type
                  << "\' not yet supported.\n";
        return NULL;
      }
  }

  //_____________________________________________________________________________
  //                                                                    openTable

  /*!
    \param tablename The name of the table you want to open
    \return dalTable * A pointer to a table object.
  */
  dalTable * dalDataset::openTable( std::string tablename )
  {
    if ( type == MSCASATYPE )
      {
#ifdef HAVE_CASA
        dalTable * lt = new dalTable( MSCASATYPE );
        if ( filter.isSet() )
          lt->openTable( tablename, ms_reader, &filter );
        else
          lt->openTable( tablename, ms_reader );
        return lt;
#else
        std::cerr << "CASA support not enabled.\n";
        return NULL;
#endif
      }
    else if ( type == H5TYPE )
      {
        dalTable * lt = new dalTable( H5TYPE );
        lt->openTable( file, tablename, "/" );
        return lt;
      }
    else if ( type == FITSTYPE )
      {
        std::cerr << "dalDataset::openTable FITS Type not yet supported.\n";
        return NULL;
      }
    else
      {
        std::cerr << "openTable operation not supported for filetype "
                  << type << endl;
        return NULL;
      }
  }

  //_____________________________________________________________________________
  //                                                                    openTable

  /*!
    \param tablename The name of the table to open.
    \param groupname The name of the group containing the table.
    \return dalTable * A pointer to a table object.
   */
  dalTable * dalDataset::openTable( std::string const &tablename,
                                    std::string const &groupname )
  {
    if ( type == MSCASATYPE )
      {
        std::cerr << "dalDataset::openTable MSCASA Type not yet supported.\n";
        return NULL;
      }
    else if ( type == H5TYPE )
      {
        dalTable * lt = new dalTable( H5TYPE );
        try
          {
            lt->openTable( file, tablename, '/' + groupname );
          }
        catch (std::string message)
          {
            std::cerr << "[dalDataset::openTable] ERROR : " << message << std::endl;
          }
        return lt;
      }
    else if ( type == FITSTYPE )
      {
        std::cerr << "dalDataset::openTable FITS Type not yet supported.\n";
        return NULL;
      }
    else
      return NULL;
  }

  //_____________________________________________________________________________
  //                                                                    openArray

  /*!
    \param arrayname The name of the array to open.
    \return dalArray * A pointer to a array object.
   */
  dalArray * dalDataset::openArray (std::string arrayname)
  {
    if ( type == MSCASATYPE )
      {
        std::cerr << "dalDataset::openArray MSCASA Type not yet supported.\n";
        return NULL;
      }
    else if ( type == H5TYPE )
      {
        dalArray * la = new dalArray;
        la->open( file, arrayname );
        return la;
      }
    else if ( type == FITSTYPE )
      {
        std::cerr << "dalDataset::openArray FITS Type not yet supported.\n";
        return NULL;
      }
    else
      return NULL;
  }

  //_____________________________________________________________________________
  //                                                                    openArray

  /*!
    \param arrayname The name of the array to open.
    \param groupname The name of the group containing the array.
    \return dalArray * A pointer to a array object.
   */
  dalArray * dalDataset::openArray (std::string arrayname,
				    std::string groupname)
  {
    if ( type == MSCASATYPE ) {
      std::cerr << "dalDataset::openArray MSCASA Type not yet supported.\n";
      return NULL;
    }
    else if ( type == H5TYPE ) {
      dalArray * test = new dalArray;
      delete test;
      dalArray * la = new dalArray;
      la->open( file, '/' + groupname + '/' + arrayname );
      return la;
    }
    else if ( type == FITSTYPE )
      {
        std::cerr << "dalDataset::openArray FITS Type not yet supported.\n";
        return NULL;
      }
    else
      return NULL;
  }
  
  //_____________________________________________________________________________
  //                                                                  getFileInfo

  /*!
    \param loc_id -- HDF5 identifier 
    \param name   --
    \param opdata --
   */
  herr_t getFileInfo (hid_t loc_id,
		      const char *name,
		      void *opdata)
  {
    H5G_stat_t statbuf;
    
    /*
     * Get type of the object and display its name and type.
     * The name of the object is passed to this function by
     * the Library. Some magic :-)
     */
    H5Gget_objinfo(loc_id, name, false, &statbuf);
    switch (statbuf.type)
      {
      case H5G_GROUP:
        (*(vector<std::string>*)opdata).push_back( std::string(name) );
        break;
      case H5G_DATASET:
      case H5G_TYPE:
      default:
        break;
      }
    return 0;
  }

  //_____________________________________________________________________________
  //                                                                getGroupNames

  /*!
    \return A vector of group names.
   */
  std::vector<std::string> dalDataset::getGroupNames()
  {
    std::vector<std::string> group_names;

    H5Giterate (h5fh_p,      /* File or group identifier                    */
		"/",         /* Group over which the iteration is performed */
		NULL,        /* Location at which to begin the iteration    */
		getFileInfo,
		&group_names );

    return group_names;
  }

  //_____________________________________________________________________________
  //                                                                    openGroup
  
  /*!
    \brief Open a group in a dataset.

    Open a group in a dataset.

    \param groupname The name of the group to open
    \return dalGroup * A pointer to a group object.
   */
  dalGroup * dalDataset::openGroup( std::string groupname )
  {
    if ( type == H5TYPE )
      {
        dalGroup * group = new dalGroup;
        //cerr << "Trying to open group " << groupname << endl;
        int retval = group->open( file, groupname );
        if ( retval < 0 )
          return NULL;
        else
          return group;
      }
    else if ( type == FITSTYPE )
      {
        std::cerr << "dalDataset::openGroup FITS Type.\n";
        return NULL;
      }
    else
      {
        std::cerr << "dalDataset::openGroup type not supported.\n";
        return NULL;
      }
  }

  //_____________________________________________________________________________
  //                                                                     read_tbb

  /*!
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
  void dalDataset::read_tbb (std::string id,
                             int start,
                             int length,
                             short data_out[])
  {
    char stid[3]; // station id
    sscanf(id.c_str(),"%3c%*6c",stid);
    char datasetname[18];
    sprintf( datasetname, "Station%c%c%c/%s", stid[0],stid[1],stid[2],id.c_str());
    hid_t dataset = H5Dopen1( h5fh_p, datasetname );
    hid_t filespace = H5Dget_space( dataset );
    if (filespace < 0)
      {
        std::cerr << "ERROR: could not get filespace.(dalDataset::read_tbb)\n";
      }
    int rank = H5Sget_simple_extent_ndims( filespace );
    if (rank < 0)
      {
        std::cerr << "ERROR: could not get filespace rank.(dalDataset::read_tbb)\n";
      }

    hsize_t dimsr[1];

    if ( H5Sget_simple_extent_dims( filespace, dimsr, NULL ) < 0 )
      {
        std::cerr << "ERROR: could not get filespace dims.(dalDataset::read_tbb)\n";
      }

    dimsr[0]=length;

    hid_t memspace = H5Screate_simple( rank, dimsr, NULL );
    if (memspace < 0)
      {
        std::cerr << "ERROR: could not create memory space.(dalDataset::read_tbb)\n";
      }

    hsize_t     offset[1];
    hsize_t     offset_out[1];         /* hyperslab offset in memory */
    offset[0] = start;
    if ( H5Sselect_hyperslab( filespace, H5S_SELECT_SET, offset, NULL,
                              dimsr, NULL ) < 0 )
      {
        std::cerr << "ERROR: could not select hyperslab.(dalDataset::read_tbb)\n";
      }

    offset_out[0] = 0;
    if ( H5Sselect_hyperslab( memspace, H5S_SELECT_SET, offset_out, NULL,
                              dimsr, NULL ) < 0 )
      {
        std::cerr << "ERROR: could not select hyperslab.(dalDataset::read_tbb)\n";
      }

    if ( H5Dread( dataset, H5T_NATIVE_SHORT, memspace, filespace, H5P_DEFAULT,
                  data_out ) < 0 )
      {
        std::cerr << "ERROR: could not read array.(dalDataset::read_tbb)\n";
      }
  }
  
  // ============================================================================
  //
  //  The following functions are boost wrappers to allow some previously
  //  defined functions to be easily called from a python prompt.
  //
  // ============================================================================
  
#ifdef PYTHON

  //_____________________________________________________________________________
  //                                                                    ct1_boost

  /*!
    \param a -- The name of the table to be created
  */
  dalTable * dalDataset::ct1_boost (std::string a)
  {
    dalTable * ret;
    ret = dalDataset::createTable(a);
    return ret;
  }

  //_____________________________________________________________________________
  //                                                                    ct2_boost

  /*!
    \param a -- The name of the group within which the new table is created.
    \param b -- The name of the table to be created
   */
  dalTable * dalDataset::ct2_boost (std::string a,
                                    std::string b)
  {
    dalTable * ret;
    ret = dalDataset::createTable(a,b);
    return ret;
  }
  
  //_____________________________________________________________________________
  //                                                                    ot1_boost

  /*!
    \param a -- The name of the table to be opened
  */
  dalTable * dalDataset::ot1_boost (std::string a)
  {
    dalTable * ret;
    ret = openTable(a);
    return ret;
  }

  //_____________________________________________________________________________
  //                                                                    ot2_boost

  /*
    \param a -- The name of the group within which the table resides.
    \param b -- The name of the table to be opened.
  */
  dalTable * dalDataset::ot2_boost (std::string a,
				    std::string b)
  {
    dalTable * ret;
    ret = openTable(a,b);
    return ret;
  }

  /******************************************************
   * wrappers for createIntArray
   ******************************************************/

  // ---------------------------------------------------------- cia_boost1
  
  dalArray * dalDataset::cia_boost1 (std::string arrayname,
				     bpl::list pydims,
                                     bpl::list pydata )
  {
    bpl::list cdims;
    
    for (int ii=0; ii<bpl::len(pydims); ii++) {
      cdims.append(10);
    }
    
    dalArray * array;
    array = cia_boost2( arrayname, pydims, pydata, cdims );

    /* Return pointer to the created array */
    return array;
  }

  // ---------------------------------------------------------- cia_boost2
  
  dalArray * dalDataset::cia_boost2 (std::string arrayname,
				     bpl::list pydims,
				     bpl::list pydata,
				     bpl::list cdims )
  {
    std::vector<int> dims;
    std::vector<int> chnkdims;
    
    for (int ii=0; ii<bpl::len(pydims); ii++) {
      dims.push_back(bpl::extract<int>(pydims[ii]));
    }
    
    for (int ii=0; ii<bpl::len(cdims); ii++) {
      chnkdims.push_back(bpl::extract<int>(cdims[ii]));
    }
    
    long size = bpl::len(pydata);
    int * data = NULL;
    data = new int[size];
    
    for (int ii=0; ii<size; ii++) {
      data[ii] = bpl::extract<int>(pydata[ii]);
    }

    dalArray * array = createIntArray(arrayname, dims, data, chnkdims);

    /* Release allocated memory */
    delete [] data;
    /* Return pointer to created array */
    return array;
  }

  // ----------------------------------------------------- cia_boost_numarray1
  
  dalArray * dalDataset::cia_boost_numarray1 (std::string arrayname,
					      bpl::list pydims,
					      bpl::numeric::array pydata )
  {
    bpl::list cdims;

    for (int ii=0; ii<bpl::len(pydims); ii++) {
      cdims.append(10);
    }

    dalArray * array;
    array = cia_boost_numarray2(arrayname, pydims, pydata, cdims);

    /* Return pointer to created array */
    return array;
  }

  // ----------------------------------------------- cia_boost_numarray2

  dalArray * dalDataset::cia_boost_numarray2 (std::string arrayname,
					      bpl::list pydims,
					      bpl::numeric::array pydata,
					      bpl::list cdims )
  {
    std::vector<int> dims;
    std::vector<int> chnkdims;
    
    for (int ii=0; ii<bpl::len(pydims); ii++) {
      dims.push_back(bpl::extract<int>(pydims[ii]));
    }
    
    for (int ii=0; ii<bpl::len(cdims); ii++) {
      chnkdims.push_back(bpl::extract<int>(cdims[ii]));
    }
    
    int size = num_util::size(pydata);
    int * data = NULL;
    data = new int[size];
    
    for (int ii=0; ii<size; ii++) {
      data[ii] = bpl::extract<int>(pydata[ii]);
    }
    
    dalArray * array = createIntArray(arrayname, dims, data, chnkdims);
    
    /* Release allocated memory */
    delete [] data;
    /* Return pointer to created array */
    return array;
  }

  /******************************************************
   * wrappers for createFloatArray
   ******************************************************/

  // ---------------------------------------------------------- cfa_boost

  dalArray * dalDataset::cfa_boost (std::string arrayname,
				    bpl::list pydims,
                                    bpl::list pydata,
				    bpl::list cdims)
  {
    std::vector<int> dims;
    std::vector<int> chnkdims;
    
    for (int ii=0; ii<bpl::len(pydims); ii++) {
      dims.push_back(bpl::extract<int>(pydims[ii]));
    }
    
    for (int ii=0; ii<bpl::len(cdims); ii++) {
      chnkdims.push_back(bpl::extract<int>(cdims[ii]));
    }
    
    long size = bpl::len(pydata);
    float * data = NULL;
    data = new float[size];
    
    for (int ii=0; ii<size; ii++) {
      data[ii] = bpl::extract<float>(pydata[ii]);
    }
    
    dalArray * array = createFloatArray( arrayname, dims, data, chnkdims );
    
    /* Release allocated memory */
    delete [] data;
    /* Return pointer to created array */
    return array;
  }

  // ------------------------------------------------------ cfa_boost_numarray

  dalArray * dalDataset::cfa_boost_numarray (std::string arrayname,
					     bpl::list pydims,
					     bpl::numeric::array pydata,
					     bpl::list cdims )
  {
    std::vector<int> dims;
    std::vector<int> chnkdims;

    for (int ii=0; ii<bpl::len(pydims); ii++)
      dims.push_back(bpl::extract<int>(pydims[ii]));

    for (int ii=0; ii<bpl::len(cdims); ii++)
      chnkdims.push_back(bpl::extract<int>(cdims[ii]));

    int size = num_util::size(pydata);
    float * data = NULL;
    data = new float[size];

    for (int ii=0; ii<size; ii++)
      data[ii] = bpl::extract<float>(pydata[ii]);

    dalArray * array = createFloatArray(arrayname, dims, data, chnkdims);

    /* Release allocated memory */
    delete [] data;
    /* Return pointer to created array */
    return array;
  }

  // ---------------------------------------------------------- ria_boost

  bpl::numeric::array dalDataset::ria_boost (std::string arrayname )
  {
    hid_t lclfile;
    hid_t  status;

    // get the dataspace
    lclfile = H5Dopen1(h5fh_p, arrayname.c_str());
    hid_t filespace = H5Dget_space(lclfile);

    // what is the rank of the array?
    hid_t data_rank = H5Sget_simple_extent_ndims(filespace);
    hsize_t dims[ data_rank ];
    status = H5Sget_simple_extent_dims(filespace, dims, NULL);

    int size = 1;
    std::vector<int> dimensions;
    for (int ii=0; ii<data_rank; ii++) {
      size *= dims[ii];
      dimensions.push_back(dims[ii]);
    }
    
    int * data = NULL;
    data = new int[size];
    /* Read data from HDF5 file */
    status = H5LTread_dataset_int( h5fh_p, arrayname.c_str(), data );
    /* Convert data array */
    bpl::numeric::array nadata = num_util::makeNum( (int*)data, dimensions );
    
    /* Release allocated memory */
    delete data;
    /* Return result */
    return nadata;
  }
  
  // ---------------------------------------------------------- rfa_boost
  
  bpl::numeric::array dalDataset::rfa_boost (std::string arrayname )
  {
    hid_t lclfile;
    hid_t status;
    
    // get the dataspace
    lclfile = H5Dopen1(h5fh_p, arrayname.c_str());
    hid_t filespace = H5Dget_space(lclfile);
    
    // what is the rank of the array?
    hid_t data_rank = H5Sget_simple_extent_ndims(filespace);
    hsize_t dims[ data_rank ];

    status = H5Sget_simple_extent_dims(filespace, dims, NULL);

    int size = 1;
    std::vector<int> dimensions;

    for (int ii=0; ii<data_rank; ii++)
      {
        size *= dims[ii];
        dimensions.push_back(dims[ii]);
      }

    float * data = NULL;
    data = new float[size];

    status = H5LTread_dataset_float( h5fh_p, arrayname.c_str(), data );
    bpl::numeric::array nadata = num_util::makeNum( (float*)data, dimensions );

    /* Release allocated memory */
    delete data;
    /* Return result */
    return nadata;
  }

  /******************************************************
   * wrappers for setFilter
   ******************************************************/

  // -------------------------------------------------------- setFilter_boost1

  void dalDataset::setFilter_boost1 (std::string columns )
  {
    setFilter( columns );
  }

  // -------------------------------------------------------- setFilter_boost2

  void dalDataset::setFilter_boost2 (std::string columns,
				     std::string conditions)
  {
    setFilter( columns, conditions );
  }

  // -------------------------------------------------------- listTables_boost

  bpl::list dalDataset::listTables_boost()
  {
    std::vector<std::string> lcltabs = listTables();
    bpl::list lcllist;
    for (uint idx=0; idx<lcltabs.size(); idx++) {
      lcllist.append( lcltabs[idx] );
    }
    return lcllist;
  }
  
  bool dalDataset::setAttribute_char (std::string attrname, char data )
  {
    return setAttribute (attrname, &data );
  }
  bool dalDataset::setAttribute_short (std::string attrname, short data )
  {
    return setAttribute (attrname, &data );
  }
  bool dalDataset::setAttribute_int (std::string attrname, int data )
  {
    return setAttribute (attrname, &data );
  }
  bool dalDataset::setAttribute_uint (std::string attrname, uint data )
  {
    return setAttribute (attrname, &data );
  }
#ifndef WORDSIZE_IS_64
  bool dalDataset::setAttribute_long (std::string attrname, int64_t data )
  {
    return setAttribute (attrname, &data );
  }
#else
  bool dalDataset::setAttribute_long (std::string attrname, long data )
  {
    return setAttribute (attrname, &data );
  }
#endif
  bool dalDataset::setAttribute_float (std::string attrname, float data )
  {
    return setAttribute (attrname, &data );
  }
  bool dalDataset::setAttribute_double (std::string attrname, double data )
  {
    return setAttribute (attrname, &data );
  }
  bool dalDataset::setAttribute_string_boost(std::string attrname, std::string data )
  {
    return setAttribute (attrname, &data );
  }
  bool dalDataset::setAttribute_char_vector (std::string attrname, bpl::list data )
  {
    int size = bpl::len(data);
    std::vector<char> mydata;

    for (int ii=0; ii<bpl::len(data); ii++)
      mydata.push_back(bpl::extract<char>(data[ii]));

    return setAttribute (attrname, reinterpret_cast<char*>(&mydata[0]), size );
  }
  bool dalDataset::setAttribute_short_vector (std::string attrname, bpl::list data )
  {
    int size = bpl::len(data);
    std::vector<short> mydata;

    for (int ii=0; ii<bpl::len(data); ii++)
      mydata.push_back(bpl::extract<short>(data[ii]));

    return setAttribute (attrname, reinterpret_cast<short*>(&mydata[0]), size );
  }
  bool dalDataset::setAttribute_int_vector (std::string attrname, bpl::list data )
  {
    int size = bpl::len(data);
    std::vector<int> mydata;

    for (int ii=0; ii<bpl::len(data); ii++)
      mydata.push_back(bpl::extract<int>(data[ii]));

    return setAttribute (attrname, reinterpret_cast<int*>(&mydata[0]), size );
  }
  bool dalDataset::setAttribute_uint_vector (std::string attrname, bpl::list data )
  {
    int size = bpl::len(data);
    std::vector<uint> mydata;

    for (int ii=0; ii<bpl::len(data); ii++)
      mydata.push_back(bpl::extract<uint>(data[ii]));

    return setAttribute (attrname, reinterpret_cast<uint*>(&mydata[0]), size );
  }
  bool dalDataset::setAttribute_long_vector (std::string attrname, bpl::list data )
  {
    int size = bpl::len(data);
    std::vector<long> mydata;

    for (int ii=0; ii<bpl::len(data); ii++)
      mydata.push_back(bpl::extract<long>(data[ii]));

#ifndef WORDSIZE_IS_64
    return setAttribute (attrname, reinterpret_cast<int64_t*>(&mydata[0]), size );
#else
    return setAttribute (attrname, reinterpret_cast<long*>(&mydata[0]), size );
#endif
  }
  bool dalDataset::setAttribute_float_vector (std::string attrname, bpl::list data )
  {
    int size = bpl::len(data);
    std::vector<float> mydata;

    for (int ii=0; ii<bpl::len(data); ii++)
      mydata.push_back(bpl::extract<float>(data[ii]));

    return setAttribute (attrname, reinterpret_cast<float*>(&mydata[0]), size );
  }
  bool dalDataset::setAttribute_double_vector (std::string attrname, bpl::list data )
  {
    int size = bpl::len(data);
    std::vector<double> mydata;

    for (int ii=0; ii<bpl::len(data); ii++)
      mydata.push_back(bpl::extract<double>(data[ii]));

    return setAttribute (attrname, reinterpret_cast<double*>(&mydata[0]), size );
  }
  bool dalDataset::setAttribute_string_vector (std::string attrname, bpl::list data )
  {
    int size = bpl::len(data);
    std::vector<std::string> mydata;

    for (int ii=0; ii<bpl::len(data); ii++)
      mydata.push_back(bpl::extract<std::string>(data[ii]));

    return setAttribute (attrname, reinterpret_cast<std::string*>(&mydata[0]), size );
  }
  bpl::numeric::array dalDataset::getAttribute_float_boost ( std::string attrname )
  {
  	 std::vector<float> value;
     h5get_attribute( h5fh_p, attrname.c_str(), value );
     std::cerr << value << std::endl;
     std::vector<int> dims;
     dims.push_back( value.size() );
  	 
//  	 float * values = NULL;


//     std::vector<int> mydims;
//     mydims.push_back( value.size() );
//
//     for (int idx=0; idx++; idx<value.size() )
//        values
//     
//      bpl::numeric::array narray = num_util::makeNum( values, mydims );
//      delete [] values;
//      values = NULL;
//      return narray;


     bpl::numeric::array arr = num_util::makeNum( reinterpret_cast<float*>(&value[0]), dims );
     return arr;
  }
  
#endif  // end #ifdef PYTHON

} // end namespace DAL
