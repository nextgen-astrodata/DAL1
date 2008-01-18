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

#ifdef PYTHON
#include <num_util.h>
#endif

/****************************************************************
 *  Sub-routine to open a FITS file
 *
 *****************************************************************/
int openFITS( char * fname )
{
	fitsfile * fptr;
	int status = 0;  /* MUST initialize status */
	int nkeys, ii;
	char card[500]; 
	//cout << "Try to open FITS...";
	
	fits_open_file(&fptr, fname, READWRITE, &status);
	
	//cout << "fits open file: " << status << endl;
	
	if (status)          /* print any error messages */
	{
	  //fits_report_error(stderr, status);
// 	  cout << " failed" << endl;
	  return status;
	}
	else
	{
	  cout << "opened a FITS file, closing" << endl;
        fits_get_hdrspace(fptr, &nkeys, NULL, &status);

        for (ii = 1; ii <= nkeys; ii++)  { 
          fits_read_record(fptr, ii, card, &status); /* read keyword */
          printf("%s\n", card);
        }
        printf("END\n\n");  /* terminate listing with END */

	  fits_close_file(fptr, &status);
	
	  return 0;
	}
}

/**
 * 
 * @param fname 
 * @return hid_t
 */
/****************************************************************
 *  Sub-routine to open a HDF5 file
 *
 *****************************************************************/
hid_t openHDF5( char * fname )
{
	// the following return an integer file handle
	// Turn off error reporting since we expect failure in cases
	//   where the file is not hdf5
	H5Eset_auto(NULL, NULL);
	hid_t fh = H5Fopen(fname, H5F_ACC_RDWR, H5P_DEFAULT);
	if (fh >= 0)
	{
		return fh;
	}
	else
	{
		return -1;
	}
}

/**
 * 
 * @param filename 
 * @return int
 */
/****************************************************************
 *  Opens a dataset (file)
 *
 *****************************************************************/
int dalDataset::open( char * filename )
{
  string lcltype;
//  void * myfile;
  
// cout << "trying to open fits..." << endl;
  if ( 0 == openFITS( filename ) )
  {
//     file = (fitsfile*)myfile;
    lcltype = FITSTYPE;
    type = lcltype;
    cout << lcltype << " file opened, but other FITS operations are not "
      << "yet supported.  Sorry." << endl;
	exit(10);
  } else if ( (h5fh = openHDF5( filename )) >= 0 ) {
     file = &h5fh;
     lcltype = H5TYPE;
     type = lcltype;
     return SUCCESS;
  } else {
#ifdef WITH_CASA
     try {
        lcltype = MSCASATYPE;
        type = lcltype;
        ms = new casa::MeasurementSet( filename );
        file = &ms;
        ms_reader = new casa::MSReader( *ms );
        return SUCCESS;
     } catch (casa::AipsError x) {
//         cout << "ERROR: " << x.getMesg() << endl;
        cout << "Unable to open file \"" << filename << "\"."
    	  << endl << "Please check file name, permissions and type.\n" << endl;  
	return FAIL;
     }
#else
     return FAIL;
#endif
  }
}

/****************************************************************
 *  Closes a dataset (file)
 *
 *****************************************************************/
int dalDataset::close()
{
   if ( type == MSCASATYPE )
   {
#ifdef WITH_CASA
// 	delete ms_reader;
	delete ms;
// 	cout << "closed MS" << endl;
#else
	cout << "CASA support not enabled." << endl;
	exit(-1);
#endif
   }
   else if ( type == H5TYPE )
   {
      // << "Try to close HDF5... ";

//       if ( H5Fclose(*(hid_t*)file ) > 0 )
// cout << "h5fh: " << h5fh << endl;
      if ( H5Fclose( h5fh ) > 0 )
        return 0;
      else
	return -1;
    }
    return -2;
}

/*
 * Operator function.
 */
