#!/bin/sh

type=$1 # 0 prediction, 1 expectation
outStr=$2

export SUBMIT_DIR=`pwd -P`

#for TStr in tW_antitop; do
for TStr in t_top t_antitop tW_top tW_antitop s_channel; do
#for TStr in tW_top; do

    export SubmitFile=submitScriptT_${TStr}.jdl
    if [ -e ${SubmitFile} ]; then
	rm ${SubmitFile}
    fi
    let a=1    
    Njobs=`ls InputFiles_T/filelist_Spring15_ST_${TStr}_* | wc -l`
    njobs=`expr $Njobs - $a`
    echo number of jobs: $njobs
    mkdir -p qsub
    
    for i in `seq 0 $njobs`; do
	
	export filenum=$i
	export outStr=$outStr
	#echo $filenum
	#echo $code 
	export TStr=$TStr
	export Suffix=${TStr}_${outStr}_$filenum
	if [ $filenum -lt 10 ]
	then
	    export ArgTwo=filelist_Spring15_ST_${TStr}_0$filenum
	    export ArgTwoB=InputFiles_T/${ArgTwo}
	else
	    export ArgTwo=filelist_Spring15_ST_${TStr}_$filenum
	    export ArgTwoB=InputFiles_T/${ArgTwo}
	fi
	export ArgThree=${Suffix}
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
	echo "ROOTSYS"  ${ROOTSYS}
	
	if [ $type -eq 0 ]; then
	    echo $filenum
	    export ArgFour=TauHad2Multiple
	    export ArgOne=./run_tauHad2    
# echo $ArgOne
# echo $ArgTwo
# echo $ArgThree
# echo $ArgFour
# echo $ArgFive
# echo $ArgSix
	    echo $Output
# echo $Error
# echo $Log
# echo $Proxy
#
	    export Output_root_file=HadTauEstimation_${TStr}_${outStr}_${i}_00.root
	    
	    
	    
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
		echo Transfer_Output_Files = HadTauEstimation_${TStr}_${outStr}_${i}_00.root','MuJetMatchRate_${TStr}_${outStr}_${i}_00.root','Probability_Tau_mu_${TStr}_${outStr}_${i}_00.root>> ${SubmitFile}
		#echo Transfer_Output_Files = MuJetMatchRate_${TStr}_${outStr}_${i}_00.root>> ${SubmitFile}        
		echo transfer_output_remaps = '"'HadTauEstimation_${TStr}_${outStr}_${i}_00.root = TauHad2Multiple/HadTauEstimation_${TStr}_${outStr}_${i}_00.root';'MuJetMatchRate_${TStr}_${outStr}_${i}_00.root = TauHad2Multiple/MuJetMatchRate_${TStr}_${outStr}_${i}_00.root';'Probability_Tau_mu_${TStr}_${outStr}_${i}_00.root = TauHad2Multiple/Probability_Tau_mu_${TStr}_${outStr}_${i}_00.root'"'>> ${SubmitFile}
#		echo transfer_output_remaps = '"'MuJetMatchRate_${TStr}_${outStr}_${i}_00.root = TauHad2Multiple/MuJetMatchRate_${TStr}_${outStr}_${i}_00.root'"'>> ${SubmitFile}
		
		echo queue>> ${SubmitFile}	
	    	
	    fi
	    
	fi
	

	if [ $type -eq 1 ]; then
	    echo $filenum
	    export ArgFour=TauHadMultiple
	    export ArgOne=./run_tauHad
#	    echo $ArgOne
#	    echo $ArgTwo
#	    echo $ArgThree
#	    echo $ArgFour
#	    echo $ArgFive
#	    echo $ArgSix
	    echo $Output
#	    echo $Error
#	    echo $Log
#	    echo $Proxy
#
	    export Output_root_file=GenInfo_HadTauEstimation_${TStr}_${outStr}_${i}_00.root
	    
	    if [ -e TauHadMultiple/${Output_root_file} ]; then
		echo warning !
		echo exist TauHadMultiple/${Output_root_file}
	    else
		echo submitting TauHadMultiple/${Output_root_file}
		
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
		echo Transfer_Input_Files = ${SUBMIT_DIR}/run_tauHad,${SUBMIT_DIR}/${ArgTwoB}>> ${SubmitFile}

	#	echo Transfer_Output_Files = GenInfo_HadTauEstimation_${TStr}_${outStr}_${i}_00.root','FailRate_GenTau_jet_${TStr}_${outStr}_${i}_00.root>> ${SubmitFile}        

		echo Transfer_Output_Files = GenInfo_HadTauEstimation_${TStr}_${outStr}_${i}_00.root','FailRate_GenTau_jet_${TStr}_${outStr}_${i}_00.root','HadTau_TauResponseTemplates_${TStr}_${outStr}_${i}_00.root','IsoEfficiencies_${TStr}_${outStr}_${i}_00.root','LostLepton2_MuonEfficienciesFrom${TStr}_${outStr}_${i}_00.root','Probability_Tau_mu_${TStr}_${outStr}_${i}_00.root','TauBtaggedRate_${TStr}_${outStr}_${i}_00.root>> ${SubmitFile}        
#		echo transfer_output_remaps = '"'GenInfo_HadTauEstimation_${TStr}_${outStr}_${i}_00.root = TauHadMultiple/GenInfo_HadTauEstimation_${TStr}_${outStr}_${i}_00.root';'FailRate_GenTau_jet_${TStr}_${outStr}_${i}_00.root = TauHadMultiple/FailRate_GenTau_jet_${TStr}_${outStr}_${i}_00.root'"'>> ${SubmitFile}

		echo transfer_output_remaps = '"'GenInfo_HadTauEstimation_${TStr}_${outStr}_${i}_00.root = TauHadMultiple/GenInfo_HadTauEstimation_${TStr}_${outStr}_${i}_00.root';'FailRate_GenTau_jet_${TStr}_${outStr}_${i}_00.root = TauHadMultiple/FailRate_GenTau_jet_${TStr}_${outStr}_${i}_00.root';'HadTau_TauResponseTemplates_${TStr}_${outStr}_${i}_00.root = TauHadMultiple/HadTau_TauResponseTemplates_${TStr}_${outStr}_${i}_00.root';'IsoEfficiencies_${TStr}_${outStr}_${i}_00.root = TauHadMultiple/IsoEfficiencies_${TStr}_${outStr}_${i}_00.root';'LostLepton2_MuonEfficienciesFrom${TStr}_${outStr}_${i}_00.root = TauHadMultiple/LostLepton2_MuonEfficienciesFrom${TStr}_${outStr}_${i}_00.root';'Probability_Tau_mu_${TStr}_${outStr}_${i}_00.root = TauHadMultiple/Probability_Tau_mu_${TStr}_${outStr}_${i}_00.root';'TauBtaggedRate_${TStr}_${outStr}_${i}_00.root = TauHadMultiple/TauBtaggedRate_${TStr}_${outStr}_${i}_00.root'"'>> ${SubmitFile}

		echo queue>> ${SubmitFile}	
	    fi
	    
	fi
	
#	sleep 1

    done
    
  #
  # Actual submission
  # 
    condor_submit ${SubmitFile}

done

