#include <iostream>
#include <iomanip>
#include <vector>
//#include <cmath>
void PlotPull_Simon() {

  //  compareResponse("hist_taugun_aditee_LPC_Dec10.root", "hist_taugun_aditee_Reweighted.root", "hTauResp_0",0,0,1,1);
  //compareResponse("hist_taugun_aditee_LPC_Dec10.root", "hist_taugun_aditee_Reweighted.root", "hTauResp_1",0,0,1,1);
  //compareResponse("hist_taugun_aditee_LPC_Dec10.root", "hist_taugun_aditee_Reweighted.root", "hTauResp_2",0,0,1,1);
  //compareResponse("hist_taugun_aditee_LPC_Dec10.root", "hist_taugun_aditee_Reweighted.root", "hTauResp_3",0,0,1,1);
  //compareResponse("hist_taugun_aditee_LPC_Dec10.root", "hist_taugun_aditee_Reweighted.root", "hGenPt_0",0,0,1,1);
  //compareResponse("hist_taugun_aditee_LPC_Dec10.root", "hist_taugun_aditee_Reweighted.root", "hGenPt_1",0,0,1,1);
  //compareResponse("hist_taugun_aditee_LPC_Dec10.root", "hist_taugun_aditee_Reweighted.root", "hGenPt_2",0,0,1,1);
  //  compareResponse("hist_taugun_aditee_LPC_Dec10.root", "hist_taugun_aditee_Reweighted.root", "hGenPt_3",0,0,1,1);

  //compareResponse("ARElog90_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee_Reweighted.root", "hGenPt_0",0,0,1,1);
 // compareResponse("ARElog90_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee_Reweighted.root", "hGenPt_1",0,0,1,1);
 // compareResponse("ARElog90_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee_Reweighted.root", "hGenPt_2",0,0,1,1);
  compareResponse("ARElog90_GenInfo_HadTauEstimation_stacked.root", "ARElog91_HadTauEstimation_stacked.root", "searchH_b",0,0,1,1,36.35);
  //  compareResponse("ARElog90_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee_Reweighted.root", "hTauResp_0",0,0,1,1);
  //compareResponse("ARElog90_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee_Reweighted.root", "hTauResp_1",0,0,1,1);
  //compareResponse("ARElog90_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee_Reweighted.root", "hTauResp_2",0,0,1,1);
  //compareResponse("ARElog90_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee_Reweighted.root", "hTauResp_3",0,0,1,1);
    

//compareResponse("ARElog69_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee.root", "hGenPt",0,0,1,1);


}

