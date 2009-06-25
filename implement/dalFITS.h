 /***************************************************************************
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
#include <tables/Tables/TiledFileAccess.h>
#include <lattices/Lattices/LatticeBase.h>
#include <lattices/Lattices/LatticeIterator.h>   // Iterator over lattices
#include <images/Images/ImageOpener.h>		 // wrapper class for
#include <images/Images/FITSImage.h>		 // high-level FITSImage interface
#include <images/Images/HDF5Image.h>
#include <images/Images/PagedImage.h>
#include <tables/Tables/TableDesc.h>
#include <tables/Tables/SetupNewTab.h>
#include <tables/Tables/Table.h>
#include <tables/Tables/TableRecord.h>

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
  //  std::string filename;

    //! CASAcore lattice handle
    casa::Lattice<casa::Float>* lattice;
    
    
    //! dimensions of FITS image
    std::vector<unsigned long long> dimensions;
    
    /*
    //! No. of axes in FITS file
    //     long naxis;
    
 
    //! Current hdu (chdu)
    int chdu;
    
    //! type of current HDU
    int hdutype;

    //! bzero value
//     double bzero;

    //! scaling factor
//     double bscale;
  */

    
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

    //! Default constructor
    dalFITS ();
    //! Argumented constructor
    dalFITS (const std::string &,
	     int mode);   
    //! Copy constructor
    dalFITS (dalFITS const& other);
    //! Copy constructor
    dalFITS(dalFITS const &other,
	    bool previous,
	    bool current,
	    bool following);
    //! Destructor
    ~dalFITS ();
    
    //________________________________________________________________
    // 
    // Methods
   //________________________________________________________________
 
    void openData(const std::string &filename, int iomode);
    void openImage(const std::string &filename, int iomode);
    void openTable(const std::string &filename, int iomode);
    void close();
    void getLattice();	// set Lattice in object
    std::string getFitsError();
    int readNumHDUs();
    int readCurrentHDU();
    void copyCHDU(dalFITS const &other);
    void moveAbsoluteHDU(int hdu);
    void moveRelativeHDU(int hdu);
    int moveNameHDU(const std::string &extname);
    void updateImageDimensions();
    int readHDUType();
    std::string readFilename();
    int readFileMode();
    std::string readURLType();
    void deleteFITSfile();
    void flushFITSfile();
    void flushFITSBuffer();

    
    // ============================================================================
    //
    //	Image access functions
    //
    // ============================================================================
    
    int getImgType();
    int getImgDim();
    void getImgSize();
    void getImgSize(int maxdim,
		    long *naxes);
    void getImgParam(int maxdim,
		     int &bitpix,
		     int &naxis,
		     long *naxes);
    void createImg(int bitpix,
		   int naxis,
		   long *naxes);
    void readPix(int datatype,
		 long *fpixel, 
		 long nelements,
		 void *nulval,
		 void *array, 
		 int *anynul);
    void writePix(int datatype, 
		  long *fpixel, 
		  long nelements, 
		  void *array);
    void writePixNull(int datatype, 
		      long *fpixel, 
		      long nelements, 
		      void *array, 
		      void *nulval); 
    void readSubset(int  datatype, 
		    long *fpixel,
		    long *lpixel, 
		    long *inc, 
		    void *nulval,  
		    void *array,
		    int *anynul);
    void writeSubset( int datatype, 
		      long *fpixel,
		      long *lpixel, 
		      double *array); 
    
    
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
    
    //! Get size of Header space of current HDU    
    void getHDRspace (int &keysexist,
		      int &morekeys);
    //! Get a record from the current HDU
    void readRecord (int keynum,
		     std::string record); 
    //! Get the record card from the current HDU
    void readCard (std::string keyname,
		   std::string record);
    //
    void readKeyn (int keynum,
		   std::string keyname,
		   std::string value,
		   std::string comment);
    //
    void readKey(int datatype,
		 std::string keyname,
		 void *value,
		 std::string comment);
    //
    void readKeyword(std::string keyname,
		     std::string value,
		     std::string comment);
    //
    void findNextKey(char **inclist,
		     int ninc,
		     char **exclist,
		     int nexc,
		     std::string card);
    //
    void readKeyUnit(std::string keyname,
		     std::string unit);

    //===============================================================
    //
    // Methods for writing keywords and records to a FITS file
    //
    //===============================================================
     
    void writeKey(int datatype, std::string keyname, void *value, std::string comment);
    void updateKey(int datatype, std::string &keyname, void *value, std::string &comment);
    void writeRecord(std::string &card);
    void modifyComment(std::string &keyname, std::string &comment);    
    void writeKeyUnit(std::string &keyname, std::string &unit);
    void writeComment(std::string &comment);
    void writeHistory(std::string &history);
    void writeDate();    
    void deleteRecord(int keynum);
    void deleteKey(std::string keyname);
    void copyHeader(dalFITS &other);
    void deleteHDU(int *hdutype);
    void writeChecksum();
    void verifyChecksum(bool &dataok, bool &hduok);
    void parseValue(std::string &card, std::string &value, std::string &comment);
    char getKeytype(std::string &value);
    int getKeyclass(std::string &card);
    int parseTemplate(std::string &templatec, std::string &card);
   
    //===============================================================
    //
    // RM specific header functions
    //
    //===============================================================
    
    
    // Strictly speaking: writeRMHeader and bin-functions should be part of
    // RM class?
    
    void writeRMHeader(int hdu=1);

    bool fileExists(const std::string &filename);

     /*!
      \brief Get the bin unit (center frequencies) of a FITS image
      
      \return binUnit - unit associated with bins
    */
    inline DALbinUnit getBinUnit () const {
      return binUnit;
    }

    /*!
      \brief Get the bins (center frequencies) of a FITS image
    
      \return bins - can be frequency or lambda squared
    */
    inline std::vector<double> getBins () const {
      return bins;
    }


    /*!
      \brief Get widths (for RM integration) of a FITS object (not necessarily in the image)
   
      \return binDeltas - delta steps between frequency bins
    */
    inline std::vector<double> getBinWidths () const {
      return binWidths;
    }

    // ============================================================================
    //
    //  RM-Cube input functions
    //
    // ============================================================================  

    void readPlane (double *plane,
		    const unsigned long z,
		    void *nulval=NULL);
    
    void readLine (double *line,
		   const unsigned long x,
		   const unsigned long y,
		   void *nulval=NULL);

    void readSubCube (double *subCube,
		      const unsigned long x_pos,
		      const unsigned long y_pos,
		      const unsigned long x_size,
		      const unsigned long y_size);

    // ============================================================================
    //
    //  RM-Cube output functions
    //
    // ============================================================================  

    void writePlane (double *plane,
		     const long x,
		     const long y,
		     const long z,
		     void *nulval=NULL);

    void writeLine(double *line, const long x, const long y, void *nulval=NULL);

    void writeTile( double* tile, 
		    const long x_size, 
		    const long y_size, 
		    const long x_pos, 
		    const long y_pos);   
    
    void writeCube(double* cube, const long x, const long y, const long z);

    void writeSubCube(  double* subcube, 
			const long x_size, 
			const long y_size, 
			const long x_pos, 
			const long y_pos);


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
