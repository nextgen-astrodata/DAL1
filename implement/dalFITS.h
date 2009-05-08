/*-------------------------------------------------------------------------*
 | $Id:: TBB_Timeseries.h 2126 2008-11-07 13:31:59Z baehren              $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008                                                    *
 *   Sven Duscha (sduscha@mpa-garching.mpg.de)                                                        *
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
#include <sstream>	// string strings

// cfitsio
#include <fitsio.h>


// AIPS++/CASA header files
#include <casa/aips.h>
#include <casa/Arrays.h>
#include <casa/BasicSL/Complex.h>
#include <casa/Utilities/DataType.h>
#include <casa/Containers/Record.h>
#include <coordinates/Coordinates/CoordinateSystem.h>
#include <coordinates/Coordinates/DirectionCoordinate.h>
#include <coordinates/Coordinates/LinearCoordinate.h>
#include <coordinates/Coordinates/SpectralCoordinate.h>
#include <lattices/Lattices/TiledShape.h>
#include <lattices/Lattices/LatticeBase.h>
#include <lattices/Lattices/LatticeIterator.h>			// Iterator over lattices
#include <images/Images/ImageOpener.h>				// wrapper class for
#include <images/Images/HDF5Image.h>
#include <images/Images/PagedImage.h>
#include <tables/Tables/TableDesc.h>
#include <tables/Tables/SetupNewTab.h>
#include <tables/Tables/Table.h>
#include <tables/Tables/TableRecord.h>

using namespace casa;

namespace DAL {
  
  /*!
    \class dalFITS

    \ingroup DAL

    \brief DAL class to provide access to FITS

    \author Sven Duscha

    \test tdalFITS.cpp
  */
  class dalFITS {


  private:
    //! file handle for direct file access
    fitsfile *fptr;
 
    //! Status of last cfitsio operation
    int fitsstatus;
    
    //! Filename associated with dalFITS object
    std::string filename;

    //! CASAcore lattice handle
    Lattice<Float>* lattice;	// casacore Lattice interface to image

    //! No. of axes in FITS file
//     long naxis;

    //! dimensions of FITS image
    std::vector<long> dimensions;

    //! bzero value
//     double bzero;

    //! scaling factor
//     double bscale;

    //! define types of bins
    enum DALbinType {
      frequency,
      lambda_sq
    };
    
    DALbinType binType;

    //! define bin units (Hz, MHz, etc)
    enum DALbinUnit {
      Hz,
      kHz,
      MHz
    };
    //! Bin type of the image
    DALbinUnit binUnit;

    //! frequency bins (central frequencies) or lambda squareds
    std::vector<double> bins;

    //! bin deltas (width of each bin)
    std::vector<double> binWidths;


    //! data type of FITS Image (in accordance with casacore naming scheme)
    enum DALimageType {
      TpFloat,
      TpDouble,
      TpComplex,
      TpDComplex
    };
 
    DALimageType imageType;

  public:

    //________________________________________________________________
    //                                        Construction/Destruction
    
    /*!
      \brief Default constructor, with no filenmae set 
    */
    dalFITS ();
 
    /*!
      \brief Constructor with associated filename
    
      \param filename --
    */
    dalFITS (const std::string &);
    
    /*!
      \brief Destructor
    */
    ~dalFITS ();
    
    /*! 
      \brief Copy constructor

      \param other --
    */
    dalFITS (dalFITS const& other);

    
    //________________________________________________________________
    // 
    // Methods
   //________________________________________________________________
 
    /*! 
      \brief open a FITS file for reading, writing or both
      
      \param iomode --

      \return fitsret --
    */
    int open(int iomode);	// modes as defined in cfitsio library: RO, W, RW
    
    /*!
      \brief close a FITS file handle
      
      \return fitsret --
    */
    int close();

    /*!
      \brief create a new FITS file

      \return fitsret --
    */
    int create();

    /*!
      \brief Get a (casa) Lattice<Float> to access the fits file
    
      \return LatticeBase* --
    */
    casa::Lattice<Float>* getLattice ();


    /*! 
      \brief Get the corresponding cfits error message to the current fitsstatus
    
      \return fitserrormsg --
    */
    std::string getFitsError ();


    /*! 
      \brief Get the number of HDUs in the FITS file

      \return fitsret --
    */
    int readNumHDUs ();


    /*! 
      \brief Read the current header postion (HDU) in FITS file
    
      \param *hdu --
      
      \return fitsret --
    */
    int readCurrentHDU(int *currenthdu);


    /*!	
      \brief Move to HDU unit given by hdu
    
      \param hdu --
      
      \return hdutype --
    */
    int moveAbsoluteHDU(int hdu);


    /*! 
      \brief Move relative by hdu units
      
      \param hdu --
      
      \return fitsret --
    */  
    int moveRelativeHDU(int hdu);


    /*! 
      \brief Move to hdu extension with name, extname
      
      \param *hdutype --
      \param extname --
      
      \return fitstret --
    */
    int moveNameHDU(int &hdutype, const std::string &extname);


    /*!	
      \brief Read type of HDU (IMAGE_HDU, ASCII_TBL, or BINARY_TBL)

      \param *hdutype --

      \return ret --
    */
    int readHDUType(int *hdutype);


    /*!
      \brief Read the filename of the currently opened FITS file
    
      \param filename --
      \param hdutype --

      \return fitsretval
    */
    int readFilename(char *filename, char &hdutype);


    /*! 
      \brief Read the IO mode of the currently opened FITS file
  
      \param mode --

      \return fitsret --
    */    
    int readFileMode(int *mode);


    /*! 
      \brief Read the url type, e.g. file://, ftp:// of the currently opened FITS file
      
      \param *urltype --
      
      \return fitsret --
    */
    int readURLType(char *urltype);


    /*!
      \brief Delete the fitsfile of the dalFITS object
    
      \return fitsret --
    */
    int deleteFITSfile();

    /*!
      \brief Flush the FITS file, close and reopen

      \return fitsret --
    */
    int flushFITSfile();

    /*!
      \brief Flush buffer (without proper closing and reopening)
    
      \return fitsret --
    */
    int flushFITSBuffer();

    
    // ============================================================================
    //
    //	Image access functions
    //
    // ============================================================================

    /*! 
      \brief Get image type of the FITS image
    
      \param &bitpix --

      \return ret --
    */
    int getImgType(int &bitpix);


    /*! 
      \brief Get image dimensions of the FITS image
      
      \param &naxis --

      \return ret --
    */
    int getImgDim(int &naxis);


    /*!
      \brief Get image size of the FITS image
  
      \param maxdim --
      \param &naxes --
      
      \return ret --
    */
    int getImgSize(int maxdim,  long &naxes);


    /*!
      \brief Get image parameters: maxdim, bitpix, naxis, naxes

      \param maxdim --
      \param *bitpix --
      \param *naxis --
      \param *naxes --

      \return ret --
    */
    int getImgParam(int maxdim,  int &bitpix, int &naxis, long &naxes);


    /*!
      \brief Create an image extension

      \param bitpix --
      \param naxis --
      \param *naxes --

      \return ret --
    */
    int createImg(int bitpix, int naxis, long *naxes);
    

    /*!
      \brief Write nelements pixels to FITS image extension
      
      \param datatype --
      \param *fpixel --
      \param nelements --
      \param *array --

      \return ret --
    */
    int writePix(int datatype, long *fpixel, long nelements, void *array);


    /*!
      \brief Write pixels to FITS image (any undefined value is replaced by nulval)
    
      \param datatype --
      \param *fpixel --
      \param nelements --
      \param *array --
      \param *nulval --
      
      \return fitsret --
    */
    int writePixNull(int datatype, long *fpixel, long nelements, void *array, void *nulval);

    /*! 
      \brief Read pixels from an FITS image
      
      \param datatype --
      \param *fpixel --
      \param n
      
      \return fitsret --
    */
    int readPix(int  datatype, long *fpixel, 
                  long nelements, void *nulval, void *array, 
                  int *anynul);


    /*! 
      \brief Write a subset to a FITS image
      
      \param datatype --
      \param *fpixel --
      \param *lpixel --
      \param *array --
      
      \return fitsret --
    */
    int writeSubset(int datatype, long *fpixel,
             long *lpixel, double *array);

    /*! 
      \brief Read a subset from a FITS image
      
      \param datatype --
      \param *fpixel --
      \param *lpixel --
      \param *inc --
      \param *nulval --
      \param *array --
      \param *anynul --
      
      \return fitsret --
    */
    int readSubset(int  datatype, long *fpixel,
             long *lpixel, long *inc, void *nulval,  void *array,
             int *anynul);

      
    // ===========================================================
    //
    // Header access functions
    //
    // ===========================================================

    //===============================================================
    //
    // Methods for reading keywords and records from a FITS file
    //
    //===============================================================

    /*!
      \brief Get size of Header space of current HDU
      
      \param &keysexist --
      \param &morekeys --
      
      \return fitsret --
    */
    int getHDRspace(int &keysexist, int &morekeys);


    /*!
      \brief Get a record from the current HDU
      
      \param keynum --
      \param *record --
      
      \return fitsret --
    */
    int readRecord(int keynum, std::string record);
    
    
    /*!
      \brief Get the record card from the current HDU
      
      \param keyname --
      \param record --
      
      \return fitsret --
    */
    int readCard(std::string keyname, std::string record);


    /*!
      \brief Read a key from the current HDU
      
      \param datatype --
      \param *keyname --
      \param *value --
      \param *comment --
      
      \return fitsret --
    */ 
    int readKey(int datatype, char *keyname, void *value, char *comment);


    /*!
      \brief Read the record card from the current HDU
      
      \param **inclist --
      \param ninc --
      \param **exclist --
      \param nexc --
      \param *card --
      
      \return fitsret --
    */
    int findNextKey(char **inclist, int ninc, char **exclist, int nexc, char *card);
    
    
    /*!
      \brief Read the key's unit from the key with keyname in the current HDU
      
      \param keyname --
      \param unit --
      
      \return fitsret --
    */
    int readKeyUnit(std::string keyname, std::string unit);
    
    
    
    //===============================================================
    //
    // Methods for writing keywords and records to a FITS file
    //
    //===============================================================
    
    /*!
      \brief Write a new key to the FITS header of the CHDU
    
      \param datatype --
      \param keyname --
      \param *value --
      \param comment --
      
      \return fitsret --
    */
    int writeKey(int datatype, std::string keyname, void *value, std::string comment);
    
    
    /*!
      \brief Update a key in the FITS header of the CHDU
      
      \param datatype --
      \param keyname --
      \param *value --
      \param comment --
      
      \return fitsret --
    */
    int updateKey(int datatype, std::string keyname, void *value, std::string comment);
    
    
    /*!
      \brief Write a record "card" to the FITS header of the CHDU
      
      \param card --
      
      \return fitsret --
    */
    int writeRecord(std::string card);


    /*!
      \brief Modify the comment of &keyname in the FITS header of the CHDU
      
      \param keyname --
      \param comment --
      
      \return fitsret --
    */
    int modifyComment(std::string keyname, std::string comment);
    
    /*!
      \brief Write a Unit to the key corresponding to &keyname in the CHDU
    
      \param keyname --
      \param unit --
      
      \return fitsret --
    */
    int writeKeyUnit(std::string keyname, std::string unit);
    
    
    /*!
      \brief Write a comment to the CHDU
    
      \param &comment --
      
      \return fitsret --
    */
    int writeComment(std::string comment);


    /*!
      \brief Write a HISTORY entry to the CHDU
      
      \param history --
      
      \return fitsret --
    */
    int writeHistory(std::string history);
    
    
    /*!
      \brief Write the current date to the CHDU
      
      \return fitsret --
    */
    int writeDate();
    
    /*!
      \brief Delete a record from the CHDU
      
      \param keynum --
      
      \return fitsret --
    */
    int deleteRecord(int keynum);


    /*!
      \brief Delete the key referenced by &keyname from the CHDU 
    
      \param keyname --
      
      \return fitsret --
    */
    int deleteKey(std::string keyname);


    /*!
      \brief Copy the Header of this FITS to the &other dalFITS object
      
      \param &other --
      
      \return fitsret --
    */
    int copyHeader(dalFITS &other);


    /*!
      \brief Write the Header checksum to the CHDU
      
      \return fitsret --
    */
    int writeChecksum();

  
    /*!
      \brief Verify checksum of the CHDU
      
      \param dataok --
      \param hduok --
      
      \return fitsret --
    */
    int verifyChecksum(bool &dataok, bool &hduok);


    /*!
      \brief Parse a record card into value and comment
      
      \param card --
      \param &value --
      \param &comment --
      
      \return fitsret -- 
    */
    int parseValue(std::string card, std::string value, std::string comment);


    /*!
      \brief Get the type of a key with &value in the CHDU
      
      \param value --
      \param dtype --
      
      \return fitsret --
    */
    int getKeytype(std::string value, char &dtype);


    /*!
      \brief Get the class of the key in &card
      
      \param card --
      
      \return fitsret --
    */
    int getKeyclass(std::string card);


    /*!
      \brief Parse the &templatec of &card
      
      \param templatec --
      \param card --
      \param &keytype --

      \return fitsret --
    */
    int parseTemplate(std::string templatec, std::string card, int &keytype);
   
    //===============================================================
    //
    // RM specific header functions
    //
    //===============================================================
    
    
    // Strictly speaking: writeRMHeader and bin-functions should be part of
    // RM class?
    
    /*!
      \brief Write RM header to a FITS file
      
      \param hdu (default=1) --
    
      \return fitsret --
    */
    int writeRMHeader(int hdu=1);


     /*!
      \brief Get the bin unit (center frequencies) of a FITS image
      
      \return binUnit -- unit associated with bins
    */
    inline DALbinUnit getBinUnit () const {
      return binUnit;
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


    /*! 
      \brief Get the associated filename of a FITS object
    
      \return filename --
    */
    inline std::string getFilename () const {
      return filename;
    }


    /*! 
      \brief Set the associated filename of a FITS object
      
      \param filename --
    */
    inline void setFilename (const std::string filename) {
      this->filename=filename;
    };


    // ============================================================================
    //
    //  RM-Cube output functions
    //
    // ============================================================================  


    /*! Append one image plane to a FITS file at the current filepointer position

      \param *plane --
      \param x --
      \param y --
    */
    int appendPlane(double *plane, int x, int y);


    /*! Append one line of sight to a FITS file at the current filepointer position

      \param line --
      \param x --
      \param y --
    */
    int appendLine(vector<double> faradayline);


    /*! Append a tile to a FITS file at the current filepointer position
    
      \param tile --
      \param x --
      \param y --
    */
    int appendTile(double* tile, int x, int y);
    
    
    /*! Append a SubCube to a FITS file at the current filepointer position

      \param cube --
      \param x --
      \param y --
      \param z --
    */
    int appendCube(double* cube, int x, int y, int z);



    // ============================================================================
    //
    // Class administrative functions
    //
    // ============================================================================

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
