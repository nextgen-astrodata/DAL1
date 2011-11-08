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

#include <data_hl/MS_Dataset.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                   MS_Dataset
  
  MS_Dataset::MS_Dataset ()
    : MS_Table ()
  {;}
  
  //_____________________________________________________________________________
  //                                                                   MS_Dataset
  
  /*!
    \param name  -- Name of the MeasurementSet.
    \param flags -- I/O mode flags.
  */
  MS_Dataset::MS_Dataset (std::string const &name,
			  IO_Mode const &flags)
    : MS_Table ()
  {
    open (name, flags);
  }
    
  //_____________________________________________________________________________
  //                                                                   MS_Dataset
  
  /*!
    \param other -- Another MS_Dataset object from which to create this new
           one.
  */
  MS_Dataset::MS_Dataset (MS_Dataset const &other)
    : MS_Table (other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  MS_Dataset::~MS_Dataset ()
  {
    destroy();
  }
  
  void MS_Dataset::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  /*!
    \param other -- Another MS_Dataset object from which to make a copy.
  */
  MS_Dataset& MS_Dataset::operator= (MS_Dataset const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  /*!
    \param other -- Another MS_Dataset object from which to make a copy.
  */
  void MS_Dataset::copy (MS_Dataset const &other)
  {
    MS_Table::copy (other);
  }

  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void MS_Dataset::summary (std::ostream &os)
  {
#ifdef DAL_WITH_CASA
    casa::TableDesc tableDesc = itsTable.tableDesc();
    unsigned int nofRows      = itsTable.nrow();
#endif

    os << "[MS_Dataset] Summary of internal parameters."       << std::endl;
    os << "-- File type           = " << itsFiletype.name()    << std::endl;
    os << "-- I/O mode flags      = " << itsFlags.names()      << std::endl;
    os << "-- Table name          = " << itsName               << std::endl;
#ifdef DAL_WITH_CASA
    os << "-- nof. sub-tables     = " << itsTableNames.size()  << std::endl;
    os << "-- Sub-table names     = " << itsTableNames         << std::endl;
    os << "-- nof. table columns  = " << itsColumnNames.size() << std::endl;
    os << "-- Column names        = " << itsColumnNames        << std::endl;
    os << "-- nof. table rows     = " << nofRows               << std::endl;
#endif
    
    if (!itsSubtables.empty()) {
      std::vector<double> data;
      std::vector<double>::iterator itMin;
      std::vector<double>::iterator itMax;
      if (channelFrequencyValues(data)) {
	itMin = min_element (data.begin(), data.end());
	itMax = max_element (data.begin(), data.end());
	os << "-- Spectral window     = " << *itMin << " .. " << *itMax << std::endl;
      }
    }
    
  }
  
  // ============================================================================
  //
  //  Public methods
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                         open
  
  /*!
    \param name  -- Name of the MeasurementSet.
    \param flags -- I/O mode flags.
  */
  bool MS_Dataset::open (std::string const &name,
			 IO_Mode const &flags)
  {
#ifdef DAL_WITH_CASA
    /*______________________________________________________
      Open the root table of the MeasurementSet.
    */
    
    bool status = MS_Table::open(name,flags);
    
    /*______________________________________________________
      Open sub-tables.
    */
    
    if (status) {
      /* Open the sub-tables */
      if (itsTableNames.empty()) {
	std::cerr << "[MS_Dataset::open] Empty list of sub-table names!" << std::endl;
	status = false;
      } else {
	std::set<std::string>::iterator it;
	/* Try opening the sub-tables */
	for (it=itsTableNames.begin(); it!=itsTableNames.end(); ++it) {
	  try {
	    itsSubtables[*it] = MS_Table (MS_Table::name(), *it, flags);
	  } catch (casa::AipsError x) {
	    std::cerr << "[MS_Dataset::open] Failed open open sub-table "
		      << *it
		      << std::endl;
	    std::cerr << x.getMesg() << std::endl;
	  }
	}
      }
    } else {
      std::cerr << "[MS_Dataset::open] Failed to open table " << name << std::endl;
      status = false;
    }

    return status;
#else
    std::cerr << "[MS_Dataset::open] Unable to open dataset " << name
    << " - missing casacore to interface to MS." << std::endl;
    itsFlags = flags;
    return false;
#endif
  }

  //_____________________________________________________________________________
  //                                                                selectAntenna
  
  /*!
    \param antenna -- ID of the antenna to select; this corresponds to 
           \c selectBaseline(antenna1==antenna2).
  */
  bool MS_Dataset::selectAntenna (unsigned int const &antenna)
  {
    return selectBaseline (antenna, antenna);
  }
  
  //_____________________________________________________________________________
  //                                                               selectBaseline
  
  /*!
    \param antenna -- ID of the antenna to be part of the baseline.
  */
  bool MS_Dataset::selectBaseline (unsigned int const &antenna)
  {
#ifdef DAL_WITH_CASA
    return setSelection ("ANTENNA1",
			 Operator::Equal,
			 antenna,
			 true);
#else
    std::cerr << "[MS_Dataset::selectBasline] Unable to select baselines containing antenna "
    << antenna
    << " - missing casacore to interface to MS."
    << std::endl;
    return false;
#endif
  }
  
  //_____________________________________________________________________________
  //                                                               selectBaseline
  
  /*!
    \param antenna1 -- ID of the first antenna forming the baseline.
    \param antenna2 -- ID of the second antenna forming the baseline.
  */
  bool MS_Dataset::selectBaseline (unsigned int const &antenna1,
				   unsigned int const &antenna2)
  {
    bool status = true;
    
#ifdef DAL_WITH_CASA
    /* New selection: first call needs to ensure previous selection is
       overwritten. */
    status *= setSelection ("ANTENNA1", Operator::Equal, antenna1, true);
    status *= setSelection ("ANTENNA2", Operator::Equal, antenna2);
#else
    std::cerr << "[MS_Dataset::selectBasline] Unable to select baselines containing antennas "
    << antenna1 << " and " << antenna2
    << " - missing casacore to interface to MS."
    << std::endl;
    status = false;
#endif
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                               exposureValues
  
  /*!
    \retval data -- Values contained within the 'EXPOSURE' column of the MS 'MAIN'
            table.
    \return status -- Status of the operation; returns \e false in case anerror
            was encountered.
  */
  bool MS_Dataset::exposureValues (std::vector<double> &data)
  {
#ifdef DAL_WITH_CASA
    return readData(data, "EXPOSURE");
#else
    data.clear();
    return false;
#endif
  }

  //_____________________________________________________________________________
  //                                                                   timeValues
  
  /*!
    \retval data -- Values contained within the 'TIME' column of the MS 'MAIN'
            table.
    \return status -- Status of the operation; returns \e false in case anerror
            was encountered.
  */
  bool MS_Dataset::timeValues (std::vector<double> &data)
  {
#ifdef DAL_WITH_CASA
    return readData(data, "TIME");
#else
    data.clear();
    return false;
#endif
  }

  //_____________________________________________________________________________
  //                                                                    uvwValues
  
  /*!
    \retval data -- Values contained within the 'UVW' column of the MS 'MAIN'
            table.
    \return status -- Status of the operation; returns \e false in case anerror
            was encountered.
  */
  bool MS_Dataset::uvwValues (std::vector<double> &data)
  {
#ifdef DAL_WITH_CASA
    return readData(data, "UVW");
#else
    data.clear();
    return false;
#endif
  }

  //_____________________________________________________________________________
  //                                                       channelFrequencyValues
  
  /*!
    \retval data -- Values contained within the 'CHAN_FREQ' column of the MS
            'SPECTRAL_WINDOW' table.
    \return status -- Status of the operation; returns \e false in case anerror
            was encountered.
  */
  bool MS_Dataset::channelFrequencyValues (std::vector<double> &data)
  {
#ifdef DAL_WITH_CASA
    bool status      = true;
    std::string name = "SPECTRAL_WINDOW";
    std::map<std::string,MS_Table>::iterator it = itsSubtables.find(name);

    if (itsSubtables.empty()) {
      std::cerr << "[MS_Dataset::channelFrequencyValues]"
		<< " Empty list of sub-tables - unable to access data!"
		<< std::endl;
      status = false;
    } else if (it==itsSubtables.end()) {
      std::cerr << "[MS_Dataset::channelFrequencyValues]"
		<< " Failed to acces sub-table '" << name << "'!"
		<< std::endl;
      status = false;
    } else {
      status = it->second.readData (data,"CHAN_FREQ");
    }
    
    return status;
#else
    data.clear();
    return false;
#endif
  }
  
  //_____________________________________________________________________________
  //                                                           channelWidthValues
  
  /*!
    \retval data -- Values contained within the 'CHAN_WIDTH' column of the MS
            'SPECTRAL_WINDOW' table.
    \return status -- Status of the operation; returns \e false in case anerror
            was encountered.
  */
  bool MS_Dataset::channelWidthValues (std::vector<double> &data)
  {
    bool status      = true;
    std::string name = "SPECTRAL_WINDOW";
    std::map<std::string,MS_Table>::iterator it = itsSubtables.find(name);

    if (it==itsSubtables.end()) {
      std::cerr << "[MS_Dataset::channelWidthValues]"
		<< " Failed to acces sub-table '" << name << "'!"
		<< std::endl;
      status = false;
    } else {
      status = it->second.readData (data,"CHAN_WIDTH");
    }
    
    return status;
  }
  
  // ============================================================================
  //
  //  Private methods
  //
  // ============================================================================
  
} // Namespace DAL -- end
