/***************************************************************************
 *   Copyright (C) 2009                                                    *
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

#ifndef FILENAME_H
#define FILENAME_H

// Standard library header files
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace DAL1 { // Namespace DAL1 -- begin
  
  /*!
    \class Filename
    
    \ingroup DAL1
    \ingroup data_common
    
    \brief Class to generate filenames matching the LOFAR convention
    
    \author Lars B&auml;hren

    \date 2009/09/28

    \test tFilename.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>\ref dal_icd_005
    </ul>
    
    <h3>Synopsis</h3>

    This class implements the naming conventions for files, as laid down in
    the LOFAR Data Format ICD LOFAR-USG-ICD-005. According to the ICD the name
    of a LOFAR data file is constructed as follows:

    \verbatim
    <Prefix><Observation ID>_<Optional Descriptors>_<Filetype>.<Extension>
    \endverbatim

    In this the individual fields are defined as follows:
    <ul>
      <li><b>Prefix</b>, for a standard LOFAR dataset, is a fixed single
      character, 'L'.
      <li><b>Observation ID</b> -- unique identifier of the observation
      as part of which (or based on which) this data product was created.
      <b>Optional Descriptors</b> are used to further indicate the
      nature of the data stored within the file.  It is best to keep the
      file name as short as possible;  optional descriptors should only be
      used if necessary to better uniquely identify a file.<br>
      While the descriptors can be used to e.g. indicate a specific sub-band,
      beam or date it is <b>not to be used for ranges</b>.
      Digits should be padded with zero's.  Please note that the number of
      digits can increase in the future;  software should be made flexible
      to take this into account.
      <li><b>Filetype</b> is a marker for the contents of the
      file. There will be several different kinds of data produced by
      LOFAR, see table below. Importantly, filetype signifies the kind of
      LOFAR data that comprise the particular data file, and therefore,
      will also signal the appropriate interface control document for
      further reference, should users find that necessary. The options for
      the file type along with their abbreviations are listed in the table below.
    </ul>

    For data that is basically generated by further processing of a standard
    data product it is advised to keep the identifiers of the standard product
    and add extra descriptors as needed just before the \e Extension separated by
    one or more dots.

    \verbatim
    <Prefix><Observation ID>_<Optional Descriptors>_<Filetype>.<Other descriptors>.<Extension>
    \endverbatim

    In this the individual fields are defined as follows:
    <ul>
      <li><b>Prefix</b>, for a standard LOFAR dataset, is a fixed single
      character, 'L'.

      <li><b>Observation ID</b> is the SAS identifier for the process that
      generated the data product that this data product is derived from.

      <li><b>Optional Descriptors</b> are the same as defined under the
      previous section, if applicable.

      <li><b>Filetype</b> is the same as defined under the previous section,
      if applicable.

      <li><b>Other descriptors</b> is relatively free form and could for example
      be things like \c flagged, \c manual, \c pub_version3,
      \c Nature.page45.image. Basically anything that can't be caught by the
      standard descriptors from the previous section. Note if needed these should
      preferably be separated by dots, not underscores, to separate them from the
      more standardized descriptors.

      <li><b>Extension</b> should be last, so it is easy to identify the type of
      data product and what tools are needed to read it.
    </ul>

    <h3>Example(s)</h3>
    
    <ol>
      <li>Simple example to generate filename for a HDF5-based BF dataset:
      \code
      // Set up the input parameters
      std::string observationID          = "123456789";
      DAL1::Filename::Type filetype       = Filename::bf;
      DAL1::Filename::Extension extension = Filename::h5;
      
      // Create object ...
      DAL1::Filename filename (observationID,
                              filetype,
                              extension);
      // ... and retrieve filename
      std::string name = filename.filename();
      \endcode
      The result will be: <tt>name = "L123456789_bf.h5"</tt>
    </ol>
    
  */  
  class Filename {

  public:

    /*!
      \brief Marker for the contents of the file
      
      There will be several different kinds of data produced by
      LOFAR, see table below. Importantly, filetype signifies the kind of
      LOFAR data that comprise the particular data file, and therefore,
      will also signal the appropriate interface control document for
      further reference, should users find that necessary.
    */
    enum Type {
      /*! LOFAR visibility file containing correlation UV information. */
      uv,
      /*! Standard LOFAR Image cube containing RA, Dec, frequency and
	polarization information. */
      sky,
      /*! Rotation Measure Synthesis Cube with axes of RA, Dec, Faraday
	Depth and polarization. */
      rm,
      /*! A Near Field sky Image with axes of position on the sky (x, y, z),
	frequency time and polarization. */
      nfi,
      /*! Dynamic Spectra file with axes of time, frequency and polarization. */
      dynspec,
      /*! A Beam-Formed file contains time series data with axes of frequency
	vs time. */
      bf,
      /*! TBB dump file is raw time-series: (1) with intensity as a function of
	frequency, or (2) containing voltage vs time. */
      tbb
    };
    
    /*!
      \brief File extensions
      
      Files generated by CASA/casacore will continue the currently existing
      conventions using upper-case suffixes.
    */
    enum Extension {
      //! CASA/casacore MeasurementSet.
      MS,
      //! HDF5 file.
      h5,
      //! FITS file.
      fits,
      //! Logfile.
      log,
      //! A parset file.
      parset,
      //! Local sky model.
      lsm,
      //! CASA/casacore image file (PagedImage).
      IM,
      //! ParmDB file generated by BBS.
      PD,
      //! Dataset description file.
      vds,
      //! Dataset description file.
      gds,
      //! Configuration file (mostly local to station).
      conf,
      //! Raw Beam-Formed (non-Incoherentstokes) file written from the Blue Gene/P.
      raw,
      //! Raw Beam-Formed Incoherent-Stokes file written from Blue Gene/P.
      incoherentstokes
    };

  private:

    //! Prefix of the filename, preceeding the observation ID
    std::string itsPrefix;
    //! Unique identifier for the observation
    std::string observationID_p;
    //! Optional descriptors
    std::string itsOptionalDescriptor;
    //! Marker for the contents of the file
    Filename::Type itsFiletype;
    //! Extension of the file
    Filename::Extension itsExtension;
    //! Path to the location of the file
    std::string itsPath;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    Filename ();

    //! Argumented constructor
    Filename (std::string const &observationID,
	      Type const &filetype);
    
    //! Argumented constructor
    Filename (std::string const &observationID,
	      Type const &filetype,
	      Extension const &extension,
	      std::string const &path="");
    
    //! Argumented constructor
    Filename (std::string const &observationID,
	      std::string const &optionalDescription,
	      Type const &filetype,
	      Extension const &extension,
	      std::string const &path="");
    
    //! Copy constructor
    Filename (Filename const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~Filename ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    Filename& operator= (Filename const &other); 
    
    // === Parameter access =====================================================

    //! Get the prefix of the filename ("L" for a standard LOFAR dataset)
    inline std::string prefix () const {
      return itsPrefix;
    }

    //! Set the prefix of the filename
    inline void setPrefix (std::string const &prefix) {
      itsPrefix = prefix;
    }

    //! Get the unique observation ID
    inline std::string observationID () const {
      return observationID_p;
    }

    //! Set the unique observation ID
    inline void setObservationID (std::string const &observationID) {
      observationID_p = observationID;
    }

    //! Get optional descriptors
    inline std::string optionalDescription () const {
      return itsOptionalDescriptor;
    }

    //! Set optional descriptors
    void setOptionalDescription (std::string const &optionalDescription);
    
    //! Get the file type
    inline Type filetype () const {
      return itsFiletype;
    }
    
    //! Get the file type name
    inline std::string filetypeName () const {
      return getName(itsFiletype);
    }

    //! Set the file type
    inline void setFiletype (Type const &filetype) {
      itsFiletype = filetype;
    }

    //! Get the file extension type
    inline Extension extension () const {
      return itsExtension;
    }

    //! Get the file extension name
    inline std::string extensionName () const {
      return getName (itsExtension);
    }
    
    //! Set the file extension
    inline void setExtension (Extension const &extension) {
      itsExtension = extension;
    }

    //! Get the path to the file
    inline std::string path () const {
      return itsPath;
    }

    //! Set the path to the file
    inline void setPath (std::string const &path) {
      itsPath = path;
    }
    
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, Filename.
    */
    inline std::string className () const {
      return "Filename";
    }

    //! Provide a summary of the internal status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the internal status
    void summary (std::ostream &os);    

    // === Public methods =======================================================

    //! Get the name of the file
    std::string filename (bool const &fullpath=false);

    // === Static methods =======================================================

    //! Get map of file extension types and names.
    static std::map<Filename::Extension,std::string> extensionMap ();
    //! Get array/vector with the extension types
    static std::vector<Filename::Extension> extensionTypes ();
    //! Get array/vector with the extension names
    static std::vector<std::string> extensionNames ();
    //! Get the file extension as string
    static std::string getName (Filename::Extension const &extension);

    //! Get map of file-type types and names.
    static std::map<Filename::Type,std::string> filetypeMap ();
    //! Get array/vector with the file-type types
    static std::vector<Filename::Type> filetypeTypes ();
    //! Get array/vector with the file-type names
    static std::vector<std::string> filetypeNames ();
    //! Get the file type as string
    static std::string getName (Filename::Type const &filetype);
    
  private:

    //! Initialize the internal parameters held by an object of this class
    void init ();
    
    //! Unconditional copying
    void copy (Filename const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class Filename -- end
  
} // Namespace DAL1 -- end

#endif /* FILENAME_H */
  
