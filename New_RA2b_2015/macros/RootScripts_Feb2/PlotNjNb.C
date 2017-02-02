
#include "Riostream.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include "TRandom3.h"
#include "TMath.h"
  
  // Version of TTree ROOT analize 
  // and using MINUIT for fitting
  // =============================
void PlotNjNb(){

  int col1 = kRed;
  int col2 = kBlue;
  gStyle->SetOptFit(101);
  //double Energy[]={0.4,0.8,1.5,3,6,10,25,50,100,200,500,1000};
  //double mean[]={35,62,80,90,100,110,120,130,140,150,160,170};
  //  double NjNb[]={1.1171,0.926828,0.830521,1.02748,0.914626,0.948882,0.877389,1.02009,0.941789,0.943062,0.819834,1.03966,0.974895,0.947101,0.828842,0.90241,0.925318,0.929288,0.866872};
  //ARElog111_104:  0.99487,0.895451,0.781671,0.943664,0.967502,0.947177,1.09334,0.981133,1.0025,1.02348,1.04245,1.03182,1.04916,1.03147,1.14096,0.860746,0.955177,1.07101,1.12201
  
  double NjNb[]={0.99487,0.895451,0.781671,0.943664,0.967502,0.947177,1.09334,0.981133,1.0025,1.02348,1.04245,1.03182,1.04916,1.03147,1.14096,0.860746,0.955177,1.07101,1.12201};

  int Xaxis[]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
  const char *Bins[40]={"Njet2_nb0","Njet2_nb1","Njet2_nb2","Njet34_nb0","Njet34_nb1","Njet34_nb2","Njet34_nb3","Njet56_nb0","Njet56_nb1","Njet56_nb2","Njet56_nb3","Njet78_nb0","Njet78_nb1","Njet78_nb2","Njet78_nb3","Njet9_nb0","Njet9_nb1","Njet9_nb2","Njet9_nb3"};
    
//double sigma[]={0.9,1.15,1.358,1.532,1.683,2.142,2.467,2.719,2.925,3.099,3.25,3.502,4.285};

  //TF1 * fE = new TF1("fE", "[0]+[1]*log(x)",2);
  //fE->FixParameter(1,1.13);
  //fE->FixParameter(0,3.502);

  //  TF1 * sE = new TF1("sE", "[0]+[1]*log(x)",2);
  //sE->FixParameter(1,13.02);
  //sE->FixParameter(0,80.01);
  //sE->SetParameter(1,13.02);
  //sE->SetParameter(0,80.01);

  TH1D * Plot=new TH1D("NjNb vs Bins","NjNb vs Bins",19,1,19);
  
  for(int i=1;i<=19;i++){
    Plot->SetBinContent(i,NjNb[i-1]);
    //Plot->GetXAxis()->SetBinLabel(i,Bins[i-1].c_str());
  }
  Plot->SetStats(0);
  //int EnergySize=sizeof(NjNb)/sizeof(double);
 
  //TGraph *IonStop = new TGraph(EnergySize,Bins,NjNb);
  //IonStop->SetTitle("NjNb vs Bins");
  gStyle->SetOptTitle(0);
  TCanvas *b = new TCanvas("NjNb vs Bins", "NjNb vs Bins",500, 500);
  Plot->SetMarkerStyle(20);
  Plot->SetMarkerSize(1.2);
  Plot->GetYaxis()->SetRangeUser(0.4,1.6);
  Plot->GetXaxis()->SetTitle("NjNb bins integrated over HT-MHT" );  
  Plot->Draw("Pl");
  TLine *l = new TLine(1, 0.6, 19, 0.6);
  l->SetLineStyle(4);
  l->Draw("same");
  TLine *l_2 = new TLine(1, 0.8, 19, 0.8);
  l_2->SetLineStyle(4);
  l_2->Draw("same");
  TLine *l_3 = new TLine(1, 1.0, 19, 1.0);
  l_3->SetLineStyle(4);
  l_3->Draw("same");
  TLine *l_4 = new TLine(1, 1.2, 19, 1.2);
  l_4->SetLineStyle(4);
  l_4->Draw("same");
  TLine *l_5 = new TLine(1, 1.4, 19, 1.4);
  l_5->SetLineStyle(4);
  l_5->Draw("same");
  
  //for(int i=1;i<=19;i++){
  //Plot->GetXAxis()->SetBinLabel(i,Bins[i-1]);
  //}
  gPad->Update();
  //IonStop->Fit("sE","","",0.05,1000);
  
//TGraph *StdDev = new TGraph(EnergySize,Energy,sigma);
//StdDev->SetTitle("StdDev;Energy(GeV);StdDev(cm)");
//TCanvas *c = new TCanvas("StdDev", "StdDev",500, 500);
//StdDev->Draw("A*");
//StdDev->Fit("fE","","",0.1,1);
//
}

 
