
void plot_TauTemplate(string Elog="Elog431_",int icomp=0){

  //
  // icomp=0: only show own results
  //       1: show also Koushik's results
  //
  
  //
  ///////////////////////////////////////////////////////////////////////////////////////////
  ////Some cosmetic work for official documents. 
  gROOT->LoadMacro("tdrstyle.C");
  setTDRStyle();
  gStyle->SetPalette(1) ; // for better color output
  gROOT->LoadMacro("CMS_lumi.C");
 writeExtraText = true;       // if extra text
  extraText  = "         Supplementary";  // default extra text is "Preliminary"
  lumi_8TeV  = "19.1 fb^{-1}"; // default is "19.7 fb^{-1}"
  lumi_7TeV  = "4.9 fb^{-1}";  // default is "5.1 fb^{-1}"
  lumi_sqrtS = "13 TeV";       // used with iPeriod = 0, e.g. for simulation-only plots (default is an empty string)

  int iPeriod = 0;    // 1=7TeV, 2=8TeV, 3=7+8TeV, 7=7+8+13TeV, 0=free form (uses lumi_sqrtS)
  // second parameter in example_plot is iPos, which drives the position of the CMS logo in the plot
  // iPos=11 : top-left, left-aligned
  // iPos=33 : top-right, right-aligned
  // iPos=22 : center, centered
  // mode generally : 
  //   iPos = 10*(alignement 1/2/3) + position (1/2/3 = left/center/right)
  int iPos =0;
  
  
  
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

  gStyle->SetOptStat(000000);
  
  Float_t legendX1 = .60; //.50;
  Float_t legendX2 = .90; //.70;
  Float_t legendY1 = .60; //.65;
  Float_t legendY2 = .90;
  TLegend* catLeg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);
  catLeg1->SetTextSize(0.042);
  catLeg1->SetTextFont(42);
  catLeg1->SetFillColor(0);
  catLeg1->SetLineColor(0);
  catLeg1->SetBorderSize(0);

  //
  sprintf(tempname,"TauHad/Stack/%sHadTau_TauResponseTemplates_stacked.root",Elog.c_str());
  TFile *file_13TeV         = new TFile(tempname,"R"); 
  //TFile *file_13TeV_Koushik = new TFile("HadTau_TauResponseTemplates_PHYS14_13TeV.root","R"); 
  TFile *file_TauGan        = new TFile("TauHad/HadTau_TauResponseTemplates_GenTau_Matching04.root","R"); 

  TH1D * thist;
  catLeg1->SetHeader("p_{T}(#tau^{tau})");

  for(int i=0;i<4;i++){
    sprintf(tempname,"hTauResp_%d",i);
    thist = (TH1D*)file_13TeV->Get(tempname)->Clone();
    sprintf(tempname2,"hTauResp_%d_AB",i);
    thist->SetName(tempname2);
	thist->SetFillColor(0);
    thist->SetLineColor(i+1);
    thist->SetLineWidth(3);
    thist->SetStats(kFALSE);
    thist->SetMaximum(1.5);
    
    if(i==0){
      thist->SetMaximum(1.4);
      thist->GetXaxis()->SetLabelFont(42);
      thist->GetXaxis()->SetLabelOffset(0.007);
      thist->GetXaxis()->SetLabelSize(0.04);
      thist->GetXaxis()->SetTitleSize(0.05);
      thist->GetXaxis()->SetTitleOffset(0.9);
      thist->GetXaxis()->SetTitleFont(42);
      thist->GetYaxis()->SetLabelFont(42);
      thist->GetYaxis()->SetLabelOffset(0.007);
      thist->GetYaxis()->SetLabelSize(0.04);
      thist->GetYaxis()->SetTitleSize(0.05);
      thist->GetYaxis()->SetTitleOffset(1.25);
      thist->GetYaxis()->SetTitleFont(42);
    }

    //KH if (i==0){
    thist->GetYaxis()->SetTitle("Arbitrary unit");
    thist->DrawNormalized("same,hist");
    /*
    if(i==0)sprintf(tempname,"20 - 30: t#bar{t}");
    if(i==1)sprintf(tempname,"30 - 50: t#bar{t}");
    if(i==2)sprintf(tempname,"50 - 100: t#bar{t}");
    if(i==3)sprintf(tempname,">100: t#bar{t}");
    */
    if(i==0)sprintf(tempname,"20 - 30 GeV");
    if(i==1)sprintf(tempname,"30 - 50 GeV");
    if(i==2)sprintf(tempname,"50 - 100 GeV");
    if(i==3)sprintf(tempname,">100 GeV");
    catLeg1->AddEntry(thist,tempname,"l");
    //KH}
    
    }
  catLeg1->Draw();

   TLatex *   tex = new TLatex(1.5,0.03,"arXiv:1602.06581");
   tex->SetTextColor(4);
   tex->SetTextFont(61);
   tex->SetTextSize(0.0375);
   tex->SetLineColor(4);
   tex->SetLineWidth(2);
   //tex->Draw();
  
  TH1D * thist_km;
  if (icomp==1){
  for(int i=0;i<4;i++){
    sprintf(tempname,"hTauResp_%d",i);
    thist_km = (TH1D*)file_TauGan->Get(tempname)->Clone();
    sprintf(tempname2,"hTauResp_%d_KM",i);
    thist_km->SetName(tempname2);
    thist_km->SetLineColor(i+1);
    thist_km->SetLineWidth(3);
    thist_km->SetLineStyle(3);

    if (i==0) {
    thist_km->DrawNormalized("same,hist");
    if(i==0)sprintf(tempname,"20 - 30: TauGan");
    if(i==1)sprintf(tempname,"30 - 50: TauGan");
    if(i==2)sprintf(tempname,"50 - 100: TauGan");
    if(i==3)sprintf(tempname,">100: TauGan");
    catLeg1->AddEntry(thist_km,tempname,"l");
    }
  }
  }
  {
    CMS_lumi( c1, iPeriod, iPos );   // writing the lumi information and the CMS "logo"
  }
  c1->Update();
  c1->RedrawAxis();
  catLeg1->Draw();  

  c1->Print("Plot_TauTemplate_TTbar_Wjets.pdf");
  c1->Print("Plot_TauTemplate_TTbar_Wjets.png");

}
