/***************************************************************************
 *   Copyright (C) 2008                                                    *
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

#ifndef TBB_DIPOLEDATASET_H
#define TBB_DIPOLEDATASET_H

#include <string>

#ifdef DAL_WITH_CASA
#include <casa/aips.h>
#include <casa/Arrays/ArrayIO.h>
#include <casa/Arrays/Vector.h>
#include <casa/Quanta/Quantum.h>
#include <casa/Containers/Record.h>
#include <measures/Measures/MFrequency.h>
#include <measures/Measures/MPosition.h>
#endif

#include <data_common/HDF5GroupBase.h>

namespace DAL {  // Namespace DAL -- begin

  /*!
    \class TBB_DipoleDataset
    
    \ingroup DAL
    \ingroup data_hl
    
    \brief Container for dipole-based data in a LOFAR TBB time-series dataset

    \author Lars B&auml;hren

    \date 2008/01/10

    \test tTBB_DipoleDataset.cc

    <h3>Prerequisite</h3>

    <ul type="square">
      <li><a href="http://www.atnf.csiro.au/computing/software/casacore/classcasa_1_1Record.html">casa::Record</a>
      -- A hierarchical collection of named fields of various types.
      <li>DAL::TBB_StationGroup
      <li>DAL::TBB_Timeseries
    </ul>

    <h3>Synopsis</h3>

    \image html TBB_DipoleDataset.png

    <h3>Example(s)</h3>

    <ul>
    <li>Use the default constructor for a new object and display its internal
    settings:
    \code
    // Create new object
    DAL::TBB_DipoleDataset dataset;
    // Summaru of the internal properties
    dataset.summary();
    \endcode
    <li>Create a new object for a specific dipole dataset embedded within a TBB
    time-series dataset:
    \code
    // The name of the file to open
    std::string fileName ("<the name of the file>");
    // The path to the dataset contained within the HDF5 file
    std::string datasetName ("<path to the dataset>");

    DAL::TBB_DipoleDataset dataset (fileName,
                                    datasetName);
    \endcode
  </ul>

  */
  class TBB_DipoleDataset : public HDF5GroupBase {
    
    //! Datatype identifier
    hid_t datatype_p;
    //! Dataspace identifier
    hid_t dataspace_p;
    //! Shape of the dataset
    std::vector<hsize_t> itsShape;
    
  public:

    // === Construction =========================================================
    
    //! Default constructor
    TBB_DipoleDataset ();
    //! Argumented constructor
    TBB_DipoleDataset (hid_t const &location,
		                   std::string const &name,
                       IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));
    //! Argumented constructor
    TBB_DipoleDataset (hid_t const &location,
		       uint const &stationID,
		       uint const &rspID,
		       uint const &rcuID);
    //! Argumented constructor
    TBB_DipoleDataset (hid_t const &location,
		       uint const &stationID,
		       uint const &rspID,
		       uint const &rcuID,
		       std::vector<hsize_t> const &shape,
		       hid_t const &datatype=H5T_NATIVE_SHORT);
    
    // === Destruction ==========================================================
    
    //! Destructor
    ~TBB_DipoleDataset ();
    
    // === Methods ==============================================================
    
    //! Overloading of the copy operator
    TBB_DipoleDataset& operator= (TBB_DipoleDataset const &other);
    
    // === Parameter access =====================================================

    //! Get the datatype of the individual array elements
    inline hid_t datatype () const {
      return datatype_p;
    }
    
    //! Get the shape of the data array
    inline std::vector<hsize_t> shape () const {
      return itsShape;
    }

    //! Get the time as Julian Day
    double julianDay (bool const &onlySeconds=false);
    
    /*!
      \brief Get the name of the class
      \return className -- The name of the class, TBB_DipoleDataset.
    */
    inline std::string className () const {
      return "TBB_DipoleDataset";
    }
    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }
    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);
    
    // === Methods ==============================================================
    
    //! Open a structure (file, group, dataset, etc.)
    bool open (hid_t const &location);
    //! Open a dipole dataset
    bool open (hid_t const &location,
	       std::string const &name,
	       IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));
    //! Open a dipole dataset
    bool open (hid_t const &location,
	       uint const &stationID,
	       uint const &rspID,
	       uint const &rcuID,
	       std::vector<hsize_t> const &shape,
	       hid_t const &datatype=H5T_NATIVE_SHORT);
    //! Get the unique channel/dipole identifier
    int dipoleNumber ();
    //! Get the unique channel/dipole identifier
    static int dipoleNumber (unsigned int const &station,
			     unsigned int const &rsp,
			     unsigned int const &rcu);
    //! Get the unique channel/dipole identifier
    std::string dipoleName ();
    //! Get the unique channel/dipole identifier
    static std::string dipoleName (unsigned int const &station,
				   unsigned int const &rsp,
				   unsigned int const &rcu);
    //! Get a number of data values as recorded for this dipole
    bool readData (int const &start,
		   int const &nofSamples,
		   short *data);
    
    //! Get a number of data values as recorded for this dipole
    /*     bool readData (int const &start, */
    /* 	     int const &nofSamples, */
    /* 	     std::vector<short> &data); */
    
#ifdef DAL_WITH_CASA
    
    //! Get the antenna position as a measure
    casa::MPosition antenna_position ();
    //! Set the antenna position from a measure
    bool set_antenna_position (casa::MPosition &pos);
    //! Get the ADC sample frequency as casa::Q1uantity
    bool sample_frequency (casa::Quantity &freq);
    //! Get the ADC sample frequency as casa::Measure
    bool sample_frequency (casa::MFrequency &freq);
    //! Set the ADC sample frequency as casa::Quantity
    bool set_sample_frequency (casa::Quantity const &freq);
    //! Get a casa::Record containing the values of the attributes
    bool getAttributes (casa::Record &rec);
    //! Get a number of data values as recorded for this dipole
    casa::Vector<double> readData (int const &start=0,
				   int const &nofSamples=1);
    
#endif
    
  private:
    
    //! Open the structures embedded within the current one
    bool openEmbedded (IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));
    //! Set up the list of attributes attached to the structure
    void setAttributes ();
    //! Initialize the object's internal parameters
    void init ();
    //! Initialize the internal dataspace
    void init (std::string const &filename,
	       std::string const &dataset);
    //! Unconditional copying
    void copy (TBB_DipoleDataset const &other);
    //! Unconditional deletion
    void destroy(void);
    
  };
  
} // Namespace DAL -- end

#endif /* TBB_DIPOLEDATASET_H */

