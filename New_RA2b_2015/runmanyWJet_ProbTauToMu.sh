#!/bin/sh

type=$1 # 0 prediction, 1 expectation
outStr=$2
#export path=`pwd -P`
#export SUBMIT_DIR=`tar -cf Dir.tar ${path}`
export SUBMIT_DIR=`pwd -P`

#for WJetStr in 100_200 200_400 400_600 600_800 800_1200 1200_2500 2500_Inf; do
for WJetStr in 100_200; do

    export SubmitFile=submitScriptWJet_${WJetStr}.jdl
    if [ -e ${SubmitFile} ]; then
	rm ${SubmitFile}
    fi
    let a=1
    Njobs=`ls InputFiles_WJet_${WJetStr}/filelist_Spring15_WJet_HT_${WJetStr}_* | wc -l`
    njobs=`expr $Njobs - $a`
    echo number of jobs: $njobs
    mkdir -p qsub
    
    for i in `seq 0 $njobs`; do
	
	export filenum=$i
	export outStr=$outStr
	echo $filenum
	echo $code 
	export WJetStr=$WJetStr
	export Suffix=${WJetStr}_${outStr}_$filenum
	if [ $filenum -lt 10 ]
	then
	    export ArgTwo=filelist_Spring15_WJet_HT_${WJetStr}_00$filenum
	    export ArgTwoB=InputFiles_WJet_${WJetStr}/${ArgTwo}
	elif [ $filenum -lt 100 ]
	then
	    export ArgTwo=filelist_Spring15_WJet_HT_${WJetStr}_0$filenum
	    export ArgTwoB=InputFiles_WJet_${WJetStr}/${ArgTwo}
	else
	    export ArgTwo=filelist_Spring15_WJet_HT_${WJetStr}_$filenum
	    export ArgTwoB=InputFiles_WJet_${WJetStr}/${ArgTwo}
	fi
	export ArgThree=WJet_${Suffix}
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
#   echo $ArgOne
#   echo $ArgTwo
#   echo $ArgThree
#   echo $ArgFour
#   echo $ArgFive
#   echo $ArgSix
	    echo $Output
#   echo $Error
#   echo $Log
#   echo $Proxy
#
	    export Output_root_file=HadTauEstimation_WJet_${WJetStr}_${outStr}_${i}_00.root
	    
	    
	    
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
		echo Transfer_Output_Files = HadTauEstimation_WJet_${WJetStr}_${outStr}_${i}_00.root','MuJetMatchRate_WJet_${WJetStr}_${outStr}_${i}_00.root','Probability_Tau_mu_WJet_${WJetStr}_${outStr}_${i}_00.root>> ${SubmitFile}
		#echo Transfer_Output_Files = MuJetMatchRate_WJet_${WJetStr}_${outStr}_${i}_00.root>> ${SubmitFile}        
		echo transfer_output_remaps = '"'HadTauEstimation_WJet_${WJetStr}_${outStr}_${i}_00.root = TauHad2Multiple/HadTauEstimation_WJet_${WJetStr}_${outStr}_${i}_00.root';'MuJetMatchRate_WJet_${WJetStr}_${outStr}_${i}_00.root = TauHad2Multiple/MuJetMatchRate_WJet_${WJetStr}_${outStr}_${i}_00.root';'Probability_Tau_mu_WJet_${WJetStr}_${outStr}_${i}_00.root = TauHad2Multiple/Probability_Tau_mu_WJet_${WJetStr}_${outStr}_${i}_00.root'"'>> ${SubmitFile}
#		echo transfer_output_remaps = '"'MuJetMatchRate_WJet_${WJetStr}_${outStr}_${i}_00.root = TauHad2Multiple/MuJetMatchRate_WJet_${WJetStr}_${outStr}_${i}_00.root'"'>> ${SubmitFile}
		
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
	    export Output_root_file=GenInfo_HadTauEstimation_WJet_${WJetStr}_${outStr}_${i}_00.root
	    
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
		echo Transfer_Output_Files = GenInfo_HadTauEstimation_WJet_${WJetStr}_${outStr}_${i}_00.root','FailRate_GenTau_jet_WJet_${WJetStr}_${outStr}_${i}_00.root','HadTau_TauResponseTemplates_WJet_${WJetStr}_${outStr}_${i}_00.root','IsoEfficiencies_WJet_${WJetStr}_${outStr}_${i}_00.root','LostLepton2_MuonEfficienciesFromWJet_${WJetStr}_${outStr}_${i}_00.root','Probability_Tau_mu_WJet_${WJetStr}_${outStr}_${i}_00.root','TauBtaggedRate_WJet_${WJetStr}_${outStr}_${i}_00.root>> ${SubmitFile}        

