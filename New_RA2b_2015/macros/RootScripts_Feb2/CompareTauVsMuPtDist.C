//void plot_Acceptance_forICHEP2016(std::string elogForPlot=""){



void CompareTauVsMuPtDist() {

  compareResponse("HadTau_TauResponseTemplates_.root","HadTauEstimation_TTbarWJet_.root","hGenPt","hRecoMuPt",0,0,1,1);
  compareResponse("HadTau_TauResponseTemplates_.root","HadTauEstimation_TTbarWJet_.root","hGenPt_0","hRecoMuPt_0",0,0,1,1);
  compareResponse("HadTau_TauResponseTemplates_.root","HadTauEstimation_TTbarWJet_.root","hGenPt_1","hRecoMuPt_1",0,0,1,1);
  compareResponse("HadTau_TauResponseTemplates_.root","HadTauEstimation_TTbarWJet_.root","hGenPt_2","hRecoMuPt_2",0,0,1,1);
  compareResponse("HadTau_TauResponseTemplates_.root","HadTauEstimation_TTbarWJet_.root","hGenPt_3","hRecoMuPt_3",0,0,1,1);

//  compareResponse("HadTau_TauResponseTemplates_.root","HadTauEstimation_TTbarWJetST_.root","hGenPt","hRecoMuPt",0,0,1,1);
//  compareResponse("HadTau_TauResponseTemplates_.root","HadTauEstimation_TTbarWJetST_.root","hGenPt_0","hRecoMuPt_0",0,0,1,1);
//  compareResponse("HadTau_TauResponseTemplates_.root","HadTauEstimation_TTbarWJetST_.root","hGenPt_1","hRecoMuPt_1",0,0,1,1);
//  compareResponse("HadTau_TauResponseTemplates_.root","HadTauEstimation_TTbarWJetST_.root","hGenPt_2","hRecoMuPt_2",0,0,1,1);
//  compareResponse("HadTau_TauResponseTemplates_.root","HadTauEstimation_TTbarWJetST_.root","hGenPt_3","hRecoMuPt_3",0,0,1,1);
//
  //compareResponse("LLContamination_T2tt.root", "T2tt_SignalFiles_Dec24_14.00_Lumi1ipb.root", "mStop_150_mLSP_1","mStop_150_mLSP_1",0,0,1,1);


}

void compareResponse(char* WJetTTbarMC,char* Wgun,char *histOne,char *histTwo, bool logy, bool logx, bool overlay,bool normalized){
  gStyle->SetOptStat(111111111);
  gStyle->SetOptTitle(0);
    
  int col1 = kRed;
  int col2 = kBlue;
  int i=0;
    
  char hname[500], htit[500];
    
  TFile *_file0 = TFile::Open(WJetTTbarMC); 
  _file0->cd();
  TH1D *h1 = (TH1D*)_file0->FindObjectAny(histOne);
  h1->SetName("GenTauPt");
  h1->GetXaxis()->SetTitle("pT");
  h1->SetTitle("GenTauPt");

  TFile *_file1 = TFile::Open(Wgun);
  _file1->cd();
  TH1D *h2 = (TH1D*)_file1->FindObjectAny(histTwo);
  h2->SetName("RecoMuPt");
  h2->GetXaxis()->SetTitle("pT");
  h2->SetTitle("RecoMuPt");



  char cname[500];

  if(overlay)
    { h1->SetLineColor(col1);
      h2->SetLineColor(col2);
  	
      if(histOne=="hGenPt")
	sprintf(cname, "c_%s", "OvrallPt");
      else if(histOne=="hGenPt_0")
	sprintf(cname, "c_%s", "Pt_20_30");
      else if(histOne=="hGenPt_1")
	sprintf(cname, "c_%s", "Pt_30_50");
      else if(histOne=="hGenPt_2")
	sprintf(cname, "c_%s", "Pt_50_100");
      else if(histOne=="hGenPt_3")
	sprintf(cname, "c_%s", "PtAbove100");
      
      TCanvas *c = new TCanvas(cname, cname, 500, 500);
	
      if(logy)
	c->SetLogy();
	
      if(logx)
	c->SetLogx();
	
      if(normalized)
	{ h1->Scale(1.0/h1->GetSumOfWeights()); 
	  h2->Scale(1.0/h2->GetSumOfWeights()); 

	}
      h1->Draw();
      h2->Draw("sames");
      gPad->Update();
	
      TPaveStats *st1 = (TPaveStats*)h1->FindObject("stats");  
      st1->SetLineColor(col1);
	
      TPaveStats *st2 = (TPaveStats*)h2->FindObject("stats");  
      st2->SetLineColor(col2);
	
	
  
      //Following lines set attributes for statistics box
      // SetX1NDC, SetX2NDC, SetY1NDC, SetY2NDC set position of statbox wrt origin at bottom left of canvas
      //Argument for these functions are in terms of fraction of whole size of canvas
      
      st1->SetX1NDC(0.6);
      st1->SetX2NDC(0.9); 
      st1->SetY1NDC(0.5);
      st1->SetY2NDC(0.7);
      st2->SetX1NDC(0.6);
      st2->SetX2NDC(0.9);  
      st2->SetY1NDC(0.7);
      st2->SetY2NDC(0.9);
      st1->SetTextColor(col1);
      st2->SetTextColor(col2);
      
      

      
      gPad->Modified();
         
    }
  
  else
    {  std::cout<<" bool overlay has to be true "<<endl;
    }

  if(histOne=="hGenPt"){
    TH1D *hRatio = (TH1D*) h1->Clone("hRatio");
    hRatio->Divide(h1,h2,1,1);	
    
    TFile* xf = new TFile("hist_Pt_GenTauVsRecoMu.root","RECREATE");
    hRatio->Write();
    xf->Close();
  }
} 







