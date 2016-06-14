
void plot_IsoTrkEff_allSamples(int IsoModel=0){

  //
  // icomp=0: only show own results
  //       1: show also Koushik's results
  //
  
  //
  ///////////////////////////////////////////////////////////////////////////////////////////
  ////Some cosmetic work for official documents. 
  gStyle->SetOptStat(0);  ///to avoid the stat. on the plots 
  gROOT->LoadMacro("tdrstyle.C");
//  setTDRStyle();
  gROOT->LoadMacro("CMS_lumi_v2.C");

  char tempname[200];
  char tempname2[200];
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
  
  Float_t legendX1 = .60; //.50;
  Float_t legendX2 = .90; //.70;
  Float_t legendY1 = .60; //.65;
  Float_t legendY2 = .70;
  TLegend* catLeg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);
  catLeg1->SetTextSize(0.042);
  catLeg1->SetTextFont(42);
  catLeg1->SetFillColor(0);
  catLeg1->SetLineColor(0);
  catLeg1->SetBorderSize(0);

  sprintf(tempname,"Stack/IsoEfficiencies_TTbar_stacked.root");
  TFile *file_TTbar   = new TFile(tempname,"R");
  sprintf(tempname,"Stack/IsoEfficiencies_WJet_stacked.root");
  TFile *file_WJet   = new TFile(tempname,"R");
  

  TH1D * thist, * thist2;
  THStack * tempstack;
//  catLeg1->SetHeader("Prob. of #mu from #tau ");


//...........................................................................//
// TTbar ....................................................................//
//...........................................................................//
    double XUp = 500. , maxVal=3.;

    if(IsoModel==0)sprintf(tempname,"IsoEff");
    else if(IsoModel==1)sprintf(tempname,"IsoEff2");
    else{
      cout << " Error! IsoModel not known \n ";
      return 2;
    }
    thist2 = (TH1D*)file_WJet->Get(tempname)->Clone();
    thist2->GetXaxis()->SetRangeUser(0.,XUp);
    thist2->SetMaximum(maxVal);
    thist2->SetTitle("");
    
    thist2->GetXaxis()->SetLabelFont(42);
    thist2->GetXaxis()->SetLabelOffset(0.007);
    thist2->GetXaxis()->SetLabelSize(0.04);
    thist2->GetXaxis()->SetTitleSize(0.05);
    thist2->GetXaxis()->SetTitleOffset(0.9);
    thist2->GetXaxis()->SetTitleFont(42);
    thist2->GetYaxis()->SetLabelFont(42);
    thist2->GetYaxis()->SetLabelOffset(0.007);
    thist2->GetYaxis()->SetLabelSize(0.04);
    thist2->GetYaxis()->SetTitleSize(0.05);
    thist2->GetYaxis()->SetTitleOffset(1.25);
    thist2->GetYaxis()->SetTitleFont(42);
    thist2->GetXaxis()->SetTitle("search bin");
    thist2->GetYaxis()->SetTitle("IsoTrk efficiency");
    thist2->GetXaxis()->SetRangeUser(0.,XUp);
    thist2->SetMaximum(maxVal);
    thist2->SetLineColor(2);

    thist2->Draw();

    thist = (TH1D*)file_TTbar->Get(tempname)->Clone();
    thist->SetLineColor(1);
//    thist->SetFillColor(0);
//    thist->SetLineWidth(3);
    
    thist->Draw("same");
    sprintf(tempname,"T#bar{T}");
    catLeg1->AddEntry(thist,tempname,"l");
    sprintf(tempname,"WJet");
    catLeg1->AddEntry(thist2,tempname,"l");    
    catLeg1->Draw();
  
    sprintf(tempname,"IsoEfficiencies_allSamples.png");
    c1->Print(tempname);

}
