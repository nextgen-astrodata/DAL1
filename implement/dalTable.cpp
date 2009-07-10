/*------------------------------------------------------------------------*
| $Id:: dalTable.cpp 389 2007-06-13 08:47:09Z masters                   $ |
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

/**
 * \file dalTable.cpp
 * \author Joseph Masters
 * \date 12-04-06
 */

#ifndef DALTABLE_H
#include "dalTable.h"
#endif

namespace DAL {
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  // ------------------------------------------------------------------- dalTable
  
  dalTable::dalTable()
  {
    filter = new dalFilter;
  }
  
  // ------------------------------------------------------------------- dalTable
  
  /*!
    \param filetype -- The type of table you want to create (i.e. "HDF5",
           "MSCASA", etc.)
  */
  dalTable::dalTable( std::string filetype )
  {
    filter = new dalFilter;
    
    type = filetype;
    columns.clear();  // clear the columns vector
    firstrecord = true;
    
    if ( type == MSCASATYPE )
      {
#ifdef HAVE_CASA
        casa_table_handle = new casa::Table;
#else
        std::cerr << "CASA support not enabled." << endl;
#endif
      }
  }

  // ---------------------------------------------------------- ~dalTable
  
  dalTable::~dalTable()
  {
    delete filter;
    if ( type == MSCASATYPE )
      {
#ifdef HAVE_CASA
        delete casa_table_handle;
#endif
      }
  }
  
  // -------------------------------------------------------------------- summary

  /*!
    \param os -- The output stream to which the summary is going to be written
  */
  void dalTable::summary(std::ostream &os)
  {
    unsigned int nofColumns = columns.size();

    os << "\n[dalTable] Summary of object properties"  << endl;

    if (name != "") {
      os << "-- Table name    = " << name << std::endl;
      os << "-- Table type    = " << type << std::endl;
      os << "-- nof. rows     = " << getNumberOfRows() << std::endl;
      os << "-- nof. columns  = " << nofColumns << std::endl;
    }

    /* If the table contains a non-zero number of columns, list their names.
     */
    if (nofColumns > 0) {
      os << "-- Column names  = [";
      for (unsigned int n(0); n<nofColumns; n++) {
	os << " " << columns[n].getName();
      }
      os << " ]" << std::endl;
    }
    
    /* If the table is working with HDF5 as back-end, provide a summary of the
     * identifiers and properties.
     */    
    if (type == H5TYPE && fileID_p > 0) {
      os << "-- HDF5 file ID  = " << fileID_p  << std::endl;
      os << "-- HDF5 table ID = " << tableID_p << std::endl;
      os << "-- nof. fields   = " << nfields  << std::endl;
      os << "-- nof. records  = " << nrecords << std::endl;
    } else {
      os << "-- File type is HDF5, but object not connected to file!"
	 << std::endl;
    }
  }
  
  // ---------------------------------------------------------- getColumn
  
  /*!
    \param colname The name of the column to retrieve.
    \return Pointer to dalColumn object.
  */
  dalColumn * dalTable::getColumn( std::string colname )
  {
    if ( type == MSCASATYPE )
      {
#ifdef HAVE_CASA
        // using the dalColumn class
        dalColumn * lclcol = NULL;
        lclcol = new dalColumn( *casa_table_handle, colname );

#ifdef DEBUGGING_MESSAGES
        lclcol->getType();
        if ( lclcol->isScalar() )
          std::cerr << colname << " is SCALAR" << endl;
        if ( lclcol->isArray() )
          std::cerr << colname << " is ARRAY" << endl;
#endif

        return lclcol;

#else

        std::cerr << "ERROR: casacore not installed" << endl;

        // prevent unsused var build warning when casacore isn't installed
        colname = colname;
        return NULL;
#endif
      }
    else if ( type == H5TYPE )
      {
        std::cerr << "ERROR: hdf5 not yet supported for this function."
        << " Try getColumn_Float32, etc." << endl;
        return NULL;
      }
    else
      {
        return NULL;
      }
  }

  // ------------------------------------------------------- getColumn_Float32

  /*!
  \brief Get a column object from a table.

   Gets a column object from a table object.

   \param colname The name of the column you want to get from the table.
   \return dalColumn Pointer to a column object.
  */
  dalColumn * dalTable::getColumn_Float32( std::string colname )
  {
    if ( type == MSCASATYPE )
      {
#ifdef HAVE_CASA
        // using the dalColumn class
        dalColumn * lclcol = NULL;
        lclcol = new dalColumn( *casa_table_handle, colname );
#ifdef DEBUGGING_MESSAGES
        lclcol->getType();
        if ( lclcol->isScalar() )
          std::cerr << colname << " is SCALAR" << endl;
        if ( lclcol->isArray() )
          std::cerr << colname << " is ARRAY" << endl;
#endif
        return lclcol;
#endif
      }
    else if ( type == H5TYPE )
      {
        dalColumn * lclcol;
        lclcol = new dalColumn( fileID_p, tableID_p, H5TYPE, name, colname,
                                dal_FLOAT );

        return lclcol;
      }
    return NULL;
  }

  // ---------------------------------------------- getColumn_complexFloat32

  /*!
  \brief Get a column object from a table.

   Gets a column object from a table object.

   \param colname The name of the column you want to get from the table.
   \return dalColumn Pointer to a column object.
  */
  dalColumn * dalTable::getColumn_complexFloat32( std::string colname )
  {
    if ( type == MSCASATYPE )
      {
#ifdef HAVE_CASA
        // using the dalColumn class
        dalColumn * lclcol = NULL;
        lclcol = new dalColumn( *casa_table_handle, colname );
#ifdef DEBUGGING_MESSAGES
        lclcol->getType();
        if ( lclcol->isScalar() )
          std::cerr << colname << " is SCALAR" << endl;
        if ( lclcol->isArray() )
          std::cerr << colname << " is ARRAY" << endl;
#endif
        return lclcol;
#endif
      }
    else if ( type == H5TYPE )
      {
        dalColumn * lclcol;
        lclcol = new dalColumn( fileID_p, tableID_p, H5TYPE, name, colname,
                                dal_COMPLEX );

        return lclcol;
      }
    return NULL;
  }


  // ------------------------------------------------ getColumn_complexInt16

  /*!
  \brief Get a column object from a table.

   Gets a column object from a table object.

   \param colname The name of the column you want to get from the table.
   \return dalColumn Pointer to a column object.
  */
  dalColumn * dalTable::getColumn_complexInt16( std::string colname )
  {
    if ( type == MSCASATYPE )
      {
#ifdef HAVE_CASA
        // using the dalColumn class
        dalColumn * lclcol = NULL;
        lclcol = new dalColumn( *casa_table_handle, colname );
#ifdef DEBUGGING_MESSAGES
        lclcol->getType();
        if ( lclcol->isScalar() )
          std::cerr << colname << " is SCALAR" << endl;
        if ( lclcol->isArray() )
          std::cerr << colname << " is ARRAY" << endl;
#endif
        return lclcol;
#else
        std::cerr << "ERROR: casacore not installed" << endl;
        // prevent unused variable build warning if casacore isn't installed
        colname = colname;
        return NULL;
#endif
      }
    else if ( type == H5TYPE )
      {
        dalColumn * lclcol;
        lclcol = new dalColumn( fileID_p, tableID_p, H5TYPE, name, colname,
                                dal_COMPLEX_SHORT );
        return lclcol;

      }
    else
      {
        return NULL;
      }
  }


  // ---------------------------------------------------------- setFilter

  /*!
  \brief Set a filter on the table.

  Sets a filter on the table so that when it is opened, it will only
  contain a subset of information contained within the full dataset table.

  \param columns A comma-separated list of columns you wish read from the
                 table.
  */
  void dalTable::setFilter( std::string columns )
  {
    filter = new dalFilter;
    filter->setFiletype( type );
    filter->set(columns);
  }


  // ---------------------------------------------------------- setFilter