#		echo Transfer_Output_Files = GenInfo_HadTauEstimation_WJet_${WJetStr}_${outStr}_${i}_00.root','FailRate_GenTau_jet_WJet_${WJetStr}_${outStr}_${i}_00.root','HadTau_TauResponseTemplates_WJet_${WJetStr}_${outStr}_${i}_00.root','IsoEfficiencies_WJet_${WJetStr}_${outStr}_${i}_00.root','LostLepton2_MuonEfficienciesFromWJet_${WJetStr}_${outStr}_${i}_00.root','Probability_Tau_mu_WJet_${WJetStr}_${outStr}_${i}_00.root','TauBtaggedRate_WJet_${WJetStr}_${outStr}_${i}_00.root','TriggerEff_WJet_${WJetStr}_${outStr}_${i}_00.root>> ${SubmitFile}        
		echo transfer_output_remaps = '"'GenInfo_HadTauEstimation_WJet_${WJetStr}_${outStr}_${i}_00.root = TauHadMultiple/GenInfo_HadTauEstimation_WJet_${WJetStr}_${outStr}_${i}_00.root';'FailRate_GenTau_jet_WJet_${WJetStr}_${outStr}_${i}_00.root = TauHadMultiple/FailRate_GenTau_jet_WJet_${WJetStr}_${outStr}_${i}_00.root';'HadTau_TauResponseTemplates_WJet_${WJetStr}_${outStr}_${i}_00.root = TauHadMultiple/HadTau_TauResponseTemplates_WJet_${WJetStr}_${outStr}_${i}_00.root';'IsoEfficiencies_WJet_${WJetStr}_${outStr}_${i}_00.root = TauHadMultiple/IsoEfficiencies_WJet_${WJetStr}_${outStr}_${i}_00.root';'LostLepton2_MuonEfficienciesFromWJet_${WJetStr}_${outStr}_${i}_00.root = TauHadMultiple/LostLepton2_MuonEfficienciesFromWJet_${WJetStr}_${outStr}_${i}_00.root';'Probability_Tau_mu_WJet_${WJetStr}_${outStr}_${i}_00.root = TauHadMultiple/Probability_Tau_mu_WJet_${WJetStr}_${outStr}_${i}_00.root';'TauBtaggedRate_WJet_${WJetStr}_${outStr}_${i}_00.root = TauHadMultiple/TauBtaggedRate_WJet_${WJetStr}_${outStr}_${i}_00.root'"'>> ${SubmitFile}

#		echo transfer_output_remaps = '"'GenInfo_HadTauEstimation_WJet_${WJetStr}_${outStr}_${i}_00.root = TauHadMultiple/GenInfo_HadTauEstimation_WJet_${WJetStr}_${outStr}_${i}_00.root';'FailRate_GenTau_jet_WJet_${WJetStr}_${outStr}_${i}_00.root = TauHadMultiple/FailRate_GenTau_jet_WJet_${WJetStr}_${outStr}_${i}_00.root';'HadTau_TauResponseTemplates_WJet_${WJetStr}_${outStr}_${i}_00.root = TauHadMultiple/HadTau_TauResponseTemplates_WJet_${WJetStr}_${outStr}_${i}_00.root';'IsoEfficiencies_WJet_${WJetStr}_${outStr}_${i}_00.root = TauHadMultiple/IsoEfficiencies_WJet_${WJetStr}_${outStr}_${i}_00.root';'LostLepton2_MuonEfficienciesFromWJet_${WJetStr}_${outStr}_${i}_00.root = TauHadMultiple/LostLepton2_MuonEfficienciesFromWJet_${WJetStr}_${outStr}_${i}_00.root';'Probability_Tau_mu_WJet_${WJetStr}_${outStr}_${i}_00.root = TauHadMultiple/Probability_Tau_mu_WJet_${WJetStr}_${outStr}_${i}_00.root';'TauBtaggedRate_WJet_${WJetStr}_${outStr}_${i}_00.root = TauHadMultiple/TauBtaggedRate_WJet_${WJetStr}_${outStr}_${i}_00.root';'TriggerEff_WJet_${WJetStr}_${outStr}_${i}_00.root = TauHadMultiple/TriggerEff_WJet_${WJetStr}_${outStr}_${i}_00.root'"'>> ${SubmitFile}
		echo queue>> ${SubmitFile}	
	    fi
	    
	fi
	
	#sleep 1

    done
    
  #
  # Actual submission
  # 
    condor_submit ${SubmitFile}

done