herr_t 
_dalDataset_attr_info(hid_t loc_id, const char *name, void *opdata)
{
    hid_t attr, atype, aspace;  /* Attribute, datatype, dataspace identifiers */
    int   rank;
    hsize_t sdim[64]; 
    herr_t ret;
    int i;
    size_t size;
    size_t npoints;             /* Number of elements in the array attribute. */ 
    int point_out;    
    float *float_array;         /* Pointer to the array attribute. */
    H5S_class_t  type_class;

    /* avoid warnings */
    opdata = opdata;

    /*  Open the attribute using its name.  */    
    attr = H5Aopen_name(loc_id, name);

    /*  Display attribute name.  */
    //printf("\nName : ");
    //puts(name);

    /* Get attribute datatype, dataspace, rank, and dimensions.  */
    atype  = H5Aget_type(attr);
    aspace = H5Aget_space(attr);
    rank = H5Sget_simple_extent_ndims(aspace);
    ret = H5Sget_simple_extent_dims(aspace, sdim, NULL);

    /* Get dataspace type */
    type_class = H5Sget_simple_extent_type (aspace);
    //printf ("H5Sget_simple_extent_type (aspace) returns: %i\n", type_class);

    /* Display rank and dimension sizes for the array attribute.  */
    if(rank > 0) {
       //printf("Rank : %d \n", rank); 
       //printf("Dimension sizes : ");
       //for (i=0; i< rank; i++) printf("%d ", (int)sdim[i]);
       //printf("\n");
    }

    if (H5T_INTEGER == H5Tget_class(atype)) {
       //printf("Type : INTEGER \n");
       ret  = H5Aread(attr, atype, &point_out);
       //printf("The value of the attribute \"Integer attribute\" is %d \n", 
         //      point_out);
	cout << name << " = " << point_out << endl;
    }

    if (H5T_FLOAT == H5Tget_class(atype)) {
       //printf("Type : FLOAT \n"); 
       npoints = H5Sget_simple_extent_npoints(aspace);
       float_array = (float *)malloc(sizeof(float)*(int)npoints); 
       ret = H5Aread(attr, atype, float_array);
       //printf("Values : ");
	cout << name << " = ";
       for( i = 0; i < (int)npoints; i++) printf("%f ", float_array[i]); 
       printf("\n");
       free(float_array);
    }

    if (H5T_STRING == H5Tget_class (atype)) {
      //printf ("Type: STRING \n");
      size = H5Tget_size (atype);

	char* data;
	data = (char *)malloc(rank * sizeof(char));
      	ret = H5Aread(attr, atype, data);
	cout << name << " = " << data << endl;

    }

    ret = H5Tclose(atype);
    ret = H5Sclose(aspace);
    ret = H5Aclose(attr);

    return 0;
}

void dalDataset::getAttributes()
{

   //status = H5Aget_num_attrs(h5fh);
   //printf ("H5Aget_num_attrs returns: %i\n", status);

   status = H5Aiterate(h5fh,NULL ,_dalDataset_attr_info, NULL);
   //printf ("\nH5Aiterate returns: %i\n", status);

}

void dalDataset::printAttribute( string attrname ) {

	hsize_t * dims;
	H5T_class_t type_class;
	size_t type_size;

	// Check if attribute exists
	if ( H5LT_find_attribute(h5fh, attrname.c_str()) <= 0 ) {
		cout << "Attribute " << attrname << " not found." << endl;
		return;
	}
	
	string fullname = "/" + name;

	int rank;
	H5LTget_attribute_ndims(h5fh, fullname.c_str(), attrname.c_str(),
				&rank );

	dims = (hsize_t *)malloc(rank * sizeof(hsize_t));

	H5LTget_attribute_info( h5fh, fullname.c_str(), attrname.c_str(),
				dims, &type_class, &type_size );

	if ( H5T_FLOAT == type_class ) {
		double data[*dims];
		H5LTget_attribute(h5fh, fullname.c_str(), attrname.c_str(),
			 H5T_NATIVE_DOUBLE, data);
		cout << attrname << " = ";
		for (unsigned int ii=0; ii<*dims; ii++) {
		  cout << data[ii];
		  if (ii < (*dims)-1)
		    cout << ',';
		  else
		    cout << endl;
		}
	}
	else if ( H5T_INTEGER == type_class ) {
		int data[*dims];
		H5LTget_attribute(h5fh, fullname.c_str(), attrname.c_str(),
			 H5T_NATIVE_INT, data);
		cout << attrname << " = ";
		for (unsigned int ii=0; ii<*dims; ii++) {
		  cout << data[ii];
		  if (ii < (*dims)-1)
		    cout << ',';
		  else
		    cout << endl;
		}
	}
	else if ( H5T_STRING == type_class ) {
		char* data;
		string fullname = "/" + name;
		data = (char *)malloc(rank * sizeof(char));
		H5LTget_attribute_string( h5fh, fullname.c_str(), 
					  attrname.c_str(),data);
		cout << attrname << " = " << data << endl;
	}
	else {
		cout << "Attribute " << attrname << " type unknown." << endl;
	}
}

