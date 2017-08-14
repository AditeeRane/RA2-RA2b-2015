/*
  root.exe -b -q HadTauEstimation_output_format.C 
*/

void binMap(TH1* input, TH1* output);
void binMap_ICHEP2016(TH1* input, TH1* output);
void binMap_QCD(TH1* input, TH1* output);
void binMap_QCD_ICHEP2016(TH1* input, TH1* output);
void printMaxMin(TH1* sys);
void takeAverage(TH1* sys);
void takeDiffForSys(TH1* sys, TH1* input_nominal);
void accErrPropagation(TH1* sys, TH1* input_nominal);
void effMapStatErrPropagation(TH1* input, TH1* output);
void effMapConstErrPropagation(TH1* input, double fractionalError, TH1* output);
void muFromTauStatErrPropagation(TH1* input, TH1* one, TH1* output);
void isoTrkVetoErrPropagation(TH1* input, TH1* PionIsoTrksys, TH1* one, TH1* output_tot, TH1* output_stat, TH1* output_sys);
void combineHighLowHT_searchBin(TH1* Default, TH1* lowHT);
void combineHighLowHT_QCDBin(TH1* Default, TH1* lowHT);
void makeNjNbCorr_searchBin(TH1* hist);
void makeNjNbCorr_QCDBin_HiDphi(TH1* hist);
void makeNjNbCorr_QCDBin_LowDphi(TH1* hist);
void reformat(TH1* input, TH1* output);
void reLabel(TH1* output);
//These correction factors are derived when low Dphi efficiencies were not corrected for low statistics.
 
//double NjNbCorr[16]={
//  1.08678, 1.1606, 1.21688, 1.38799,
//  0.99422, 1.03512, 1.09677, 1.15551,
//  0.960125, 0.99264, 1.04157, 1.22838,
//  0.863833, 0.803388, 1.01109, 1.12302};
//

//double NjNbCorr[16]={
//  1.06726,1.13884,1.19437,1.36361,
//  0.982114,1.02468,1.08705,1.14535,
//  0.954231,0.986859,1.03692,1.221,
//  0.858298,0.794967,1.00711,1.11734};
//
//double NjNbCorr[16]={1.10302,1.05589,1.11467,1.15494,1.02888,1.00077,1.04321,0.921019,1.00569,0.993849,1.01237,1.02326,0.906839,0.815019,0.990902,0.988837};
//double NjNbCorr[16]={1.09554,1.07391,1.12317,1.25163,1.0102,1.00845,1.05714,1.02093,0.996454,1.01738,0.990167,1.04041,0.833852,0.818807,0.96625,0.95135};
double NjNbCorr[19]=
		  {1.17399,1.20094,1.10832,1.02584,1.03485,1.0356,1.21258,0.98518,1.00936,1.00807,1.04635,0.959842,0.970069,0.945353,1.01596,0.90192,0.925152,0.970212,0.919978};
//double NjNbCorr_QCDHDP[16]={
//  1.06067,1.15766,1.21137,1.38428,
//  0.988007,1.02257,1.05086,1.17749,
//  0.937987,0.97341,0.99937,1.16912,
//  0.842024,0.854788,0.97895,1.12172};
//
//double NjNbCorr_QCDHDP[16]={
//  1.04212,1.13864,1.19202,1.36303,
//  0.980526,1.0161,1.04462,1.17064,
//  0.93641,0.972226,0.998193,1.16753,
//  0.843044,0.856616,0.981188,1.12397};
//
//double NjNbCorr_QCDHDP[16]={1.07954,1.05912,1.11673,1.13352,1.02719,1.00215,1.00608,0.932549,0.986,0.983652,0.977785,0.979731,0.891571,0.879714,0.96842,0.992207};
//double NjNbCorr_QCDHDP[16]={1.0727,1.07053,1.12063,1.17505,1.0059,1.00026,1.02861,0.998668,0.977091,0.989521,0.966759,1.01327,0.891883,0.843374,0.962022,1.00473};                
double NjNbCorr_QCDHDP[19]={1.11067,1.16299,0.998984,1.00782,1.035,1.04784,1.12393,0.976999,0.980355,0.995285,1.02251,0.943574,0.950614,0.95249,0.996299,0.963399,0.937785,0.956104,0.96402};

//double NjNbCorr_QCDLDP[16]={
//  0.864283,1.04062,1.10166,1.31812,
//  0.843319,0.915936,0.940394,1.13485,
//  0.850232,0.866817,0.924276,1.04468,
//  0.788246,0.804593,0.883932,0.949569};           
//
//double NjNbCorr_QCDLDP[16]={
//  0.915218,1.10424,1.17326,1.4113,
//  0.886127,0.961631,0.987567,1.19345,
//  0.885528,0.903907,0.965426,1.09268,
//  0.816972,0.834622,0.917226,0.984751};           
//
//double NjNbCorr_QCDLDP[16]={0.969584,0.960883,1.02801,0.958625,0.938399,0.939326,0.944426,0.913042,0.937141,0.91543,0.945902,0.915756,0.863937,0.856496,0.910216,0.872439};
//double NjNbCorr_QCDLDP[16]={0.953996,0.968867,1.05964,1.03167,0.915907,0.947684,0.950087,0.988761,0.918411,0.903336,0.968701,0.958839,0.914316,0.877071,0.883752,0.891303};
double NjNbCorr_QCDLDP[19]={1.00357,1.00596,1.01692,0.993508,0.982486,1.02538,1.09642,0.957791,0.936144,0.961578,1.00259,0.915095,0.916172,0.92677,1.01113,0.904738,0.880713,0.911209,0.928022};

