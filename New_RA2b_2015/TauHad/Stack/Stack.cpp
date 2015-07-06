#include "Selection.h"

#include <iostream>
#include <cassert>
#include <vector>
#include <map>
#include <string>
#include <cstdlib>
#include <cstdio>
#include "THStack.h"
#include "TFile.h"
#include "TH1.h"
#include "TDirectory.h"



using namespace std;

class mainClass{

  // Some variables
  double tempvalue;
  char tempname[200];
  vector<double> T_scalevec, WJet_scalevec, TTbar_scalevec;
  vector<TFile *> T_inputfilevec,WJet_inputfilevec, TTbar_inputfilevec;
  map<int, string> cutname, histname, Hname;
  map<int, string> Ttype, WJettype, TTbartype;
  TFile *file, *file2, *file3;
  TH1D *temphist, *temphist2, *temphistI, *temphistII, *temphistIII;
  THStack * tempstack;
  TDirectory *cdtoitt, *cdtoit;

public:
mainClass(int luminosity=10000){ // luminosity is in /pb unit


  Selection * sel = new Selection();
  cutname = sel->cutName();



  WJettype[0]="allEvents";
  TTbartype[0]="allEvents";
  Ttype[0]="allEvents";

// .....................................................................................................................................................//
// Single Top Section
// .....................................................................................................................................................//

  //build a vector of scale factors
  //first load the cross sections into a vector
  vector<double> T_xs_vec;
  T_xs_vec.push_back(2.); // T_s
  T_xs_vec.push_back(103.4);  // T_t 
  T_xs_vec.push_back(35.);  // T_u 
  T_xs_vec.push_back(1.); // Tbar_s
  T_xs_vec.push_back(61.6);  // Tbar_t 
  T_xs_vec.push_back(35.);  // Tbar_u 


  const int tnHT = (int) T_xs_vec.size();   // Total number of HT bin samples

  for(int i=1; i<=tnHT ; i++){
    if(i==1)sprintf(tempname,"../../Results/results_T_s_.root");
    else if(i==2)sprintf(tempname,"../../Results/results_T_t_.root");
    else if(i==3)sprintf(tempname,"../../Results/results_T_u_.root");
    else if(i==4)sprintf(tempname,"../../Results/results_Tbar_s_.root");
    else if(i==5)sprintf(tempname,"../../Results/results_Tbar_t_.root");
    else if(i==6)sprintf(tempname,"../../Results/results_Tbar_u_.root");
    else{cout << " Error!! There are only 6 T ht binned sample " << endl;}
    file = new TFile(tempname, "R");
    sprintf(tempname,"allEvents/PreSel/MHT_PreSel_allEvents");
    tempvalue = (luminosity*T_xs_vec[i-1])/((* (TH1D* ) file->Get(tempname)).GetEntries());
    T_scalevec.push_back(tempvalue);
  }//end of loop over HTbins 
  std::cout << "T normalization scale factor determination done \n " << std::endl;




//..........................................//
// main histograms like HT, MHT, ...
//..........................................//

  // Load the files to a vector 
  // These are the HT, MHT, .. variables
  for(int i=1; i<=tnHT ; i++){
    if(i==1)sprintf(tempname,"../GenInfo_HadTauEstimation_T_s_.root");
    else if(i==2)sprintf(tempname,"../GenInfo_HadTauEstimation_T_t_.root");
    else if(i==3)sprintf(tempname,"../GenInfo_HadTauEstimation_T_u_.root");
    else if(i==4)sprintf(tempname,"../GenInfo_HadTauEstimation_Tbar_s_.root");
    else if(i==5)sprintf(tempname,"../GenInfo_HadTauEstimation_Tbar_t_.root");
    else if(i==6)sprintf(tempname,"../GenInfo_HadTauEstimation_Tbar_u_.root");
    else{cout << " Error!! There are only 6 T ht binned sample " << endl;}
    T_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins 


  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"GenInfo_HadTauEstimation_T_stacked.root");
  file = new TFile(tempname,"RECREATE");
  histname.clear();
  histname[0]="weight";
  histname[1]="HT";
  histname[2]="MHT";
  histname[3]="NJet";
  histname[4]="NBtag";
  histname[5]="DelPhi1";
  histname[6]="DelPhi2";
  histname[7]="DelPhi3";

  Hname.clear();
  Hname[0]="searchH";
  Hname[1]="searchH_b";
//  Hname[2]="QCD_Up";
//  Hname[3]="QCD_Low";
  
  for(int j=0; j< Hname.size(); j++){

    for(int i=0; i<tnHT ; i++){                                                  // loop over different HT bins

      sprintf(tempname,"%s",(Hname[j]).c_str());
      temphist = (TH1D *) T_inputfilevec.at(i)->Get(tempname)->Clone();
      temphist->Scale(T_scalevec[i]);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7
    sprintf(tempname,"%s",(Hname[j]).c_str());
    tempstack->Write(tempname);

    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");

  }




  for(map<int , string >::iterator itt=Ttype.begin(); itt!=Ttype.end();itt++){        // loop over different event types

    cdtoitt = file->mkdir((itt->second).c_str());
    cdtoitt->cd();

    for(map<int , string >::iterator it=cutname.begin(); it!=cutname.end();it++){   // loop over different cutnames

      cdtoit =  cdtoitt->mkdir((it->second).c_str());
      cdtoit->cd();

      for(int j=0; j<histname.size(); j++){                                        // loop over different histograms

        for(int i=0; i<tnHT ; i++){                                                  // loop over different HT bins

          //cout << "================================" << endl;
          //cout << "HT#: " <<i << ", WJtype: " << itt->second << ", cutname: " << it->second << ", hist#: " << j << endl;  
          sprintf(tempname,"%s/%s/%s_%s_%s",(itt->second).c_str(),(it->second).c_str(),(histname[j]).c_str(),(it->second).c_str(),(itt->second).c_str());
          temphist = (TH1D *) T_inputfilevec.at(i)->Get(tempname)->Clone();
          temphist->Scale(T_scalevec[i]);
          temphist->SetFillColor(i+2);
          tempstack->Add(temphist);

        }//end of loop over HTbins 1..7

        sprintf(tempname,"%s_%s_%s",histname[j].c_str(),(it->second).c_str(),(itt->second).c_str());
        tempstack->Write(tempname);

        delete tempstack;
        tempstack = new THStack("stack","Binned Sample Stack");

      }//end of loop over histograms

    }//end of loop over cutnames 

  }//end of loop over event types

  file->Close();
  printf("T main histograms stacked \n ");




// .....................................................................................................................................................//
// WJet Section
// .....................................................................................................................................................//

  //build a vector of scale factors
  //first load the cross sections into a vector
  vector<double> WJet_xs_vec;
  double k_w = 1.23;
  WJet_xs_vec.push_back(k_w*1817.0); // HT 100-200
  WJet_xs_vec.push_back(k_w*471.6);  // HT 200-400
  WJet_xs_vec.push_back(k_w*55.61);  // HT 400-600
  WJet_xs_vec.push_back(k_w*18.81);  // HT 600-Inf

  const int wjnHT = (int) WJet_xs_vec.size();   // Total number of HT bin samples

  for(int i=1; i<=wjnHT ; i++){
    if(i==1)sprintf(tempname,"../../Results/results_WJet_100_200_.root");
    else if(i==2)sprintf(tempname,"../../Results/results_WJet_200_400_.root");
    else if(i==3)sprintf(tempname,"../../Results/results_WJet_400_600_.root");
    else if(i==4)sprintf(tempname,"../../Results/results_WJet_600_inf_.root");
    else{cout << " Error!! There are only 4 WJet ht binned sample " << endl;}
    file = new TFile(tempname, "R");
    sprintf(tempname,"allEvents/PreSel/MHT_PreSel_allEvents");
    tempvalue = (luminosity*WJet_xs_vec[i-1])/((* (TH1D* ) file->Get(tempname)).GetEntries());
    WJet_scalevec.push_back(tempvalue);
  }//end of loop over HTbins 
  std::cout << "WJet normalization scale factor determination done \n " << std::endl;




//..........................................//
// main histograms like HT, MHT, ...
//..........................................//

  // Load the files to a vector 
  // These are the HT, MHT, .. variables
  for(int i=1; i<=wjnHT ; i++){
    if(i==1)sprintf(tempname,"../GenInfo_HadTauEstimation_WJet_100_200_.root");
    else if(i==2)sprintf(tempname,"../GenInfo_HadTauEstimation_WJet_200_400_.root");
    else if(i==3)sprintf(tempname,"../GenInfo_HadTauEstimation_WJet_400_600_.root");
    else if(i==4)sprintf(tempname,"../GenInfo_HadTauEstimation_WJet_600_inf_.root");
    else{cout << " Error!! There are only 4 WJet ht binned sample " << endl;}
    WJet_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins 


  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"GenInfo_HadTauEstimation_WJet_stacked.root");
  file = new TFile(tempname,"RECREATE");
  histname.clear();
  histname[0]="weight";
  histname[1]="HT";
  histname[2]="MHT";
  histname[3]="NJet";
  histname[4]="NBtag";
  histname[5]="DelPhi1";
  histname[6]="DelPhi2";
  histname[7]="DelPhi3";

  Hname.clear();
  Hname[0]="searchH";
  Hname[1]="searchH_b";
  Hname[2]="QCD_Up";
  Hname[3]="QCD_Low";
  
  for(int j=0; j< Hname.size(); j++){

    for(int i=0; i<wjnHT ; i++){                                                  // loop over different HT bins

      sprintf(tempname,"%s",(Hname[j]).c_str());
      temphist = (TH1D *) WJet_inputfilevec.at(i)->Get(tempname)->Clone();
      temphist->Scale(WJet_scalevec[i]);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7
    sprintf(tempname,"%s",(Hname[j]).c_str());
    tempstack->Write(tempname);

    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");

  }




  for(map<int , string >::iterator itt=WJettype.begin(); itt!=WJettype.end();itt++){        // loop over different event types

    cdtoitt = file->mkdir((itt->second).c_str());
    cdtoitt->cd();

    for(map<int , string >::iterator it=cutname.begin(); it!=cutname.end();it++){   // loop over different cutnames

      cdtoit =  cdtoitt->mkdir((it->second).c_str());
      cdtoit->cd();

      for(int j=0; j<histname.size(); j++){                                        // loop over different histograms

        for(int i=0; i<wjnHT ; i++){                                                  // loop over different HT bins

          //cout << "================================" << endl;
          //cout << "HT#: " <<i << ", WJtype: " << itt->second << ", cutname: " << it->second << ", hist#: " << j << endl;  
          sprintf(tempname,"%s/%s/%s_%s_%s",(itt->second).c_str(),(it->second).c_str(),(histname[j]).c_str(),(it->second).c_str(),(itt->second).c_str());
          temphist = (TH1D *) WJet_inputfilevec.at(i)->Get(tempname)->Clone();
          temphist->Scale(WJet_scalevec[i]);
          temphist->SetFillColor(i+2);
          tempstack->Add(temphist);

        }//end of loop over HTbins 1..7

        sprintf(tempname,"%s_%s_%s",histname[j].c_str(),(it->second).c_str(),(itt->second).c_str());
        tempstack->Write(tempname);

        delete tempstack;
        tempstack = new THStack("stack","Binned Sample Stack");

      }//end of loop over histograms

    }//end of loop over cutnames 

  }//end of loop over event types

  file->Close();
  printf("WJet main histograms stacked \n ");




//..........................................//
// Tau Response Template
//..........................................//

  // Load the files to a vector 
  // These are tau template files

  WJet_inputfilevec.clear();

  for(int i=1; i<=wjnHT ; i++){
    if(i==1)sprintf(tempname,"../HadTau_TauResponseTemplates_WJet_100_200_.root");
    else if(i==2)sprintf(tempname,"../HadTau_TauResponseTemplates_WJet_200_400_.root");
    else if(i==3)sprintf(tempname,"../HadTau_TauResponseTemplates_WJet_400_600_.root");
    else if(i==4)sprintf(tempname,"../HadTau_TauResponseTemplates_WJet_600_inf_.root");
    else{cout << " Error!! There are only 4 WJet ht binned sample " << endl;}
    WJet_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins 

  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"HadTau_TauResponseTemplates_WJet_stacked.root");
  file = new TFile(tempname,"RECREATE");

  histname.clear();
  histname[0]="hTauResp_0";
  histname[1]="hTauResp_1";
  histname[2]="hTauResp_2";
  histname[3]="hTauResp_3";
  histname[4]="hTauResp_0_xy";
  histname[5]="hTauResp_1_xy";
  histname[6]="hTauResp_2_xy";
  histname[7]="hTauResp_3_xy";
//  histname[4]="genHadTauPtHist";

  for(int j=0; j<histname.size(); j++){

    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<wjnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) WJet_inputfilevec.at(i)->Get(tempname)->Clone();
      temphist->Scale(WJet_scalevec[i]);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);
  
    }//end of loop over HTbins 1..7

    temphist = (TH1D *) tempstack->GetStack()->Last();
    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");


  }

