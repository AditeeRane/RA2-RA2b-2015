#! /bin/tcsh

#
# for prediction
#-----------------

cd TauHad2Multiple

foreach i ( 100_200 200_400 400_600 600_800 800_1200 1200_2500 2500_Inf ) 

  rm HadTauEstimation_WJet_${i}_.root
  hadd HadTauEstimation_WJet_${i}_.root  `ls HadTauEstimation_WJet_${i}_*00.root `  
  cp HadTauEstimation_WJet_${i}_.root ../TauHad2

  rm Probability_Tau_mu_WJet_${i}_.root
  hadd Probability_Tau_mu_WJet_${i}_.root `ls Probability_Tau_mu_WJet_${i}*00.root `
  cp Probability_Tau_mu_WJet_${i}_.root ../TauHad2

  rm TauBtaggedRate_WJet_${i}_.root
  hadd TauBtaggedRate_WJet_${i}_.root `ls TauBtaggedRate_WJet_${i}*00.root `
  cp TauBtaggedRate_WJet_${i}_.root ../TauHad2


end


foreach i ( t_top tW_top t_antitop tW_antitop s_channel )

  rm HadTauEstimation_${i}_.root
  hadd HadTauEstimation_${i}_.root  `ls HadTauEstimation_${i}_*00.root `
  cp HadTauEstimation_${i}_.root ../TauHad2

  rm Probability_Tau_mu_${i}_.root
  hadd Probability_Tau_mu_${i}_.root `ls Probability_Tau_mu_${i}*00.root `
  cp Probability_Tau_mu_${i}_.root ../TauHad2

  rm TauBtaggedRate_${i}_.root
  hadd TauBtaggedRate_${i}_.root `ls TauBtaggedRate_${i}*00.root `
  cp TauBtaggedRate_${i}_.root ../TauHad2

end

foreach i ( HT_1200_2500 HT_600_800 HT_800_1200 HT_2500_Inf DiLept Inclusive T_SingleLep Tbar_SingleLep )
    rm HadTauEstimation_TTbar_${i}_.root
	hadd HadTauEstimation_TTbar_${i}_.root  `ls HadTauEstimation_TTbar_${i}_*00.root `
    cp HadTauEstimation_TTbar_${i}_.root ../TauHad2/
    
    rm Probability_Tau_mu_TTbar_${i}_.root
	hadd Probability_Tau_mu_TTbar_${i}_.root `ls Probability_Tau_mu_TTbar_${i}*00.root `
    cp Probability_Tau_mu_TTbar_${i}_.root ../TauHad2
    
    rm TauBtaggedRate_TTbar_${i}_.root
	hadd TauBtaggedRate_TTbar_${i}_.root `ls TauBtaggedRate_TTbar_${i}*00.root `
    cp TauBtaggedRate_TTbar_${i}_.root ../TauHad2
    
end
cd ..

#root -l -b -q Scale_ByLumi.C

echo " hadd done. Stacking... " 

cd TauHad2/Stack/

./Stack "10000"

cd ../../

#
# for expectation
#-----------------

cd TauHadMultiple

foreach i ( 100_200 200_400 400_600 600_800 800_1200 1200_2500 2500_Inf )

  rm GenInfo_HadTauEstimation_WJet_${i}_.root
  hadd GenInfo_HadTauEstimation_WJet_${i}_.root  `ls GenInfo_HadTauEstimation_WJet_${i}_*00.root `
  cp GenInfo_HadTauEstimation_WJet_${i}_.root ../TauHad

  rm IsoEfficiencies_WJet_${i}_.root
  hadd IsoEfficiencies_WJet_${i}_.root `ls IsoEfficiencies_WJet_${i}_*_00.root`
  cp IsoEfficiencies_WJet_${i}_.root ../TauHad

  rm LostLepton2_MuonEfficienciesFromWJet_${i}_.root
  hadd LostLepton2_MuonEfficienciesFromWJet_${i}_.root `ls LostLepton2_MuonEfficienciesFromWJet_${i}_*00.root`
  cp LostLepton2_MuonEfficienciesFromWJet_${i}_.root ../TauHad

  rm HadTau_TauResponseTemplates_WJet_${i}_.root
  hadd HadTau_TauResponseTemplates_WJet_${i}_.root `ls HadTau_TauResponseTemplates_WJet_${i}_*00.root`
  cp HadTau_TauResponseTemplates_WJet_${i}_.root ../TauHad

  rm TauBtaggedRate_WJet_${i}_.root
  hadd TauBtaggedRate_WJet_${i}_.root `ls TauBtaggedRate_WJet_${i}_*00.root`
  cp TauBtaggedRate_WJet_${i}_.root ../TauHad