void * dalDataset::getAttribute( string attrname ) {

	hsize_t dims;
	H5T_class_t type_class;
	size_t type_size;

	// Check if attribute exists
	if ( H5LT_find_attribute(h5fh, attrname.c_str()) <= 0 ) {
		return NULL;
	}
	
	string fullname = "/" + name;

	int rank;
	H5LTget_attribute_ndims(h5fh, fullname.c_str(), attrname.c_str(),
				&rank );

	H5LTget_attribute_info( h5fh, fullname.c_str(), attrname.c_str(),
				&dims, &type_class, &type_size );

	if ( H5T_FLOAT == type_class ) {
		void * data;
		if ( 0 < H5LTget_attribute(h5fh, fullname.c_str(), attrname.c_str(),
			 H5T_NATIVE_DOUBLE, data) )
		  return NULL;
		else
		  return reinterpret_cast<double*>(data);
	}
	else if ( H5T_INTEGER == type_class ) {
		void * data;
		if ( 0 < H5LTget_attribute(h5fh, fullname.c_str(), attrname.c_str(),
			 H5T_NATIVE_INT, data) )
		  return NULL;
		else
		  return reinterpret_cast<int*>(data);
	}
	else if ( H5T_STRING == type_class ) {
		char* data;
		string fullname = "/" + name;
		data = (char *)malloc(rank * sizeof(char));
		if ( 0 < H5LTget_attribute_string( h5fh, fullname.c_str(), 
			  attrname.c_str(),data) )
		  return NULL;
		else
		  return data;
	}
	else {
		return NULL;
	}
}

void dalDataset::setAttribute_string( string attrname, string data )
{
	if ( H5LTset_attribute_string( h5fh, "/",
					attrname.c_str(), data.c_str() ) < 0 ) {
		cout << "ERROR: could not set attribute " << attrname << endl;
	}
}

void dalDataset::setAttribute_int( string attrname, int * data, int size )
{
// 	int size=1;
	if ( H5LTset_attribute_int( h5fh, "/",
		attrname.c_str(), /*&*/data, size ) < 0 ) {
		cout << "ERROR: could not set attribute " << attrname << endl;
	}
}

void dalDataset::setAttribute_uint( string attrname, unsigned int * data, int size ) {
// 	int size=1;
	if ( H5LTset_attribute_uint( h5fh, "/",
					attrname.c_str(), /*&*/data, size ) < 0 ) {
		cout << "ERROR: could not set attribute " << attrname << endl;
	}
}

void dalDataset::setAttribute_double( string attrname, double * data, int size )
{
// 	int size=1;
	if ( H5LTset_attribute_double( h5fh, "/",
					attrname.c_str(), /*&*/data, size ) < 0 ) {
		cout << "ERROR: could not set attribute " << attrname << endl;
	}
}

/****************************************************************
 *  Creates a group (mainly for hdf5)
 *
 *****************************************************************/
dalGroup * dalDataset::createGroup( char* gname )
{
    if ( type == H5TYPE )
   {
	hid_t * lclfile = &h5fh;
	   dalGroup * lg = new dalGroup( gname, lclfile );
	   return lg;
   }
   else
   	return NULL;  
}

/****************************************************************
 *  Default constructor
 *
 *****************************************************************/
dalDataset::dalDataset()
{
  type = "UNDEFINED";
  name = "UNDEFINED";
#ifdef PYTHON
    Py_Initialize();
#endif
    filter = new dalFilter;
}

/****************************************************************
 *  List tables in a dataset (file)
 *
 *****************************************************************/