void HadTauEstimation_output_format(//string elogForData="KHElog425_",       // Data
				    //string elogForData="ARElog40_4fb_",
				    //string elogForData="ARElog41_2.6fb_",
				    //string elogForData="ARElog46_7.6ifb_",
				    //string elogForData="ARElog49_7.6ifb_",
				    //string elogForData="ARElog94_36.35ifb_",
				    string elogForData="ARElog116_35.9ifb_",
				    //string elogForData="ARElog42_4fb_",
				    string elogForData2="KHElog424_",      // Data 
				    //string elogForMCExp="KHElog420_",      // MC expectation
				    //string elogForMCPre="KHElog424_",      // MC prediction
				    //string elogForMCExp="ARElog41_",
				    //string elogForMCPre="ARElog41_",
				    //
				    string elogForMCExp="ARElog115_",
				    string elogForMCPre="ARElog116_",
				    string elogForSys="ARElog116_",          // MC-based systematics evaluation for Btag mistag uncertainties and muon efficiency stat uncertainties
				    string elogForMuSys="ARElog116_",        // MC-based systematics evaluation for muon ID&Iso systematic efficiencies
				    //string elogForJECSysUp="Elog426_",     // JEC Up systematics
				    //string elogForJECSysDn="Elog426_",     // JEC Down systematics
				    //string elogForJECSysRef="Elog426_",    // JEC uncertainty reference


				    string elogForJECSysUp="ARElog116_",     // JEC Up systematics
				    string elogForJECSysDn="ARElog116_",     // JEC Down systematics
				    string elogForJECSysRef="ARElog116_",    // JEC uncertainty reference
				    
				    string elogForAccPDF="ARElog115_",      // Acceptance uncertainty due to PDF
				    string elogForAccScale="ARElog115_",    // Acceptance uncertainty due to scale
				    //
				    string elogForIsoTrkVeto="ARElog115_",   // Isotrack veto efficiency stat uncertainty
				    string elogForMuFromTau="ARElog116_",    // Muon from tau stat uncertainty
				    string elogForAccStat="ARElog115_",      // Acceptance stat uncertainty
				    string elogForMTStat="ARElog116_",       // MT cut efficiency
				    //
				    double trigEff=1.000,                  // Trigger efficiency for highHT selection (now corrected in the tauhad2_templace.cpp code)
				    double trigEff2=1.000,                 //                    for lowHT  selection
				    double trigEffErr=0.02,                // Trigger efficiency fractional uncertainties
				    double dilep=0.02,                     // Dilepton contamination subtraction uncertainty (fractional)
				    double MtSysFlat=0.01,                 // MT cut efficiency uncertainty
				    double IsoTrkVetoFlat=0.1,             // IsoTrkVeto efficiency uncertainty
                                    // because 2015C does not have our trigger, we scale 2015D to account for that
				    //double lumiTarget=2.584653,            // Luminosity of the search trigger sample 
				    //double lumiControl=2.585297,           // Luminosity of the SingleMuon PD used for the control sample
				    // lumi for V8
				    double lumiTarget=35.9,				                               double lumiControl=35.9,
				    int isys=0){

  char tempname[200];

  //
  // ----- Opening input files
  // -------------------------------

  //
  // Open data files
  //
  //  sprintf(tempname,"TauHad2/%sHadTauEstimation_data_SingleMuon_v17a_20160624v1_hadd.root",elogForData.c_str());
  //    sprintf(tempname,"TauHad2/%sHadTauEstimation_data_SingleMuon_v16b_.root",elogForData.c_str());
  //sprintf(tempname,"TauHad2/%sHadTauEstimation_data_SingleMuon_v17a_.root",elogForData.c_str());
  sprintf(tempname,"TauHad2/%sHadTauEstimation_data_SingleMuon_V12_.root",elogForData.c_str());
  TFile *DataEstFile = TFile::Open(tempname,"R");
  printf("Opened %s\n",tempname);

  sprintf(tempname,"Inputs/NJets_uncertainty.root");
  TFile *PionIsoTrkSysFile =TFile::Open(tempname,"R");
  TH1D* PionIsoTrksys=(TH1D*)PionIsoTrkSysFile->Get("pion_trkveto_syst");
  /*
  sprintf(tempname,"TauHad2/%sHadTauEstimation_data_SingleMuon_v17a_20160623v4_hadd.root",elogForData2.c_str());
  TFile *DataEstFile_lowHT = TFile::Open(tempname,"R");
  printf("Opened %s\n",tempname);
  */

  //
  // Convenient utility histogram
  // *AR,Feb24,2017-searchBin_one a flat histogram with bincontent 1 and error 0
  TH1D* searchBin_one = (TH1D*)DataEstFile->Get("searchH_b")->Clone("searchBin_one");  
  searchBin_one->Reset();
  searchBin_one->SetLineColor(1);
  for (int ibin=0; ibin<searchBin_one->GetNbinsX(); ibin++){
    searchBin_one->SetBinContent(ibin+1,1.);
    searchBin_one->SetBinError(ibin+1,0.);
  }
  // *AR,Feb24,2017-searchBin_box is original searchH_b histogram 
  TH1D* searchBin_box = (TH1D*)DataEstFile->Get("searchH_b")->Clone("searchBin_box");
  searchBin_box->Reset();
  searchBin_box->SetLineColor(1);
  // *AR,Feb24,2017-QCDBin_one a flat histogram with bincontent 1 (why error is not set to 0)
  TH1D* QCDBin_one = (TH1D*)DataEstFile->Get("QCD_Up")->Clone("QCDBin_one");
  QCDBin_one->Reset();
  QCDBin_one->SetLineColor(1);
  for (int ibin=0; ibin<QCDBin_one->GetNbinsX(); ibin++){
    QCDBin_one->SetBinContent(ibin+1,1.);
    //QCDBin_one->SetBinError(ibin+1,0.);
  }
  // *AR,Feb24,2017-QCDBin_box is original QCD_Up histogram
  //*AR,Mar14,2017-Where is original QCD_Low histogram?
  TH1D* QCDBin_box = (TH1D*)DataEstFile->Get("QCD_Up")->Clone("QCDBin_box");
  QCDBin_box->Reset();
  QCDBin_box->SetLineColor(1);

  // reformat
  //*AR,Feb24,2017-"reformat" makes histogram bin boundaries and bin labels of searchBin_one and QCDBin_one consistent
  //*AR,Mar14,2017-Make sure "reformat" function is modified according to existing bins in search bin and QCD histogram
  TH1D* searchBin_one_input = static_cast<TH1D*>(searchBin_one->Clone("searchBin_one_input"));
  reformat(searchBin_one_input,searchBin_one);
  TH1D* searchBin_box_input = static_cast<TH1D*>(searchBin_box->Clone("searchBin_box_input"));
  reformat(searchBin_box_input,searchBin_box);

  TH1D* QCDBin_one_input = static_cast<TH1D*>(QCDBin_one->Clone("QCDBin_one_input"));
  reformat(QCDBin_one_input,QCDBin_one);
  TH1D* QCDBin_box_input = static_cast<TH1D*>(QCDBin_box->Clone("QCDBin_box_input"));
  reformat(QCDBin_box_input,QCDBin_box);
  
  //
  // Open MC expectation and prediction code for closure systematics
  // 
  string sample="stacked";

  if(sample.find("stack")==string::npos)sprintf(tempname,"TauHad/%sGenInfo_HadTauEstimation_%s.root",elogForMCExp.c_str(),sample.c_str());
  else sprintf(tempname,"TauHad/Stack/%sGenInfo_HadTauEstimation_%s.root",elogForMCExp.c_str(),sample.c_str());
  TFile * MCGenFile = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);

  if(sample.find("stack")==string::npos)sprintf(tempname,"TauHad2/%sHadTauEstimation_%s.root",elogForMCPre.c_str(),sample.c_str());
  else sprintf(tempname,"TauHad2/Stack/%sHadTauEstimation_%s.root",elogForMCPre.c_str(),sample.c_str());
  TFile * MCEstFile = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);

  //
  // Open all MC files including systematics.
  //*AR, Mar14,2017-These are MC based bmistag, muon Id, muon Iso systematics
  //sprintf(tempname,"TauHad2/HadTauEstimation_%sContainigStatandSysErrors.root",elogForSys.c_str());
  sprintf(tempname,"TauHad2/%sHadTauEstimation_hadd_withSystematics.root",elogForSys.c_str());
  TFile * MCSysFile = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);

  //sprintf(tempname,"TauHad2/HadTauEstimation_MuonIDIsoSys_AllSamples_%sV2_.root",elogForMuSys.c_str());
  sprintf(tempname,"TauHad2/%sHadTauEstimation_hadd_withSystematics.root",elogForSys.c_str());
  TFile * MuSysFile = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);
  
  //
  // JEC variation
  //
  //if(sample.find("stack")==string::npos)sprintf(tempname,"TauHad2/%stemplatePlus_HadTauEstimation_%s.root",elogForJECSysUp.c_str(),sample.c_str());
  //else sprintf(tempname,"TauHad2/Stack/%stemplatePlus_HadTauEstimation_%s.root",elogForJECSysUp.c_str(),sample.c_str());
  //sprintf(tempname,"TauHad2/%stemplatePlus_HadTauEstimation_.root",elogForJECSysUp.c_str());
  sprintf(tempname,"TauHad2/%sHadTauEstimation_hadd_JECSysUp.root",elogForJECSysUp.c_str());
  TFile * JECSysUpFile = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);

  //if(sample.find("stack")==string::npos)sprintf(tempname,"TauHad2/%stemplateMinus_HadTauEstimation_%s.root",elogForJECSysDn.c_str(),sample.c_str());
  //else sprintf(tempname,"TauHad2/Stack/%stemplateMinus_HadTauEstimation_%s.root",elogForJECSysDn.c_str(),sample.c_str());
  //sprintf(tempname,"TauHad2/%stemplateMinus_HadTauEstimation_.root",elogForJECSysDn.c_str());
  sprintf(tempname,"TauHad2/%sHadTauEstimation_hadd_JECSysDn.root",elogForJECSysDn.c_str());
  TFile * JECSysDnFile = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);

  //if(sample.find("stack")==string::npos)sprintf(tempname,"TauHad2/%sHadTauEstimation_%s.root",elogForJECSysRef.c_str(),sample.c_str());
  //else sprintf(tempname,"TauHad2/Stack/%sHadTauEstimation_%s.root",elogForJECSysRef.c_str(),sample.c_str());
  sprintf(tempname,"TauHad2/%sHadTauEstimation_hadd_JECSysRef.root",elogForJECSysRef.c_str());
  TFile * JECSysRefFile = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);

  // Isotrack veto efficiency
  //
  sprintf(tempname,"Inputs/%sIsoEfficiencies_stacked.root",elogForIsoTrkVeto.c_str());
  TFile * IsoTrkVetoFile = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);
  
  TH1D* IsoTrkVetoEff = (TH1D*)IsoTrkVetoFile->Get("IsoEff")->Clone("IsoTrkVetoEff");
  TH1D* IsoTrkVetoEff_LowDphi = (TH1D*)IsoTrkVetoFile->Get("IsoEff_lowDphi")->Clone("IsoTrkVetoEff_LowDphi");

  //*AR,Feb27,2016-84 bin efficiency histograms are mapped to 174/223 bin histogram and bin content and bin error of output maps are set using average value in order to apply correction to searchbin/QCD histograms.
  //*AR,Mar 14,2017- Make sure binMap_ICHEP2016 and binMap_QCD_ICHEP2016 are modified according to existing search bin and QCD histograms.
  TH1D* searchBin_IsoTrkVetoEff = (TH1D*)searchBin_box->Clone("searchBin_IsoTrkVetoEff"); //searchBin_box--->searchbin histogram of data file
  searchBin_IsoTrkVetoEff->Reset();
  binMap_ICHEP2016(IsoTrkVetoEff,searchBin_IsoTrkVetoEff);

  TH1D* QCDBin_HiDphi_IsoTrkVetoEff = (TH1D*)QCDBin_box->Clone("QCDBin_HiDphi_IsoTrkVetoEff");
  QCDBin_HiDphi_IsoTrkVetoEff->Reset();
  binMap_QCD_ICHEP2016(IsoTrkVetoEff,QCDBin_HiDphi_IsoTrkVetoEff);

  TH1D* QCDBin_LowDphi_IsoTrkVetoEff = (TH1D*)QCDBin_box->Clone("QCDBin_LowDphi_IsoTrkVetoEff");
  QCDBin_LowDphi_IsoTrkVetoEff->Reset();
  binMap_QCD_ICHEP2016(IsoTrkVetoEff_LowDphi,QCDBin_LowDphi_IsoTrkVetoEff);

  //*AR,Feb 27,2016-These are 174 bin histograms
  //*AR, Mar14,2017- Separate histograms for statistical, systematic and total uncertainty.
  TH1D* searchBin_IsoTrkVetoEffUncertaintyTot  = (TH1D*)searchBin_IsoTrkVetoEff->Clone("searchBin_IsoTrkVetoEffUncertaintyTot");
  TH1D* searchBin_IsoTrkVetoEffUncertaintyStat = (TH1D*)searchBin_IsoTrkVetoEff->Clone("searchBin_IsoTrkVetoEffUncertaintyStat");
  TH1D* searchBin_IsoTrkVetoEffUncertaintySys  = (TH1D*)searchBin_IsoTrkVetoEff->Clone("searchBin_IsoTrkVetoEffUncertaintySys");

  //*AR,Feb 27,2016-These are 223 bin histograms
  TH1D* QCDBin_HiDphi_IsoTrkVetoEffUncertaintyTot  = (TH1D*)QCDBin_HiDphi_IsoTrkVetoEff->Clone("QCDBin_HiDphi_IsoTrkVetoEffUncertaintyTot");
  TH1D* QCDBin_HiDphi_IsoTrkVetoEffUncertaintyStat = (TH1D*)QCDBin_HiDphi_IsoTrkVetoEff->Clone("QCDBin_HiDphi_IsoTrkVetoEffUncertaintyStat");
  TH1D* QCDBin_HiDphi_IsoTrkVetoEffUncertaintySys  = (TH1D*)QCDBin_HiDphi_IsoTrkVetoEff->Clone("QCDBin_HiDphi_IsoTrkVetoEffUncertaintySys");

  TH1D* QCDBin_LowDphi_IsoTrkVetoEffUncertaintyTot  = (TH1D*)QCDBin_LowDphi_IsoTrkVetoEff->Clone("QCDBin_LowDphi_IsoTrkVetoEffUncertaintyTot");
  TH1D* QCDBin_LowDphi_IsoTrkVetoEffUncertaintyStat = (TH1D*)QCDBin_LowDphi_IsoTrkVetoEff->Clone("QCDBin_LowDphi_IsoTrkVetoEffUncertaintyStat");
  TH1D* QCDBin_LowDphi_IsoTrkVetoEffUncertaintySys  = (TH1D*)QCDBin_LowDphi_IsoTrkVetoEff->Clone("QCDBin_LowDphi_IsoTrkVetoEffUncertaintySys");

  //
  // --- Final propagation of isotrack veto efficiency uncertainty
  //*AR,Feb27,2017-IsoTrkVetoFlat=10%
  //For isotrack veto we consider both stat and syst uncertainties
  //output_sys:(1-eff)*0.1/eff
  //output_stat:sigma(eff)/eff, here sigma(eff) is a bin error
  //output_tot:sqrt(output_sys^2 + output_stat^2)
  //output_sys,output_stat,output_tot have only bin contents and not bin errors.
  isoTrkVetoErrPropagation(searchBin_IsoTrkVetoEff, PionIsoTrksys, searchBin_one,
			   searchBin_IsoTrkVetoEffUncertaintyTot, searchBin_IsoTrkVetoEffUncertaintyStat, searchBin_IsoTrkVetoEffUncertaintySys);
  isoTrkVetoErrPropagation(QCDBin_HiDphi_IsoTrkVetoEff, PionIsoTrksys, QCDBin_one,
			   QCDBin_HiDphi_IsoTrkVetoEffUncertaintyTot, QCDBin_HiDphi_IsoTrkVetoEffUncertaintyStat, QCDBin_HiDphi_IsoTrkVetoEffUncertaintySys);
  isoTrkVetoErrPropagation(QCDBin_LowDphi_IsoTrkVetoEff, PionIsoTrksys, QCDBin_one,
			   QCDBin_LowDphi_IsoTrkVetoEffUncertaintyTot, QCDBin_LowDphi_IsoTrkVetoEffUncertaintyStat, QCDBin_LowDphi_IsoTrkVetoEffUncertaintySys);


  //
  // Mu from tau
  // -----------
  sprintf(tempname,"Inputs/%sProbability_Tau_mu_stacked.root",elogForMuFromTau.c_str());
  TFile * Prob_Tau_mu_file = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);
  sprintf(tempname,"hProb_Tau_mu");
  TH1D * hProb_Tau_mu =(TH1D *) Prob_Tau_mu_file->Get(tempname)->Clone();
  sprintf(tempname,"hProb_Tau_mu_lowDelphi");
  TH1D * hProb_Tau_mu_LowDphi =(TH1D *) Prob_Tau_mu_file->Get(tempname)->Clone();
  //*AR, Mar 14,2017- Tau to Mu probability mapped to 174 bins
  TH1D* searchBin_MuFromTau = (TH1D*)searchBin_box->Clone("searchBin_MuFromTau");
  searchBin_MuFromTau->Reset();
  binMap_ICHEP2016(hProb_Tau_mu,searchBin_MuFromTau);
  //*AR, Mar 14,2017- Tau to Mu probability mapped to 223 bins
  TH1D* QCDBin_HiDphi_MuFromTau = (TH1D*)QCDBin_box->Clone("QCDBin_HiDphi_MuFromTau");
  QCDBin_HiDphi_MuFromTau->Reset();
  binMap_QCD_ICHEP2016(hProb_Tau_mu,QCDBin_HiDphi_MuFromTau);

  TH1D* QCDBin_LowDphi_MuFromTau = (TH1D*)QCDBin_box->Clone("QCDBin_LowDphi_MuFromTau");
  QCDBin_LowDphi_MuFromTau->Reset();
  binMap_QCD_ICHEP2016(hProb_Tau_mu_LowDphi,QCDBin_LowDphi_MuFromTau);

  TH1D* searchBin_MuFromTauStat  = (TH1D*)searchBin_MuFromTau->Clone("searchBin_MuFromTauStat");
  TH1D* QCDBin_HiDphi_MuFromTauStat  = (TH1D*)QCDBin_HiDphi_MuFromTau->Clone("QCDBin_HiDphi_MuFromTauStat");
  TH1D* QCDBin_LowDphi_MuFromTauStat = (TH1D*)QCDBin_LowDphi_MuFromTau->Clone("QCDBin_LowDphi_MuFromTauStat");

  //*AR, Feb 27,2017-For Mu from Tau we consider only statistical uncertainty, why so?
  //output_stat:sigma(eff)/(1-eff),where as for isoTrkVeto it is sigma(eff)/eff, why so?
  // 
  // --- Propagation of stat uncertainty on the subtraction of muons from taus
  muFromTauStatErrPropagation(searchBin_MuFromTau,searchBin_one,searchBin_MuFromTauStat);
  muFromTauStatErrPropagation(QCDBin_HiDphi_MuFromTau, QCDBin_one,QCDBin_HiDphi_MuFromTauStat);
  muFromTauStatErrPropagation(QCDBin_LowDphi_MuFromTau,QCDBin_one,QCDBin_LowDphi_MuFromTauStat);

  //
  // Acceptance
  // ----------
  //*AR, Feb 27,2017-For nominal acceptance also we consider only statistical uncertainty, why so?
  //*AR,Mar 14,2017-Nominal acceptance efficiency is mapped to 174/ 223 bins
  sprintf(tempname,"Inputs/%sLostLepton2_MuonEfficienciesFromstacked.root",elogForAccStat.c_str());
  TFile * MuAcc_file = new TFile(tempname,"R");  
  printf("Opened %s\n",tempname);
  sprintf(tempname,"hAcc");
  TH1D * hAcc =(TH1D *) MuAcc_file->Get(tempname)->Clone();
  sprintf(tempname,"hAcc_lowDphi");
  TH1D * hAcc_LowDphi =(TH1D *) MuAcc_file->Get(tempname)->Clone();

  TH1D* searchBin_Acc = (TH1D*)searchBin_box->Clone("searchBin_Acc");
  searchBin_Acc->Reset();
  binMap_ICHEP2016(hAcc,searchBin_Acc);

  TH1D* QCDBin_HiDphi_Acc = (TH1D*)QCDBin_box->Clone("QCDBin_HiDphi_Acc");
  QCDBin_HiDphi_Acc->Reset();
  binMap_QCD_ICHEP2016(hAcc,QCDBin_HiDphi_Acc);

  TH1D* QCDBin_LowDphi_Acc = (TH1D*)QCDBin_box->Clone("QCDBin_LowDphi_Acc");
  QCDBin_LowDphi_Acc->Reset();
  binMap_QCD_ICHEP2016(hAcc_LowDphi,QCDBin_LowDphi_Acc);

  //
  TH1D* searchBin_AccStat  = (TH1D*)searchBin_Acc->Clone("searchBin_AccStat");
  TH1D* QCDBin_HiDphi_AccStat  = (TH1D*)QCDBin_HiDphi_Acc->Clone("QCDBin_HiDphi_AccStat");
  TH1D* QCDBin_LowDphi_AccStat = (TH1D*)QCDBin_LowDphi_Acc->Clone("QCDBin_LowDphi_AccStat");

  //* AR, Mar14,2017-output_stat:sigma(eff)/(eff)
  //
  effMapStatErrPropagation(searchBin_Acc,searchBin_AccStat);
  effMapStatErrPropagation(QCDBin_HiDphi_Acc, QCDBin_HiDphi_AccStat);
  effMapStatErrPropagation(QCDBin_LowDphi_Acc,QCDBin_LowDphi_AccStat);

  // Due to PDF
  //----------
  //*AR,Feb27,2017-Gets hAccSysMax,hAccSysMin hists from acceptance systematics. Mapped from 84 bins to 174/223 bins.
  //hAccSysMax(84)--->hAccSysPDFUp(84)--->searchBin_AccSysPDFUp(174)
  //hAccSysMin(84)--->hAccSysPDFDn(84)--->searchBin_AccSysPDFDn(174)
  sprintf(tempname,"TauHad/%sAcceptanceSystematicsFromPDF_AllSamples.root",elogForAccPDF.c_str());
  TFile * AccSysFromPDFFile = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);
  TH1D * hAccSysPDFUp = (TH1D *) AccSysFromPDFFile->Get("hAccSysMax")->Clone();
  TH1D * hAccSysPDFDn = (TH1D *) AccSysFromPDFFile->Get("hAccSysMin")->Clone();
  TH1D * hAccSysPDFUp_LowDphi = (TH1D *) AccSysFromPDFFile->Get("hAccSysMax_lowDphi")->Clone();
  TH1D * hAccSysPDFDn_LowDphi = (TH1D *) AccSysFromPDFFile->Get("hAccSysMin_lowDphi")->Clone();

  TH1D* searchBin_AccSysPDFUp = (TH1D*)DataEstFile->Get("searchH_b")->Clone("searchBin_AccSysPDFUp");
  searchBin_AccSysPDFUp->Reset();
  binMap_ICHEP2016(hAccSysPDFUp,searchBin_AccSysPDFUp);
  //*AR, Feb27,2017-Here first histogram is 174 bin histogram after mapping PDF uncertainties.Second histogram is 174 bin histogram after mapping nominal acceptance efficiencies. In output histogram(searchBin_AccSysPDFUp or searchBin_AccSysPDFDn) bin content of first histogram is changed to (nominal)/(nominal+PDF) uncertainty using GetBinContent(). Why? 
  accErrPropagation(searchBin_AccSysPDFUp,searchBin_Acc);
  TH1D* searchBin_AccSysPDFDn = (TH1D*)DataEstFile->Get("searchH_b")->Clone("searchBin_AccSysPDFDn");
  searchBin_AccSysPDFDn->Reset();
  binMap_ICHEP2016(hAccSysPDFDn,searchBin_AccSysPDFDn);
  accErrPropagation(searchBin_AccSysPDFDn,searchBin_Acc);

  TH1D* QCDBin_HiDphi_AccSysPDFUp = (TH1D*)DataEstFile->Get("QCD_Up")->Clone("QCDBin_HiDphi_AccSysPDFUp");
  QCDBin_HiDphi_AccSysPDFUp->Reset();
  binMap_QCD_ICHEP2016(hAccSysPDFUp,QCDBin_HiDphi_AccSysPDFUp);
  accErrPropagation(QCDBin_HiDphi_AccSysPDFUp,QCDBin_HiDphi_Acc);
  TH1D* QCDBin_HiDphi_AccSysPDFDn = (TH1D*)DataEstFile->Get("QCD_Up")->Clone("QCDBin_HiDphi_AccSysPDFDn");
  QCDBin_HiDphi_AccSysPDFDn->Reset();
  binMap_QCD_ICHEP2016(hAccSysPDFDn,QCDBin_HiDphi_AccSysPDFDn);
  accErrPropagation(QCDBin_HiDphi_AccSysPDFDn,QCDBin_HiDphi_Acc);

  TH1D* QCDBin_LowDphi_AccSysPDFUp = (TH1D*)DataEstFile->Get("QCD_Low")->Clone("QCDBin_LowDphi_AccSysPDFUp");
  QCDBin_LowDphi_AccSysPDFUp->Reset();
  binMap_QCD_ICHEP2016(hAccSysPDFUp_LowDphi,QCDBin_LowDphi_AccSysPDFUp);
  accErrPropagation(QCDBin_LowDphi_AccSysPDFUp,QCDBin_LowDphi_Acc);
  TH1D* QCDBin_LowDphi_AccSysPDFDn = (TH1D*)DataEstFile->Get("QCD_Low")->Clone("QCDBin_LowDphi_AccSysPDFDn");
  QCDBin_LowDphi_AccSysPDFDn->Reset();
  binMap_QCD_ICHEP2016(hAccSysPDFDn_LowDphi,QCDBin_LowDphi_AccSysPDFDn);
  accErrPropagation(QCDBin_LowDphi_AccSysPDFDn,QCDBin_LowDphi_Acc);

  // Due to scale
  //----------
  sprintf(tempname,"TauHad/%sAcceptanceSystematicsFromScale_AllSamples.root",elogForAccScale.c_str());
  TFile * AccSysFromScaleFile = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);
  TH1D * hAccSysScaleUp = (TH1D *) AccSysFromScaleFile->Get("hScaleAccSysMax")->Clone();
  TH1D * hAccSysScaleDn = (TH1D *) AccSysFromScaleFile->Get("hScaleAccSysMin")->Clone();
  TH1D * hAccSysScaleUp_LowDphi = (TH1D *) AccSysFromScaleFile->Get("hScaleAccSysMax_lowDphi")->Clone();
  TH1D * hAccSysScaleDn_LowDphi = (TH1D *) AccSysFromScaleFile->Get("hScaleAccSysMin_lowDphi")->Clone();

  TH1D* searchBin_AccSysScaleUp = (TH1D*)searchBin_box->Clone("searchBin_AccSysScaleUp");
  searchBin_AccSysScaleUp->Reset();
  binMap_ICHEP2016(hAccSysScaleUp,searchBin_AccSysScaleUp);
  accErrPropagation(searchBin_AccSysScaleUp,searchBin_Acc);
  TH1D* searchBin_AccSysScaleDn = (TH1D*)searchBin_box->Clone("searchBin_AccSysScaleDn");
  searchBin_AccSysScaleDn->Reset();
  binMap_ICHEP2016(hAccSysScaleDn,searchBin_AccSysScaleDn);
  accErrPropagation(searchBin_AccSysScaleDn,searchBin_Acc);

  TH1D* QCDBin_HiDphi_AccSysScaleUp = (TH1D*)QCDBin_box->Clone("QCDBin_HiDphi_AccSysScaleUp");
  QCDBin_HiDphi_AccSysScaleUp->Reset();
  binMap_QCD_ICHEP2016(hAccSysScaleUp,QCDBin_HiDphi_AccSysScaleUp);
  accErrPropagation(QCDBin_HiDphi_AccSysScaleUp,QCDBin_HiDphi_Acc);
  TH1D* QCDBin_HiDphi_AccSysScaleDn = (TH1D*)QCDBin_box->Clone("QCDBin_HiDphi_AccSysScaleDn");
  QCDBin_HiDphi_AccSysScaleDn->Reset();
  binMap_QCD_ICHEP2016(hAccSysScaleDn,QCDBin_HiDphi_AccSysScaleDn);
  accErrPropagation(QCDBin_HiDphi_AccSysScaleDn,QCDBin_HiDphi_Acc);

  TH1D* QCDBin_LowDphi_AccSysScaleUp = (TH1D*)QCDBin_box->Clone("QCDBin_LowDphi_AccSysScaleUp");
  QCDBin_LowDphi_AccSysScaleUp->Reset();
  binMap_QCD_ICHEP2016(hAccSysScaleUp_LowDphi,QCDBin_LowDphi_AccSysScaleUp);
  accErrPropagation(QCDBin_LowDphi_AccSysScaleUp,QCDBin_LowDphi_Acc);
  TH1D* QCDBin_LowDphi_AccSysScaleDn = (TH1D*)QCDBin_box->Clone("QCDBin_LowDphi_AccSysScaleDn");
  QCDBin_LowDphi_AccSysScaleDn->Reset();
  binMap_QCD_ICHEP2016(hAccSysScaleDn_LowDphi,QCDBin_LowDphi_AccSysScaleDn);
  accErrPropagation(QCDBin_LowDphi_AccSysScaleDn,QCDBin_LowDphi_Acc);

  //
  // MT cut efficiency
  // -----------------
  //*AR,Mar14,2017- creates 174/223 bin mT efficiency histograms
  sprintf(tempname,"Inputs/%sMtEff.root",elogForMTStat.c_str());
  TFile * MtFile = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);
  TH1D * hMT = (TH1D *) MtFile->Get("MtCutEff")->Clone();
  TH1D * hMT_LowDphi = (TH1D *) MtFile->Get("MtCutEff_lowDphi")->Clone();

  string histname;  

  TH1D* searchBin_MtEff = (TH1D*)searchBin_box->Clone("searchBin_MtEff");
  searchBin_MtEff->Reset();
  binMap_ICHEP2016(hMT,searchBin_MtEff);

  TH1D* QCDBin_HiDphi_MtEff = (TH1D*)QCDBin_box->Clone("QCDBin_HiDphi_MtEff");
  QCDBin_HiDphi_MtEff->Reset();
  binMap_QCD_ICHEP2016(hMT,QCDBin_HiDphi_MtEff);

  TH1D* QCDBin_LowDphi_MtEff = (TH1D*)QCDBin_box->Clone("QCDBin_LowDphi_MtEff");
  QCDBin_LowDphi_MtEff->Reset();
  binMap_QCD_ICHEP2016(hMT_LowDphi,QCDBin_LowDphi_MtEff);

  //
  TH1D* searchBin_MtEffStat  = (TH1D*)searchBin_MtEff->Clone("searchBin_MtEffStat");
  TH1D* QCDBin_HiDphi_MtEffStat  = (TH1D*)QCDBin_HiDphi_MtEff->Clone("QCDBin_HiDphi_MtEffStat");
  TH1D* QCDBin_LowDphi_MtEffStat = (TH1D*)QCDBin_LowDphi_MtEff->Clone("QCDBin_LowDphi_MtEffStat");
  // *AR, Feb 27,2017--- Propagation of stat uncertainties for mT efficiency
  //output_stat:sigma(eff)/(eff)

  //*AR, Mar14,2017-Same as used to set statistical uncertainty on Nominal acceptance
  effMapStatErrPropagation(searchBin_MtEff,searchBin_MtEffStat);
  effMapStatErrPropagation(QCDBin_HiDphi_MtEff, QCDBin_HiDphi_MtEffStat);
  effMapStatErrPropagation(QCDBin_LowDphi_MtEff,QCDBin_LowDphi_MtEffStat);

  //*AR, Mar14,2017- Define 174/223 bin histogram to get mT systematic.
  TH1D * searchBin_MTSysUp  = (TH1D*) searchBin_one->Clone("searchBin_MTSysUp");
  TH1D * searchBin_MTSysDn  = (TH1D*) searchBin_one->Clone("searchBin_MTSysDn");
  TH1D * searchBin_MTSysRef = (TH1D*) searchBin_one->Clone("searchBin_MTSysRef");

  TH1D * QCDBin_HiDphi_MTSysUp  = (TH1D*)QCDBin_one->Clone("QCDBin_HiDphi_MTSysUp");   // Template for MTSysUp
  TH1D * QCDBin_HiDphi_MTSysDn  = (TH1D*)QCDBin_one->Clone("QCDBin_HiDphi_MTSysDn");   // Template for MTSysDn
  TH1D * QCDBin_HiDphi_MTSysRef = (TH1D*)QCDBin_one->Clone("QCDBin_HiDphi_MTSysRef");  

  TH1D * QCDBin_LowDphi_MTSysUp  = (TH1D*)QCDBin_one->Clone("QCDBin_LowDphi_MTSysUp");  // Template for MTSysUp
  TH1D * QCDBin_LowDphi_MTSysDn  = (TH1D*)QCDBin_one->Clone("QCDBin_LowDphi_MTSysDn");  // Template for MTSysDn
  TH1D * QCDBin_LowDphi_MTSysRef = (TH1D*)QCDBin_one->Clone("QCDBin_LowDphi_MTSysRef");

  //*AR, Feb27,2017-effMapConstErrPropagation is different from what we did for isotrk veto(output_sys:(1-eff)*IsotrkvetoFlat/eff)
  //*AR, Mar14,2017-Here first histogram is unity histogram.So output histograms are: searchBin_MTSysUp--> 1/(1-MtSysFlat),searchBin_MTSysDn--->1/(1+MtSysFlat)
  //* AR, Mar14,2017-MtSysFlat is set 1%, from where this value came?
  effMapConstErrPropagation(searchBin_MTSysRef,-MtSysFlat,searchBin_MTSysUp); // MT cut efficiency reduces -> prediction goes up (i.e. plus)  
  effMapConstErrPropagation(searchBin_MTSysRef,+MtSysFlat,searchBin_MTSysDn);
  effMapConstErrPropagation(QCDBin_HiDphi_MTSysRef,-MtSysFlat,QCDBin_HiDphi_MTSysUp);
  effMapConstErrPropagation(QCDBin_HiDphi_MTSysRef,+MtSysFlat,QCDBin_HiDphi_MTSysDn);
  effMapConstErrPropagation(QCDBin_LowDphi_MTSysRef,-MtSysFlat,QCDBin_LowDphi_MTSysUp);
  effMapConstErrPropagation(QCDBin_LowDphi_MTSysRef,+MtSysFlat,QCDBin_LowDphi_MTSysDn);

  //
  // Const uncertainty
  //
  // dileptonic subtraction
  //*AR, Feb27,2017-creates flat histogram of value 2% and error zero, from where this value came? 
  TH1D *searchBin_DileptonUncertainty = (TH1D*) searchBin_one->Clone("searchBin_DileptonUncertainty");
  searchBin_DileptonUncertainty->Scale(dilep);

  TH1D *QCDBin_HiDphi_DileptonUncertainty = (TH1D*) QCDBin_one->Clone("QCDBin_HiDphi_DileptonUncertainty");
  QCDBin_HiDphi_DileptonUncertainty->Scale(dilep);

  TH1D *QCDBin_LowDphi_DileptonUncertainty = (TH1D*) QCDBin_one->Clone("QCDBin_LowDphi_DileptonUncertainty");
  QCDBin_LowDphi_DileptonUncertainty->Scale(dilep);

  //
  // Const uncertainty
  //
  // trigger efficiency
  //*AR, Feb27,2017-creates flat histogram of value 2% and error zero,from where this value came? 
  TH1D *searchBin_TrigEffUncertainty = (TH1D*) searchBin_one->Clone("searchBin_TrigEffUncertainty");
  searchBin_TrigEffUncertainty->Scale(trigEffErr/trigEff);

  TH1D *QCDBin_HiDphi_TrigEffUncertainty = (TH1D*) QCDBin_one->Clone("QCDBin_HiDphi_TrigEffUncertainty");
  QCDBin_HiDphi_TrigEffUncertainty->Scale(trigEffErr/trigEff);

  TH1D *QCDBin_LowDphi_TrigEffUncertainty = (TH1D*) QCDBin_one->Clone("QCDBin_LowDphi_TrigEffUncertainty");
  QCDBin_LowDphi_TrigEffUncertainty->Scale(trigEffErr/trigEff);

  //
  // ---- Nominal predictions and stat uncertainties -----
  // -----------------------------------------------------
  //  std::cout<<"*************************"<<endl;
  //
  // ----- Normal search bin predicitons -----
  //
  //*AR,Feb27,2017-Gets searchH_b histogram from data estimation file
  TH1D* searchBin_nominal = (TH1D*)DataEstFile->Get("searchH_b")->Clone("searchBin_nominal");  
  TH1D* searchBin_nominal_input = static_cast<TH1D*>(searchBin_nominal->Clone("searchBin_nominal_input"));
  reformat(searchBin_nominal_input,searchBin_nominal);
  //searchBin_nominal->Print();
  //*AR, Feb27, 2017-Corrects data prediction for trigEff but is set to 1, as we correct for trigger efficiency in prediction code itself.
  searchBin_nominal->Scale(1/trigEff*lumiTarget/lumiControl); 
  //searchBin_nominal->Print();
  TH1D* searchBin_nominal_fullstatuncertainty = (TH1D*)searchBin_nominal->Clone("searchBin_nominal_fullstatuncertainty");
  //*AR, Feb 27,2017-From where this 0.460255 uncertainty came? Why is it called full statistical uncertainty?
  //*AR, Mar 14,2017-fullstatuncertainty=sqrt[sigma(Data_hist)^2+0.460255^2].Here we set the bin error and not bin content. So final histogram is same as data prediction histogram but with total error(stat and syst in quadrature)
  for (int ibin=0; ibin<searchBin_nominal->GetNbinsX(); ibin++){
    searchBin_nominal_fullstatuncertainty->SetBinError(ibin+1,pow(pow(searchBin_nominal->GetBinError(ibin+1),2)+pow(0.460255,2),0.5));
  }

  /* KH: no longer used
  TH1D* searchBin_nominal_lowHT = (TH1D*)DataEstFile_lowHT->Get("searchH_b")->Clone("searchBin_nominal_lowHT");
  //searchBin_nominal->Print();
  searchBin_nominal_lowHT->Scale(1/trigEff2*lumiTarget/lumiControl); 
  //searchBin_nominal->Print();
  TH1D* searchBin_nominal_fullstatuncertainty_lowHT = (TH1D*)searchBin_nominal_lowHT->Clone("searchBin_nominal_fullstatuncertainty_lowHT");
  for (int ibin=0; ibin<searchBin_nominal_lowHT->GetNbinsX(); ibin++){
    searchBin_nominal_fullstatuncertainty_lowHT->SetBinError(ibin+1,pow(pow(searchBin_nominal_lowHT->GetBinError(ibin+1),2)+pow(0.460255,2),0.5));
  }
  */

  /* KH: no longer used
  combineHighLowHT_searchBin(searchBin_nominal, searchBin_nominal_lowHT);
  makeNjNbCorr_searchBin(searchBin_nominal);

  combineHighLowHT_searchBin(searchBin_nominal_fullstatuncertainty, searchBin_nominal_fullstatuncertainty_lowHT);
  makeNjNbCorr_searchBin(searchBin_nominal_fullstatuncertainty);
  */

  //
  // ----- QCD bin predicitons -----
  //
  TH1D* QCDBin_HiDphi_nominal  = (TH1D*)DataEstFile->Get("QCD_Up")->Clone("QCDBin_HiDphi_nominal");
  TH1D* QCDBin_HiDphi_nominal_input = static_cast<TH1D*>(QCDBin_HiDphi_nominal->Clone("QCDBin_HiDphi_nominal_input"));
  reformat(QCDBin_HiDphi_nominal_input,QCDBin_HiDphi_nominal);
  QCDBin_HiDphi_nominal->Scale(1/trigEff*lumiTarget/lumiControl);
  TH1D* QCDBin_HiDphi_nominal_fullstatuncertainty = (TH1D*)QCDBin_HiDphi_nominal->Clone("QCDBin_HiDphi_nominal_fullstatuncertainty");
  for (int ibin=0; ibin<QCDBin_HiDphi_nominal->GetNbinsX(); ibin++){
    QCDBin_HiDphi_nominal_fullstatuncertainty->SetBinError(ibin+1,pow(pow(QCDBin_HiDphi_nominal->GetBinError(ibin+1),2)+pow(0.460255,2),0.5));
  }

  TH1D* QCDBin_LowDphi_nominal = (TH1D*)DataEstFile->Get("QCD_Low")->Clone("QCDBin_LowDphi_nominal");
  TH1D* QCDBin_LowDphi_nominal_input = static_cast<TH1D*>(QCDBin_LowDphi_nominal->Clone("QCDBin_LowDphi_nominal_input"));
  reformat(QCDBin_LowDphi_nominal_input,QCDBin_LowDphi_nominal);
  QCDBin_LowDphi_nominal->Scale(1/trigEff*lumiTarget/lumiControl);
  TH1D* QCDBin_LowDphi_nominal_fullstatuncertainty = (TH1D*)QCDBin_LowDphi_nominal->Clone("QCDBin_LowDphi_nominal_fullstatuncertainty");
  for (int ibin=0; ibin<QCDBin_LowDphi_nominal->GetNbinsX(); ibin++){
    QCDBin_LowDphi_nominal_fullstatuncertainty->SetBinError(ibin+1,pow(pow(QCDBin_LowDphi_nominal->GetBinError(ibin+1),2)+pow(0.460255,2),0.5));
  }

  //---

  /* KH: no longer used
  TH1D* QCDBin_HiDphi_nominal_lowHT = (TH1D*)DataEstFile->Get("QCD_Up")->Clone("QCDBin_HiDphi_nominal_lowHT");
  QCDBin_HiDphi_nominal_lowHT->Scale(1/trigEff2*lumiTarget/lumiControl);
  TH1D* QCDBin_HiDphi_nominal_fullstatuncertainty_lowHT = (TH1D*)QCDBin_HiDphi_nominal_lowHT->Clone("QCDBin_HiDphi_nominal_fullstatuncertainty_lowHT");
  for (int ibin=0; ibin<QCDBin_HiDphi_nominal_lowHT->GetNbinsX(); ibin++){
    QCDBin_HiDphi_nominal_fullstatuncertainty_lowHT->SetBinError(ibin+1,pow(pow(QCDBin_HiDphi_nominal_lowHT->GetBinError(ibin+1),2)+pow(0.460255,2),0.5));
  }

  TH1D* QCDBin_LowDphi_nominal_lowHT = (TH1D*)DataEstFile->Get("QCD_Low")->Clone("QCDBin_LowDphi_nominal_lowHT");
  QCDBin_LowDphi_nominal_lowHT->Scale(1/trigEff2*lumiTarget/lumiControl);
  TH1D* QCDBin_LowDphi_nominal_fullstatuncertainty_lowHT = (TH1D*)QCDBin_LowDphi_nominal->Clone("QCDBin_LowDphi_nominal_fullstatuncertainty_lowHT");
  for (int ibin=0; ibin<QCDBin_LowDphi_nominal_lowHT->GetNbinsX(); ibin++){
    QCDBin_LowDphi_nominal_fullstatuncertainty_lowHT->SetBinError(ibin+1,pow(pow(QCDBin_LowDphi_nominal_lowHT->GetBinError(ibin+1),2)+pow(0.460255,2),0.5));
  }
  */

  /* KH: no longer used
  combineHighLowHT_QCDBin(QCDBin_HiDphi_nominal, QCDBin_HiDphi_nominal_lowHT);
  combineHighLowHT_QCDBin(QCDBin_LowDphi_nominal,QCDBin_LowDphi_nominal_lowHT);
  combineHighLowHT_QCDBin(QCDBin_HiDphi_nominal_fullstatuncertainty, QCDBin_HiDphi_nominal_fullstatuncertainty_lowHT);
  combineHighLowHT_QCDBin(QCDBin_LowDphi_nominal_fullstatuncertainty, QCDBin_LowDphi_nominal_fullstatuncertainty_lowHT);
  */

  // Some additional variables
  const int NSearchBinArray=175;
  const int NQCDBinArray=224;
  double searchBin_stat_uncertainty_fractional[NSearchBinArray];
  double searchBin_uncertainty_correction_stats_up[NSearchBinArray];
  double searchBin_uncertainty_correction_stats_dn[NSearchBinArray];
  double searchBin_uncertainty_correction_stats[NSearchBinArray];

  /* KH: no longer used
  double searchBin_stat_uncertainty_fractional_lowHT[NSearchBinArray];
  double searchBin_uncertainty_correction_stats_up_lowHT[NSearchBinArray];
  double searchBin_uncertainty_correction_stats_dn_lowHT[NSearchBinArray];
  double searchBin_uncertainty_correction_stats_lowHT[NSearchBinArray];
  */

  double QCDBin_HiDphi_stat_uncertainty_fractional[NQCDBinArray];
  double QCDBin_HiDphi_uncertainty_correction_stats_up[NQCDBinArray];
  double QCDBin_HiDphi_uncertainty_correction_stats_dn[NQCDBinArray];
  double QCDBin_HiDphi_uncertainty_correction_stats[NQCDBinArray];

  double QCDBin_LowDphi_stat_uncertainty_fractional[NQCDBinArray];
  double QCDBin_LowDphi_uncertainty_correction_stats_up[NQCDBinArray];
  double QCDBin_LowDphi_uncertainty_correction_stats_dn[NQCDBinArray];
  double QCDBin_LowDphi_uncertainty_correction_stats[NQCDBinArray];
  //* AR, Mar 14,2017-Here searchBin_nominal is "searchH_b" histogram frm data estimation file.
  TH1D* searchBin_StatUncertainties = (TH1D*)searchBin_nominal->Clone("searchBin_StatUncertainties");
  searchBin_StatUncertainties->Reset();
  TH1D* searchBin_StatUncertaintiesFractional = (TH1D*)searchBin_nominal->Clone("searchBin_StatUncertaintiesFractional");
  searchBin_StatUncertaintiesFractional->Reset();

  /* KH: no longer used
  TH1D* searchBin_StatUncertainties_lowHT = (TH1D*)searchBin_nominal_lowHT->Clone("searchBin_StatUncertainties_lowHT");
  searchBin_StatUncertainties_lowHT->Reset();
  TH1D* searchBin_StatUncertaintiesFractional_lowHT = (TH1D*)searchBin_nominal_lowHT->Clone("searchBin_StatUncertaintiesFractional_lowHT");
  searchBin_StatUncertaintiesFractional_lowHT->Reset();
  */
  //* AR, Mar 14,2017-Here QCDBin_HiDphi_nominal and QCDBin_LowDphi_nominal are histograms frm data estimation file.

  TH1D* QCDBin_HiDphi_StatUncertainties = (TH1D*)QCDBin_HiDphi_nominal->Clone("QCDBin_HiDphi_StatUncertainties");
  QCDBin_HiDphi_StatUncertainties->Reset();
  TH1D* QCDBin_HiDphi_StatUncertaintiesFractional = (TH1D*)QCDBin_HiDphi_nominal->Clone("QCDBin_HiDphi_StatUncertaintiesFractional");
  QCDBin_HiDphi_StatUncertaintiesFractional->Reset();

  TH1D* QCDBin_LowDphi_StatUncertainties = (TH1D*)QCDBin_LowDphi_nominal->Clone("QCDBin_LowDphi_StatUncertainties");
  QCDBin_LowDphi_StatUncertainties->Reset();
  TH1D* QCDBin_LowDphi_StatUncertaintiesFractional = (TH1D*)QCDBin_LowDphi_nominal->Clone("QCDBin_LowDphi_StatUncertaintiesFractional");
  QCDBin_LowDphi_StatUncertaintiesFractional->Reset();

  //
  // ----- Stat uncertainties
  //

  //  printf("\nstat uncertainty check\n\n");
  
  for (int ibin=1;ibin<=searchBin_nominal->GetNbinsX();ibin++){
    // Stat uncertainty
    searchBin_stat_uncertainty_fractional[ibin]=0.;
    if (searchBin_nominal->GetBinContent(ibin)!=0.){
      /*
      printf("ibin=%3d: %8.4f\n",ibin,
	     searchBin_nominal->GetBinError(ibin)/searchBin_nominal->GetBinContent(ibin) 
	     );
      */
      //searchBin_StatUncertainties=1+(error/mean value)

      //* AR,Mar14,2017-searchBin_StatUncertainties filled with sigma(Data_searchH_b)
      //* AR,Mar14,2017-searchBin_StatUncertaintiesFractional filled with full_err(Data_searchH_b)/Data_searchH_b
      //* AR,Mar14,2017-Note following differences:QCDBin_HiDphi_StatUncertaintiesFractional filled with sigma(Data_QCD)/Data_QCD instead of using full_err(Data_QCD)

      searchBin_StatUncertainties->SetBinContent(ibin,searchBin_nominal->GetBinError(ibin));
      searchBin_stat_uncertainty_fractional[ibin] = searchBin_nominal_fullstatuncertainty->GetBinError(ibin)/searchBin_nominal->GetBinContent(ibin);
      searchBin_StatUncertaintiesFractional->SetBinContent(ibin,searchBin_stat_uncertainty_fractional[ibin]);
    } else {
      searchBin_StatUncertaintiesFractional->SetBinContent(ibin,2.);
    }
  }
  //*AR, Feb27,2017-searchBin_StatUncertainties is reset to 1+[sigma(Data_searchH_b)/Data_searchH_b)]
  //* AR,Mar14,2017-Note following differences:Above step is not done for QCDBin_HiDphi_StatUncertainties/QCDBin_LowDphi_StatUncertainties
  for (int ibin=1;ibin<=searchBin_StatUncertainties->GetNbinsX();ibin++){
    double FracErr=0;
    if (searchBin_nominal->GetBinContent(ibin)!=0.)
      FracErr=searchBin_StatUncertainties->GetBinContent(ibin)/searchBin_nominal->GetBinContent(ibin);
    searchBin_StatUncertainties->SetBinContent(ibin,1+FracErr);
  }
  /* KH: no longer used
  for (int ibin=1;ibin<=searchBin_nominal_lowHT->GetNbinsX();ibin++){
    // Stat uncertainty
    searchBin_stat_uncertainty_fractional_lowHT[ibin]=0.;
    if (searchBin_nominal_lowHT->GetBinContent(ibin)!=0.){
      printf("ibin=%3d: %8.4f\n",ibin,
	     searchBin_nominal_lowHT->GetBinError(ibin)/searchBin_nominal_lowHT->GetBinContent(ibin) 
	     );
      searchBin_StatUncertainties_lowHT->SetBinContent(ibin,searchBin_nominal_lowHT->GetBinError(ibin));
      searchBin_stat_uncertainty_fractional_lowHT[ibin] = searchBin_nominal_fullstatuncertainty_lowHT->GetBinError(ibin)/searchBin_nominal_lowHT->GetBinContent(ibin);
      searchBin_StatUncertaintiesFractional_lowHT->SetBinContent(ibin,searchBin_stat_uncertainty_fractional[ibin]);
    } else {
      searchBin_StatUncertaintiesFractional_lowHT->SetBinContent(ibin,2.);
    }
  }
  */

  for (int ibin=1;ibin<=QCDBin_HiDphi_nominal->GetNbinsX();ibin++){
    //
    // Stat uncertainty
    QCDBin_HiDphi_stat_uncertainty_fractional[ibin]=0.;
    if (QCDBin_HiDphi_nominal->GetBinContent(ibin)!=0.){
      QCDBin_HiDphi_StatUncertainties->SetBinContent(ibin,QCDBin_HiDphi_nominal->GetBinError(ibin));
      QCDBin_HiDphi_stat_uncertainty_fractional[ibin] = QCDBin_HiDphi_nominal->GetBinError(ibin)/QCDBin_HiDphi_nominal->GetBinContent(ibin);    
      QCDBin_HiDphi_StatUncertaintiesFractional->SetBinContent(ibin,QCDBin_HiDphi_stat_uncertainty_fractional[ibin]);
    } else {
      QCDBin_HiDphi_StatUncertaintiesFractional->SetBinContent(ibin,2.);
    }
    //
    // Stat uncertainty
    QCDBin_LowDphi_stat_uncertainty_fractional[ibin]=0.;
    if (QCDBin_LowDphi_nominal->GetBinContent(ibin)!=0.){
      QCDBin_LowDphi_StatUncertainties->SetBinContent(ibin,QCDBin_LowDphi_nominal->GetBinError(ibin));
      QCDBin_LowDphi_stat_uncertainty_fractional[ibin] = QCDBin_LowDphi_nominal->GetBinError(ibin)/QCDBin_LowDphi_nominal->GetBinContent(ibin);
      QCDBin_LowDphi_StatUncertaintiesFractional->SetBinContent(ibin,QCDBin_LowDphi_stat_uncertainty_fractional[ibin]);
    } else {
      QCDBin_LowDphi_StatUncertaintiesFractional->SetBinContent(ibin,2.);
    }
  }

  /* KHtmp
  
  //
  // Jack's inclusive binning
  // ------------------------
  TH1D* hPredHTMHT0b_nominal_ABbins = (TH1D*)DataEstFile->Get("hPredHTMHT0b")->Clone("hPredHTMHT0b_nominal_ABbins");
  TH1D* hPredHTMHTwb_nominal_ABbins = (TH1D*)DataEstFile->Get("hPredHTMHTwb")->Clone("hPredHTMHTwb_nominal_ABbins");
  TH1D* hPredNJetBins_nominal = (TH1D*)DataEstFile->Get("hPredNJetBins")->Clone("hPredNJetBins_nominal");
  TH1D* hPredNbBins_nominal = (TH1D*)DataEstFile->Get("hPredNbBins")->Clone("hPredNbBins_nominal");
  
  TH1D* hPredHTMHT0b_nominal = new TH1D("hPredHTMHT0b_nominal", ";HTMHT Box;Events / Bin", 6, 0.5, 6.5);
  TH1D* hPredHTMHTwb_nominal = new TH1D("hPredHTMHTwb_nominal", ";HTMHT Box;Events / Bin", 6, 0.5, 6.5);
  for (int ibin=0;ibin<6;ibin++){
    hPredHTMHT0b_nominal->SetBinContent(ibin+1,hPredHTMHT0b_nominal_ABbins->GetBinContent(ibin+1));
    hPredHTMHT0b_nominal->SetBinError(ibin+1,pow(pow(hPredHTMHT0b_nominal_ABbins->GetBinError(ibin+1),2)+pow(0.460255,2),0.5));
    hPredHTMHTwb_nominal->SetBinContent(ibin+1,hPredHTMHTwb_nominal_ABbins->GetBinContent(ibin+1));
    hPredHTMHTwb_nominal->SetBinError(ibin+1,pow(pow(hPredHTMHTwb_nominal_ABbins->GetBinError(ibin+1),2)+pow(0.460255,2),0.5));
  }
  for (int ibin=0; ibin<hPredNJetBins_nominal->GetNbinsX(); ibin++){
    hPredNJetBins_nominal->SetBinError(ibin+1,pow(pow(hPredNJetBins_nominal->GetBinError(ibin+1),2)+pow(0.460255,2),0.5));
  }
  for (int ibin=0; ibin<hPredNbBins_nominal->GetNbinsX(); ibin++){
    hPredNbBins_nominal->SetBinError(ibin+1,pow(pow(hPredNbBins_nominal->GetBinError(ibin+1),2)+pow(0.460255,2),0.5));
  }

  KHtmp */

  //
  // ---- Closure systematics -----
  // -----------------------------------------------------

  //
  // From MC file
  //
  TH1D * GenHist, * EstHist, * thist;
  TH1D * histTemplate;
  THStack * tempstack;
  TH1D * GenHist_input, * EstHist_input;
  // For closure systematics
  //histname="searchH_b";
  //* AR, Mar14,2017-Gets MC expectation and prediction histograms.
  sprintf(tempname,"%s","searchH_b");
  tempstack=(THStack*)MCEstFile->Get(tempname)->Clone();
  EstHist=(TH1D*) tempstack->GetStack()->Last();
  tempstack=(THStack*)MCGenFile->Get(tempname)->Clone();   
  GenHist=(TH1D*) tempstack->GetStack()->Last();
  //* AR, Mar14,2017-Correct MC expectation and prediction histograms for bin labels and bin boundaries.
  // reformat
  EstHist_input = static_cast<TH1D*>(EstHist->Clone("EstHist_input"));
  reformat(EstHist_input,EstHist);
  GenHist_input = static_cast<TH1D*>(GenHist->Clone("GenHist_input"));
  reformat(GenHist_input,GenHist);
  //*AR,Feb27,2017-Corrects MC prediction with NjNb correction. Both bin content and bin error are multiplied by a same correction factor.
  makeNjNbCorr_searchBin(EstHist);
  
  GenHist->SetFillColor(0);
  GenHist->SetLineColor(1);
  //*AR,Feb27,2017-searchBin_closureUncertainty_adhoc is 174 bin histogram which gives deviation of NjNb corr from 1 as an absolute value   
  TH1D * searchBin_closureUncertainty_adhoc = static_cast<TH1D*>(GenHist->Clone("searchBin_closureUncertainty_adhoc"));
  searchBin_closureUncertainty_adhoc->Reset();
  for (int ibin=1;ibin<=searchBin_closureUncertainty_adhoc->GetNbinsX();ibin++){
    int index=-1;
    if(ibin<=110)
      index=int((ibin-1)/10);
    else
      index=int((ibin-111)/8)+11;
    //std::cout << index << " " << ibin << std::endl;
    searchBin_closureUncertainty_adhoc->SetBinContent(ibin,fabs(NjNbCorr[index]-1.));
  }
  //searchBin_closureUncertainty_adhoc->Print("all");
  
  //
  // Preparing ratio histograms: Expectation/Prediction for closure uncertainty
  //
  TH1D * searchBin_closureUncertainty = static_cast<TH1D*>(GenHist->Clone("searchBin_closureUncertainty"));
  searchBin_closureUncertainty->Reset();
  //* AR, Mar 14,2017-closureRatio is ratio histogram Exp/Pre
  TH1D * closureRatio= static_cast<TH1D*>(GenHist->Clone("closureRatio"));
  TH1D * numerator   = static_cast<TH1D*>(GenHist->Clone("numerator"));

  TH1D * GenHist_Clone = static_cast<TH1D*>(GenHist->Clone("GenHist_Clone"));
  TH1D * EstHist_Clone = static_cast<TH1D*>(EstHist->Clone("EstHist_Clone"));
  closureRatio->Divide(GenHist_Clone,EstHist_Clone,1,1,"");  // Expectation/Prediction-1 - Non closure

  //EstHist_Clone->Print("all");

  //closureRatio->Print("all");
  
  double searchBin_closure_stat_uncertainty_fractional[NSearchBinArray];
  //-----
  int n10percent=0;
  int n30percent=0;
  double avenj1;
  double avenj1nb01;
  double avenj1nb23;
  double avenj2;
  double avenj3;
  //-----
  //*AR,Feb27,2017-For searchBin_closureUncertainty(174 bin) , the value set is the maximum of deviation of Exp/Pre from 1 and fractional error on closure[stat_err(closure)/(closure)]
  for (int ibin=1;ibin<=closureRatio->GetNbinsX();ibin++){
    // Stat uncertainty
    searchBin_closure_stat_uncertainty_fractional[ibin]=0.;
    double Unc,Unc1,Unc2;
    if (closureRatio->GetBinContent(ibin)!=0.){
      //*AR,Mar14,2017,gets fractional error: sigma(closureRatio)/closureRatio
      searchBin_closure_stat_uncertainty_fractional[ibin] = closureRatio->GetBinError(ibin)/closureRatio->GetBinContent(ibin);
      Unc1=searchBin_closure_stat_uncertainty_fractional[ibin];
    }
    Unc2=fabs(closureRatio->GetBinContent(ibin)-1.);
    Unc = TMath::Max(fabs(closureRatio->GetBinContent(ibin)-1.),searchBin_closure_stat_uncertainty_fractional[ibin]);
    //
    //
    //double Unc3=searchBin_closureUncertainty_adhoc->GetBinContent(ibin);
    //Unc = pow(Unc3*Unc3+Unc*Unc,0.5); 
    //
    //
    if (Unc>1.) Unc=1.;
    searchBin_closureUncertainty->SetBinContent(ibin,Unc);    
    //----
    //printf("%5d, %8.2f, %8.2f, %8.2f\n",ibin,Unc,Unc1,Unc2);
    if (Unc<0.1) n10percent++;
    if (Unc<0.3) n30percent++;
    if (ibin>=1 &&ibin<=24) avenj1+=Unc/24.;
    if (ibin>=1 &&ibin<=12) avenj1nb01+=Unc/12.;
    if (ibin>=13&&ibin<=24) avenj1nb23+=Unc/12.;
    if (ibin>=25&&ibin<=48) avenj2+=Unc/24.;
    if (ibin>=49&&ibin<=72) avenj3+=Unc/24.;
    //-----
  }
