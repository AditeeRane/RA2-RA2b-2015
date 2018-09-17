#!/bin/sh

ArgOne=$1   # 
ArgTwo=$2   # 
ArgThree=$3 # 
ArgFour=$4  # 
ArgFive=$5  # 
ArgSix=$6   # 
ArgSeven=$7 #  

#
# first go to the submission dir, and set up the environment
#

cd ${_CONDOR_SCRATCH_DIR} 
source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc6_amd64_gcc530
eval `scramv1 project CMSSW CMSSW_10_1_7`
cd CMSSW_10_1_7/src/
eval `scramv1 runtime -sh` # cmsenv is an alias not on the workers
echo "CMSSW: "$CMSSW_BASE
cd -  #goes back one directory; equivalent to cd ${_CONDOR_SCRATCH_DIR}
#mkdir -p TauHadMultiple
mkdir -p TauHad2Multiple
pwd
ls -l
#mkdir -p HadTauMultiple
#mkdir -p HadTau2Multiple
#make all

echo "compilation done"
ls -l
#
# run the job
#
echo $ArgOne $ArgTwo $ArgThree "." $ArgFive $ArgSix 
$ArgOne $ArgTwo $ArgThree "." $ArgFive $ArgSix

