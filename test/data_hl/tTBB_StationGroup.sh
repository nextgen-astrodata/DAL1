#!${BASH_EXCUTABLE}

## Determine local file name of the dataset

dataset_tbb_local=`echo ${dataset_tbb} | tr '/' '\n' | grep ".h5"`

## copy the original dataset to the local directory

echo "[tTBB_StationGroup] Copying dataset to local directory..."
cp ${dataset_tbb} .

echo "[tTBB_StationGroup] Adjusting file permissions on test dataset ..."
chmod a+rw $dataset_tbb_local

## Run the test program

echo "[tTBB_StationGroup] Running test program on input file $dataset_tbb_local ..."
./tTBB_StationGroup $dataset_tbb_local

## Clean up the test directory

echo "[tTBB_StationGroup] Cleaning up the test directory ..."
rm $dataset_tbb_local
