#include <iostream>
#include <iomanip>
#include <vector>

void bRateDebugSysMinusNominal() {

  compareResponse("ARElog96_HadTauEstimation_hadd_withSystematics_TauMinusMu.root", "ARElog96_HadTauEstimation_hadd_withSystematics_Tau.root","BMistagMinus","allEvents","searchH_b_",0,0,1,1);

}

void compareResponse(char* ffast, char* ffull,char *SysDir,char *NomDir, char *histname, bool logy, bool logx,bool overlay,bool normalized){
  //void compareResponse(char* WJetTTbarMC, char* Wgun, char* histOne, char *histTwo, bool logy, bool logx, bool overlay,bool normalized){
  gStyle->SetOptStat(111111111);
  gStyle->SetOptTitle(0);
    
  int col1 = kBlue;
  int col2 = kRed;
  int i=0;
    
  char hname[500], htit[500];
    
  //  TFile *_file0 = TFile::Open(ffast);
  //TFile *_file1 = TFile::Open(ffull);
  char Directory[500];
  char NomDirectory[500];
  char SysDirectory[500];
  TFile *_file0 = new TFile(ffast);
  TFile *_file1 = new TFile(ffull);
  //  _file0->cd();
    
    
  sprintf(hname, "%s", histname); //string "hname" is set to the content of histname 
  sprintf(NomDirectory,"%s",NomDir);
  sprintf(SysDirectory,"%s",SysDir);
  sprintf(Directory,"%s","delphi");    

  //  sprintf(htit, "WJetTTbarMC vs Wgun");
  TDirectory *demo0    = (TDirectory*)_file0->FindObjectAny(NomDirectory);
  TDirectory *demo00    = (TDirectory*)demo0->FindObjectAny(Directory);

  TDirectory *demoSys0    = (TDirectory*)_file0->FindObjectAny(SysDirectory);
  TDirectory *demoSys00    = (TDirectory*)demoSys0->FindObjectAny(Directory);



  //_file1->cd();
      
  //if(histOne="hDiff")    
  //sprintf(hname, "Tau Vs Mu B mistag rate");   
  TH1D *hSysNum0 = new TH1D("Sys-nominal_TauMinusMu","Sys-nominal_TauMinusMu",174,1.,174.);    
  int Nbins=174;

    
  //  TH1D * h1 = (TH1D*)demo1->FindObjectAny("searchH_b_");
  TH1D * h1 = (TH1D*)demo00->Get("searchH_b_");
  
  TH1D * h2 = (TH1D*)demoSys00->Get("searchH_b_");
  
  for(int j=1;j<=Nbins;j++){
    double NomOut=h1->GetBinContent(j);
    double SysOut=h2->GetBinContent(j);
    //double aveRate=(Diff_WJet+Diff_TTbar)/2;
    hSysNum0->SetBinContent(j,SysOut-NomOut);
  }

  
  TDirectory *demo1    = (TDirectory*)_file1->FindObjectAny(NomDirectory);
  TDirectory *demo11    = (TDirectory*)demo1->FindObjectAny(Directory);

  TDirectory *demoSys1    = (TDirectory*)_file1->FindObjectAny(SysDirectory);
  TDirectory *demoSys11    = (TDirectory*)demoSys1->FindObjectAny(Directory);

  TH1D *hSysNum1 = new TH1D("Sys-nominal_Tau","Sys-nominal_Tau",174,1.,174.);    
  int Nbins=174;

    
  //  TH1D * h1 = (TH1D*)demo1->FindObjectAny("searchH_b_");
  TH1D * h1 = (TH1D*)demo11->Get("searchH_b_");
  
  TH1D * h2 = (TH1D*)demoSys11->Get("searchH_b_");
  
  for(int j=1;j<=Nbins;j++){
    double NomOut=h1->GetBinContent(j);
    double SysOut=h2->GetBinContent(j);
    //double aveRate=(Diff_WJet+Diff_TTbar)/2;
    hSysNum1->SetBinContent(j,SysOut-NomOut);
  }

  
  //  std::cout<<" Directory "<<Directory<<endl;  
  char cname[500];
  //char cone[500];
  //char ctwo[500];
    
  if(overlay)
    { hSysNum0->SetLineColor(col1);
      hSysNum1->SetLineColor(col2);
      sprintf(cname, "c_%s_%s", "Diff_Sys-Nominal",SysDir);
	
      TCanvas *c = new TCanvas(cname, cname, 500, 500);
	
      if(logy)
	c->SetLogy();
	
      if(logx)
	c->SetLogx();
	
      if(normalized)
	{ //h1->Scale(1.0/h1->GetEntries()); // scales to normalized histogram with area unity. Number of entries in each bin is divided by total number of entries.
	  //h1->Scale(1.0/20.0); //for htau_resp
	  //h2->Scale(1.0/h2->GetEntries());
	  //h1->Scale(1.0/20.0);
	}
	
      hSysNum0->Draw();
      hSysNum1->Draw("sames");
	
      gPad->Update();
	
      TPaveStats *st1 = (TPaveStats*)hSysNum0->FindObject("stats");  
      st1->SetLineColor(col1);
	
      TPaveStats *st2 = (TPaveStats*)hSysNum1->FindObject("stats");  
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
  
  //TFile* xf = new TFile("hist_AvebRateDiff_TauVsMu.root","RECREATE");
  //hAveDiff.Write();
  //xf.Close();

  
}    

