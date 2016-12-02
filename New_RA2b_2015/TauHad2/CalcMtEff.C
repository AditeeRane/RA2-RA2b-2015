#include <vector>
#include <cstdio>
using namespace std;


void CalcMtEff(string Elog="Elog433_",string outStr=""){
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
 double XUp=0;
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
 TH1D * thist_tot,* thist_tot2;
 TH1D * thist_totAfter;
 TH1D * thist_totAfter_nb_njet2;
 TH1D * thist_totAfter_nb_njet2_lowDphi;
 TH1D * thist_totAfter_nb_njet34;
 TH1D * thist_totAfter_nb_njet34_lowDphi;
 TH1D * thist_totAfter_nb_njet56;
 TH1D * thist_totAfter_nb_njet56_lowDphi;
 TH1D * thist_totAfter_nb_njet78;
 TH1D * thist_totAfter_nb_njet78_lowDphi;
 TH1D * thist_totAfter_nb_njet9;
 TH1D * thist_totAfter_nb_njet9_lowDphi;

 TH1D * thist_totBefore;
 TH1D * thist_totBefore_nb_njet2;
 TH1D * thist_totBefore_nb_njet2_lowDphi;
 TH1D * thist_totBefore_nb_njet34;
 TH1D * thist_totBefore_nb_njet34_lowDphi;
 TH1D * thist_totBefore_nb_njet56;
 TH1D * thist_totBefore_nb_njet56_lowDphi;
 TH1D * thist_totBefore_nb_njet78;
 TH1D * thist_totBefore_nb_njet78_lowDphi;
 TH1D * thist_totBefore_nb_njet9;
 TH1D * thist_totBefore_nb_njet9_lowDphi;




 TH1D * MtCutEff_wj,* MtCutEff_tt,* MtCutEff_tot;

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

 MtCutEff_tt = static_cast<TH1D*>(thist_tt->Clone("MtCutEff_tt"));
MtCutEff_tt->Divide(thist_tt,thist_tt2,1,1,"B");



//
//one time drawing options
//
 XUp = 85. , maxVal=2.;
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
    std::cout<<" MtCutEff_tt:after_tt/before_tt being drawn "<<endl;
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
 
 MtCutEff_wj = static_cast<TH1D*>(thist_wj->Clone("MtCutEff_wj"));
 MtCutEff_wj->Divide(thist_wj,thist_wj2,1,1,"B");
 std::cout<<" MtCutEff_wj:after_wj/before_wj being drawn "<<endl;
 MtCutEff_wj->SetLineColor(2);
 MtCutEff_wj->Draw("same");
 MtCutEff_wj->Write();
 




////////
// tt+wj
////////
thist_tot = static_cast<TH1D*>(thist_tt->Clone("thist_tot"));
thist_tot->Add(thist_wj);
thist_tot->Add(thist_tt);
 thist_totAfter = static_cast<TH1D*>(thist_tot->Clone("thist_totAfter"));
 thist_totAfter->Write();
thist_tot2 = static_cast<TH1D*>(thist_tt2->Clone("thist_tot2"));
thist_tot2->Add(thist_wj2);
thist_tot2->Add(thist_tt2);
 thist_totBefore = static_cast<TH1D*>(thist_tot2->Clone("thist_totBefore"));
 thist_totBefore->Write();
MtCutEff_tot = static_cast<TH1D*>(thist_tot->Clone("MtCutEff"));
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
    catLeg1->AddEntry(MtCutEff_tot,"t#bar{t} + WJet ","l");
    catLeg1->Draw();

    //std::cout<<" png file created "<<endl;
    //sprintf(tempname,"MtEffi_%s_.png",outStr.c_str());
    //std::cout<<"png file gets printed"<<endl;
 //c1->Print(tempname);
 c1->Print("MtEffi.png");
 c1->Print("MtEffi.pdf");




////////
// ttbar_njet2
////////
 stack = (THStack *) after_tt->Get("searchH_nb_njet2")->Clone("after");
 thist_tt = (TH1D *) stack->GetStack()->Last();
 stack = (THStack *) before_tt->Get("searchH_nb_njet2")->Clone("before");
 thist_tt2 = (TH1D *) stack->GetStack()->Last();
 //thist_tt =  (TH1D *) after_tt->Get("searchH_nb_njet2")->Clone("after");
 //thist_tt2 =  (TH1D *) before_tt->Get("searchH_nb_njet2")->Clone("before");
 TH1D * MtCutEff_nb_njet2_tt = static_cast<TH1D*>(thist_tt->Clone("MtCutEff_nb_njet2_tt")); 
 MtCutEff_nb_njet2_tt->Divide(thist_tt,thist_tt2,thist_tt2->GetSumOfWeights(),thist_tt->GetSumOfWeights(),"B");
 MtCutEff_nb_njet2_tt->SetName("MtCutEff_nb_njet2_tt");
 MtCutEff_nb_njet2_tt->SetTitle("MtCutEff_nb_njet2_tt");
 MtCutEff_nb_njet2_tt->Write();

////////
// wjet_njet2
////////

 stack = (THStack *) after_wj->Get("searchH_nb_njet2")->Clone("after");
 thist_wj = (TH1D *) stack->GetStack()->Last();
 stack = (THStack *) before_wj->Get("searchH_nb_njet2")->Clone("before");
 thist_wj2 = (TH1D *) stack->GetStack()->Last();
 
 // thist_wj =  (TH1D *) after_wj->Get("searchH_nb_njet2")->Clone("after");
 //thist_wj2 =  (TH1D *) before_wj->Get("searchH_nb_njet2")->Clone("before");
 TH1D * MtCutEff_nb_njet2_wj = static_cast<TH1D*>(thist_wj->Clone("MtCutEff_nb_njet2_wj")); 
 MtCutEff_nb_njet2_wj->Divide(thist_wj,thist_wj2,thist_wj2->GetSumOfWeights(),thist_wj->GetSumOfWeights(),"B");
 MtCutEff_nb_njet2_wj->SetName("MtCutEff_nb_njet2_wj");
 MtCutEff_nb_njet2_wj->SetTitle("MtCutEff_nb_njet2_wj");
 MtCutEff_nb_njet2_wj->Write();


////////
// tt+wj_njet2
////////
 
 thist_tot = static_cast<TH1D*>(thist_tt->Clone("thist_tot"));
 thist_tot->Add(thist_wj);
 thist_tot->Add(thist_tt);
 thist_totAfter_nb_njet2 = static_cast<TH1D*>(thist_tot->Clone("thist_totAfter_nb_njet2"));
 thist_totAfter_nb_njet2->Write();
 thist_tot2 = static_cast<TH1D*>(thist_tt2->Clone("thist_tot2"));
 thist_tot2->Add(thist_wj2);
 thist_tot2->Add(thist_tt2);
 thist_totBefore_nb_njet2 = static_cast<TH1D*>(thist_tot2->Clone("thist_totBefore_nb_njet2"));
 thist_totBefore_nb_njet2->Write();

 TH1D * MtCutEff_tot_nb_njet2 = static_cast<TH1D*>(thist_tot->Clone("MtCutEff"));
 MtCutEff_tot_nb_njet2->Divide(thist_tot,thist_tot2,thist_tot2->GetSumOfWeights(),thist_tot->GetSumOfWeights(),"B");
 MtCutEff_tot_nb_njet2->SetName("MtCutEff_nb_njet2_tot");
 MtCutEff_tot_nb_njet2->SetTitle("MtCutEff_nb_njet2_tot");
 MtCutEff_tot_nb_njet2->Write();
 

////////
// ttbar_njet34
////////
 stack = (THStack *) after_tt->Get("searchH_nb_njet34")->Clone("after");
 thist_tt = (TH1D *) stack->GetStack()->Last();
 stack = (THStack *) before_tt->Get("searchH_nb_njet34")->Clone("before");
 thist_tt2 = (TH1D *) stack->GetStack()->Last();
 
 TH1D * MtCutEff_nb_njet34_tt = static_cast<TH1D*>(thist_tt->Clone("MtCutEff_nb_njet34_tt")); 
 MtCutEff_nb_njet34_tt->Divide(thist_tt,thist_tt2,thist_tt2->GetSumOfWeights(),thist_tt->GetSumOfWeights(),"B");
 MtCutEff_nb_njet34_tt->SetName("MtCutEff_nb_njet34_tt");
 MtCutEff_nb_njet34_tt->SetTitle("MtCutEff_nb_njet34_tt");
 MtCutEff_nb_njet34_tt->Write();

////////
// wjet_njet34
////////

 stack = (THStack *) after_wj->Get("searchH_nb_njet34")->Clone("after");
 thist_wj = (TH1D *) stack->GetStack()->Last();
 stack = (THStack *) before_wj->Get("searchH_nb_njet34")->Clone("before");
 thist_wj2 = (TH1D *) stack->GetStack()->Last();
  
 //thist_wj =  (TH1D *) after_wj->Get("searchH_nb_njet34")->Clone("after");
 //thist_wj2 =  (TH1D *) before_wj->Get("searchH_nb_njet34")->Clone("before");
 TH1D * MtCutEff_nb_njet34_wj = static_cast<TH1D*>(thist_wj->Clone("MtCutEff_nb_njet34_wj")); 
 MtCutEff_nb_njet34_wj->Divide(thist_wj,thist_wj2,thist_wj2->GetSumOfWeights(),thist_wj->GetSumOfWeights(),"B");
 MtCutEff_nb_njet34_wj->SetName("MtCutEff_nb_njet34_wj");
 MtCutEff_nb_njet34_wj->SetTitle("MtCutEff_nb_njet34_wj");
 MtCutEff_nb_njet34_wj->Write();


////////
// tt+wj_njet34
////////
 
 thist_tot = static_cast<TH1D*>(thist_tt->Clone("thist_tot"));
 thist_tot->Add(thist_wj);
 thist_tot->Add(thist_tt);
 thist_totAfter_nb_njet34 = static_cast<TH1D*>(thist_tot->Clone("thist_totAfter_nb_njet34"));
 thist_totAfter_nb_njet34->Write();
 thist_tot2 = static_cast<TH1D*>(thist_tt2->Clone("thist_tot2"));
 thist_tot2->Add(thist_wj2);
 thist_tot2->Add(thist_tt2);
 thist_totBefore_nb_njet34 = static_cast<TH1D*>(thist_tot2->Clone("thist_totBefore_nb_njet34"));
 thist_totBefore_nb_njet34->Write();

 TH1D * MtCutEff_tot_nb_njet34 = static_cast<TH1D*>(thist_tot->Clone("MtCutEff"));
 MtCutEff_tot_nb_njet34->Divide(thist_tot,thist_tot2,thist_tot2->GetSumOfWeights(),thist_tot->GetSumOfWeights(),"B");
 MtCutEff_tot_nb_njet34->SetName("MtCutEff_nb_njet34_tot");
 MtCutEff_tot_nb_njet34->SetTitle("MtCutEff_nb_njet34_tot");
 MtCutEff_tot_nb_njet34->Write();
 


////////
// ttbar_njet56
////////
 
 stack = (THStack *) after_tt->Get("searchH_nb_njet56")->Clone("after");
 thist_tt = (TH1D *) stack->GetStack()->Last();
 stack = (THStack *) before_tt->Get("searchH_nb_njet56")->Clone("before");
 thist_tt2 = (TH1D *) stack->GetStack()->Last();
 
 //thist_tt =  (TH1D *) after_tt->Get("searchH_nb_njet34")->Clone("after");
 //thist_tt2 =  (TH1D *) before_tt->Get("searchH_nb_njet34")->Clone("before");

 TH1D * MtCutEff_nb_njet56_tt = static_cast<TH1D*>(thist_tt->Clone("MtCutEff_nb_njet56_tt")); 
 MtCutEff_nb_njet56_tt->Divide(thist_tt,thist_tt2,thist_tt2->GetSumOfWeights(),thist_tt->GetSumOfWeights(),"B");
 MtCutEff_nb_njet56_tt->SetName("MtCutEff_nb_njet56_tt");
 MtCutEff_nb_njet56_tt->SetTitle("MtCutEff_nb_njet56_tt");
 MtCutEff_nb_njet56_tt->Write();

////////
// wjet_njet56
////////

 stack = (THStack *) after_wj->Get("searchH_nb_njet56")->Clone("after");
 thist_wj = (TH1D *) stack->GetStack()->Last();
 stack = (THStack *) before_wj->Get("searchH_nb_njet56")->Clone("before");
 thist_wj2 = (TH1D *) stack->GetStack()->Last();
 
 // thist_wj =  (TH1D *) after_wj->Get("searchH_nb_njet56")->Clone("after");
 //thist_wj2 =  (TH1D *) before_wj->Get("searchH_nb_njet56")->Clone("before");
 TH1D * MtCutEff_nb_njet56_wj = static_cast<TH1D*>(thist_wj->Clone("MtCutEff_nb_njet56_wj")); 
 MtCutEff_nb_njet56_wj->Divide(thist_wj,thist_wj2,thist_wj2->GetSumOfWeights(),thist_wj->GetSumOfWeights(),"B");
 MtCutEff_nb_njet56_wj->SetName("MtCutEff_nb_njet56_wj");
 MtCutEff_nb_njet56_wj->SetTitle("MtCutEff_nb_njet56_wj");
 MtCutEff_nb_njet56_wj->Write();
////////
// tt+wj_njet56
////////

 thist_tot = static_cast<TH1D*>(thist_tt->Clone("thist_tot"));
 thist_tot->Add(thist_wj);
 thist_tot->Add(thist_tt);
 thist_totAfter_nb_njet56 = static_cast<TH1D*>(thist_tot->Clone("thist_totAfter_nb_njet56"));
 thist_totAfter_nb_njet56->Write();
 thist_tot2 = static_cast<TH1D*>(thist_tt2->Clone("thist_tot2"));
 thist_tot2->Add(thist_wj2);
 thist_tot2->Add(thist_tt2);
 thist_totBefore_nb_njet56 = static_cast<TH1D*>(thist_tot2->Clone("thist_totBefore_nb_njet56"));
 thist_totBefore_nb_njet56->Write();

 TH1D * MtCutEff_tot_nb_njet56 = static_cast<TH1D*>(thist_tot->Clone("MtCutEff"));
 MtCutEff_tot_nb_njet56->Divide(thist_tot,thist_tot2,thist_tot2->GetSumOfWeights(),thist_tot->GetSumOfWeights(),"B");
 MtCutEff_tot_nb_njet56->SetName("MtCutEff_nb_njet56_tot");
 MtCutEff_tot_nb_njet56->SetTitle("MtCutEff_nb_njet56_tot");
 MtCutEff_tot_nb_njet56->Write();



////////
// ttbar_njet78
////////

 stack = (THStack *) after_tt->Get("searchH_nb_njet78")->Clone("after");
 thist_tt = (TH1D *) stack->GetStack()->Last();
 stack = (THStack *) before_tt->Get("searchH_nb_njet78")->Clone("before");
 thist_tt2 = (TH1D *) stack->GetStack()->Last();

 // thist_tt =  (TH1D *) after_tt->Get("searchH_nb_njet78")->Clone("after");
 //thist_tt2 =  (TH1D *) before_tt->Get("searchH_nb_njet78")->Clone("before");
 TH1D * MtCutEff_nb_njet78_tt = static_cast<TH1D*>(thist_tt->Clone("MtCutEff_nb_njet78_tt")); 
 MtCutEff_nb_njet78_tt->Divide(thist_tt,thist_tt2,thist_tt2->GetSumOfWeights(),thist_tt->GetSumOfWeights(),"B");
 MtCutEff_nb_njet78_tt->SetName("MtCutEff_nb_njet78_tt");
 MtCutEff_nb_njet78_tt->SetTitle("MtCutEff_nb_njet78_tt");
 MtCutEff_nb_njet78_tt->Write();

////////
// wjet_njet78
////////

 stack = (THStack *) after_wj->Get("searchH_nb_njet78")->Clone("after");
 thist_wj = (TH1D *) stack->GetStack()->Last();
 stack = (THStack *) before_wj->Get("searchH_nb_njet78")->Clone("before");
 thist_wj2 = (TH1D *) stack->GetStack()->Last();
 
 // thist_wj =  (TH1D *) after_wj->Get("searchH_nb_njet78")->Clone("after");
 //thist_wj2 =  (TH1D *) before_wj->Get("searchH_nb_njet78")->Clone("before");
 TH1D * MtCutEff_nb_njet78_wj = static_cast<TH1D*>(thist_wj->Clone("MtCutEff_nb_njet78_wj")); 
 MtCutEff_nb_njet78_wj->Divide(thist_wj,thist_wj2,thist_wj2->GetSumOfWeights(),thist_wj->GetSumOfWeights(),"B");
 MtCutEff_nb_njet78_wj->SetName("MtCutEff_nb_njet78_wj");
 MtCutEff_nb_njet78_wj->SetTitle("MtCutEff_nb_njet78_wj");
 MtCutEff_nb_njet78_wj->Write();

////////
// tt+wj_njet78
////////

 thist_tot = static_cast<TH1D*>(thist_tt->Clone("thist_tot"));
 thist_tot->Add(thist_wj);
 thist_tot->Add(thist_tt);
 thist_totAfter_nb_njet78 = static_cast<TH1D*>(thist_tot->Clone("thist_totAfter_nb_njet78"));
 thist_totAfter_nb_njet78->Write();
 thist_tot2 = static_cast<TH1D*>(thist_tt2->Clone("thist_tot2"));
 thist_tot2->Add(thist_wj2);
 thist_tot2->Add(thist_tt2);
 thist_totBefore_nb_njet78 = static_cast<TH1D*>(thist_tot2->Clone("thist_totBefore_nb_njet78"));
 thist_totBefore_nb_njet78->Write();

 TH1D * MtCutEff_tot_nb_njet78 = static_cast<TH1D*>(thist_tot->Clone("MtCutEff"));
 MtCutEff_tot_nb_njet78->Divide(thist_tot,thist_tot2,thist_tot2->GetSumOfWeights(),thist_tot->GetSumOfWeights(),"B");
 MtCutEff_tot_nb_njet78->SetName("MtCutEff_nb_njet78_tot");
 MtCutEff_tot_nb_njet78->SetTitle("MtCutEff_nb_njet78_tot");
 MtCutEff_tot_nb_njet78->Write();



////////
// ttbar_njet9
////////
 stack = (THStack *) after_tt->Get("searchH_nb_njet9")->Clone("after");
 thist_tt = (TH1D *) stack->GetStack()->Last();
 stack = (THStack *) before_tt->Get("searchH_nb_njet9")->Clone("before");
 thist_tt2 = (TH1D *) stack->GetStack()->Last();

 // thist_tt =  (TH1D *) after_tt->Get("searchH_nb_njet9")->Clone("after");
 // thist_tt2 =  (TH1D *) before_tt->Get("searchH_nb_njet9")->Clone("before");
 TH1D * MtCutEff_nb_njet9_tt = static_cast<TH1D*>(thist_tt->Clone("MtCutEff_nb_njet9_tt")); 
 MtCutEff_nb_njet9_tt->Divide(thist_tt,thist_tt2,thist_tt2->GetSumOfWeights(),thist_tt->GetSumOfWeights(),"B");
 MtCutEff_nb_njet9_tt->SetName("MtCutEff_nb_njet9_tt");
 MtCutEff_nb_njet9_tt->SetTitle("MtCutEff_nb_njet9_tt");
 MtCutEff_nb_njet9_tt->Write();


////////
// wjet_njet9
////////

 stack = (THStack *) after_wj->Get("searchH_nb_njet9")->Clone("after");
 thist_wj = (TH1D *) stack->GetStack()->Last();
 stack = (THStack *) before_wj->Get("searchH_nb_njet9")->Clone("before");
 thist_wj2 = (TH1D *) stack->GetStack()->Last();
 
 // thist_wj =  (TH1D *) after_wj->Get("searchH_nb_njet9")->Clone("after");
 //thist_wj2 =  (TH1D *) before_wj->Get("searchH_nb_njet9")->Clone("before");
 TH1D * MtCutEff_nb_njet9_wj = static_cast<TH1D*>(thist_wj->Clone("MtCutEff_nb_njet9_wj")); 
 MtCutEff_nb_njet9_wj->Divide(thist_wj,thist_wj2,thist_wj2->GetSumOfWeights(),thist_wj->GetSumOfWeights(),"B");
 MtCutEff_nb_njet9_wj->SetName("MtCutEff_nb_njet9_wj");
 MtCutEff_nb_njet9_wj->SetTitle("MtCutEff_nb_njet9_wj");
 MtCutEff_nb_njet9_wj->Write();


////////
// tt+wj_njet9
////////

 thist_tot = static_cast<TH1D*>(thist_tt->Clone("thist_tot"));
 thist_tot->Add(thist_wj);
 thist_tot->Add(thist_tt);
 thist_totAfter_nb_njet9 = static_cast<TH1D*>(thist_tot->Clone("thist_totAfter_nb_njet9"));
 thist_totAfter_nb_njet9->Write();
 thist_tot2 = static_cast<TH1D*>(thist_tt2->Clone("thist_tot2"));
 thist_tot2->Add(thist_wj2);
 thist_tot2->Add(thist_tt2);
 thist_totBefore_nb_njet9 = static_cast<TH1D*>(thist_tot2->Clone("thist_totBefore_nb_njet9"));
 thist_totBefore_nb_njet9->Write();

 TH1D * MtCutEff_tot_nb_njet9 = static_cast<TH1D*>(thist_tot->Clone("MtCutEff"));
 MtCutEff_tot_nb_njet9->Divide(thist_tot,thist_tot2,thist_tot2->GetSumOfWeights(),thist_tot->GetSumOfWeights(),"B");
 MtCutEff_tot_nb_njet9->SetName("MtCutEff_nb_njet9_tot");
 MtCutEff_tot_nb_njet9->SetTitle("MtCutEff_nb_njet9_tot");
 MtCutEff_tot_nb_njet9->Write();


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


//////////
//////////
//LowDphi
//////////
//////////
 TCanvas* c2 = new TCanvas("nameTwo","nameTwo",10,10,W,H);
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

MtCutEff_tt = static_cast<TH1D*>(thist_tt->Clone("MtCutEff_tt_lowDphi"));
MtCutEff_tt->Divide(thist_tt,thist_tt2,1,1,"B");
//
//one time drawing options
//
 XUp = 73. , maxVal=2.;
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
 std::cout<<".......MtCutEff_tt drawn ................."<<endl;
////////
// wjet
////////
stack = (THStack *) after_wj->Get("searchH_lowDphi")->Clone("after");
thist_wj = (TH1D *) stack->GetStack()->Last();
stack = (THStack *) before_wj->Get("searchH_lowDphi")->Clone("before");
thist_wj2 = (TH1D *) stack->GetStack()->Last();

MtCutEff_wj = static_cast<TH1D*>(thist_wj->Clone("MtCutEff_wj_lowDphi"));
MtCutEff_wj->Divide(thist_wj,thist_wj2,1,1,"B");
MtCutEff_wj->SetLineColor(2);
MtCutEff_wj->Draw("same");
MtCutEff_wj->Write();
 std::cout<<".......MtCutEff_wj drawn ................."<<endl;
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
thist_tot = static_cast<TH1D*>(thist_tt->Clone("thist_tot_lowDphi"));
thist_tot->Add(thist_wj);
thist_tot->Add(thist_tt);
 TH1D * thist_totAfter_lowDphi = static_cast<TH1D*>(thist_tot->Clone("thist_totAfter_lowDphi"));
 thist_totAfter_lowDphi->Write();

thist_tot2 = static_cast<TH1D*>(thist_tt2->Clone("thist_tot2_lowDphi"));
thist_tot2->Add(thist_wj2);
thist_tot2->Add(thist_tt2);
 TH1D * thist_totBefore_lowDphi = static_cast<TH1D*>(thist_tot2->Clone("thist_totBefore_lowDphi"));
 thist_totBefore_lowDphi->Write();

MtCutEff_tot = static_cast<TH1D*>(thist_tot->Clone("MtCutEff_lowDphi"));
MtCutEff_tot->Divide(thist_tot,thist_tot2,1,1,"B");
MtCutEff_tot->SetLineColor(3);
MtCutEff_tot->SetLineStyle(2);
MtCutEff_tot->SetLineWidth(2);
MtCutEff_tot->Draw("same");
MtCutEff_tot->Write();
 std::cout<<".......MtCutEff_lowDphi drawn ................."<<endl;

    sprintf(tempname,"t#bar{t}");
    catLeg2->AddEntry(MtCutEff_tt,tempname,"l");
    sprintf(tempname,"WJet");
    catLeg2->AddEntry(MtCutEff_wj,tempname,"l");
    //catLeg2->AddEntry(MtCutEff_tot,"t#bar{t} + WJet + top","l");
    catLeg2->AddEntry(MtCutEff_tot,"t#bar{t} + WJet","l");
    catLeg2->Draw();

    //sprintf(tempname,"MtEff_lowDphi_%s_.png",outStr.c_str());
 std::cout<<"png file(lowDphi) gets printed"<<endl;
 //c2->Print(tempname);
 c2->Print("MtEffi_lowDphi.png");
 c2->Print("MtEffi_lowDphi.pdf");


////////
// ttbar_njet2_lowDphi
////////
 stack = (THStack *) after_tt->Get("searchH_nb_njet2_lowDphi")->Clone("after");
 thist_tt = (TH1D *) stack->GetStack()->Last();
 stack = (THStack *) before_tt->Get("searchH_nb_njet2_lowDphi")->Clone("before");
 thist_tt2 = (TH1D *) stack->GetStack()->Last();
 //thist_tt =  (TH1D *) after_tt->Get("searchH_nb_njet2_lowDphi")->Clone("after");
 //thist_tt2 =  (TH1D *) before_tt->Get("searchH_nb_njet2_lowDphi")->Clone("before");
 TH1D * MtCutEff_nb_njet2_lowDphi_tt = static_cast<TH1D*>(thist_tt->Clone("MtCutEff_nb_njet2_lowDphi_tt")); 
 MtCutEff_nb_njet2_lowDphi_tt->Divide(thist_tt,thist_tt2,thist_tt2->GetSumOfWeights(),thist_tt->GetSumOfWeights(),"B");
 MtCutEff_nb_njet2_lowDphi_tt->SetName("MtCutEff_nb_njet2_lowDphi_tt");
 MtCutEff_nb_njet2_lowDphi_tt->SetTitle("MtCutEff_nb_njet2_lowDphi_tt");
 MtCutEff_nb_njet2_lowDphi_tt->Write();

////////
// wjet_njet2_lowDphi
////////

 stack = (THStack *) after_wj->Get("searchH_nb_njet2_lowDphi")->Clone("after");
 thist_wj = (TH1D *) stack->GetStack()->Last();
 stack = (THStack *) before_wj->Get("searchH_nb_njet2_lowDphi")->Clone("before");
 thist_wj2 = (TH1D *) stack->GetStack()->Last();
 
 // thist_wj =  (TH1D *) after_wj->Get("searchH_nb_njet2_lowDphi")->Clone("after");
 //thist_wj2 =  (TH1D *) before_wj->Get("searchH_nb_njet2_lowDphi")->Clone("before");
 TH1D * MtCutEff_nb_njet2_lowDphi_wj = static_cast<TH1D*>(thist_wj->Clone("MtCutEff_nb_njet2_lowDphi_wj")); 
 MtCutEff_nb_njet2_lowDphi_wj->Divide(thist_wj,thist_wj2,thist_wj2->GetSumOfWeights(),thist_wj->GetSumOfWeights(),"B");
 MtCutEff_nb_njet2_lowDphi_wj->SetName("MtCutEff_nb_njet2_lowDphi_wj");
 MtCutEff_nb_njet2_lowDphi_wj->SetTitle("MtCutEff_nb_njet2_lowDphi_wj");
 MtCutEff_nb_njet2_lowDphi_wj->Write();


////////
// tt+wj_njet2_lowDphi
////////
 
 thist_tot = static_cast<TH1D*>(thist_tt->Clone("thist_tot"));
 thist_tot->Add(thist_wj);
 thist_tot->Add(thist_tt);
 thist_totAfter_nb_njet2_lowDphi = static_cast<TH1D*>(thist_tot->Clone("thist_totAfter_nb_njet2_lowDphi"));
 thist_totAfter_nb_njet2_lowDphi->Write();
 thist_tot2 = static_cast<TH1D*>(thist_tt2->Clone("thist_tot2"));
 thist_tot2->Add(thist_wj2);
 thist_tot2->Add(thist_tt2);
 thist_totBefore_nb_njet2_lowDphi = static_cast<TH1D*>(thist_tot2->Clone("thist_totBefore_nb_njet2_lowDphi"));
 thist_totBefore_nb_njet2_lowDphi->Write();

 TH1D * MtCutEff_tot_nb_njet2_lowDphi = static_cast<TH1D*>(thist_tot->Clone("MtCutEff"));
 MtCutEff_tot_nb_njet2_lowDphi->Divide(thist_tot,thist_tot2,thist_tot2->GetSumOfWeights(),thist_tot->GetSumOfWeights(),"B");
 MtCutEff_tot_nb_njet2_lowDphi->SetName("MtCutEff_nb_njet2_lowDphi_tot");
 MtCutEff_tot_nb_njet2_lowDphi->SetTitle("MtCutEff_nb_njet2_lowDphi_tot");
 MtCutEff_tot_nb_njet2_lowDphi->Write();


////////
// ttbar_njet34_lowDphi
////////
 stack = (THStack *) after_tt->Get("searchH_nb_njet34_lowDphi")->Clone("after");
 thist_tt = (TH1D *) stack->GetStack()->Last();
 stack = (THStack *) before_tt->Get("searchH_nb_njet34_lowDphi")->Clone("before");
 thist_tt2 = (TH1D *) stack->GetStack()->Last();
 //thist_tt =  (TH1D *) after_tt->Get("searchH_nb_njet34_lowDphi")->Clone("after");
 //thist_tt2 =  (TH1D *) before_tt->Get("searchH_nb_njet34_lowDphi")->Clone("before");
 TH1D * MtCutEff_nb_njet34_lowDphi_tt = static_cast<TH1D*>(thist_tt->Clone("MtCutEff_nb_njet34_lowDphi_tt")); 
 MtCutEff_nb_njet34_lowDphi_tt->Divide(thist_tt,thist_tt2,thist_tt2->GetSumOfWeights(),thist_tt->GetSumOfWeights(),"B");
 MtCutEff_nb_njet34_lowDphi_tt->SetName("MtCutEff_nb_njet34_lowDphi_tt");
 MtCutEff_nb_njet34_lowDphi_tt->SetTitle("MtCutEff_nb_njet34_lowDphi_tt");
 MtCutEff_nb_njet34_lowDphi_tt->Write();

////////
// wjet_njet34_lowDphi
////////

 stack = (THStack *) after_wj->Get("searchH_nb_njet34_lowDphi")->Clone("after");
 thist_wj = (TH1D *) stack->GetStack()->Last();
 stack = (THStack *) before_wj->Get("searchH_nb_njet34_lowDphi")->Clone("before");
 thist_wj2 = (TH1D *) stack->GetStack()->Last();
 
 // thist_wj =  (TH1D *) after_wj->Get("searchH_nb_njet34_lowDphi")->Clone("after");
 //thist_wj2 =  (TH1D *) before_wj->Get("searchH_nb_njet34_lowDphi")->Clone("before");
 TH1D * MtCutEff_nb_njet34_lowDphi_wj = static_cast<TH1D*>(thist_wj->Clone("MtCutEff_nb_njet34_lowDphi_wj")); 
 MtCutEff_nb_njet34_lowDphi_wj->Divide(thist_wj,thist_wj2,thist_wj2->GetSumOfWeights(),thist_wj->GetSumOfWeights(),"B");
 MtCutEff_nb_njet34_lowDphi_wj->SetName("MtCutEff_nb_njet34_lowDphi_wj");
 MtCutEff_nb_njet34_lowDphi_wj->SetTitle("MtCutEff_nb_njet34_lowDphi_wj");
 MtCutEff_nb_njet34_lowDphi_wj->Write();


////////
// tt+wj_njet34_lowDphi
////////
 
 thist_tot = static_cast<TH1D*>(thist_tt->Clone("thist_tot"));
 thist_tot->Add(thist_wj);
 thist_tot->Add(thist_tt);
 thist_totAfter_nb_njet34_lowDphi = static_cast<TH1D*>(thist_tot->Clone("thist_totAfter_nb_njet34_lowDphi"));
 thist_totAfter_nb_njet34_lowDphi->Write();
 thist_tot2 = static_cast<TH1D*>(thist_tt2->Clone("thist_tot2"));
 thist_tot2->Add(thist_wj2);
 thist_tot2->Add(thist_tt2);
 thist_totBefore_nb_njet34_lowDphi = static_cast<TH1D*>(thist_tot2->Clone("thist_totBefore_nb_njet34_lowDphi"));
 thist_totBefore_nb_njet34_lowDphi->Write();

 TH1D * MtCutEff_tot_nb_njet34_lowDphi = static_cast<TH1D*>(thist_tot->Clone("MtCutEff"));
 MtCutEff_tot_nb_njet34_lowDphi->Divide(thist_tot,thist_tot2,thist_tot2->GetSumOfWeights(),thist_tot->GetSumOfWeights(),"B");
 MtCutEff_tot_nb_njet34_lowDphi->SetName("MtCutEff_nb_njet34_lowDphi_tot");
 MtCutEff_tot_nb_njet34_lowDphi->SetTitle("MtCutEff_nb_njet34_lowDphi_tot");

 MtCutEff_tot_nb_njet34_lowDphi->Write();


////////
// ttbar_njet56_lowDphi
////////
 stack = (THStack *) after_tt->Get("searchH_nb_njet56_lowDphi")->Clone("after");
 thist_tt = (TH1D *) stack->GetStack()->Last();
 stack = (THStack *) before_tt->Get("searchH_nb_njet56_lowDphi")->Clone("before");
 thist_tt2 = (TH1D *) stack->GetStack()->Last();
 //thist_tt =  (TH1D *) after_tt->Get("searchH_nb_njet56_lowDphi")->Clone("after");
 //thist_tt2 =  (TH1D *) before_tt->Get("searchH_nb_njet56_lowDphi")->Clone("before");
 TH1D * MtCutEff_nb_njet56_lowDphi_tt = static_cast<TH1D*>(thist_tt->Clone("MtCutEff_nb_njet56_lowDphi_tt")); 
 MtCutEff_nb_njet56_lowDphi_tt->Divide(thist_tt,thist_tt2,thist_tt2->GetSumOfWeights(),thist_tt->GetSumOfWeights(),"B");
 MtCutEff_nb_njet56_lowDphi_tt->SetName("MtCutEff_nb_njet56_lowDphi_tt");
 MtCutEff_nb_njet56_lowDphi_tt->SetTitle("MtCutEff_nb_njet56_lowDphi_tt");
 MtCutEff_nb_njet56_lowDphi_tt->Write();

////////
// wjet_njet56_lowDphi
////////

 stack = (THStack *) after_wj->Get("searchH_nb_njet56_lowDphi")->Clone("after");
 thist_wj = (TH1D *) stack->GetStack()->Last();
 stack = (THStack *) before_wj->Get("searchH_nb_njet56_lowDphi")->Clone("before");
 thist_wj2 = (TH1D *) stack->GetStack()->Last();
 
 // thist_wj =  (TH1D *) after_wj->Get("searchH_nb_njet56_lowDphi")->Clone("after");
 //thist_wj2 =  (TH1D *) before_wj->Get("searchH_nb_njet56_lowDphi")->Clone("before");
 TH1D * MtCutEff_nb_njet56_lowDphi_wj = static_cast<TH1D*>(thist_wj->Clone("MtCutEff_nb_njet56_lowDphi_wj")); 
 MtCutEff_nb_njet56_lowDphi_wj->Divide(thist_wj,thist_wj2,thist_wj2->GetSumOfWeights(),thist_wj->GetSumOfWeights(),"B");
 MtCutEff_nb_njet56_lowDphi_wj->SetName("MtCutEff_nb_njet56_lowDphi_wj");
 MtCutEff_nb_njet56_lowDphi_wj->SetTitle("MtCutEff_nb_njet56_lowDphi_wj");
 MtCutEff_nb_njet56_lowDphi_wj->Write();


////////
// tt+wj_njet56_lowDphi
////////
 
 thist_tot = static_cast<TH1D*>(thist_tt->Clone("thist_tot"));
 thist_tot->Add(thist_wj);
 thist_tot->Add(thist_tt);
 thist_totAfter_nb_njet56_lowDphi = static_cast<TH1D*>(thist_tot->Clone("thist_totAfter_nb_njet56_lowDphi"));
 thist_totAfter_nb_njet56_lowDphi->Write();
 thist_tot2 = static_cast<TH1D*>(thist_tt2->Clone("thist_tot2"));
 thist_tot2->Add(thist_wj2);
 thist_tot2->Add(thist_tt2);
 thist_totBefore_nb_njet56_lowDphi = static_cast<TH1D*>(thist_tot2->Clone("thist_totBefore_nb_njet56_lowDphi"));
 thist_totBefore_nb_njet56_lowDphi->Write();

 TH1D * MtCutEff_tot_nb_njet56_lowDphi = static_cast<TH1D*>(thist_tot->Clone("MtCutEff"));
 MtCutEff_tot_nb_njet56_lowDphi->Divide(thist_tot,thist_tot2,thist_tot2->GetSumOfWeights(),thist_tot->GetSumOfWeights(),"B");
 MtCutEff_tot_nb_njet56_lowDphi->SetName("MtCutEff_nb_njet56_lowDphi_tot");
 MtCutEff_tot_nb_njet56_lowDphi->SetTitle("MtCutEff_nb_njet56_lowDphi_tot");

 MtCutEff_tot_nb_njet56_lowDphi->Write();

////////
// ttbar_njet78_lowDphi
////////
 stack = (THStack *) after_tt->Get("searchH_nb_njet78_lowDphi")->Clone("after");
 thist_tt = (TH1D *) stack->GetStack()->Last();
 stack = (THStack *) before_tt->Get("searchH_nb_njet78_lowDphi")->Clone("before");
 thist_tt2 = (TH1D *) stack->GetStack()->Last();
 //thist_tt =  (TH1D *) after_tt->Get("searchH_nb_njet78_lowDphi")->Clone("after");
 //thist_tt2 =  (TH1D *) before_tt->Get("searchH_nb_njet78_lowDphi")->Clone("before");
 TH1D * MtCutEff_nb_njet78_lowDphi_tt = static_cast<TH1D*>(thist_tt->Clone("MtCutEff_nb_njet78_lowDphi_tt")); 
 MtCutEff_nb_njet78_lowDphi_tt->Divide(thist_tt,thist_tt2,thist_tt2->GetSumOfWeights(),thist_tt->GetSumOfWeights(),"B");
 MtCutEff_nb_njet78_lowDphi_tt->SetName("MtCutEff_nb_njet78_lowDphi_tt");
 MtCutEff_nb_njet78_lowDphi_tt->SetTitle("MtCutEff_nb_njet78_lowDphi_tt");
 MtCutEff_nb_njet78_lowDphi_tt->Write();

////////
// wjet_njet78_lowDphi
////////

 stack = (THStack *) after_wj->Get("searchH_nb_njet78_lowDphi")->Clone("after");
 thist_wj = (TH1D *) stack->GetStack()->Last();
 stack = (THStack *) before_wj->Get("searchH_nb_njet78_lowDphi")->Clone("before");
 thist_wj2 = (TH1D *) stack->GetStack()->Last();
 
 // thist_wj =  (TH1D *) after_wj->Get("searchH_nb_njet78_lowDphi")->Clone("after");
 //thist_wj2 =  (TH1D *) before_wj->Get("searchH_nb_njet78_lowDphi")->Clone("before");
 TH1D * MtCutEff_nb_njet78_lowDphi_wj = static_cast<TH1D*>(thist_wj->Clone("MtCutEff_nb_njet78_lowDphi_wj")); 
 MtCutEff_nb_njet78_lowDphi_wj->Divide(thist_wj,thist_wj2,thist_wj2->GetSumOfWeights(),thist_wj->GetSumOfWeights(),"B");
 MtCutEff_nb_njet78_lowDphi_wj->SetName("MtCutEff_nb_njet78_lowDphi_wj");
 MtCutEff_nb_njet78_lowDphi_wj->SetTitle("MtCutEff_nb_njet78_lowDphi_wj");
 MtCutEff_nb_njet78_lowDphi_wj->Write();


////////
// tt+wj_njet78_lowDphi
////////
 
 thist_tot = static_cast<TH1D*>(thist_tt->Clone("thist_tot"));
 thist_tot->Add(thist_wj);
 thist_tot->Add(thist_tt);
 thist_totAfter_nb_njet78_lowDphi = static_cast<TH1D*>(thist_tot->Clone("thist_totAfter_nb_njet78_lowDphi"));
 thist_totAfter_nb_njet78_lowDphi->Write();
 thist_tot2 = static_cast<TH1D*>(thist_tt2->Clone("thist_tot2"));
 thist_tot2->Add(thist_wj2);
 thist_tot2->Add(thist_tt2);
 thist_totBefore_nb_njet78_lowDphi = static_cast<TH1D*>(thist_tot2->Clone("thist_totBefore_nb_njet78_lowDphi"));
 thist_totBefore_nb_njet78_lowDphi->Write();

 TH1D * MtCutEff_tot_nb_njet78_lowDphi = static_cast<TH1D*>(thist_tot->Clone("MtCutEff"));
 MtCutEff_tot_nb_njet78_lowDphi->Divide(thist_tot,thist_tot2,thist_tot2->GetSumOfWeights(),thist_tot->GetSumOfWeights(),"B");
 MtCutEff_tot_nb_njet78_lowDphi->SetName("MtCutEff_nb_njet78_lowDphi_tot");
 MtCutEff_tot_nb_njet78_lowDphi->SetTitle("MtCutEff_nb_njet78_lowDphi_tot");

 MtCutEff_tot_nb_njet78_lowDphi->Write();

////////
// ttbar_njet9_lowDphi
////////
 stack = (THStack *) after_tt->Get("searchH_nb_njet9_lowDphi")->Clone("after");
 thist_tt = (TH1D *) stack->GetStack()->Last();
 stack = (THStack *) before_tt->Get("searchH_nb_njet9_lowDphi")->Clone("before");
 thist_tt2 = (TH1D *) stack->GetStack()->Last();
 //thist_tt =  (TH1D *) after_tt->Get("searchH_nb_njet9_lowDphi")->Clone("after");
 //thist_tt2 =  (TH1D *) before_tt->Get("searchH_nb_njet9_lowDphi")->Clone("before");
 TH1D * MtCutEff_nb_njet9_lowDphi_tt = static_cast<TH1D*>(thist_tt->Clone("MtCutEff_nb_njet9_lowDphi_tt")); 
 MtCutEff_nb_njet9_lowDphi_tt->Divide(thist_tt,thist_tt2,thist_tt2->GetSumOfWeights(),thist_tt->GetSumOfWeights(),"B");
 MtCutEff_nb_njet9_lowDphi_tt->SetName("MtCutEff_nb_njet9_lowDphi_tt");
 MtCutEff_nb_njet9_lowDphi_tt->SetTitle("MtCutEff_nb_njet9_lowDphi_tt");
 MtCutEff_nb_njet9_lowDphi_tt->Write();

////////
// wjet_njet9_lowDphi
////////

 stack = (THStack *) after_wj->Get("searchH_nb_njet9_lowDphi")->Clone("after");
 thist_wj = (TH1D *) stack->GetStack()->Last();
 stack = (THStack *) before_wj->Get("searchH_nb_njet9_lowDphi")->Clone("before");
 thist_wj2 = (TH1D *) stack->GetStack()->Last();
 
 // thist_wj =  (TH1D *) after_wj->Get("searchH_nb_njet9_lowDphi")->Clone("after");
 //thist_wj2 =  (TH1D *) before_wj->Get("searchH_nb_njet9_lowDphi")->Clone("before");
 TH1D * MtCutEff_nb_njet9_lowDphi_wj = static_cast<TH1D*>(thist_wj->Clone("MtCutEff_nb_njet9_lowDphi_wj")); 
 MtCutEff_nb_njet9_lowDphi_wj->Divide(thist_wj,thist_wj2,thist_wj2->GetSumOfWeights(),thist_wj->GetSumOfWeights(),"B");
 MtCutEff_nb_njet9_lowDphi_wj->SetName("MtCutEff_nb_njet9_lowDphi_wj");
 MtCutEff_nb_njet9_lowDphi_wj->SetTitle("MtCutEff_nb_njet9_lowDphi_wj");
 MtCutEff_nb_njet9_lowDphi_wj->Write();


////////
// tt+wj_njet9_lowDphi
////////
 
 thist_tot = static_cast<TH1D*>(thist_tt->Clone("thist_tot"));
 thist_tot->Add(thist_wj);
 thist_tot->Add(thist_tt);
 thist_totAfter_nb_njet9_lowDphi = static_cast<TH1D*>(thist_tot->Clone("thist_totAfter_nb_njet9_lowDphi"));
 thist_totAfter_nb_njet9_lowDphi->Write();
 thist_tot2 = static_cast<TH1D*>(thist_tt2->Clone("thist_tot2"));
 thist_tot2->Add(thist_wj2);
 thist_tot2->Add(thist_tt2);
 thist_totBefore_nb_njet9_lowDphi = static_cast<TH1D*>(thist_tot2->Clone("thist_totBefore_nb_njet9_lowDphi"));
 thist_totBefore_nb_njet9_lowDphi->Write();

 TH1D * MtCutEff_tot_nb_njet9_lowDphi = static_cast<TH1D*>(thist_tot->Clone("MtCutEff"));
 MtCutEff_tot_nb_njet9_lowDphi->Divide(thist_tot,thist_tot2,thist_tot2->GetSumOfWeights(),thist_tot->GetSumOfWeights(),"B");
 MtCutEff_tot_nb_njet9_lowDphi->SetName("MtCutEff_nb_njet9_lowDphi_tot");
 MtCutEff_tot_nb_njet9_lowDphi->SetTitle("MtCutEff_nb_njet9_lowDphi_tot");
 MtCutEff_tot_nb_njet9_lowDphi->Write();










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