  /*!
  \brief Set a filter on the table.

  Sets a filter on the table so that when it is opened, it will only
  contain a subset of information contained within the full dataset table.

  \param columns A comma-separated list of columns you wish read from the
                 table.
  \param conditions The condition you wish to apply to the columns in the
                   filter.  For example: "TIME>100".
  */
  void dalTable::setFilter( std::string columns, std::string conditions )
  {
    filter->setFiletype( type );
    filter->set(columns,conditions);
  }

  // ---------------------------------------------------------- getColumnData

  /*!
    \brief Retrieve a dalColumn by name.

    Retrieve a dalColumn by name.
    \param colname The name of the column to retrieve.
    \return A generic pointer to the column data.
   */
  void * dalTable::getColumnData( std::string colname )
  {
    if ( type == MSCASATYPE )
      {
#ifdef HAVE_CASA
        // using the dalColumn class
        dalColumn lclcol;
        lclcol = dalColumn( *casa_table_handle, colname );
        lclcol.getType();
#ifdef DEBUGGING_MESSAGES
        if ( lclcol.isScalar() )
          std::cerr << colname << " is SCALAR" << endl;
        if ( lclcol.isArray() )
          std::cerr << colname << " is ARRAY" << endl;
#endif
        casa::uInt nrow;
        nrow = casa_table_handle->nrow();

        // create a column object
        casa::ROTableColumn column( *casa_table_handle, colname );

        // get the column descriptor
        casa::ColumnDesc col_desc = column.columnDesc();

        std::cerr << "-------\n" << col_desc.name() << "\n-------" << endl;
        std::cerr << "Is the column an array? ";
        casa::Bool isarray = col_desc.isArray();
        if ( casa::True == isarray )
          {
#ifdef DEBUGGING_MESSAGES
            std::cerr << "YES" << endl;
#endif
            casa::uInt ndimcol;
            ndimcol = column.ndimColumn();
            casa::IPosition shape = column.shape(1);
#ifdef DEBUGGING_MESSAGES
            std::cerr << "  Number of global dims: " << ndimcol << endl;
            std::cerr << "  Shape: " << shape << endl;
#endif
            switch ( col_desc.dataType() )
              {
              case casa::TpComplex:
              {
#ifdef DEBUGGING_MESSAGES
                std::cerr << "Data type is Complex." << endl;
#endif
                casa::ROArrayColumn<casa::Complex>
                arcolumn( *casa_table_handle, colname );
                int polarization = 0;
                casa::IPosition start (2,polarization,0)/*, length (1,1)*/;
                casa::Slicer slicer (start/*, length*/);
                array_vals_comp = arcolumn.getColumn( slicer );
#ifdef DEBUGGING_MESSAGES
                std::cerr << "number of dims: " << array_vals_comp.ndim() << endl;
                std::cerr << "shape: " << array_vals_comp.shape() << endl;
                std::cerr << "size: " << array_vals_comp.size() << endl;
#endif
                std::vector< complex< float > > valvec;
                array_vals_comp.tovector( valvec );
#ifdef DEBUGGING_MESSAGES
                std::cerr << valvec[0] << valvec[1] << valvec[2] << endl;
                std::cerr << "vector size: " << valvec.size() << endl;
                std::cerr << "Polarization number: " << polarization << endl;
#endif
                return array_vals_comp.data();
              }
              break;
              case casa::TpDComplex:
                std::cerr << "Data type is DComplex." << endl;
                break;
              case casa::TpBool:
                std::cerr << "Data type is Boolean." << endl;
                break;
              case casa::TpChar:
                std::cerr << "Data type is Char." << endl;
                break;
              case casa::TpShort:
                std::cerr << "Data type is Short." << endl;
                break;
              case casa::TpFloat:
                std::cerr << "Data type is Float." << endl;
                break;
              case casa::TpDouble:
              {
                std::cerr << "Data type is Double." << endl;
                casa::ROArrayColumn<casa::Double>
                arcolumn( *casa_table_handle, colname );
                int cell = 0;
                casa::IPosition start (1,cell)/*, length (1,1)*/;
                casa::Slicer slicer (start/*, length*/);
                array_vals_dbl = arcolumn.getColumn( slicer );
#ifdef DEBUGGING_MESSAGES
                std::cerr << "number of dims: " << array_vals_dbl.ndim() << endl;
                std::cerr << "shape: " << array_vals_dbl.shape() << endl;
                std::cerr << "size: " << array_vals_dbl.size() << endl;
#endif
                std::vector<double> valvec;
                array_vals_dbl.tovector( valvec );
#ifdef DEBUGGING_MESSAGES
                std::cerr << "vector size: " << valvec.size() << endl;
                std::cerr << "Data from cell number: " << cell << endl;
#endif
                return array_vals_dbl.data();
              }
              break;
              case casa::TpString:
                std::cerr << "Data type is String." << endl;
                break;
              case casa::TpInt:
              {
#ifdef DEBUGGING_MESSAGES
                std::cerr << "Data type is Int." << endl;
#endif
                int * value = new int[100];
                for (int ii=0; ii<100; ii++)
                  {
                    column.getScalar(ii,value[ii]);
                  }
                return value;
              }
              break;
              default:
                std::cerr << "Datatype not recognized." << endl;
              }
          }
        else
          std::cerr << "NO" << endl;

        if (col_desc.isScalar())
          {
            std::cerr << "isScalar" << endl;
            switch ( col_desc.dataType() )
              {
              case casa::TpComplex:
                std::cerr << "Data type is Complex." << endl;
                break;
              case casa::TpBool:
                std::cerr << "Data type is Boolean." << endl;
                break;
              case casa::TpChar:
                std::cerr << "Data type is Char." << endl;
                break;
              case casa::TpShort:
                std::cerr << "Data type is Short." << endl;
                break;
              case casa::TpFloat:
                std::cerr << "Data type is Float." << endl;
                break;
              case casa::TpDouble:
              {
                std::cerr << "Data type is Double." << endl;
                double * value = new double[100];
                for (int ii=0; ii<100; ii++)
                  {
                    column.getScalar(ii,value[ii]);
                  }
                return value;
              }
              break;
              case casa::TpString:
                std::cerr << "Data type is String." << endl;
                break;
              case casa::TpInt:
              {
                std::cerr << "Data type is Int." << endl;
                int * value = new int[100];
                for (int ii=0; ii<100; ii++)
                  {
                    column.getScalar(ii,value[ii]);
                  }
                return value;
              }
              break;
              default:
                std::cerr << "Datatype not recognized." << endl;
              }
          }
        else if (col_desc.isTable())
          {
            std::cerr << "isTable" << endl;
          }

        casa::TableRecord table_rec = column.keywordSet();
#ifdef DEBUGGING_MESSAGES
        std::cerr << "Number of fields: " << table_rec.nfields() << endl;
        std::cerr << "Datatype: " << col_desc.dataType() << endl;
#endif
        return NULL;
#else
        // prevent unused variable build warning when casacore isn't installed
        colname = colname;
        std::cerr << "ERROR: casacore not installed." << endl;
        return NULL;
#endif
      }
    else
      {
        std::cerr << "dalTable::getColumnData operation not supported for type "
        << type << endl;
        return NULL;
      }
  }

  // ---------------------------------------------------------- getName

  void dalTable::getName()
  {
    if ( type == MSCASATYPE )
      {
        // print the name of the table
#ifdef HAVE_CASA
        std::cerr << "CASA table name: " << casa_table_handle->tableName() << endl;
#else
        std::cerr << "CASA support not enabled." << endl;
#endif
      }
    else
      {
        std::cerr << "dalTable::getName operation not supported for type "
        << type << endl;
      }
  }

#ifdef HAVE_CASA

  // ---------------------------------------------------------- openTable

