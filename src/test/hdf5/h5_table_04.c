/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF5.  The full HDF5 copyright notice, including     *
 * terms governing use, modification, and redistribution, is contained in    *
 * the files COPYING and Copyright.html.  COPYING can be found at the root   *
 * of the source code distribution tree; Copyright.html can be found at the  *
 * root level of an installed copy of the electronic HDF5 document set and   *
 * is linked from the top-level documents page.  It can also be found at     *
 * http://hdfgroup.org/HDF5/doc/Copyright.html.  If you do not have          *
 * access to either file, you may request a copy from help@hdfgroup.org.     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <hdf5.h>
#include <stdlib.h>
#include "tHDF5_table.h"

/*!
  \file h5_table_04.c
  \ingroup HDF5
  \brief Table API example
 
  Table API exmaple for the functions
  - H5TBwrite_fields_name
*/

#define NRECORDS_ADD  (hsize_t)   3
#define TABLE_NAME               "table"

int main( void )
{
  Particle dst_buf[NRECORDS];
  /* Calculate the size and the offsets of our struct members in memory */
  size_t dst_size =  sizeof( Particle );
  size_t dst_offset[NFIELDS] = { HOFFSET( Particle, name ),
				 HOFFSET( Particle, lati ),
				 HOFFSET( Particle, longi ),
				 HOFFSET( Particle, pressure ),
				 HOFFSET( Particle, temperature )};
  size_t dst_sizes[NFIELDS] = { sizeof( dst_buf[0].name),
				sizeof( dst_buf[0].lati),
				sizeof( dst_buf[0].longi),
				sizeof( dst_buf[0].pressure),
				sizeof( dst_buf[0].temperature)};
  size_t field_offset_pos[2] = { HOFFSET( Position, lati ),
				 HOFFSET( Position, longi )};
  hid_t      field_type[NFIELDS];
  hid_t      string_type;
  hid_t      fileID;
  hsize_t    chunk_size = 10;
  Particle   fill_data[1] =
    { {"no data",-1,-1, -99.0f, -99.0} };  /* Fill value particle */
  hsize_t    start;                        /* Record to start reading/writing */
  hsize_t    nrecords;                     /* Number of records to read/write */
  int        compress  = 0;
  int        n;
  herr_t     status;
  Particle  *p_data = NULL;               /* Initially no data */
  float      pressure_in [NRECORDS_ADD] = /* Define new values for the field "Pressure"  */
    { 0.0f,1.0f,2.0f};
  Position   position_in[NRECORDS_ADD] = {/* Define new values for "Latitude,Longitude"  */
    {0,0},
    {10,10},
    {20,20}};
  NamePressure   namepre_in[NRECORDS_ADD] =/* Define new values for "Name,Pressure"  */
    { {"zero",0.0f},
      {"one",   1.0f},
      {"two",   2.0f},
    };
  size_t field_sizes_pos[2]=
    {
      sizeof(position_in[0].longi),
      sizeof(position_in[0].lati)
    };
  size_t field_sizes_pre[1]=
    {
      sizeof(namepre_in[0].pressure)
    };
  
  /* Initialize the field field_type */
  string_type = H5Tcopy( H5T_C_S1 );
  H5Tset_size( string_type, 16 );
  field_type[0] = string_type;
  field_type[1] = H5T_NATIVE_INT;
  field_type[2] = H5T_NATIVE_INT;
  field_type[3] = H5T_NATIVE_FLOAT;
  field_type[4] = H5T_NATIVE_DOUBLE;
  
  /*__________________________________________________________________
    Create a new file using default properties.
  */

  fileID = H5Fcreate ("h5_table_04.h5",
		       H5F_ACC_TRUNC,
		       H5P_DEFAULT,
		       H5P_DEFAULT);
  
  /*__________________________________________________________________
    Make the table
  */

  status = H5TBmake_table ("Table Title",
			   fileID,
			   TABLE_NAME,
			   NFIELDS,
			   NRECORDS,
			   dst_size,
			   field_names,
			   dst_offset,
			   field_type,
			   chunk_size,
			   fill_data,
			   compress,
			   p_data);

  /*__________________________________________________________________
    Write the pressure field starting at record 2.
  */

  start    = 2;
  nrecords = NRECORDS_ADD;
  status   = H5TBwrite_fields_name (fileID,
				    TABLE_NAME,
				    "Pressure",
				    start,
				    nrecords,
				    sizeof( float ),
				    0,
				    field_sizes_pre,
				    pressure_in);
  
  /*__________________________________________________________________
    Write the new longitude and latitude information starting at
    record 2.
  */
  start    = 2;
  nrecords = NRECORDS_ADD;
  status = H5TBwrite_fields_name (fileID,
				  TABLE_NAME,
				  "Latitude,Longitude",
				  start,
				  nrecords,
				  sizeof( Position ),
				  field_offset_pos,
				  field_sizes_pos,
				  position_in);
  
  /*__________________________________________________________________
    Read the table
  */

  status = H5TBread_table (fileID,
			   TABLE_NAME,
			   dst_size,
			   dst_offset,
			   dst_sizes,
			   dst_buf);
  
  /* print it by rows */
  for (n=0; n<NRECORDS; n++) {
    printf ("%-5s %-5d %-5d %-5f %-5f",
	    dst_buf[n].name,
	    dst_buf[n].lati,
	    dst_buf[n].longi,
	    dst_buf[n].pressure,
	    dst_buf[n].temperature);
    printf ("\n");
  }
  
  /*-------------------------------------------------------------------------
   * end
   *-------------------------------------------------------------------------
   */
  
  /* close type */
  H5Tclose( string_type );
  
  /* close the file */
  H5Fclose (fileID);
  
  return 0;
  
  
}

