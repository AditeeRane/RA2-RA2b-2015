#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include "TH1.h"
#include "TH1D.h"

enum icol {ic_kh=kRed, ic_arld=kBlue};

void compareMTEff_ARvsKH() {
  compareMTEff_ARvsKH("MtCutEff");
}

void compareMTEff_ARvsKH(char *hname) {

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  
  TFile *ar = TFile::Open("ARElog35_MtEff.root");
  TFile *kh = TFile::Open("Elog433_MtEff.root");

  TH1D *h_ar  = (TH1D*) ar->FindObjectAny(hname);
  TH1D *h_kh  = (TH1D*) kh->FindObjectAny(hname);
  h_kh->SetLineColor  (ic_kh);
  h_kh->SetMarkerColor(ic_kh);

  char hname2[200];
  sprintf(hname2, "%s_lowDphi", hname);
  TH1D *h_arld= (TH1D*) ar->FindObjectAny(hname2);
  h_arld->SetLineColor  (ic_arld);
  h_arld->SetMarkerColor(ic_arld);
    
  h_ar->GetYaxis()->SetRangeUser(0.2, 1.0);
  h_kh->GetYaxis()->SetRangeUser(0.2, 1.0);

  h_ar  ->GetXaxis()->SetTitle("Eff bin");
  h_arld->GetXaxis()->SetTitle("Eff bin");
  h_kh  ->GetXaxis()->SetTitle("Eff bin");
  h_ar  ->GetYaxis()->SetTitle("MT Cut Eff.");
  h_arld->GetYaxis()->SetTitle("MT Cut Eff.");
  h_kh  ->GetYaxis()->SetTitle("MT Cut Eff.");

  char cname[200];
  sprintf(cname, "c_%s_SignalAndLowDPhi", hname);
  TCanvas *c = new TCanvas(cname, cname, 700, 500);
  h_kh  ->Draw();
  h_ar  ->Draw("sames");
  h_arld->Draw("sames");

  TLegend *leg = new TLegend(0.58, 0.13, 0.85, 0.33);
  leg->SetHeader("ttbar + Wjets");
  leg->AddEntry(h_kh,   "Elog433 MTEff (Signal)", "lp");
  leg->AddEntry(h_ar,   "Updated MTEff (signal)", "lp");
  leg->AddEntry(h_arld, "low-dPhi MTEff (QCD)", "lp");
  leg->Draw();

  char cname2[200];
  sprintf(cname2, "%s.pdf", cname);
  c->Print(cname2);
  sprintf(cname2, "%s.png", cname);
  c->Print(cname2);
  
}