  /*!
  \brief Open a CASA table, not in a MeasurementSet.

  Open a CASA table, not in a MeasurementSet.

  \param tablename The name of the table you want to open.
  */
  void dalTable::openTable( std::string tablename )
  {
    if ( type == MSCASATYPE )
      {
#ifdef HAVE_CASA
        if ( filter->isSet() )
          {
            try
              {
                *casa_table_handle = casa::Table( tablename );
                *casa_table_handle = casa::tableCommand( filter->get(),
                                     *casa_table_handle );
              }
            catch (casa::AipsError x)
              {
                std::cerr << "ERROR: " << x.getMesg() << endl;
              }
          }
        else
          {
            try
              {
                *casa_table_handle = casa::Table( tablename );
              }
            catch (casa::AipsError x)
              {
                std::cerr << "ERROR: " << x.getMesg() << endl;
              }
          }
#else
        std::cerr << "ERROR: CASA support not enabled.\n";
#endif
      }
    else
      {
        std::cerr << "ERROR: dalTable::openTable operation not supported for type "
        << type << endl;
      }
  }

  // ---------------------------------------------------------- openTable
  
  /*!
    \param tablename The name of the table you want to open.
    \param reader The CASA MS table reader object.
  */
  void dalTable::openTable( std::string tablename,
			    casa::MSReader * reader )
  {
    if ( type == MSCASATYPE )
      {
#ifdef HAVE_CASA
        try
          {
            *casa_table_handle = reader->table( tablename );
          }
        catch (casa::AipsError x)
          {
            std::cerr << "ERROR: " << x.getMesg() << endl;
          }
#else
        std::cerr << "CASA support not enabled.\n";
#endif
      }
    else
      {
        std::cerr << "dalTable::openTable operation not supported for type "
        << type << endl;
      }
  }

  // ------------------------------------------------------------------ openTable

  /*!
    \param tablename -- Name of the table
    \param reader    -- MeasurementSet reader
    \param filter    -- Access filter to be applied to the table
  */
  void dalTable::openTable( std::string tablename,
                            casa::MSReader * reader,
			    dalFilter * filter )
  {
    if ( type == MSCASATYPE )
      {
#ifdef HAVE_CASA
        try
          {
            *casa_table_handle = reader->table( tablename );
            *casa_table_handle = casa::tableCommand( filter->get(),
                                 *casa_table_handle );
          }
        catch (casa::AipsError x)
          {
            std::cerr << "ERROR: " << x.getMesg() << endl;
          }
#else
        std::cerr << "ERROR: CASA support not enabled.\n";
#endif
      }
    else
      {
        std::cerr << "ERROR: dalTable::openTable operation not supported for type "
        << type << endl;
      }
  }
#endif

  // ---------------------------------------------------------- openTable

  /*!
  \brief Open the table.  Called from dalDataset, not from the user.

  Open the table.  Called from dalDataset, not from the user.

  \param voidfile A pointer to the file you want to open.
  \param tablename The name of the table you want to open.
  \param groupname The name of the group containing the table you want
                   to open.
  */
  void dalTable::openTable( void * voidfile,
			    std::string tablename,
			    std::string groupname )
  {
    if ( type == H5TYPE )
      {
        name = groupname + '/' + tablename;
        hid_t * lclfile = (hid_t*)voidfile; // H5File object
        file = lclfile;
        fileID_p = *lclfile;  // get the file handle

        tableID_p = H5Dopen1 ( fileID_p, name.c_str() );
      }
    else
      {
        std::cerr << "dalTable::openTable operation not supported for type "
        << type << endl;
      }
  }

  // ---------------------------------------------------------- printColumns

  /*!
    \brief Print a list of the columns contained in the table.
    
    Print a list of the columns contained in the table.
  */
  void dalTable::printColumns()
  {
    if ( type == H5TYPE )
      {
        size_t * field_sizes = NULL;
        size_t * field_offsets = NULL;
        size_t * size_out = NULL;

        // retrieve the input fields needed for the append_records call
        H5TBget_table_info ( fileID_p, name.c_str(), &nfields, &nrecords );

        field_sizes = (size_t*)malloc( nfields * sizeof(size_t) );
        field_offsets = (size_t*)malloc( nfields * sizeof(size_t) );
        size_out = (size_t*)malloc( sizeof(size_t) );

        /* Alocate space */
        field_names = (char**)malloc( sizeof(char*) * (size_t)nfields );
        for ( unsigned int i = 0; i < nfields; i++)
          {
            field_names[i]=(char*)malloc(sizeof(char) * MAX_COL_NAME_SIZE );
          }
        status = H5TBget_field_info( fileID_p, name.c_str(), field_names,
                                     field_sizes, field_offsets, size_out );

        for (unsigned int ii=0; ii<nfields; ii++)
          {
            std::cerr << setw(17) << field_names[ii];
            free(field_names[ii]);
          }
        free(field_names);
        std::cerr << endl;
      }
    else if ( type == MSCASATYPE )
      {
#ifdef HAVE_CASA
        casa::TableDesc td = casa_table_handle->tableDesc();
        std::cerr << td.columnNames() << endl;
#else
        std::cerr << "ERROR: CASA support not enabled.\n";
#endif
      }
    else
      {
        std::cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
      }
  }

  // ---------------------------------------------------------- createTable
  
  /*!
    \brief Create a new table.
    
    Create a new table in the dataset, possibly within a group.  This
    function is usually called by dalDataset, and not by the developer.
    
    \param voidfile A pointer to the file.
    \param tablename The name of the table you want to create.
    \param groupname The name of the group you where you want to create
           the table.
  */
  void dalTable::createTable( void * voidfile,
			      std::string tablename,
			      std::string groupname )
  {
    if ( type == H5TYPE )
      {
        // It is necessary to have at least one column for table creation
        // so we create a dummy column that will be deleted when real
        // columns are added
        //

        name = groupname + '/' + tablename;// set the private class variable: name
        // cast the voidfile to an hdf5 file
        hid_t * lclfile = (hid_t*)voidfile; // H5File object
        file = lclfile;
        fileID_p = *lclfile;  // get the file handle

        const char * lclfield_names[1]  = { "000dummy000" };
        size_t dst_offset[1] = { 0  };
        hid_t field_type[1] = { H5T_NATIVE_INT };
        hsize_t chunk_size = (hsize_t)CHUNK_SIZE;
        int compress  = 0;  // 0=off 1=on
        int * fill    = NULL;
        fill          = new int[1];
        fill[0]       = 0;
	
        typedef struct Particle
	{
	  int dummy;
	} Particle;
        Particle data[1] = {{0}};
        size_t dst_size =  sizeof(Particle);
	
        dst_size = sizeof( Particle );
        dst_offset[0] = HOFFSET( Particle, dummy );
	
        tablename = groupname + '/' + tablename;
	
        status = H5TBmake_table( tablename.c_str(), fileID_p, tablename.c_str(),
                                 1, 1, dst_size, lclfield_names,
                                 dst_offset, field_type, chunk_size,
                                 fill, compress, data );
        delete [] fill;
        fill = NULL;
        tableID_p = H5Dopen1( fileID_p, tablename.c_str() );
      }
    else
      {
        std::cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
      }
    
  }
  
  /*!
    \param column_name -- Name of the column to be added
    \param removedummy -- Remove dummy column
  */
  void dalTable::h5addColumn_setup( std::string const column_name,
				    bool &removedummy )
  {

      // make sure the column name isn't blank
        if ( 0 == column_name.length() )
          {
            std::cerr << "WARNING: Trying to add column without a name.\n";
            return;
          }

        // retrieve table information
        H5TBget_table_info ( fileID_p, name.c_str(), &nfields, &nrecords );


        // allocate space for the column/field names and retrieve them from
        // the table
        field_names = (char**)malloc( nfields * sizeof(char*) );
        for (unsigned int ii=0; ii<nfields; ii++)
          {
            field_names[ii] = (char*)malloc(MAX_COL_NAME_SIZE * sizeof(char));
          }
        status = H5TBget_field_info( fileID_p, name.c_str(), field_names, NULL,
                                     NULL, NULL );

        // check to make sure column doesn't already exist
        for (unsigned int ii=0; ii<nfields; ii++)
          {
            if (0 == strcmp( column_name.c_str(), field_names[ii] ))
              {
                std::cerr << "WARNING: Cannot create column \'"
                << column_name.c_str()
                <<	"\'. Column already exists." << endl;
                return;
              }
            else if (0 == strcmp("000dummy000",field_names[ii]))
              {
                removedummy = true;
              }
          }

        for (unsigned int ii=0; ii<nfields; ii++)
          {
            free(field_names[ii]);
          }
        free(field_names);
  }

