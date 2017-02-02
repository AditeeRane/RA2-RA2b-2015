#include <iostream>
#include <iomanip>
#include <vector>

void PlotDirectVsPrediction() {

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
  compareResponse("ARElog90_GenInfo_HadTauEstimation_stacked.root", "ARElog91_HadTauEstimation_stacked.root", "searchH_b",0,0,1,1);
  //  compareResponse("ARElog90_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee_Reweighted.root", "hTauResp_0",0,0,1,1);
  //compareResponse("ARElog90_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee_Reweighted.root", "hTauResp_1",0,0,1,1);
  //compareResponse("ARElog90_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee_Reweighted.root", "hTauResp_2",0,0,1,1);
  //compareResponse("ARElog90_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee_Reweighted.root", "hTauResp_3",0,0,1,1);
    

//compareResponse("ARElog69_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee.root", "hGenPt",0,0,1,1);


}

//void compareResponse(char* ffast, char* ffull, char *histname, bool logy, bool logx,double cNo){
void compareResponse(char* WJetTTbarMC, char* Wgun, char *histname, bool logy, bool logx, bool overlay,bool normalized){
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
    
  tempstack=(THStack*)_file0->Get(histname)->Clone();
  TH1F *h1 = (TH1F*) tempstack->GetStack()->Last();

  tempstack=(THStack*)_file1->Get(histname)->Clone();
  TH1F *h2 = (TH1F*) tempstack->GetStack()->Last();

  //h1->SetName("WJetTTbar");
  //h1->GetXaxis()->SetTitle("pT(genTau)");
  //h1->SetTitle(hname);
    
  //TH1F *h2 = (TH1F*)_file1->FindObjectAny(histname);
  //h2->SetName("Reweighted");
  //h2->GetXaxis()->SetTitle("pT(genTau)");
  TH1F *hRatio = new TH1F("DirectVsPrediction","Distribution of Direct/Prediction",140,0,7);
  int Nbins=h1->GetXaxis()->GetNbins();
  std::cout<<" Nbins "<<Nbins<<endl;
  for(int j=1;j<=Nbins;j++){
    double ratio=h1->GetBinContent(j)/h2->GetBinContent(j);
    std::cout<<" j "<<j<<" h1 "<<h1->GetBinContent(j)<<" h2 "<<h2->GetBinContent(j)<<" ratio "<<ratio<<endl;
    hRatio->Fill(ratio);
  }

  TCanvas *c = new TCanvas("c_DirectVsPrediction", "c_DirectVsPrediction", 500, 500);
  hRatio->Draw();
  gPad->Update(); 

  //  TFile* xf = new TFile("hist_WgunTemplateReweight.root","UPDATE");
  //h1->Write();
  //h2->Write();
  //hDiff->Write();
  //xf->Close();

  
  
}    

