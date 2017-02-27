#!/bin/sh

type=$1 # 0 prediction, 1 expectation
outStr=$2
#submitscript=submitScriptSignal.sh
export SUBMIT_DIR=`pwd -P`
export SubmitFile=submitScriptT2ttSignal.jdl
if [ -e ${SubmitFile} ]; then
    rm ${SubmitFile}
fi

let a=-1
i=`expr $a + 1`
for m in 150 167 175 183 192 200 208 217 225 233 242 250 258 267 275 283 292 300 308 317 325 333 342 350 358 367 375 383 392 400 408 417 425 433 442 450 458 467 475 483 492 500 508 517 525 533 542 550 558 567 575 583 592 600 608 617 625 633 642 650 658 667 675 683 692 700 708 717 725 733 742 750 758 767 775 783 792 800 808 817 825 833 850 875 900 925 950 1000 1050 1100 1150 1200 ; do
    
    for d in 1 25 50 63 75 88 100 113 125 138 150 163 175 188 200 213 225 238 250 263 275 288 300 313 325 338 350 363 375 388 400 413 425 438 450 463 475 488 500 513 525 538 550 563 575 588 600 613 625 638 650 ; do 

 	if [ "$d" -gt "$m" ] ; then
	    break
	fi
	
	

#	njobs=`ls InputFiles_Signal/RA2bin_T2tt_* | wc -l`
#	echo number of jobs: $njobs
	mkdir -p qsub
	#
	# Prediction
	#

	export filename=RA2bin_T2tt_${m}_${d}_fast 
	export ArgTwo=$filename
	export ArgTwoB=InputFiles_Signal/${filename} #input file
	
	if [ -e $ArgTwoB ]; then
	    i=`expr $i + 1`
	    echo $i    

	    export skmFile=tree_T2tt_${m}_${d}_fast.root
#	    export btagOne=CSVv2_ichep.csv
#	    export btagOneB=btag/${btagOne}    
#	    export btagTwo=CSV_13TEV_Combined_14_7_2016.csv
#	    export btagTwoB=btag/${btagTwo}
	    export motherM=${m}
	    export lspM=${d}
	    export outStr=$outStr
	    export Suffix=${filename}-${outStr}
	    export ArgThree=$Suffix
	    echo $filename
	    export ArgFive=00
	    export ArgSix=0
	    export ArgSeven=$SUBMIT_DIR
#	    export ArgEight=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV11/scan/tree_SLm
	    export ArgEight=/eos/uscms/store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV12/scan/tree_SLm
	    export Output=qsub/condor_T2tt_${m}_${d}.out
	    export Error=qsub/condor_T2tt_${m}_${d}.err
	    export Log=qsub/condor_T2tt_${m}_${d}.log
	    export Proxy=\$ENV\(X509_USER_PROXY\)
	    
	    source /cvmfs/cms.cern.ch/cmsset_default.sh
	    
	    cd $SUBMIT_DIR
	    eval `scram runtime -sh`
	#echo "ROOTSYS"  ${ROOTSYS}
	    

	    if [ $type -eq 0 ]; then
		export ArgFour=TauHad2Multiple
		export ArgOne=./run_tauHad2    
		echo $Output
		
		export Output_root_file=HadTauEstimation_${filename}-${outStr}_00.root
	        
	    
		if [ -e TauHad2Multiple/${Output_root_file} ]; then
		    echo warning !
		    echo exist TauHad2Multiple/${Output_root_file}
		else
		    echo submitting TauHad2Multiple/${Output_root_file}
		    
                #
                # Creating the submit .jdl file
                #
		    if [ $i -eq 1 ]; then
			echo executable = submitScript_condor.sh>> ${SubmitFile}
			echo universe =vanilla>> ${SubmitFile}
			echo x509userproxy = ${Proxy}>> ${SubmitFile}
			echo notification = never>> ${SubmitFile}
			echo should_transfer_files = YES>> ${SubmitFile}
			echo WhenToTransferOutput = ON_EXIT>> ${SubmitFile}
		    fi
		    
		    echo "">> ${SubmitFile}
		    echo Arguments =${ArgOne} ${ArgTwo} ${ArgThree} ${ArgFour} ${ArgFive} ${ArgSix} ${ArgSeven} ${skmFile} ${btagOne} ${btagTwo}>> ${SubmitFile} 
		    echo Output = ${Output}>> ${SubmitFile}
		    echo Error = ${Error}>> ${SubmitFile}
		    echo Log = ${Log}>> ${SubmitFile}
		    echo Transfer_Input_Files = ${SUBMIT_DIR}/run_tauHad2,${SUBMIT_DIR}/Inputs,${SUBMIT_DIR}/${ArgTwoB},${ArgEight}/${skmFile},${SUBMIT_DIR}/btag>> ${SubmitFile}
		    echo Transfer_Output_Files = HadTauEstimation_${filename}-${outStr}_00.root','MuJetMatchRate_${filename}-${outStr}_00.root>> ${SubmitFile}
		#echo Transfer_Output_Files = MuJetMatchRate_TTbar_${TTbarStr}_${outStr}_${i}_00.root>> ${SubmitFile}        
		    echo transfer_output_remaps = '"'HadTauEstimation_${filename}-${outStr}_00.root = TauHad2Multiple/HadTauEstimation_${filename}-${outStr}_00.root';'MuJetMatchRate_${filename}-${outStr}_00.root = TauHad2Multiple/MuJetMatchRate_${filename}-${outStr}_00.root'"'>> ${SubmitFile}
#		echo transfer_output_remaps = '"'MuJetMatchRate_TTbar_${TTbarStr}_${outStr}_${i}_00.root = TauHad2Multiple/MuJetMatchRate_TTbar_${TTbarStr}_${outStr}_${i}_00.root'"'>> ${SubmitFile}
		    echo queue>> ${SubmitFile}	
		fi # else of if [ -e TauHad2Multiple/${Output_root_file} ]; then
		
	    fi     # if [ $type -eq 0 ]; then
	else
	    continue
	    
	fi
	
    done
done
    #
    # Actual submission
    # 
condor_submit ${SubmitFile}
