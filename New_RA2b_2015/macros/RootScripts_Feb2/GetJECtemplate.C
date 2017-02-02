//void plot_Acceptance_forICHEP2016(std::string elogForPlot=""){



void GetJECtemplate() {

  compareResponse("HadTau_TauResponseTemplates_.root", "tauJetJECtemplate","tauJetJECtemplateStat",0,0,1,1);

  //compareResponse("LLContamination_T2tt.root", "T2tt_SignalFiles_Dec24_14.00_Lumi1ipb.root", "mStop_150_mLSP_1","mStop_150_mLSP_1",0,0,1,1);


}

void compareResponse(char* WJetTTbarMC,char *histOne,char *histTwo, bool logy, bool logx, bool overlay,bool normalized){
  gStyle->SetOptStat(111111111);
  gStyle->SetOptTitle(0);
    
  int col1 = kRed;
  int col2 = kBlue;
  int i=0;
    
  char hname[500], htit[500];
    
  TFile *_file0 = TFile::Open(WJetTTbarMC);
  
    
  _file0->cd();
  TH2D *h1 = (TH2D*)_file0->FindObjectAny(histOne);
  h1->SetName("tauJetJECtemplate");
  TH2D *h2 = (TH2D*)_file0->FindObjectAny(histTwo);
  h2->SetName("tauJetJECtemplateStat");
    
  TH2D *hRatio = (TH2D*) h1->Clone("hRatio");
  hRatio->Divide(h2);
  hRatio->GetXaxis()->SetName("raw tau jet pT");
  hRatio->GetYaxis()->SetName("tau jet eta");
  hRatio->SetName("AvetauJetJECtemplate");

  TFile * xf=new TFile("JECtemplatefile.root","RECREATE");
  xf->cd();
  hRatio->Write();


  /*
  char cname[500];
  h1->Write();  TCanvas *c = new TCanvas("Signal_genHTMHTvsReco","Signal_genHTMHTvsReco", 500, 500);
  
  hRatio->Draw();
  
  
  gPad->Update();
  
*/    
  
} 