//  printf("10percent %5d\n",n10percent);
//  printf("30precent %5d\n",n30percent);
//  printf("nj1 average=%8.2f\n",avenj1);
//  printf("nj1nb01 average=%8.2f\n",avenj1nb01);
//  printf("nj1nb23 average=%8.2f\n",avenj1nb23);
//  printf("nj2 average=%8.2f\n",avenj2);
//  printf("nj3 average=%8.2f\n",avenj3);
//  //searchBin_closureUncertainty->Print("all");

  // For closure systematics
  //histname="QCD_Up";
  sprintf(tempname,"%s","QCD_Up");
  tempstack=(THStack*)MCEstFile->Get(tempname)->Clone();
  EstHist=(TH1D*) tempstack->GetStack()->Last();
  tempstack=(THStack*)MCGenFile->Get(tempname)->Clone();   
  GenHist=(TH1D*) tempstack->GetStack()->Last();
  
  // reformat
  EstHist_input = static_cast<TH1D*>(EstHist->Clone("EstHist_input"));
  reformat(EstHist_input,EstHist);
  GenHist_input = static_cast<TH1D*>(GenHist->Clone("GenHist_input"));
  reformat(GenHist_input,GenHist);

  makeNjNbCorr_QCDBin_HiDphi(EstHist);

  GenHist->SetFillColor(0);
  GenHist->SetLineColor(1);
  //*AR,Mar15,2017- QCDBin_HiDphi_closureUncertainty_adhoc is 223 bin histogram which gives deviation of NjNb corr from 1 as an absolute value    
  TH1D * QCDBin_HiDphi_closureUncertainty_adhoc = static_cast<TH1D*>(GenHist->Clone("QCDBin_HiDphi_closureUncertainty_adhoc"));
  QCDBin_HiDphi_closureUncertainty_adhoc->Reset();
  for (int ibin=1;ibin<=QCDBin_HiDphi_closureUncertainty_adhoc->GetNbinsX();ibin++){
    int index=-1;
    if(ibin<=143)
      index=int((ibin-1)/13);
    else
      index=int((ibin-144)/10)+11;
    //std::cout << index << " " << ibin << std::endl;
    QCDBin_HiDphi_closureUncertainty_adhoc->SetBinContent(ibin,fabs(NjNbCorr_QCDHDP[index]-1.));
  }
  //QCDBin_HiDphi_closureUncertainty_adhoc->Print("all");
  
  //
  // Preparing ratio histograms: Expectation/Prediction for closure uncertainty
  //
  TH1D * QCDBin_HiDphi_closureUncertainty = static_cast<TH1D*>(GenHist->Clone("QCDBin_HiDphi_closureUncertainty"));
  QCDBin_HiDphi_closureUncertainty->Reset();
  QCDBin_HiDphi_closureUncertainty->SetFillColor(0);
  QCDBin_HiDphi_closureUncertainty->SetLineColor(1);
  //*AR,Mar15,2017- QCDBin_HiDphi_closureRatio is 223 bin Exp/Pre histogram 
  TH1D * QCDBin_HiDphi_closureRatio= static_cast<TH1D*>(GenHist->Clone("QCDBin_HiDphi_closureRatio"));
  TH1D * QCDBin_HiDphi_numerator   = static_cast<TH1D*>(GenHist->Clone("QCDBin_HiDphi_numerator"));

  TH1D * QCDBin_HiDphi_GenHist_Clone = static_cast<TH1D*>(GenHist->Clone("QCDBin_HiDphi_GenHist_Clone"));
  TH1D * QCDBin_HiDphi_EstHist_Clone = static_cast<TH1D*>(EstHist->Clone("QCDBin_HiDphi_EstHist_Clone"));
  QCDBin_HiDphi_closureRatio->Divide(QCDBin_HiDphi_GenHist_Clone,QCDBin_HiDphi_EstHist_Clone,1,1,"");  
  // Expectation/Prediction-1 - Non closure

  double QCDBin_HiDphi_closure_stat_uncertainty_fractional[223];

  for (int ibin=1;ibin<=223;ibin++){
    // Stat uncertainty
    QCDBin_HiDphi_closure_stat_uncertainty_fractional[ibin]=0.;
    if (QCDBin_HiDphi_closureRatio->GetBinContent(ibin)!=0.){
      QCDBin_HiDphi_closure_stat_uncertainty_fractional[ibin] = QCDBin_HiDphi_closureRatio->GetBinError(ibin)/QCDBin_HiDphi_closureRatio->GetBinContent(ibin);
    }
    /*
    std::cout << ibin << " "
  	      << QCDBin_HiDphi_closureRatio->GetBinContent(ibin)-1. << " "
  	      << QCDBin_HiDphi_closure_stat_uncertainty_fractional[ibin] << std::endl;
    */
  //*AR,Mar15,2017-For QCDBin_HiDphi_closureUncertainty(223 bin) , the value set is the maximum of deviation of Exp/Pre from 1 and fractional error on closure[stat_err(closure)/(closure)]. Similar is the case of QCDBin_LowDphi_closureUncertainty(223 bin)
    double Unc;
    Unc = TMath::Max(fabs(QCDBin_HiDphi_closureRatio->GetBinContent(ibin)-1.),QCDBin_HiDphi_closure_stat_uncertainty_fractional[ibin]);
    if (Unc>1.) Unc=1.;
    QCDBin_HiDphi_closureUncertainty->SetBinContent(ibin,Unc);    
  }
  //QCDBin_HiDphi_closureUncertainty->Print("all");

  // For closure systematics
  //  histname="QCD_Low";
  sprintf(tempname,"QCD_Low");
  tempstack=(THStack*)MCEstFile->Get(tempname)->Clone();
  EstHist=(TH1D*) tempstack->GetStack()->Last();
  tempstack=(THStack*)MCGenFile->Get(tempname)->Clone();   
  GenHist=(TH1D*) tempstack->GetStack()->Last();
  //std::cout<<" histname_QCD_Low "<<endl;

  // reformat
  EstHist_input = static_cast<TH1D*>(EstHist->Clone("EstHist_input"));
  reformat(EstHist_input,EstHist);
  GenHist_input = static_cast<TH1D*>(GenHist->Clone("GenHist_input"));
  reformat(GenHist_input,GenHist);

  makeNjNbCorr_QCDBin_LowDphi(EstHist);

  GenHist->SetFillColor(0);
  GenHist->SetLineColor(1);

  //std::cout<<" QCDBin_LowDphi_closureUncertainty_adhoc "<<endl; 
    TH1D * QCDBin_LowDphi_closureUncertainty_adhoc = static_cast<TH1D*>(GenHist->Clone("QCDBin_LowDphi_closureUncertainty_adhoc"));
    QCDBin_LowDphi_closureUncertainty_adhoc->Reset();
    for (int ibin=1;ibin<=QCDBin_LowDphi_closureUncertainty_adhoc->GetNbinsX();ibin++){
      int index=-1;
      if(ibin<=143)
	index=int((ibin-1)/13);
      else
	index=int((ibin-144)/10)+11;
        
      //std::cout << index << " " << ibin << std::endl;
      QCDBin_LowDphi_closureUncertainty_adhoc->SetBinContent(ibin,fabs(NjNbCorr_QCDLDP[index]-1.));
    }
  //QCDBin_LowDphi_closureUncertainty_adhoc->Print("all");

  //
  // Preparing ratio histograms: Expectation/Prediction for closure uncertainty
  //
  //std::cout<<" QCDBin_LowDphi_closureUncertainty "<<endl;
  TH1D * QCDBin_LowDphi_closureUncertainty = static_cast<TH1D*>(GenHist->Clone("QCDBin_LowDphi_closureUncertainty"));
  QCDBin_LowDphi_closureUncertainty->Reset();
  QCDBin_LowDphi_closureUncertainty->SetFillColor(0);
  QCDBin_LowDphi_closureUncertainty->SetLineColor(1);

  TH1D * QCDBin_LowDphi_closureRatio= static_cast<TH1D*>(GenHist->Clone("QCDBin_LowDphi_closureRatio"));
  TH1D * QCDBin_LowDphi_numerator   = static_cast<TH1D*>(GenHist->Clone("QCDBin_LowDphi_numerator"));

  TH1D * QCDBin_LowDphi_GenHist_Clone = static_cast<TH1D*>(GenHist->Clone("QCDBin_LowDphi_GenHist_Clone"));
  TH1D * QCDBin_LowDphi_EstHist_Clone = static_cast<TH1D*>(EstHist->Clone("QCDBin_LowDphi_EstHist_Clone"));
  QCDBin_LowDphi_closureRatio->Divide(QCDBin_LowDphi_GenHist_Clone,QCDBin_LowDphi_EstHist_Clone,1,1,"");  
  // Expectation/Prediction-1 - Non closure
  //std::cout<<" QCDBin_LowDphi_closure_stat_uncertainty_fractional "<<endl;
  double QCDBin_LowDphi_closure_stat_uncertainty_fractional[223];
  for (int ibin=1;ibin<=223;ibin++){
    // Stat uncertainty
    //std::cout<<" Inside for loop "<<" ibin "<<ibin<< endl;
    QCDBin_LowDphi_closure_stat_uncertainty_fractional[ibin]=0.;
    if (QCDBin_LowDphi_closureRatio->GetBinContent(ibin)!=0.){
      QCDBin_LowDphi_closure_stat_uncertainty_fractional[ibin] = QCDBin_LowDphi_closureRatio->GetBinError(ibin)/QCDBin_LowDphi_closureRatio->GetBinContent(ibin);
    }
    /*
    std::cout << ibin << " "
  	      << QCDBin_LowDphi_closureRatio->GetBinContent(ibin)-1. << " "
  	      << QCDBin_LowDphi_closure_stat_uncertainty_fractional[ibin] << std::endl;
    */
    double Unc;
    Unc = TMath::Max(fabs(QCDBin_LowDphi_closureRatio->GetBinContent(ibin)-1.),QCDBin_LowDphi_closure_stat_uncertainty_fractional[ibin]);
    if (Unc>1.) Unc=1.;
    //std::cout<<" setbin content for QCDBin_LowDphi "<<endl;
    QCDBin_LowDphi_closureUncertainty->SetBinContent(ibin,Unc);    
  }
  //QCDBin_LowDphi_closureUncertainty->Print("all");

  //
  // ---- BMistag & MuRecoIso systematics -----
  // -----------------------------------------------------
  // 
  // From MC sys file
  //
  //  histname="searchH_b_";
  //string cutname="delphi";
  //* AR, Feb27,2017-Gets original histogram under "allEvents" and name them as "searchBin_default" and "searchBin_default2"histograms.
  sprintf(tempname,"allEvents/%s/%s","delphi","searchH_b_");
  TH1D * searchBin_default = (TH1D*)MCSysFile->Get(tempname)->Clone();
  TH1D * searchBin_default2 = (TH1D*)MuSysFile->Get(tempname)->Clone();
  //searchBin_default->Print("all");
  //searchBin_default2->Print("all");

  //* AR, Feb27,2017-Gets histograms under folders corresponding to systematics.
  sprintf(tempname,"BMistagPlus/%s/%s","delphi","searchH_b_");  
  TH1D* searchBin_BMistagUp = (TH1D*)MCSysFile->Get(tempname)->Clone("searchBin_BMistagUp");
  sprintf(tempname,"BMistagMinus/%s/%s","delphi","searchH_b_");
  TH1D* searchBin_BMistagDn = (TH1D*)MCSysFile->Get(tempname)->Clone("searchBin_BMistagDn");
  //std::cout<<" searchBin_BMistagDn "<<endl;
  sprintf(tempname,"RecoSysPlus/%s/%s","delphi","searchH_b_");  
  TH1D* searchBin_MuRecoSysUp = (TH1D*)MuSysFile->Get(tempname)->Clone("searchBin_MuRecoSysUp");
  sprintf(tempname,"RecoSysMinus/%s/%s","delphi","searchH_b_");
  TH1D* searchBin_MuRecoSysDn = (TH1D*)MuSysFile->Get(tempname)->Clone("searchBin_MuRecoSysDn");
  //std::cout<<" searchBin_MuRecoSysDn "<<endl;
  sprintf(tempname,"IsoSysPlus/%s/%s","delphi","searchH_b_");  
  TH1D* searchBin_MuIsoSysUp = (TH1D*)MuSysFile->Get(tempname)->Clone("searchBin_MuIsoSysUp");
  sprintf(tempname,"IsoSysMinus/%s/%s","delphi","searchH_b_");
  TH1D* searchBin_MuIsoSysDn = (TH1D*)MuSysFile->Get(tempname)->Clone("searchBin_MuIsoSysDn");

  //std::cout<<" searchBin_MuIsoSysDn "<<endl;
  //*AR, Mar15,2017-"reformat" default and histogram under systematic folder to make bin labels and bin boundaries consistent.Same is true for QCD_high and QCD_low histograms.

  sprintf(tempname,"MuRecoIsoPlus/%s/%s","delphi","searchH_b_");  
  TH1D* searchBin_MuRecoIsoUp = (TH1D*)MCSysFile->Get(tempname)->Clone("searchBin_MuRecoIsoUp");
  sprintf(tempname,"MuRecoIsoMinus/%s/%s","delphi","searchH_b_");
  TH1D* searchBin_MuRecoIsoDn = (TH1D*)MCSysFile->Get(tempname)->Clone("searchBin_MuRecoIsoDn");

  TH1D* searchBin_default_input = static_cast<TH1D*>(searchBin_default->Clone("searchBin_default_input")); reformat(searchBin_default_input,searchBin_default);
  TH1D* searchBin_default2_input = static_cast<TH1D*>(searchBin_default2->Clone("searchBin_default2_input")); reformat(searchBin_default2_input,searchBin_default2);

  TH1D* searchBin_BMistagUp_input = static_cast<TH1D*>(searchBin_BMistagUp->Clone("searchBin_BMistagUp_input")); reformat(searchBin_BMistagUp_input,searchBin_BMistagUp);
  TH1D* searchBin_BMistagDn_input = static_cast<TH1D*>(searchBin_BMistagDn->Clone("searchBin_BMistagDn_input")); reformat(searchBin_BMistagDn_input,searchBin_BMistagDn);

  TH1D* searchBin_MuRecoSysUp_input = static_cast<TH1D*>(searchBin_MuRecoSysUp->Clone("searchBin_MuRecoSysUp_input")); reformat(searchBin_MuRecoSysUp_input,searchBin_MuRecoSysUp);
  TH1D* searchBin_MuRecoSysDn_input = static_cast<TH1D*>(searchBin_MuRecoSysDn->Clone("searchBin_MuRecoSysDn_input")); reformat(searchBin_MuRecoSysDn_input,searchBin_MuRecoSysDn);

  TH1D* searchBin_MuIsoSysUp_input = static_cast<TH1D*>(searchBin_MuIsoSysUp->Clone("searchBin_MuIsoSysUp_input")); reformat(searchBin_MuIsoSysUp_input,searchBin_MuIsoSysUp);
  TH1D* searchBin_MuIsoSysDn_input = static_cast<TH1D*>(searchBin_MuIsoSysDn->Clone("searchBin_MuIsoSysDn_input")); reformat(searchBin_MuIsoSysDn_input,searchBin_MuIsoSysDn);

  TH1D* searchBin_MuRecoIsoUp_input = static_cast<TH1D*>(searchBin_MuRecoIsoUp->Clone("searchBin_MuRecoIsoUp_input")); reformat(searchBin_MuRecoIsoUp_input,searchBin_MuRecoIsoUp);
  TH1D* searchBin_MuRecoIsoDn_input = static_cast<TH1D*>(searchBin_MuRecoIsoDn->Clone("searchBin_MuRecoIsoDn_input")); reformat(searchBin_MuRecoIsoDn_input,searchBin_MuRecoIsoDn);
  
  //
  //  histname="QCD_";
  //string cutname="delphi";
  sprintf(tempname,"allEvents/%s/%s","delphi","QCD_");
  TH1D * QCDBin_HiDphi_default = (TH1D*)MCSysFile->Get(tempname)->Clone();

  sprintf(tempname,"BMistagPlus/%s/%s","delphi","QCD_");  
  TH1D* QCDBin_HiDphi_BMistagUp = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_HiDphi_BMistagUp");
  sprintf(tempname,"BMistagMinus/%s/%s","delphi","QCD_");
  TH1D* QCDBin_HiDphi_BMistagDn = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_HiDphi_BMistagDn");
  
  sprintf(tempname,"RecoSysPlus/%s/%s","delphi","QCD_");  
  TH1D* QCDBin_HiDphi_MuRecoSysUp = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_HiDphi_MuRecoSysUp");
  sprintf(tempname,"RecoSysMinus/%s/%s","delphi","QCD_");
  TH1D* QCDBin_HiDphi_MuRecoSysDn = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_HiDphi_MuRecoSysDn");
  //std::cout<< " QCDBin_HiDphi_MuRecoSysDn "<<endl;
  sprintf(tempname,"IsoSysPlus/%s/%s","delphi","QCD_");  
  TH1D* QCDBin_HiDphi_MuIsoSysUp = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_HiDphi_MuIsoSysUp");
  sprintf(tempname,"IsoSysMinus/%s/%s","delphi","QCD_");
  TH1D* QCDBin_HiDphi_MuIsoSysDn = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_HiDphi_MuIsoSysDn");

  sprintf(tempname,"MuRecoIsoPlus/%s/%s","delphi","QCD_");  
  TH1D* QCDBin_HiDphi_MuRecoIsoUp = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_HiDphi_MuRecoIsoUp");
  sprintf(tempname,"MuRecoIsoMinus/%s/%s","delphi","QCD_");
  TH1D* QCDBin_HiDphi_MuRecoIsoDn = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_HiDphi_MuRecoIsoDn");

  TH1D* QCDBin_HiDphi_default_input = static_cast<TH1D*>(QCDBin_HiDphi_default->Clone("QCDBin_HiDphi_default_input")); reformat(QCDBin_HiDphi_default_input,QCDBin_HiDphi_default);

  TH1D* QCDBin_HiDphi_BMistagUp_input = static_cast<TH1D*>(QCDBin_HiDphi_BMistagUp->Clone("QCDBin_HiDphi_BMistagUp_input")); reformat(QCDBin_HiDphi_BMistagUp_input,QCDBin_HiDphi_BMistagUp);
  TH1D* QCDBin_HiDphi_BMistagDn_input = static_cast<TH1D*>(QCDBin_HiDphi_BMistagDn->Clone("QCDBin_HiDphi_BMistagDn_input")); reformat(QCDBin_HiDphi_BMistagDn_input,QCDBin_HiDphi_BMistagDn);

  TH1D* QCDBin_HiDphi_MuRecoSysUp_input = static_cast<TH1D*>(QCDBin_HiDphi_MuRecoSysUp->Clone("QCDBin_HiDphi_MuRecoSysUp_input")); reformat(QCDBin_HiDphi_MuRecoSysUp_input,QCDBin_HiDphi_MuRecoSysUp);
  TH1D* QCDBin_HiDphi_MuRecoSysDn_input = static_cast<TH1D*>(QCDBin_HiDphi_MuRecoSysDn->Clone("QCDBin_HiDphi_MuRecoSysDn_input")); reformat(QCDBin_HiDphi_MuRecoSysDn_input,QCDBin_HiDphi_MuRecoSysDn);

  TH1D* QCDBin_HiDphi_MuIsoSysUp_input = static_cast<TH1D*>(QCDBin_HiDphi_MuIsoSysUp->Clone("QCDBin_HiDphi_MuIsoSysUp_input")); reformat(QCDBin_HiDphi_MuIsoSysUp_input,QCDBin_HiDphi_MuIsoSysUp);
  TH1D* QCDBin_HiDphi_MuIsoSysDn_input = static_cast<TH1D*>(QCDBin_HiDphi_MuIsoSysDn->Clone("QCDBin_HiDphi_MuIsoSysDn_input")); reformat(QCDBin_HiDphi_MuIsoSysDn_input,QCDBin_HiDphi_MuIsoSysDn);

  TH1D* QCDBin_HiDphi_MuRecoIsoUp_input = static_cast<TH1D*>(QCDBin_HiDphi_MuRecoIsoUp->Clone("QCDBin_HiDphi_MuRecoIsoUp_input")); reformat(QCDBin_HiDphi_MuRecoIsoUp_input,QCDBin_HiDphi_MuRecoIsoUp);
  TH1D* QCDBin_HiDphi_MuRecoIsoDn_input = static_cast<TH1D*>(QCDBin_HiDphi_MuRecoIsoDn->Clone("QCDBin_HiDphi_MuRecoIsoDn_input")); reformat(QCDBin_HiDphi_MuRecoIsoDn_input,QCDBin_HiDphi_MuRecoIsoDn);

  //
  
  histname="QCD_";
  string cutname="low_Dphi";
  sprintf(tempname,"allEvents/%s/%s","low_Dphi","QCD_");
  TH1D * QCDBin_LowDphi_default = (TH1D*)MCSysFile->Get(tempname)->Clone();

  sprintf(tempname,"BMistagPlus/%s/%s","low_Dphi","QCD_");  
  TH1D* QCDBin_LowDphi_BMistagUp = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_LowDphi_BMistagUp");
  sprintf(tempname,"BMistagMinus/%s/%s","low_Dphi","QCD_");
  TH1D* QCDBin_LowDphi_BMistagDn = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_LowDphi_BMistagDn");
  
  sprintf(tempname,"RecoSysPlus/%s/%s","low_Dphi","QCD_");  
  TH1D* QCDBin_LowDphi_MuRecoSysUp = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_LowDphi_MuRecoSysUp");
  sprintf(tempname,"RecoSysMinus/%s/%s","low_Dphi","QCD_");
  TH1D* QCDBin_LowDphi_MuRecoSysDn = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_LowDphi_MuRecoSysDn");

  sprintf(tempname,"IsoSysPlus/%s/%s","low_Dphi","QCD_");  
  TH1D* QCDBin_LowDphi_MuIsoSysUp = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_LowDphi_MuIsoSysUp");
  sprintf(tempname,"IsoSysMinus/%s/%s","low_Dphi","QCD_");
  TH1D* QCDBin_LowDphi_MuIsoSysDn = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_LowDphi_MuIsoSysDn");

  sprintf(tempname,"MuRecoIsoPlus/%s/%s","low_Dphi","QCD_");  
  TH1D* QCDBin_LowDphi_MuRecoIsoUp = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_LowDphi_MuRecoIsoUp");
  sprintf(tempname,"MuRecoIsoMinus/%s/%s","low_Dphi","QCD_");
  TH1D* QCDBin_LowDphi_MuRecoIsoDn = (TH1D*)MCSysFile->Get(tempname)->Clone("QCDBin_LowDphi_MuRecoIsoDn");

  TH1D* QCDBin_LowDphi_default_input = static_cast<TH1D*>(QCDBin_LowDphi_default->Clone("QCDBin_LowDphi_default_input")); reformat(QCDBin_LowDphi_default_input,QCDBin_LowDphi_default);

  TH1D* QCDBin_LowDphi_BMistagUp_input = static_cast<TH1D*>(QCDBin_LowDphi_BMistagUp->Clone("QCDBin_LowDphi_BMistagUp_input")); reformat(QCDBin_LowDphi_BMistagUp_input,QCDBin_LowDphi_BMistagUp);
  TH1D* QCDBin_LowDphi_BMistagDn_input = static_cast<TH1D*>(QCDBin_LowDphi_BMistagDn->Clone("QCDBin_LowDphi_BMistagDn_input")); reformat(QCDBin_LowDphi_BMistagDn_input,QCDBin_LowDphi_BMistagDn);

  TH1D* QCDBin_LowDphi_MuRecoSysUp_input = static_cast<TH1D*>(QCDBin_LowDphi_MuRecoSysUp->Clone("QCDBin_LowDphi_MuRecoSysUp_input")); reformat(QCDBin_LowDphi_MuRecoSysUp_input,QCDBin_LowDphi_MuRecoSysUp);
  TH1D* QCDBin_LowDphi_MuRecoSysDn_input = static_cast<TH1D*>(QCDBin_LowDphi_MuRecoSysDn->Clone("QCDBin_LowDphi_MuRecoSysDn_input")); reformat(QCDBin_LowDphi_MuRecoSysDn_input,QCDBin_LowDphi_MuRecoSysDn);

  TH1D* QCDBin_LowDphi_MuIsoSysUp_input = static_cast<TH1D*>(QCDBin_LowDphi_MuIsoSysUp->Clone("QCDBin_LowDphi_MuIsoSysUp_input")); reformat(QCDBin_LowDphi_MuIsoSysUp_input,QCDBin_LowDphi_MuIsoSysUp);
  TH1D* QCDBin_LowDphi_MuIsoSysDn_input = static_cast<TH1D*>(QCDBin_LowDphi_MuIsoSysDn->Clone("QCDBin_LowDphi_MuIsoSysDn_input")); reformat(QCDBin_LowDphi_MuIsoSysDn_input,QCDBin_LowDphi_MuIsoSysDn);

  TH1D* QCDBin_LowDphi_MuRecoIsoUp_input = static_cast<TH1D*>(QCDBin_LowDphi_MuRecoIsoUp->Clone("QCDBin_LowDphi_MuRecoIsoUp_input")); reformat(QCDBin_LowDphi_MuRecoIsoUp_input,QCDBin_LowDphi_MuRecoIsoUp);
  TH1D* QCDBin_LowDphi_MuRecoIsoDn_input = static_cast<TH1D*>(QCDBin_LowDphi_MuRecoIsoDn->Clone("QCDBin_LowDphi_MuRecoIsoDn_input")); reformat(QCDBin_LowDphi_MuRecoIsoDn_input,QCDBin_LowDphi_MuRecoIsoDn);
  

  //
  // search bin
  // ----------
  //*AR,Feb27,2017-Here bincontents of first histogram are set to (bincontent_first-bincontent_second)/bincontent_second. Same is done for QCD_HighDphi and QCD_LowDphi.
  takeDiffForSys(searchBin_BMistagUp,searchBin_default);
  takeDiffForSys(searchBin_BMistagDn,searchBin_default);

   
  takeDiffForSys(searchBin_MuRecoSysUp,searchBin_default2);
  takeDiffForSys(searchBin_MuRecoSysDn,searchBin_default2);

  takeDiffForSys(searchBin_MuIsoSysUp,searchBin_default2);
  takeDiffForSys(searchBin_MuIsoSysDn,searchBin_default2);

  takeDiffForSys(searchBin_MuRecoIsoUp,searchBin_default);
  takeDiffForSys(searchBin_MuRecoIsoDn,searchBin_default);

  //
  // QCDBin HiDphi
  // -------------
  takeDiffForSys(QCDBin_HiDphi_BMistagUp,QCDBin_HiDphi_default);
  takeDiffForSys(QCDBin_HiDphi_BMistagDn,QCDBin_HiDphi_default);


  takeDiffForSys(QCDBin_HiDphi_MuRecoSysUp,QCDBin_HiDphi_default);
  takeDiffForSys(QCDBin_HiDphi_MuRecoSysDn,QCDBin_HiDphi_default);

  takeDiffForSys(QCDBin_HiDphi_MuIsoSysUp,QCDBin_HiDphi_default);
  takeDiffForSys(QCDBin_HiDphi_MuIsoSysDn,QCDBin_HiDphi_default);

  takeDiffForSys(QCDBin_HiDphi_MuRecoIsoUp,QCDBin_HiDphi_default);
  takeDiffForSys(QCDBin_HiDphi_MuRecoIsoDn,QCDBin_HiDphi_default);

  // //
  // // QCDBin LowDphi
  // // --------------
  takeDiffForSys(QCDBin_LowDphi_BMistagUp,QCDBin_LowDphi_default);
  takeDiffForSys(QCDBin_LowDphi_BMistagDn,QCDBin_LowDphi_default);


 takeDiffForSys(QCDBin_LowDphi_MuRecoSysUp,QCDBin_LowDphi_default);
 takeDiffForSys(QCDBin_LowDphi_MuRecoSysDn,QCDBin_LowDphi_default);

 takeDiffForSys(QCDBin_LowDphi_MuIsoSysUp,QCDBin_LowDphi_default);
 takeDiffForSys(QCDBin_LowDphi_MuIsoSysDn,QCDBin_LowDphi_default);

 takeDiffForSys(QCDBin_LowDphi_MuRecoIsoUp,QCDBin_LowDphi_default);
 takeDiffForSys(QCDBin_LowDphi_MuRecoIsoDn,QCDBin_LowDphi_default);

  //
  // ---- JEC & MT cut efficiency systematics -----
  // -----------------------------------------------------
  //
  // JEC & MT cut efficiency
  //

 //  histname="searchH_b";

  sprintf(tempname,"searchH_b");
  //tempstack=(THStack*)JECSysUpFile->Get(tempname)->Clone("searchBin_JECSysUp");  
  //TH1D * searchBin_JECSysUp  = (TH1D*) tempstack->GetStack()->Last();
  //tempstack=(THStack*)JECSysDnFile->Get(tempname)->Clone("searchBin_JECSysDn");  
  //TH1D * searchBin_JECSysDn  = (TH1D*) tempstack->GetStack()->Last();
  //tempstack=(THStack*)JECSysRefFile->Get(tempname)->Clone("searchBin_JECSysRef");  
  //TH1D * searchBin_JECSysRef = (TH1D*) tempstack->GetStack()->Last();
  TH1D * searchBin_JECSysUp  = (TH1D*) JECSysUpFile->Get(tempname)->Clone("totalPred_JECSysUp");
  TH1D * searchBin_JECSysDn  = (TH1D*) JECSysDnFile->Get(tempname)->Clone("totalPred_JECSysDn");
  TH1D * searchBin_JECSysRef = (TH1D*) JECSysRefFile->Get(tempname)->Clone("totalPred_JECSysRef");
  searchBin_JECSysUp->Scale(3000.);
  searchBin_JECSysDn->Scale(3000.);
  searchBin_JECSysRef->Scale(3000.);

  TH1D* searchBin_JECSysUp_input = static_cast<TH1D*>(searchBin_JECSysUp->Clone("searchBin_JECSysUp_input")); reformat(searchBin_JECSysUp_input,searchBin_JECSysUp);
  TH1D* searchBin_JECSysDn_input = static_cast<TH1D*>(searchBin_JECSysDn->Clone("searchBin_JECSysDn_input")); reformat(searchBin_JECSysDn_input,searchBin_JECSysDn);
  TH1D* searchBin_JECSysRef_input = static_cast<TH1D*>(searchBin_JECSysRef->Clone("searchBin_JECSysRef_input")); reformat(searchBin_JECSysRef_input,searchBin_JECSysRef);
  //*AR,Feb27,2017-Same method used as in case of muon Iso/Reco/BMistag sys
  takeDiffForSys(searchBin_JECSysUp,searchBin_JECSysRef);
  takeDiffForSys(searchBin_JECSysDn,searchBin_JECSysRef);

  //  histname="QCD_Up";
  sprintf(tempname,"QCD_Up");
  //tempstack=(THStack*)JECSysUpFile->Get(tempname)->Clone("QCDBin_HiDphi_JECSysUp");  
  //TH1D * QCDBin_HiDphi_JECSysUp  = (TH1D*) tempstack->GetStack()->Last();
  //tempstack=(THStack*)JECSysDnFile->Get(tempname)->Clone("QCDBin_HiDphi_JECSysDn");  
  //TH1D * QCDBin_HiDphi_JECSysDn  = (TH1D*) tempstack->GetStack()->Last();
  //tempstack=(THStack*)JECSysRefFile->Get(tempname)->Clone("QCDBin_HiDphi_JECSysRef");  
  //TH1D * QCDBin_HiDphi_JECSysRef = (TH1D*) tempstack->GetStack()->Last();
  TH1D * QCDBin_HiDphi_JECSysUp  = (TH1D*) JECSysUpFile->Get(tempname)->Clone("QCDBin_HiDphi_JECSysUp");
  TH1D * QCDBin_HiDphi_JECSysDn  = (TH1D*) JECSysDnFile->Get(tempname)->Clone("QCDBin_HiDphi_JECSysDn");
  TH1D * QCDBin_HiDphi_JECSysRef = (TH1D*) JECSysRefFile->Get(tempname)->Clone("QCDBin_HiDphi_JECSysRef");
  QCDBin_HiDphi_JECSysUp->Scale(3000.);
  QCDBin_HiDphi_JECSysDn->Scale(3000.);
  QCDBin_HiDphi_JECSysRef->Scale(3000.);

  TH1D* QCDBin_HiDphi_JECSysUp_input = static_cast<TH1D*>(QCDBin_HiDphi_JECSysUp->Clone("QCDBin_HiDphi_JECSysUp_input")); reformat(QCDBin_HiDphi_JECSysUp_input,QCDBin_HiDphi_JECSysUp);
  TH1D* QCDBin_HiDphi_JECSysDn_input = static_cast<TH1D*>(QCDBin_HiDphi_JECSysDn->Clone("QCDBin_HiDphi_JECSysDn_input")); reformat(QCDBin_HiDphi_JECSysDn_input,QCDBin_HiDphi_JECSysDn);
  TH1D* QCDBin_HiDphi_JECSysRef_input = static_cast<TH1D*>(QCDBin_HiDphi_JECSysRef->Clone("QCDBin_HiDphi_JECSysRef_input")); reformat(QCDBin_HiDphi_JECSysRef_input,searchBin_JECSysRef);
  
  takeDiffForSys(QCDBin_HiDphi_JECSysUp,QCDBin_HiDphi_JECSysRef);
  takeDiffForSys(QCDBin_HiDphi_JECSysDn,QCDBin_HiDphi_JECSysRef);

  //*AR-Feb27, 2017-All bins are set to average over all bin values. Hence get flat histogram.Only used for QCDBin_HiDphi/QCDBin_LowDphi JEC sys.
  takeAverage(QCDBin_HiDphi_JECSysUp);
  takeAverage(QCDBin_HiDphi_JECSysDn);

  //  histname="QCD_Low";
  sprintf(tempname,"QCD_Low");
  //tempstack=(THStack*)JECSysUpFile->Get(tempname)->Clone("QCDBin_LowDphi_JECSysUp");  
  //TH1D * QCDBin_LowDphi_JECSysUp  = (TH1D*) tempstack->GetStack()->Last();
  //tempstack=(THStack*)JECSysDnFile->Get(tempname)->Clone("QCDBin_LowDphi_JECSysDn");  
  //TH1D * QCDBin_LowDphi_JECSysDn  = (TH1D*) tempstack->GetStack()->Last();
  //tempstack=(THStack*)JECSysRefFile->Get(tempname)->Clone("QCDBin_LowDphi_JECSysRef");  
  //TH1D * QCDBin_LowDphi_JECSysRef = (TH1D*) tempstack->GetStack()->Last();
  TH1D * QCDBin_LowDphi_JECSysUp  = (TH1D*) JECSysUpFile->Get(tempname)->Clone("QCDBin_LowDphi_JECSysUp");
  TH1D * QCDBin_LowDphi_JECSysDn  = (TH1D*) JECSysDnFile->Get(tempname)->Clone("QCDBin_LowDphi_JECSysDn");
  TH1D * QCDBin_LowDphi_JECSysRef = (TH1D*) JECSysRefFile->Get(tempname)->Clone("QCDBin_LowDphi_JECSysRef");
  QCDBin_LowDphi_JECSysUp->Scale(3000.);
  QCDBin_LowDphi_JECSysDn->Scale(3000.);
  QCDBin_LowDphi_JECSysRef->Scale(3000.);

  TH1D* QCDBin_LowDphi_JECSysUp_input = static_cast<TH1D*>(QCDBin_LowDphi_JECSysUp->Clone("QCDBin_LowDphi_JECSysUp_input")); reformat(QCDBin_LowDphi_JECSysUp_input,QCDBin_LowDphi_JECSysUp);
  TH1D* QCDBin_LowDphi_JECSysDn_input = static_cast<TH1D*>(QCDBin_LowDphi_JECSysDn->Clone("QCDBin_LowDphi_JECSysDn_input")); reformat(QCDBin_LowDphi_JECSysDn_input,QCDBin_LowDphi_JECSysDn);
  TH1D* QCDBin_LowDphi_JECSysRef_input = static_cast<TH1D*>(QCDBin_LowDphi_JECSysRef->Clone("QCDBin_LowDphi_JECSysRef_input")); reformat(QCDBin_LowDphi_JECSysRef_input,searchBin_JECSysRef);

  takeDiffForSys(QCDBin_LowDphi_JECSysUp,QCDBin_LowDphi_JECSysRef);
  takeDiffForSys(QCDBin_LowDphi_JECSysDn,QCDBin_LowDphi_JECSysRef);
  //*AR, Feb27, 2017-All bins are set to average over all bin values. Hence get flat histogram.
  takeAverage(QCDBin_LowDphi_JECSysUp);
  takeAverage(QCDBin_LowDphi_JECSysDn);
  //-----------------------------------------------------------------------------------------------------------------------------------------------------
  //

  //
  // Store outputs
  //
  sprintf(tempname,"HadTauEstimation_data_formatted.root");
  TFile HadTauEstimation_OutputFile(tempname,"RECREATE");
  //*AR, Mar15, 2017-reLabel sets bin labels of 174 bin search bin or 223 QCD histograms. Make sure reLabel function is modified with existing number of search bins and QCD bins.
  //
  searchBin_nominal->Write(); //data_searchH_b hist
  searchBin_nominal_fullstatuncertainty->Write(); //*AR,Mar15,2017-data 174 bin stat+sys errors added in quadrature.
  reLabel(searchBin_StatUncertainties);             searchBin_StatUncertainties->Write(); //sigma(Data_searchH_b) 
  std::cout<<"*************CH1 *************"<<endl;    
  searchBin_closureUncertainty->Add(searchBin_one); //*AR,Mar15,2017-the maximum of deviation of Exp/Pre from 1 and fractional error on closure[stat_err(closure)/(closure)]
      reLabel(searchBin_closureUncertainty);            searchBin_closureUncertainty->Write();
  std::cout<<"*************CH1A *************"<<endl;
  searchBin_closureUncertainty_adhoc->Add(searchBin_one);//*AR,Mar15,2017-deviation of NjNb corr from 1 as an absolute value
 reLabel(searchBin_closureUncertainty_adhoc);      searchBin_closureUncertainty_adhoc->Write();
  std::cout<<"*************CH1B *************"<<endl;
  searchBin_BMistagUp->Add(searchBin_one); //*AR,Mar15,2017-(searchBin_BMistagUp-searchBin_default)/searchBin_default from MC sys file. Same is true for MuRecoSys,MuIsoSys,MuRecoIso,JECSys below.
                reLabel(searchBin_BMistagUp);                     searchBin_BMistagUp->Write();
  std::cout<<"*************CH2 *************"<<endl;
  searchBin_BMistagDn->Add(searchBin_one);                reLabel(searchBin_BMistagDn);                     searchBin_BMistagDn->Write("totalPredBMistagDown_HadTau");
  searchBin_MuRecoSysUp->Add(searchBin_one);              reLabel(searchBin_MuRecoSysUp);                   searchBin_MuRecoSysUp->Write();
  searchBin_MuRecoSysDn->Add(searchBin_one);              reLabel(searchBin_MuRecoSysDn);                   searchBin_MuRecoSysDn->Write("totalPredMuRecoSysDown_LL");
  searchBin_MuIsoSysUp->Add(searchBin_one);               reLabel(searchBin_MuIsoSysUp);                    searchBin_MuIsoSysUp->Write();
  searchBin_MuIsoSysDn->Add(searchBin_one);               reLabel(searchBin_MuIsoSysDn);                    searchBin_MuIsoSysDn->Write("totalPredMuIsoSysDown_LL");
  searchBin_MuRecoIsoUp->Add(searchBin_one);              reLabel(searchBin_MuRecoIsoUp);                   searchBin_MuRecoIsoUp->Write();
  searchBin_MuRecoIsoDn->Add(searchBin_one);              reLabel(searchBin_MuRecoIsoDn);                   searchBin_MuRecoIsoDn->Write("totalPredMuRecoStatDown_LL");
  searchBin_JECSysUp->Add(searchBin_one);                 reLabel(searchBin_JECSysUp);                      searchBin_JECSysUp->Write("totalPred_JECSysUp");
  searchBin_JECSysDn->Add(searchBin_one);                 reLabel(searchBin_JECSysDn);                      searchBin_JECSysDn->Write("totalPred_JECSysDown");
  searchBin_MTSysUp->SetName("searchBin_MTSysUp");        reLabel(searchBin_MTSysUp);
  searchBin_MTSysDn->SetName("searchBin_MTSysDn");        reLabel(searchBin_MTSysDn);
  searchBin_MTSysUp->Write(); //searchBin_MTSysUp--> 1/(1-MtSysFlat)
  std::cout<<"*************CH3 *************"<<endl;
  searchBin_MTSysDn->Write("totalPredMTSysDown_HadTau"); //searchBin_MTSysDn--->1/(1+MtSysFlat)
  searchBin_MtEffStat->Add(searchBin_one); //sigma(mTeff_174)/(mTeff_174) 
                 reLabel(searchBin_MtEffStat);                      searchBin_MtEffStat->Write();  
		 searchBin_IsoTrkVetoEffUncertaintyStat->Add(searchBin_one); //sigma(Isotrkeff_174)/(Isotrkeff_174)
 reLabel(searchBin_IsoTrkVetoEffUncertaintyStat);searchBin_IsoTrkVetoEffUncertaintyStat->Write();
 searchBin_IsoTrkVetoEffUncertaintySys->Add(searchBin_one); //(1-eff)*0.1/eff
  reLabel(searchBin_IsoTrkVetoEffUncertaintySys); searchBin_IsoTrkVetoEffUncertaintySys->Write();
  searchBin_AccStat->Add(searchBin_one); //sigma(NomAcceff_174)/(NomAcceff_174)
                   reLabel(searchBin_AccStat);                        searchBin_AccStat->Write();
		   reLabel(searchBin_AccSysPDFUp);                    searchBin_AccSysPDFUp->Write(); //(nominalAcc_174)/(nominalAcc_174+PDFAcc_174)
							   reLabel(searchBin_AccSysPDFDn);                    searchBin_AccSysPDFDn->Write("totalPredPDFDown_LL");
							   reLabel(searchBin_AccSysScaleUp);                  searchBin_AccSysScaleUp->Write();//(nominalAcc_174)/(nominalAcc_174+ScaleAcc_174)
							   reLabel(searchBin_AccSysScaleDn);                  searchBin_AccSysScaleDn->Write("totalPredScaleDown_LL");
  std::cout<<"*************CH4 *************"<<endl;
  searchBin_MuFromTauStat->Add(searchBin_one);            reLabel(searchBin_MuFromTauStat);                   searchBin_MuFromTauStat->Write(); //sigma(muFrmtaueff)/(1-muFrmtaueff)
  searchBin_DileptonUncertainty->Add(searchBin_one);//flat histogram of value 2% and error zero
      reLabel(searchBin_DileptonUncertainty);             searchBin_DileptonUncertainty->Write();
  searchBin_TrigEffUncertainty->Add(searchBin_one); //flat histogram of value 2% and error zero
      reLabel(searchBin_TrigEffUncertainty);              searchBin_TrigEffUncertainty->Write();

  //
  QCDBin_HiDphi_nominal->Write();
  QCDBin_HiDphi_nominal_fullstatuncertainty->Write();
  QCDBin_HiDphi_StatUncertainties->Write();
  std::cout<<"*************CH5 *************"<<endl;  
    QCDBin_HiDphi_closureUncertainty->Add(QCDBin_one);       QCDBin_HiDphi_closureUncertainty->Write();
    QCDBin_HiDphi_closureUncertainty_adhoc->Add(QCDBin_one); QCDBin_HiDphi_closureUncertainty_adhoc->Write();
  QCDBin_HiDphi_BMistagUp->Add(QCDBin_one);                QCDBin_HiDphi_BMistagUp->Write();
  QCDBin_HiDphi_BMistagDn->Add(QCDBin_one);                QCDBin_HiDphi_BMistagDn->Write("QCDBin_HiDphi_BMistagDn");
  QCDBin_HiDphi_MuRecoSysUp->Add(QCDBin_one);              QCDBin_HiDphi_MuRecoSysUp->Write();
  QCDBin_HiDphi_MuRecoSysDn->Add(QCDBin_one);              QCDBin_HiDphi_MuRecoSysDn->Write("QCDBin_HiDphi_MuRecoSysDown");
  QCDBin_HiDphi_MuIsoSysUp->Add(QCDBin_one);               QCDBin_HiDphi_MuIsoSysUp->Write();
  std::cout<<"*************CH6 *************"<<endl;
  QCDBin_HiDphi_MuIsoSysDn->Add(QCDBin_one);               QCDBin_HiDphi_MuIsoSysDn->Write("QCDBin_HiDphi_MuIsoSysDown");
  QCDBin_HiDphi_MuRecoIsoUp->Add(QCDBin_one);              QCDBin_HiDphi_MuRecoIsoUp->Write();
  QCDBin_HiDphi_MuRecoIsoDn->Add(QCDBin_one);              QCDBin_HiDphi_MuRecoIsoDn->Write("QCDBin_HiDphi_MuRecoIsoDown");
  QCDBin_HiDphi_JECSysUp->Add(QCDBin_one);                 QCDBin_HiDphi_JECSysUp->Write("QCDBin_HiDphi_JECSysUp");
  std::cout<<"*************CH7 *************"<<endl;
  QCDBin_HiDphi_JECSysDn->Add(QCDBin_one);                 QCDBin_HiDphi_JECSysDn->Write("QCDBin_HiDphi_JECSysDown");
  QCDBin_HiDphi_MTSysUp->Write("QCDBin_HiDphi_MTSysUp");
  QCDBin_HiDphi_MTSysDn->Write("QCDBin_HiDphi_MTSysDown");
  QCDBin_HiDphi_MtEffStat->Add(QCDBin_one);                QCDBin_HiDphi_MtEffStat->Write();  
  QCDBin_HiDphi_IsoTrkVetoEffUncertaintyStat->Add(QCDBin_one); QCDBin_HiDphi_IsoTrkVetoEffUncertaintyStat->Write();
  QCDBin_HiDphi_IsoTrkVetoEffUncertaintySys->Add(QCDBin_one);  QCDBin_HiDphi_IsoTrkVetoEffUncertaintySys->Write();
  QCDBin_HiDphi_AccStat->Add(QCDBin_one);                  QCDBin_HiDphi_AccStat->Write();
  QCDBin_HiDphi_AccSysPDFUp->Write();
  QCDBin_HiDphi_AccSysPDFDn->Write("QCDBin_HiDphi_AccSysPDFDown");
  QCDBin_HiDphi_AccSysScaleUp->Write();
  QCDBin_HiDphi_AccSysScaleDn->Write("QCDBin_HiDphi_AccSysScaleDown");
  std::cout<<"*************CH8 *************"<<endl;
  QCDBin_HiDphi_MuFromTauStat->Add(QCDBin_one);            QCDBin_HiDphi_MuFromTauStat->Write();
  QCDBin_HiDphi_DileptonUncertainty->Add(QCDBin_one);      QCDBin_HiDphi_DileptonUncertainty->Write();
  QCDBin_HiDphi_TrigEffUncertainty->Add(QCDBin_one);       QCDBin_HiDphi_TrigEffUncertainty->Write();

  //
  QCDBin_LowDphi_nominal->Write();
  QCDBin_LowDphi_nominal_fullstatuncertainty->Write();
  QCDBin_LowDphi_StatUncertainties->Write();
  std::cout<<"*************CH9 *************"<<endl;

  QCDBin_LowDphi_closureUncertainty->Add(QCDBin_one);      QCDBin_LowDphi_closureUncertainty->Write();
  QCDBin_LowDphi_closureUncertainty_adhoc->Add(QCDBin_one);QCDBin_LowDphi_closureUncertainty_adhoc->Write();
  QCDBin_LowDphi_BMistagUp->Add(QCDBin_one);               QCDBin_LowDphi_BMistagUp->Write();
  QCDBin_LowDphi_BMistagDn->Add(QCDBin_one);               QCDBin_LowDphi_BMistagDn->Write("QCDBin_LowDphi_BMistagDown");
  QCDBin_LowDphi_MuRecoSysUp->Add(QCDBin_one);             QCDBin_LowDphi_MuRecoSysUp->Write();
  QCDBin_LowDphi_MuRecoSysDn->Add(QCDBin_one);             QCDBin_LowDphi_MuRecoSysDn->Write("QCDBin_LowDphi_MuRecoSysDown");
  QCDBin_LowDphi_MuIsoSysUp->Add(QCDBin_one);              QCDBin_LowDphi_MuIsoSysUp->Write();
  QCDBin_LowDphi_MuIsoSysDn->Add(QCDBin_one);              QCDBin_LowDphi_MuIsoSysDn->Write("QCDBin_LowDphi_MuIsoSysDown");
  QCDBin_LowDphi_MuRecoIsoUp->Add(QCDBin_one);             QCDBin_LowDphi_MuRecoIsoUp->Write();
  std::cout<<"*************CH10 *************"<<endl;
  QCDBin_LowDphi_MuRecoIsoDn->Add(QCDBin_one);             QCDBin_LowDphi_MuRecoIsoDn->Write("QCDBin_LowDphi_MuRecoIsoDown");
  QCDBin_LowDphi_JECSysUp->Add(QCDBin_one);                QCDBin_LowDphi_JECSysUp->Write("QCDBin_LowDphi_JECSysUp");
  QCDBin_LowDphi_JECSysDn->Add(QCDBin_one);                QCDBin_LowDphi_JECSysDn->Write("QCDBin_LowDphi_JECSysDown");
  QCDBin_LowDphi_MTSysUp->Write("QCDBin_LowDphi_MTSysUp");
  QCDBin_LowDphi_MTSysDn->Write("QCDBin_LowDphi_MTSysDown");
  QCDBin_LowDphi_MtEffStat->Add(QCDBin_one);                QCDBin_LowDphi_MtEffStat->Write();  
  QCDBin_LowDphi_IsoTrkVetoEffUncertaintyStat->Add(QCDBin_one); QCDBin_LowDphi_IsoTrkVetoEffUncertaintyStat->Write();
  QCDBin_LowDphi_IsoTrkVetoEffUncertaintySys->Add(QCDBin_one);  QCDBin_LowDphi_IsoTrkVetoEffUncertaintySys->Write();
  QCDBin_LowDphi_AccStat->Add(QCDBin_one);                  QCDBin_LowDphi_AccStat->Write();
  QCDBin_LowDphi_AccSysPDFUp->Write();
  QCDBin_LowDphi_AccSysPDFDn->Write("QCDBin_LowDphi_AccSysPDFDown");
  QCDBin_LowDphi_AccSysScaleUp->Write();
  QCDBin_LowDphi_AccSysScaleDn->Write("QCDBin_LowDphi_AccSysScaleDown");
  QCDBin_LowDphi_MuFromTauStat->Add(QCDBin_one);            QCDBin_LowDphi_MuFromTauStat->Write();
  QCDBin_LowDphi_DileptonUncertainty->Add(QCDBin_one);      QCDBin_LowDphi_DileptonUncertainty->Write();
  QCDBin_LowDphi_TrigEffUncertainty->Add(QCDBin_one);       QCDBin_LowDphi_TrigEffUncertainty->Write();

  HadTauEstimation_OutputFile.Close();

}