void dalDataset::listTables()
{
   if ( type == MSCASATYPE )
   {
#ifdef WITH_CASA
	ms_tables = ms_reader->tables();

	// list the names of the tables
	cout << ms_tables << endl;
#else
	cout << "CASA support not enabled." << endl;
	exit(-1);
#endif
   }
   else
   {
	cout << "This operation is not supported for filetype " << type << endl;
   }
}

/*!
 *
 *  Create a a new dataset (file)
 */
dalDataset::dalDataset( char * name, string filetype )
{
   type = filetype;  // set the global class variable: type
   filter = new dalFilter;

   if ( filetype == H5TYPE )
   {

	h5fh = H5Fcreate(name, H5F_ACC_TRUNC, H5P_DEFAULT,
				   H5P_DEFAULT);
        file = &h5fh;
	
   } else if ( filetype == FITSTYPE )
   {
	fitsfile *fptr; /* pointer to the FITS file; defined in fitsio.h */
	int status;
	fits_create_file(&fptr, name, &status); /*create new file*/
// h5fh = fits_create_file(fitsfile **fptr, char *filename, > int *status);
   }
   else {
   	cout << "(dalDataset::dalDataset) Data format \'" << type
             << "\' not supported for this operation." << endl;
   }
}

/****************************************************************
 *  Default destructor
 *
 *****************************************************************/
dalDataset::~dalDataset()
{
}

void dalDataset::setFilter( string columns )
{
    filter = new dalFilter;
    filter->setFiletype( type );
    filter->set(columns);
}

void dalDataset::setFilter( string columns, string conditions )
{
    filter->setFiletype( type );
    filter->set(columns,conditions);
}

dalArray * dalDataset::createArray( string arrayname, dalData * data_object )
{
  vector<int> cdims;
  complex<float> * cdata;
  long indx;
  long mysize = 1;

  for (unsigned int ll=0; ll<data_object->shape.size(); ll++)
     mysize *= data_object->shape[ll];

  cdata = new complex<float>[ mysize ];
  for(int xx=0; xx<data_object->shape[0]; xx++)
    for(int yy=0; yy<data_object->shape[1]; yy++)
      for(int zz=0; zz<data_object->shape[2]; zz++)
      {
        indx = data_object->c_index(xx,yy,zz);
        cdata[ indx ] = *( (complex<float>*)data_object->get(xx,yy,zz) );
      }

   if ( dal_COMPLEX == data_object->get_datatype() )
      return createComplexArray( arrayname, data_object->shape, cdata, cdims);
   else
   {
// ADD MORE DATATYPES HERE!
      cout << "(dalDataset::createArray) This datatype is not yet supported."
           << endl;
      exit(99);
      return NULL;
   }
}


/****************************************************************
 *  Create an Integer array of N dimensions
 *
 *****************************************************************/
dalArray * dalDataset::createIntArray( string arrayname, vector<int> dims, int data[], vector<int> cdims)
{
   if ( type == H5TYPE )
   {
	dalIntArray * la = new dalIntArray( h5fh, arrayname, dims, data, cdims );
	return la;
   }
   else if ( type == FITSTYPE )
   {
	cout << "dalDataset::createIntArray FITS Type not yet supported." << endl;
	return NULL;
   }
   else
   {
   	cout << "(dalDataset::createIntArray) Data format \'" << type
             << "\' not supported." << endl;
   	exit(97);
   }
}

/****************************************************************
 *  Create an Float array of N dimensions
 *
 *****************************************************************/
dalArray * dalDataset::createFloatArray( string arrayname, vector<int> dims, float data[], vector<int> cdims)
{
   if ( type == H5TYPE )
   {
	dalFloatArray * la =
	   new dalFloatArray( h5fh, arrayname, dims, data, cdims );
	return la;
   }
   else
   {
	cout << "(dalDataset::createFloatArray) Filetype \'" << type
             << "\' not yet supported."
             << endl;
	return NULL;
   }
}

/****************************************************************
 *  Create an Copmlex array of N dimensions
 *
 *****************************************************************/
