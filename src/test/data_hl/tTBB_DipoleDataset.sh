#!${BASH_EXCUTABLE}

## Determine local file name of the dataset

dataset_tbb_local=`echo ${dataset_tbb} | tr '/' '\n' | grep ".h5"`

## copy the original dataset to the local directory

echo "[`date`] Copying dataset to local directory..."
cp ${dataset_tbb} .

echo "[`date`] Adjusting file permissions on test dataset ..."
chmod a+rw ${dataset_tbb_local}

## Run the test program

echo "[`date`] Running test program on input file $dataset_tbb_local ..."
./tTBB_DipoleDataset ${dataset_tbb_local}

## Clean up the test directory

echo "[`date`] Cleaning up the test directory ..."
rm ${dataset_tbb_local}
