#include <vector>
#include <cstdio>
using namespace std;


CalcMtEff(string Elog="Elog433_",string outStr=""){
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



Float_t legendX1 = .7; //.50;
Float_t legendX2 = .9; //.70;
Float_t legendY1 = .70; //.65;
Float_t legendY2 = .85;
TLegend* catLeg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);
catLeg1->SetTextSize(0.032);
catLeg1->SetTextFont(42);
catLeg1->SetTextSize(0.04);
catLeg1->SetFillColor(0);
catLeg1->SetLineColor(0);
catLeg1->SetBorderSize(0);

TH1D * thist_tt, * thist_wj;
TH1D * thist_tt2, * thist_wj2;
TH1D * thist_t, * thist_t2;
THStack * stack;

sprintf(tempname,"Stack/%sAfterMT_HadTauEstimation_TTbar_stacked.root",Elog.c_str());
TFile * after_tt = new TFile(tempname,"R");
sprintf(tempname,"Stack/%sBeforeMT_HadTauEstimation_TTbar_stacked.root",Elog.c_str());
TFile * before_tt = new TFile(tempname,"R");
sprintf(tempname,"Stack/%sAfterMT_HadTauEstimation_WJet_stacked.root",Elog.c_str());
TFile * after_wj = new TFile(tempname,"R");
sprintf(tempname,"Stack/%sBeforeMT_HadTauEstimation_WJet_stacked.root",Elog.c_str());
TFile * before_wj = new TFile(tempname,"R");
/*
sprintf(tempname,"Stack/%sAfterMT_HadTauEstimation_T_stacked.root",Elog.c_str());
TFile * after_t = new TFile(tempname,"R");
sprintf(tempname,"Stack/%sBeforeMT_HadTauEstimation_T_stacked.root",Elog.c_str());
TFile * before_t = new TFile(tempname,"R");
*/
sprintf(tempname,"MtEff_%s_.root",outStr.c_str());
TFile * outFile = new TFile(tempname,"RECREATE");

////////
// ttbar
////////

stack = (THStack *) after_tt->Get("searchH")->Clone("after");
thist_tt = (TH1D *) stack->GetStack()->Last();
stack = (THStack *) before_tt->Get("searchH")->Clone("before");
thist_tt2 = (TH1D *) stack->GetStack()->Last(); 

TH1D * MtCutEff_tt = static_cast<TH1D*>(thist_tt->Clone("MtCutEff_tt"));
MtCutEff_tt->Divide(thist_tt,thist_tt2,1,1,"B");
//
//one time drawing options
//
double XUp = 46. , maxVal=2.;
  MtCutEff_tt->SetMaximum(maxVal);
    MtCutEff_tt->SetTitle("");
    MtCutEff_tt->GetXaxis()->SetLabelFont(42);
    MtCutEff_tt->GetXaxis()->SetLabelOffset(0.007);
    MtCutEff_tt->GetXaxis()->SetLabelSize(0.04);
    MtCutEff_tt->GetXaxis()->SetTitleSize(0.04);
    MtCutEff_tt->GetXaxis()->SetTitleOffset(0.9);
    MtCutEff_tt->GetXaxis()->SetTitleFont(42);
    MtCutEff_tt->GetYaxis()->SetLabelFont(42);
    MtCutEff_tt->GetYaxis()->SetLabelOffset(0.007);
    MtCutEff_tt->GetYaxis()->SetLabelSize(0.04);
    MtCutEff_tt->GetYaxis()->SetTitleSize(0.04);
    MtCutEff_tt->GetYaxis()->SetTitleOffset(1.25);
    MtCutEff_tt->GetYaxis()->SetTitleFont(42);
    MtCutEff_tt->GetXaxis()->SetTitle("search bins");
    MtCutEff_tt->GetYaxis()->SetTitle("MT efficiency");
    MtCutEff_tt->GetXaxis()->SetRangeUser(1.,XUp);
    MtCutEff_tt->SetMaximum(maxVal);
//
//end of drawing options
//
MtCutEff_tt->SetLineColor(1);
MtCutEff_tt->Draw("same");
MtCutEff_tt->Write();

////////
// wjet
////////
stack = (THStack *) after_wj->Get("searchH")->Clone("after");
thist_wj = (TH1D *) stack->GetStack()->Last();
stack = (THStack *) before_wj->Get("searchH")->Clone("before");
thist_wj2 = (TH1D *) stack->GetStack()->Last();