dalArray * dalDataset::createComplexArray( string arrayname, vector<int> dims, /*vector< complex<float> >*/complex<float> data[]/*dalcomplex data[]*/, vector<int> cdims)
{
   if ( type == H5TYPE )
   {
	dalComplexArray * la = new dalComplexArray(
	   file, arrayname, dims, data, cdims );
	return la;
   }
   else
   {
	cout << "(dalDataset::createComplexArray) Filetype \'" << type
             << "\' not yet supported."
             << endl;
	return NULL;
   }
}

/****************************************************************
 *  Creates a table at the highest level of a dataset
 *
 *****************************************************************/
dalTable * dalDataset::createTable( string tablename )
{
   if ( type == H5TYPE )
   {
	   dalTable * lt = new dalTable( H5TYPE );
	   lt->createTable( file, tablename, "/" );
	   return lt;
   }
   else
   {
	cout << "(dalDataset::createTable) Filetype \'" << type
             << "\' not yet supported."
             << endl;
	return NULL;
   }
}

/****************************************************************
 *  Creates a table in a group (mainly for hdf5)
 *
 *****************************************************************/
dalTable * dalDataset::createTable( string tablename, string groupname )
{
   if ( type == H5TYPE )
   {
	   dalTable * lt = new dalTable( H5TYPE );
	   lt->createTable( file, tablename, '/' + groupname );
	   return lt;
   }
   else
   {
	cout << "(dalDataset::createTable) Filetype \'" << type
             << "\' not yet supported."
             << endl;
	return NULL;
   }
}

/****************************************************************
 *  Opens a table at the higest level of a dataset.
 *
 *****************************************************************/
dalTable * dalDataset::openTable( string tablename )
{
   if ( type == MSCASATYPE )
   {
#ifdef WITH_CASA
	dalTable * lt = new dalTable( MSCASATYPE );
	if ( filter->isSet() )
	  lt->openTable( tablename, ms_reader, filter );
        else
	  lt->openTable( tablename, ms_reader );
	return lt;
#else
	cout << "CASA support not enabled." << endl;
	exit(-1);
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
	cout << "dalDataset::openTable FITS Type not yet supported." << endl;
	return NULL;
   }
   else
   {
	cout << "openTable operation not supported for filetype "
	  << type << endl;
   	return NULL;
   }
}

/****************************************************************
 *  Opens a table in a group (mainly for hdf5)
 *
 *****************************************************************/
dalTable * dalDataset::openTable( string tablename, string groupname )
{
  if ( type == MSCASATYPE )
  {
	cout << "dalDataset::openTable MSCASA Type not yet supported." << endl;
	return NULL;
  }
  else if ( type == H5TYPE )
  {
	dalTable * lt = new dalTable( H5TYPE );
	//cout << "Trying to open table " << endl; 
	lt->openTable( file, tablename, '/' + groupname );
	return lt;
   }
   else if ( type == FITSTYPE )
   {
	cout << "dalDataset::openTable FITS Type not yet supported." << endl;
	return NULL;
   }
   else
   	return NULL;
}

/****************************************************************
 *  Opens an array at the higest level of a dataset.
 *
 *****************************************************************/
dalArray * dalDataset::openArray( string arrayname )
{
  if ( type == MSCASATYPE )
  {
	cout << "dalDataset::openArray MSCASA Type not yet supported." << endl;
	return NULL;
  }
  else if ( type == H5TYPE )
  {
	dalArray * la = new dalArray();
	la->open( file, arrayname );
	return la;
   }
   else if ( type == FITSTYPE )
   {
	cout << "dalDataset::openArray FITS Type not yet supported." << endl;
	return NULL;
   }
   else
   	return NULL;
}

/****************************************************************
 *  Opens an array in a group.
 *
 *****************************************************************/
dalArray * dalDataset::openArray( string arrayname, string groupname )
{
  if ( type == MSCASATYPE )
  {
	cout << "dalDataset::openArray MSCASA Type not yet supported." << endl;
	return NULL;
  }
  else if ( type == H5TYPE )
  {
	dalArray * la = new dalArray();
	la->open( file, '/' + groupname + '/' + arrayname );
	return la;
   }
   else if ( type == FITSTYPE )
   {
	cout << "dalDataset::openArray FITS Type not yet supported." << endl;
	return NULL;
   }
   else
   	return NULL;
}


/*
 * Operator function.
 */