// ----------
void printMaxMin(TH1* input){

  double max=0.;
  double min=100.;
  int    ient=0;
  for (int ibin=0;ibin<input->GetNbinsX();ibin++){
    double content = fabs(input->GetBinContent(ibin+1)-1.);
    if (content>max) max=content; 
    if (content<min) min=content; 
  }
  //std::cout << input->GetTitle() << std::endl;
  //  std::cout << input->GetName() << std::endl;
  //printf("max: %8.3f, min: %8.3f\n",max,min);
  //sum = sum/double(ient);
  //std::cout << sum << std::endl;
  //for (int ibin=0;ibin<sys->GetNbinsX();ibin++){
  //  sys->SetBinContent(ibin+1,sum);
  //}

};

// ----------
void takeAverage(TH1* sys){

  TH1* input_sys = (TH1*)sys->Clone("input_sys");
  sys->Reset();
  sys->SetLineColor(1);
  sys->SetFillColor(0);

  double sum=0.;
  int    ient=0;
  for (int ibin=0;ibin<sys->GetNbinsX();ibin++){
    sum  += input_sys->GetBinContent(ibin+1);
    ient ++;
  }
  sum = sum/double(ient);
  //std::cout << sum << std::endl;
  for (int ibin=0;ibin<sys->GetNbinsX();ibin++){
    sys->SetBinContent(ibin+1,sum);
  }

};

