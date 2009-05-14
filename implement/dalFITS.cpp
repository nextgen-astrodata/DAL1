/*-------------------------------------------------------------------------*
 | $Id:: dalFITS.h 2126 2008-11-07 13:31:59Z baehren              $ |
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

#include "dalFITS.h"

using namespace std;
using namespace casa;

namespace DAL
{
  // ============================================================================
  //
  //  Construction / Destruction
  //
  // ============================================================================
  
  //! Default constructor
  dalFITS::dalFITS()
  {
  
  
  }


  //! Constructor with associated filename
  dalFITS::dalFITS(const string &filename) 
  {

  
  
  
  }


  
  dalFITS::~dalFITS() 
  {
    // close the FITS file
  
    // deallocate memory
  
  }

  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
 
  /*!
    Functions to get FITS administrative handles (fileptr, lattice, hdu etc.)
  */
  
  
  /*!
    File access functions open/close etc.  
  */

  /*!	Open a FITS file in mode: r, w, rw
  
    \param string --
  
    \return fitsret --
  */
  int dalFITS::open (int iomode)
  {
    int fitsret=0; 
	
    // Open fits file, status will be forwarded to object variable fitsstatus
    // need std::string.cstr()-method to get const char* which is needed by fits_open_file
    if(!(fitsret=fits_open_file(&fptr, filename.c_str(), iomode, &fitsstatus)))
    {
      throw "dalFITS::open";	// get fits error from fitsstatus property later
    }
    
    return fitsret;
  }
  


  /*
  casa::Lattice<Float>* dalFITS::getLattice ()
  {
    LatticeBase *lattice;			// generic lattice variable for casa lattice

    FITSImage::registerOpenFunction();		// Register the FITS and Miriad image types.


    lattice_Q=ImageOpener::openImage (filename_Q);	// try open the file with generic casa function
    
    if(lattice_Q==NULL)				// on error	
    {
	    throw "dalFITS::getLattice ";
    }

    // determine data type of lattice
    switch(lattice_Q->dataType()){
	    case TpFloat:
		    lattice_Q_float=dynamic_cast<ImageInterface<Float>*>(lattice_Q);
		    break;
	    case TpDouble:
		    lattice_Q_float=dynamic_cast<ImageInterface<Float>*>(lattice_Q);
		    break;
	    case TpComplex:
		    lattice_Q_complex=dynamic_cast<ImageInterface<Complex>*>(lattice_Q);
		    break;
	    case TpDComplex:
		    lattice_Q_dcomplex=dynamic_cast<ImageInterface<DComplex>*>(lattice_Q);
		    break;
	    default:
		    throw AipsError("Image has an invalid data type");
		    break;
    }
    
    // check if data type is in accordance with FITS header entry
 
  }
  */
  
  int dalFITS::close ()
  {
    int fitsret=0;
    
    if(!(fitsret=fits_close_file(fptr, &fitsstatus)))
    {
      throw "dalFITS::close";
    }
    
    return fitsret;
  }


  std::string dalFITS::getFitsError ()
  {
    char* fits_error_message;
  
    fits_get_errstatus(fitsstatus, fits_error_message);
    
    /* Martin's code
    void fitshandle::check_errors() const
   {
    if (status==0) return;
    char msg[81];
    fits_get_errstatus (status, msg);
    cerr << msg << endl;
    while (fits_read_errmsg(msg)) cerr << msg << endl;
    planck_fail("FITS error");
    }
   */
    
    return (string) fits_error_message;
  }


  int dalFITS::moveAbsoluteHDU(int hdu)
  {
    int fitsret=0;
    int hdutype;	// type of HDU
    
    if(!(fitsret=fits_movabs_hdu(fptr, hdu, &hdutype, &fitsstatus)))
    {
      throw "dalFITS::moveAbsoluteHDU";
    }
    
    return hdutype;
  }
  

  int dalFITS::moveRelativeHDU(int nhdu)
  {
    int fitsret=0;

    if(!(fitsret=fits_movrel_hdu(fptr, nhdu, NULL, &fitsstatus)))	// try to move nhdu
    {
      throw "dalFITS::moveRelativeHDU";
    }
  
    return fitsret;
  }


  int dalFITS::readCurrentHDU(int *hdupos)
  {
    int fitsret=0;
    
    if(!(fitsret=fits_get_hdu_num(fptr, hdupos)))
    {
      throw "dalFITS::readCurrentHDU";
    }
  
    return fitsret;
  }


  int dalFITS::moveNameHDU(int &hdutype, const std::string &extname)
  {
    int fitsret=0;

    // ignoring the version number of the extension
    if(!(fitsret=fits_movnam_hdu(fptr, hdutype, const_cast<char*>(extname.c_str()) , NULL, &fitsstatus)))
    {
      throw "dalFITS::moveNameHDU";
    }
    
    return fitsret;
  }


  int dalFITS::readHDUType(int *hdutype)
  {
    int fitsret=0;
  
    if(!(fitsret=fits_get_hdu_type(fptr,  hdutype, &fitsstatus)))
    {
      throw "dalFITS::readHDUType";
    }
    
    return fitsret;
  }


  int dalFITS::readFilename(char *filename, char &hdutype)
  {
    int fitsret=0;
  
    if(!(fitsret=fits_file_name(fptr, &hdutype, &fitsstatus)))
    {
      throw "dalFITS::readFilename";
    }
    
    return fitsret;
  }


  int dalFITS::readFileMode(int *mode)
  {
    int fitsret=0;
  
    if(!(fitsret=fits_file_mode(fptr, mode, &fitsstatus)))
    {
      throw "dalFITS::readFileMode";
    }

    return fitsret;
  }

 
  int dalFITS::readURLType(char *urltype)
  {
    int fitsret=0;
  
    if(!(fitsret=fits_url_type(fptr, urltype, &fitsstatus)))
    {
      throw "dalFITS::readURLType";
    }

    return fitsret;
  }


  int dalFITS::deleteFITSfile()
  {
    int fitsret=0;
    
    if(!(fitsret=fits_delete_file(fptr, &fitsstatus)))
    {
      throw "dalFITS::deleteFITSfile";
    }
    
    return fitsret;
  }

 
  int dalFITS::flushFITSfile()
  {
    int fitsret=0;
  
    if(!(fitsret=fits_flush_file(fptr, &fitsstatus)))
    {
      throw "dalFITS::fits_flush_file";
    }
    
    return fitsret;
  }


  int dalFITS::flushFITSBuffer()
  {
    int fitsret=0;
  
    if(!(fitsret=fits_flush_buffer(fptr, 0, &fitsstatus)))
    {
      throw "dalFITS::fits_flush_buffer";
    }
    
    return fitsret;
  }


  // ============================================================================
  //
  //	Image access functions
  //
  // ============================================================================

  int dalFITS::getImgType(int &bitpix)
  {
    int fitsret=0;

    if(!(fitsret=fits_get_img_type(fptr, &bitpix, &fitsstatus)))
    {
      throw "dalFITS::get_image_type";
    }

    return fitsret;	// pass on cfitsio return value
  }


  int dalFITS::getImgDim(int &naxis)
  {
    int fitsret=0;

    if(!(fitsret=fits_get_img_dim(fptr, &naxis,  &fitsstatus)))
    {
      throw "dalFITS::getImageDim";
    }

    return fitsret;	// pass on cfitsio return value
  }


  int dalFITS::getImgSize(int maxdim,  long &naxes)
  {
    int fitsret=0;

    if(!(fitsret=fits_get_img_size(fptr, maxdim, &naxes , &fitsstatus)))
    {
      throw "dalFITS::getImageSize";
    }

    return fitsret;	// pass on cfitsio return value
  }

  
  int dalFITS::getImgParam(int maxdim,  int &bitpix, int &naxis, long &naxes)
  {
    int fitsret=0;

    if(!(fitsret=fits_get_img_param(fptr, maxdim, &bitpix, &naxis, &naxes, &fitsstatus)))
    {
      throw "dalFITS::getImageParam";
    }

    return fitsret;	// pass on cfitsio return value
  }


  int dalFITS::createImg(int bitpix, int naxis, long *naxes)
  {
    int fitsret=0;

    if(!(fitsret=fits_create_img(fptr, bitpix, naxis , naxes, &fitsstatus)))
    {
      throw "dalFITS::createImg";
    }

    return fitsret;		// pass on cfitsio return value
  }


  int dalFITS::writePix(int datatype, long *fpixel, long nelements, void *array)
  {
    int fitsret=0;

    if(!(fitsret=fits_write_pix(fptr, datatype, fpixel, nelements, array, &fitsstatus)))
    {
      throw "dalFITS::writePix";
    }

    return fitsret;		// pass on cfitsio return value
  }


  int dalFITS::writePixNull(int datatype, long *fpixel, long nelements, void *array, void *nulval)
  {
    int fitsret=0;

    if(!(fitsret=fits_write_pixnull(fptr, datatype, fpixel , nelements, array, nulval, &fitsstatus)))
    {
      throw "dalFITS::writePix";
    }

    return fitsret;		// pass on cfitsio return value
  }

  
  int dalFITS::readPix(int datatype, long *fpixel, long nelements, void *nulval, void *array, int *anynul)
  {
    int fitsret=0;
 
    if(!(fitsret=fits_read_pix(fptr, datatype, fpixel, nelements, nulval, array, anynul, &fitsstatus)))
    {
      throw "dalFITS::readPix";
    }

    return fitsret;
  }


  int dalFITS::writeSubset(int datatype, long *fpixel, long *lpixel, double *array)
  {
    int fitsret=0;
  
    if(!(fitsret=fits_write_subset(fptr, datatype, fpixel, lpixel, array, &fitsstatus)))
    {
      throw "dalFITS::writeSubset";
    }
    
    return fitsret;
  }


  int dalFITS::readSubset(int  datatype, long *fpixel,
	  long *lpixel, long *inc, void *nulval,  void *array,
	  int *anynul)
  {
    int fitsret=0;

    if(!(fitsret=fits_read_subset(fptr, datatype, fpixel, lpixel, inc, nulval, array, anynul, &fitsstatus)))
    {
      throw "dalFITS::readSubset";
    }

    return fitsret;
  }

  
  // ============================================================================
  //
  //  RM-Cube output functions
  //
  // ============================================================================  


  int appendPlane(double *plane, int x, int y)
  {
    int fitsret=0;

    // Check if Faraday plane has the same x-/y-dimensions as naxes dimensions of FITS
  
    // Write to FITS file

    return fitsret;
  }


  int appendLine(vector<double> faradayline)
  {
    int fitsret=0;


    return fitsret;
  }


  /*! Append a tile to a FITS file
  
    \param tile --
    \param x --
    \param y --
  */
  int dalFITS::appendTile(double* tile,
			  int x,
			  int y)
  {
    int fitsret=0;
    
    
    return fitsret;
  }
  
  
  /*! Append a SubCube to a FITS file
    
    \param cube --
    \param x --
    \param y --
    \param z --
    
    \return fitsret
  */
  int dalFITS::appendCube(double* cube,
			  int x,
			  int y,
			  int z)
  {
    int fitsret=0;
    
    
    return fitsret;
  }




  // ============================================================================
  //
  //  Header access functions (keywords etc.)
  //
  // ============================================================================


  // Methods for reading keywords and records from a dalFITS file
  int dalFITS::getHDRspace(int &keysexist,
			   int &morekeys)
  {
    int fitsret=0;

    if(!(fitsret=fits_get_hdrspace(fptr, &keysexist, &morekeys, &fitsstatus)))
    {
      throw "dalFITS::getHDRspace";
    }

    return fitsret;
  }


  int dalFITS::readRecord(int keynum,
			  std::string record)
  {
    int fitsret=0;

    if(!(fitsret=fits_read_record(fptr, keynum, const_cast<char *>(record.c_str()), &fitsstatus)))
    {
      throw "dalFITS::readRecord";
    }

    return fitsret;
  }
  
  
  int dalFITS::readCard(std::string keyname, std::string record)
  {
    int fitsret=0;

    if(!(fitsret=fits_read_card(fptr, const_cast<char *>(keyname.c_str()), const_cast<char *>(record.c_str()), &fitsstatus)))
    {
      throw "dalFITS::readCard";
    }

    return fitsret;
  }

  /*!
    \param datatype
    \param keyname
    \param value
    \param comment
  */
  int dalFITS::readKey(int datatype,
		       char *keyname,
		       void *value,
		       char *comment)
  {
    int fitsret=0;

    if(!(fitsret=fits_read_key(fptr, datatype, keyname, value, comment, &fitsstatus)))
    {
      throw "dalFITS::readKey";
    }

    return fitsret;
  }
    
  /*!
    \param inclist
    \param ninc
    \param exclist
    \param nexc
    \param card
  */
  int dalFITS::findNextKey(char **inclist,
			   int ninc,
			   char **exclist,
			   int nexc,
			   char *card)
  {
    int fitsret=0;

    if(!(fitsret=fits_find_nextkey(fptr, inclist, ninc, exclist, nexc, card, &fitsstatus)))
    {
      throw "dalFITS::findNextKey";
    }

    return fitsret;
  }

  /*!
    \param keyname
    \param unit
  */
  int dalFITS::readKeyUnit(std::string keyname,
			   std::string unit)
  {
    int fitsret=0;
  
    if(!(fitsret=fits_read_key_unit(fptr, const_cast<char *>(keyname.c_str()), const_cast<char *>(unit.c_str()), &fitsstatus)))
    {
      throw "dalFITS::readKeyUnit";
    }

    return fitsret;
  }


  //===============================================================
  //
  // Methods for writing keywords and records to a dalFITS file
  //
  //===============================================================

  /*!
    \param 
  */
  int dalFITS::writeKey(int datatype,
			std::string keyname,
			void *value,
			std::string comment)
  {
    int fitsret=0;
  
    if(!(fitsret=fits_write_key(fptr, datatype, const_cast<char 
				*>(keyname.c_str()), value, const_cast<char *>(comment.c_str()), &fitsstatus)))
      {
	throw "dalFITS::writeKey";
      }
    
    return fitsret;
  }
  
  /*!
    \param datatype
    \param keyname
    \param value
    \param comment
  */
  int dalFITS::updateKey(int datatype,
			 std::string keyname,
			 void *value,
			 std::string comment)
  {
    int fitsret=0;
    
    if(!(fitsret=fits_update_key(fptr,
				 datatype,
				 const_cast<char *>(keyname.c_str()),
				 value,
				 const_cast<char *>(comment.c_str()),
				 &fitsstatus)))
      {
	throw "dalFITS::updateKey";
      }
    
    return fitsret;
  }
  
  
  int dalFITS::writeRecord(std::string card)
  {
    int fitsret=0;
    
    if(!(fitsret=fits_write_record(fptr, const_cast<char *>(card.c_str()), &fitsstatus)))
    {
      throw "dalFITS:writeRecord";
    }

    return fitsret;
  }
  

  int dalFITS::modifyComment(std::string keyname, std::string comment)
  {
    int fitsret=0;
  
    if(!(fitsret=fits_modify_comment(fptr, const_cast<char *>(keyname.c_str()), const_cast<char *>(comment.c_str()), &fitsstatus)))
    {
      throw "dalFITS::modifyComment";
    }

    return fitsret;
  }


  int dalFITS::writeKeyUnit(std::string keyname, std::string unit)
  {
    int fitsret=0;
  
    if(!(fitsret=fits_write_key_unit(fptr, const_cast<char *>(keyname.c_str()), const_cast<char *>(unit.c_str()), &fitsstatus)))
    {
      throw "dalFITS::writeKeyUnit";
    }

    return fitsret;
  }


  int dalFITS::writeComment(std::string comment)
  {
    int fitsret=0;
    
    if(!(fitsret=fits_write_comment(fptr, const_cast<char *>(comment.c_str()),  &fitsstatus)))
    {
      throw "dalFITS::writeComment";
    }

    return fitsret;
  }


  int dalFITS::writeHistory(std::string history)
  {
    int fitsret=0;
  
    if(!(fitsret=fits_write_history(fptr, const_cast<char *>(history.c_str()),  &fitsstatus)))
    {
      throw "dalFITS::writeHistory";
    }
  
    return fitsret;
  }
  

  int dalFITS::writeDate()
  {
    int fitsret=0;
    
    if(!(fitsret=fits_write_date(fptr,  &fitsstatus)))
    {
      throw "dalFITS::writeDate";
    }

    return fitsret;
  }


  int dalFITS::deleteRecord(int keynum)
  {
    int fitsret=0;
  
    if(!(fitsret=fits_delete_record(fptr, keynum,  &fitsstatus)))
    {
      throw "dalFITS::deleteRecord";
    }

    return fitsret;
  }


  int dalFITS::deleteKey(std::string keyname)
  {
    int fitsret=0;

    if(!(fitsret=fits_delete_key(fptr, const_cast<char *>(keyname.c_str()),  &fitsstatus)))
    {
      throw "dalFITS::deleteKey";
    }

    return fitsret;
  }


  // Header utility routines
  
  int dalFITS::copyHeader(dalFITS &other)
  {
    int fitsret=0;

    if(!(fitsret=fits_copy_header(fptr, other.fptr, &fitsstatus)))
    {
      throw "dalFITS::copyHeader";
    }
  
    return fitsret;
  }


  int dalFITS::writeChecksum()
  {
    int fitsret=0;
    
    if(!(fitsret=fits_write_chksum(fptr, &fitsstatus)))
    {

    }
    
    return fitsret;
  }

  
  int dalFITS::verifyChecksum(bool &dataok, bool &hduok)
  {
    int fitsret=0;
    
    int dataok_int=0, hduok_int=0;
  
    if(!(fitsret=fits_verify_chksum(fptr, &dataok_int, &hduok_int, &fitsstatus)))
    {
      throw "dalFITS::verifyChecksum";
    }

    // convert int dataok and int hduk to bools
    if(dataok_int)
      dataok=true;
    if(hduok_int)
      hduok=true;
    
    return fitsret;
  }


  int dalFITS::parseValue(std::string card, std::string value, std::string comment)
  {
    int fitsret=0;
    
    if(!(fitsret=fits_parse_value(const_cast<char *>(card.c_str()), const_cast<char *>(value.c_str()), const_cast<char *>(comment.c_str()), &fitsstatus)))
    {
      throw "dalFITS::parseValue";
    }

    return fitsret;
  }


  int dalFITS::getKeytype(std::string value, char &dtype)
  {
    int fitsret=0;
    
    if(!(fitsret=fits_get_keytype(const_cast<char *>(value.c_str()), &dtype, &fitsstatus)))
    {
      throw "dalFITS::getKeytype";
    }

    return fitsret;
  }


  int dalFITS::getKeyclass(std::string card)
  {
    int fitsret=0;
  
    if(!(fitsret=fits_get_keyclass(const_cast<char *>(card.c_str()))))
    {
      throw "dalFITS::getKeyclass";
    }

    return fitsret;
  }
  
  
  int dalFITS::parseTemplate(std::string templatec, std::string card, int &keytype)
  {
    int fitsret=0;
    
    if(!(fitsret=fits_parse_template(const_cast<char *>(templatec.c_str()), const_cast<char *>(card.c_str()), &keytype, &fitsstatus)))
    {
      throw "dalFITS::parseTemplate";
    }
    
    return fitsret;
  }


  // ============================================================================
  //
  //  Header access functions (specific to RM)
  //
  // ============================================================================


  int dalFITS::writeRMHeader(int hdu)
  {
    int fitsret=0;
  
    // Move current FITS HDU to primary image

    // Write individual header keywords to FITS
  
    return fitsret;
  }


  // ============================================================================
  //
  //	Table access functions (not necessary for RM cubes at the moment)
  //
  // ============================================================================

  // Implement these in a dalFITSTable.h/.cpp class?


  // ============================================================================
  //
  //  Class summary functions & Co.
  //
  // ============================================================================

  /*!
      \param os -- Output stream to which the summary is going to be written
  */

  void dalFITS::summary (std::ostream &os)
  {
    os << "[dalFITS] Summary of object properties" << std::endl;
    os << "-- Filename : " << std::endl;
  }
  
}