herr_t dalDataset_file_info(hid_t loc_id, const char *name, void *opdata)
{
    H5G_stat_t statbuf;
    string myname;

    /*
     * Get type of the object and display its name and type.
     * The name of the object is passed to this function by 
     * the Library. Some magic :-)
     */
    H5Gget_objinfo(loc_id, name, FALSE, &statbuf);
    switch (statbuf.type) {
    case H5G_GROUP:
         myname = string(name);
         (*(vector<string>*)opdata).push_back( myname );
         break;
    case H5G_DATASET: 
    case H5G_TYPE: 
    default:
         break;
    }
    return 0;
 }

vector<string> dalDataset::getGroupNames()
{
  vector<string> group_names;
  if ( type == H5TYPE )
  {
    H5Giterate(h5fh, "/", NULL, dalDataset_file_info, &group_names);
    return group_names;
  }
  else
  {
    cout << "dalDataset::getGroupNames Filetype not supported." << endl;
    return group_names;
  }
}

/****************************************************************
 *  Opens a group (mainly for hdf5)
 *
 *****************************************************************/
dalGroup * dalDataset::openGroup( string groupname )
{
   if ( type == H5TYPE )
   {
	   dalGroup * group = new dalGroup();
	   //cout << "Trying to open group " << groupname << endl; 
	   int retval = group->open( file, groupname );
	   if ( retval < 0 )
		return NULL;
	   else
	   	return group;
   }
   else if ( type == FITSTYPE )
   {
	cout << "dalDataset::openGroup FITS Type" << endl;
	return NULL;
   }
   else
   {
   	cout << "dalDataset::openGroup type not supported" << endl;
	return NULL;
   }
}

/****************************************************************
 *  Return the type of file the dataset represents
 *    (HDF5, FITS, CASA, etc.)
 *
 *****************************************************************/
string dalDataset::getType()
{
	return type;
}


void dalDataset::read_tbb (string id,
			   int start,
			   int length,
			   short data_out[])
{
  char stid[3]; // station id
  sscanf(id.c_str(),"%3c%*6c",stid);
  char datasetname[18];
  sprintf( datasetname, "Station%c%c%c/%s", stid[0],stid[1],stid[2],id.c_str());
  hid_t dataset = H5Dopen( h5fh, datasetname );
  hid_t filespace = H5Dget_space( dataset );
  if (filespace < 0)
    {
      cout << "ERROR: could not get filespace.(dalDataset::read_tbb)\n";
      exit(9);
    }
  int rank = H5Sget_simple_extent_ndims( filespace );
  if (rank < 0)
    {
      cout << "ERROR: could not get filespace rank.(dalDataset::read_tbb)\n";
      exit(9);
    }

  hsize_t dimsr[1];

  herr_t status_lcl = H5Sget_simple_extent_dims (filespace, dimsr, NULL);
  if (status_lcl < 0)
    {
      cout << "ERROR: could not get filespace dims.(dalDataset::read_tbb)\n";
      exit(9);
    }

  dimsr[0]=length;

  hid_t memspace = H5Screate_simple (rank,dimsr,NULL);
  if (memspace < 0)
    {
      cout << "ERROR: could not create memory space.(dalDataset::read_tbb)\n";
      exit(9);
    }

  hsize_t     offset[1];
  hsize_t     offset_out[1];         /* hyperslab offset in memory */
  offset[0] = start;
  status_lcl = H5Sselect_hyperslab (filespace, H5S_SELECT_SET, offset, NULL, 
                                dimsr, NULL);
  if (status_lcl < 0)
    {
      cout << "ERROR: could not select hyperslab.(dalDataset::read_tbb)\n";
      exit(9);
    }

  offset_out[0] = 0;
  status_lcl = H5Sselect_hyperslab (memspace,
				    H5S_SELECT_SET,
				    offset_out,
				    NULL, 
				    dimsr,
				    NULL);
  if (status_lcl < 0)
    {
      cout << "ERROR: could not select hyperslab.(dalDataset::read_tbb)\n";
      exit(10);
    }

  status_lcl = H5Dread (dataset,
			H5T_NATIVE_SHORT,
			memspace,
			filespace,
			H5P_DEFAULT,
			data_out);
  if (status_lcl < 0)
    {
      cout << "ERROR: could not read array.(dalDataset::read_tbb)\n";
      exit(9);
    }
}


