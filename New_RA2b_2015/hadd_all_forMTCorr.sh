#! /bin/bash

# Modified from hadd_all.sh on June 16, 2016
# To execute, use the following commands------
#./hadd_all.sh BeforeMT                                                                                                                                                 
#./hadd_all.sh AfterMT         

cd TauHad2Multiple

for i in 100_200 200_400 400_600 600_800 800_1200 1200_2500 2500_Inf ; do 

  rm HadTauEstimation_WJet_${i}_.root
  hadd HadTauEstimation_WJet_${i}_.root  `ls HadTauEstimation_WJet_${i}_${1}_*00.root `  
  cp HadTauEstimation_WJet_${i}_.root ../TauHad2

  #rm Probability_Tau_mu_WJet_${i}_.root
  #hadd Probability_Tau_mu_WJet_${i}_.root `ls Probability_Tau_mu_WJet_${i}_${1}*00.root `
  #cp Probability_Tau_mu_WJet_${i}_.root ../TauHad2

done 

# Single top events not used for MT Corr, need to check with Ahmad.
#for i in  t_top tW_top t_antitop tW_antitop ; do
#
#  rm HadTauEstimation_${i}_.root
#  hadd HadTauEstimation_${i}_.root  `ls HadTauEstimation_${i}_*00.root `
#  cp HadTauEstimation_${i}_.root ../TauHad2
#
#  rm Probability_Tau_mu_${i}_.root
#  hadd Probability_Tau_mu_${i}_.root `ls Probability_Tau_mu_${i}*00.root `
#  cp Probability_Tau_mu_${i}_.root ../TauHad2
#
#done

rm HadTauEstimation_TTbar_.root
hadd HadTauEstimation_TTbar_.root  `ls HadTauEstimation_TTbar_*${1}*00.root `
cp HadTauEstimation_TTbar_.root ../TauHad2/

#rm Probability_Tau_mu_TTbar_.root
#hadd Probability_Tau_mu_TTbar_.root `ls Probability_Tau_mu_TTbar*00.root `
#cp Probability_Tau_mu_TTbar_.root ../TauHad2
#
#cd ..
#
##root -l -b -q Scale_ByLumi.C

echo " hadd done. Stacking... " 

cd TauHad2/Stack/

./Stack "10000"

cd ../../

