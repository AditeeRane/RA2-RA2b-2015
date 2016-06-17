#!/bin/sh

type=$1
outStr=$2

submitscript=submitScriptWJet.sh
submitscript1=submitScriptWJet_Expectation.sh

  mkdir -p qsub

if [ $type -eq 0 ]; then
  for WJetStr in 100_200 200_400 400_600 600_800 800_1200 1200_2500 2500_Inf; do 


      njobs=`ls InputFiles_WJet_${WJetStr}/filelist_Spring15_WJet_HT_${WJetStr}* | wc -l`
      #njobs=$[$njobs+1]
      echo number of jobs: $njobs
      #for i in `seq 1 $njobs`; do
        for i in `seq 0 $njobs`; do

          export filenum=$i
          export WJetStr=$WJetStr
          export outStr=$outStr
          echo $filenum
          if [ -e TauHad2Multiple/HadTauEstimation_WJet_${WJetStr}_${outStr}_${i}_00.root ]; then
            echo warning !
            echo exist TauHad2Multiple/HadTauEstimation_WJet_${WJetStr}_${outStr}_${i}_00.root
          else
            echo submitting TauHad2Multiple/HadTauEstimation_WJet_${WJetStr}_${outStr}_${i}_00.root
            qsub -l walltime=12:00:00 -N WJet_$WJetStr_$outStr -o qsub/ -e qsub/ -V $submitscript  
          fi

          sleep 1

        done


  done

fi


if [ $type -eq 1 ]; then

  for WJetStr in 100_200 200_400 400_600 600_800 800_1200 1200_2500 2500_Inf; do

      njobs=`ls InputFiles_WJet_${WJetStr}/filelist_Spring15_WJet_HT_${WJetStr}* | wc -l`
      #njobs=$[$njobs+1]
      echo number of jobs: $njobs
      #for i in `seq 1 $njobs`; do
        for i in `seq 0 $njobs`; do

          export filenum=$i
          export WJetStr=$WJetStr
          export outStr=$outStr
          echo $filenum
          if [ -e TauHad2Multiple/HadTauEstimation_WJet_${WJetStr}_${outStr}_${i}_00.root ]; then
            echo warning !
            echo exist TauHad2Multiple/HadTauEstimation_WJet_${WJetStr}_${outStr}_${i}_00.root
          else
            echo submitting TauHad2Multiple/HadTauEstimation_WJet_${WJetStr}_${outStr}_${i}_00.root          
            qsub -l walltime=12:00:00 -N WJet_$WJetStr_$outStr -o qsub/ -e qsub/ -V $submitscript1 
          fi

          sleep 1

        done

  done

fi