TH1D * MtCutEff_wj = static_cast<TH1D*>(thist_wj->Clone("MtCutEff_wj"));
MtCutEff_wj->Divide(thist_wj,thist_wj2,1,1,"B");
MtCutEff_wj->SetLineColor(2);
MtCutEff_wj->Draw("same");
MtCutEff_wj->Write();


////////
// single top
////////
/*
stack = (THStack *) after_t->Get("searchH")->Clone("after");
thist_t = (TH1D *) stack->GetStack()->Last();
stack = (THStack *) before_t->Get("searchH")->Clone("before");
thist_t2 = (TH1D *) stack->GetStack()->Last();

TH1D * MtCutEff_t = static_cast<TH1D*>(thist_t->Clone("MtCutEff_t"));
MtCutEff_t->Divide(thist_t,thist_t2,1,1,"B");
MtCutEff_t->SetLineColor(2);
MtCutEff_t->Draw("same");
MtCutEff_t->Write();
*/

////////
// tt+wj
////////
TH1D * thist_tot = static_cast<TH1D*>(thist_tt->Clone("thist_tot"));
thist_tot->Add(thist_wj);
thist_tot->Add(thist_t);
TH1D * thist_tot2 = static_cast<TH1D*>(thist_tt2->Clone("thist_tot2"));
thist_tot2->Add(thist_wj2);
thist_tot2->Add(thist_t2);
TH1D * MtCutEff_tot = static_cast<TH1D*>(thist_tot->Clone("MtCutEff"));
MtCutEff_tot->Divide(thist_tot,thist_tot2,1,1,"B");
MtCutEff_tot->SetLineColor(3);
MtCutEff_tot->SetLineStyle(2);
MtCutEff_tot->SetLineWidth(2);
MtCutEff_tot->Draw("same");
MtCutEff_tot->Write();


    sprintf(tempname,"t#bar{t}");
    catLeg1->AddEntry(MtCutEff_tt,tempname,"l");
    sprintf(tempname,"WJet");
    catLeg1->AddEntry(MtCutEff_wj,tempname,"l");
    catLeg1->AddEntry(MtCutEff_tot,"t#bar{t} + WJet + top","l");
    catLeg1->Draw();

sprintf(tempname,"MtEffi_%s_.png",outStr.c_str());
c1->Print(tempname);

//////////
//////////
//LowDphi
//////////
//////////
TCanvas* c2 = new TCanvas("name","name",10,10,W,H);
c2->SetFillColor(0);
c2->SetBorderMode(0);
c2->SetFrameFillStyle(0);
c2->SetFrameBorderMode(0);
c2->SetLeftMargin( L/W );
c2->SetRightMargin( R/W );
c2->SetTopMargin( T/H );
c2->SetBottomMargin( B/H );
c2->SetTickx(0);
c2->SetTicky(0);

TLegend* catLeg2 = new TLegend(legendX1,legendY1,legendX2,legendY2);
catLeg2->SetTextSize(0.032);
catLeg2->SetTextFont(42);
catLeg2->SetTextSize(0.04);
  catLeg2->SetFillColor(0);
  catLeg2->SetLineColor(0);
  catLeg2->SetBorderSize(0);

////////
// ttbar
////////
stack = (THStack *) after_tt->Get("searchH_lowDphi")->Clone("after");
thist_tt = (TH1D *) stack->GetStack()->Last();
stack = (THStack *) before_tt->Get("searchH_lowDphi")->Clone("before");
thist_tt2 = (TH1D *) stack->GetStack()->Last(); 

TH1D * MtCutEff_tt = static_cast<TH1D*>(thist_tt->Clone("MtCutEff_tt_lowDphi"));
MtCutEff_tt->Divide(thist_tt,thist_tt2,1,1,"B");
//
//one time drawing options
//
double XUp = 46. , maxVal=2.;
  MtCutEff_tt->SetMaximum(maxVal);
    MtCutEff_tt->SetTitle("");
    MtCutEff_tt->GetXaxis()->SetLabelFont(42);
    MtCutEff_tt->GetXaxis()->SetLabelOffset(0.007);
    MtCutEff_tt->GetXaxis()->SetLabelSize(0.04);
    MtCutEff_tt->GetXaxis()->SetTitleSize(0.04);
    MtCutEff_tt->GetXaxis()->SetTitleOffset(0.9);
    MtCutEff_tt->GetXaxis()->SetTitleFont(42);
    MtCutEff_tt->GetYaxis()->SetLabelFont(42);
    MtCutEff_tt->GetYaxis()->SetLabelOffset(0.007);
    MtCutEff_tt->GetYaxis()->SetLabelSize(0.04);
    MtCutEff_tt->GetYaxis()->SetTitleSize(0.04);
    MtCutEff_tt->GetYaxis()->SetTitleOffset(1.25);
    MtCutEff_tt->GetYaxis()->SetTitleFont(42);
    MtCutEff_tt->GetXaxis()->SetTitle("search bins");
    MtCutEff_tt->GetYaxis()->SetTitle("MT efficiency");
    MtCutEff_tt->GetXaxis()->SetRangeUser(1.,XUp);
    MtCutEff_tt->SetMaximum(maxVal);
