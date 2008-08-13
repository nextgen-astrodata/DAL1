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

#ifndef DALDATASET_H
#include "dalDataset.h"
#endif

#ifndef DATABASE_H
#include "Database.h"
#endif

#endif // DAL_H