  void dalTable::h5addColumn_insert( uint const & indims,
				     std::string const & colname,
                                     hid_t const & field_type,
				     bool const & removedummy )
  {
    // set additional required fields for new column call
    int * data = NULL;
    data = new int[ indims ];
    for (unsigned int idx=0; idx<indims; idx++)
      data[idx] = 0;
    
    // create the new column
    status = H5TBinsert_field( fileID_p, name.c_str(), colname.c_str(),
			       field_type, nfields, data, data );
    
    delete [] data;
    data = NULL;
    
    if ( removedummy )
      removeColumn("000dummy000");
    
  }
  
  // ---------------------------------------------------------- addColumn

  /*!
    \param colname Name of the column you want to add.
    \param coltype Datatype of the column you want to add (i.e. dalINT,
           dalFLOAT, dalSTRING, etc.)
    \param dims Number of dimensions of the column you want to add.
  */
  void dalTable::addColumn( std::string colname,
			    std::string coltype,
			    uint indims )
  {
    if ( type == H5TYPE )
      {
	bool removedummy = false;
	
	h5addColumn_setup( colname, removedummy );
	
        // set the column type
        hsize_t dd = indims;
        hsize_t * dims = &dd;
        hid_t	field_type = 0;
        hid_t   h5type = 0;
	
        if ( dal_CHAR == coltype )
          h5type =  H5T_NATIVE_CHAR;
	
        else if ( dal_INT == coltype )
          h5type =  H5T_NATIVE_INT;
	
        else if ( dal_UINT == coltype )
          h5type = H5T_NATIVE_UINT;

        else if ( dal_SHORT == coltype )
          h5type = H5T_NATIVE_SHORT;

        else if ( dal_FLOAT == coltype )
          h5type = H5T_NATIVE_FLOAT;

        else if ( dal_DOUBLE == coltype )
          h5type = H5T_NATIVE_DOUBLE;

        else if ( dal_STRING == coltype )
          h5type = H5T_NATIVE_CHAR;

        else if ( dal_COMPLEX_CHAR == coltype || dal_COMPLEX == coltype ||
                  dal_COMPLEX_SHORT == coltype )
          {
            std::vector<dalColumn> cv;
            std::string component_type;

            if ( dal_COMPLEX_CHAR == coltype )
              component_type = dal_CHAR;

            else if ( dal_COMPLEX == coltype )
              component_type = dal_DOUBLE;

            else if ( dal_COMPLEX_SHORT == coltype )
              component_type = dal_SHORT;

            dalColumn col_a( "r", component_type );  // real component
            dalColumn col_b( "i", component_type );  // imaginary component

            cv.push_back( col_a );
            cv.push_back( col_b );

            size_t sz = 0;

            // compute the size of the compound column
            for ( unsigned int ii=0; ii<cv.size(); ii++)
              {
                //cerr << "subcolumn name is " << foo[ii].getName()
                //	 << ". Type is " << foo[ii].getType() << endl;
                sz += cv[ii].getSize();
              }

            // create a compound type that can hold each field
            h5type = H5Tcreate( H5T_COMPOUND, sz );

            size_t offset = 0;
            hid_t lcl_datatype = 0;
            for ( unsigned int ii=0; ii<cv.size(); ii++)
              {

                if (0==ii)
                  offset=0;
                else
                  offset += cv[ii-1].getSize();

                if ( dal_CHAR == cv[ii].getType() )
                  lcl_datatype = H5T_NATIVE_CHAR;

                else if ( dal_SHORT == cv[ii].getType() )
                  lcl_datatype = H5T_NATIVE_SHORT;

                else if ( dal_INT == cv[ii].getType() )
                  lcl_datatype = H5T_NATIVE_INT;

                else if ( dal_FLOAT == cv[ii].getType() )
                  lcl_datatype = H5T_NATIVE_FLOAT;

                else if ( dal_DOUBLE == cv[ii].getType() )
                  lcl_datatype = H5T_NATIVE_DOUBLE;

                H5Tinsert( h5type, cv[ii].getName().c_str(), offset, lcl_datatype );
              }
          }
        else
          {
            std::cerr << "ERROR: column type " << coltype << " is not supported."
            << endl;
            return;
          }
        if ((*dims) > 1)
        	field_type = H5Tarray_create1( h5type, 1, dims, NULL );
		else
			field_type = h5type;        	
		
        h5addColumn_insert( indims, colname, field_type, removedummy );
      }
    else
      {
        std::cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
      }
  }

  // -------------------------------------------------------- addComplexColumn
  
  /*!
    Create an complex type column to the table.  This is usually called by the
    addColumn method when the addColumn datatype is dalCOMPLEX, and not by the
    developer.
    
    \param compname Name of the column you want to add.
    \param ri The real and imaginary column components, as represented by
           dalColumn objects.
    \param subfields This is the dimensionality of the complex column type
           and should always be 2.
  */
  void dalTable::addComplexColumn( std::string compname,
				   std::vector<dalColumn> foo,
                                   int subfields )
  {
    if ( type == H5TYPE )
      {
        bool removedummy = false;
	
	h5addColumn_setup( compname, removedummy );
	
	// ----------   begin complex column-specific code. -------------
	
        size_t sz = 0;
	
        // compute the size of the compound column
        for ( unsigned int ii=0; ii< foo.size(); ii++ )
          {
// 		cerr << "subcolumn name is " << foo[ii].getName()
// 			 << ". Type is " << foo[ii].getType() << endl;
            sz += foo[ii].getSize();
          }

        // create a compound type that can hold each field
        hid_t fieldtype = H5Tcreate( H5T_COMPOUND, sz );

        size_t offset = 0;
        for ( unsigned int ii=0; ii<foo.size(); ii++)
          {

            if (0==ii)
              offset=0;
            else
              offset += foo[ii-1].getSize();

            if ( dal_CHAR == foo[ii].getType() )
              {
                H5Tinsert( fieldtype, foo[ii].getName().c_str(), offset,
                           H5T_NATIVE_CHAR);
              }
            else if ( dal_SHORT == foo[ii].getType() )
              {
                H5Tinsert( fieldtype, foo[ii].getName().c_str(), offset,
                           H5T_NATIVE_SHORT);
              }
            else if ( dal_INT == foo[ii].getType() )
              {
                H5Tinsert( fieldtype, foo[ii].getName().c_str(), offset,
                           H5T_NATIVE_INT);
              }
            else if ( dal_FLOAT == foo[ii].getType() )
              {
                H5Tinsert( fieldtype, foo[ii].getName().c_str(), offset,
                           H5T_NATIVE_FLOAT);
              }
            else if ( dal_DOUBLE == foo[ii].getType() )
              {
                H5Tinsert( fieldtype, foo[ii].getName().c_str(), offset,
                           H5T_NATIVE_DOUBLE);
              }
          }

// ----------   end complex column-specific code. -------------

        h5addColumn_insert( subfields, compname, fieldtype, removedummy );

        return;

        // if successful, add corresponding column object to list
        if ( 0 == status )
          {
            dalColumn * lc = new dalColumn( compname, "foo" );

            // add new dalColumn to the 'columns' vector
            columns.push_back( *lc );

            lc->addMember( "a_member", dal_INT );
          }
      }
    else
      {
        std::cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
      }

  }

  // ---------------------------------------------------------- removeColumn
  
