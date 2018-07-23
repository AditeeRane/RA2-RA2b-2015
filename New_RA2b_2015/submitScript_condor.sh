#!/bin/sh

ArgOne=$1   # 
ArgTwo=$2   # 
ArgThree=$3 # 
ArgFour=$4  # 
ArgFive=$5  # 
ArgSix=$6   # 
#ArgSeven=$7 #    //*AR---commented on 180126

#
# first go to the submission dir, and set up the environment
#
#cd $ArgSeven //*AR, 180126---this code was modified to make it consistent with other job submission scripts used for average TF method. These changes were in connection with nobackup area not accessible by condor
cd ${_CONDOR_SCRATCH_DIR}
source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc6_amd64_gcc530
eval `scramv1 project CMSSW CMSSW_8_0_25`
cd CMSSW_8_0_25/src/
eval `scramv1 runtime -sh` # cmsenv is an alias not on the workers
echo "CMSSW: "$CMSSW_BASE
cd -  #goes back one directory; equivalent to cd ${_CONDOR_SCRATCH_DIR}
#
# now go to the condor's scratch area, where we copied the contents of New_RA2b_2015
#
cd ${_CONDOR_SCRATCH_DIR} 
mkdir -p TauHadMultiple
mkdir -p TauHad2Multiple
pwd
ls -l
#mkdir -p HadTauMultiple
#mkdir -p HadTau2Multiple

#
# run the job
#
echo $ArgOne $ArgTwo $ArgThree "." $ArgFive $ArgSix
$ArgOne $ArgTwo $ArgThree "." $ArgFive $ArgSix

