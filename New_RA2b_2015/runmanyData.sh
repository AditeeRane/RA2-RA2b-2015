#!/bin/sh

sample=$1
outputStr=$2
export SUBMIT_DIR=`pwd -P`

export SubmitFile=submitScriptData.jdl
if [ -e ${SubmitFile} ]; then
    rm ${SubmitFile}
fi
let a=1
Njobs=`ls InputFiles_Data/filelist_data_SingleMuon_${sample}_* | wc -l `
    #let njobs=Njobs - a
njobs=`expr $Njobs - $a`

echo number of jobs: $njobs
mkdir -p qsub

for i in `seq 0 $njobs`; do
#for i in 0 100 101 102 103 104 105 106 107 108 109 ; do
    
    export filenum=$i
    export sample=$sample
    export outStr=$outputStr
	#echo $filenum
	#echo $code 
	#export TTbarStr=$TTbarStr
    export Suffix=SingleMuon_${sample}_${outStr}_$filenum
    if [ $filenum -lt 10 ]
    then
	export ArgTwo=filelist_data_SingleMuon_${sample}_000$filenum
	export ArgTwoB=InputFiles_Data/${ArgTwo}
    elif [ $filenum -lt 100 ]
    then
	export ArgTwo=filelist_data_SingleMuon_${sample}_00$filenum
	export ArgTwoB=InputFiles_Data/${ArgTwo}
    elif [ $filenum -lt 1000 ]
    then
	export ArgTwo=filelist_data_SingleMuon_${sample}_0$filenum
	export ArgTwoB=InputFiles_Data/${ArgTwo}
    else
	export ArgTwo=filelist_data_SingleMuon_${sample}_$filenum
	export ArgTwoB=InputFiles_Data/${ArgTwo}
    fi
    export ArgThree=data_${Suffix}
    export ArgFive=00
    export ArgSix=0
    export ArgSeven=$SUBMIT_DIR
    export Output=qsub/condor_${Suffix}.out
    export Error=qsub/condor_${Suffix}.err
    export Log=qsub/condor_${Suffix}.log
    export Proxy=\$ENV\(X509_USER_PROXY\)
    
    source /cvmfs/cms.cern.ch/cmsset_default.sh

    cd $SUBMIT_DIR
    eval `scram runtime -sh`
	#echo "ROOTSYS"  ${ROOTSYS}
    
	#
	# Prediction
	#
    
	    #echo $filenum
    export ArgFour=TauHad2Multiple
    export ArgOne=./run_tauHad2    
	    #echo $ArgOne
	    #echo $ArgTwo
	    #echo $ArgThree
	    #echo $ArgFour
	    #echo $ArgFive
	    #echo $ArgSix
    echo $Output
	    #echo $Error
	    #echo $Log
	    #echo $Proxy

    export Output_root_file=HadTauEstimation_data_SingleMuon_${sample}_${outStr}_${i}_00.root
    
    
    
    if [ -e TauHad2Multiple/${Output_root_file} ]; then
	echo warning !
	echo exist TauHad2Multiple/${Output_root_file}
    else
	echo submitting TauHad2Multiple/${Output_root_file}
	
                #
                # Creating the submit .jdl file
                #
	if [ $i -eq 0 ]; then
	    echo executable = submitScript_condor.sh>> ${SubmitFile}
	    echo universe =vanilla>> ${SubmitFile}
	    echo x509userproxy = ${Proxy}>> ${SubmitFile}
	    echo notification = never>> ${SubmitFile}
	    echo should_transfer_files = YES>> ${SubmitFile}
	    echo WhenToTransferOutput = ON_EXIT>> ${SubmitFile}
	fi
	
	echo "">> ${SubmitFile}
	echo Arguments =${ArgOne} ${ArgTwo} ${ArgThree} ${ArgFour} ${ArgFive} ${ArgSix} ${ArgSeven}>> ${SubmitFile} 
	echo Output = ${Output}>> ${SubmitFile}
	echo Error = ${Error}>> ${SubmitFile}
	echo Log = ${Log}>> ${SubmitFile}
	echo Transfer_Input_Files = ${SUBMIT_DIR}/run_tauHad2,${SUBMIT_DIR}/Inputs,${SUBMIT_DIR}/${ArgTwoB}>> ${SubmitFile}
	echo Transfer_Output_Files = HadTauEstimation_data_SingleMuon_${sample}_${outStr}_${i}_00.root','MuJetMatchRate_data_SingleMuon_${sample}_${outStr}_${i}_00.root>> ${SubmitFile}
		#echo Transfer_Output_Files = MuJetMatchRate_TTbar_${TTbarStr}_${outStr}_${i}_00.root>> ${SubmitFile}        
	echo transfer_output_remaps = '"'HadTauEstimation_data_SingleMuon_${sample}_${outStr}_${i}_00.root = TauHad2Multiple/HadTauEstimation_data_SingleMuon_${sample}_${outStr}_${i}_00.root';'MuJetMatchRate_data_SingleMuon_${sample}_${outStr}_${i}_00.root = TauHad2Multiple/MuJetMatchRate_data_SingleMuon_${sample}_${outStr}_${i}_00.root'"'>> ${SubmitFile}
#		echo transfer_output_remaps = '"'MuJetMatchRate_TTbar_${TTbarStr}_${outStr}_${i}_00.root = TauHad2Multiple/MuJetMatchRate_TTbar_${TTbarStr}_${outStr}_${i}_00.root'"'>> ${SubmitFile}
	
	echo queue>> ${SubmitFile}	
	
    fi # if [ -e TauHad2Multiple/${Output_root_file} ]; then
    
    

	#
	# Expectation
	#
    
	#sleep 1

    
done
    #
    # Actual submission
    # 
condor_submit ${SubmitFile}
    