  file->Close();
  printf("WJet tau response ready. \n ");



//..........................................//
// Muon From Tau 
//..........................................//

  // Load the files to a vector 
  // These are tau template files

  WJet_inputfilevec.clear();

  for(int i=1; i<=wjnHT ; i++){
    if(i==1)sprintf(tempname,"../Probability_Tau_mu_WJet_100_200_.root");
    else if(i==2)sprintf(tempname,"../Probability_Tau_mu_WJet_200_400_.root");
    else if(i==3)sprintf(tempname,"../Probability_Tau_mu_WJet_400_600_.root");
    else if(i==4)sprintf(tempname,"../Probability_Tau_mu_WJet_600_inf_.root");
    else{cout << " Error!! There are only 4 WJet ht binned sample " << endl;}
    WJet_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins 

  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"Probability_Tau_mu_WJet_stacked.root");
  file = new TFile(tempname,"RECREATE");

  histname.clear();
  histname[0]="hProb_Tau_mu";
  histname[1]="hTau_mu";
  histname[2]="hW_mu";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<wjnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) WJet_inputfilevec.at(i)->Get(tempname)->Clone();
      temphist->Scale(WJet_scalevec[i]);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7

    temphist = (TH1D *) tempstack->GetStack()->Last();
    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");


  }

  file->Close();
  printf("WJet muon from tau probability calculated. \n ");


