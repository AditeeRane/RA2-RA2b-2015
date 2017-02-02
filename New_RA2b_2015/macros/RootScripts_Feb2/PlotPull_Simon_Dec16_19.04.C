#include <iostream>
#include <iomanip>
#include <vector>

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
      
  tempstack=(THStack*)_file0->Get(histname)->Clone();
  TH1F *h1 = (TH1F*) tempstack->GetStack()->Last();

  tempstack=(THStack*)_file1->Get(histname)->Clone();
  TH1F *h2 = (TH1F*) tempstack->GetStack()->Last();

  h1->Scale(lumi/3);
  h2->Scale(lumi/3);
  //std::cout<<" bin_number "<<j<<" Direct(scaled to 36.35/fb) "<<h1->GetBinContent(j)<<" Pred(scaled to 36.35/fb) "<<h2->GetBinContent(j)<<endl;
   
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

    if(abs(hClosure->GetBinContent(j))>3)
      //    std::cout<<" bin_number "<<j<<" Direct(scaled to 3/fb) "<<h1->GetBinContent(j)<<" Direct after rescaling to 36.35/fb "<<h1->GetBinContent(j)*36.35/3<<" Pred(scaled to 3/fb) "<<h2->GetBinContent(j)<<" Pred after rescaling to 36.35/fb "<<h2->GetBinContent(j)*36.35/3<<" Direct/Pred - 1 "<<hClosure->GetBinContent(j) -1<<" Direct_err "<<h1->GetBinError(j)<<" Pred_err "<<h2->GetBinError(j)<<" (Direct/Pred)_err "<<hClosure->GetBinError(j)<<" pull "<<ratio<<endl;
    

      std::cout<<" bin_number "<<j<<" Direct(scaled to 36.35/fb) "<<h1->GetBinContent(j)<<" Pred(scaled to 36.35/fb) "<<h2->GetBinContent(j)<<" Direct_err "<<h1->GetBinError(j)<<" Pred_err "<<h2->GetBinError(j)<<" (Direct/Pred)_err "<<hClosure->GetBinError(j)<<" pull "<<hClosure->GetBinContent(j)<<endl;
    hRatio->Fill(hClosure->GetBinContent(j));

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