//
//end of drawing options
//
MtCutEff_tt->SetLineColor(1);
MtCutEff_tt->Draw("same");
MtCutEff_tt->Write();

////////
// wjet
////////
stack = (THStack *) after_wj->Get("searchH_lowDphi")->Clone("after");
thist_wj = (TH1D *) stack->GetStack()->Last();
stack = (THStack *) before_wj->Get("searchH_lowDphi")->Clone("before");
thist_wj2 = (TH1D *) stack->GetStack()->Last();

TH1D * MtCutEff_wj = static_cast<TH1D*>(thist_wj->Clone("MtCutEff_wj_lowDphi"));
MtCutEff_wj->Divide(thist_wj,thist_wj2,1,1,"B");
MtCutEff_wj->SetLineColor(2);
MtCutEff_wj->Draw("same");
MtCutEff_wj->Write();

////////
// top
////////
/*
stack = (THStack *) after_t->Get("searchH_lowDphi")->Clone("after");
thist_t = (TH1D *) stack->GetStack()->Last();
stack = (THStack *) before_t->Get("searchH_lowDphi")->Clone("before");
thist_t2 = (TH1D *) stack->GetStack()->Last();

TH1D * MtCutEff_t = static_cast<TH1D*>(thist_t->Clone("MtCutEff_t_lowDphi"));
MtCutEff_t->Divide(thist_t,thist_t2,1,1,"B");
MtCutEff_t->SetLineColor(2);
MtCutEff_t->Draw("same");
MtCutEff_t->Write();
*/
////////
// tt+wj
////////
TH1D * thist_tot = static_cast<TH1D*>(thist_tt->Clone("thist_tot_lowDphi"));
thist_tot->Add(thist_wj);
//thist_tot->Add(thist_t);
TH1D * thist_tot2 = static_cast<TH1D*>(thist_tt2->Clone("thist_tot2_lowDphi"));
thist_tot2->Add(thist_wj2);
//thist_tot2->Add(thist_t2);
TH1D * MtCutEff_tot = static_cast<TH1D*>(thist_tot->Clone("MtCutEff_lowDphi"));
MtCutEff_tot->Divide(thist_tot,thist_tot2,1,1,"B");
MtCutEff_tot->SetLineColor(3);
MtCutEff_tot->SetLineStyle(2);
MtCutEff_tot->SetLineWidth(2);
MtCutEff_tot->Draw("same");
MtCutEff_tot->Write();


    sprintf(tempname,"t#bar{t}");
    catLeg2->AddEntry(MtCutEff_tt,tempname,"l");
    sprintf(tempname,"WJet");
    catLeg2->AddEntry(MtCutEff_wj,tempname,"l");
    catLeg2->AddEntry(MtCutEff_tot,"t#bar{t} + WJet + top","l");
    catLeg2->Draw();

sprintf(tempname,"MtEff_lowDphi_%s_.png",outStr.c_str());
c2->Print(tempname);


/*
stack = (THStack *)after_tt->Get("searchH_lowDphi")->Clone("after_lowDphi");
thist_tt = (TH1D *) stack->GetStack()->Last();
stack = (THStack *)before_tt->Get("searchH_lowDphi")->Clone("before_lowDphi");
thist_tt2 = (TH1D *) stack->GetStack()->Last();

TH1D * MtCutEff_lowDphi = static_cast<TH1D*>(thist_tt->Clone("MtCutEff_lowDphi"));
MtCutEff_lowDphi->Divide(thist_tt,thist_tt2,1,1,"B");

MtCutEff_lowDphi->Draw();
sprintf(tempname,"MtEff_lowDphi.png");
c1->Print(tempname);

MtCutEff_lowDphi->Write();
*/

outFile->Close();


}
