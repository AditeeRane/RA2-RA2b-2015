#include <iostream>
#include <iomanip>
#include <vector>

void WgunTemplateReweighting() {

  compareResponse("ARElog90_HadTau_TauResponseTemplates_stacked.root","hist_taugun_aditee_LPC_Dec10.root", "hGenPt_0",0,0,1,1);
  compareResponse("ARElog90_HadTau_TauResponseTemplates_stacked.root","hist_taugun_aditee_LPC_Dec10.root", "hGenPt_1",0,0,1,1);
  compareResponse("ARElog90_HadTau_TauResponseTemplates_stacked.root","hist_taugun_aditee_LPC_Dec10.root", "hGenPt_2",0,0,1,1);
  compareResponse("ARElog90_HadTau_TauResponseTemplates_stacked.root","hist_taugun_aditee_LPC_Dec10.root", "hGenPt_3",0,0,1,1);
  //  compareResponse("ARElog90_HadTau_TauResponseTemplates_stacked.root","hist_taugun_aditee_LPC_Dec10.root", "hGenPt",0,0,1,1);

  //compareResponse("ARElog69_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee.root", "hTauResp_0_Up",0,0,1,1);
  //compareResponse("ARElog69_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee.root", "hTauResp_1_Up",0,0,1,1);
  //compareResponse("ARElog69_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee.root", "hTauResp_2_Up",0,0,1,1);
  //compareResponse("ARElog69_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee.root", "hTauResp_3_Up",0,0,1,1);
//compareResponse("ARElog69_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee.root", "hTauResp_0_Down",0,0,1,1);
//compareResponse("ARElog69_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee.root", "hTauResp_1_Down",0,0,1,1);
//compareResponse("ARElog69_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee.root", "hTauResp_2_Down",0,0,1,1);
//compareResponse("ARElog69_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee.root", "hTauResp_3_Down",0,0,1,1);

  //compareResponse("ARElog69_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee.root", "hGenPt_0",0,0,1,1);
  //compareResponse("ARElog69_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee.root", "hGenPt_1",0,0,1,1);
  //compareResponse("ARElog69_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee.root", "hGenPt_2",0,0,1,1);
  //compareResponse("ARElog69_HadTau_TauResponseTemplates_stacked.root", "hist_taugun_aditee.root", "hGenPt_3",0,0,1,1);
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
  char WJetTTbar[500],WGun[500];
  TFile *_file0 = TFile::Open(WJetTTbarMC);
  TFile *_file1 = TFile::Open(Wgun);
    
  //  _file0->cd();
  //TDirectory *demo1    = (TDirectory*)_file0->FindObjectAny("demo");
  //_file1->cd();
  //TDirectory *demo2    = (TDirectory*)_file1->FindObjectAny("demo");
    
    
  //sprintf(hname, "%s", histname); //string "hname" is set to the content of histname 
    
  //  sprintf(htit, "WJetTTbarMC vs Wgun");
    
  if(histname=="hGenPt_0"){    
    sprintf(hname, "Reweight_0");
    sprintf(WJetTTbar,"WJetTTbar_0");
    sprintf(WGun,"WGun_0");
  }
  else if(histname=="hGenPt_1"){    
    sprintf(hname, "Reweight_1");
    sprintf(WJetTTbar,"WJetTTbar_1");
    sprintf(WGun,"WGun_1");
  } 
  else if(histname=="hGenPt_2"){    
    sprintf(hname, "Reweight_2");
    sprintf(WJetTTbar,"WJetTTbar_2");
    sprintf(WGun,"WGun_2");
  }
  else if(histname=="hGenPt_3"){    
    sprintf(hname, "Reweight_3");
    sprintf(WJetTTbar,"WJetTTbar_3");
    sprintf(WGun,"WGun_3");
  }
  else{   
    sprintf(hname, "Reweight");   
    sprintf(WJetTTbar,"WJetTTbar");
    sprintf(WGun,"WGun");
  }
  
    
    
  TH1F *h1 = (TH1F*)_file0->FindObjectAny(histname);
  TH1F *h2 = (TH1F*)_file1->FindObjectAny(histname);
  h1->SetName(WJetTTbar);
  h2->SetName(WGun);
  h2->GetXaxis()->SetTitle("pT(visible)/pT(genTau)");
    
  if(histname=="hGenPt_0" || histname=="hGenPt_1" || histname=="hGenPt_2"){ 
    TH1F *hDiff = (TH1F*) h1->Clone("hDiff");
    hDiff->Divide(h1,h2,1,1);
    hDiff->SetName(hname);
  }
  else{
    //TH1F *hDiff = (TH1F*) h1->Clone("hDiff"); 
    TH1F *h2New = new TH1F("Restricth2NBins","Restricth2NBins",70,100,170);
    int Nbins=h2New->GetXaxis()->GetNbins();
    for(int i=1;i<=Nbins;i++){
      h2New->SetBinContent(i,h2->GetBinContent(i));
      h2New->SetBinError(i,h2->GetBinError(i));
      std::cout<<" i "<<i <<" h2_i "<<h2->GetBinContent(i)<<" h2New_i "<<h2New->GetBinContent(i)<<" h2_e "<<h2->GetBinError(i)<<" h2New_e "<<h2New->GetBinError(i)<<endl;
    }
    TH1F *h1New = new TH1F("Restricth1NBins","Restricth1NBins",70,100,170);
    int Nbins=h1New->GetXaxis()->GetNbins();
    for(int i=1;i<=Nbins;i++){
      h1New->SetBinContent(i,h1->GetBinContent(i));
      h1New->SetBinError(i,h1->GetBinError(i));
      std::cout<<" i "<<i <<" h1_i "<<h1->GetBinContent(i)<<" h1New_i "<<h1New->GetBinContent(i)<<" h1_e "<<h1->GetBinError(i)<<" h1New_e "<<h1New->GetBinError(i)<<endl;
    }
    TH1F *hDiff = (TH1F*) h1New->Clone("hDiff");
    hDiff->Divide(h1New,h2New,1,1);
    hDiff->SetName(hname);
  }

    /*
    //std::cout<<"h2->GetBinContent(70) "<<h2->GetBinContent(70)<<endl;
    TH1F *hNew = (TH1F*) h1->Clone("hNew");
    int Nbins=hNew->GetXaxis()->GetNbins();
    std::cout<<" Nbins "<<Nbins<<endl;
    if(histname=="hGenPt_3") int copyBin=70;
    if(histname=="hGenPt") int copyBin=150;
    for(int i=1;i<=Nbins;i++){
      std::cout<<" i "<<i <<" h2 "<<h2->GetBinContent(i)<<endl;
      if(h2->GetBinContent(i)==0){
	hNew->SetBinContent(i,h2->GetBinContent(copyBin));
	hNew->SetBinError(i,h2->GetBinError(copyBin));
      }
      else{
	hNew->SetBinContent(i,h2->GetBinContent(i));
	hNew->SetBinError(i,h2->GetBinError(i));
      }
      std::cout<<" i "<<i <<" h2 "<<h2->GetBinContent(i)<<" hNew "<<hNew->GetBinContent(i)<<endl;
    }
    TH1F *hDiff = (TH1F*) h1->Clone("hDiff");
    hDiff->Divide(h1,hNew,1,1);
    hDiff->SetName(hname);
  }


*/
  /*  
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
	{ //h1->Scale(1.0/h1->GetEntries()); // scales to normalized histogram with area unity. Number of entries in each bin is divided by total number of entries.
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
*/  
  TFile* xf = new TFile("hist_WgunTemplateReweight.root","UPDATE");
  h1->Write();
  h2->Write();
  hDiff->Write();
  xf->Close();
  
  
}    

