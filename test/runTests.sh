#!/bin/sh

logdir=Testing/Temporary
logfile=$logdir/LastTest.log
FILES="tdalFilter test_std_cerr"
#FILES="tHDF5Attribute tHDF5Dataset tBF_StokesDataset"

## ==============================================================================
##
##  Do not edit blow this point!
##
## ==============================================================================

##_______________________________________________________________________________
## Function definitions

runExperimental ()
{
    make ExperimentalStart
    make ExperimentalConfigure
    make ExperimentalBuild
    make ExperimentalTest
    make ExperimentalMemCheck
    make ExperimentalSubmit
}

##_______________________________________________________________________________

## Build DAL Library (libdal)

echo "-- Building library dal..."

make -j5 dal >& $logfile

for FILE in $FILES
{
  echo "## =========================================================" >> $logfile
  echo "##                                                          " >> $logfile
  echo "## Build log for $FILE    "                                   >> $logfile
  echo "##                                                          " >> $logfile
  echo "## =========================================================" >> $logfile

  echo "-- Building test program $FILE ..."

  make $FILE >& $logdir/$FILE.log

  cat $logdir/$FILE.log >> $logdir/LastTest.log

  echo "## =========================================================" >> $logfile
  echo "##                                                          " >> $logfile
  echo "## Valgrind results for $FILE    "                            >> $logfile
  echo "##                                                          " >> $logfile
  echo "## =========================================================" >> $logfile

  testfile=`find . -name $FILE`

  echo "-- Running Valgrind test for $testfile ..."
  
  if [[ -z $1 ]] ; then
      valgrind --leak-check=full --track-origins=yes --read-var-info=yes $testfile >& $logdir/$FILE.log
  else
      valgrind --leak-check=full --track-origins=yes --read-var-info=yes $testfile $1 >& $logdir/$FILE.log
  fi

  cat $logdir/$FILE.log >> $logdir/LastTest.log
}
