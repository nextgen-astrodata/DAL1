/***************************************************************************
 *   Copyright (C) 2010                                                    *
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

#ifndef BF_STOKESDATASET_H
#define BF_STOKESDATASET_H

// Standard library header files
#include <iostream>
#include <sstream>
#include <string>

#include <coordinates/Stokes.h>
#include <data_common/HDF5DatasetBase.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class BF_StokesDataset
    
    \ingroup DAL
    \ingroup data_hl
    
    \brief High-level interface to the Stokes dataset of Beam-Formed Data
    
    \author Lars B&auml;hren

    \date 2010/12/05

    \test tBF_StokesDataset.cc

    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>\ref dal_icd_003
      <li>\ref dal_devel_bf
    </ul>
    
    <h3>Synopsis</h3>

    Within each Beam group, there are either one or four Stokes Datasets; or the
    Stokes Group contains BF RAW data with two 32-bit complex numbers (containing
    Xreal, Ximg, Yreal and Yimg).  If the data are summed, then there is only one
    Stokes I Group containing all the channel intensities per subbeam. If the
    data are not summed, then there are four Stokes tables -- \f$ (I, Q, U, V) \f$
    or \f$ (XX, XY, YX, YY) \f$ -- one per polarization, containing all the channel
    intensities per subbeam.

    \verbatim
    STOKES_{N}                Dataset
    |-- GROUPTYPE             Attribute           string
    |-- DATATYPE              Attribute           string
    |-- STOKES_COMPONENT      Attribute           string
    |-- NOF_SAMPLES           Attribute           int
    |-- NOF_SUBBANDS          Attribute           int
    `-- NOF_CHANNELS          Attribute           array<int,1>
    \endverbatim

    The datatype of the elements stored within the dataset depends on the Stokes
    component to be recorded:

    <center>
    <table>
      <tr>
        <td class="indexkey">Description</td>
        <td class="indexkey">Stokes</td>
        <td class="indexkey">Datatype</td>
        <td class="indexkey">Specification</td>
        <td class="indexkey">HDF5 datatype</td>
      </tr>
      <tr>
        <td>Complex voltages</td>
        <td>\f$ (X,Y) \f$</td>
        <td>fcomplex</td>
        <td>2 x 32 bit IEEE floating point</td>
	<td>(H5T_NATIVE_FLOAT,H5T_NATIVE_FLOAT)</td>
      </tr>
      <tr>
        <td>Coherent Stokes</td>
        <td>\f$ (I,Q,U,V) \f$</td>
        <td>float</td>
        <td>32 bit IEEE floating point</td>
	<td>H5T_NATIVE_FLOAT</td>
      </tr>
      <tr>
        <td>Incoherent Stokes</td>
        <td>\f$ (I,Q,U,V) \f$</td>
        <td>float</td>
        <td>32 bit IEEE floating point</td>
	<td>H5T_NATIVE_FLOAT</td>
      </tr>
    </table>
    </center>

    The shape of the dataset is determined by
    - the number of time bins (\c NOF_SAMPLES),
    - the number of sub-bands (\c NOF_SUBBANDS),
    - the number of channels per sub-band (\c NOF_CHANNELS)

    Mapping of input variables onto internal parameters describing organization
    and shape of the dataset:

    <center>
    <table>
      <tr>
        <td class="indexkey">nofSamples</td>
        <td class="indexkey">nofSubbands</td>
        <td class="indexkey">nofChannels</td>
        <td class="indexkey">shape</td>
        <td class="indexkey">\f$ N_{\rm Samples} \f$</td>
        <td class="indexkey">\f$ N_{\rm Bands} \f$</td>
        <td class="indexkey">\f$ N_{\rm Channels} \f$</td>
        <td class="indexkey">\f$ N_{\rm Shape} \f$</td>
      </tr>
      <tr>
        <td>yes</td>
        <td>yes</td>
        <td>yes</td>
        <td>--</td>
        <td>nofSamples</td>
        <td>nofSubbands</td>
        <td>nofChannels</td>
        <td></td>
      </tr>
      <tr>
        <td>yes</td>
        <td>--</td>
        <td>yes</td>
        <td>--</td>
        <td>nofSamples</td>
        <td>1</td>
        <td>nofChannels</td>
        <td>[nofSamples,nofChannels]</td>
      </tr>
      <tr>
        <td>--</td>
        <td>yes</td>
        <td>yes</td>
        <td>--</td>
        <td>1</td>
        <td>nofSubbands</td>
        <td>nofChannels</td>
        <td>[1,nofSubbands*nofChannels]</td>
      </tr>
      <tr>
        <td>--</td>
        <td>--</td>
        <td>yes</td>
        <td>--</td>
        <td>1</td>
        <td>1</td>
        <td>nofChannels</td>
        <td>[1,nofChannels]</td>
      </tr>
      <tr>
        <td>--</td>
        <td>--</td>
        <td>--</td>
        <td>yes</td>
        <td>shape[0]</td>
        <td>1</td>
        <td>shape[1]</td>
        <td>shape</td>
      </tr>
    </table>
    </center>
    
    <h3>Example(s)</h3>
    
  */  
  class BF_StokesDataset : public HDF5DatasetBase {

    //! Set of attributes attached to the dataset
    std::set<std::string> itsAttributes;
    //! Stokes component stored inside this dataset
    DAL::Stokes itsStokesComponent;
    //! Number of channels within the subbands
    std::vector<unsigned int> itsNofChannels;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    BF_StokesDataset ();
    
    //! Argumented constructor, opening an existing Stokes dataset
    BF_StokesDataset (hid_t const &location,
		      std::string const &name,
		      IO_Mode const &flags=IO_Mode(IO_Mode::Open));

    //! Argumented constructor, opening an existing Stokes dataset
    BF_StokesDataset (hid_t const &location,
		      unsigned int const &index,
		      IO_Mode const &flags=IO_Mode(IO_Mode::Open));
    
    //! Argumented constructor, creating a new Stokes dataset
    BF_StokesDataset (hid_t const &location,
		      unsigned int const &index,
		      unsigned int const &nofSubbands,
		      unsigned int const &nofChannels,
		      DAL::Stokes::Component const &component=DAL::Stokes::I,
		      hid_t const &datatype=H5T_NATIVE_FLOAT,
		      IO_Mode const &flags=IO_Mode(IO_Mode::CreateNew));
    
    //! Argumented constructor, creating a new Stokes dataset
    BF_StokesDataset (hid_t const &location,
		      unsigned int const &index,
		      unsigned int const &nofSamples,
		      unsigned int const &nofSubbands,
		      unsigned int const &nofChannels,
		      DAL::Stokes::Component const &component=DAL::Stokes::I,
		      hid_t const &datatype=H5T_NATIVE_FLOAT,
		      IO_Mode const &flags=IO_Mode(IO_Mode::CreateNew));
    
    //! Argumented constructor, creating a new Stokes dataset
    BF_StokesDataset (hid_t const &location,
		      unsigned int const &index,
		      unsigned int const &nofSamples,
		      std::vector<unsigned int> const &nofChannels,
		      DAL::Stokes::Component const &component=DAL::Stokes::I,
		      hid_t const &datatype=H5T_NATIVE_FLOAT,
		      IO_Mode const &flags=IO_Mode(IO_Mode::CreateNew));
    
    //! Argumented constructor, creating a new Stokes dataset
    BF_StokesDataset (hid_t const &location,
		      unsigned int const &index,
		      std::vector<hsize_t> const &shape,
		      DAL::Stokes::Component const &component=DAL::Stokes::I,
		      hid_t const &datatype=H5T_NATIVE_FLOAT,
		      IO_Mode const &flags=IO_Mode(IO_Mode::CreateNew));
    
    //! Argumented constructor, creating a new Stokes dataset
    BF_StokesDataset (hid_t const &location,
		      std::vector<hsize_t> const &shape,
		      DAL::Stokes::Component const &component=DAL::Stokes::I,
		      IO_Mode const &flags=IO_Mode(IO_Mode::CreateNew));
    
    //! Copy constructor
    BF_StokesDataset (BF_StokesDataset const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~BF_StokesDataset ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    BF_StokesDataset& operator= (BF_StokesDataset const &other); 
    
    // === Parameter access =====================================================

    //! Get the number of bins along the time axis.
    unsigned int nofSamples ();
    
    //! Get the number of bins along the frequency axis.
    unsigned int nofFrequencies ();
    
    //! Get the number of sub-bands
    inline unsigned int nofSubbands () const {
      return itsNofChannels.size();
    }

    //! Get the number of channels per sub-band
    inline std::vector<unsigned int> nofChannels () const {
      return itsNofChannels;
    }
    
    //! Get the number of channels per sub-band
    inline unsigned int nofChannels (unsigned int const &subband)
    {
      if (itsNofChannels.size()<subband) {
	return 0;
      } else {
	return itsNofChannels[subband];
      }
    }
    
    /*!
      \brief Get the name of the class
      \return className -- The name of the class, BF_StokesDataset.
    */
    inline std::string className () const {
      return "BF_StokesDataset";
    }

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);    

    // === Methods ==============================================================
    
    //! Get Stokes component stored within this dataset
    inline DAL::Stokes stokesComponent () const {
      return itsStokesComponent;
    }
    
    //! Get name of Stokes component stored within this dataset
    inline std::string stokesComponentName () const {
      return itsStokesComponent.name();
    }

    //! Get type of Stokes component stored within this dataset
    inline DAL::Stokes::Component stokesComponentType () const {
      return itsStokesComponent.type();
    }
    
    //! Open an existing Stokes dataset
    bool open (hid_t const &location,
	       std::string const &name,
	       IO_Mode const &flags=IO_Mode(IO_Mode::Open));
    
    //! Create a new Stokes dataset
    bool open (hid_t const &location,
	       DAL::Stokes::Component const &component,
	       unsigned int const &nofSamples,
	       unsigned int const &nofSubbands,
	       unsigned int const &nofChannels,
	       IO_Mode const &flags=IO_Mode(IO_Mode::CreateNew));
    
    //! Create a new Stokes dataset
    bool open (hid_t const &location,
	       DAL::Stokes::Component const &component,
	       unsigned int const &nofSamples,
	       std::vector<unsigned int> const &nofChannels,
	       IO_Mode const &flags=IO_Mode(IO_Mode::CreateNew));
    
    // === Static methods =======================================================
    
    //! Convert dataset index to name of the HDF5 dataset
    static std::string getName (unsigned int const &index);
    
  private:
    
    //! Set up the list of attributes attached to the structure
    inline void setAttributes () {
      itsAttributes.clear();

      itsAttributes.insert("GROUPTYPE");
      itsAttributes.insert("DATATYPE");
      itsAttributes.insert("STOKES_COMPONENT");
      itsAttributes.insert("NOF_SAMPLES");
      itsAttributes.insert("NOF_SUBBANDS");
      itsAttributes.insert("NOF_CHANNELS");
    }

    //! Initialize the object's internal parameters
    bool init ();
    
    //! Initialize the internal parameters 
    inline bool init (hid_t const &location,
		      DAL::Stokes::Component const &component,
		      unsigned int const &nofChannels,
		      IO_Mode const &flags) {
      return open (location, component, 1, nofChannels, flags);
    }
    //! Initialize the internal parameters 
    inline bool open (hid_t const &location,
		      DAL::Stokes::Component const &component,
		      unsigned int const &nofSubbands,
		      unsigned int const &nofChannels,
		      IO_Mode const &flags) {
      return open (location, component, 1, nofSubbands, nofChannels, flags);
    }
    //! Initialize the internal parameters 
    inline bool init (hid_t const &location,
		      DAL::Stokes::Component const &component,
		      std::vector<unsigned int> const &nofChannels,
		      IO_Mode const &flags=IO_Mode(IO_Mode::Open)) {
      if (nofChannels.size()>1) {
	return open (location, component, nofChannels[0], 1, nofChannels[1], flags);
      } else {
	return false;
      }
    }
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class BF_StokesDataset -- end
  
} // Namespace DAL -- end

#endif /* BF_STOKESDATASET_H */

