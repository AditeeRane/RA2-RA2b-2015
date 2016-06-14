#!/bin/sh
#cd /home/borzou/CMSSW_7_4_0_pre1/src/RA2-RA2b-2015/New_RA2b_2015
cd $PBS_O_WORKDIR
hostname
source ~cmssoft/shrc
eval `scramv1 runtime -sh`
#module load root
#
if [ $filenum -lt 10 ]
then
./run_tauHad2 "InputFiles_T/filelist_Spring15_ST_${TStr}_0$filenum" "${TStr}_${outStr}_$filenum" "TauHad2Multiple" "00" "0"
else
./run_tauHad2 "InputFiles_T/filelist_Spring15_ST_${TStr}_$filenum" "${TStr}_${outStr}_$filenum" "TauHad2Multiple" "00" "0"
fi
