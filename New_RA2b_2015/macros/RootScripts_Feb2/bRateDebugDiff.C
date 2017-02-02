#include <iostream>
#include <iomanip>
#include <vector>

void bRateDebugDiff() {

  compareResponse("ARElog96_HadTauEstimation_hadd_withSystematics_TauMinusMu.root", "ARElog96_HadTauEstimation_hadd_withSystematics_Tau.root","allEvents","BMistagPlus","BMistagMinus","searchH_b_",0,0,1,1);

}

void compareResponse(char* ffast, char* ffull,char *NomDir,char *SysUpDir,char *SysDnDir, char *histname, bool logy, bool logx,bool overlay,bool normalized){
  //void compareResponse(char* WJetTTbarMC, char* Wgun, char* histOne, char *histTwo, bool logy, bool logx, bool overlay,bool normalized){
  gStyle->SetOptStat(111111111);
  gStyle->SetOptTitle(0);
    
  int col1 = kBlue;
  int col2 = kRed;
  int col3= kGreen;
  int i=0;
    
  char hname[500], htit[500];
    
  char Directory[500];
  char NomDirectory[500];
  char SysUpDirectory[500];
  char SysDnDirectory[500];

  TFile *_file0 = new TFile(ffast);
  TFile *_file1 = new TFile(ffull);
   
    
  sprintf(hname, "%s", histname); //string "hname" is set to the content of histname 
  sprintf(NomDirectory,"%s",NomDir);
  sprintf(SysUpDirectory,"%s",SysUpDir);
  sprintf(SysDnDirectory,"%s",SysDnDir);
  sprintf(Directory,"%s","delphi");    

  //  sprintf(htit, "WJetTTbarMC vs Wgun");
  TDirectory *demo0    = (TDirectory*)_file0->FindObjectAny(NomDirectory);
  TDirectory *demo00    = (TDirectory*)demo0->FindObjectAny(Directory);

   
  TDirectory *demo1    = (TDirectory*)_file1->FindObjectAny(NomDirectory);
  TDirectory *demo11    = (TDirectory*)demo1->FindObjectAny(Directory);


  TH1D *hDiffNom = new TH1D("hDiffNom","hDiffNom",174,1.,174.);    
  int Nbins=174;

    
  
  TH1D * h1 = (TH1D*)demo00->Get("searchH_b_");
  
  TH1D * h2 = (TH1D*)demo11->Get("searchH_b_");
  
  for(int j=1;j<=Nbins;j++){
    double NomOut=h1->GetBinContent(j);
    double SysOut=h2->GetBinContent(j);
    //double aveRate=(Diff_WJet+Diff_TTbar)/2;
    hDiffNom->SetBinContent(j,SysOut-NomOut);
  }


  TDirectory *demo0    = (TDirectory*)_file0->FindObjectAny(SysUpDirectory);
  TDirectory *demo00    = (TDirectory*)demo0->FindObjectAny(Directory);

   
  TDirectory *demo1    = (TDirectory*)_file1->FindObjectAny(SysUpDirectory);
  TDirectory *demo11    = (TDirectory*)demo1->FindObjectAny(Directory);


  TH1D *hDiffSysUp = new TH1D("hDiffSysUp","hDiffSysUp",174,1.,174.);    
  int Nbins=174;

    
  
  TH1D * h1 = (TH1D*)demo00->Get("searchH_b_");
  
  TH1D * h2 = (TH1D*)demo11->Get("searchH_b_");
  
  for(int j=1;j<=Nbins;j++){
    double NomOut=h1->GetBinContent(j);
    double SysOut=h2->GetBinContent(j);
    
    hDiffSysUp->SetBinContent(j,SysOut-NomOut);
  }
  

  TDirectory *demo0    = (TDirectory*)_file0->FindObjectAny(SysDnDirectory);
  TDirectory *demo00    = (TDirectory*)demo0->FindObjectAny(Directory);

   
  TDirectory *demo1    = (TDirectory*)_file1->FindObjectAny(SysDnDirectory);
  TDirectory *demo11    = (TDirectory*)demo1->FindObjectAny(Directory);


  TH1D *hDiffSysDn = new TH1D("hDiffSysDn","hDiffSysDn",174,1.,174.);    
  int Nbins=174;

    
  
  TH1D * h1 = (TH1D*)demo00->Get("searchH_b_");
  
  TH1D * h2 = (TH1D*)demo11->Get("searchH_b_");
  
  for(int j=1;j<=Nbins;j++){
    double NomOut=h1->GetBinContent(j);
    double SysOut=h2->GetBinContent(j);
    
    hDiffSysDn->SetBinContent(j,SysOut-NomOut);
  }

  //  std::cout<<" Directory "<<Directory<<endl;  
  char cname[500];
  if(overlay)
    { hDiffNom->SetLineColor(col1);
      hDiffSysUp->SetLineColor(col2);
      hDiffSysDn->SetLineColor(col3);
      sprintf(cname, "c_%s", "Difference of predictions with (Tau-Mu) and Tau");
	
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
	
      hDiffNom->Draw();
      hDiffSysUp->Draw("sames");
      hDiffSysDn->Draw("sames");

	
      gPad->Update();
	
      TPaveStats *st1 = (TPaveStats*)hDiffNom->FindObject("stats");  
      st1->SetLineColor(col1);
	
      TPaveStats *st2 = (TPaveStats*)hDiffSysUp->FindObject("stats");  
      st2->SetLineColor(col2);

      TPaveStats *st3 = (TPaveStats*)hDiffSysDn->FindObject("stats");  
      st3->SetLineColor(col3);


	
	
  
      //Following lines set attributes for statistics box
      // SetX1NDC, SetX2NDC, SetY1NDC, SetY2NDC set position of statbox wrt origin at bottom left of canvas
      //Argument for these functions are in terms of fraction of whole size of canvas
      
      st1->SetX1NDC(0.6);
      st1->SetX2NDC(0.9); 
      st1->SetY1NDC(0.3);
      st1->SetY2NDC(0.5);
      st2->SetX1NDC(0.6);
      st2->SetX2NDC(0.9);  
      st2->SetY1NDC(0.5);
      st2->SetY2NDC(0.7);
      st3->SetX1NDC(0.6);
      st3->SetX2NDC(0.9);  
      st3->SetY1NDC(0.7);
      st3->SetY2NDC(0.9);

      st1->SetTextColor(col1);
      st2->SetTextColor(col2);
      st3->SetTextColor(col3);
      
      

      
      gPad->Modified();

         
    }
  
  else
    {  std::cout<<" bool overlay has to be true "<<endl;
    }
  
  //TFile* xf = new TFile("hist_AvebRateDiff_TauVsMu.root","RECREATE");
  //hAveDiff.Write();
  //xf.Close();

  
}    