//..........................................//
// Mistag rate 
//..........................................//

  // Load the files to a vector
  // These are tau template files

  WJet_inputfilevec.clear();

  for(int i=1; i<=wjnHT ; i++){
    if(i==1)sprintf(tempname,"../TauBtaggedRate_WJet_100_200_.root");
    else if(i==2)sprintf(tempname,"../TauBtaggedRate_WJet_200_400_.root");
    else if(i==3)sprintf(tempname,"../TauBtaggedRate_WJet_400_600_.root");
    else if(i==4)sprintf(tempname,"../TauBtaggedRate_WJet_600_inf_.root");
    else{cout << " Error!! There are only 4 WJet ht binned sample " << endl;}
    WJet_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins

  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"TauBtaggedRate_WJet_stacked.root");
  file = new TFile(tempname,"RECREATE");

  histname.clear();
  histname[0]="TauBtaggedRate";
  histname[1]="B_rate_tagged";
  histname[2]="B_rate_all";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<wjnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) WJet_inputfilevec.at(i)->Get(tempname)->Clone();
      temphist->Scale(WJet_scalevec[i]);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7

    temphist = (TH1D *) tempstack->GetStack()->Last();
    if(j==1)temphistI=(TH1D*)temphist->Clone();
    if(j==2)temphistII=(TH1D*)temphist->Clone();
    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");

  }
  temphistIII = static_cast<TH1D*>(temphistI->Clone("TauBtaggedRate"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("TauBtaggedRate");
  temphistIII->SetTitle("TauBtaggedRate");
  temphistIII->Write();


  file->Close();
  printf("WJet mistag rate calculated. \n ");



//..........................................//
// Acceptance  
//..........................................//

  // Load the files to a vector
  // These are tau template files

  WJet_inputfilevec.clear();

  for(int i=1; i<=wjnHT ; i++){
    if(i==1)sprintf(tempname,"../LostLepton2_MuonEfficienciesFromWJet_100_200_.root");
    else if(i==2)sprintf(tempname,"../LostLepton2_MuonEfficienciesFromWJet_200_400_.root");
    else if(i==3)sprintf(tempname,"../LostLepton2_MuonEfficienciesFromWJet_400_600_.root");
    else if(i==4)sprintf(tempname,"../LostLepton2_MuonEfficienciesFromWJet_600_inf_.root");
    else{cout << " Error!! There are only 4 WJet ht binned sample " << endl;}
    WJet_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins

  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"LostLepton2_MuonEfficienciesFromWJet_stacked.root");
  file = new TFile(tempname,"RECREATE");

  histname.clear();
  histname[0]="hAcc";
  histname[1]="hAccPass";
  histname[2]="hAccAll";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<wjnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) WJet_inputfilevec.at(i)->Get(tempname)->Clone();
      temphist->Scale(WJet_scalevec[i]);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7

    temphist = (TH1D *) tempstack->GetStack()->Last();
    if(j==1)temphistI=(TH1D*)temphist->Clone();
    if(j==2)temphistII=(TH1D*)temphist->Clone();
    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");


  }
  temphistIII = static_cast<TH1D*>(temphistI->Clone("hAcc"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("hAcc");
  temphistIII->SetTitle("hAcc");
  temphistIII->Write();

  file->Close();
  printf("WJet acceptance calculated. \n ");



//..........................................//
// IsoTrk  
//..........................................//


  // Load the files to a vector
  // These are tau template files

  WJet_inputfilevec.clear();

  for(int i=1; i<=wjnHT ; i++){
    if(i==1)sprintf(tempname,"../IsoEfficiencies_WJet_100_200_.root");
    else if(i==2)sprintf(tempname,"../IsoEfficiencies_WJet_200_400_.root");
    else if(i==3)sprintf(tempname,"../IsoEfficiencies_WJet_400_600_.root");
    else if(i==4)sprintf(tempname,"../IsoEfficiencies_WJet_600_inf_.root");
    else{cout << " Error!! There are only 4 WJet ht binned sample " << endl;}
    WJet_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins

  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"IsoEfficiencies_WJet_stacked.root");
  file = new TFile(tempname,"RECREATE");

  histname.clear();
  histname[0]="IsoEff";
  histname[1]="Iso_pass";
  histname[2]="Iso_all";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<wjnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) WJet_inputfilevec.at(i)->Get(tempname)->Clone();
      temphist->Scale(WJet_scalevec[i]);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7

    temphist = (TH1D *) tempstack->GetStack()->Last();
    if(j==1)temphistI=(TH1D*)temphist->Clone();
    if(j==2)temphistII=(TH1D*)temphist->Clone();
    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");

  }
  temphistIII = static_cast<TH1D*>(temphistI->Clone("IsoEff"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("IsoEff");
  temphistIII->SetTitle("IsoEff");
  temphistIII->Write();

  file->Close();
  printf("WJet IsoTrks calculated. \n ");





// .....................................................................................................................................................//
// TTbar Section
// .....................................................................................................................................................//

  //build a vector of scale factors
  //first load the cross sections into a vector
  vector<double> TTbar_xs_vec;

  TTbar_xs_vec.push_back(806.1); // 

  const int ttbarnHT = (int) TTbar_xs_vec.size();   // Total number of HT bin samples

  for(int i=1; i<=ttbarnHT ; i++){
    if(i==1)sprintf(tempname,"../../Results/results_TTbar_.root");
    else{cout << " Error!! There are only 1 TTbar ht binned sample " << endl;}
    file = new TFile(tempname, "R");
    sprintf(tempname,"allEvents/PreSel/MHT_PreSel_allEvents");
    tempvalue = (luminosity*TTbar_xs_vec[i-1])/((* (TH1D* ) file->Get(tempname)).GetEntries());
    TTbar_scalevec.push_back(tempvalue);
  }//end of loop over HTbins 
  std::cout << "TTbar normalization scale factor determination done \n " << std::endl;
  



//..........................................//
// main histograms like HT, MHT, ...
//..........................................//

  // Load the files to a vector 
  // These are the HT, MHT, .. variables
  for(int i=1; i<=ttbarnHT ; i++){
    if(i==1)sprintf(tempname,"../GenInfo_HadTauEstimation_TTbar_.root");
    else{cout << " Error!! There are only 1 TTbaret ht binned sample " << endl;}
    TTbar_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins 

  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"GenInfo_HadTauEstimation_TTbar_stacked.root");
  file = new TFile(tempname,"RECREATE");
  histname.clear();
  histname[0]="weight";
  histname[1]="HT";
  histname[2]="MHT";
  histname[3]="NJet";
  histname[4]="NBtag";
  histname[5]="DelPhi1";
  histname[6]="DelPhi2";
  histname[7]="DelPhi3";

  Hname.clear();
  Hname[0]="searchH";
  Hname[1]="searchH_b";
  Hname[2]="QCD_Up";
  Hname[3]="QCD_Low";
 
  for(int j=0; j< Hname.size(); j++){

    for(int i=0; i<ttbarnHT ; i++){                                                  // loop over different HT bins

      sprintf(tempname,"%s",(Hname[j]).c_str());
      temphist = (TH1D *) TTbar_inputfilevec.at(i)->Get(tempname)->Clone();
      temphist->Scale(TTbar_scalevec[i]);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7
    sprintf(tempname,"%s",(Hname[j]).c_str());
    tempstack->Write(tempname);

    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");

  }



  for(map<int , string >::iterator itt=TTbartype.begin(); itt!=TTbartype.end();itt++){        // loop over different event types

    cdtoitt = file->mkdir((itt->second).c_str());
    cdtoitt->cd();

    for(map<int , string >::iterator it=cutname.begin(); it!=cutname.end();it++){   // loop over different cutnames

      cdtoit =  cdtoitt->mkdir((it->second).c_str());
      cdtoit->cd();

      for(int j=0; j<histname.size(); j++){                                        // loop over different histograms

        for(int i=0; i<ttbarnHT ; i++){                                                  // loop over different HT bins

          //cout << "================================" << endl;
          //cout << "HT#: " <<i << ", TTbartype: " << itt->second << ", cutname: " << it->second << ", hist#: " << j << endl;  
          sprintf(tempname,"%s/%s/%s_%s_%s",(itt->second).c_str(),(it->second).c_str(),(histname[j]).c_str(),(it->second).c_str(),(itt->second).c_str());
          temphist = (TH1D *) TTbar_inputfilevec.at(i)->Get(tempname)->Clone();
          temphist->Scale(TTbar_scalevec[i]);
          temphist->SetFillColor(i+2);
          tempstack->Add(temphist);

        }//end of loop over HTbins 1..7

        sprintf(tempname,"%s_%s_%s",histname[j].c_str(),(it->second).c_str(),(itt->second).c_str());
        tempstack->Write(tempname);
        delete tempstack;
        tempstack = new THStack("stack","Binned Sample Stack");

      }//end of loop over histograms

    }//end of loop over cutnames 

  }//end of loop over event types

  file->Close();
  printf("TTbar main histograms stacked \n ");




//..........................................//
// Tau Response Template
//..........................................//

  // Load the files to a vector 
  // These are tau template files

  TTbar_inputfilevec.clear();

  for(int i=1; i<=ttbarnHT ; i++){
    if(i==1)sprintf(tempname,"../HadTau_TauResponseTemplates_TTbar_.root");
    else{cout << " Error!! There are only 1 TTbaret ht binned sample " << endl;}
    TTbar_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins 

  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"HadTau_TauResponseTemplates_TTbar_stacked.root");
  file = new TFile(tempname,"RECREATE");

  histname.clear();
  histname[0]="hTauResp_0";
  histname[1]="hTauResp_1";
  histname[2]="hTauResp_2";
  histname[3]="hTauResp_3";
  histname[4]="hTauResp_0_xy";
  histname[5]="hTauResp_1_xy";
  histname[6]="hTauResp_2_xy";
  histname[7]="hTauResp_3_xy";
//  histname[4]="genHadTauPtHist";

  for(int j=0; j<histname.size(); j++){

    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<ttbarnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) TTbar_inputfilevec.at(i)->Get(tempname)->Clone();
      temphist->Scale(TTbar_scalevec[i]);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);
  
    }//end of loop over HTbins 1..7

    temphist = (TH1D *) tempstack->GetStack()->Last();
    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");

  }

  file->Close();
  printf("TTbar tau response ready. \n ");