  /*!
    \param colname Name of the column you want to remove from the table.
  */
  void dalTable::removeColumn(const std::string &colname)
  {
    if ( type == H5TYPE )
      {
        status = H5TBget_table_info( fileID_p, name.c_str(), &nfields, &nrecords );
        if ( nfields < 2 )
          {
            std::cerr << "WARNING: Cannot delete last column." << endl;
            return;
          }


        hsize_t nfields_start = nfields;

        field_names = (char **)malloc(nfields_start * sizeof(char *));
        for (unsigned int ii=0; ii<nfields_start; ii++)
          {
            field_names[ii] = (char *)malloc(MAX_COL_NAME_SIZE * sizeof(char));
          }

        status = H5TBget_field_info( fileID_p, name.c_str(), field_names, NULL,
                                     NULL, NULL );

        bool columnpresent = false;
        for (unsigned int ii=0; ii < nfields; ii++)
          {

            if (0 == strcmp(colname.c_str(),field_names[ii]))
              {

                status = H5TBdelete_field( fileID_p, name.c_str(),
                                           field_names[ii]);

                status = H5TBget_table_info ( fileID_p, name.c_str(),
                                              &nfields, &nrecords );
                columnpresent = true;
                break;

              }
          }

        for (unsigned int ii=0; ii<nfields_start; ii++)
          {
            free(field_names[ii]);
          }
        free(field_names);

        if ( !columnpresent )
          std::cerr << "WARNING: Column \'" << colname <<
          "\' not present.  Cannot delete." << endl;
      }
    else
      {
        std::cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
      }
  }

  // ----------------------------------------------------- writeDataByColNum

  /*!
  \brief Write data to a column identified by it's position.

  Write data to a column identified by it's position within the table.

  \param data A pointer to a structure containing the data you want
                   to write.
  \param index The position of the column you want to write to.
  \param rownum The row position where you want to start writing data.
  */
  void dalTable::writeDataByColNum( void * data, int index, int rownum, long nrecs )
  {
    if ( type == H5TYPE )
      {

        size_t * field_sizes = NULL;
        size_t * field_offsets = NULL;
        size_t * size_out = NULL;

        // retrieve the input fields needed for the append_records call
        H5TBget_table_info ( fileID_p, name.c_str(), &nfields, &nrecords );

        field_sizes   = new size_t[ nfields ];
        field_offsets = new size_t[ nfields ];
        size_out      = new size_t[ 1 ];

        status = H5TBget_field_info( fileID_p, name.c_str(), NULL, field_sizes,
                                     field_offsets, size_out );
        /*
         * Cleanup to make more efficient.  Check the last three fields for
         * H5TBget_field_info();
         */
        int num_fields 		= 1;	  // number of fields to overwrite
        const int inum		= index;  // column number to overwrite
        const int * index_num	= &inum;  // pointer to column number to overwrite
        hsize_t start		= hsize_t(rownum); // record index to start at
        hsize_t numrecords	= nrecs;	  // number of records to write

		size_t col_offset[1] = { 0 };
		size_t col_size[1] = { field_sizes[index] };
        status = H5TBwrite_fields_index(fileID_p, name.c_str(), num_fields,
                                        index_num, start, numrecords, *col_size,
                                        col_offset, col_size, data);

        delete [] field_sizes;
        field_sizes = NULL;
        delete [] field_offsets;
        field_offsets = NULL;
        delete [] size_out;
        size_out = NULL;

      }
    else
      {
        std::cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
      }
  }

  // ---------------------------------------------------------- appendRow

  /*!
  \brief Append a row to the table.

  Append a row to the table.

  \param data The data you want to write at the end of the table.  The
  		  structure of the data parameter should match that of the
  		  table itself.
  */
  void dalTable::appendRow( void * data )
  {
    if ( type == H5TYPE )
      {
        hsize_t recs2write     = 1; // number of records to append
        size_t * field_sizes   = NULL;
        size_t * field_offsets = NULL;
        size_t * size_out      = NULL;

        // retrieve the input fields needed for the append_records call
        H5TBget_table_info ( fileID_p, name.c_str(), &nfields, &nrecords );

        field_sizes = (size_t*)malloc( nfields * sizeof(size_t) );
        field_offsets = (size_t*)malloc( nfields * sizeof(size_t) );
        size_out = (size_t*)malloc( sizeof(size_t) );

        status = H5TBget_field_info( fileID_p, name.c_str(), NULL, field_sizes,
                                     field_offsets, size_out );

        if ( firstrecord )
          {
            hsize_t start = 0;
            hsize_t numrows = 1;
            status = H5TBwrite_records( fileID_p, name.c_str(), start,
                                        numrows, *size_out, field_offsets, field_sizes,
                                        data);
            firstrecord = false;
          }
        else
          {
            status = H5TBappend_records ( fileID_p, name.c_str(), recs2write,
                                          *size_out, field_offsets,
                                          field_sizes, data );
          }
        free( field_sizes );
        free( field_offsets );
        free( size_out );
      }
    else
      {
        std::cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
      }
  }

  // ---------------------------------------------------------- appendRows

  /*!
    \brief Append multiple rows.

    Append multiple rows to the end of the table.

    \param data The data you want to write at the end of the table.  The
                structure of the data parameter should match that of the
                table itself.
    \param row_count The number of rows you wish to append.
  */
  void dalTable::appendRows( void * data, long row_count )
  {
    if ( type == H5TYPE )
      {
        size_t * field_sizes = NULL;
        size_t * field_offsets = NULL;
        size_t * size_out = NULL;

        // retrieve the input fields needed for the append_records call
        H5TBget_table_info( fileID_p, name.c_str(), &nfields, &nrecords );

        field_sizes  = (size_t *)malloc((size_t)nfields * sizeof(size_t));
        field_offsets = (size_t *)malloc((size_t)nfields * sizeof(size_t));
        size_out = (size_t*)malloc( sizeof(size_t) );

        status = H5TBget_field_info( fileID_p, name.c_str(), NULL, field_sizes,
                                     field_offsets, size_out );

        if ( firstrecord )
          {
            hsize_t start = 0;
            if (row_count>1)
              {
                status = H5TBappend_records ( fileID_p, name.c_str(),
                                              (hsize_t)row_count-1, *size_out,
                                              field_offsets, field_sizes, data);
              }
            status = H5TBwrite_records( fileID_p, name.c_str(), start,
                                        (hsize_t)row_count, *size_out,
                                        field_offsets, field_sizes, data );
            firstrecord = false;
          }
        else
          {
            status = H5TBappend_records ( fileID_p, name.c_str(),
                                          (hsize_t)row_count, *size_out,
                                          field_offsets, field_sizes, data);
          }
        free( field_sizes );
        free( field_offsets );
        free( size_out );
      }
    else
      {
        std::cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
      }
  }


  // ---------------------------------------------- setAttribute

  /*!
    \brief Add a character attribute.

    Add a character attribute to the table.

    \param attrname The name of the attribute you wish to add.
    \param data The value of the attribute you wish to add.
    \param size An optional parameter specifiying the array size of the
                attribute.  If not supplied, it will default to being
                a scalar attribute.
  */
  bool dalTable::setAttribute( std::string attrname,
			       char * data,
			       int size )
  {
    return h5set_attribute( H5T_NATIVE_CHAR, tableID_p, attrname, data, size );
  }

  // ---------------------------------------------- setAttribute

  /*!
    \brief Add a short attribute.

    Add a short attribute to the table.

    \param attrname The name of the attribute you wish to add.
    \param data The value of the attribute you wish to add.
    \param size An optional parameter specifiying the array size of the
                attribute.  If not supplied, it will default to being
                a scalar attribute.
  */
  bool dalTable::setAttribute( std::string attrname,
			       short * data,
			       int size )
  {
    return h5set_attribute( H5T_NATIVE_SHORT, tableID_p, attrname, data, size );
  }

  // ---------------------------------------------- setAttribute

