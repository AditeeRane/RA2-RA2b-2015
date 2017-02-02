#include <iostream>
#include <iomanip>
#include <vector>

void CompareTemplates_UnweightVsReweight() {

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
  //compareResponse("ARElog90_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee_Reweighted.root", "hGenPt_3",0,0,1,1);
  //  compareResponse("ARElog90_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee_Reweighted.root", "hTauResp_0",0,0,1,1);
  //compareResponse("ARElog90_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee_Reweighted.root", "hTauResp_1",0,0,1,1);
  //compareResponse("ARElog90_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee_Reweighted.root", "hTauResp_2",0,0,1,1);
  //compareResponse("ARElog90_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee_Reweighted.root", "hTauResp_3",0,0,1,1);
    
//  compareResponse("ARElog102_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee_Reweighted.root", "hGenPt_0",0,0,1,1);
//  compareResponse("ARElog102_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee_Reweighted.root", "hGenPt_1",0,0,1,1);
//  compareResponse("ARElog102_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee_Reweighted.root", "hGenPt_2",0,0,1,1);
//  compareResponse("ARElog102_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee_Reweighted.root", "hGenPt_3",0,0,1,1);
//
  //compareResponse("ARElog102_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee_Reweighted.root", "hTauResp_0",0,0,1,1);
  //compareResponse("ARElog102_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee_Reweighted.root", "hTauResp_1",0,0,1,1);
  //compareResponse("ARElog102_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee_Reweighted.root", "hTauResp_2",0,0,1,1);
  //compareResponse("ARElog102_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee_Reweighted.root", "hTauResp_3",0,0,1,1);

  //compareResponse("hist_taugun_aditee_LPC_Dec27.root", "hist_taugun_aditee_Reweighted.root", "hTauResp_0",0,0,1,1);
  //compareResponse("hist_taugun_aditee_LPC_Dec27.root", "hist_taugun_aditee_Reweighted.root", "hTauResp_1",0,0,1,1);
  //compareResponse("hist_taugun_aditee_LPC_Dec27.root", "hist_taugun_aditee_Reweighted.root", "hTauResp_2",0,0,1,1);
  //compareResponse("hist_taugun_aditee_LPC_Dec27.root", "hist_taugun_aditee_Reweighted.root", "hTauResp_3",0,0,1,1);
  //
  compareResponse("hist_taugun_aditee_LPC_Dec27.root", "hist_taugun_aditee_Reweighted.root", "hGenPt_0",0,0,1,1);
  compareResponse("hist_taugun_aditee_LPC_Dec27.root", "hist_taugun_aditee_Reweighted.root", "hGenPt_1",0,0,1,1);
  compareResponse("hist_taugun_aditee_LPC_Dec27.root", "hist_taugun_aditee_Reweighted.root", "hGenPt_2",0,0,1,1);
  compareResponse("hist_taugun_aditee_LPC_Dec27.root", "hist_taugun_aditee_Reweighted.root", "hGenPt_3",0,0,1,1);
  
 // 
//compareResponse("ARElog69_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee.root", "hGenPt",0,0,1,1);


}

//void compareResponse(char* ffast, char* ffull, char *histname, bool logy, bool logx,double cNo){
void compareResponse(char* WJetTTbarMC, char* Wgun, char *histname, bool logy, bool logx, bool overlay,bool normalized){
  gStyle->SetOptStat(111111111);
  gStyle->SetOptTitle(0);
    
  int col1 = kRed;
  int col2 = kBlue;
  int i=0;
    
  char hname[500], htit[500];
    
  TFile *_file0 = TFile::Open(WJetTTbarMC);
  TFile *_file1 = TFile::Open(Wgun);
    
  //  _file0->cd();
  //TDirectory *demo1    = (TDirectory*)_file0->FindObjectAny("demo");
  //_file1->cd();
  //TDirectory *demo2    = (TDirectory*)_file1->FindObjectAny("demo");
    
    
  //sprintf(hname, "%s", histname); //string "hname" is set to the content of histname 
    
  //  sprintf(htit, "WJetTTbarMC vs Wgun");
    
  if(histname=="hGenPt_0" || histname=="hTauResp_0")    
    sprintf(hname, "WJetTTbar vs Reweighted(20-30GeV");   
  else if(histname=="hGenPt_1" || histname=="hTauResp_1")    
    sprintf(hname, "WJetTTbar vs Reweighted(30-50GeV");
  else if(histname=="hGenPt_2"|| histname=="hTauResp_2")    
    sprintf(hname, "WJetTTbar vs Reweighted(50-100GeV");
  else
    sprintf(hname, "WJetTTbar vs Reweighted(Above 100GeV");
    
    
    
  TH1F *h1 = (TH1F*)_file0->FindObjectAny(histname);
  //  h1->SetName("WJetTTbar");
  h1->SetName("Unweighted");

  h1->GetXaxis()->SetTitle("pT(genTau)");
  //  h1->GetXaxis()->SetTitle("pT(visible)/pT(genTau)");

  h1->SetTitle(hname);
    
  TH1F *h2 = (TH1F*)_file1->FindObjectAny(histname);
  h2->SetName("Reweighted");
  h2->GetXaxis()->SetTitle("pT(genTau)");
    
    
  char cname[500];
  char cone[500];
  char ctwo[500];
    
  if(overlay)
    { h1->SetLineColor(col1);
      h2->SetLineColor(col2);
      sprintf(cname, "c_%s", histname);
	
      TCanvas *c = new TCanvas(cname, cname, 500, 500);
	
      if(logy)
	c->SetLogy();
	
      if(logx)
	c->SetLogx();
	
      if(normalized)
	{ if(histname=="hGenPt_3"){
	    double Integral_100_170=0.0;
	    for(int j=1;j<=70;j++)
	      Integral_100_170 +=h1->GetBinContent(j)*h1->GetBinWidth(j);
	    //double Integral_100_170=h1->DoIntegral(1,70);
	    std::cout<<" Integral_100_170 "<<Integral_100_170<<endl;
	    h1->Scale(1.0/Integral_100_170); // scales to normalized histogram with area unity. Number of entries in each bin is divided by total number of entries.
	  }
	  //h1->Scale(1.0/20.0); //for htau_resp
	  //	  h2->Scale(1.0/h2->GetEntries());
	  //h1->Scale(1.0/20.0);
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
  
  
  
}    

