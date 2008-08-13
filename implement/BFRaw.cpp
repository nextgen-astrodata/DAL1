/*-------------------------------------------------------------------------*
 | $Id:: dal.h 1126 2007-12-10 17:14:20Z masters                         $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008 by Joseph Masters                                  *
 *   J.S.Masters@uva.nl                                               *
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
#ifndef BFRAW_H
#include "BFRaw.h"
#endif

/*
dataStruct * channelize( dataStruct * data,
                         int start,
                         const unsigned long arraylength,
                         int factor )
{
  const int DS_SIZE = arraylength / factor;
  dataStruct * ds_data = NULL;
  try
  {
    ds_data = new dataStruct[ DS_SIZE ];
  }
  catch (bad_alloc)
  {
    cerr << "Can't allocate memory for downsampled array." << endl;
  }

  for (int count=0; count<DS_SIZE; count++)
  {
    for (int idx=start; idx < (start+factor); idx++)
    {
      ds_data[count].xx += data[idx].xx;
      ds_data[count].yy += data[idx].yy;
    }
    start += factor;
  }
  return ds_data;
}

*/

namespace DAL
  {

  BFRaw::BFRaw( string const& filename, bool doIntensity, bool doDownsample,
                bool doChannelization, int factor )
  {
    // initializations (private)
    bigendian = BigEndian();
    first_block = true;
    outputfilename = filename;
    rawfile = NULL;
    downsample_factor = factor;
    DO_FLOAT32_INTENSITY = doIntensity;
    DO_DOWNSAMPLE = doDownsample;
    DO_CHANNELIZATION = doChannelization;
  }

  BFRaw::~BFRaw()
  {
    if (rawfile)
      {
        if (rawfile->is_open())
          {
            rawfile->close();
          }
        delete rawfile;
        rawfile = NULL;
      }
  }


  bool BFRaw::eof()
  {
    if ( rawfile->peek() == EOF )
      return true;
    else
      return false;
  }

  char * BFRaw::RArad2deg( const float &rad )
  {
    char * ra_string = new char[14];

    float deg_hour = ( rad * (180./PI) ) / 15.;
    int   hour = (int)deg_hour;
    float deg_min = (deg_hour - hour) * 60.;
    int    min = int( deg_min );
    float  sec = (deg_min - min) * 60;

    sprintf(ra_string,"%02d:%02d:%02.4f", hour, min, sec);

    return ra_string;
  }

  char * BFRaw::DECrad2deg( const float &rad )
  {
    char * ra_string = new char[14];

    float deg_hour = ( rad * (180./PI) );
    int   hour = int( deg_hour );
    float deg_min = (deg_hour - hour) * 60.;
    int    min = int( deg_min );
    float  sec = (deg_min - min) * 60;

    sprintf(ra_string,"%02d:%02d:%02.4f", hour, min, sec);

    return ra_string;
  }

  void BFRaw::openRawFile( char* filename )
  {
    delete rawfile;
    rawfile = new fstream( filename, ios::binary|ios::in );
    rawfile->seekg (0, ios::beg);  // move to start of file
  }

  void BFRaw::readRawFileHeader()
  {
    if ( !rawfile->read ( reinterpret_cast<char *>(&fileheader),
                          sizeof(fileheader) ))
      {
        cout << "ERROR: problem with read (fileheader)." << endl;
        cout << "read pointer position: " << rawfile->tellg() << endl;
      }

#ifdef DEBUGGING_MESSAGES
    printf("size of file header: %lu\n", sizeof(fileheader));
    cout << "read pointer position: " << rawfile->tellg() << endl;
#endif

    // swap values when necessary
    if ( !bigendian )
      {
        // change byte order for all frequencies.
        for (int ii=0;ii<54;ii++)
          {
            swapbytes((char *)&fileheader.subbandFrequencies[ii],8);
            swapbytes((char *)&fileheader.beamlet2beams,2);
          }

        for (int ii=0;ii<8;ii++)
          {
            for (int jj=0;jj<2;jj++)
              {
                swapbytes((char *)&fileheader.beamDirections[ii][jj],8);
              }
          }

        // change byte order for beamlets.
        swapbytes((char *)&fileheader.nrBeamlets,2);

        // change byte order for nrSamplesPerBeamlet
        swapbytes((char *)&fileheader.nrSamplesPerBeamlet,4);

        // change byte order for sampleRate
        swapbytes((char *)&fileheader.sampleRate,8);

        // change byte order for Magic Number
        swapbytes((char *)&fileheader.magic,4);

      }

#ifdef DEBUGGING_MESSAGES
    printf("Magic number: %8X\n",fileheader.magic);
    printf("bits per sample: %u\n",fileheader.bitsPerSample);
    printf("Polarizations : %u\n",fileheader.nrPolarizations);
    printf("Beamlets : %u\n",fileheader.nrBeamlets);
    printf("Samples per beamlet: %u\n", fileheader.nrSamplesPerBeamlet);
    printf("Station ID: %s\n", fileheader.station);
    printf("Sample rate: %g\n", fileheader.sampleRate);
    printf("Centre Freq. of subbands (MHz): \n");
    for (int ii=0;ii<fileheader.nrBeamlets;ii++)
      {
        printf("%9.6f ",fileheader.subbandFrequencies[ii]/1000000.0);
        if (((ii+1)%4) == 0 ) printf("\n");
      }
    printf("Beam Directions J2000 radians:\n");
    char * ra = NULL;
    char * dec = NULL;
    for (int ii=0;ii<8;ii++)
      {
        printf("[%d] ", ii );
        for (int jj=0;jj<2;jj++)
          {
            printf("%f   ",fileheader.beamDirections[ii][jj]);
            if ( 0 == jj )
              ra = RArad2deg( fileheader.beamDirections[ii][jj] );
            else
              dec = DECrad2deg( fileheader.beamDirections[ii][jj] );
          }
        printf("[  %s, %s ]", ra, dec );
        printf("\n");
        delete [] ra;
        ra = NULL;
        delete [] dec;
        dec = NULL;
      }
#endif
  }

  void BFRaw::makeH5OutputFile()
  {
    dataset = dalDataset( outputfilename.c_str(), "HDF5" );

    // root-level headers
    int n_stations[] = { 1 };
    vector<string> srcvec;
    srcvec.push_back( "" );
    int main_beam_diam[] = { 0 };
    int bandwidth[] = { 0 }; // Total bandwidth (MHz)
    int breaks_in_data[] = { 0 }; // Any breaks in data?
    int dispersion_measure[] = { 0 };
    int number_of_samples[] =
      { fileheader.nrBeamlets * fileheader.nrSamplesPerBeamlet };
    Float64 sampling_time[] = { fileheader.sampleRate };
    int number_of_beams[] = { 1 };
    int sub_beam_diameter[] = { 0 }; // fwhm of the sub-beams (arcmin)
    int weather_temperature[] = { 0 }; // approx. centigrade
    int weather_humidity[] = { 0 }; // approx. %
    int tsys[] = { 0 }; // for various stations (K)

    // write headers using above
    dataset.setAttribute_string( "FILENAME", outputfilename.c_str() );
    dataset.setAttribute_string( "TELESCOPE", "LOFAR" );
    dataset.setAttribute_int( "NUMBER_OF_STATIONS", n_stations );
    dataset.setAttribute_string( "DATATYPE", "" );
    dataset.setAttribute_string( "EMBAND", "" );
    dataset.setAttribute_string( "SOURCE", srcvec );
    dataset.setAttribute_string( "OBSERVATION_ID", "" );
    dataset.setAttribute_string( "PROJ_ID", "" );
    dataset.setAttribute_string( "POINT_RA", "" );
    dataset.setAttribute_string( "POINT_DEC", "" );
    dataset.setAttribute_string( "OBSERVER", "" );
    dataset.setAttribute_int( "MAIN_BEAM_DIAM", main_beam_diam );
//   dataset.setAttribute_int( "CENTER_FREQUENCY", center_freq );
    dataset.setAttribute_int( "BANDWIDTH", bandwidth );
    dataset.setAttribute_int( "BREAKS_IN_DATA", breaks_in_data );
    dataset.setAttribute_int( "DISPERSION_MEASURE", dispersion_measure );
    dataset.setAttribute_int( "NUMBER_OF_SAMPLES", number_of_samples );
    dataset.setAttribute_double( "SAMPLING_TIME", sampling_time );
    dataset.setAttribute_string( "NOTES", "" );
    dataset.setAttribute_int( "NUMBER_OF_BEAMS", number_of_beams );
    dataset.setAttribute_int( "SUB_BEAM_DIAMETER", sub_beam_diameter );
    dataset.setAttribute_int( "WEATHER_TEMPERATURE", weather_temperature );
    dataset.setAttribute_int( "WEATHER_HUMIDITY", weather_humidity );
    dataset.setAttribute_int( "TSYS", tsys );

    dalGroup * beamGroup;

    char * beamstr = new char[10];

    int beam_number;

    beam_number = 0;
    sprintf( beamstr, "beam%03d", beam_number );
    beamGroup = dataset.createGroup( beamstr );

    char * ra_val  = new char[20];
    char * dec_val = new char[20];
    sprintf( ra_val, "%f", fileheader.beamDirections[beam_number+1][0] );
    sprintf( dec_val, "%f",fileheader.beamDirections[beam_number+1][1] );
    beamGroup->setAttribute_string( "RA", ra_val );
    beamGroup->setAttribute_string( "DEC", dec_val );
    delete [] ra_val;
    ra_val = NULL;
    delete [] dec_val;
    dec_val = NULL;

    int n_subbands[] = { fileheader.nrBeamlets };
    beamGroup->setAttribute_int( "NUMBER_OF_SUBBANDS", n_subbands );

    delete beamGroup;

#ifdef DEBUGGING_MESSAGES
    cerr << "CREATED New beam group: " << string(beamstr) << endl;
#endif

    table = new dalTable * [ fileheader.nrBeamlets ];

    char * sbName = new char[8];
    int * center_frequency = new int[fileheader.nrBeamlets];

    // nrBeamlets is actually the number of subbands (see email from J.Romein)
    for (unsigned int idx=0; idx<fileheader.nrBeamlets; idx++)
      {
        sprintf( sbName, "SB%03d", idx );
        table[idx] = dataset.createTable( sbName, beamstr );
      }

    for (unsigned int idx=0; idx<fileheader.nrBeamlets; idx++)
      {
        if ( DO_DOWNSAMPLE || DO_FLOAT32_INTENSITY )
          {
            table[idx]->addColumn( "TOTAL_INTENSITY", dal_FLOAT );
          }
        else
          {
            table[idx]->addColumn( "X", dal_COMPLEX_SHORT );
            table[idx]->addColumn( "Y", dal_COMPLEX_SHORT );
          }
      }
    for (unsigned int idx=0; idx<fileheader.nrBeamlets; idx++)
      {
        center_frequency[idx] = (int)fileheader.subbandFrequencies[ idx ];
        table[idx]->setAttribute_int( "CENTER_FREQUENCY", &center_frequency[idx] );
      }

    delete [] sbName;
    sbName = NULL;
    delete [] center_frequency;
    center_frequency = NULL;
    delete [] beamstr;
    beamstr = NULL;

  } // BFRaw::makeH5OutputFile

  void BFRaw::processBlocks()
  {
    int16_t nseconds = 60;
    bool ret = true;
    do
      {
        ret = processBlocks( nseconds );
      }
    while ( true == ret );
  }

  bool BFRaw::processBlocks( int16_t blocks )
  {

    bool retval = true;

    int64_t oneSecondBlockSize =

      sizeof(blockheader)
      +
      (
        fileheader.nrSamplesPerBeamlet *
        2 * sizeof(complex<Int16>) * fileheader.nrBeamlets
      );

    int64_t blocksize = oneSecondBlockSize * blocks;

    char * buf = NULL;
    try
      {
#ifdef DEBUGGING_MESSAGES
        printf("Allocating %ld bytes (%d seconds)\n",blocksize,blocks);
#endif
        buf = new char[ blocksize ];
      }
    catch (bad_alloc)
      {
        printf("WARNING: Can't allocate memory buffer for %d seconds of data.\n",
               blocks );
        printf("    Retrying with %d seconds of data.\n", blocks/2 );
        delete [] buf;
        processBlocks( blocks/2 );
        return retval;
      }

    rawfile->read( buf, blocksize );

#ifdef DEBUGGING_MESSAGES
    cerr << "read pointer position: " << rawfile->tellg() << endl;
    cerr << "bytes read:            " << rawfile->gcount() << endl;
#endif

    if ( rawfile->fail() || rawfile->eof() || (rawfile->gcount() != blocksize) )
      {
        blocksize = rawfile->gcount();
        blocks = blocksize / oneSecondBlockSize;
        retval = false;
      }

#ifdef DEBUGGING_MESSAGES
    cerr << "blocksize " << blocksize << endl;
    cerr << "blocks    " << blocks << endl;
#endif

    BlockHeader * pbuf = NULL;
    Sample * sample = new Sample[ fileheader.nrSamplesPerBeamlet ];
    int32_t index = 0;

    for ( int blk=0 ; blk < blocks ; blk++ )
      {
        pbuf = reinterpret_cast<BlockHeader*>(&(buf[ blk * (blocksize/blocks) ]));

        // swap values when necessary
        if ( !bigendian )
          {
            swapbytes((char *)&pbuf->magic,4);
            for ( uint ii = 0; ii < 8; ii++ )
              {
                swapbytes((char *)&pbuf->coarseDelayApplied[ ii ],4);
                swapbytes((char *)&pbuf->fineDelayRemainingAtBegin[ ii ],8);
                swapbytes((char *)&pbuf->fineDelayRemainingAfterEnd[ ii ],8);
                swapbytes((char *)&pbuf->time[ ii ],8);
                swapbytes((char *)&pbuf->nrFlagsRanges[ ii ],4);
                for ( uint jj = 0; jj < 16; jj++ )
                  {
                    swapbytes( (char *)&pbuf->flagsRanges[ ii ][ jj ].begin,4 );
                    swapbytes( (char *)&pbuf->flagsRanges[ ii ][ jj ].end,4 );
                  }
              }
          }

        if ( first_block )
          {
            time_t utc;
            utc = (time_t)(pbuf->time[0]/(Int64)fileheader.sampleRate);
            char * timeDateString = NULL;
            uint16_t buf_size = 128;
            if (!timeDateString)
              timeDateString = (char *)malloc(buf_size*sizeof(char));

            memset (timeDateString,'\0',buf_size);
            strftime(timeDateString, buf_size, "%T", gmtime(&utc));
            dataset.setAttribute_string( "EPOCH_UTC", timeDateString );

            memset (timeDateString,'\0',buf_size);
            strftime(timeDateString, buf_size, "%d/%m/%y", gmtime(&utc));
            dataset.setAttribute_string( "EPOCH_DATE", timeDateString );

            memset (timeDateString,'\0',buf_size);

            memset (timeDateString,'\0',buf_size);

            free(timeDateString);
            first_block = false;
          }

#ifdef _OPENMP
#pragma omp parallel for ordered schedule(dynamic)
#endif
        for ( uint8_t subband=0; subband < fileheader.nrBeamlets; subband++ )
          {

            index = blk *
                    (
                      (fileheader.nrSamplesPerBeamlet * 8) *
                      fileheader.nrBeamlets
                    )
                    + (blk+1) * sizeof(blockheader) +
                    subband * fileheader.nrSamplesPerBeamlet * 8;

            sample = reinterpret_cast<Sample*>(&( buf[ index ]));

            if ( DO_DOWNSAMPLE )  // if downsampling
              {
                Float32 * downsampled_data;
                int start = 0;
                downsampled_data =
                  downsample_to_float32_intensity( sample,
                                                   start,
                                                   fileheader.nrSamplesPerBeamlet,
                                                   downsample_factor );
#ifdef _OPENMP
#pragma omp ordered
#endif
                table[subband]->appendRows( downsampled_data,
                                            fileheader.nrSamplesPerBeamlet / downsample_factor );
                delete [] downsampled_data;
                downsampled_data = NULL;
              }
            else  // no downsampling
              {

                if ( DO_FLOAT32_INTENSITY )
                  {
                    Float32 * intensity_data;
                    int start = 0;
                    intensity_data =
                      compute_float32_intensity( sample,
                                                 start,
                                                 fileheader.nrSamplesPerBeamlet );
#ifdef _OPENMP
#pragma omp ordered
#endif
                    table[subband]->appendRows( intensity_data,
                                                fileheader.nrSamplesPerBeamlet );
                    delete [] intensity_data;
                    intensity_data = NULL;
                  }
                else
                  {
                    table[subband]->appendRows( sample,
                                                fileheader.nrSamplesPerBeamlet );
                  }
              }

          } // for subband

      } // for block (blk)

    delete [] buf;
    buf = NULL;

#ifdef DEBUGGING_MESSAGES
// if (!first_block) return false;
#endif

    return retval;

  }

  Float32 *
  BFRaw::compute_float32_intensity( Sample * data,
                                    int32_t start,
                                    const uint64_t arraylength )
  {
    double xx_intensity = 0;
    double yy_intensity = 0;
    Float32 * totalintensity = NULL;
    try
      {
        totalintensity = new Float32[ arraylength ];
      }
    catch (bad_alloc)
      {
        cerr << "Can't allocate memory for total intensity array." << endl;
      }

#ifdef _OPENMP
#pragma omp parallel for ordered schedule(dynamic)
#endif
    for ( uint count = 0; count < arraylength; count++ )
      {
        totalintensity[ count ] = 0;

        for ( uint idx = start; idx < ( start + arraylength ); idx++ )
          {
            xx_intensity = ( (double)real(data[ idx ].xx) * real(data[ idx ].xx) +
                             (double)imag(data[ idx ].xx) * imag(data[ idx ].xx) );
            yy_intensity = ( (double)real(data[ idx ].yy) * real(data[ idx ].yy) +
                             (double)imag(data[ idx ].yy) * imag(data[ idx ].yy) );
            totalintensity[count] +=
              (Float32)std::sqrt( xx_intensity * xx_intensity +
                                  yy_intensity * yy_intensity );
          }
      }
    return totalintensity;
  }

  Float32 *
  BFRaw::downsample_to_float32_intensity( Sample * data,
                                          int32_t start,
                                          const uint64_t arraylength,
                                          int32_t factor )
  {
    const int DS_SIZE = arraylength / factor;
    double xx_intensity = 0;
    double yy_intensity = 0;
    Float32 * ds_data = NULL;
    try
      {
        ds_data = new Float32[ DS_SIZE ];
      }
    catch (bad_alloc)
      {
        cerr << "Can't allocate memory for downsampled array." << endl;
      }

#ifdef _OPENMP
#pragma omp parallel for ordered schedule(dynamic)
#endif
    for ( int count = 0; count < DS_SIZE; count++ )
      {
        ds_data[count] = 0;
        for ( int idx = start; idx < (start+factor); idx++ )
          {
            xx_intensity = ( (double)real(data[ idx ].xx) * real(data[ idx ].xx) +
                             (double)imag(data[ idx ].xx) * imag(data[ idx ].xx) );
            yy_intensity = ( (double)real(data[ idx ].yy) * real(data[ idx ].yy) +
                             (double)imag(data[ idx ].yy) * imag(data[ idx ].yy) );
            ds_data[count] +=
              (Float32)std::sqrt( xx_intensity * xx_intensity +
                                  yy_intensity * yy_intensity );
          }
        start += factor;
      }
    return ds_data;
  }


} // DAL namespace