// ----------
void takeDiffForSys(TH1* sys, TH1* input_nominal){

  TH1* input_sys = (TH1*)sys->Clone("input_sys");
  sys->Reset();
  sys->SetLineColor(1);
  sys->SetFillColor(0);

  Float_t diff;
  for (int ibin=0;ibin<sys->GetNbinsX();ibin++){
    if (input_nominal->GetBinContent(ibin+1)){
      diff = float((input_sys->GetBinContent(ibin+1)-input_nominal->GetBinContent(ibin+1))
		   /input_nominal->GetBinContent(ibin+1));
      sys->SetBinContent(ibin+1,diff);
    }
  }
  
}

// ----------
//*AR, Feb27,2017-Used for modulating PDF and scale uncertainties
void accErrPropagation(TH1* sys, TH1* input_nominal){

  TH1* input_sys = (TH1*)sys->Clone("input_sys");
  sys->Reset();
  sys->SetLineColor(1);
  sys->SetFillColor(0);

  Float_t diff;
  for (int ibin=0;ibin<sys->GetNbinsX();ibin++){
    if (input_nominal->GetBinContent(ibin+1)){
      /* */
      diff = (input_nominal->GetBinContent(ibin+1)) /
	(input_nominal->GetBinContent(ibin+1) + input_sys->GetBinContent(ibin+1));
      /* */
      //diff=input_sys->GetBinContent(ibin+1);
      sys->SetBinContent(ibin+1,diff);
    }
  }
  
}