//void compareResponse(char* ffast, char* ffull, char *histname, bool logy, bool logx,double cNo){
void compareResponse(char* WJetTTbarMC, char* Wgun, char *histname, bool logy, bool logx, bool overlay,bool normalized,double lumi){
  gStyle->SetOptStat(1111);
  gStyle->SetOptTitle(0);
    
  int col1 = kRed;
  int col2 = kBlue;
  int i=0;
    
  char hname[500], htit[500];
  TH1D * GenHist_input, * EstHist_input;
  THStack *tempstack;
 
  TFile *_file0 = TFile::Open(WJetTTbarMC);
  TFile *_file1 = TFile::Open(Wgun);
    
  //  _file0->cd();
  //TDirectory *demo1    = (TDirectory*)_file0->FindObjectAny("demo");
  //_file1->cd();
  //TDirectory *demo2    = (TDirectory*)_file1->FindObjectAny("demo");
    
    
  //sprintf(hname, "%s", histname); //string "hname" is set to the content of histname 
    
  //  sprintf(htit, "WJetTTbarMC vs Wgun");
  sprintf(hname, "DirectVsPrediction");
  //  if(histname=="hGenPt_0")    
  // sprintf(hname, "WJetTTbar vs Reweighted(20-30GeV");   
  //else if(histname=="hGenPt_1")    
  //  sprintf(hname, "WJetTTbar vs Reweighted(30-50GeV");
  //else if(histname=="hGenPt_2")    
  //  sprintf(hname, "WJetTTbar vs Reweighted(50-100GeV");
  //else
  //  sprintf(hname, "WJetTTbar vs Reweighted(Above 100GeV");
  //  

  //  char* RenameBins(int x){
  //char *binString[174]={"nJet0_nB0_MHT0_HT0","nJet0_nB0_MHT0_HT1","nJet0_nB0_MHT0_HT2","nJet0_nB0_MHT1_HT3","nJet0_nB0_MHT1_HT4","nJet0_nB0_MHT1_HT5","nJet0_nB0_MHT2_HT6","nJet0_nB0_MHT2_HT7","nJet0_nB0_MHT3_HT8","nJet0_nB0_MHT3_HT9","nJet0_nB1_MHT0_HT0","nJet0_nB1_MHT0_HT1","nJet0_nB1_MHT0_HT2","nJet0_nB1_MHT1_HT3","nJet0_nB1_MHT1_HT4","nJet0_nB1_MHT1_HT5","nJet0_nB1_MHT2_HT6","nJet0_nB1_MHT2_HT7","nJet0_nB1_MHT3_HT8","nJet0_nB1_MHT3_HT9","nJet0_nB2_MHT0_HT0","nJet0_nB2_MHT0_HT1","nJet0_nB2_MHT0_HT2","nJet0_nB2_MHT1_HT3","nJet0_nB2_MHT1_HT4","nJet0_nB2_MHT1_HT5","nJet0_nB2_MHT2_HT6","nJet0_nB2_MHT2_HT7","nJet0_nB2_MHT3_HT8","nJet0_nB2_MHT3_HT9","nJet1_nB0_MHT0_HT0","nJet1_nB0_MHT0_HT1","nJet1_nB0_MHT0_HT2","nJet1_nB0_MHT1_HT3","nJet1_nB0_MHT1_HT4","nJet1_nB0_MHT1_HT5","nJet1_nB0_MHT2_HT6","nJet1_nB0_MHT2_HT7","nJet1_nB0_MHT3_HT8","nJet1_nB0_MHT3_HT9","nJet1_nB1_MHT0_HT0","nJet1_nB1_MHT0_HT1","nJet1_nB1_MHT0_HT2","nJet1_nB1_MHT1_HT3","nJet1_nB1_MHT1_HT4","nJet1_nB1_MHT1_HT5","nJet1_nB1_MHT2_HT6","nJet1_nB1_MHT2_HT7","nJet1_nB1_MHT3_HT8","nJet1_nB1_MHT3_HT9","nJet1_nB2_MHT0_HT0","nJet1_nB2_MHT0_HT1","nJet1_nB2_MHT0_HT2","nJet1_nB2_MHT1_HT3","nJet1_nB2_MHT1_HT4","nJet1_nB2_MHT1_HT5","nJet1_nB2_MHT2_HT6","nJet1_nB2_MHT2_HT7","nJet1_nB2_MHT3_HT8","nJet1_nB2_MHT3_HT9","nJet1_nB3_MHT0_HT0","nJet1_nB3_MHT0_HT1","nJet1_nB3_MHT0_HT2","nJet1_nB3_MHT1_HT3","nJet1_nB3_MHT1_HT4","nJet1_nB3_MHT1_HT5","nJet1_nB3_MHT2_HT6","nJet1_nB3_MHT2_HT7","nJet1_nB3_MHT3_HT8","nJet1_nB3_MHT3_HT9","nJet2_nB0_MHT0_HT0","nJet2_nB0_MHT0_HT1","nJet2_nB0_MHT0_HT2","nJet2_nB0_MHT1_HT3","nJet2_nB0_MHT1_HT4","nJet2_nB0_MHT1_HT5","nJet2_nB0_MHT2_HT6","nJet2_nB0_MHT2_HT7","nJet2_nB0_MHT3_HT8","nJet2_nB0_MHT3_HT9","nJet2_nB1_MHT0_HT0","nJet2_nB1_MHT0_HT1","nJet2_nB1_MHT0_HT2","nJet2_nB1_MHT1_HT3","nJet2_nB1_MHT1_HT4","nJet2_nB1_MHT1_HT5","nJet2_nB1_MHT2_HT6","nJet2_nB1_MHT2_HT7","nJet2_nB1_MHT3_HT8","nJet2_nB1_MHT3_HT9","nJet2_nB2_MHT0_HT0","nJet2_nB2_MHT0_HT1","nJet2_nB2_MHT0_HT2","nJet2_nB2_MHT1_HT3","nJet2_nB2_MHT1_HT4","nJet2_nB2_MHT1_HT5","nJet2_nB2_MHT2_HT6","nJet2_nB2_MHT2_HT7","nJet2_nB2_MHT3_HT8","nJet2_nB2_MHT3_HT9","nJet2_nB3_MHT0_HT0","nJet2_nB3_MHT0_HT1","nJet2_nB3_MHT0_HT2","nJet2_nB3_MHT1_HT3","nJet2_nB3_MHT1_HT4","nJet2_nB3_MHT1_HT5","nJet2_nB3_MHT2_HT6","nJet2_nB3_MHT2_HT7","nJet2_nB3_MHT3_HT8","nJet2_nB3_MHT3_HT9","nJet3_nB0_MHT0_HT0","nJet3_nB0_MHT0_HT1","nJet3_nB0_MHT1_HT2","nJet3_nB0_MHT1_HT3","nJet3_nB0_MHT2_HT4","nJet3_nB0_MHT2_HT5","nJet3_nB0_MHT3_HT6","nJet3_nB0_MHT3_HT7","nJet3_nB1_MHT0_HT0","nJet3_nB1_MHT0_HT1","nJet3_nB1_MHT1_HT2","nJet3_nB1_MHT1_HT3","nJet3_nB1_MHT2_HT4","nJet3_nB1_MHT2_HT5","nJet3_nB1_MHT3_HT6","nJet3_nB1_MHT3_HT7","nJet3_nB2_MHT0_HT0","nJet3_nB2_MHT0_HT1","nJet3_nB2_MHT1_HT2","nJet3_nB2_MHT1_HT3","nJet3_nB2_MHT2_HT4","nJet3_nB2_MHT2_HT5","nJet3_nB2_MHT3_HT6","nJet3_nB2_MHT3_HT7","nJet3_nB3_MHT0_HT0","nJet3_nB3_MHT0_HT1","nJet3_nB3_MHT1_HT2","nJet3_nB3_MHT1_HT3","nJet3_nB3_MHT2_HT4","nJet3_nB3_MHT2_HT5","nJet3_nB3_MHT3_HT6","nJet3_nB3_MHT3_HT7","nJet4_nB0_MHT0_HT0","nJet4_nB0_MHT0_HT1","nJet4_nB0_MHT1_HT2","nJet4_nB0_MHT1_HT3","nJet4_nB0_MHT2_HT4","nJet4_nB0_MHT2_HT5","nJet4_nB0_MHT3_HT6","nJet4_nB0_MHT3_HT7","nJet4_nB1_MHT0_HT0","nJet4_nB1_MHT0_HT1","nJet4_nB1_MHT1_HT2","nJet4_nB1_MHT1_HT3","nJet4_nB1_MHT2_HT4","nJet4_nB1_MHT2_HT5","nJet4_nB1_MHT3_HT6","nJet4_nB1_MHT3_HT7","nJet4_nB2_MHT0_HT0","nJet4_nB2_MHT0_HT1","nJet4_nB2_MHT1_HT2","nJet4_nB2_MHT1_HT3","nJet4_nB2_MHT2_HT4","nJet4_nB2_MHT2_HT5","nJet4_nB2_MHT3_HT6","nJet4_nB2_MHT3_HT7","nJet4_nB3_MHT0_HT0","nJet4_nB3_MHT0_HT1","nJet4_nB3_MHT1_HT2","nJet4_nB3_MHT1_HT3","nJet4_nB3_MHT2_HT4","nJet4_nB3_MHT2_HT5","nJet4_nB3_MHT3_HT6","nJet4_nB3_MHT3_HT7"};
  //return binString[x-1];
  //}
//





  double NjNbCorr[19]={1.11998,1.02071,0.899496,1.00848,1.03618,1.03088,1.21098,0.960755,0.993052,1.02152,1.02597,0.952303,0.984147,0.975219,1.06904,0.788829,0.863781,0.973593,1.02607};
      
  tempstack=(THStack*)_file0->Get(histname)->Clone();
  TH1F *h1 = (TH1F*) tempstack->GetStack()->Last();

  tempstack=(THStack*)_file1->Get(histname)->Clone();
  //  TH1F *h2 = (TH1F*) tempstack->GetStack()->Last();
  TH1F *h2BeforeNjNb = (TH1F*) tempstack->GetStack()->Last();
  TH1F *h2=(TH1F*)h2BeforeNjNb->Clone("h2");
  int NJ,NB,HTMHT;
  /*
     for(int j=1;j<=174;j++){
    //    std::cout<<" Raname Bins "<< RenameBins(j)<<endl;
    h2->SetBinError(j,h2BeforeNjNb->GetBinError(j));
    if(j<=10){
         
      h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[0]);
    }
    else if(j<=20){
      h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[1]);
      }

    else if(j<=30){
      h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[2]);
       }
    else if(j<=40){
      h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[3]);
      }
    else if(j<=50){
      h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[4]);
      }
    else if(j<=60){
      h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[5]);
      }
    else if(j<=70){
      h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[6]);
      }
    else if(j<=80){
      h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[7]);
      }
    else if(j<=90){
      h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[8]);
      }
    else if(j<=100){
      h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[9]);
      }
    else if(j<=110){
      h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[10]);
    }
    else if(j<=118){
      h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[11]);
    }
    else if(j<=126){
      h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[12]);
    }
    else if(j<=134){
      h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[13]);
    }
    else if(j<=142){
      h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[14]);
    }
    else if(j<=150){
      h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[15]);
    }
    else if(j<=158){
      h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[16]);
    }
    else if(j<=166){
      h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[17]);
    }
    else if(j<=174){
      h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[18]); 
    }
  }
  */
  h1->Scale(lumi/3);
  h2->Scale(lumi/3);
  //std::cout<<" bin_number "<<j<<" Direct(scaled to 36.35/fb) "<<h1->GetBinContent(j)<<" Pred(scaled to 36.35/fb) "<<h2->GetBinContent(j)<<endl;
   
  TH1F *hDirUponPred=(TH1F*)h1->Clone("hDirUponPred");
  hDirUponPred->Divide(h1,h2,1,1," ");


  TH1F *One_NoError=(TH1F*)h1->Clone("One_NoError");
  for (int ibin=0; ibin<One_NoError->GetNbinsX()+2; ibin++){
    One_NoError->SetBinContent(ibin,1.);
    One_NoError->SetBinError(ibin,0.);
  }


  TH1F *hClosure=(TH1F*)h1->Clone("hClosure");
  hClosure->Divide(h1,h2,1,1," ");
  hClosure->Add(One_NoError,-1.);


  for (int j=0; j<hClosure->GetNbinsX()+2; j++){ // scan including underflow and overflow bins
    if(hClosure->GetBinError(j) < 0.00000001){
      hClosure->SetBinContent(j, 0);
    }else{
      hClosure->SetBinContent(j,hClosure->GetBinContent(j)/hClosure->GetBinError(j));
    }         
    hClosure->SetBinError(j,0.);
    //std::cout<<" bin_number "<<j<<" Direct(scaled to 36.35/fb) "<<h1->GetBinContent(j)<<" Pred(scaled to 36.35/fb) "<<h2->GetBinContent(j)<<" Direct/Pred - 1 "<<hClosure->GetBinContent(j)<<" Direct_err "<<h1->GetBinError(j)<<" Pred_err "<<h2->GetBinError(j)<<" (Direct/Pred)_err "<<hClosure->GetBinError(j)<<endl;
   
  }







  //h1->SetName("WJetTTbar");
  //h1->GetXaxis()->SetTitle("pT(genTau)");
  //h1->SetTitle(hname);
    
  //TH1F *h2 = (TH1F*)_file1->FindObjectAny(histname);
  //h2->SetName("Reweighted");
  //h2->GetXaxis()->SetTitle("pT(genTau)");
  
  TH1F *hRatio = new TH1F("Pull Distribution","Distribution of Pull",100,-10,10);
  int Nbins=hClosure->GetXaxis()->GetNbins();
  std::cout<<" Nbins "<<Nbins<<endl;


  for(int j=1;j<=Nbins;j++){
    //    double ratio=(hClosure->GetBinContent(j)-1)/hClosure->GetBinError(j);
    //    double ratio=hClosure->GetBinContent(j)/hClosure->GetBinError(j);
    
    if(j<=10){
      NJ=2;
      NB=0;
      HTMHT=j;     
      
    }
    else if(j<=20){
      
      NJ=2;
      NB=1;
      HTMHT=j-10;
    }

    else if(j<=30){
      
      NJ=2;
      NB=2;
      HTMHT=j-20;
    }
    else if(j<=40){
      
      NJ=34;
      NB=0;
      HTMHT=j-30;
    }
    else if(j<=50){
      
      NJ=34;
      NB=1;
      HTMHT=j-40;
    }
    else if(j<=60){
      
      NJ=34;
      NB=2;
      HTMHT=j-50;
    }
    else if(j<=70){
      
      NJ=34;
      NB=3;
      HTMHT=j-60;
    }
    else if(j<=80){
      
      NJ=56;
      NB=0;
      HTMHT=j-70;
    }
    else if(j<=90){
      
      NJ=56;
      NB=1;
      HTMHT=j-80;
    }
    else if(j<=100){
      
      NJ=56;
      NB=2;
      HTMHT=j-90;
    }
    else if(j<=110){
      
      NJ=56;
      NB=3;
      HTMHT=j-100;
    }
    else if(j<=118){
      
      NJ=78;
      NB=0;
      HTMHT=j-110;
    }
    else if(j<=126){
      
      NJ=78;
      NB=1;
      HTMHT=j-118;
    }
    else if(j<=134){
      
      NJ=78;
      NB=2;
      HTMHT=j-126;
    }
    else if(j<=142){
      
      NJ=78;
      NB=3;
      HTMHT=j-134;
    }
    else if(j<=150){
      
      NJ=9;
      NB=0;
      HTMHT=j-142;
    }
    else if(j<=158){
      
      NJ=9;
      NB=1;
      HTMHT=j-150;
    }
    else if(j<=166){
      
      NJ=9;
      NB=2;
      HTMHT=j-158;
    }
    else if(j<=174){
       
      NJ=9;
      NB=3;
      HTMHT=j-166;
    }
 
      char Label[500];
      sprintf(Label,"%s%i_%s%i_%s%i","NJ",NJ,"NB",NB,"HTMHT",HTMHT);

    //*AR,Dec17,2016-It is found that abs(pull) returns only integral part of a number.
    //std::cout<<" bin "<<j<<" pull "<<hClosure->GetBinContent(j)<<" abs(pull) "<<abs(hClosure->GetBinContent(j))<<endl;
    //    if(hClosure->GetBinContent(j)>3 || hClosure->GetBinContent(j)<-3){
    if(fabs(hClosure->GetBinContent(j))>3){
      //    std::cout<<" bin_number "<<j<<" Direct(scaled to 3/fb) "<<h1->GetBinContent(j)<<" Direct after rescaling to 36.35/fb "<<h1->GetBinContent(j)*36.35/3<<" Pred(scaled to 3/fb) "<<h2->GetBinContent(j)<<" Pred after rescaling to 36.35/fb "<<h2->GetBinContent(j)*36.35/3<<" Direct/Pred - 1 "<<hClosure->GetBinContent(j) -1<<" Direct_err "<<h1->GetBinError(j)<<" Pred_err "<<h2->GetBinError(j)<<" (Direct/Pred)_err "<<hClosure->GetBinError(j)<<" pull "<<ratio<<endl;
      //std::cout<<" bin label "<<Label<<endl;      
      std::cout<<" bin_number "<<j<<" bin label "<<Label<<" Direct(scaled to 36.35/fb) "<<h1->GetBinContent(j)<<" Pred(scaled to 36.35/fb) "<<h2->GetBinContent(j)<<" Direct_err "<<h1->GetBinError(j)<<" Pred_err "<<h2->GetBinError(j)<<" (Direct/Pred)_err "<<hDirUponPred->GetBinError(j)<<" pull "<<hClosure->GetBinContent(j)<<endl;
      
    }
    //if(h1->GetBinContent(j)>0.1)
      hRatio->Fill(hClosure->GetBinContent(j));
    //else
    //std::cout<<" bin_number "<<j<<" bin label "<<Label<<" Direct(scaled to 36.35/fb) "<<h1->GetBinContent(j)<<" Expectation is less than 0.1"<<endl;

  }

  TCanvas *c = new TCanvas("c_Pull", "c_Pull", 500, 500);
  //  hClosure->Draw();
  hRatio->Draw();
  gPad->Update(); 

  //  TFile* xf = new TFile("hist_WgunTemplateReweight.root","UPDATE");
  //h1->Write();
  //h2->Write();
  //hDiff->Write();
  //xf->Close();

  
  
}    