  /*!
    \brief Add a integer attribute.

    Add a integer attribute to the table.

    \param attrname The name of the attribute you wish to add.
    \param data The value of the attribute you wish to add.
    \param size An optional parameter specifiying the array size of the
                attribute.  If not supplied, it will default to being
                a scalar attribute.
    \return bool -- DAL::FAIL or DAL::SUCCESS
   */
  bool dalTable::setAttribute( std::string attrname,
			       int * data,
			       int size )
  {
    return h5set_attribute( H5T_NATIVE_INT, tableID_p, attrname, data, size );
  }

  // ---------------------------------------------- setAttribute

  /*!
    \brief Add a unsigned integer attribute.

    Add a unsigned integer attribute to the table.

    \param attrname The name of the attribute you wish to add.
    \param data The value of the attribute you wish to add.
    \param size An optional parameter specifiying the array size of the
                attribute.  If not supplied, it will default to being
                a scalar attribute.
    \return bool -- DAL::FAIL or DAL::SUCCESS
  */
  bool dalTable::setAttribute( std::string attrname,
			       uint * data,
			       int size )
  {
    return h5set_attribute( H5T_NATIVE_UINT, tableID_p, attrname, data, size );
  }

  // ---------------------------------------------- setAttribute

  /*!
    \brief Add a long attribute.

    Add a long attribute to the table.

    \param attrname The name of the attribute you wish to add.
    \param data The value of the attribute you wish to add.
    \param size An optional parameter specifiying the array size of the
                attribute.  If not supplied, it will default to being
                a scalar attribute.
  */
  bool dalTable::setAttribute( std::string attrname,
			       long * data,
			       int size )
  {
    return h5set_attribute( H5T_NATIVE_LONG, tableID_p, attrname, data, size );
  }

  // ---------------------------------------------- setAttribute

  /*!
    \brief Add a float attribute.

    Add a float attribute to the table.

    \param attrname The name of the attribute you wish to add.
    \param data The value of the attribute you wish to add.
    \param size An optional parameter specifiying the array size of the
                attribute.  If not supplied, it will default to being
                a scalar attribute.
  */
  bool dalTable::setAttribute( std::string attrname,
			       float * data,
			       int size )
  {
    return h5set_attribute( H5T_NATIVE_FLOAT, tableID_p, attrname, data, size );
  }

  // ---------------------------------------------- setAttribute

  /*!
    \brief Add a double precision floating point attribute.

    Add a double precision floating point attribute to the table.

    \param attrname The name of the attribute you wish to add.
    \param data The value of the attribute you wish to add.
    \param size An optional parameter specifiying the array size of the
                attribute.  If not supplied, it will default to being
                a scalar attribute.
    \return bool -- DAL::FAIL or DAL::SUCCESS
   */
  bool dalTable::setAttribute( std::string attrname,
			       double * data,
			       int size )
  {
    return h5set_attribute( H5T_NATIVE_DOUBLE, tableID_p, attrname, data, size );
  }

  // ---------------------------------------------- setAttribute_string

  /*!
    \brief Define a string attribute.

    \param attrname The name of the attribute you want to create.
    \param data The value of the attribute you want to create.
    \return bool -- DAL::FAIL or DAL::SUCCESS
  */
  bool dalTable::setAttribute( std::string attrname,
			       std::string data )
  {
    return h5setAttribute_string( tableID_p, attrname, &data, 1 );
  }

  // ---------------------------------------------- setAttribute_string

  /*!
    \brief Define a string attribute.

    \param attrname The name of the attribute you want to create.
    \param data The value of the attribute you want to create.
    \return bool -- DAL::FAIL or DAL::SUCCESS
  */
  bool dalTable::setAttribute( std::string attrname,
			       std::string * data,
			       int size )
  {
    return h5setAttribute_string( tableID_p, attrname, data, size );
  }

  // ---------------------------------------------------------- listColumns

  /*!
    \brief List the table columns.

    \return a list of columns contained within the opened table.
   */
  std::vector<std::string> dalTable::listColumns()
  {
    std::vector<std::string> colnames;
    colnames.clear();

    if ( type == H5TYPE )
      {
        size_t * field_sizes = NULL;
        size_t * field_offsets = NULL;
        size_t * size_out = NULL;

        // retrieve the input fields needed for the append_records call
        H5TBget_table_info ( fileID_p, name.c_str(), &nfields, &nrecords );

        field_sizes = (size_t*)malloc( nfields * sizeof(size_t) );
        field_offsets = (size_t*)malloc( nfields * sizeof(size_t) );
        size_out = (size_t*)malloc( sizeof(size_t) );

        /* Alocate space */
        field_names = (char**)malloc( sizeof(char*) * (size_t)nfields );
        for ( unsigned int i = 0; i < nfields; i++)
          {
            field_names[i]=(char*)malloc(sizeof(char) * MAX_COL_NAME_SIZE );
          }
        status = H5TBget_field_info( fileID_p, name.c_str(), field_names,
                                     field_sizes, field_offsets, size_out );

        for (unsigned int ii=0; ii<nfields; ii++)
          {
            colnames.push_back( field_names[ii] );
            free(field_names[ii]);
          }
        free(field_names);
        return colnames;
      }
    else if ( type == MSCASATYPE )
      {
#ifdef HAVE_CASA
        casa::TableDesc td = casa_table_handle->tableDesc();
        casa::Vector<casa::String> names = td.columnNames();
        casa::Array<casa::String> myarray = names;
        std::vector<casa::String> foovec;
        myarray.tovector( foovec );
        for ( uint idx=0; idx < foovec.size(); idx++ )
          colnames.push_back( std::string( foovec[ idx ].c_str() ) );
        return colnames;
#else
        std::cerr << "ERROR: CASA support not enabled.\n";
#endif
      }
    else
      {
        std::cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
        return colnames;
      }

  }

  // ------------------------------------------------------------ getNumberOfRows

  /*!
    \brief Get the number of rows in a table.
    
    \return nofRows -- The number of rows in the table as a long precision
            integer.
  */
  long dalTable::getNumberOfRows()
  {
    if ( type == H5TYPE ) {
      if (fileID_p > 0) {
	H5TBget_table_info ( fileID_p, name.c_str(), &nfields, &nrecords );
      } else if (tableID_p > 0) {
	H5TBget_table_info ( tableID_p, name.c_str(), &nfields, &nrecords );
      }
      return nrecords;
    } else {
      std::cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
      return(-1);
    }
  }
  
  // ------------------------------------------------------------------- readRows
  
  /*!
    \brief Read rows from the table.
    
    \param data_out A structure which the data will be read in to.  It should
           match the structure of the table itself.
    \param start The row number to start reading from.
    \param stop The row number where you want to stop reading.
    \param buffersize An optional parameter specifying the size of the output
           buffer.
  */
  void dalTable::readRows( void * data_out,
			   long nstart,
			   long numberRecs,
			   long buffersize )
  {
    if ( type == H5TYPE )
      {
        size_t * field_sizes = NULL;
        size_t * field_offsets = NULL;
        size_t * size_out = NULL;

        // retrieve the input fields needed for the append_records call
        H5TBget_table_info ( fileID_p, name.c_str(), &nfields, &nrecords );

        field_sizes = (size_t*)malloc( nfields * sizeof(size_t) );
        field_offsets = (size_t*)malloc( nfields * sizeof(size_t) );
        size_out = (size_t*)malloc( sizeof(size_t) );
        field_names = (char**)malloc( nfields * sizeof(char*) );
        for (unsigned int ii=0; ii<nfields; ii++)
          {
            field_names[ii] = (char*)malloc(MAX_COL_NAME_SIZE*sizeof(char));
          }

        status = H5TBget_field_info( fileID_p, name.c_str(), field_names,
                                     field_sizes, field_offsets, size_out );

        hsize_t start = nstart;
        hsize_t nrecs = numberRecs;

        if (buffersize > 0)
          size_out[0] = buffersize;
        status = H5TBread_records( fileID_p, name.c_str(), start, nrecs,
                                   size_out[0], field_offsets, field_sizes,
                                   data_out );

        free(field_sizes);
        free(field_offsets);
        free(size_out);
        for (unsigned int ii=0; ii<nfields; ii++)
          {
            free(field_names[ii]);
          }
        free(field_names);
        if (status < 0)
          {
            std::cerr << "ERROR: Problem reading records. Row buffer may be too big."
            << " Make sure the buffer is smaller than the size of the "
            << "table." << endl;
          }
      }
    else
      {
        std::cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
      }
  }

#ifdef HAVE_CASA
  // ---------------------------------------------------------- findAttribute