// ----------
void effMapStatErrPropagation(TH1* input, TH1* output){
  
  output->Reset();

  for (int ibin=0;ibin<input->GetNbinsX();ibin++){
    if (input->GetBinContent(ibin+1))
      output->SetBinContent(ibin+1,input->GetBinError(ibin+1)/input->GetBinContent(ibin+1));
  }
  //output->Divide(input);  

}
// ----------
void effMapConstErrPropagation(TH1* input, double fractionalError, TH1* output){
  
  output->Reset();

  for (int ibin=0;ibin<input->GetNbinsX();ibin++){
    if (input->GetBinContent(ibin+1))
      output->SetBinContent(ibin+1,input->GetBinContent(ibin+1)/(input->GetBinContent(ibin+1)*(1.+fractionalError)));
  }
  //output->Divide(input);  

}

// ----------
//*AR, Mar14,2017-Note that TH1 * one histogram is not actually used here.
void muFromTauStatErrPropagation(TH1* input, TH1* one, TH1* output){
  
  output->Reset();

  for (int ibin=0;ibin<input->GetNbinsX();ibin++){
    if (input->GetBinContent(ibin+1))
      output->SetBinContent(ibin+1,input->GetBinError(ibin+1)/(1.-input->GetBinContent(ibin+1)));
  }
  //input->Add(one,-1.);    // (f-1.) ~ -0.7
  //input->Scale(-1.);      // (1-f) ~ 0.7
  //output->Divide(input);  // sigma(f)/(1-f)

}

