//void plot_Acceptance_forICHEP2016(std::string elogForPlot=""){



void plot_SignalRatio() {

  compareResponse("T2tt_SignalFiles.root", "T2tt_SignalFiles_Dec24_14.00_Lumi1ipb.root", "RA2bin_1000_50","mStop_1000_mLSP_50",0,0,1,1);

  //compareResponse("LLContamination_T2tt.root", "T2tt_SignalFiles_Dec24_14.00_Lumi1ipb.root", "mStop_150_mLSP_1","mStop_150_mLSP_1",0,0,1,1);


}

void compareResponse(char* WJetTTbarMC, char* Wgun, char *histOne,char *histTwo, bool logy, bool logx, bool overlay,bool normalized){
  gStyle->SetOptStat(111111111);
  gStyle->SetOptTitle(0);
    
  int col1 = kRed;
  int col2 = kBlue;
  int i=0;
    
  char hname[500], htit[500];
    
  TFile *_file0 = TFile::Open(WJetTTbarMC);
  TFile *_file1 = TFile::Open(Wgun);
    
  _file0->cd();
  TDirectory *demo1    = (TDirectory*)_file0->FindObjectAny("SearchH_b");  
  //  TDirectory *demo1    = (TDirectory*)_file0->FindObjectAny("SignalContamination");  

  TH1F *h1 = (TH1F*)demo1->FindObjectAny(histOne);
  h1->SetName(histOne);
    

  _file1->cd();
  TDirectory *demo2    = (TDirectory*)_file1->FindObjectAny("SignalContamination");   
  TH1F *h2 = (TH1F*)demo2->FindObjectAny(histTwo);
  h2->SetName(histTwo);
    

    TH1F *hRatio=new TH1F("hRatio","Ratio of signal predictions",80,1,80);

  //TH1F *hRatio=new TH1F("hRatio","Ratio of signal predictions",174,1,174);
  
  //  TH1F *hRatio=new TH1F("hRatio","Ratio of signal predictions",174,1,174);
  int Nbins=h1->GetXaxis()->GetNbins();
  
for(int i=1;i<Nbins;i++){
  if(h2->GetBinContent(i)<0.0000001)
    hRatio->SetBinContent(i,0);
  else
    hRatio->SetBinContent(i,h1->GetBinContent(i)/h2->GetBinContent(i));
  }

/*



 double x=0;
  for(int i=1;i<Nbins;i++){
    if(i<=30)
      hRatio->SetBinContent(i,0);
    else{
      if(h2->GetBinContent(i)<0.000000001)
	hRatio->SetBinContent(i,0);   
      else{
	if(i<=110)
	  hRatio->SetBinContent(i,h1->GetBinContent(i-30)/h2->GetBinContent(i));
	else{
	  int multiple=2+2*((i-111)/8);
	  std::cout<<" h1 "<<i<<" h2 "<<i-30+multiple<<endl;
	  hRatio->SetBinContent(i,h1->GetBinContent(i-30+multiple)/h2->GetBinContent(i));
	  if(i<=160) 
	    x=h1->GetBinContent(i);
	}
      }
    }
    std::cout<<" bin "<<i<<" h1Content "<<x<<" h2Content "<<h2->GetBinContent(i)<<" ratio "<<hRatio->GetBinContent(i)<<endl;
  }


*/
  char cname[500];
  TCanvas *c = new TCanvas(cname, cname, 500, 500);
  
  hRatio->Draw();
  
  
  gPad->Update();
  
    
  
}    







