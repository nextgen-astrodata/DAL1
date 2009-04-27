/*-------------------------------------------------------------------------*
 | $Id:: TBB_Timeseries.h 2126 2008-11-07 13:31:59Z baehren              $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008                                                    *
 *   Sven Duscha (sduscha@mpa-garching.mpg.de)                             *
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

#ifndef DALFITS_H
#define DALFITS_H


// C++ Standard library
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// cfitsio
#include <fitsio.h>

namespace DAL {
  
  /*!
    \class dalFITS

    \ingroup DAL

    \brief DAL class to provide access to FITS

    \author Sven Duscha

    \test tdalFITS.cpp
  */
  class dalFITS {

  public:
    
    //! define bin units (Hz, MHz, etc)
    enum DALbinUnit {
      Hz,
      kHz,
      MHz
    };
    
    //! Data type of FITS Image (in accordance with casacore naming scheme)
    enum DALImageType {
      TpFloat,
      TpDouble,
      TpComplex,
      TpDComplex
    };

  private:

    //! File handle for direct file access
    fitsfile **fptr;
    
    //! Name of the file storing the image
    std::string filename_p;
    
    //! CASAcore lattice handle

    //! dimensions of FITS image
    std::vector<int> dimensions;

    //! define types of bins
    enum DALbinType {
      frequency,
      lambda_sq
    };

    //! Type of the image
    DALImageType imageType_p;
    //! Bin type of the image
    DALbinType binType_p;

    DALbinUnit binUnit_p;

    //! frequency bins (central frequencies) or lambda squareds
    std::vector<double> bins;

    //! bin deltas (width of each bin)
    std::vector<double> binWidths;


  public:
 
    //________________________________________________________________
    // Construction/Destruction
    
    //! Default constructor
    dalFITS ();
    
    //! Constructor with associated filename
    dalFITS (const std::string &);
    
    //! Destructor
    ~dalFITS ();
    
    
    //! Copy constructor
    dalFITS (dalFITS const& other);
    
    //________________________________________________________________
    // Methods


    //! open a FITS file for reading, writing or both
    //open(std::string mode);
    
    //! close a FITS file hanle
    //close();
    
    /*!
      \brief Get the bin unit (center frequencies) of a FITS image
      
      \return binUnit -- unit associated with bins
    */
    inline DALbinUnit getBinUnit () const {
      return binUnit_p;
    }

    /*!
      \brief Get the bins (center frequencies) of a FITS image
    
      \return bins -- can be frequency or lambda squared
    */
    inline std::vector<double> getBins () const {
      return bins;
    }

    /*!
      \brief Get widths (for RM integration) of a FITS object (not necessarily in the image)
   
      \return binDeltas --
    */
    inline std::vector<double> getBinWidths () const {
      return binWidths;
    }
  
    //! get the associated filename of a FITS object
    inline std::string filename () const {
      return filename_p;
    }

    //! Set the associated filename of a FITS object
    inline void setFilename (const std::string filename) {
      this->filename_p=filename;
    };
    
    //! Get the type of the image
    inline std::string getImageType () const {
      switch (imageType_p) {
      case TpFloat:
	return "TpFloat";
	break;
      case TpDouble:
	return "TpDouble";
	break;
      case TpComplex:
	return "TpComplex";
	break;
      case TpDComplex:
	return "TpDComplex";
	break;
      };
    }
    
    //! Get the name of the class
    std::string className () const {
      return "dalFITS";
    }

    //! Provide a summary of the internal status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the internal status
    void summary (std::ostream &os);    

  };
  
}

#endif