  /*!
  \brief Find an attribute.

  Look for the prescence of an attribute.

  \param attrname The name of the attribute you want to find.
  \return True if found.  False if not found.
  */
  casa::Bool dalTable::findAttribute( std::string attrname )
  {
    if ( type == H5TYPE )
      {
        if ( H5Aexists( tableID_p, attrname.c_str() ) <= 0 )
          {
            std::cerr << "Attribute " << attrname << " not found." << endl;
            return false;
          }
        else
          {
            return true;
          }
      }
    else
      {
        std::cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
        return false;
      }
  }
#endif

  // ---------------------------------------------------------- getAttribute

  /*!
  \brief Get the value of an attribute.

  Get the value of a table attribute.

  \param attrname The name of the attribute you want to read.
  \return A pointer to the attribute data.
  */
  void * dalTable::getAttribute( std::string attrname )
  {

    if ( type == H5TYPE )
      {
        hsize_t dims;
        H5T_class_t type_class;
        size_t type_size;

        // Check if attribute exists
        if ( H5Aexists( tableID_p, attrname.c_str() ) <= 0 )
          {
            return NULL;
          }

        std::string fullname = "/" + name;

        int rank;
        H5LTget_attribute_ndims( fileID_p, fullname.c_str(),
                                 attrname.c_str(), &rank );

        H5LTget_attribute_info( fileID_p, fullname.c_str(), attrname.c_str(),
                                &dims, &type_class, &type_size );

        if ( H5T_FLOAT == type_class )
          {
            void * data = NULL;
            if ( 0 < H5LTget_attribute(fileID_p, fullname.c_str(),
                                       attrname.c_str(),
                                       H5T_NATIVE_DOUBLE, data) )
              return NULL;
            else
              return reinterpret_cast<double*>(data);
          }
        else if ( H5T_INTEGER == type_class )
          {
            void * data = NULL;
            if ( 0 < H5LTget_attribute(fileID_p, fullname.c_str(),
                                       attrname.c_str(),
                                       H5T_NATIVE_INT, data) )
              return NULL;
            else
              return reinterpret_cast<int*>(data);
          }
        else if ( H5T_STRING == type_class )
          {
            char* data = NULL;
            std::string fullname = "/" + name;
            data = (char *)malloc(rank * sizeof(char));
            if ( 0 < H5LTget_attribute_string( fileID_p, fullname.c_str(),
                                               attrname.c_str(),data) )
              return NULL;
            else
              return data;
          }
        else
          {
            return NULL;
          }
      }
    else if ( type == MSCASATYPE )
      {
#ifdef HAVE_CASA
        casa::TableRecord table_rec = casa_table_handle->keywordSet();
        casa::String dtype = GetKeywordType(attrname);
        if (dtype == "Float")
          {
            casa::Float result;
            casa::Float * result_p = &result;
            if (GetKeyword(attrname,&result))
              return result_p;
            else
              {
                std::cerr << "ERROR: dalTable:GetKeyword Could not get '" << attrname << endl;
                return NULL;
              }
          }
        else
          {
            std::cerr << "(TBD) datatype is " << type << endl;
            return NULL;
          }
#else
        std::cerr << "ERROR: CASA not enabled [dalTable::getAttribute()]" << endl;
        return NULL;
#endif
      }
    else
      {
        std::cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
        return NULL;
      }

    return NULL;
  }

#ifdef HAVE_CASA

  // ---------------------------------------------------------- GetKeyword

  /*!
    \brief Retrieve a casa keykword.

    Retrieve a casa keykword.

    \param KeywordName Name of the keyword to retrieve.
    \param result Keyword value.
    \return Casa boolean value. True on success. False on fail.
   */

  casa::Bool dalTable::GetKeyword(casa::String const KeywordName,
				  casa::String *result)
  {
#ifdef DEBUGGING_MESSAGES
    std::cerr << "dalTable::GetKeyword called for " << KeywordName << endl;
#endif
    try
      {
        if (!casa_table_handle->keywordSet().isDefined(KeywordName))
          {
            std::cerr << "dalTable::GetKeyword: Keyword named \"" << KeywordName <<
            "\" does not exist" << endl;
            return casa::False;
          };
        *result = casa_table_handle->keywordSet().asString(KeywordName);
      }
    catch (casa::AipsError x)
      {
        std::cerr << "dalTable::GetKeyword: " << x.getMesg() << endl;
        return casa::False;
      };
    return casa::True;
  }

  // ---------------------------------------------------------- GetKeyword

  /*!
    \brief Retrieve a casa keykword.

    Retrieve a casa keykword.

    \param KeywordName Name of the keyword to retrieve.
    \param result Keyword value.
    \return Casa boolean value. True on success. False on fail.
   */

  casa::Bool dalTable::GetKeyword(casa::String const KeywordName,
				  casa::Double *result)
  {
#ifdef DEBUGGING_MESSAGES
    std::cerr << "dalTable::GetKeyword called for " << KeywordName << endl;
#endif
    try
      {
        if (!casa_table_handle->keywordSet().isDefined(KeywordName))
          {
            std::cerr << "dalTable::GetKeyword: Keyword named \"" << KeywordName <<
            "\" does not exist" << endl;
            return casa::False;
          };
        *result = casa_table_handle->keywordSet().asDouble(KeywordName);
      }
    catch (casa::AipsError x)
      {
        std::cerr << "dalTable::GetKeyword: " << x.getMesg() << endl;
        return casa::False;
      };
    return casa::True;
  }

  // ---------------------------------------------------------- GetKeyword

  /*!
    \brief Retrieve a casa keykword.

    Retrieve a casa keykword.

    \param KeywordName Name of the keyword to retrieve.
    \param result Keyword value.
    \return Casa boolean value. True on success. False on fail.
   */

  casa::Bool dalTable::GetKeyword(casa::String const KeywordName,
				  casa::Float *result)
  {
#ifdef DEBUGGING_MESSAGES
    std::cerr << "dalTable::GetKeyword called for " << KeywordName << endl;
#endif
    try
      {
        if (!casa_table_handle->keywordSet().isDefined(KeywordName))
          {
            std::cerr << "dalTable::GetKeyword: Keyword named \"" << KeywordName <<
            "\" does not exist" << endl;
            return casa::False;
          };
        *result = casa_table_handle->keywordSet().asFloat(KeywordName);
      }
    catch (casa::AipsError x)
      {
        std::cerr << "dalTable::GetKeyword: " << x.getMesg() << endl;
        return casa::False;
      };
    return casa::True;
  }

  // ---------------------------------------------------------- GetKeyword

  /*!
    \brief Retrieve a casa keykword.

    Retrieve a casa keykword.

    \param KeywordName Name of the keyword to retrieve.
    \param result Keyword value.
    \return Casa boolean value. True on success. False on fail.
   */

  casa::Bool dalTable::GetKeyword(casa::String const KeywordName,
				  casa::DComplex *result)
  {
#ifdef DEBUGGING_MESSAGES
    std::cerr << "dalTable::GetKeyword called for " << KeywordName << endl;
#endif
    try
      {
        if (!casa_table_handle->keywordSet().isDefined(KeywordName))
          {
            std::cerr << "dalTable::GetKeyword: Keyword named \"" << KeywordName <<
            "\" does not exist" << endl;
            return casa::False;
          };
        *result = casa_table_handle->keywordSet().asDComplex(KeywordName);
      }
    catch (casa::AipsError x)
      {
        std::cerr << "dalTable::GetKeyword: " << x.getMesg() << endl;
        return casa::False;
      };
    return casa::True;
  }

