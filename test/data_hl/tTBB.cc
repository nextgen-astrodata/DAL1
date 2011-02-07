/*-------------------------------------------------------------------------*
 | $Id:: tNewClass.cc 1964 2008-09-06 17:52:38Z baehren                  $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008                                                    *
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

#include <dal.h>

#ifndef TBB_H
#include "TBB.h"
#endif

// Namespace usage
using std::cerr;
using std::cout;
using DAL::TBB;

/*!
  \file tTBB.cc

  \ingroup DAL

  \brief A collection of test routines for the TBB class

  \author Lars B&auml;hren

  \date 2008/11/05
*/

// -----------------------------------------------------------------------------

/*!
  \brief Test constructors for a new TBB object

  \return nofFailedTests -- The number of failed tests within this function.
*/
int test_constructors (std::string const &filename)
{
  cout << "\n[tTBB::test_constructors]\n" << std::endl;

  int nofFailedTests (0);

  cout << "[1] Testing default constructor ..." << std::endl;
  try
    {
      TBB newTBB (filename);
      //
      newTBB.summary();
    }
  catch (std::string message)
    {
      cerr << message << std::endl;
      nofFailedTests++;
    }

  return nofFailedTests;
}

// -----------------------------------------------------------------------------

/*!
  \brief Test the functions providing access to internal parameters

  \param outfile - Name of the HDF5 output file

  \return nofFailedTests -- The number of failed tests within this function.
*/
int test_parameters (std::string const &outfile)
{
  std::cout << "\n[tTBB::test_parameters]\n" << std::endl;

  int nofFailedTests (0);
  TBB tbb (outfile);

  std::cout << "[1] Test assigning top-level attributes ..." << std::endl;
  try
    {
      tbb.setTelescope ("TheTelescope");
      tbb.setObserver ("TheObserver");
      tbb.setProject ("ThePoject");
      tbb.setObservation_id ("TheObservationID");
      tbb.setObservation_mode ("TheObservationMode");
      //
      tbb.summary();
    }
  catch (std::string message)
    {
      std::cerr << message << std::endl;
      nofFailedTests++;
    }

  std::cout << "[2] Assign time-outs ..." << std::endl;
  try
    {
      tbb.setTimeoutStart (1.5);
      tbb.summary();
      //
      tbb.setTimeoutStart (1,200);
      tbb.summary();
      //
      tbb.setTimeoutRead (2,5);
      tbb.summary();
      //
      tbb.setTimeoutRead (0,50);
      tbb.summary();
    }
  catch (std::string message)
    {
      std::cerr << message << std::endl;
      nofFailedTests++;
    }

  cout << "[3] Retrieve time-outs ..." << endl;
  try
    {
      unsigned int time_sec;
      unsigned int time_usec;
      //
      tbb.timeoutStart(time_sec,time_usec);
      cout << "-- Start time-out = [" << time_sec << ";" << time_usec << "]" << endl;
      //
      tbb.timeoutRead(time_sec,time_usec);
      cout << "-- Read time-out  = [" << time_sec << ";" << time_usec << "]" << endl;
    }
  catch (std::string message)
    {
      std::cerr << message << std::endl;
      nofFailedTests++;
    }

  return nofFailedTests;
}

// -----------------------------------------------------------------------------

int main (int argc, char *argv[])
{
  int nofFailedTests (0);
  std::string rawfile;
  std::string outfile;
  std::string calfile;
  bool have_rawfile (false);
  bool have_outfile (false);
  bool have_calfile (false);

  //__________________________________________________________________
  // Check the input parameters provided form the command line

  if (argc < 2)
    {
      cerr << "[tTBB] Missing name of output file!" << std::endl;
      cerr << std::endl;
      cerr << " tTBB <HDF5 outfile>" << std::endl;
      cerr << " tTBB <HDF5 outfile> <TBB raw file>" << std::endl;
      cerr << " tTBB <HDF5 outfile> <TBB raw file> <Cal. file>" << std::endl;
      cerr << std::endl;
    }

  if (argc > 1)
    {
      outfile      = argv[1];
      have_outfile = true;
    }

  if (argc > 2)
    {
      rawfile      = argv[2];
      have_rawfile = true;
    }

  if (argc > 3)
    {
      calfile      = argv[3];
      have_calfile = true;
    }

  //__________________________________________________________________
  // Parameter summary

#ifdef DEBUGGING_MESSAGES
  cout << "[tTBB]" << endl;
  cout << "-- Output file      = " << outfile << endl;
  cout << "-- Input file       = " << rawfile << endl;
  cout << "-- Calibration file = " << calfile << endl;
#endif

  //__________________________________________________________________
  // Run the tests

  // Test for the constructor(s)
  nofFailedTests += test_constructors (outfile);

  if (nofFailedTests == 0)
    {

      nofFailedTests += test_parameters (outfile);

    }

  return nofFailedTests;
}
