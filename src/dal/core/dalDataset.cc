/***************************************************************************
 *   Copyright (C) 2006-2011                                               *
 *   Joseph Masters (jmasters@science.uva.nl)                              *
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

#include "dalDataset.h"

namespace DAL {
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                   dalDataset
  
  dalDataset::dalDataset()
  {
    init();
  }

  //_____________________________________________________________________________
  //                                                                   dalDataset
  
  /*!
    \param filename -- Name of the dataset to be opened.
  */
  dalDataset::dalDataset (std::string const &filename)
  {
    /* Initialize internal parameters ... */
    init ();
    /* ... and try to open the file. */
    open (filename.c_str());
  }
  
  //_____________________________________________________________________________
  //                                                                   dalDataset

  /*!
    \param filename  -- The name of the dataset/file to open.
    \param filetype  -- Type of file to open ("HDF5", "MSCASA", etc.).
    \param overwrite -- Overwrite existing file if one already exists for name
           \e filename. By default an already existing file is kept and only
	   opened -- if you want to overwrite use <tt>overwrite=true</tt>
  */
  dalDataset::dalDataset (std::string const &filename,
			  dalFileType const &filetype,
			  const bool &overwrite)
  {
    /* Initialize internal parameters ... */
    init (name.c_str(),
	  filetype,
	  overwrite);
    /* ... and try to open the file. */
    open (filename.c_str());
  }
  
  //_____________________________________________________________________________
  //                                                                   dalDataset
  
  /*!
    \param filename  -- The name of the dataset/file to open.
    \param filetype  -- Type of file to open ("HDF5", "MSCASA", etc.).
    \param overwrite -- Overwrite existing file if one already exists for name
           \e filename. By default an already existing file is kept and only
	   opened -- if you want to overwrite use <tt>overwrite=true</tt>
  */
  dalDataset::dalDataset (std::string const &filename,
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
      if (overwrite_p) {
	/* Directly try to create the dataset */
	if ( ( h5fh_p = H5Fcreate( filename.c_str(),
				   H5F_ACC_TRUNC,
				   H5P_DEFAULT,
				   H5P_DEFAULT ) ) < 0 )
	  {
	    std::cerr << "ERROR: Could not create file '" << filename << "'."
		      << std::endl;
	  }
      }  // END -- if (overwrite_p)
      else {
	FILE * pFile;
	pFile = fopen ( filename.c_str(), "r" );
	if ( pFile == NULL )  /* check to see if the file exists */
	  {
	    /* if not, create it */
	    if ( ( h5fh_p = H5Fcreate( filename.c_str(),
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
	    
	    h5fh_p = H5Fopen (filename.c_str(),
			      H5F_ACC_RDWR,
			      H5P_DEFAULT);

	    if (!H5Iis_valid(h5fh_p)) {
	      std::cerr << "ERROR: There was a problem opening the file '"
			<< filename << "'.\n";
	      exit(9);
	    }
	    
	  }
	
	itsFilePointer = &h5fh_p;
      }
    }
    else if ( filetype == FITSTYPE ) {
#ifdef DAL_WITH_CFITSIO
      fitsfile *fptr; /* pointer to the FITS file; defined in fitsio.h */
      int status;
      fits_create_file(&fptr, filename.c_str(), &status); /*create new file*/
#else
      std::cerr << "ERROR: CFITSIO libraries not found.\n";
#endif
    }
    else if (filetype == MSCASATYPE) {
    }
    else {
      std::cerr << "(dalDataset::dalDataset) Data format \'" << itsFiletype.name()
		<< "\' not supported for this operation." << endl;
    }
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                  ~dalDataset
  
  dalDataset::~dalDataset()
  {
    destroy ();
  }
  
  //_____________________________________________________________________________
  //                                                                      destroy
  
  bool dalDataset::destroy()
  {
    bool status (true);

    /* Release pointer of casacore objects */
    
#ifdef DAL_WITH_CASA
    if (ms != NULL)           { ms          = NULL;  }
    if (itsMSReader != NULL)  { itsMSReader = NULL;  }
#endif
    
    /* Release HDF5 object handler */

    DAL::HDF5Object::close(h5fh_p);

    /* Release generic file pointer */

    if (itsFilePointer != NULL) {
      itsFilePointer = NULL;
    }

    return status;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                         init
  
  void dalDataset::init()
  {
    itsFilePointer = NULL;
    itsFiletype    = dalFileType();
    name           = "UNDEFINED";
    overwrite_p    = false;
    filter         = dalFilter();
    h5fh_p         = 0;

#ifdef DAL_WITH_CASA
    ms          = NULL;
    itsMSReader = NULL;
#endif
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
  void dalDataset::init (std::string const &filename,
			 dalFileType const &filetype,
			 const bool &overwrite)
  {
    /* Initialize internal data with default values */
    init ();

    itsFilePointer = NULL;
    name           = filename;
    itsFiletype    = filetype;
    overwrite_p    = overwrite;
    h5fh_p         = 0;
  }

  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void dalDataset::summary (std::ostream &os)
  {
    os << "[dalDataset] Summary of object properties" << std::endl;

    os << "-- Dataset type     = " << itsFiletype.name() << std::endl;
    os << "-- Dataset name     = " << getName()          << std::endl;
    os << "-- HDF5 file handle = " << getFileHandle()    << std::endl;
    os << "-- HDF5 group ID    = " << getId()            << std::endl;

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
    
#ifdef DAL_WITH_CFITSIO
    
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
    \param filename  -- The name of the file to open.
    \return bool     -- Status of the operation, either DAL::FAIL or
            DAL::SUCCESS.
  */
  bool dalDataset::open (const char * filename)
  {
    bool status (true);
    std::string lcltype;

    /*!_______________________________________________________________
      Generic check whether or not the provided name points to a file
      that is accessable.
    */

    std::ifstream infile;
    infile.open (filename, std::ifstream::in);
    
    if (infile.is_open() && infile.good()) {
      status = true;
    } else {
      status = false;
    }
    infile.close();

    if (!status) {
      std::cerr << "[dalDataset::open] Low-level test to open file "
		<< std::string(filename)
		<< " failed!"
		<< std::endl;
      return false;
    }
    
    /*!_______________________________________________________________
      Try opening file using the supported underlying libraries.
    */

    if ( DAL::SUCCESS == openFITS( filename ) ) {
      // store the type of the dataset
      lcltype = FITSTYPE;
      itsFiletype.setType (dalFileType::FITS);
      // report successful opening of file
      std::cerr << lcltype << " file opened, but other FITS operations are not "
		<< "yet supported.  Sorry." << endl;
      return DAL::SUCCESS;
    }
    else if ( (h5fh_p = openHDF5( filename )) >= 0 )
      {
        itsFilePointer = &h5fh_p;
        lcltype = H5TYPE;
        itsFiletype.setType (dalFileType::HDF5);
        name = filename;
        return DAL::SUCCESS;
      }
    else
      {
#ifdef DAL_WITH_CASA
        try {
	  lcltype = MSCASATYPE;
	  itsFiletype.setType (dalFileType::CASA_MS);
	  casa::File msfile( filename );
	  // first treat it as a symbolic link
	  if ( msfile.isSymLink() )
	    {
	      casa::SymLink link( msfile );
	      casa::Path realFileName = link.followSymLink();
	      ms        = new casa::MeasurementSet( realFileName.absoluteName() );
	      itsFilePointer   = &ms;
	      itsMSReader = new casa::MSReader( *ms );
	      name      = filename;
	      return DAL::SUCCESS;
	    }
	  else // treat it as a regular file
	    {
	      ms        = new casa::MeasurementSet( filename );
	      itsFilePointer   = &ms;
	      itsMSReader = new casa::MSReader( *ms );
	      name      = filename;
	      return DAL::SUCCESS;
	    }
	}
        catch (casa::AipsError x) {
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
    return destroy();
  }
  
  //_____________________________________________________________________________
  //                                                                getAttributes
  
  /*!
    \brief Print the group attributes.
    
    \return bool -- DAL::FAIL or DAL::SUCCESS
  */
  bool dalDataset::getAttributes()
  {
    if (H5Iis_valid(h5fh_p)) {
      if ( H5Aiterate1( h5fh_p, NULL, attr_info, NULL ) < 0 )
	{
	  std::cerr << "[dalDataset::getAttributes]"
		    << " Could not iterate over dataset attributes."
		    << std::endl;
	  return false;
	}
      else {
	return true;
      }
    }
    else {
      std::cout << "[dalDataset::getAttributes]"
		<< " Unable to get attributes - invalid object identifier!"
		<< std::endl;
      return false;
    }
    
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
    return HDF5Attribute::write (h5fh_p,
				 attrname,
				 data);
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
    switch (itsFiletype.type()) {
    case dalFileType::HDF5:
      {
	dalGroup * lg = NULL;
	lg = new dalGroup( gname, &h5fh_p );
	return lg;
      }
      break;
    default:
      std::cerr << "ERROR: Operation not supported for filetype ("
		<< itsFiletype.name() << ").\n";
      return NULL;
      break;
    };
    
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

    switch (itsFiletype.type()) {
    case dalFileType::CASA_MS:
      {
#ifdef DAL_WITH_CASA
        ms_tables = itsMSReader->tables();
        unsigned int nelem (ms_tables.nelements());
	
        // list the names of the tables
        for (unsigned int table(0); table<nelem; table++) {
	  tabs.push_back( ms_tables(table) );
	}
        return tabs;
#else
        std::cerr << "CASA support not enabled." << endl;
        return tabs;
#endif
      }
      break;
    default:
      {
        std::cerr << "This operation is not supported for filetype "
                  << itsFiletype.name() << endl;
        return tabs;
      }
    };
    
  }
  
  //_____________________________________________________________________________
  //                                                                    setFilter
  
  /*!
    \param columns A string containing a comma-separated list of columns to
           include in a filtered dataset.
  */
  void dalDataset::setFilter ( std::string columns )
  {
    filter.setFiletype(itsFiletype);
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
    filter.setFiletype(itsFiletype);
    filter.set(columns,conditions);
  }

  //_____________________________________________________________________________
  //                                                                  createArray

  /*!
    \param arrayname The name of the array you want to create.
    \param data_object A pointer to a data object containing the intitial
                       data you want to include in the array.
    \return dalArray * pointer to an array object.
   */
  dalArray * dalDataset::createArray (std::string arrayname,
                                      dalData * data_object )
  {
    long indx   = 0;
    long mysize = 1;
    std::vector<int> cdims;

    /*________________________________________________________________
      Get the number of data array elements
    */

    for (unsigned int ll=0; ll<data_object->shape.size(); ll++) {
      mysize *= data_object->shape[ll];
    }

    /*________________________________________________________________
      Copy the data
    */

    std::complex<float> * cdata = new std::complex<float>[mysize];

    try {

      int xx=0;
      int yy=0;
      int zz=0;
      
      for (xx=0; xx<data_object->shape[0]; xx++) {
	for (yy=0; yy<data_object->shape[1]; yy++) {
	  for (zz=0; zz<data_object->shape[2]; zz++) {
	    indx = data_object->c_index(xx,yy,zz);
	    cdata[ indx ] = *( (std::complex<float>*)data_object->get(xx,yy,zz) );
	  }   //   END -- zz
	}   //   END -- yy
      }   //   END -- xx
      
    } catch (std::string message) {
      std::cerr << "[dalDataset::createArray]" << message << std::endl;
    }
    
    if ( dal_COMPLEX == data_object->datatype() )
      return createComplexFloatArray (arrayname,
				      data_object->shape,
                                      cdata,
				      cdims);
    else {
      // ADD MORE DATATYPES HERE!
      std::cerr << "ERROR: " << data_object->datatype()
		<< "is not yet supported.\n";
      return NULL;
    }
  }
  
  //_____________________________________________________________________________
  //                                                               createIntArray

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
  dalArray * dalDataset::createIntArray (std::string arrayname,
                                         std::vector<int> dims,
                                         int data[],
                                         std::vector<int> cdims )
  {
    switch (itsFiletype.type()) {
    case dalFileType::HDF5:
      {
	if (H5Iis_valid(h5fh_p)) {
	  dalIntArray * la = new dalIntArray( h5fh_p, arrayname, dims,
					      data, cdims );
	  return la;
	} else {
	  std::cerr << "[dalDataset::createIntArray]"
		    << " Invalid HDF5 object handler!"
		    << std::endl;
	  return NULL;
	}
      }
      break;
    default:
      {
	std::cerr << "[dalDataset::createIntArray]"
		  << " File type " << itsFiletype.name() 
		    << " not yet supported!"
		    << std::endl;
	  return NULL;
      }
      break;
    };  //  END switch()
  }

  //_____________________________________________________________________________
  //                                                             createFloatArray

  /*!
    Create an array of N dimensions of type \c float

    \param arrayname The name of the array you want to create.
    \param dims A vector containing the dimensions of the array.
    \param data An array of floating point values.
    \param cdims A vector a chunk dimensions (necessary for extending an
                 hdf5 dataset).
    \return dalArray * pointer to an array object.
     */
  dalArray *
  dalDataset::createFloatArray (std::string arrayname,
				std::vector<int> dims,
                                float data[],
				std::vector<int> cdims)
  {
    switch (itsFiletype.type()) {
    case dalFileType::HDF5:
      {
	if (H5Iis_valid(h5fh_p)) {
	  dalFloatArray * la = new dalFloatArray (h5fh_p,
						  arrayname,
						  dims,
						  data,
						  cdims);
	  return la;
	} else {
	  std::cerr << "[dalDataset::createFloatArray]"
		    << " Invalid HDF5 object handler!"
		    << std::endl;
	  return NULL;
	}
      }
      break;
    default:
      {
	std::cerr << "[dalDataset::createFloatArray]"
		  << " File type " << itsFiletype.name() 
		    << " not yet supported!"
		    << std::endl;
	  return NULL;
      }
      break;
    };  //  END switch()
  }
  
  //_____________________________________________________________________________
  //                                                      createComplexFloatArray

  /*!
    Create an array of N dimensions of type \c complex<float>

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
                                       std::complex<float> data[],
                                       std::vector<int> cdims )
  {
    switch (itsFiletype.type()) {
    case dalFileType::HDF5:
      {
	if (H5Iis_valid(h5fh_p)) {
	  dalComplexArray_float32 * la = new dalComplexArray_float32 (h5fh_p,
								      arrayname,
								      dims,
								      data,
								      cdims);
	  return la;
	} else {
	  std::cerr << "[dalDataset::createComplexFloatArray]"
		    << " Invalid HDF5 object handler!"
		    << std::endl;
	  return NULL;
	}
      }
      break;
    default:
      {
	std::cerr << "[dalDataset::createComplexFloatArray]"
		  << " File type " << itsFiletype.name() 
		    << " not yet supported!"
		    << std::endl;
	  return NULL;
      }
      break;
    };  //  END switch()
  }
  
  //_____________________________________________________________________________
  //                                                                  createTable

  /*!
    Creates a table at the highest level of a dataset

    \param tablename -- Name of the table to be created.
    \return dalTable -- Pointer to the created table object.
  */
  dalTable * dalDataset::createTable (std::string tablename)
  {
    switch (itsFiletype.type()) {
    case dalFileType::HDF5:
      {
	if (H5Iis_valid(h5fh_p)) {
	  dalTable * lt = new dalTable( H5TYPE );
	  lt->createTable( itsFilePointer, tablename, "/" );
	  return lt;
	} else {
	  std::cerr << "[dalDataset::createTable]"
		    << " Invalid HDF5 object handler!"
		    << std::endl;
	  return NULL;
	}
      }
      break;
    default:
      {
	std::cerr << "[dalDataset::createTable]"
		  << " File type " << itsFiletype.name() 
		  << " not yet supported!"
		  << std::endl;
	  return NULL;
      }
      break;
    };  //  END switch()
  }
  
  //_____________________________________________________________________________
  //                                                                  createTable
  
  /*!
    Creates a table in a group (mainly for hdf5)

    \param tablename -- Name of the table to be created
    \param groupname -- Name of the group within which the table is to be
           created.
    \return dalTable
  */
  dalTable * dalDataset::createTable( std::string tablename,
                                      std::string groupname )
  {
    switch (itsFiletype.type()) {
    case dalFileType::HDF5:
      {
	if (H5Iis_valid(h5fh_p)) {
	  dalTable * lt = new dalTable( H5TYPE );
	  lt->createTable( itsFilePointer, tablename, groupname );
	  return lt;
	} else {
	  std::cerr << "[dalDataset::createTable]"
		    << " Invalid HDF5 object handler!"
		    << std::endl;
	  return NULL;
	}
      }
      break;
    default:
      {
	std::cerr << "[dalDataset::createTable]"
		  << " File type " << itsFiletype.name() 
		  << " not yet supported!"
		  << std::endl;
	  return NULL;
      }
      break;
    };  //  END switch()
  }
  
  //_____________________________________________________________________________
  //                                                                    openTable
  
  /*!
    \param tablename The name of the table you want to open
    \return dalTable * A pointer to a table object.
  */
  dalTable * dalDataset::openTable (std::string const &tablename)
  {
    switch (itsFiletype.type()) {
    case dalFileType::CASA_MS:
      {
#ifdef DAL_WITH_CASA
        dalTable * lt = new dalTable( MSCASATYPE );
        if ( filter.isSet() )
          lt->openTable( tablename, itsMSReader, &filter );
        else
          lt->openTable( tablename, itsMSReader );
        return lt;
#else
        std::cerr << "CASA support not enabled.\n";
        return NULL;
#endif
      }
      break;
    case dalFileType::HDF5:
      {
        dalTable * lt = new dalTable( H5TYPE );
        lt->openTable( itsFilePointer, tablename, "/" );
        return lt;
      }
      break;
    default:
      {
	std::cerr << "[dalDataset::openTable]"
		  << " File type " << itsFiletype.name() 
		  << " not yet supported!"
		  << std::endl;
	  return NULL;
      }
      break;
    };
  }

  //_____________________________________________________________________________
  //                                                                    openTable

  /*!
    \param tablename The name of the table to open.
    \param groupname The name of the group containing the table.
    \return dalTable * A pointer to a table object.
   */
  dalTable * dalDataset::openTable (std::string const &tablename,
                                    std::string const &groupname )
  {
    switch (itsFiletype.type()) {
    case dalFileType::HDF5:
      {
        dalTable * lt = new dalTable( H5TYPE );
        try {
	  lt->openTable( itsFilePointer, tablename, '/' + groupname );
	}
        catch (std::string message) {
	  std::cerr << "[dalDataset::openTable] ERROR : " << message << std::endl;
	}
        return lt;
      }
      break;
    default:
      {
	std::cerr << "[dalDataset::openTable]"
		  << " File type " << itsFiletype.name() 
		  << " not yet supported!"
		  << std::endl;
	return NULL;
      }
      break;
    };
  }

  //_____________________________________________________________________________
  //                                                                    openArray

  /*!
    \param arrayname The name of the array to open.
    \return dalArray * A pointer to a array object.
   */
  dalArray * dalDataset::openArray (std::string const &arrayname)
  {
    switch (itsFiletype.type()) {
    case dalFileType::HDF5:
      {
        dalArray * la = new dalArray;
        la->open( itsFilePointer, arrayname );
        return la;
      }
      break;
    default:
      {
	std::cerr << "[dalDataset::openArray]"
		  << " File type " << itsFiletype.name() 
		  << " not yet supported!"
		  << std::endl;
	return NULL;
      }
      break;
    };
  }

  //_____________________________________________________________________________
  //                                                                    openArray

  /*!
    \param arrayname The name of the array to open.
    \param groupname The name of the group containing the array.
    \return dalArray * A pointer to a array object.
   */
  dalArray * dalDataset::openArray (std::string const &arrayname,
				    std::string const &groupname)
  {
    switch (itsFiletype.type()) {
    case dalFileType::HDF5:
      {
	dalArray * la = new dalArray;
	la->open( itsFilePointer, '/' + groupname + '/' + arrayname );
	return la;
      }
      break;
    default:
      {
	std::cerr << "[dalDataset::openArray]"
		  << " File type " << itsFiletype.name() 
		  << " not yet supported!"
		  << std::endl;
	return NULL;
      }
      break;
    };
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
        (*(std::vector<std::string>*)opdata).push_back( std::string(name) );
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
    \return groupNames --  A vector of group names; returns an empty vector in
            case no sub-groups exist or the operation is not defined with the
	    current state of the object (e.g. because it is not connected to an
	    actual dataset).
   */
  std::vector<std::string> dalDataset::getGroupNames()
  {
    std::vector<std::string> group_names;

    if (H5Iis_valid(h5fh_p)) {
      H5Giterate (h5fh_p,      /* File or group identifier                    */
		  "/",         /* Group over which the iteration is performed */
		  NULL,        /* Location at which to begin the iteration    */
		  getFileInfo,
		  &group_names );
    }
    
    return group_names;
  }
  
  //_____________________________________________________________________________
  //                                                                    openGroup
  
  /*!
    \brief Open a group in a dataset.
    
    \param groupname The name of the group to open
    \return dalGroup * A pointer to a group object.
   */
  dalGroup * dalDataset::openGroup( std::string groupname )
  {
    switch (itsFiletype.type()) {
    case dalFileType::HDF5:
      {
        dalGroup * group = new dalGroup;
        //cerr << "Trying to open group " << groupname << endl;
        int retval = group->open( itsFilePointer, groupname );
        if ( retval < 0 ) {
          return NULL;
	} else {
          return group;
	}
      }
      break;
    default:
      {
	std::cerr << "[dalDataset::openArray]"
		  << " File type " << itsFiletype.name() 
		  << " not yet supported!"
		  << std::endl;
	return NULL;
      }
      break;
    };
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
    hid_t dataset = H5Dopen ( h5fh_p, datasetname, H5P_DEFAULT );
    hid_t filespace = H5Dget_space( dataset );
    if (filespace < 0)
      {
        std::cerr << "ERROR: could not get filespace.(dalDataset::read_tbb)\n";
      }
    int rank = H5Sget_simple_extent_ndims( filespace );
    if (rank < 0) {
      std::cerr << "ERROR: could not get filespace rank.(dalDataset::read_tbb)\n";
    }
    
    hsize_t dimsr[1];
    
    if ( H5Sget_simple_extent_dims( filespace, dimsr, NULL ) < 0 ) {
      std::cerr << "ERROR: could not get filespace dims.(dalDataset::read_tbb)\n";
    }
    
    dimsr[0]=length;
    
    hid_t memspace = H5Screate_simple( rank, dimsr, NULL );
    if (memspace < 0) {
      std::cerr << "ERROR: could not create memory space.(dalDataset::read_tbb)\n";
    }
    
    hsize_t     offset[1];
    hsize_t     offset_out[1];         /* hyperslab offset in memory */
    offset[0] = start;
    if ( H5Sselect_hyperslab (filespace,
			      H5S_SELECT_SET,
			      offset,
			      NULL,
                              dimsr,
			      NULL) < 0 )
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
  
} // end namespace DAL