#ifdef PYTHON
/************************************************************************
 *
 * The following functions are boost wrappers to allow some previously
 *   defined functions to be easily called from a python prompt.
 *
 ************************************************************************/

/******************************************************
 * wrappers for createTable
 ******************************************************/

dalTable * dalDataset::ct1_boost(string a) {
	dalTable * ret;
	ret = dalDataset::createTable(a);
	return ret;
}

dalTable * dalDataset::ct2_boost(string a, string b) {
	dalTable * ret;
	ret = dalDataset::createTable(a,b);
	return ret;
}

/******************************************************
 * wrappers for openTable
 ******************************************************/

dalTable * dalDataset::ot1_boost(string a) {
	dalTable * ret;
	ret = openTable(a);
	return ret;
}

dalTable * dalDataset::ot2_boost(string a, string b) {
	dalTable * ret;
	ret = openTable(a,b);
	return ret;
}

/******************************************************
 * wrappers for createIntArray
 ******************************************************/

dalArray * dalDataset::cia_boost1( string arrayname, bpl::list pydims,
				  bpl::list pydata ){
  bpl::list cdims;

  for(int ii=0; ii<bpl::len(pydims); ii++)
     cdims.append(10);

  dalArray * array;
  array = cia_boost2( arrayname, pydims, pydata, cdims );

  return array;
}

dalArray * dalDataset::cia_boost2(
	string arrayname,
	bpl::list pydims,
	bpl::list pydata,
	bpl::list cdims ){

  vector<int> dims;
  vector<int> chnkdims;

  for(int ii=0; ii<bpl::len(pydims); ii++)
    dims.push_back(bpl::extract<int>(pydims[ii]));

  for(int ii=0; ii<bpl::len(cdims); ii++)
    chnkdims.push_back(bpl::extract<int>(cdims[ii]));

  long size = bpl::len(pydata);
  int * data = NULL;
  data = new int[size];

  for(int ii=0; ii<size; ii++)
    data[ii] = bpl::extract<int>(pydata[ii]);

  dalArray * array = createIntArray(arrayname, dims, data, chnkdims);

  delete [] data;

  return array;
}

dalArray * dalDataset::cia_boost_numarray1(
	string arrayname, 
	bpl::list pydims,
	bpl::numeric::array pydata ){

  bpl::list cdims;

  for(int ii=0; ii<bpl::len(pydims); ii++)
     cdims.append(10);

  dalArray * array;
  array = cia_boost_numarray2(arrayname, pydims, pydata, cdims);

  return array;
}

dalArray * dalDataset::cia_boost_numarray2(
	string arrayname, 
	bpl::list pydims,
	bpl::numeric::array pydata,
	bpl::list cdims ){

  vector<int> dims;

  for(int ii=0; ii<bpl::len(pydims); ii++)
    dims.push_back(bpl::extract<int>(pydims[ii]));

  long size = bpl::len(pydata);
  int * data = NULL;
  data = new int[size];

// cout << "cia_boost_numarray "<< pydata.nelements() << endl;
  pydata.setshape( pydata.nelements() );
  bpl::object flat_data = pydata.getflat();
  bpl::list list_data( flat_data );

  dalArray * array = cia_boost2(arrayname, pydims, list_data, cdims);

  return array;
}

/******************************************************
 * wrappers for createFloatArray
 ******************************************************/
dalArray * dalDataset::cfa_boost( string arrayname, bpl::list pydims, bpl::list pydata, bpl::list cdims ){

  vector<int> dims;
  vector<int> chnkdims;

  for(int ii=0; ii<bpl::len(pydims); ii++)
    dims.push_back(bpl::extract<int>(pydims[ii]));

  for(int ii=0; ii<bpl::len(cdims); ii++)
    chnkdims.push_back(bpl::extract<int>(cdims[ii]));

  long size = bpl::len(pydata);
  float * data = NULL;
  data = new float[size];

  for(int ii=0; ii<size; ii++)
    data[ii] = bpl::extract<float>(pydata[ii]);

  dalArray * array = dalDataset::createFloatArray(arrayname, dims, data, chnkdims);

  delete [] data;

  return array;
}

