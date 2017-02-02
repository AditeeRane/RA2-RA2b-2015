#include <iostream>
#include <iomanip>
#include <vector>

void PlotPull() {

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
  double NjNbCorr[19]={1.11998,1.02071,0.899496,1.00848,1.03618,1.03088,1.21098,0.960755,0.993052,1.02152,1.02597,0.952303,0.984147,0.975219,1.06904,0.788829,0.863781,0.973593,1.02607};
  int BinBoundary[18]={11,21,31,41,51,61,71,81,91,101,111,119,127,135,143,151,159,167};      
  tempstack=(THStack*)_file0->Get(histname)->Clone();
  TH1F *h1 = (TH1F*) tempstack->GetStack()->Last();
  
  tempstack=(THStack*)_file1->Get(histname)->Clone();
  TH1F *h2BeforeNjNb = (TH1F*) tempstack->GetStack()->Last();

  h1->Scale(lumi/3);
  //    h2->Scale(lumi/3);
  //h2BeforeNjNb->Scale(lumi/3);
  TH1F *h2=(TH1F*)h2BeforeNjNb->Clone("h2");
  for(int j=1;j<=174;j++){
    if(j<=10)h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[0]);
    else if(j<=20)h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[1]);
    else if(j<=30)h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[2]);
    else if(j<=40)h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[3]);
    else if(j<=50)h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[4]);
    else if(j<=60)h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[5]);
    else if(j<=70)h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[6]);
    else if(j<=80)h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[7]);
    else if(j<=90)h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[8]);
    else if(j<=100)h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[9]);
    else if(j<=110)h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[10]);
    else if(j<=118)h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[11]);
    else if(j<=126)h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[12]);
    else if(j<=134)h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[13]);
    else if(j<=142)h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[14]);
    else if(j<=150)h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[15]);
    else if(j<=158)h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[16]);
    else if(j<=166)h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[17]);
    else if(j<=174)h2->SetBinContent(j,h2BeforeNjNb->GetBinContent(j)*NjNbCorr[18]); 
  }
  h2->Scale(lumi/3);

  TH1F *hClosure=(TH1F*)h1->Clone("hClosure");
  hClosure->Divide(h1,h2,1,1);



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
    double ratio=(hClosure->GetBinContent(j)-1)/hClosure->GetBinError(j);
    if(abs(ratio)>3)
      //    std::cout<<" bin_number "<<j<<" Direct(scaled to 3/fb) "<<h1->GetBinContent(j)<<" Direct after rescaling to 36.35/fb "<<h1->GetBinContent(j)*36.35/3<<" Pred(scaled to 3/fb) "<<h2->GetBinContent(j)<<" Pred after rescaling to 36.35/fb "<<h2->GetBinContent(j)*36.35/3<<" Direct/Pred - 1 "<<hClosure->GetBinContent(j) -1<<" Direct_err "<<h1->GetBinError(j)<<" Pred_err "<<h2->GetBinError(j)<<" (Direct/Pred)_err "<<hClosure->GetBinError(j)<<" pull "<<ratio<<endl;
    

    std::cout<<" bin_number "<<j<<" Direct(scaled to 36.35/fb) "<<h1->GetBinContent(j)<<" Pred(scaled to 36.35/fb) "<<h2->GetBinContent(j)<<" Direct/Pred - 1 "<<hClosure->GetBinContent(j) -1<<" Direct_err "<<h1->GetBinError(j)<<" Pred_err "<<h2->GetBinError(j)<<" (Direct/Pred)_err "<<hClosure->GetBinError(j)<<" pull "<<ratio<<endl;
    hRatio->Fill(ratio);

  }

  TCanvas *c = new TCanvas("c_Pull", "c_Pull", 500, 500);
  hRatio->Draw();
  gPad->Update(); 

  //  TFile* xf = new TFile("hist_WgunTemplateReweight.root","UPDATE");
  //h1->Write();
  //h2->Write();
  //hDiff->Write();
  //xf->Close();

  
  
}    