  // ---------------------------------------------------------- GetKeyword

  /*!
    \brief Retrieve a casa keykword.

    Retrieve a casa keykword.

    \param KeywordName Name of the keyword to retrieve.
    \param result Keyword value.
    \return Casa boolean value. True on success. False on fail.
   */

  casa::Bool dalTable::GetKeyword(casa::String const KeywordName,
				  casa::Array<casa::Double> *result)
  {
#ifdef DEBUGGING_MESSAGES
    std::cerr << "dalTable::GetKeyword called for " << KeywordName << endl;
#endif
    try
      {
        if (!casa_table_handle->keywordSet().isDefined(KeywordName))
          {
            std::cerr << "dalTable::GetKeyword: Keyword named \"" << KeywordName <<
            "\" does not exist" << endl;
            return casa::False;
          };
        result->reference(casa_table_handle->keywordSet().asArrayDouble(KeywordName));
      }
    catch (casa::AipsError x)
      {
        std::cerr << "dalTable::GetKeyword: " << x.getMesg() << endl;
        return casa::False;
      };
    return casa::True;
  }

  // ---------------------------------------------------------- GetKeyword

  /*!
    \brief Retrieve a casa keykword.

    Retrieve a casa keykword.

    \param KeywordName Name of the keyword to retrieve.
    \param result Keyword value.
    \return Casa boolean value. True on success. False on fail.
   */

  casa::Bool dalTable::GetKeyword(casa::String const KeywordName,
				  casa::Array<casa::DComplex> *result)
  {
#ifdef DEBUGGING_MESSAGES
    std::cerr << "dalTable::GetKeyword called for " << KeywordName << endl;
#endif
    try
      {
        if (!casa_table_handle->keywordSet().isDefined(KeywordName))
          {
            std::cerr << "dalTable::GetKeyword: Keyword named \"" << KeywordName <<
            "\" does not exist" << endl;
            return casa::False;
          };
        result->reference(casa_table_handle->keywordSet().asArrayDComplex(KeywordName));
      }
    catch (casa::AipsError x)
      {
        std::cerr << "dalTable::GetKeyword: " << x.getMesg() << endl;
        return casa::False;
      };
    return casa::True;
  }


  // ---------------------------------------------------------- GetKeywordType

  // ==============================================================================
  // Get the type of the given keyword
  // ==============================================================================
  /*!
    \brief Get the type of the given casa keyword.

    Get the type of the given casa keyword.

    \param KeywordName Name of the keyword to retrieve.
    \return String containing the keyword type.
   */

  casa::String dalTable::GetKeywordType(casa::String const KeywordName)
  {
    try
      {
        if (!casa_table_handle->keywordSet().isDefined(KeywordName))
          {
            std::cerr << "dalTable::GetKeywordType: Keyword named \"" << KeywordName <<
            "\" does not exist" << endl;
            return "";
          };
        casa::DataType type = casa_table_handle->keywordSet().dataType(KeywordName);
        switch (type)
          {
          case casa::TpString:
            return "String";
          case casa::TpFloat:
            return "Float";
          case casa::TpDouble:
            return "Double";
          case casa::TpDComplex:
            return "DComplex";
          case casa::TpArrayDouble:
            return "Array<Double>";
          case casa::TpArrayDComplex:
            return "Array<DComplex>";
          case casa::TpInt:
            return "Int";
          case casa::TpUInt:
            return "uInt";
          default:
            return "unknown";
          }
      }
    catch (casa::AipsError x)
      {
        std::cerr << "dalTable::GetKeywordType: " << x.getMesg() << endl;
        return "unknown";
      };
  }
#endif // HAVE_CASA

  /************************************************************************
   *
   * The following functions are boost wrappers to allow some previously
   *   defined functions to be easily called from a python prompt.
   *
   ************************************************************************/
#ifdef PYTHON

  // ---------------------------------------------------------- ot_hdf5

  /****************************************************************
   *  wrapper for openTable (hdf5)
   *
   *****************************************************************/
  void dalTable::ot_hdf5( void * voidfile, std::string tablename, std::string groupname )
  {
    openTable( voidfile, tablename, groupname );
  }

  // -------------------------------------------------------- append_row_boost

  /****************************************************************
   *  wrapper for appendRow (hdf5)
   *
   *****************************************************************/
  bool dalTable::append_row_boost( bpl::object data )
  {	
//  	printf("list size: %d\n",PyList_Size( data.ptr() ) );
    return append_rows_boost(data,1);
  }
  
  /****************************************************************
   *  wrapper for appendRows (hdf5)
   *
   *****************************************************************/
  bool dalTable::append_rows_boost( bpl::object data, long nrows )
  {	
    appendRows(data.ptr(), nrows);
    return PyList_Check(data.ptr());
  }

  void dalTable::write_col_by_index_boost( bpl::numeric::array data, int index,
                                           int rownum, long nrecords )
  {
      void * mydata = num_util::data(data);
  	  writeDataByColNum( mydata, index, rownum, nrecords );
  }

//  bool dalTable::append_row_boost( bpl::numeric::array data )
//  {
//  	void * mydata = num_util::data(data);
//    appendRow(mydata);
//    return true;
//  }

  // ------------------------------------------------------- listColumns_boost

  /****************************************************************
   *  wrapper for listColumns
   *
   *****************************************************************/
  bpl::list dalTable::listColumns_boost()
  {
    std::vector<std::string> cols = listColumns();
    std::vector<int> mydims;
    mydims.push_back( cols.size() );
    bpl::list mylist;
    for ( uint idx=0; idx < cols.size(); idx++ )
      mylist.append( cols[ idx ].c_str() );
    return mylist;
  }


  bool dalTable::setAttribute_char( std::string attrname, char data )
  {
     return setAttribute( attrname, &data );
  }
  bool dalTable::setAttribute_short( std::string attrname, short data )
  {
     return setAttribute( attrname, &data );
  }
  bool dalTable::setAttribute_int( std::string attrname, int data )
  {
     return setAttribute( attrname, &data );
  }
  bool dalTable::setAttribute_uint( std::string attrname, uint data )
  {
     return setAttribute( attrname, &data );
  }
  bool dalTable::setAttribute_long( std::string attrname, long data )
  {
     return setAttribute( attrname, &data );
  }
  bool dalTable::setAttribute_float( std::string attrname, float data )
  {
     return setAttribute( attrname, &data );
  }
  bool dalTable::setAttribute_double( std::string attrname, double data )
  {
     return setAttribute( attrname, &data );
  }
  bool dalTable::setAttribute_string( std::string attrname, std::string data )
  {
     return setAttribute( attrname, &data );
  }

#ifdef HAVE_CASA

  // ---------------------------------------------------------- ot_nonMStable

  /****************************************************************
   *  wrapper for openTable
   *    This opens a casa table, but not in a MeasurementSet
   *
   *****************************************************************/
  void dalTable::ot_nonMStable( std::string tablename )
  {
    openTable( tablename );
  }

  /******************************************************
   * wrappers for setFilter
   ******************************************************/
  // -------------------------------------------------------- setFilter_boost1

  void dalTable::setFilter_boost1( std::string columns )
  {
    setFilter( columns );
  }

  // -------------------------------------------------------- setFilter_boost2

  void dalTable::setFilter_boost2( std::string columns, std::string conditions )
  {
    setFilter( columns, conditions );
  }

#endif // HAVE_CASA

  // ----------------------------------------------------- getAttribute_boost

  bpl::numeric::array dalTable::getAttribute_boost(std::string attrname)
  {
    std::vector<int> mydims;
    mydims.push_back(1);
    float * dta = (float*)getAttribute(attrname);
    return num_util::makeNum(((float*)dta),mydims);
  }
  
//  bpl::numeric::array dalTable::readRows_boost( long start, long nrecs )
//  {
//  	void * data_out;
//    readRows((int*)data_out, start, nrecs);
//    return num_util::makeNum(data_out);
//  }

#endif // PYTHON

} // end namespace DAL
