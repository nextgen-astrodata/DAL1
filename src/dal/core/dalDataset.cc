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
    : dalObjectBase()
  {
    init();
  }

  //_____________________________________________________________________________
  //                                                                   dalDataset

  /*!
    \param filename  -- The name of the dataset/file to open.
    \param filetype  -- Type of file to open ("HDF5", "CASA_MS", etc.).
    \param flags     -- I/O mode flags
           \e filename. By default an already existing file is kept and only
	   opened -- if you want to overwrite use <tt>overwrite=true</tt>
  */
  dalDataset::dalDataset (std::string const &filename,
			  dalFileType const &filetype,
			  IO_Mode const &flags)
    : dalObjectBase()
  {
    /* Initialize internal parameters ... */
    init (itsName.c_str(),
	  filetype,
	  flags);
    /* ... and try to open the file. */
    open (filename.c_str(),
	  flags);
  }
  
  //_____________________________________________________________________________
  //                                                                   dalDataset

  /*!
    \param filename  -- The name of the dataset/file to open.
    \param filetype  -- Type of file to open ("HDF5", "CASA_MS", etc.).
    \param flags     -- I/O mode flags.
           \e filename. By default an already existing file is kept and only
	   opened -- if you want to overwrite use <tt>overwrite=true</tt>
  */
  dalDataset::dalDataset (std::string const &filename,
			  dalFileType::Type const &filetype,
			  IO_Mode const &flags)
    : dalObjectBase()
  {
    /* Initialize internal parameters ... */
    init (itsName.c_str(),
	  dalFileType(filetype),
	  flags);
    /* ... and try to open the file. */
    open (filename.c_str(),
	  flags);
  }
  
  //_____________________________________________________________________________
  //                                                                   dalDataset
  
  /*!
    \param filename  -- The name of the dataset/file to open.
    \param filetype  -- Type of file to open ("HDF5", "CASA_MS", etc.).
    \param overwrite -- Overwrite existing file if one already exists for name
           \e filename. By default an already existing file is kept and only
	   opened -- if you want to overwrite use <tt>overwrite=true</tt>
  */
  dalDataset::dalDataset (std::string const &filename,
                          std::string filetype,
			  IO_Mode const &flags)
    : dalObjectBase()
  {
    init (filename,
	  filetype,
	  flags);

    open (filename,
	  flags);
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

    /* Release HDF5 object handler */

    DAL::HDF5Object::close(h5fh_p);

    /* Release generic file pointer */

    if (itsObjectHandler != NULL) {
      itsObjectHandler = NULL;
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
    itsFilter      = dalFilter();
    h5fh_p         = 0;
#ifdef DAL_WITH_CASA
    itsMS          = casa::MeasurementSet();
#endif
  }

  //_____________________________________________________________________________
  //                                                                         init
  
  /*!
    \param filename  -- The name of the dataset/file to open.
    \param filetype  -- Type of file to open ("HDF5", "MSCASA", etc.).
    \param flags     -- I/O mode flags.
           \e filename. By default an already existing file is kept and only
	   opened -- if you want to overwrite use <tt>overwrite=true</tt>
  */
  void dalDataset::init (std::string const &filename,
			 dalFileType const &filetype,
			 IO_Mode const &flags)
  {
    /* Initialize internal data with default values */
    init ();

    itsName     = filename;
    itsFiletype = filetype;
    itsFlags    = flags;
  }

  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void dalDataset::summary (std::ostream &os)
  {
    os << "[dalDataset] Summary of object properties" << std::endl;

    os << "-- Dataset type     = " << itsFiletype.name()       << std::endl;
    os << "-- Dataset name     = " << itsName                  << std::endl;
    os << "-- HDF5 file handle = " << getFileHandle()          << std::endl;
    os << "-- HDF5 group ID    = " << getId()                  << std::endl;
    os << "-- I/O mode flags   = " << itsFlags.names()         << std::endl;
    os << "-- Filter string    = " << itsFilter.filterString() << std::endl;

    /*______________________________________________________
      Further properties only can be requested if the object
      is connected to a dataset.
     */

    if (itsName != "UNDEFINED") {
      std::vector<std::string> groupNames = getGroupNames();
      
      os << "-- Group names      = [";
      for (unsigned int n(0); n<groupNames.size(); n++) {
	os << " " << groupNames[n];
      }
      os << " ]" << std::endl;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                     openHDF5

  /*!
    \param filename    -- The name of the dataset/file to open.
    \param flags       -- I/O mode flags
    \return fileIsOpen -- Has the file been opened by reaching the end of this
            function? Returns \e false in case an error was encountered trying
	    to open the file.
  */  
  bool dalDataset::openHDF5 (std::string const &filename,
			     IO_Mode const &flags)
  {
    bool fileIsOpen    = false;
    bool fileTruncated = false;
    
    fileTruncated = HDF5Object::openFile (h5fh_p,
					  filename,
					  flags);
    
    /* Check the HDF5 objec identifier; if it is ok, do internal book-keeping */
    if (H5Iis_valid(h5fh_p)) {
      itsFiletype.setType (dalFileType::HDF5);
      itsObjectHandler = &h5fh_p;
      itsName          = filename;
      fileIsOpen       = true;
    } else {
      fileIsOpen = false;
    }

    return fileIsOpen;
  }

  //_____________________________________________________________________________
  //                                                                     openFITS

#ifdef DAL_WITH_CFITSIO

  /*!
    \param filename    -- The name of the dataset/file to open.
    \param flags       -- I/O mode flags
    \return fileIsOpen -- Has the file been opened by reaching the end of this
            function? Returns \e false in case an error was encountered trying
	    to open the file.
  */  
  bool dalDataset::openFITS (std::string const &filename,
			     IO_Mode const &flags)
  {
    fitsfile * fptr    = NULL;
    int status         = 0;  /* MUST initialize status */
    bool fileExists    = false;
    bool fileTruncated = false; 
    bool fileIsOpen    = false;
    std::ifstream infile (filename.c_str(), std::ifstream::in);
    char card [500];
    memset (card, '\0', 500);
    
    /*______________________________________________________
      Check if the file already exists
    */
    
    if (infile.is_open() && infile.good()) {
      fileExists = true;
    } else {
      fileExists = false;
    }
    infile.close();
    
    /*______________________________________________________
      Open or create file.
    */
    
    if (fileExists) {
      if ( flags.flags() & IO_Mode::Truncate ) {
	std::string tmpname = "!" + filename;
	/* Truncate existing file */
	fileTruncated = true;
	fits_create_file(&fptr, tmpname.c_str(), &status);
      } else if ( flags.flags() & IO_Mode::Create ) {
	std::string tmpname = "!" + filename;
	/* Truncate existing file */
	fileTruncated = true;
	fits_create_file(&fptr, tmpname.c_str(), &status);
      } else {
	if ( flags.flags() & IO_Mode::ReadWrite ) {
	  /* Open file as read/write */
	  fileTruncated = false;
	  fits_open_file (&fptr, filename.c_str(), READWRITE, &status);
	} else {
	  /* Open file as read-only */
	  fileTruncated = false;
	  fits_open_file (&fptr, filename.c_str(), READONLY, &status);
	}
      }
    } else {
      /* Create new file from scratch */
      fileTruncated = true;
      fits_create_file(&fptr, filename.c_str(), &status);
    }

    /* Get header information of opened FITS file */
    if (status) {
      fileIsOpen = false;
    } else {

      fileIsOpen = true;

      int nkeys = 0;

      fits_get_hdrspace(fptr, &nkeys, NULL, &status);
      
      for (int ii=1; ii <= nkeys; ii++) {
	fits_read_record(fptr, ii, card, &status); /* read keyword */
	printf("%s\n", card);
      }
      
      fits_close_file(fptr, &status);
    }

    return fileIsOpen;
  }

#endif

  //_____________________________________________________________________________
  //                                                                       openMS

#ifdef DAL_WITH_CASA

  /*!
    \param filename    -- The name of the dataset/file to open.
    \param flags       -- I/O mode flags
    \return fileIsOpen -- Has the file been opened by reaching the end of this
            function? Returns \e false in case an error was encountered trying
	    to open the file.
  */  
  bool dalDataset::openMS (std::string const &filename,
			   IO_Mode const &flags)
  {
    bool fileIsOpen    = false;
    bool fileTruncated = false;
    
    /*______________________________________________________
      Check if the dataset already exists.
    */
    
    casa::File msfile (filename);

    /*______________________________________________________
      Open or create file.
    */
    
    if (msfile.exists()) {
      
      std::string absoluteName;

      if (msfile.isDirectory()) {
	if (msfile.isSymLink()) {
	  casa::SymLink link (msfile);
	  casa::Path realFileName = link.followSymLink();
	  absoluteName = realFileName.absoluteName();
	} else {
	  absoluteName = filename;
	}
      } else {
	return false;
      }
      
      if ( flags.flags() & IO_Mode::Truncate ) {
	std::cout << "[dalDataset::openMS(IO_Mode::Truncate)]" << std::endl;
	/* Truncate existing file */
	fileTruncated = true;
	itsMS = casa::MeasurementSet (absoluteName, casa::Table::New);
      } else if ( flags.flags() & IO_Mode::Create ) {
	std::cout << "[dalDataset::openMS(IO_Mode::Create)]" << std::endl;
	/* Truncate existing file */
	fileTruncated = true;
	itsMS = casa::MeasurementSet (absoluteName, casa::Table::New);
      } else {
	if ( flags.flags() & IO_Mode::ReadWrite ) {
	  /* Open file as read/write */
	  fileTruncated = false;
	  itsMS = casa::MeasurementSet (absoluteName);
	} else {
	  /* Open file as read-only */
	  fileTruncated = false;
	  itsMS = casa::MeasurementSet (absoluteName);
	}
      }
      
    } else {
      // create table descriptor
      casa::TableDesc simpleDesc = casa::MS::requiredTableDesc();
      // set up a new table
      casa::SetupNewTable newTab (filename,
				  simpleDesc,
				  casa::Table::New);
      // create the new MeasurementSet
      itsMS = casa::MeasurementSet (newTab);
      // create the required sub-tables
      itsMS.createDefaultSubtables(casa::Table::New);
      /* book-keeping */
      fileTruncated = true;
    }
    
    if (itsMS.validate()) {
      fileIsOpen       = true;
      itsName          = filename;
      itsObjectHandler = &itsMS;
      itsFiletype.setType (dalFileType::CASA_MS);
    } else {
      fileIsOpen     = false;
    }

    return fileIsOpen;
  }

#endif
  
  //_____________________________________________________________________________
  //                                                                         open

  /*!
    \param filename  -- The name of the file to open.
    \param flags     -- I/O mode flags.
    \return bool     -- Status of the operation, either DAL::FAIL or
            DAL::SUCCESS.
  */
  bool dalDataset::open (std::string const &filename,
			 IO_Mode const &flags)
  {
    bool status    = true;
    bool fileExits = false;
    itsFlags       = flags;
    
    /* Turn off error reporting */
    H5Eset_auto(H5E_DEFAULT, NULL, NULL);

    /*________________________________________________________________
      Generic check whether or not the provided name points to a file
      that is accessable.
    */

    std::ifstream infile;
    infile.open (filename.c_str(), std::ifstream::in);
    
    if (infile.is_open() && infile.good()) {
      fileExits = true;
    } else {
      fileExits = false;
    }
    infile.close();

    /*________________________________________________________________
      Open existing file
    */
    
    if (fileExits) {

      bool fileIsOpen = false;

      /* Set up I/O mode flags for the case the file already exists;
	 if this indeed is the case, flags resulting in creation need
	 to be removed.
      */
      DAL::IO_Mode tmpFlags (flags);
      tmpFlags.addFlag (IO_Mode::Open);
      tmpFlags.removeFlag (IO_Mode::OpenOrCreate);
      tmpFlags.removeFlag (IO_Mode::Create);
      tmpFlags.removeFlag (IO_Mode::CreateNew);
      
      // Try opening file through HDF5 library _______________________
      
      fileIsOpen = openHDF5 (filename,
			     tmpFlags);
      
      // Try opening file through CFITSIO library ____________________
      
#ifdef DAL_WITH_CFITSIO
      if (!fileIsOpen) {
	fileIsOpen = openFITS (filename,
			       tmpFlags);
      }
#endif
      
      // Try opening file through casacore library _____________________
      
#ifdef DAL_WITH_CASA
      if (!fileIsOpen) {
	fileIsOpen = openMS (filename,
			     tmpFlags);
      }
#endif
      
    } else {
      switch (itsFiletype.type()) {
	// Create new file of type HDF5
      case dalFileType::HDF5:
	{
	  status = openHDF5 (filename,
			     flags);
	}
	break;
#ifdef DAL_WITH_CFITSIO
      case dalFileType::FITS:
	{
	  openFITS (filename,
		    flags);
	}
	break;
#endif
#ifdef DAL_WITH_CASA
      case dalFileType::CASA_MS:
	{
	  openMS (filename,
		  flags);
	}
	break;
#endif
      default:
	{
	  std::cerr << "[dalDataset::open] Creating of new dataset of type "
		    << itsFiletype.name()
		    << " not supported yet!"
		    << std::endl;
	  status = false;
	}
	break;
      };
    }

    return status;
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
	casa::MSReader msReader (itsMS);
        ms_tables = msReader.tables();
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
  void dalDataset::setFilter (std::string const &columns)
  {
    itsFilter.setFiletype(itsFiletype);
    itsFilter.setFilter(columns);
  }

  //_____________________________________________________________________________
  //                                                                    setFilter

  /*!
    \param columns A string containing a comma-separated list of columns to
           include in a filtered dataset.
    \param conditions A string describing restraints on which rows are
           included in an opened dataset.  For example: "time>100".
  */
  void dalDataset::setFilter (std::string const &columns,
			      std::string const &conditions)
  {
    itsFilter.setFiletype(itsFiletype);
    itsFilter.setFilter(columns,conditions);
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

    for (unsigned int ll=0; ll<data_object->itsShape.size(); ll++) {
      mysize *= data_object->itsShape[ll];
    }

    /*________________________________________________________________
      Copy the data
    */

    std::complex<float> * cdata = new std::complex<float>[mysize];

    try {

      int xx=0;
      int yy=0;
      int zz=0;
      
      for (xx=0; xx<data_object->itsShape[0]; xx++) {
	for (yy=0; yy<data_object->itsShape[1]; yy++) {
	  for (zz=0; zz<data_object->itsShape[2]; zz++) {
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
				      data_object->itsShape,
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
	  dalIntArray * la = new dalIntArray (h5fh_p,
					      arrayname,
					      dims,
					      data,
					      cdims);
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
	  dalTable * lt = new dalTable (DAL::dalFileType::HDF5);
	  lt->createTable( itsObjectHandler, tablename, "/" );
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
	  dalTable * lt = new dalTable( DAL::dalFileType::HDF5 );
	  lt->createTable( itsObjectHandler, tablename, groupname );
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
	casa::MSReader msReader (itsMS);
        dalTable * lt = new dalTable (itsFiletype);
        if ( itsFilter.isSet() )
          lt->openTable( tablename, &msReader, itsFilter);
        else
          lt->openTable( tablename, &msReader );
        return lt;
#else
        std::cerr << "CASA support not enabled.\n";
        return NULL;
#endif
      }
      break;
    case dalFileType::HDF5:
      {
        dalTable * lt = new dalTable( DAL::dalFileType::HDF5 );
        lt->openTable( itsObjectHandler, tablename, "/" );
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
        dalTable * lt = new dalTable (DAL::dalFileType::HDF5);
        try {
	  lt->openTable( itsObjectHandler, tablename, '/' + groupname );
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
        la->open( itsObjectHandler, arrayname );
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
	la->open( itsObjectHandler, '/' + groupname + '/' + arrayname );
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
  dalGroup * dalDataset::openGroup (std::string groupname )
  {
    switch (itsFiletype.type()) {
    case dalFileType::HDF5:
      {
        dalGroup * group = new dalGroup;
        //cerr << "Trying to open group " << groupname << endl;
        int retval = group->open( itsObjectHandler, groupname );
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
                             int const &start,
                             int const &length,
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