// ----------
//AR, Mar6,2017-Earlier this function had form void isoTrkVetoErrPropagation(TH1* input, double syserr, TH1* one, TH1* output_tot, TH1* output_stat, TH1* output_sys). But now instead of flat systematic uncertainty of 10% we will use Njet dependent values.
//Note that TH1 * one histogram is not actually used here. 
void isoTrkVetoErrPropagation(TH1* input, TH1* PionIsoTrksys, TH1* one,
			      TH1* output_tot, TH1* output_stat, TH1* output_sys){
std::cout<<"*******************"<<endl;  

  //
  // Apply a constant systematic error on the fraction of rejected events
  // Also, the statistical uncertainty is propagated
  //

  output_tot->Reset();
  output_stat->Reset();
  output_sys->Reset();
  double syserr=0;
  int Njetbin=0;
  for (int ibin=0;ibin<output_sys->GetNbinsX();ibin++){
    if(ibin<30)Njetbin=1;
    else if(ibin<70)Njetbin=2;
    else if(ibin<110)Njetbin=3;
    else if(ibin<142)Njetbin=4;
    else Njetbin=5;
    syserr=PionIsoTrksys->GetBinContent(Njetbin);
    //    std::cout<<" ibin "<<ibin<<" Njetbin "<<Njetbin<<" syserr "<<syserr<<endl;
    if (input->GetBinContent(ibin+1)){      
      output_sys->SetBinContent(ibin+1,(1.-input->GetBinContent(ibin+1))*syserr/input->GetBinContent(ibin+1));
      output_stat->SetBinContent(ibin+1,input->GetBinError(ibin+1)/input->GetBinContent(ibin+1));
      double unc = pow(pow(output_sys->GetBinContent(ibin+1),2)
		       + pow(output_stat->GetBinError(ibin+1),2),0.5);   
      std::cout<<" ibin "<<ibin<<" sys "<<output_sys->GetBinContent(ibin+1)<<" stat "<<output_stat->GetBinContent(ibin+1)<<endl;   
      output_tot->SetBinContent(ibin+1,unc);      
    }
  }

  /*
  output_sys->Add(one,-1.);  // (eff-1.) ~ -0.4
  output_sys->Scale(-1.);    // (1-eff) ~ 0.4
  output_sys->Scale(syserr); // (1-eff)*0.1 ~ 0.04, i.e. 10% of the (1-eff)
  for (int ibin=0;ibin<output_tot->GetNbinsX();ibin++){
    double unc = pow(pow(output_sys->GetBinContent(ibin+1),2)
		     + pow(input->GetBinError(ibin+1),2),0.5);
    output_tot->SetBinContent(ibin+1,unc);
    output_stat->SetBinContent(ibin+1,input->GetBinError(ibin+1));
  }
  output_sys->Divide(input);   // (1-eff)*0.1/eff
  output_stat->Divide(input);  // sigma(eff)/eff
  output_tot->Divide(input);   // 
  */

}

// ----------
void binMap(TH1* input, TH1* output){
  //
  // Map from 11*3+6*2=45 binning to 72 binning
  //
  // This involves two steps
  // 1: 45 binning to 6*3=18 bins
  // 2: 18 bins to 18*4=72 bins

  double noB[18]
    ={0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.};
  double noBe[18]
    ={0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.};
  double noB_ent[18]
    ={6,6,6,6,3,6,
      1,1,1,1,1,1,
      1,1,1,1,1,1};
  int map_input[45] // which 72/4=18 bins this 45 efficiency binning corresponds to?
    ={1,2,3,1,2,3,4,4,5,6,6,
      1,2,3,1,2,3,4,4,5,6,6,
      1,2,3,1,2,3,4,4,5,6,6,
       7, 8, 9,10,11,12,
      13,14,15,16,17,18};

  int map_output[72]
    ={1,2,3,4,5,6,
      1,2,3,4,5,6,
      1,2,3,4,5,6,
      1,2,3,4,5,6,
      7,8,9,10,11,12,
      7,8,9,10,11,12,
      7,8,9,10,11,12,
      7,8,9,10,11,12,
      13,14,15,16,17,18,
      13,14,15,16,17,18,
      13,14,15,16,17,18,
      13,14,15,16,17,18};

  // 1: 45 binning to 6*3=18 bins
  for (int ibin=1; ibin<=input->GetNbinsX(); ibin++){    
    noB[map_input[ibin-1]-1] += input->GetBinContent(ibin)/noB_ent[map_input[ibin-1]-1];
    noBe[map_input[ibin-1]-1] += input->GetBinError(ibin)/noB_ent[map_input[ibin-1]-1];
  }
  
  // 2: 18 bins to 18*4=72 bins
  for (int ibin=1; ibin<=output->GetNbinsX(); ibin++){    
    output->SetBinContent(ibin,noB[map_output[ibin-1]-1]);
    output->SetBinError(ibin,noBe[map_output[ibin-1]-1]);
  }

};

