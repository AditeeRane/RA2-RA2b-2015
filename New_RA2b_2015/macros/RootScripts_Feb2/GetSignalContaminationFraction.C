#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

//* AR, Mar01, 2017-Change filenames, arguments of GetInputs function, mother and daughter array accordingly.

using namespace std;

//int mother[]={1700,1600,1600,1000};
//int daughter[]={1,600,800,800};
void GetSignalContaminationFraction(){
  int mother[]={1700,1600,1600,1000};
  int daughter[]={1,600,800,800}; 
 //  GetInputs("T1tttt_SignalFiles_recoMHT_NoPUWt.root","ARElog116_35.9ifb_HadTauEstimation_data_formatted_V12.root","mGluino","mLSP");
  //GetInputs("T1tttt_SignalFiles_recoMHT_NoPUWt.root","ARElog116_35.9ifb_HadTauEstimation_data_SingleMuon_V12_.root","mGluino","mLSP");
  GetInputs("T5qqqqVV_SignalFiles_recoMHT_NoPUWt.root","ARElog116_35.9ifb_HadTauEstimation_data_SingleMuon_V12_.root","mGluino","mLSP","T5qqqqVV",4,mother,daughter);
  //GetInputs("T1bbtt_SignalFiles_recoMHT_NoPUWt.root","ARElog116_35.9ifb_HadTauEstimation_data_SingleMuon_V12_.root","mGluino","mLSP");
  //GetInputs("T1tbtb_SignalFiles_recoMHT_NoPUWt.root","ARElog116_35.9ifb_HadTauEstimation_data_SingleMuon_V12_.root","mGluino","mLSP");
  //GetInputs("T1tbtt_SignalFiles_recoMHT_NoPUWt.root","ARElog116_35.9ifb_HadTauEstimation_data_SingleMuon_V12_.root","mGluino","mLSP");
  //    GetInputs("T2tt_SignalFiles_recoMHT_NoPUWt.root","ARElog116_35.9ifb_HadTauEstimation_data_SingleMuon_V12_.root","mStop","mLSP");

}



