#include <iostream>
#include <iomanip>
#include <TFile.h>	
#include <vector>
#include "TH1D.h"
#include "string.h"

using namespace std;

void WriteSysFile(char* InputFile, char* histname);


void CombineJECSys() {
  //  WriteSysFile("TauHad2/ARElog40_HadTauEstimation_hadd_JECSysRef.root","searchH_b");
  WriteSysFile("/home/aditee/work/work_DOS_9Apr16_Analysis/hist_taugun_aditee.root","hTauResp_0");
}

void WriteSysFile(char* InputFile, char* histname){
//void WriteSysFile(string InputFile, string histname){
  //vector<string> FileVec;

  //TFile file_0 = TFile::Open(ARElog40_HadTauEstimation_hadd_JECSysUp.root);
  //FileVec.push_back("ARElog40_HadTauEstimation_hadd_JECSysUp.root");
  //FileVec.push_back("ARElog40_HadTauEstimation_hadd_JECSysDn.root");
  //FileVec.push_back("ARElog40_HadTauEstimation_hadd_JECSysRef.root");

  std::cout<<"*******************"<<endl;
  TFile *fOut = new TFile("JECSys.root","RECREATE");
  //for(i=0;i<FileVec.size();i++){
  TFile* fIn = TFile::Open(InputFile);
  //if(FileVec[i].find("JEC")!=string::npos){
  int x=0;int y=0; 
  TH1D* hNew = new TH1D("searchH_b","search bin histogram",174,0,174);
  //if( strncmp(histname,"searchH_b") ){
  if(histname=="hTauResp_0"){
    //fIn->cd();
    //TH1D *h = (TH1D*)fIn->Get("searchH_b");
    TH1D* h=(TH1D*)fIn->FindObjectAny(histname);
    int Nbins=h->GetNbinX();
    //    Double_t scale=1000000.0;
    //TH1D* hScaled=scale*h;
    //h->Multiply(h,1000000.);
    //h->Add(h,1000000.);
    std::cout<<" integral_org "<< (double) h->Integral()<<std::endl;    
    //std::cout<<" integral_scaled "<< (double) hScaled->Integral()<<std::endl;
    for(int j=1;j<=Nbins;j++){
      std::cout<<"bin "<<j<< " Bin content " << h->GetBinContent(j) << endl;
      if(j<=30)
	y=30;	
      else if(j<=110)
	y=0;			
      else{
	//x=j-110;
	int multiple=(j-111)/8;
	int rem=(j-111)%8;
	if(rem<2)
	  y=2*multiple+1;
	else
	  y=2*multiple+2;
      }
      int Oldbin=j-30+y;
      double val=h->GetBinContent(Oldbin);
      //std::cout<<" New bin "<<j<<" Old bin "<<j-30+y<<" Bin content "<<val<<endl;
      hNew->SetBinContent(j,val);
    } // close for loop
  }
	
  fOut->cd();

  hNew->Write();	
  fOut->Close();
}
