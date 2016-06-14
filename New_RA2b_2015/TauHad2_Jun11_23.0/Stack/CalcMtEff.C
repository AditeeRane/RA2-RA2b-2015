#include <vector>
#include <cstdio>
using namespace std;


CalcMtEff(){
gStyle->SetOptStat(0);  ///to avoid the stat. on the plots
char tempname[200];
 int W = 600;
int H = 600;
int H_ref = 600;
int W_ref = 800;
float T = 0.08*H_ref;
float B = 0.12*H_ref;
float L = 0.12*W_ref;
float R = 0.04*W_ref;

TCanvas* c1 = new TCanvas("name","name",10,10,W,H);
c1->SetFillColor(0);
c1->SetBorderMode(0);
c1->SetFrameFillStyle(0);
c1->SetFrameBorderMode(0);
c1->SetLeftMargin( L/W );
c1->SetRightMargin( R/W );
c1->SetTopMargin( T/H );
c1->SetBottomMargin( B/H );
c1->SetTickx(0);
c1->SetTicky(0);



Float_t legendX1 = .75; //.50;
Float_t legendX2 = .99; //.70;
Float_t legendY1 = .60; //.65;
Float_t legendY2 = .75;
TLegend* catLeg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);
catLeg1->SetTextSize(0.032);
catLeg1->SetTextFont(42);

TFile * before = new TFile("AfterMTBeforeIsoHadTauEstimation_stacked.root","R");
TFile * after = new TFile("BeforeMTandIsoHadTauEstimation_stacked.root","R");
TFile * before_TTbar = new TFile("AfterMTBeforeIsoHadTauEstimation_TTbar_stacked.root","R");
TFile * after_TTbar = new TFile("BeforeMTandIsoHadTauEstimation_TTbar_stacked.root","R");
TFile * before_WJet = new TFile("AfterMTBeforeIsoHadTauEstimation_WJet_stacked.root","R");
TFile * after_WJet = new TFile("BeforeMTandIsoHadTauEstimation_WJet_stacked.root","R");


TH1D * thist, *thist_TTbar, *thist_WJet;
TH1D * thist2, *thist_TTbar2, *thist_WJet2;
THStack * tempstack;

// WJet
THStack * tempstack_WJet = (THStack *) before_WJet->Get("searchH")->Clone("after");
thist_WJet = (TH1D*) tempstack_WJet->GetStack()->Last();
tempstack_WJet = (THStack *) after_WJet->Get("searchH")->Clone("before");
thist_WJet2 = (TH1D*) tempstack_WJet->GetStack()->Last();
TH1D * MtCutEff_WJet = static_cast<TH1D*>(thist_WJet->Clone("MtCutEff_WJet"));
MtCutEff_WJet->Divide(thist_WJet,thist_WJet2,1,1,"B");
MtCutEff_WJet->SetLineColor(2);
MtCutEff_WJet->SetMinimum(0.);
MtCutEff_WJet->SetMaximum(2.);
catLeg1->AddEntry(MtCutEff_WJet,"WJet","l");
MtCutEff_WJet->Draw();



// TTbar
THStack * tempstack_TTbar = (THStack *) before_TTbar->Get("searchH")->Clone("after");
thist_TTbar = (TH1D*) tempstack_TTbar->GetStack()->Last();
tempstack_TTbar = (THStack *) after_TTbar->Get("searchH")->Clone("before");
thist_TTbar2 = (TH1D*) tempstack_TTbar->GetStack()->Last();
TH1D * MtCutEff_TTbar = static_cast<TH1D*>(thist_TTbar->Clone("MtCutEff_TTbar"));
MtCutEff_TTbar->Divide(thist_TTbar,thist_TTbar2,1,1,"B");
MtCutEff_TTbar->SetLineColor(4);
MtCutEff_TTbar->Draw("same");
catLeg1->AddEntry(MtCutEff_TTbar,"T#bar{T}","l");

// TTbar+WJet
tempstack = (THStack *) before->Get("searchH")->Clone("after");
thist = (TH1D*) tempstack->GetStack()->Last();
tempstack = (THStack *) after->Get("searchH")->Clone("before");
thist2 = (TH1D*) tempstack->GetStack()->Last();
TH1D * MtCutEff = static_cast<TH1D*>(thist->Clone("MtCutEff"));
MtCutEff->Divide(thist,thist2,1,1,"B");
MtCutEff->SetLineColor(3);
//MtCutEff->Draw("same");

catLeg1->Draw();
sprintf(tempname,"MtEff.png");
c1->Print(tempname);

TFile * outFile = new TFile("MtEff_stacked.root","RECREATE");
MtCutEff_WJet->Write();
MtCutEff_TTbar->Write();
MtCutEff->Write();
outFile->Close();


}
