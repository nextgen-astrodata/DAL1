/***************************************************************************
 *   Copyright (C) 2011                                                    *
 *   Lars B"ahren (lbaehren@gmail.com)                                     *
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

#ifndef MS_DATASET_H
#define MS_DATASET_H

// Standard library header files
#include <iostream>
#include <string>
#include <map>
#include <algorithm>

#include <core/MS_Table.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class MS_Dataset
    
    \ingroup DAL
    \ingroup data_hl
    
    \brief Brief description for class DAL::MS_Dataset
    
    \author Lars B&auml;hren

    \date 2011-08-11

    \test tMS_Dataset.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>[start filling in your text here]
    </ul>
    
    <h3>Synopsis</h3>
    
        \verbatim
    /  MAIN                        Table     Data of all samples for individual interferometers
    |-- UVW                        Column    
    |-- FLAG_CATEGORY              Column    
    |-- WEIGHT                     Column    
    |-- SIGMA                      Column    
    |-- ANTENNA1                   Column    
    |-- ANTENNA2                   Column    
    |-- ARRAY_ID                   Column    
    |-- DATA_DESC_ID               Column    
    |-- EXPOSURE                   Column    
    |
    |-- ANTENNA                    Table     Antenna information
    |   |-- ANTENNA_ID             Column
    |   |-- FEED_ID                Column
    |   |-- SPECTRAL_WINDOW_ID     Column
    |   |-- TIME                   Column
    |   |-- NUM_RECEPTORS          Column
    |   |-- BEAM_ID                Column
    |   |-- BEAM_OFFSET            Column
    |   |-- POLARIZATION_TYPE      Column
    |   |-- POL_RESPONSE           Column
    |   |-- POSITION               Column
    |   `-- RECEPTOR_ANGLE         Column
    |
    |-- DATA_DESCRIPTION           Table
    |  |-- SPECTRAL_WINDOW_ID
    |  |-- POLARIZATION_ID
    |  `-- FLAG_ROW
    |-- FEED                       Table     Feed (Frontend) related information
    |-- FLAG_CMD                   Table     Flag information
    |-- FIELD                      Table     Information on observed positions
    |-- HISTORY                    Table     History log of MS
    |-- OBSERVATION                Table     General observation information
    |-- POINTING                   Table     Antenna pointing information
    |-- POLARIZATION               Table     Polarization information description
    |   |-- NUM_CORR
    |   |-- CORR_TYPE
    |   |-- CORR_PRODUCT
    |   `-- FLAG_ROW
    |-- PROCESSOR                  Table with Correlator information
    |-- SPECTRAL_WINDOW            Table with Frequency/IF information
    |   |-- NUM_CHAN               Column    int
    |   |-- NAME                   Column    string
    |   |-- REF_FREQUENCY          Column    double
    |   |-- CHAN_FREQ              Column    array<double,1>
    |   |-- CHAN_WIDTH             Column    array<double,1>
    |   |-- MEAS_FREQ_REF          Column    int
    |   |-- EFFECTIVE_BW           Column    array<double,1>
    |   |-- RESOLUTION             Column    array<double,1>
    |   |-- TOTAL_BANDWIDTH        Column    double
    |   |-- NET_SIDEBAND           Column    int
    |   |-- IF_CONV_CHAIN          Column    int
    |   |-- FREQ_GROUP             Column    int
    |   |-- FREQ_GROUP_NAME        Column    string
    |   `-- FLAG_ROW               Column    bool
    `-- STATE
    \endverbatim

    <h3>Example(s)</h3>

    Besides inheriting the more generic interface to access table columns - as
    implemented in DAL::MS_Table - there are a few convenience functions available
    as well:
    
    \code
    std::vector<double> data;

    // Get the values from 'EXPOSURE' column
    exposureValues (data);

    // Get the values from 'TIME' column
    timeValues (data);

    // Get the values from 'UVW' column
    uvwValues (data);
    \endcode
    
  */  
  class MS_Dataset : public MS_Table {

    //! Sub-tables attached to the root table
    std::map<std::string,MS_Table> itsSubtables;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    MS_Dataset ();
    
    //! Argumented constructor to open table of given \e name
    MS_Dataset (std::string const &name,
		IO_Mode const &flags=IO_Mode());
    
    //! Copy constructor
    MS_Dataset (MS_Dataset const &other);
    
    // === Destruction ==========================================================
    
    //! Destructor
    ~MS_Dataset ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    MS_Dataset& operator= (MS_Dataset const &other); 
    
    // === Parameter access =====================================================
    
    /*!
      \brief Get the name of the class
      \return className -- The name of the class, MS_Dataset.
    */
    inline std::string className () const {
      return "MS_Dataset";
    }

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);    

    // === Public methods =======================================================

    //! Open MeasurementSet of given \e name.
    bool open (std::string const &name,
	       IO_Mode const &flags=IO_Mode());

    //! Select data from a specific antenna
    bool selectAntenna (unsigned int const &antenna);
    
    //! Select data from baselines including a specific antenna
    bool selectBaseline (unsigned int const &antenna);
    
    //! Select data from a specific baseline
    bool selectBaseline (unsigned int const &antenna1,
			 unsigned int const &antenna2);
    
    //! Get the values from 'EXPOSURE' column
    bool exposureValues (std::vector<double> &data);
    //! Get the values from 'TIME' column
    bool timeValues (std::vector<double> &data);
    //! Get the values from 'UVW' column
    bool uvwValues (std::vector<double> &data);
    
    //! Get the values from 'CHAN_FREQ' column of the 'SPECTRAL_WINDOW' table
    bool channelFrequencyValues (std::vector<double> &data);
    //! Get the values from 'CHAN_WIDTH' column of the 'SPECTRAL_WINDOW' table
    bool channelWidthValues (std::vector<double> &data);
    
    // === Static methods =======================================================
    
    
    
  private:
    
    //! Unconditional copying
    void copy (MS_Dataset const &other);
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class MS_Dataset -- end
  
} // Namespace DAL -- end

#endif /* MS_DATASET_H */
  