void GetInputs(char* WJetTTbarMC,char* Wgun, char* IdOne, char* IdTwo, char* sample, int nMassPoints, int mother[], int daughter[]){

  THStack *tempstack;
  //Open files
  TFile *_file0 = TFile::Open(WJetTTbarMC);
  TFile *_file1 = TFile::Open(Wgun);

  //Get data prediction histogram
  //  TH1D *DataHist=(TH1D*)_file1->Get("searchBin_nominal");

  TH1D *DataHist=(TH1D*)_file1->Get("searchH_b");


  //std::cout<<"****CH1*****"<<endl;
  //create a vector of relevant histgrams
  //T2tt
  //int mother[]={800,800,600,400,450,300,700,175};
  //int daughter[]={1,300,300,300,250,200,50,1};
  //double xsec[]={0.0283338,0.0283338,0.174599,1.83537,0.948333,8.51615,0.0670476,121.42}; 

  //T1tttt or T1tbtb
  //int mother[]={1600,1600,1600,1200,1400};
  //int daughter[]={1,400,600,800,800};
  //double xsec[]={0.00810078,0.00810078,0.00810078,0.0856418,0.0252977}; 
  
  //T5qqqqVV
  //  int mother_T5qqqqVV[]={1700,1600,1600,1000};
  //int daughter_T5qqqqVV[]={1,600,800,800};
  //double xsec[]={0.00810078,0.00810078,0.00810078,0.0856418,0.0252977}; 
  


  char hIdOne[500];
  char hIdTwo[500];
  char hname[500];
  double ScaleLumi=35900;
  vector<TH1D*> SigHists;
  vector<string>SigHistNames;
  vector<double>RelSigContamination;
  int RelMassPoints=nMassPoints;
  sprintf(hIdOne,"%s",IdOne);
  sprintf(hIdTwo,"%s",IdTwo);

  _file0->cd();                                            

  for(int i=0;i<RelMassPoints;i++){
    sprintf(hname,"%s_%i_%s_%i",hIdOne,mother[i],hIdTwo,daughter[i]);
    SigHistNames.push_back(hname);
    //std::cout<<" i "<<i<<" hname "<<hname<<endl;
    TDirectory *demo    = (TDirectory*)_file0->FindObjectAny("SignalContamination");
    TH1D *RelHist=(TH1D*)demo->Get(hname);
    SigHists.push_back(RelHist);
    
  }
  //std::cout<<" RelHist_size "<<SigHists.size()<<endl;

  for(int i=0;i<RelMassPoints;i++){
    //Define array to find bins with max. Sig/SM
    int MaxBinArray[]={0,0,0,0,0};

    //Get signal histogram 
    TH1D* hSig=(TH1D*)SigHists[i]->Clone("hSig");
    
    //In WrapUp*Contamination.C lumi was scaled to 1/pb. So now we need to scale it to real luminosity in order to be consistent with data.
    hSig->Scale(ScaleLumi);

    //Get sig/SM ratio
    TH1D* hSigVsSM=(TH1D*)hSig->Clone("hSigVsSM");
    hSigVsSM->Divide(DataHist);
    int NumBins=hSigVsSM->GetXaxis()->GetNbins();
    /*
    for(int k=1;k<=NumBins;k++){
      std::cout<<" k "<< k << " sig_beforescale "<<SigHists[i]->GetBinContent(k)<< " sig_afterscale "<<hSig->GetBinContent(k)<<" data "<<DataHist->GetBinContent(k)<<" hSigVsSM "<<hSigVsSM->GetBinContent(k)<<endl;
    }
    */

    //select 5 bins with maximum ratio
    for(int j=0;j<=4;j++){
      //std::cout<<" j "<<j<< " B0 "<<MaxBinArray[0]<< " B1 "<<MaxBinArray[1]<< " B2 "<<MaxBinArray[2]<< " B3 "<<MaxBinArray[3]<< " B4 "<<MaxBinArray[4]<<endl;
      double maxRatio=-1.0;
      int maxBin=-1;
      for(int indx=1;indx<=NumBins;indx++){
	if(DataHist->GetBinContent(indx)<0.5)
	  continue;
	if(indx==MaxBinArray[0] || indx==MaxBinArray[1] ||indx==MaxBinArray[2] || indx==MaxBinArray[3] || indx==MaxBinArray[4])
	  continue;
	if(hSigVsSM->GetBinContent(indx)>maxRatio){
	  maxRatio=hSigVsSM->GetBinContent(indx);
	  maxBin=indx;
	}
      }
      //std::cout<<" j "<<j<<"  maxBin "<<maxBin<<" maxRatio "<<maxRatio<<endl;
      MaxBinArray[j]=maxBin;
    }

    //Get relative signal contamination by averaging over these 5 bins
    double Numerator=0.0;
    double Denominator=0.0;

    //Get Sig+SM to get relative contamination fraction
    TH1D* hSigPlusSM=(TH1D*)hSig->Clone("hSigPlusSM");
    hSigPlusSM->Add(DataHist);
    /*  
    for(int k=1;k<=NumBins;k++){
      std::cout<<" k "<< k << " sig "<<hSig->GetBinContent(k)<<" data "<<DataHist->GetBinContent(k)<<" hSigPlusSM "<<hSigPlusSM->GetBinContent(k)<<endl;
    }
*/
    //Find relative contamination
    std::cout<<"****************************"<<endl;
    std::cout<<" mass point "<<SigHistNames[i]<<endl;
    for(int j=0;j<=4;j++){
      //      std::cout<<" j "<<j<<" Bin_j "<<MaxBinArray[j]<<" (Sig/SM)_j "<<hSig->GetBinContent(MaxBinArray[j])<<endl; 
      Numerator +=hSig->GetBinContent(MaxBinArray[j]);
      Denominator +=hSigPlusSM->GetBinContent(MaxBinArray[j]);
      std::cout<< " j "<<j<<" Bin_j "<<MaxBinArray[j]<<" (Sig/SM)_j "<<hSig->GetBinContent(MaxBinArray[j])/DataHist->GetBinContent(MaxBinArray[j])<<" Sig_j "<<hSig->GetBinContent(MaxBinArray[j])<< " Numerator "<<Numerator<<" SM_j "<<DataHist->GetBinContent(MaxBinArray[j])<<" Sig_j+SM_j "<<hSigPlusSM->GetBinContent(MaxBinArray[j])<<" Denominator "<<Denominator<< endl;
    }
    double Contamination=Numerator/Denominator;
    std::cout<<" (Sig_j/(Sig_j+SM_j))_j0To4 "<<Contamination<<endl;
    RelSigContamination.push_back(Contamination);
  }

  /*
  for(int i=0;i<RelMassPoints;i++){
    std::cout<<" mass point "<<SigHistNames[i]<<" RelSigContamination "<<RelSigContamination[i]<<endl;
  }
*/
}

