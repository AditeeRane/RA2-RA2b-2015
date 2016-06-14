
void plot_MistagRate_allSamples_foraux(string sample="TTbar_", int wjets_hadtau_only=0, bool debug=false){

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
  gROOT->LoadMacro("CMS_lumi.C");

  writeExtraText = true;
  extraText   = "       Supplementary (Simulation)";  // default extra text is "Preliminary"
  lumi_8TeV  = "19.1 fb^{-1}"; // default is "19.7 fb^{-1}"
  lumi_7TeV  = "4.9 fb^{-1}";  // default is "5.1 fb^{-1}"
  lumi_sqrtS = "13 TeV";       // used with iPeriod = 0, e.g. for simulation-only plots (default is an empty string)
  //cmsTextSize  = 0.60;
  //lumiTextSize = 0.52;

  int iPeriod = 0;    // 1=7TeV, 2=8TeV, 3=7+8TeV, 7=7+8+13TeV, 0=free form (uses lumi_sqrtS)
  int iPos=0;

  char tempname[200];
  char tempname2[200];
  int W = 1000;
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
  //c1->SetTickx(0);
  //c1->SetTicky(0);
  c1->SetTopMargin( 0.1 );
  c1->SetLeftMargin( 0.10 );
  
  gStyle->SetOptStat(000000);
  gStyle->SetErrorX(0.5); 

  Float_t legendX1 = .15; //.50;
  Float_t legendX2 = .50; //.70;
  Float_t legendY1 = .55; //.65;
  Float_t legendY2 = .78;
  TLegend* catLeg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);
  catLeg1->SetTextSize(0.05);
  catLeg1->SetTextFont(42);
  catLeg1->SetFillColor(0);
  catLeg1->SetLineColor(0);
  catLeg1->SetBorderSize(0);

  TH1D * temp_tt_tauH, * temp_wj_tauH, *temp_tt_muH, * temp_wj_muH;
  
  //
  // From W+jets
  TFile file_WJet("TauHad/Stack/TauBtaggedRate_WJet_stacked_Elog282.root","R");
  temp_wj_tauH = (TH1D*)file_WJet.Get("TauBtaggedRate")->Clone();
  TH1D * wj_tauH = new TH1D("wj_tauH","WJet -- Tau mistag",
			    temp_wj_tauH->GetNbinsX(),
			    temp_wj_tauH->GetXaxis()->GetXmin(),
			    temp_wj_tauH->GetXaxis()->GetXmax());
  for(int ibin=0;ibin<wj_tauH->GetNbinsX()+2;ibin++){
    double con = (double)temp_wj_tauH->GetBinContent(ibin);
    double err = (double)temp_wj_tauH->GetBinError(ibin);
    if (debug) cout << "con: " << con << " err: " << err << endl;
    wj_tauH->SetBinContent(ibin,con);
    wj_tauH->SetBinError(ibin,err);
  }

  //
  // From ttbar
  TFile file_TTbar("TauHad/Stack/TauBtaggedRate_TTbar_stacked.root","R");  
  temp_tt_tauH = (TH1D*)file_TTbar.Get("TauBtaggedRate")->Clone();
  TH1D * tt_tauH = new TH1D("tt_tauH","WJet -- Tau mistag",
			    temp_tt_tauH->GetNbinsX(),
			    temp_tt_tauH->GetXaxis()->GetXmin(),
			    temp_tt_tauH->GetXaxis()->GetXmax());
  for(int ibin=0;ibin<tt_tauH->GetNbinsX()+2;ibin++){
    double con = (double)temp_tt_tauH->GetBinContent(ibin);
    double err = (double)temp_tt_tauH->GetBinError(ibin);
    if (debug) cout << "con: " << con << " err: " << err << endl;
    tt_tauH->SetBinContent(ibin,con);
    tt_tauH->SetBinError(ibin,err);
  }

  //
  // From W+jets - predictions
  TFile file_WJet_mu("TauHad2/Stack/TauBtaggedRate_WJet_stacked.root","R");  
  temp_wj_muH = (TH1D*)file_WJet_mu.Get("TauBtaggedRate")->Clone();
  TH1D * wj_muH = new TH1D("wj_muH","WJet -- Tau mistag",
			   temp_wj_muH->GetNbinsX(),
			   temp_wj_muH->GetXaxis()->GetXmin(),
			   temp_wj_muH->GetXaxis()->GetXmax());
  for(int ibin=0;ibin<wj_muH->GetNbinsX()+2;ibin++){
    double con = (double)temp_wj_muH->GetBinContent(ibin);
    double err = (double)temp_wj_muH->GetBinError(ibin);
    if (debug) cout << "con: " << con << " err: " << err << endl;
    wj_muH->SetBinContent(ibin,con);
    wj_muH->SetBinError(ibin,err);
  }

  //
  // From ttbar - predictions
  TFile file_TTbar_mu("TauHad2/Stack/TauBtaggedRate_TTbar_stacked.root","R");
  temp_tt_muH = (TH1D*)file_TTbar_mu.Get("TauBtaggedRate")->Clone();
  TH1D * tt_muH = new TH1D("tt_muH","WJet -- Tau mistag",
			   temp_tt_muH->GetNbinsX(),
			   temp_tt_muH->GetXaxis()->GetXmin(),
			   temp_tt_muH->GetXaxis()->GetXmax());
  for(int ibin=0;ibin<tt_muH->GetNbinsX()+2;ibin++){
    double con = (double)temp_tt_muH->GetBinContent(ibin);
    double err = (double)temp_tt_muH->GetBinError(ibin);
    if (debug) cout << "con: " << con << " err: " << err << endl;
    tt_muH->SetBinContent(ibin,con);
    tt_muH->SetBinError(ibin,err);
  }

  //  catLeg1->SetHeader("Prob. of #mu from #tau ");

  gPad->SetTickx();
  gPad->SetTicky();

  //...........................................................................//
  // TTbar ....................................................................//
  //...........................................................................//
  double XUp = 400. , maxVal=1.;

  TH1F *tbox = new TH1F("tbox","tbox",60,0.,600.);

  tbox->GetXaxis()->SetRangeUser(30.,XUp);
    tbox->SetMaximum(maxVal);
    tbox->SetTitle("");
    
    tbox->GetXaxis()->SetLabelFont(42);
    tbox->GetXaxis()->SetLabelOffset(0.007);
    tbox->GetXaxis()->SetLabelSize(0.045);
    tbox->GetXaxis()->SetTitleSize(0.06);
    tbox->GetXaxis()->SetTitleOffset(0.9);
    tbox->GetXaxis()->SetTitleFont(42);
    tbox->GetYaxis()->SetLabelFont(42);
    tbox->GetYaxis()->SetLabelOffset(0.007);
    tbox->GetYaxis()->SetLabelSize(0.045);
    tbox->GetYaxis()->SetTitleSize(0.06);
    tbox->GetYaxis()->SetTitleOffset(0.8);
    tbox->GetYaxis()->SetTitleFont(42);
    tbox->GetXaxis()->SetTitle("Hadronic tau jet p_{T} [GeV]");
    tbox->GetYaxis()->SetTitle("b-tagging mistag rate");
    tbox->GetXaxis()->SetRangeUser(30.,XUp);
    tbox->SetMaximum(maxVal);
    tbox->SetLineColor(1);
    //tbox->SetLineWidth(3);
    tbox->Draw();
  
  //

    wj_tauH->GetXaxis()->SetRangeUser(30.,XUp);
    wj_tauH->SetMaximum(maxVal);
    wj_tauH->SetTitle("");
    
    wj_tauH->GetXaxis()->SetLabelFont(42);
    wj_tauH->GetXaxis()->SetLabelOffset(0.007);
    wj_tauH->GetXaxis()->SetLabelSize(0.045);
    wj_tauH->GetXaxis()->SetTitleSize(0.06);
    wj_tauH->GetXaxis()->SetTitleOffset(0.9);
    wj_tauH->GetXaxis()->SetTitleFont(42);
    wj_tauH->GetYaxis()->SetLabelFont(42);
    wj_tauH->GetYaxis()->SetLabelOffset(0.007);
    wj_tauH->GetYaxis()->SetLabelSize(0.045);
    wj_tauH->GetYaxis()->SetTitleSize(0.06);
    wj_tauH->GetYaxis()->SetTitleOffset(0.8);
    wj_tauH->GetYaxis()->SetTitleFont(42);
    wj_tauH->GetXaxis()->SetTitle("Hadronic tau jet p_{T} [GeV]");
    wj_tauH->GetYaxis()->SetTitle("b-tagging mistag rate");
    wj_tauH->GetXaxis()->SetRangeUser(30.,XUp);
    wj_tauH->SetMaximum(maxVal);
    wj_tauH->SetLineColor(1);
    wj_tauH->SetLineWidth(3);
    wj_tauH->Draw("same");

    tt_tauH->SetLineColor(2);
    //thist.SetFillColor(0);
    tt_tauH.SetLineWidth(3);
    if (!wjets_hadtau_only) tt_tauH->Draw("same");

    /*
    wj_muH->SetLineColor(8); 
    //    wj_muH->SetMarkerStyle(20);
    if (!wjets_hadtau_only) {
      wj_muH->GetXaxis()->SetRangeUser(30.,500.);
      wj_muH->Draw("same");
    }
      
    tt_muH->SetLineColor(4);   
    if (!wjets_hadtau_only){
      tt_muH->GetXaxis()->SetRangeUser(30.,500.);
      tt_muH->Draw("same");
    }
    */

    /*
    // sanity plot
    TH1 * sanityH = static_cast<TH1D*>(wj_tauH->Clone("sanity"));
    sanityH->Add(tt_muH);
    TH1 * tempH = static_cast<TH1D*>(wj_tauH->Clone("sanity"));
    tempH->Multiply(tt_muH);
    sanityH->Add(tempH,-1);
    sanityH->SetLineColor(34); 
    if (!wjets_hadtau_only){
      sanityH->GetXaxis()->SetRangeUser(30.,500.);
      sanityH->Draw("same");
    }
    */
    
    //
    // TLegend
    sprintf(tempname,"t#bar{t}");
    if (!wjets_hadtau_only) catLeg1->AddEntry(tt_tauH,tempname,"l");

    if (wjets_hadtau_only)
      sprintf(tempname,"W+jets");
    else 
      sprintf(tempname,"W+jets");
    catLeg1->AddEntry(wj_tauH,tempname,"l");    

    /*
    sprintf(tempname,"(c) #mu-jets in t#bar{t}");
    if (!wjets_hadtau_only) catLeg1->AddEntry(tt_muH,tempname,"l");

    sprintf(tempname,"(d) #mu-jets in W+jets");
    if (!wjets_hadtau_only) catLeg1->AddEntry(wj_muH,tempname,"l");

    //sprintf(tempname,"blue+(1 - blue)*red ");
    if (!wjets_hadtau_only){
      sprintf(tempname,"(e) #mu-jets (t#bar{t}) + #tau_{h}-jets (W+jets)");
      catLeg1->AddEntry(sanityH,tempname,"l");      
      TH1F *sanityHdummy = (TH1F*) sanityH->Clone();
      sanityHdummy->SetLineColor(0);
      sprintf(tempname,"= (c) + [1-(c)]#times(b)");
      catLeg1->AddEntry(sanityHdummy,tempname,"l");      
    }
    */
    catLeg1->Draw();
    
  CMS_lumi( c1, iPeriod, iPos );   // writing the lumi information and the CMS "logo"

  double xlatex=0.65;
  double ylatex=0.65;
  /*
  TLatex *   tex = new TLatex(xlatex,ylatex,"arXiv:1602.06581");
  tex->SetTextColor(4);
  tex->SetTextFont(61);
  tex->SetTextSize(0.055);
  tex->SetLineColor(4);
  tex->SetLineWidth(2);
  //tex->Draw();
  tex->DrawLatexNDC(xlatex,ylatex-0.2,"arXiv:1602.06581");
  */
  TPaveText pt(xlatex,ylatex,xlatex+0.19,ylatex+0.1,"NDC");
  pt.AddText("arXiv:1602.06581");
  pt.SetFillColor(0);
  pt.SetLineColor(0);
  pt.SetLineWidth(0);
  pt.SetBorderSize(0);
  pt.SetTextColor(4);
  pt.SetTextFont(61);
  pt.SetTextSize(0.055);
  pt.Draw();

    if (wjets_hadtau_only) {
      sprintf(tempname,"TauBtaggedRate_allSamples_hadtau_Wjets.png");
      c1->Print(tempname);
      sprintf(tempname,"TauBtaggedRate_allSamples_hadtau_Wjets.pdf");
      c1->Print(tempname);
    } else {
      sprintf(tempname,"TauBtaggedRate_allSamples.png");
      c1->Print(tempname);
      sprintf(tempname,"TauBtaggedRate_allSamples.pdf");
      c1->Print(tempname);
    }
    
}
