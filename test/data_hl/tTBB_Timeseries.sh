#!${BASH_EXCUTABLE}

## copy the original dataset to the local directory

echo "-- Copying dataset to local directory..."
cp ${dataset_tbb} .
echo "-- Adjusting file permissions on test dataset ..."
chmod a+rw ${dataset_tbb_local}

## Run the test program

echo "-- Running test program on input file $dataset_tbb_local ..."
./tTBB_Timeseries ${dataset_tbb_local}

## Clean up the test directory

echo "-- Cleaning up the test directory ..."
rm ${dataset_tbb_local}