// ----------
void binMap_ICHEP2016(TH1* input, TH1* output){
  //
  // Map from 12*6=72 binning to 4*4*10=160 binning
  //
  // This involves two steps
  // 1: 72 binning to 12*4= 48  bins
  // 2: 48 bins    to 40*4=160 bins
  std::cout<<" binMap_ICHEP2016 "<< endl;

  double noB[60]
    ={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
  double noBe[60]
    ={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
  //*AR,Feb24,2017-Lists denominators for averaging efficiencies needed to move from 72 bins to 60 bins
  double noB_ent[60]
    ={1,1,1,1,1,1,1,1,1,1,1,1,
      2,2,2,2,2,2,2,2,2,2,2,2,
      2,2,2,2,2,2,2,2,2,2,2,2,
      1,1,1,1,1,1,1,1,1,1,1,1,
      1,1,1,1,1,1,1,1,1,1,1,1};
  //*AR,Feb24,2017-Used to define index of output map while moved from 72 bins to 60 bins
  int map_input[84] 
    ={ 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,  
       13,14,15,16,17,18,19,20,21,22,23,24,
       13,14,15,16,17,18,19,20,21,22,23,24,
       25,26,27,28,29,30,31,32,33,34,35,36,
       25,26,27,28,29,30,31,32,33,34,35,36,
       37,38,39,40,41,42,43,44,45,46,47,48,
       49,50,51,52,53,54,55,56,57,58,59,60};
  //*AR,Feb24,2017-This map is used to pick up a bin of 60 binned averaged efficiency map to assign efficiency to 174 bin output histogram.
  //*AR,Feb24,2017-We are using same efficiency value for all bjet bins corresponding to same [Njet,HT,MHT]
  //*AR,Feb24,2017-Here note that numbers start from 4 as first 3 bins of efficiency map of given Njet corresponds to QCD control region bins.  
  int map_output[174]
    ={ 4, 5, 6, 7, 8, 9,10,11,12,12,
       4, 5, 6, 7, 8, 9,10,11,12,12,
       4, 5, 6, 7, 8, 9,10,11,12,12,
       16,17,18,19,20,21,22,23,24,24,
       16,17,18,19,20,21,22,23,24,24,
       16,17,18,19,20,21,22,23,24,24,
       16,17,18,19,20,21,22,23,24,24,
       28,29,30,31,32,33,34,35,36,36,
       28,29,30,31,32,33,34,35,36,36,
       28,29,30,31,32,33,34,35,36,36,
       28,29,30,31,32,33,34,35,36,36,
       41,42,44,45,46,47,48,48,
       41,42,44,45,46,47,48,48,
       41,42,44,45,46,47,48,48,
       41,42,44,45,46,47,48,48,
       53,54,56,57,58,59,60,60,
       53,54,56,57,58,59,60,60,
       53,54,56,57,58,59,60,60,
       53,54,56,57,58,59,60,60       
  };
  
  // 1: 72 binning to 60 bins
  //*AR,Feb24,2017-Actual averaging over efficiencies to move from 72 bins to 60 bins. For ex. Bincontent of 13 th bin in output map is average of 13th and 26th bins in input 74 bin map 
 
 for (int ibin=1; ibin<=input->GetNbinsX(); ibin++){ 
    noB[map_input[ibin-1]-1] += input->GetBinContent(ibin)/noB_ent[map_input[ibin-1]-1];
    noBe[map_input[ibin-1]-1] += input->GetBinError(ibin)/noB_ent[map_input[ibin-1]-1];
    //std::cout<<" 72_binIndx "<<ibin<<" 72_binContent "<<input->GetBinContent(ibin)<<" newIndx "<< map_input[ibin-1]-1<<" denom to get average efficiency "<<noB_ent[map_input[ibin-1]-1]<<  " newBinContent "<< noB[map_input[ibin-1]-1]<<endl; 

  }

  for(int j=0;j<60;j++){
    //  std::cout<< " j "<< j<<" noB[j] "<<noB[j]<<endl;
  }
  
  //*AR,Feb24,2017-Here we create 174 efficiency histogram from 60 binned averaged efficiency map
  for (int ibin=1; ibin<=output->GetNbinsX(); ibin++){    
    //std::cout<<" ibin "<<ibin<<" map_output[ibin-1] "<< map_output[ibin-1]<< " noB[map_output[ibin-1]-1] "<< noB[map_output[ibin-1]-1]<<endl;
    output->SetBinContent(ibin,noB[map_output[ibin-1]-1]);
    output->SetBinError(ibin,noBe[map_output[ibin-1]-1]);
  }

};

// ----------
void binMap_QCD(TH1* input, TH1* output){
  //
  // Map from 11*3+6*2=45 binning to 44*5 binning
  //

  double noB[45]
    ={0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.};
  double noBe[45]
    ={0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.};

  int map_output[220]
  ={1,2,3,4,5,6,7,8,9,10,11,
    1,2,3,4,5,6,7,8,9,10,11,
    1,2,3,4,5,6,7,8,9,10,11,
    1,2,3,4,5,6,7,8,9,10,11,
    12,13,14,15,16,17,18,19,20,21,22, // 12-22,
    12,13,14,15,16,17,18,19,20,21,22, // 
    12,13,14,15,16,17,18,19,20,21,22, // 
    12,13,14,15,16,17,18,19,20,21,22, // 
    23,24,25,26,27,28,29,30,31,32,33, // 23-33,
    23,24,25,26,27,28,29,30,31,32,33, // 
    23,24,25,26,27,28,29,30,31,32,33, // 
    23,24,25,26,27,28,29,30,31,32,33, // 
    34,35,36,34,35,36,37,37,38,39,39, // 34-39
    34,35,36,34,35,36,37,37,38,39,39, //
    34,35,36,34,35,36,37,37,38,39,39, //
    34,35,36,34,35,36,37,37,38,39,39, //
    40,41,42,40,41,42,43,43,44,45,45, // 40-45
    40,41,42,40,41,42,43,43,44,45,45, //
    40,41,42,40,41,42,43,43,44,45,45, //
    40,41,42,40,41,42,43,43,44,45,45  //
  };

  for (int ibin=1; ibin<=input->GetNbinsX(); ibin++){    
    noB[ibin-1]  = input->GetBinContent(ibin);
    noBe[ibin-1] = input->GetBinError(ibin);
  }

  for (int ibin=1; ibin<=output->GetNbinsX(); ibin++){    
    output->SetBinContent(ibin,noB[map_output[ibin-1]-1]);
    output->SetBinError(ibin,noBe[map_output[ibin-1]-1]);
  }

};

// ----------
void binMap_QCD_ICHEP2016(TH1* input, TH1* output){
  //
  // Map from 12*6=72 binning to 4*4*13=208 binning
  //
  // This involves two steps
  // 1: 72 binning to 12*4= 48  bins
  // 2: 48 bins    to 13*4*4=208 bins
  //  std::cout<<" binMap_QCD_ICHEP2016 "<< endl;
  double noB[60]
    ={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
  double noBe[60]
    ={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
      0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
  double noB_ent[60]
    ={1,1,1,1,1,1,1,1,1,1,1,1,
      2,2,2,2,2,2,2,2,2,2,2,2,
      2,2,2,2,2,2,2,2,2,2,2,2,
      1,1,1,1,1,1,1,1,1,1,1,1,
      1,1,1,1,1,1,1,1,1,1,1,1};
  int map_input[84] // which 48 bins this 72 efficiency binning corresponds to?
    ={1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,  
      13,14,15,16,17,18,19,20,21,22,23,24,
      13,14,15,16,17,18,19,20,21,22,23,24,
      25,26,27,28,29,30,31,32,33,34,35,36,
      25,26,27,28,29,30,31,32,33,34,35,36,
      37,38,39,40,41,42,43,44,45,46,47,48,
      49,50,51,52,53,54,55,56,57,58,59,60
      };

  int map_output[223]
    ={ 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,12,
       1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,12,
       1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,12,
       13,14,15,16,17,18,19,20,21,22,23,24,24,
       13,14,15,16,17,18,19,20,21,22,23,24,24,
       13,14,15,16,17,18,19,20,21,22,23,24,24,
       13,14,15,16,17,18,19,20,21,22,23,24,24,
       25,26,27,28,29,30,31,32,33,34,35,36,36,
       25,26,27,28,29,30,31,32,33,34,35,36,36,
       25,26,27,28,29,30,31,32,33,34,35,36,36,
       25,26,27,28,29,30,31,32,33,34,35,36,36,
       38,39,41,42,44,45,46,47,48,48,
       38,39,41,42,44,45,46,47,48,48,
       38,39,41,42,44,45,46,47,48,48,
       38,39,41,42,44,45,46,47,48,48,
       50,51,53,54,56,57,58,59,60,61,
       50,51,53,54,56,57,58,59,60,61,
       50,51,53,54,56,57,58,59,60,61,
       50,51,53,54,56,57,58,59,60,61
  };
      
  // 1: 72 binning to 9*4=48 bins
  for (int ibin=1; ibin<=input->GetNbinsX(); ibin++){    
    //std::cout<<" ibin "<<ibin<<" map_input[ibin-1] "<< map_input[ibin-1]<<" input_GetBinContent"<<input->GetBinContent(ibin)<< " noB_ent[map_input[ibin-1]-1] "<< noB_ent[map_input[ibin-1]-1]<<endl; 
    noB[map_input[ibin-1]-1] += input->GetBinContent(ibin)/noB_ent[map_input[ibin-1]-1];
    noBe[map_input[ibin-1]-1] += input->GetBinError(ibin)/noB_ent[map_input[ibin-1]-1];
  }
  for(int j=0;j<60;j++){
    //std::cout<< " j "<< j<<" noB[j] "<<noB[j]<<endl;
  }
  
  // 2: 48 bins to 13*4*4=208 bins
  for (int ibin=1; ibin<=output->GetNbinsX(); ibin++){    
    //std::cout<<" ibin "<<ibin<<" map_output[ibin-1] "<< map_output[ibin-1]<< " noB[map_output[ibin-1]-1] "<< noB[map_output[ibin-1]-1]<<endl;
    output->SetBinContent(ibin,noB[map_output[ibin-1]-1]);
    output->SetBinError(ibin,noBe[map_output[ibin-1]-1]);
  }

};

void combineHighLowHT_searchBin(TH1* Default, TH1* lowHT){

  for (int ibin=1; ibin<=Default->GetNbinsX(); ibin++){    
    //std::cout << ibin%10 << std::endl;
    if (ibin%10==1 || ibin%10==4 ){
    Default->SetBinContent(ibin,lowHT->GetBinContent(ibin));
    Default->SetBinError(ibin,lowHT->GetBinError(ibin));
    }
  }

};

void combineHighLowHT_QCDBin(TH1* Default, TH1* lowHT){

  for (int ibin=1; ibin<=Default->GetNbinsX(); ibin++){    
    //std::cout << ibin%13 << std::endl;
    if (ibin%13==1 || ibin%13==4 || ibin%13==7 ){
    Default->SetBinContent(ibin,lowHT->GetBinContent(ibin));
    Default->SetBinError(ibin,lowHT->GetBinError(ibin));
    }
  }

};

void makeNjNbCorr_searchBin(TH1* hist){

  /*
  double NjNbCorr[16]={
  1.08678, 1.1606, 1.21688, 1.38799,
  0.99422, 1.03512, 1.09677, 1.15551,
  0.960125, 0.99264, 1.04157, 1.22838,
  0.863833, 0.803388, 1.01109, 1.12302};
  */

  for (int ibin=1; ibin<=hist->GetNbinsX(); ibin++){    
    int index;   
    if(ibin<=110)
      index=int((ibin-1)/10);
    else
      index=int(11+(ibin-111)/8);
    //std::cout << index << std::endl;
    double corr = NjNbCorr[index];
    //std::cout << corr << std::endl;
    hist->SetBinContent(ibin,corr*hist->GetBinContent(ibin));
    hist->SetBinError(ibin,corr*hist->GetBinError(ibin));
  }
  
}

void makeNjNbCorr_QCDBin_HiDphi(TH1* hist){
  int index;
  for (int ibin=1; ibin<=hist->GetNbinsX(); ibin++){    
    if(ibin<=143)
      index=int((ibin-1)/13);
    else
      index=int(11+(ibin-144)/10);
    //std::cout <<" QCDBin_HiDphi "<<" ibin "<<ibin<<" index "<< index << std::endl;
    double corr = NjNbCorr_QCDHDP[index];
    //std::cout << corr << std::endl;
    hist->SetBinContent(ibin,corr*hist->GetBinContent(ibin));
    hist->SetBinError(ibin,corr*hist->GetBinError(ibin));
  }
  
}

void makeNjNbCorr_QCDBin_LowDphi(TH1* hist){
  int index;
  for (int ibin=1; ibin<=hist->GetNbinsX(); ibin++){    
    if(ibin<=143)
      index=int((ibin-1)/13);
    else
      index=int(11+(ibin-144)/10);
    //std::cout <<" QCDBin_LowDphi "<<" ibin "<<ibin<<" index "<< index << std::endl;

    
    //std::cout << index << std::endl;
    double corr = NjNbCorr_QCDLDP[index];
    //std::cout << corr << std::endl;
    hist->SetBinContent(ibin,corr*hist->GetBinContent(ibin));
    hist->SetBinError(ibin,corr*hist->GetBinError(ibin));
  }
  
}

void reformat(TH1* input, TH1* output){

  char tempname[200];  
  //string histname;
  //char hname[500];
  //std::cout << input->GetBinLowEdge(1) << std::endl;
  if (input->GetBinLowEdge(1)==1) {
    output->SetBins(input->GetNbinsX(),input->GetBinLowEdge(1)-0.5,input->GetBinLowEdge(input->GetNbinsX()+1)-0.5);
  } else {
    output->SetBins(input->GetNbinsX(),input->GetBinLowEdge(1)+0.5,input->GetBinLowEdge(input->GetNbinsX()+1)+0.5);
  }

  for (int ibin=1;ibin<=input->GetNbinsX();ibin++){    
    output->SetBinContent(ibin,input->GetBinContent(ibin));    
    output->SetBinError(ibin,input->GetBinError(ibin));    

  }
  //histname=output->GetName();
  // Search Binning
  if(output->GetNbinsX()==174){
    //std::cout<<"***********searchbin labels"<<endl;
    int MaxNb=0;
    int MaxHTMHT=0;
    int bi=0;
    for(int nji = 0; nji<5; ++nji){
      if(nji==0)
	MaxNb=3;
      else
	MaxNb=4;
      if(nji>2)
	MaxHTMHT=8;
      else
	MaxHTMHT=10;
      for(int nbi = 0; nbi<MaxNb; ++nbi){
	for(int hti = 0; hti<MaxHTMHT; ++hti){
	  int mhti =0;
	  bi++;
	  if(MaxHTMHT==10){
	    if(hti >=0 && hti <=2) mhti = 0;
	    else if(hti >=3 && hti <=5) mhti = 1;
	    else if(hti >=6 && hti <=7) mhti = 2;
	    else mhti = 3;
	  }
	  if(MaxHTMHT==8){
            if(hti >=0 && hti <=1) mhti = 0;
            else if(hti >=2 && hti <=3) mhti = 1;
            else if(hti >=4 && hti <=5) mhti = 2;
            else mhti = 3;
          }
	  
	  char binlabel[100];
	  //int bi = (nji) * 30 + nbi * MaxHTMHT + hti + 1;
	  //	  sprintf(binlabel, "nJet%d-nB%d-MHT%d-HT%d", nji, nbi, mhti, hti);
	  sprintf(binlabel, "NJets%d_BTags%d_MHT%d_HT%d", nji, nbi, mhti, hti);
	  //if(histname.find("Sys")!=string::npos){
	    //sprintf(hname,"%s_%s","htau",histname.c_str());
	    //sprintf(binlabel, "%s_%s_NJets%d_BTags%d_MHT%d_HT%d","htau",histname.c_str(), nji, nbi, mhti, hti);
	    //}
	  output -> GetXaxis() -> SetBinLabel(bi, binlabel);
	  //std::cout<<" bin "<<bi<<" label "<<binlabel<<endl;
	}
      }
    }
  }

  // QCD Binning
  if(output->GetNbinsX()==223){
    //std::cout<<"***********QCD labels"<<endl;
    int MaxNb=0;
    int MaxHTMHT=0;
    int bi=0;
    int mhti =0;
    for(int nji = 0; nji<5; ++nji){
      if(nji==0)
	MaxNb=3;
      else
	MaxNb=4;
      if(nji>2)
	MaxHTMHT=10;
      else
	MaxHTMHT=13;
      for(int nbi = 0; nbi<MaxNb; ++nbi){
	for(int hti = 0; hti<MaxHTMHT; ++hti){
	  bi++;
	  if(MaxHTMHT==13){
	    if(hti >=0 && hti <=2) mhti = -1;
	    else if(hti >=3 && hti <=5) mhti = 0;
	    else if(hti >=6 && hti <=8) mhti = 1;
	    else if(hti >=9 && hti <=10) mhti = 2;
	    else mhti = 3;
	  }
	  if(MaxHTMHT==10){
	    if(hti >=0 && hti <=1) mhti = -1;
	    else if(hti >=2 && hti <=3) mhti = 0;
	    else if(hti >=4 && hti <=5) mhti = 1;
	    else if(hti >=6 && hti <=7) mhti = 2;
	    else mhti = 3;
	  }
	  
	  char binlabel[100];
	  //int bi = (nji) * 30 + nbi * MaxHTMHT + hti + 1;
	  //sprintf(binlabel, "nJet%d-nB%d-MHT%d-HT%d", nji, nbi, mhti, hti);
	  sprintf(binlabel, "NJets%d_BTags%d_MHT%d_HT%d", nji, nbi, mhti, hti);
	  //if(histname.find("Sys")!=string::npos){
	  //sprintf(binlabel, "%s_%s_NJets%d_BTags%d_MHT%d_HT%d","htau",histname.c_str(), nji, nbi, mhti, hti);
	    //sprintf(hname,"%s_%s","htau",histname.c_str());
	    //sprintf(binlabel, "%s_NJets%d-BTags%d-MHT%d-HT%d",hname, nji, nbi, mhti, hti);
	  //}
	  output -> GetXaxis() -> SetBinLabel(bi, binlabel);
	  //std::cout<<" bin "<<bi<<" label "<<binlabel<<endl;
	}
      }
    }
  }

  
  output -> GetXaxis() -> LabelsOption("v");
  
};





void reLabel(TH1* output){

  char tempname[200];  
  string histname;
  //char hname[500];
  //std::cout << input->GetBinLowEdge(1) << std::endl;
 
  histname=output->GetName();
   
  // Search Binning
  if(output->GetNbinsX()==174){
    //    std::cout<<"***********searchbin labels"<<endl;
    int MaxNb=0;
    int MaxHTMHT=0;
    int bi=0;
    char hname[100];
    for(int nji = 0; nji<5; ++nji){
      if(nji==0)
	MaxNb=3;
      else
	MaxNb=4;
      if(nji>2)
	MaxHTMHT=8;
      else
	MaxHTMHT=10;
      for(int nbi = 0; nbi<MaxNb; ++nbi){
	for(int hti = 0; hti<MaxHTMHT; ++hti){
	  int mhti =0;
	  bi++;
	  if(MaxHTMHT==10){
	    if(hti >=0 && hti <=2) mhti = 0;
	    else if(hti >=3 && hti <=5) mhti = 1;
	    else if(hti >=6 && hti <=7) mhti = 2;
	    else mhti = 3;
	  }
	  if(MaxHTMHT==8){
            if(hti >=0 && hti <=1) mhti = 0;
            else if(hti >=2 && hti <=3) mhti = 1;
            else if(hti >=4 && hti <=5) mhti = 2;
            else mhti = 3;
          }
	  
	  char binlabel[100];
	  //int bi = (nji) * 30 + nbi * MaxHTMHT + hti + 1;
	  //	  sprintf(binlabel, "nJet%d-nB%d-MHT%d-HT%d", nji, nbi, mhti, hti);
	  //sprintf(binlabel, "NJets%d_BTags%d_MHT%d_HT%d", nji, nbi, mhti, hti);
	  //if(histname.find("Sys")!=string::npos){
	    //sprintf(hname,"%s_%s","htau",histname.c_str());
	  if(histname.find("nominal")!=string::npos){
	    sprintf(binlabel, "NJets%d_BTags%d_MHT%d_HT%d", nji, nbi, mhti, hti);
	    sprintf(hname,"totalPred_LL");
	  }
	  else if(histname.find("StatUncertainties")!=string::npos){
	    sprintf(binlabel, "%sNJets%d_BTags%d_MHT%d_HT%d","HadTauClosure", nji, nbi, mhti, hti);
	    sprintf(hname,"totalPredControlStat_LL");
	  }
	  else if(histname.find("closureUncertainty_adhoc")!=string::npos){
	    sprintf(binlabel, "%s_NJets%d_BTags%d","HadTauCorr", nji, nbi);
	    sprintf(hname,"totalPredAdhoc_HadTau");
	  }
	  else if(histname.find("closureUncertainty")!=string::npos){
	    sprintf(binlabel, "%sNJets%d_BTags%d_MHT%d_HT%d","HadTauClosure", nji, nbi, mhti, hti);
	    sprintf(hname,"totalPredNonClosure_HadTau");
	  }
	  else if(histname.find("BMistag")!=string::npos){
	    sprintf(binlabel, "HadTauBMistag");
	    if(histname.find("Up")!=string::npos)
	      sprintf(hname,"totalPredBMistagUp_HadTau");
	    else
	      sprintf(hname,"totalPredBMistagDown_HadTau");
	  }
	  else if(histname.find("MuRecoSys")!=string::npos){
	    sprintf(binlabel, "llp_muRecoSys");
	    if(histname.find("Up")!=string::npos)
	      sprintf(hname,"totalPredMuRecoSysUp_LL");
	    else
	      sprintf(hname,"totalPredMuRecoSysDown_LL");
	  }
	  else if(histname.find("MuIsoSys")!=string::npos){
	    sprintf(binlabel, "llp_muIsoSys");
	    if(histname.find("Up")!=string::npos)
	      sprintf(hname,"totalPredMuIsoSysUp_LL");
	    else
	      sprintf(hname,"totalPredMuIsoSysDown_LL");
	  }
	  else if(histname.find("MuRecoIso")!=string::npos){
	    sprintf(binlabel, "llp_muRecoStat");
	    if(histname.find("Up")!=string::npos)
	      sprintf(hname,"totalPredMuRecoStatUp_LL");
	    else
	      sprintf(hname,"totalPredMuRecoStatDown_LL");

	  }
	  else if(histname.find("JEC")!=string::npos){
	    sprintf(binlabel, "HadTauEScale");
	    if(histname.find("Up")!=string::npos)
	      sprintf(hname,"totalPredJECSysUp_HadTau");
	    else
	      sprintf(hname,"totalPredJECSysDown_HadTau");
	  }
	  else if(histname.find("MTSys")!=string::npos){
	    sprintf(binlabel, "NJets%d_BTags%d_MHT%d_HT%d", nji, nbi, mhti, hti);
	    if(histname.find("Up")!=string::npos)
	      sprintf(hname,"totalPredMTSysUp_HadTau");
	    else
	      sprintf(hname,"totalPredMTSysDown_HadTau");
	  }
	  else if(histname.find("MtEffStat")!=string::npos){
	    sprintf(binlabel, "NJets%d_BTags%d_MHT%d_HT%d", nji, nbi, mhti, hti);
	    sprintf(hname,"totalPredMtEffStat_HadTau");
	  }
	  else if(histname.find("IsoTrkVetoEffUncertaintyStat")!=string::npos){
	    sprintf(binlabel, "NJets%d_BTags%d_MHT%d_HT%d", nji, nbi, mhti, hti);
	    sprintf(binlabel, "llp_muIsoStat");
	    sprintf(hname,"totalPredIsoTrackStat_LL");
	  }
	  else if(histname.find("IsoTrkVetoEffUncertaintySys")!=string::npos){
	    sprintf(binlabel, "NJets%d_BTags%d_MHT%d_HT%d", nji, nbi, mhti, hti);
	    sprintf(hname,"totalPredIsoTrackSys_LL");
	  }
	  else if(histname.find("AccStat")!=string::npos){
	    sprintf(binlabel, "%s_%s_NJets%d_BTags%d_MHT%d_HT%d","llp","muAccStat", nji, nbi, mhti, hti);
	    sprintf(hname,"totalPredLepAccStat_LL");
	  }
	  else if(histname.find("AccSysPDF")!=string::npos){
	    sprintf(binlabel, "%s_%s_NJets%d_BTags%d_MHT%d_HT%d","llp","muAccSys", nji, nbi, mhti, hti);
	    if(histname.find("Up")!=string::npos)
	    sprintf(hname,"totalPredPDFUp_LL");
	    else
	      sprintf(hname,"totalPredPDFDown_LL");
	  }
	  else if(histname.find("AccSysScale")!=string::npos){
	    sprintf(binlabel, "%s_%s_NJets%d_BTags%d_MHT%d_HT%d","llp","muAccQsquareSys", nji, nbi, mhti, hti);
	    if(histname.find("Up")!=string::npos)
	      sprintf(hname,"totalPredScaleUp_LL");
	    else
	      sprintf(hname,"totalPredScaleDown_LL");
	  }
	  else if(histname.find("MuFromTauStat")!=string::npos){
	    sprintf(binlabel, "HadtauMuFromTau");
	    sprintf(hname,"totalPredMuFromTauStat_HadTau");
	  }
	  else if(histname.find("Dilepton")!=string::npos){
	    sprintf(binlabel,"HadTauDilep");
	    sprintf(hname,"totalPredDilep_HadTau");
	  }
	  else if(histname.find("TrigEff")!=string::npos){
	    sprintf(binlabel, "HadTauTrigSyst");
	    sprintf(hname,"totalPredTrigSyst_HadTau");
	  }

	    //}
	  output -> GetXaxis() -> SetBinLabel(bi, binlabel);
	  output ->SetName(hname);
	  //std::cout<<" bin "<<bi<<" label "<<binlabel<<endl;
	}
      }
    }
  }

  // QCD Binning
  if(output->GetNbinsX()==223){
    //    std::cout<<"***********QCD labels"<<endl;
    int MaxNb=0;
    int MaxHTMHT=0;
    int bi=0;
    int mhti =0;
    for(int nji = 0; nji<5; ++nji){
      if(nji==0)
	MaxNb=3;
      else
	MaxNb=4;
      if(nji>2)
	MaxHTMHT=10;
      else
	MaxHTMHT=13;
      for(int nbi = 0; nbi<MaxNb; ++nbi){
	for(int hti = 0; hti<MaxHTMHT; ++hti){
	  bi++;
	  if(MaxHTMHT==13){
	    if(hti >=0 && hti <=2) mhti = -1;
	    else if(hti >=3 && hti <=5) mhti = 0;
	    else if(hti >=6 && hti <=8) mhti = 1;
	    else if(hti >=9 && hti <=10) mhti = 2;
	    else mhti = 3;
	  }
	  if(MaxHTMHT==10){
	    if(hti >=0 && hti <=1) mhti = -1;
	    else if(hti >=2 && hti <=3) mhti = 0;
	    else if(hti >=4 && hti <=5) mhti = 1;
	    else if(hti >=6 && hti <=7) mhti = 2;
	    else mhti = 3;
	  }
	  
	  char binlabel[100];
	  //int bi = (nji) * 30 + nbi * MaxHTMHT + hti + 1;
	  //sprintf(binlabel, "nJet%d-nB%d-MHT%d-HT%d", nji, nbi, mhti, hti);
	  //sprintf(binlabel, "NJets%d_BTags%d_MHT%d_HT%d", nji, nbi, mhti, hti);
	  //if(histname.find("Sys")!=string::npos){
	  sprintf(binlabel, "%s_%s_NJets%d_BTags%d_MHT%d_HT%d","htau",histname.c_str(), nji, nbi, mhti, hti);
	    //sprintf(hname,"%s_%s","htau",histname.c_str());
	    //sprintf(binlabel, "%s_NJets%d-BTags%d-MHT%d-HT%d",hname, nji, nbi, mhti, hti);
	  //}
	  output -> GetXaxis() -> SetBinLabel(bi, binlabel);
	  //std::cout<<" bin "<<bi<<" label "<<binlabel<<endl;
	}
      }
    }
  }

  
  output -> GetXaxis() -> LabelsOption("v");
  
};
