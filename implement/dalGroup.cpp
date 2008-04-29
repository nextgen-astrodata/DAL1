/*-------------------------------------------------------------------------*
 | $Id:: cs1.h 389 2007-06-13 08:47:09Z baehren                          $ |
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

#ifndef DALGROUP_H
#include "dalGroup.h"
#endif

namespace DAL {

// ==============================================================================
//
//  Construction
//
// ==============================================================================

dalGroup::dalGroup()
{
   file = NULL;
   name = "";
   group = NULL;
   tables.clear();
   attributes.clear();
   filter = NULL;
   file_id = 0;
   group_id = 0;
   status = 0;
   subgroup = NULL;
}

dalGroup::dalGroup( const char * gname, void * voidfile ) {

   dalGroup();

   hid_t * lclfile = (hid_t*)voidfile; // H5File object
   file = lclfile;
   file_id = *lclfile;  // get the file handle

   name = gname;
   string fullgroupname = "/" + stringify(gname);
   group_id = H5Gcreate(*(hid_t*)file, fullgroupname.c_str(), 0);
}

dalGroup::dalGroup( hid_t group_id, const char * gname )
{
   dalGroup();

   group_id = H5Gcreate(group_id, gname, 0);
}

int dalGroup::open( void * voidfile, string groupname ) {

  name = groupname;

  hid_t * lclfile = (hid_t*)voidfile; // H5File object
  file = lclfile;
  file_id = *lclfile;  // get the file handle

  string fullgroupname = "/" + groupname;
  group_id = H5Gopen( file_id, fullgroupname.c_str() );
  return( group_id );
}

dalGroup::~dalGroup()
{
  if ( 0 != group_id )
    if ( H5Gclose(group_id) < 0 )
      cout << "ERROR: dalGroup::close() failed.\n";
}

hid_t dalGroup::getId() {
	return group_id;
}

/*
 * Operator function.
 */
herr_t dalGroup_file_info(hid_t loc_id, const char *name, void *opdata)
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
    case H5G_DATASET: 
         myname = string(name);
         (*(vector<string>*)opdata).push_back( myname );
         break;
    case H5G_GROUP:
    case H5G_TYPE: 
    default:
         break;
    }
    return 0;
 }

vector<string> dalGroup::getMemberNames()
{
  vector<string> member_names;
  H5Giterate(file_id, name.c_str(), NULL, dalGroup_file_info, &member_names);
  return member_names;
}

bool dalGroup::setName ( string gname ) {
   if ( gname.length() > 0 )
   {
     name = gname;
     return DAL::SUCCESS;
   }
   else
   {
     cout << "Error:  Group name must not be empty." << endl;
     return DAL::FAIL;
   }
}

dalArray * dalGroup::createShortArray( string arrayname,
				     vector<int> dims,
				     short data[],
				     vector<int> cdims )
{
	   dalShortArray * la;
	   la = new dalShortArray( group_id, arrayname, dims, data, cdims );
	   return la;
}


dalArray * dalGroup::createIntArray( string arrayname,
				     vector<int> dims,
				     int data[],
				     vector<int> cdims )
{
	   dalIntArray * la;
	   la = new dalIntArray( group_id, arrayname, dims, data, cdims );
	   return la;
}

dalArray * dalGroup::createFloatArray( string arrayname, vector<int> dims, float data[], vector<int> cdims)
{
// cout << "group_id: " << group_id << endl;
	   dalFloatArray * la;
	   la = new dalFloatArray( group_id, arrayname, dims, data, cdims );
	   return la;
}

string dalGroup::getName () {
   return name;
}

void dalGroup::setAttribute_string( string attrname, string data )
{
	if ( H5LTset_attribute_string( file_id, name.c_str(),
					attrname.c_str(), data.c_str() ) < 0 ) {
		cout << "ERROR: could not set attribute " << attrname << endl;
	}
}

void dalGroup::setAttribute_int( string attrname, int * data, int size )
{
// 	int size=1;
	if ( H5LTset_attribute_int( file_id, name.c_str(),
		attrname.c_str(), /*&*/data, size ) < 0 ) {
		cout << "ERROR: could not set attribute " << attrname << endl;
	}
}

void dalGroup::setAttribute_uint( string attrname, unsigned int * data, int size ) {
// 	int size=1;
	if ( H5LTset_attribute_uint( file_id, name.c_str(),
					attrname.c_str(), /*&*/data, size ) < 0 ) {
		cout << "ERROR: could not set attribute " << attrname << endl;
	}
}

