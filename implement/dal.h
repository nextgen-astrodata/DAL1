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

/*!
  \file dal.h

  \brief Common header file for the Data Access Library (DAL)

  \defgroup DAL Data Access Library (DAL)

  \author Joseph Masters

  <h3>Synopsis</h3>
  The Data Access Library (DAL) is a library that abstracts the underlying
  file format from the user when working with radio astronomy data.  Underlying
  formats may include HDF5, AIPS++/CASA tables, FITS or specific raw formats.
  This library is available to both the C/C++ developer and the Python user via
  the pydal module.

  <h3>Examples</h3>
  Typical C++ usage of the <b><i>DAL</i></b> would look something like the
  following:
  <br/>
  \code
  #include <dal.h>
  #include <dalDataset.h>
  #include <dalData.h>

  using namespace casa;

  int main(int argc, char *argv[])
  {
    // create a dataset object of type MSCASA
    cout << " -- Creating a dataset object of type MSCASA ..." << endl;
    dalDataset * msds = new dalDataset( argv[1], "MSCASA" );
  
    // print out a list of tables in the dataset
    msds->listTables();
  
    // open a table in the dataset
    cout << " -- Opening table in the dataset ..." << endl;
    string tablename = "MAIN";
    string filter_cols = "UVW, TIME, ANTENNA1, ANTENNA2, DATA";
    string filter_conditions = "ANTENNA1 = 1 AND ANTENNA2 = 1";
    msds->setFilter( filter_cols, filter_conditions );
    dalTable * maintable = msds->openTable( tablename );
  
    //
    // get data from the TIME column
    //
    dalColumn * time_col = maintable->getColumn("TIME");
    cout << time_col->getDataType() << endl;
    if ( time_col->isScalar() ) cout << "SCALAR" << endl;
    if ( time_col->isArray() ) cout << "ARRAY" << endl;
    cout << "Number of rows: " << time_col->nrows() << endl;

    dalData * data_object = time_col->data();
    double * value1;
    for(unsigned int xx=0; xx<13; xx++)
    {
      value1 = (double*)data_object->get(xx);
      cout << *value1 << endl;
    }
    delete time_col;

    ds->close();    // close the hdf5 file

    return SUCCESS;
  }
  \endcode

  Typical python usage of the <b><i>pydal</i></b> module might look like this:
  <br/>
  \code
  import sys
  import pydal as dal
  from pylab import *

  msds= dal.dalDataset()
  if ( msds.open(sys.argv[1]) ):
	sys.exit(1)

  tablename = "MAIN";

  msds.setFilter("UVW")
  title("UV coverage for\n" + sys.argv[1] )

  maintable = msds.openTable( tablename )

  # get the UVW column
  uvw_column = maintable.getColumn("UVW")

  # get the data from the column
  data = uvw_column.data()

  # plot the data
  xlabel("U(-U)")
  ylabel("V(-V)")
  plot(data[0],data[1],'r,',-(data[0]),-(data[1]),'b,')
  show()
  \endcode
*/

#ifndef DAL_H
#define DAL_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>  // for cout field width
#include <cstdio>

/* #include "H5LT.h" */
#include <H5LT.h>

extern "C" {
#include "fitsio.h"
}

#include "convert.h"  // for stringify function

#ifndef DALBASETYPES_H
#include "dalBaseTypes.h"
#endif

using namespace std;

/*
   if creating python bindings
*/
#ifdef PYTHON
#include <boost/python.hpp>
#include <boost/python/object.hpp>
#include <boost/python/list.hpp>
#include <boost/python/extract.hpp>
namespace bpl = boost::python;
#endif

/*
   if there is a casa installation
*/
#ifdef WITH_CASA
#include <casa/aips.h>
#include <tables/Tables.h>
#include <tables/Tables/Table.h>
#include <ms/MeasurementSets.h>
#include <ms/MeasurementSets/MSReader.h>
#include <iostream>
#include <casa/aipstype.h>
#include <casa/complex.h>
#include <casa/BasicMath/Math.h>
#include <tables/Tables/ScalarColumn.h>
#include <tables/Tables/TableParse.h>
#include <tables/Tables/TableKeyword.h>
#endif


const string H5TYPE = "HDF5";
const string FITSTYPE = "FITS";
const string MSCASATYPE = "MSCASA";

const int SUCCESS = 0;
const int FAIL = 1;

// CHUNK_SIZE arbitrarily chosen, more research needed on this feature
const int CHUNK_SIZE = 5000;

const short MAX_COL_NAME_SIZE = 256;

const string dal_CHAR = "dalCHAR";
const string dal_STRING = "dalSTRING";

const string dal_INT = "dalINT";
const string dal_UINT = "dalINT";
const string dal_SHORT = "dalSHORT";
const string dal_LONG = "dalLONG";

const string dal_FLOAT = "dalFLOAT";
const string dal_DOUBLE = "dalDOUBLE";

const string dal_COMPLEX = "dalCOMPLEX";
const string dal_DCOMPLEX = "dalDCOMPLEX";

typedef struct dalcomplex {
	double r;  // real
	double i;  // imaginary
} dalcomplex;

#endif