dalArray * dalDataset::cfa_boost_numarray( string arrayname, 
	bpl::list pydims,
	bpl::numeric::array pydata, bpl::list cdims ){

  vector<int> dims;

  for(int ii=0; ii<bpl::len(pydims); ii++)
    dims.push_back(bpl::extract<int>(pydims[ii]));

  long size = bpl::len(pydata);
  float * data = NULL;
  data = new float[size];

// cout << "cia_boost_numarray "<< pydata.nelements() << endl;
  pydata.setshape( pydata.nelements() );
  bpl::object flat_data = pydata.getflat();
  bpl::list list_data( flat_data );

  dalArray * array = dalDataset::cfa_boost(arrayname, pydims, list_data, cdims);

  return array;
}

// void dalDataset::sfe(bpl::numeric::array& y, int value)
// {
//     y[bpl::make_tuple(0,0)] = value;
// }

/******************************************************
 * wrapper for readIntArray
 ******************************************************/

bpl::numeric::array dalDataset::ria_boost( string arrayname )
{
	hid_t lclfile;
	hid_t  status;
// 	hid_t datatype, dataspace;

	// get the dataspace
	lclfile = H5Dopen(h5fh, arrayname.c_str());
	hid_t filespace = H5Dget_space(lclfile);

	// what is the rank of the array?
	hid_t data_rank = H5Sget_simple_extent_ndims(filespace);
	hsize_t dims[ data_rank ];
// cout << "data rank: " << data_rank << endl;
	status = H5Sget_simple_extent_dims(filespace, dims, NULL);

	int size = 1;
	bpl::list dims_list;
	for (int ii=0; ii<data_rank; ii++)
	{
// cout << "dims["  << ii << "]: " << dims[ii] << endl;
	  size *= dims[ii];
	  dims_list.append(dims[ii]);
	}
// cout << "size: " << size << endl;

	int * data = NULL;
	data = new int[size];

	status = H5LTread_dataset_int( h5fh, arrayname.c_str(), data );
// 	for (int ii=0; ii<size; ii++)
// 	{
// 	  cout << data[ii] << endl;
// 	}

	bpl::list data_list;
	// for each dimension
	for (int ii=0; ii<size; ii++)
	{
	    data_list.append(data[ii]);
	}
	bpl::numeric::array nadata(
//           bpl::make_tuple(
	    data_list
// 	  )
	);
// 	dims_list.reverse();
	nadata.setshape(dims_list);
	delete data;
	return nadata;
}

/******************************************************
 * wrapper for readFloatArray
 ******************************************************/

bpl::numeric::array dalDataset::rfa_boost( string arrayname )
{
	hid_t lclfile;
	hid_t status;
// 	hid_t datatype, dataspace;

	// get the dataspace
	lclfile = H5Dopen(h5fh, arrayname.c_str());
	hid_t filespace = H5Dget_space(lclfile);

	// what is the rank of the array?
	hid_t data_rank = H5Sget_simple_extent_ndims(filespace);
	hsize_t dims[ data_rank ];

	status = H5Sget_simple_extent_dims(filespace, dims, NULL);

	int size = 1;
	bpl::list dims_list;
	for (int ii=0; ii<data_rank; ii++)
	{
	  size *= dims[ii];
	  dims_list.append(dims[ii]);
	}

	float * data = NULL;
	data = new float[size];

	status = H5LTread_dataset_float( h5fh, arrayname.c_str(), data );

	bpl::list data_list;
	// for each dimension
	for (int ii=0; ii<size; ii++)
	{
	    data_list.append(data[ii]);
	}
	bpl::numeric::array nadata(
          bpl::make_tuple(
	    bpl::make_tuple(data_list)
	  )
	);
	nadata.setshape(dims_list);
	return nadata;
}

/******************************************************
 * wrappers for setFilter
 ******************************************************/
void dalDataset::setFilter_boost1( string columns )
{
   setFilter( columns );
}

void dalDataset::setFilter_boost2( string columns, string conditions )
{
   setFilter( columns, conditions );
}
#endif