void dalGroup::setAttribute_double( string attrname, double * data, int size )
{
// 	int size=1;
	if ( H5LTset_attribute_double( file_id, name.c_str(),
					attrname.c_str(), /*&*/data, size ) < 0 ) {
		cout << "ERROR: could not set attribute " << attrname << endl;
	}
}

void dalGroup::getAttributes()
{

   //status = H5Aget_num_attrs(group_id);
   //printf ("H5Aget_num_attrs returns: %i\n", status);

   status = H5Aiterate( group_id, NULL, attr_info, NULL );
   //printf ("\nH5Aiterate returns: %i\n", status);

}

void dalGroup::printAttribute( string attrname ) {

	hsize_t * dims;
	H5T_class_t type_class;
	size_t type_size;

	// Check if attribute exists
	if ( H5LT_find_attribute(group_id, attrname.c_str()) <= 0 ) {
		cout << "Attribute " << attrname << " not found." << endl;
		return;
	}
	
	string fullname = "/" + name;

	int rank;
	H5LTget_attribute_ndims(file_id, fullname.c_str(), attrname.c_str(),
				&rank );

	dims = (hsize_t *)malloc(rank * sizeof(hsize_t));

	H5LTget_attribute_info( file_id, fullname.c_str(), attrname.c_str(),
				dims, &type_class, &type_size );

	if ( H5T_FLOAT == type_class ) {
		double data[*dims];
		H5LTget_attribute(file_id, fullname.c_str(), attrname.c_str(),
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
		H5LTget_attribute(file_id, fullname.c_str(), attrname.c_str(),
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
		H5LTget_attribute_string( file_id, fullname.c_str(), 
					  attrname.c_str(),data);
		cout << attrname << " = " << data << endl;
	}
	else {
		cout << "Attribute " << attrname << " type unknown." << endl;
	}
}

void * dalGroup::getAttribute( string attrname ) {

	hsize_t dims;
	H5T_class_t type_class;
	size_t type_size;

	// Check if attribute exists
	if ( H5LT_find_attribute(group_id, attrname.c_str()) <= 0 ) {
		return NULL;
	}
	
	string fullname = "/" + name;

	int rank;
	H5LTget_attribute_ndims(file_id, fullname.c_str(), attrname.c_str(),
				&rank );

	H5LTget_attribute_info( file_id, fullname.c_str(), attrname.c_str(),
				&dims, &type_class, &type_size );

	if ( H5T_FLOAT == type_class ) {
		double * data;
		data = new double[1];
		if ( 0 < H5LTget_attribute(file_id, fullname.c_str(), attrname.c_str(),
			 H5T_NATIVE_DOUBLE, data) )
		  return NULL;
		else
		  return reinterpret_cast<double*>(data);
	}
	else if ( H5T_INTEGER == type_class ) {
		int * data;
		data = new int[1];
		if ( 0 < H5LTget_attribute(file_id, fullname.c_str(), attrname.c_str(),
			 H5T_NATIVE_INT, data) )
		  return NULL;
		else
		  return reinterpret_cast<int*>(data);
	}
	else if ( H5T_STRING == type_class ) {
		char * data;
		string fullname = "/" + name;
		data = new char[256];
		if ( 0 < H5LTget_attribute_string( file_id, fullname.c_str(), 
			  attrname.c_str(),data) )
		  return NULL;
		else
		  return data;
	}
	else {
		return NULL;
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
 * wrappers for createIntArray
 ******************************************************/

dalArray * dalGroup::cia_boost1( string arrayname, bpl::list pydims,
				  bpl::list pydata ){
  bpl::list cdims;

  for(int ii=0; ii<bpl::len(pydims); ii++)
     cdims.append(10);

  dalArray * array;
  array = cia_boost2( arrayname, pydims, pydata, cdims );

  return array;
}

dalArray * dalGroup::cia_boost2(
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
  data = NULL;

  return array;
}

dalArray * dalGroup::cia_boost_numarray1(
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

dalArray * dalGroup::cia_boost_numarray2(
	string arrayname, 
	bpl::list pydims,
	bpl::numeric::array pydata,
	bpl::list cdims ){

  vector<int> dims;

  for(int ii=0; ii<bpl::len(pydims); ii++)
    dims.push_back(bpl::extract<int>(pydims[ii]));

  long size = bpl::len(pydata);

// cout << "cia_boost_numarray "<< pydata.nelements() << endl;
  pydata.setshape( pydata.nelements() );
  bpl::object flat_data = pydata.getflat();
  bpl::list list_data( flat_data );

  dalArray * array = cia_boost2(arrayname, pydims, list_data, cdims);

  return array;
}

// dalArray * dalGroup::cia_boost( string arrayname, bpl::list pydims,
// 				  bpl::list pydata, bpl::list cdims ){
// 
//   vector<int> dims;
//   vector<int> chnkdims;
// 
//   for(int ii=0; ii<bpl::len(pydims); ii++)
//     dims.push_back(bpl::extract<int>(pydims[ii]));
// 
//   for(int ii=0; ii<bpl::len(cdims); ii++)
//     chnkdims.push_back(bpl::extract<int>(cdims[ii]));
// 
//   long size = bpl::len(pydata);
//   int * data = NULL;
//   data = new int[size];
// 
//   for(int ii=0; ii<size; ii++)
//     data[ii] = bpl::extract<int>(pydata[ii]);
// 
//   dalArray * array = createIntArray(arrayname, dims, data, chnkdims);
// 
//   delete [] data;
// 
//   return array;
// }
// 
// dalArray * dalGroup::cia_boost_numarray( string arrayname, 
// 	bpl::list pydims,
// 	bpl::numeric::array pydata, bpl::list cdims ){
// 
//   vector<int> dims;
// 
//   for(int ii=0; ii<bpl::len(pydims); ii++)
//     dims.push_back(bpl::extract<int>(pydims[ii]));
// 
//   long size = bpl::len(pydata);
//   int * data = NULL;
//   data = new int[size];
// 
// // cout << "cia_boost_numarray "<< pydata.nelements() << endl;
//   pydata.setshape( pydata.nelements() );
//   bpl::object flat_data = pydata.getflat();
//   bpl::list list_data( flat_data );
// 
//   dalArray * array = cia_boost(arrayname, pydims, list_data, cdims);
// 
//   return array;
// }

/******************************************************
 * wrapper for readIntArray
 ******************************************************/

bpl::numeric::array dalGroup::ria_boost( string arrayname )
{
	hid_t lclfile;
	hid_t  status;
// 	hid_t datatype, dataspace;

	// get the dataspace
	lclfile = H5Dopen( group_id, arrayname.c_str() );
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

	status = H5LTread_dataset_int( group_id, arrayname.c_str(), data );
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

        delete [] data;
        data = NULL;

	bpl::numeric::array nadata(
          bpl::make_tuple(
	    bpl::make_tuple(data_list)
	  )
	);
// 	dims_list.reverse();
	nadata.setshape(dims_list);
	return nadata;
}

/******************************************************
 * wrappers for createFloatArray
 ******************************************************/
dalArray * dalGroup::cfa_boost( string arrayname, bpl::list pydims, bpl::list pydata, bpl::list cdims ){

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

  dalArray * array = createFloatArray(arrayname, dims, data, chnkdims);

  delete [] data;
  data = NULL;

  return array;
}

dalArray * dalGroup::cfa_boost_numarray( string arrayname, 
	bpl::list pydims,
	bpl::numeric::array pydata, bpl::list cdims ){

  vector<int> dims;

  for(int ii=0; ii<bpl::len(pydims); ii++)
    dims.push_back(bpl::extract<int>(pydims[ii]));

  long size = bpl::len(pydata);

// cout << "cia_boost_numarray "<< pydata.nelements() << endl;
  pydata.setshape( pydata.nelements() );
  bpl::object flat_data = pydata.getflat();
  bpl::list list_data( flat_data );

  dalArray * array = cfa_boost(arrayname, pydims, list_data, cdims);

  return array;
}


#endif // end #ifdef PYTHON

/*
void dalGroup::setAttribute( string attrname, void * data, string coltype )
{
	const int size = 1;
	if ( dal_INT == coltype ) {
			status = H5LTset_attribute_int( file_id, name.c_str(), attrname.c_str(), (const int*)data, size );
	}
	else if ( dal_FLOAT == coltype ) {
			status = H5LTset_attribute_float( file_id, name.c_str(), attrname.c_str(), (const float*)data, size );
	}
	else if ( dal_DOUBLE == coltype ) {
			status = H5LTset_attribute_double( file_id, name.c_str(), attrname.c_str(), (const double*)data, size );
	}
	else {
			cout << "ERROR: datatype " << coltype << " not supported for setAttribute." << endl;
	}			
}
*/

} // end namespace DAL