//..........................................//
// Muon From Tau 
//..........................................//

  // Load the files to a vector 
  // These are tau template files

  TTbar_inputfilevec.clear();

  for(int i=1; i<=ttbarnHT ; i++){
    if(i==1)sprintf(tempname,"../Probability_Tau_mu_TTbar_.root");
    else{cout << " Error!! There are only 1 TTbaret ht binned sample " << endl;}
    TTbar_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins 

  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"Probability_Tau_mu_TTbar_stacked.root");
  file = new TFile(tempname,"RECREATE");

  histname.clear();
  histname[0]="hProb_Tau_mu";
  histname[1]="hTau_mu";
  histname[2]="hW_mu";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<ttbarnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) TTbar_inputfilevec.at(i)->Get(tempname)->Clone();
      temphist->Scale(TTbar_scalevec[i]);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7

    temphist = (TH1D *) tempstack->GetStack()->Last();
    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");


  }

  file->Close();
  printf("TTbar muon from tau probability calculated. \n ");


//..........................................//
// Mistag rate 
//..........................................//

  // Load the files to a vector
  // These are tau template files

  TTbar_inputfilevec.clear();

  for(int i=1; i<=ttbarnHT ; i++){
    if(i==1)sprintf(tempname,"../TauBtaggedRate_TTbar_.root");
    else{cout << " Error!! There are only 1 TTbar ht binned sample " << endl;}
    TTbar_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins

  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"TauBtaggedRate_TTbar_stacked.root");
  file = new TFile(tempname,"RECREATE");

  histname.clear();
  histname[0]="TauBtaggedRate";
  histname[1]="B_rate_tagged";
  histname[2]="B_rate_all";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<ttbarnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) TTbar_inputfilevec.at(i)->Get(tempname)->Clone();
      temphist->Scale(TTbar_scalevec[i]);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7

    temphist = (TH1D *) tempstack->GetStack()->Last();
    if(j==1)temphistI=(TH1D*)temphist->Clone();
    if(j==2)temphistII=(TH1D*)temphist->Clone();
    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");


  }
  temphistIII = static_cast<TH1D*>(temphistI->Clone("TauBtaggedRate"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("TauBtaggedRate");
  temphistIII->SetTitle("TauBtaggedRate");
  temphistIII->Write();

  file->Close();
  printf("TTbar mistag rate calculated. \n ");




//..........................................//
// Acceptance  
//..........................................//

  // Load the files to a vector
  // These are tau template files

  TTbar_inputfilevec.clear();

  for(int i=1; i<=ttbarnHT ; i++){
    if(i==1)sprintf(tempname,"../LostLepton2_MuonEfficienciesFromTTbar_.root");
    else{cout << " Error!! There are only 1 TTbar ht binned sample " << endl;}
    TTbar_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins

  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"LostLepton2_MuonEfficienciesFromTTbar_stacked.root");
  file = new TFile(tempname,"RECREATE");

  histname.clear();
  histname[0]="hAcc";
  histname[1]="hAccPass";
  histname[2]="hAccAll";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<ttbarnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) TTbar_inputfilevec.at(i)->Get(tempname)->Clone();
      temphist->Scale(TTbar_scalevec[i]);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7

    temphist = (TH1D *) tempstack->GetStack()->Last();
    if(j==1)temphistI=(TH1D*)temphist->Clone();
    if(j==2)temphistII=(TH1D*)temphist->Clone();
    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");


  }
  temphistIII = static_cast<TH1D*>(temphistI->Clone("hAcc"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("hAcc");
  temphistIII->SetTitle("hAcc");
  temphistIII->Write();

  file->Close();
  printf("TTbar acceptance calculated. \n ");


//..........................................//
// IsoTrk  
//..........................................//


  // Load the files to a vector
  // These are tau template files

  TTbar_inputfilevec.clear();

  for(int i=1; i<=ttbarnHT ; i++){
    if(i==1)sprintf(tempname,"../IsoEfficiencies_TTbar_.root");
    else{cout << " Error!! There are only 4 TTbar ht binned sample " << endl;}
    TTbar_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins

  // Stack
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"IsoEfficiencies_TTbar_stacked.root");
  file = new TFile(tempname,"RECREATE");

  histname.clear();
  histname[0]="IsoEff";
  histname[1]="Iso_pass";
  histname[2]="Iso_all";

  for(int j=0; j<histname.size(); j++){

    if(j==0)continue; // Stacking probability histograms has no meaning.
    sprintf(tempname,"%s",(histname[j]).c_str());

    for(int i=0; i<ttbarnHT ; i++){ // loop over different HT bins

      temphist = (TH1D *) TTbar_inputfilevec.at(i)->Get(tempname)->Clone();
      temphist->Scale(TTbar_scalevec[i]);
      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);

    }//end of loop over HTbins 1..7

    temphist = (TH1D *) tempstack->GetStack()->Last();
    if(j==1)temphistI=(TH1D*)temphist->Clone();
    if(j==2)temphistII=(TH1D*)temphist->Clone();
    temphist->Write(tempname);
    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");

  }
  temphistIII = static_cast<TH1D*>(temphistI->Clone("IsoEff"));
  temphistIII->Divide(temphistI,temphistII,1,1,"B");
  temphistIII->SetName("IsoEff");
  temphistIII->SetTitle("IsoEff");
  temphistIII->Write();

  file->Close();
  printf("TTbar IsoTrks calculated. \n ");




// ..................................................................................................................................................... //
// Stack main histograms from TTbar and WJet and Single Top
// ..................................................................................................................................................... //

  // There are three contributors 1-TTbar and 2-WJet 3-T
  int NSamples=3;

  // A vector that contains all the samples
  vector<TFile*> sample_inputfilevec;

  THStack * tempstack2 = new THStack("stack","Binned Sample Stack");

  // Load the files to a vector 
  // These are the HT, MHT, .. variables
  for(int i=1; i<=NSamples ; i++){
    if(i==1)sprintf(tempname,"GenInfo_HadTauEstimation_TTbar_stacked.root");
    else if(i==2)sprintf(tempname,"GenInfo_HadTauEstimation_WJet_stacked.root");
    else if(i==3)sprintf(tempname,"GenInfo_HadTauEstimation_T_stacked.root");
    else{cout << " Error!! There are only 3 contributors! " << endl;}
    sample_inputfilevec.push_back(TFile::Open(tempname,"R"));
  }//end of loop over HTbins 

  // Stack
  delete tempstack;
  tempstack = new THStack("stack","Binned Sample Stack");
  sprintf(tempname,"GenInfo_HadTauEstimation_stacked.root");
  file = new TFile(tempname,"RECREATE");
  histname.clear();
  histname[0]="weight";
  histname[1]="HT";
  histname[2]="MHT";
  histname[3]="NJet";
  histname[4]="NBtag";
  histname[5]="DelPhi1";
  histname[6]="DelPhi2";
  histname[7]="DelPhi3";

  Hname.clear();
  Hname[0]="searchH";
  Hname[1]="searchH_b";
//  Hname[2]="QCD_Up";
//  Hname[3]="QCD_Low";

  for(int j=0; j< Hname.size(); j++){

    for(int i=0; i<NSamples ; i++){                                                  // loop over different HT bins

      sprintf(tempname,"%s",(Hname[j]).c_str());
      tempstack2 = (THStack *) sample_inputfilevec.at(i)->Get(tempname)->Clone();
      temphist = (TH1D*)tempstack2->GetStack()->Last();

      temphist->SetFillColor(i+2);
      tempstack->Add(temphist);



    }//end of loop over HTbins 1..7
    sprintf(tempname,"%s",(Hname[j]).c_str());
    tempstack->Write(tempname);

    delete tempstack;
    tempstack = new THStack("stack","Binned Sample Stack");

  }



  for(map<int , string >::iterator itt=WJettype.begin(); itt!=WJettype.end();itt++){        // loop over different event types

    cdtoitt = file->mkdir((itt->second).c_str());
    cdtoitt->cd();

    for(map<int , string >::iterator it=cutname.begin(); it!=cutname.end();it++){   // loop over different cutnames

      cdtoit =  cdtoitt->mkdir((it->second).c_str());
      cdtoit->cd();


      for(int j=0; j<histname.size(); j++){                                        // loop over different histograms

        for(int i=0; i<NSamples ; i++){                                                  // loop over different HT bins

          //cout << "================================" << endl;
          //cout << "HT#: " <<i << ", WJettype: " << itt->second << ", cutname: " << it->second << ", hist#: " << j << endl;  
          sprintf(tempname,"%s/%s/%s_%s_%s",(itt->second).c_str(),(it->second).c_str(),(histname[j]).c_str(),(it->second).c_str(),(itt->second).c_str());
          tempstack2 = (THStack *) sample_inputfilevec.at(i)->Get(tempname)->Clone();
          temphist = (TH1D*)tempstack2->GetStack()->Last(); 
          
          temphist->SetFillColor(i+2);
          tempstack->Add(temphist);

        }//end of loop over HTbins 1..7

        sprintf(tempname,"%s_%s_%s",histname[j].c_str(),(it->second).c_str(),(itt->second).c_str());
        tempstack->Write(tempname);
        delete tempstack;
        tempstack = new THStack("stack","Binned Sample Stack");

      }//end of loop over histograms

    }//end of loop over cutnames 

  }//end of loop over event types

  file->Close();
  printf("All samples main histograms stacked \n ");





// ..................................................................................................................................................... //
// Combine all tau response templates
// ..................................................................................................................................................... //

  // Add all the tau templates from different sources.
  histname.clear();
  histname[0]="hTauResp_0";
  histname[1]="hTauResp_1";
  histname[2]="hTauResp_2";
  histname[3]="hTauResp_3";
  histname[4]="hTauResp_0_xy";
  histname[5]="hTauResp_1_xy";
  histname[6]="hTauResp_2_xy";
  histname[7]="hTauResp_3_xy";

  // Open the files to read
  sprintf(tempname,"HadTau_TauResponseTemplates_TTbar_stacked.root");
  file = new TFile(tempname,"R");
  sprintf(tempname,"HadTau_TauResponseTemplates_WJet_stacked.root");
  file2 = new TFile(tempname,"R");

  // Open a file to write
  sprintf(tempname,"HadTau_TauResponseTemplates_stacked.root");
  file3 = new TFile(tempname,"RECREATE");

  for(int j=0; j<histname.size(); j++){ 
  
  sprintf(tempname,"%s",(histname[j]).c_str());
  temphist = (TH1D *) file->Get(tempname)->Clone();
  temphist2 = (TH1D *) file2->Get(tempname)->Clone();

  temphist->Add(temphist,temphist2,1,1);
  

  // Normalize the response distributions to get the probability density
  if( temphist->Integral("width") > 0. ){
    temphist->Scale(1./temphist->Integral("width"));
  }

  temphist->Write();

  
  }
  file3->Close();
  file2->Close();
  file->Close();






// ..................................................................................................................................................... //
// Calcualte the rate of muons coming from tau
// ..................................................................................................................................................... //

  // Open the files to read
  sprintf(tempname,"Probability_Tau_mu_TTbar_stacked.root");
  file = new TFile(tempname,"R");
  sprintf(tempname,"Probability_Tau_mu_WJet_stacked.root");
  file2 = new TFile(tempname,"R");

  // Open a file to write
  sprintf(tempname,"Probability_Tau_mu_stacked.root");
  file3 = new TFile(tempname,"RECREATE");


  histname.clear();
  histname[0]="hTau_mu";
  histname[1]="hW_mu";

  for(int j=0; j<histname.size(); j++){

    sprintf(tempname,"%s",(histname[j]).c_str());
    temphist = (TH1D *) file->Get(tempname)->Clone();
    temphist2 = (TH1D *) file2->Get(tempname)->Clone();

    temphist->Add(temphist,temphist2,1,1);

  temphist->Write();

  }
  file3->Close();
  file2->Close();
  file->Close();

  // Open a file to write
  sprintf(tempname,"Probability_Tau_mu_stacked.root");
  file2 = new TFile(tempname,"R");
  file = new TFile(tempname,"UPDATE");
  
  sprintf(tempname,"hTau_mu");
  temphist = (TH1D *) file->Get(tempname)->Clone();
  sprintf(tempname,"hW_mu");
  temphist2 = (TH1D *) file2->Get(tempname)->Clone();

  temphist->Divide(temphist,temphist2,1,1,"B");
  temphist->SetName("hProb_Tau_mu");
  temphist->SetTitle("hProb_Tau_mu");
  temphist->Write();

  file->Close();
  file2->Close();



// ........................................... //
// Calcualte TauBtaggedRate
// ........................................... //

  // Open the files to read
  sprintf(tempname,"TauBtaggedRate_TTbar_stacked.root");
  file = new TFile(tempname,"R");
  sprintf(tempname,"TauBtaggedRate_WJet_stacked.root");
  file2 = new TFile(tempname,"R");

  // Open a file to write
  sprintf(tempname,"TauBtaggedRate_stacked.root");
  file3 = new TFile(tempname,"RECREATE");


  histname.clear();
  histname[0]="B_rate_tagged";
  histname[1]="B_rate_all";


  for(int j=0; j<histname.size(); j++){

    sprintf(tempname,"%s",(histname[j]).c_str());
    temphist = (TH1D *) file->Get(tempname)->Clone();
    temphist2 = (TH1D *) file2->Get(tempname)->Clone();

    temphist->Add(temphist,temphist2,1,1);

  temphist->Write();

  }
  file3->Close();
  file2->Close();
  file->Close();

  // Open a file to write
  sprintf(tempname,"TauBtaggedRate_stacked.root");
  file2 = new TFile(tempname,"R");
  file = new TFile(tempname,"UPDATE");
  
  sprintf(tempname,"B_rate_tagged");
  temphist = (TH1D *) file->Get(tempname)->Clone();
  sprintf(tempname,"B_rate_all");
  temphist2 = (TH1D *) file2->Get(tempname)->Clone();

  temphist->Divide(temphist,temphist2,1,1,"B");
  temphist->SetName("TauBtaggedRate");
  temphist->SetTitle("TauBtaggedRate");
  temphist->Write();

  file->Close();
  file2->Close();




// ........................................... //
// Calcualte Acceptance
// ........................................... //

  // Open the files to read
  sprintf(tempname,"LostLepton2_MuonEfficienciesFromTTbar_stacked.root");
  file = new TFile(tempname,"R");
  sprintf(tempname,"LostLepton2_MuonEfficienciesFromWJet_stacked.root");
  file2 = new TFile(tempname,"R");

  // Open a file to write
  sprintf(tempname,"LostLepton2_MuonEfficienciesFromstacked.root");
  file3 = new TFile(tempname,"RECREATE");


  histname.clear();
  histname[0]="hAccPass";
  histname[1]="hAccAll";


  for(int j=0; j<histname.size(); j++){

    sprintf(tempname,"%s",(histname[j]).c_str());
    temphist = (TH1D *) file->Get(tempname)->Clone();
    temphist2 = (TH1D *) file2->Get(tempname)->Clone();

    temphist->Add(temphist,temphist2,1,1);

  temphist->Write();

  }
  file3->Close();
  file2->Close();
  file->Close();

  // Open a file to write
  sprintf(tempname,"LostLepton2_MuonEfficienciesFromstacked.root");
  file2 = new TFile(tempname,"R");
  file = new TFile(tempname,"UPDATE");
  
  sprintf(tempname,"hAccPass");
  temphist = (TH1D *) file->Get(tempname)->Clone();
  sprintf(tempname,"hAccAll");
  temphist2 = (TH1D *) file2->Get(tempname)->Clone();

  temphist->Divide(temphist,temphist2,1,1,"B");
  temphist->SetName("hAcc");
  temphist->SetTitle("hAcc");
  temphist->Write();

  file->Close();
  file2->Close();



// ........................................... //
// Calcualte IsoTrk
// ........................................... //

  // Open the files to read
  sprintf(tempname,"IsoEfficiencies_TTbar_stacked.root");
  file = new TFile(tempname,"R");
  sprintf(tempname,"IsoEfficiencies_WJet_stacked.root");
  file2 = new TFile(tempname,"R");

  // Open a file to write
  sprintf(tempname,"IsoEfficiencies_stacked.root");
  file3 = new TFile(tempname,"RECREATE");


  histname.clear();
  histname[0]="Iso_pass";
  histname[1]="Iso_all";


  for(int j=0; j<histname.size(); j++){

    sprintf(tempname,"%s",(histname[j]).c_str());
    temphist = (TH1D *) file->Get(tempname)->Clone();
    temphist2 = (TH1D *) file2->Get(tempname)->Clone();

    temphist->Add(temphist,temphist2,1,1);

  temphist->Write();

  }
  file3->Close();
  file2->Close();
  file->Close();

  // Open a file to write
  sprintf(tempname,"IsoEfficiencies_stacked.root");
  file2 = new TFile(tempname,"R");
  file = new TFile(tempname,"UPDATE");
  
  sprintf(tempname,"Iso_pass");
  temphist = (TH1D *) file->Get(tempname)->Clone();
  sprintf(tempname,"Iso_all");
  temphist2 = (TH1D *) file2->Get(tempname)->Clone();

  temphist->Divide(temphist,temphist2,1,1,"B");
  temphist->SetName("IsoEff");
  temphist->SetTitle("IsoEff");
  temphist->Write();

  file->Close();
  file2->Close();




} // End of the constructor 

}; // End of the class 

int main(int argc, char *argv[]){

/////////////////////////////////////
if (argc != 2)
{
std::cout << "Please enter something like: ./Stack \"5000\" " << std::endl;
return EXIT_FAILURE;
}
const string lum = argv[1];
/////////////////////////////////////

int luminosity = atoi(lum.c_str());

mainClass mainObj(luminosity);

}