end

foreach i ( HT_1200_2500 HT_600_800 HT_800_1200 HT_2500_Inf DiLept Inclusive T_SingleLep Tbar_SingleLep )

rm GenInfo_HadTauEstimation_TTbar_${i}_.root
hadd GenInfo_HadTauEstimation_TTbar_${i}_.root  `ls GenInfo_HadTauEstimation_TTbar_${i}_*00.root `
cp GenInfo_HadTauEstimation_TTbar_${i}_.root ../TauHad

rm IsoEfficiencies_TTbar_${i}_.root
hadd IsoEfficiencies_TTbar_${i}_.root `ls IsoEfficiencies_TTbar_${i}_*00.root`
cp IsoEfficiencies_TTbar_${i}_.root ../TauHad

rm LostLepton2_MuonEfficienciesFromTTbar_${i}_.root
hadd LostLepton2_MuonEfficienciesFromTTbar_${i}_.root `ls LostLepton2_MuonEfficienciesFromTTbar_${i}_*00.root`
cp LostLepton2_MuonEfficienciesFromTTbar_${i}_.root ../TauHad

rm HadTau_TauResponseTemplates_TTbar_${i}_.root
hadd HadTau_TauResponseTemplates_TTbar_${i}_.root `ls HadTau_TauResponseTemplates_TTbar_${i}_*00.root`
cp HadTau_TauResponseTemplates_TTbar_${i}_.root ../TauHad

rm TauBtaggedRate_TTbar_${i}_.root
hadd TauBtaggedRate_TTbar_${i}_.root `ls TauBtaggedRate_TTbar_${i}_*00.root`
cp TauBtaggedRate_TTbar_${i}_.root ../TauHad

end
foreach i ( t_top tW_top t_antitop tW_antitop s_channel )

  rm GenInfo_HadTauEstimation_${i}_.root
  hadd GenInfo_HadTauEstimation_${i}_.root `ls GenInfo_HadTauEstimation_${i}_*00.root `
  cp GenInfo_HadTauEstimation_${i}_.root ../TauHad

  rm IsoEfficiencies_${i}_.root
  hadd IsoEfficiencies_${i}_.root `ls IsoEfficiencies_${i}_*_00.root`
  cp IsoEfficiencies_${i}_.root ../TauHad

  rm LostLepton2_MuonEfficienciesFrom${i}_.root
  hadd LostLepton2_MuonEfficienciesFrom${i}_.root `ls LostLepton2_MuonEfficienciesFrom${i}_*00.root`
  cp LostLepton2_MuonEfficienciesFrom${i}_.root ../TauHad

  rm HadTau_TauResponseTemplates_${i}_.root
  hadd HadTau_TauResponseTemplates_${i}_.root `ls HadTau_TauResponseTemplates_${i}_*00.root`
  cp HadTau_TauResponseTemplates_${i}_.root ../TauHad

  rm TauBtaggedRate_${i}_.root
  hadd TauBtaggedRate_${i}_.root `ls TauBtaggedRate_${i}_*00.root`
  cp TauBtaggedRate_${i}_.root ../TauHad

end


cd ..

cd TauHad/Stack/

./Stack "10000"

cd ../..

echo " Making the plots \n\n\n" 
./makeplots.sh 1 1 1 1 

