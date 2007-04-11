/***************************************************************************
 *   Copyright (C) 2006                                                    *
 *   Andreas Horneffer (<mail>)                                            *
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

/* $Id: dalLopesEvent.h,v 1.2 2007/04/04 18:45:38 bahren Exp $*/

#ifndef LOPESEVENTIN_H
#define LOPESEVENTIN_H

#include <string>
#include <blitz/array.h>

// Custom header files
#include <lopesevent.h>

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using blitz::Array;
using blitz::neverDeleteData;
using blitz::Range;
using blitz::shape;

namespace CR { // Namespace CR -- begin
  
  //! Not more than 30 antennas available (yet).
#define MAX_NUM_ANTENNAS 31
  //! LOPES et al. have a (fixed) samplerate of 80 MSPS
#define LOPES_SAMPLERATE 80e6 
  //! LOPES and LORUN work in the 2nd Nyquist zone.
#define LOPES_NYQUIST_ZONE 2
    
  /*!
    \class dalLopesEvent
    
    \brief Read in LOPES event files
    
    \author Andreas Horneffer, Lars B&auml;hren
    
    \date 2006/12/14
    
    \test tdalLopesEvent.cc
    
    <h3>Prerequisite</h3>
    
    <h3>Synopsis</h3>
    
    This is a port of Andreas Horneffer's original LopesEventIn class as
    distributed with the LOPES-Tools software package. The main difference
    w.r.t. the original implementation is, that (a) there is no longer any
    dependency on the DataReader class and (b) the usage of CASA arrays has
    been replaced by the corresponding objects of the Blitz library.

    <h3>Example(s)</h3>
    
  */  
  class dalLopesEvent {
    
  public:
    
    // ------------------------------------------------------------- Data
    
    /*!
      \brief Type of event for which the data have been stored
    */
    enum EvType {
      //! Unspecified event
      Unspecified,
      //! Cosmic ray
      Cosmic,
      //! Simulation output
      Simulation,
      //! Test event
      Test,
      //! Solar flare event
      SolarFlare,
      //! Other, not further specified event
      Other
    };
    
    /*!
      \brief List of observatories from which to expect data
    */
    enum Observatory {
      //! LOFAR Prototype Station
      LOPES,
      //! LOFAR at Radboud Universiteit Nijmegen
      LORUN
    };
    

  private:

    //! filename (incl. path) of the file to be read
    string filename_p;

    //! Is this object attached to a file?
    bool attached_p;
    
    //! Pointer to the (header-)data
    lopesevent *headerpoint_p;

    //! Number of antennas in the event
    int NumAntennas_p;
    
    //! Matrix with the data itself
    Array<short,2> channeldata_p;

    //! Vector with the antenna IDs
    Array<int,1> AntennaIDs_p;
    
  public:

    // ------------------------------------------------------------- Construction
    
    /*!
      \brief Default constructor
    */
    dalLopesEvent();

    /*!
      \brief Augmented constructor

      \param filename -- name (incl. path) of the lopes-eventfile to be read.
    */
    dalLopesEvent(string filename);
        
    // -------------------------------------------------------------- Destruction

    /*!
      \brief Destructor
    */
    virtual ~dalLopesEvent();
    
    // ---------------------------------------------------------------- Operators
           
    // ------------------------------------------------------------------ Methods
    
    /*!
      \brief Attach to a (another) lopes-eventfile
      
      \param filename -- name (incl. path) of the lopes-eventfile to be read.

      \return ok -- True if successfull
    */
    bool attachFile (string filename);

    /*!
      \brief return the raw ADC time series, in ADC-counts
      
      \return Matrix with the data
    */
/*     Array<double,2> fx(); */
    
    // --------------------------------------------------------------- Parameters
    
    /*!
      \brief Get the name of the data file.
      
      \return filename -- Name of the data file 
    */
    inline string  filename() {
      return filename_p;
    }

    /*!
      \brief Get the samplerate of the A/D conversion

      \return samplerate -- The samplerate of the A/D conversion, [Hz]
    */
    inline double samplerate () {
      return LOPES_SAMPLERATE;
    }

    /*!
      \brief Get the Nyquist zone in which the data are sampled
      
      \return nyquistZone -- The Nyquist zone in which the data are sampled
    */
    inline unsigned int nyquistZone () {
      return LOPES_NYQUIST_ZONE;
    }

    /*!
      \brief Get the length of the data set (in bytes)

      \return length -- The length of the data set (in bytes)
    */
    inline unsigned int length () {
      return headerpoint_p->length;
    }

    /*!
      \brief Get the channel data

      \return channeldata -- Data for thhe individual channels, i.e. dipoles
     */
    inline Array<short,2> channeldata () {
      return channeldata_p;
    }

    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, dalLopesEvent.
    */
    virtual std::string className () const {
      return "dalLopesEvent";
    }


  protected:
    
    /*!
      \brief Fill the header information into a header record.
      
      \return status -- Status of the operation; returns <i>true</i> if
      everything went fine.
    */
/*     bool generateHeaderRecord(); */
    
  private:
    
    /*!
      \brief Unconditional copying
    */
    void copy (dalLopesEvent const &other);
    
    /*!
      \brief Unconditional deletion 
    */
    void destroy(void);

    /*!
      \brief Initialization 
    */
    void init ();
    
  };
  
} // Namespace CR -- end

#endif /* LOPESEVENTIN_H */
  
