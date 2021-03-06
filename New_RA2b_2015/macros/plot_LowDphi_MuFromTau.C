#include <cstdio>
using namespace std;

void plot_LowDphi_MuFromTau(){

  gStyle->SetOptStat(0);
  gStyle->SetPalette(1) ; // for better color output
  char tempname[200];
  int W = 600;
  int H = 600;
  int H_ref = 600;
  int W_ref = 800;
  float T = 0.08*H_ref;
  float B = 0.12*H_ref;
  float L = 0.12*W_ref;
  float R = 0.04*W_ref;

  TCanvas* canvas = new TCanvas("canvas","canvas",10,10,W,H);
  canvas->SetFillColor(0);
  canvas->SetBorderMode(0);
  canvas->SetFrameFillStyle(0);
  canvas->SetFrameBorderMode(0);
  canvas->SetLeftMargin( L/W );
  canvas->SetRightMargin( R/W );
  canvas->SetTopMargin( T/H );
  canvas->SetBottomMargin( B/H );
  canvas->SetTickx(0);
  canvas->SetTicky(0);

  Float_t legendX1 = .20; //.50;
  Float_t legendX2 = .90; //.70;
  Float_t legendY1 = .68; //.65;
  Float_t legendY2 = .85;
  TLegend* catLeg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);
  catLeg1->SetTextSize(0.032);
  catLeg1->SetTextFont(42);
  catLeg1->SetFillColor(0);
  catLeg1->SetLineColor(0);
  catLeg1->SetBorderSize(0);

  TFile * file = new TFile("InputRootFiles/Probability_Tau_mu_TTbar_plusLowDphi.root","R");
  TH1D * hProb_Tau_mu = (TH1D *) file->Get("hProb_Tau_mu")->Clone();
  TH1D * hProb_Tau_mu_lowDelphi = (TH1D *) file->Get("hProb_Tau_mu_lowDelphi")->Clone();
  
  hProb_Tau_mu->SetLineColor(4);
  hProb_Tau_mu->GetXaxis()->SetTitle("search bin");
  hProb_Tau_mu->GetYaxis()->SetTitle("Probability(#tau -> #mu)");
  hProb_Tau_mu->GetYaxis()->SetTitleOffset(1.2);
  hProb_Tau_mu->SetTitle("");
  hProb_Tau_mu_lowDelphi->SetLineColor(1);
  
  hProb_Tau_mu->Draw();
  hProb_Tau_mu_lowDelphi->Draw("same");
  
  catLeg1->AddEntry(hProb_Tau_mu,"delphi","l");
  catLeg1->AddEntry(hProb_Tau_mu_lowDelphi,"low delphi","l");
  catLeg1->Draw();
  
  canvas->Print("Probability_Tau_mu_TTbar_plusLowDphi.png");
  
  
  
  
  










}