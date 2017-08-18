#include <vector>
#include <cstdio>
#include <string> 

using namespace std;

/*

Usage:

root.exe -b -q 'Plot_Commissioning.C("NJet")'
root.exe -b -q 'Plot_Commissioning.C("NBtag")'
root.exe -b -q 'Plot_Commissioning.C("HT")'
root.exe -b -q 'Plot_Commissioning.C("MHT")'
root.exe -b -q 'Plot_Commissioning.C("HT2")'
root.exe -b -q 'Plot_Commissioning.C("MHT2")'

Please note that the root files for the expectation code is 
expected to be normalized to 3/fb already. If not, the
lumi scaling needs to be adjusted in this room macro.
The following two lines should be adjusted.
  hExpTT->Scale(lumi/(3.));
  hExpWJ->Scale(lumi/(3.));

Input arguments:
 cutname:   the name of the cutflow stage where we want to do data/MC comparisons
 lumi:      the value of the data luminosity
 normalize: if true, the MC yields will be normalized to data (false by default)
 rebin:     if 1, "HT" and "MHT" bins will be rebinned to coarser bins (0 by default)
            at the root macro level. This is not encouraged because statistical correlation
            between different bins cannot be treated rigorously. Now, the statistical uncertainties
            between bins are considered fully correlated. This option is for experimental use only.

 */

void Plot_Commissioning_ForCS(string histname="Njets", 
		   //double lumi=2.26198, double lumiControl=2.24572,
		   double lumi=1.77, double lumiControl=2.585297,
		   string PDname="SingleMuon",
		   bool normalize=false, int rebin=0,
		   double lowPredictionCutOff=0.15,
		   double trigEff=1.00
		   ){

  ///////////////////////////////////////////////////////////////////////////////////////////
  ////Some cosmetic work for official documents.
  //
  // Set basic style
  //
  bool skipSingleTop = false;
  bool skipRare = true;
  
  gROOT->LoadMacro("tdrstyle.C");
  //setTDRStyle();
  gStyle->SetPalette(1) ; // for better color output
  gROOT->LoadMacro("CMS_lumi.C");

  bool writeExtraText = true;       // if extra text
  TString extraText  = "         Preliminary";  // default extra text is "Preliminary"
  TString lumi_8TeV  = "19.1 fb^{-1}"; // default is "19.7 fb^{-1}"
  TString lumi_7TeV  = "4.9 fb^{-1}";  // default is "5.1 fb^{-1}"
  TString lumi_sqrtS = "13 TeV";       // used with iPeriod = 0, e.g. for simulation-only plots (default is an empty string)

  int iPeriod = 0;    // 1=7TeV, 2=8TeV, 3=7+8TeV, 7=7+8+13TeV, 0=free form (uses lumi_sqrtS)
  // second parameter in example_plot is iPos, which drives the position of the CMS logo in the plot
  // iPos=11 : top-left, left-aligned
  // iPos=33 : top-right, right-aligned
  // iPos=22 : center, centered
  // mode generally : 
  //   iPos = 10*(alignement 1/2/3) + position (1/2/3 = left/center/right)
  int iPos =0;

  TString line = "";
  char tempname[200];
  sprintf(tempname,"%8.1f",lumi);
  line+=tempname;
  line+=" fb^{-1} (13 TeV)";
  lumi_sqrtS = line;

  ///////////////////////////////////////////////////////////////////////////////////////////
  //TPad * canvas_1;
  //TPad * canvas_2;
  TH1 *hExpT;

  TH1 *hExpTT_Rebin;
  TH1 *hExpWJ_Rebin;
  TH1 *hPre_Rebin;
  TH1 *hExp_Rebin;  
  int W = 600;
  int H = 600;
  int H_ref = 600;
  int W_ref = 800;
  float T = 0.08*H_ref;
  float B = 0.12*H_ref;
  float L = 0.12*W_ref;
  float R = 0.08*W_ref;

  double xtext_top;
  double ymax_top;
  double ymin_top;
  double xmax;
  double xmin;
  
  ///////////////////////////////////////////////////////////////////////////////////////////
  //
  // More specific style set, opening input files etc

  gStyle->SetOptStat(0);  ///to avoid the stat. on the plots
  char xtitlename[200];
  char ytitlename[200];

  //  sprintf(tempname,"TauHad2/HadTauEstimation_data_%s_v17a_.root",PDname.c_str()); 
  //  sprintf(tempname,"TauHad2/HadTauEstimation_data_%s_v16b_.root",PDname.c_str());
  //  sprintf(tempname,"TauHad2/ARElog49_7.6ifb_HadTauEstimation_data_%s_V9bc_.root",PDname.c_str()); 
  //* AR, 170815- Reads data prediction and MC expectation files
  sprintf(tempname,"TauHad2/HadTauEstimation_data_%s_Bv2C_Aug17_.root",PDname.c_str());
  TFile * PreData = new TFile(tempname,"R");
  TFile * ExpTT = new TFile("TauHad2/Stack/HadTauEstimation_TTbar_stacked.root","R");
  TFile * ExpWJ = new TFile("TauHad2/Stack/HadTauEstimation_WJet_stacked.root","R");
  TFile * ExpT  = new TFile("TauHad2/Stack/HadTauEstimation_T_stacked.root","R");
  TFile * ExpRare;
  //* AR, 170815-skipRare=true, hence we do not need to care for next sentence.
  if (!skipRare) ExpRare = new TFile("TauHad/GenInfo_HadTauEstimation_Rare_Elog410.root","R");
  //
  // Define legend
  //
  //* AR, 170815-Everything related to cosmetics
  Float_t legendX1 = .58; //.50;
  Float_t legendX2 = .88; //.70;
  Float_t legendY1 = .68; //.65;
  Float_t legendY2 = .88;

  TLegend* catLeg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);
  catLeg1->SetTextSize(0.032);
  catLeg1->SetTextFont(42);

  catLeg1->SetTextSize(0.032);
  catLeg1->SetTextFont(42);
  catLeg1->SetFillColor(0);
  catLeg1->SetLineColor(0);
  catLeg1->SetBorderSize(0);

  //
  // Define canvas
  //
  TCanvas *canvas = new TCanvas("canvas","canvas",10,10,W,H);

  canvas->SetFillColor(0);
  canvas->SetBorderMode(0);
  canvas->SetFrameFillStyle(0);
  canvas->SetFrameBorderMode(0);
  canvas->SetLeftMargin( L/W );
  canvas->SetRightMargin( R/W );
  canvas->SetRightMargin( 0.2 );
  canvas->SetTopMargin( T/H*3.0 );
  canvas->SetBottomMargin( B/H );
  canvas->SetTickx(0);
  canvas->SetTicky(0);

  canvas->Divide(1, 2);
  //canvas_1->SetTopMargin(0.1);
  //canvas_2->SetBottomMargin(1.8);
  //  std::cout<<" ******Check SegVio***********"<<endl;
  //
  // Define pads
  //
  TPad* canvas_up = (TPad*) canvas->GetListOfPrimitives()->FindObject("canvas_1");
  TPad* canvas_dw = (TPad*) canvas->GetListOfPrimitives()->FindObject("canvas_2");
 
  // define the size
  double up_height     = 0.8;  // please tune so that the upper figures size will meet your requirement
  double dw_correction = 1.30; // please tune so that the smaller canvas size will work in your environment
  double font_size_dw  = 0.1;  // please tune the font size parameter for bottom figure
  double dw_height    = (1. - up_height) * dw_correction;
  double dw_height_offset = 0.040; // KH, added to put the bottom one closer to the top panel
 
  // set pad size
  canvas_up->SetPad(0., 1 - up_height, 0.97, 1.);
  canvas_dw->SetPad(0., dw_height_offset, 0.97, dw_height+dw_height_offset);
  canvas_up->SetFrameFillColor(0);
  canvas_up->SetFillColor(0);
  canvas_dw->SetFillColor(0);
  canvas_dw->SetFrameFillColor(0);
  canvas_dw->SetBottomMargin(0.30);
  
  // set top margin 0 for bottom figure
  canvas_dw->SetTopMargin(0);
  
  // draw top figure
  canvas_up->cd();

  TH1 * hExpTT, * hExpWJ, * hPreTT, * hPreWJ12, * hPreWJ24, * hPreWJ46, * hPreWJ6I;
  TH1 * hExpRare;
  TH1 * hPreData, * hPreData_StatError;
  TH1 * histTemplate;
  THStack * stackTT, * stackWJ, * stackT, * ExpStack;
  ExpStack = new THStack("","");

  double HT_x_max=2500.;
  //double HT_x_min=400.;
  double HT_x_min=200.;
  double MHT_x_max=1000.;
  double NJet_x_max=15.;
  double NBtag_x_max=4.;
  double search_x_max=19.;
  double search_x_min=1.;

  //  sprintf(tempname,"allEvents/%s/StatError_%s_allEvents",cutname.c_str(),cutname.c_str());
  //* AR, 170815-hPreData_StatError only used for printing purpose
  //  hPreData_StatError  =(TH1D*) PreData->Get(tempname)->Clone("hPreData_StatError");
  sprintf(tempname,"MuonCS_%s",histname.c_str());
  //* AR, 170815-Picks histogram of distribution to be plotted (obviously after DelPhi cut) both from data file and MC expectation file(last from the stacks)
  hPreData  =(TH1D*) PreData->Get(tempname)->Clone("hPreData");
  stackTT=(THStack*)ExpTT->Get(tempname)->Clone("ExpTT");
  hExpTT=(TH1D*) stackTT->GetStack()->Last();
  stackWJ=(THStack*)ExpWJ->Get(tempname)->Clone("ExpWJ");   
  hExpWJ=(TH1D*) stackWJ->GetStack()->Last();
  stackT=(THStack*)ExpT->Get(tempname)->Clone("ExpT");   
  hExpT=(TH1D*) stackT->GetStack()->Last();
  //* AR, 170815-skipRare=true, hence we do not need to care for next sentence.
  if (!skipRare) hExpRare=(TH1D*)ExpRare->Get(tempname)->Clone("EXpRare");
  /////TH1D * hPre = static_cast<TH1D*>(hPreTT->Clone("hPre"));
  //* AR, 170815-Scales data file if required. lumiControl is used only at this point.
  TH1 * hPre = static_cast<TH1*>(hPreData->Clone("hPre"));
//  hPre->Scale(lumi/lumiControl);

  ////* AR, 170815-hExp_forScale is only used to derive "scale" which is not used in case of normalize=false which is our case, hence we do not need to care about this/use of this histogram. 
  TH1D * hExp_forScale = static_cast<TH1D*>(hExpTT->Clone("hExp_forScale"));
  hExp_forScale->Add(hExpWJ);
  if (!skipSingleTop) hExp_forScale->Add(hExpT);
  if (!skipRare) hExp_forScale->Add(hExpRare);

  hPre->SetMarkerSize(1.2);
  hPre->SetMarkerStyle(20);

  double scale = hPre->GetSumOfWeights()/trigEff/hExp_forScale->GetSumOfWeights();
  printf("data prediction: %8.2f\n",hPre->GetSumOfWeights()/trigEff);
//printf("Bin content: %g Stat error: %g \n ",hPreData_StatError->GetBinContent(1)/trigEff,hPreData_StatError->GetBinError(1));
//  printf("MC expectation:  %8.2f\n",hExp_forScale->GetSumOfWeights()*lumi/3.);
//printf("scale to match exp to pre = %10.5f, and %10.5f from lumi info\n",
//	 scale,lumi/(3.));
  //* AR, 170815-For us trigEff=1, as in prediction code itself we are correcting for trigger efficiency
  if (trigEff!=1.) hPre->Scale(1/trigEff);
  std::cout<<"scale factor "<<lumi/3.<<endl;
std::cout<< " Before scaling "<< " ttbar "<<hExpTT->GetBinContent(1)<<" wjet "<<hExpWJ->GetBinContent(1)<<" st "<<hExpT->GetBinContent(1)<<endl;
if (normalize) {hExpTT->Scale(scale); std::cout<<" normalize false "<<endl;}
  else           hExpTT->Scale(lumi/(3.)); //AR, 170815-Relevent for us as normalize=false
  hExpTT->SetFillColor(kBlue-6);
  std::cout<<"****SegVio****"<<endl;

  //* AR, 170815-skipRare=true, hence we do not need to care for this if loop.
  if (!skipRare){
	if (normalize) hExpRare->Scale(scale);
	else           hExpRare->Scale(lumi*1000.);
	hExpRare->SetFillColor(kCyan);
  }
  
  if (normalize) hExpWJ->Scale(scale);
  else           hExpWJ->Scale(lumi/(3.)); //AR, 170815-Relevent for us as normalize=false
  hExpWJ->SetFillColor(kGreen);

  if (!skipSingleTop){ //AR, 170815-Relevent as skipSingleTop=false 
    if (normalize) hExpT->Scale(scale);
    else           hExpT->Scale(lumi/(3.)); //AR, 170815-Relevent for us as normalize=false
    hExpT->SetFillColor(kRed);
  }
std::cout<< " After scaling "<< " ttbar "<<hExpTT->GetBinContent(1)<<" wjet "<<hExpWJ->GetBinContent(1)<<" st "<<hExpT->GetBinContent(1)<<endl;
  
  //AR, 170815-hExp=ttbar+wjet+ST
  TH1 * hExp = static_cast<TH1*>(hExpTT->Clone("hExp"));
  if(!skipRare)hExp->Add(hExpRare);
  hExp->Add(hExpWJ);
  if (!skipSingleTop) hExp->Add(hExpT);
std::cout<< " After scaling "<< " total "<<hExp->GetBinContent(1)<<endl;
  //AR, 170815-rebin=0, not relevent for us
  if (rebin==1 && histname=="MHT"){
    Double_t mht_bins[13] = {
          0., 50.,100.,150.,200.,250.,300.,350.,400.,500.,
	700.,1000.,5000.};
    hExpTT_Rebin = hExpTT->Rebin(12,"hExpTT_Rebin",mht_bins);
    hExpWJ_Rebin = hExpWJ->Rebin(12,"hExpWJ_Rebin",mht_bins);
    hPre_Rebin   = hPre->Rebin(12,"hPre_Rebin",mht_bins);
    hExp_Rebin   = hExp->Rebin(12,"hExp_Rebin",mht_bins);
    hPre_Rebin->Print("all");
    hExp_Rebin->Print("all");
    /*
    hExp_Rebin->SetBinContent(11,hExp->GetBinContent(11)+hExp->GetBinContent(12));
    hExp_Rebin->SetBinError(11,hExp->GetBinError(11)+hExp->GetBinError(12));
    hExp_Rebin->SetBinContent(12,hExp->GetBinContent(13)+hExp->GetBinContent(14)
				+hExp->GetBinContent(15)+hExp->GetBinContent(16));
    hExp_Rebin->SetBinError(12,hExp->GetBinError(13)+hExp->GetBinError(14)
				+hExp->GetBinError(15)+hExp->GetBinError(16));
    hPre_Rebin->SetBinContent(11,hPre->GetBinContent(11)+hPre->GetBinContent(12));
    hPre_Rebin->SetBinError(11,hPre->GetBinError(11)+hPre->GetBinError(12));
    hPre_Rebin->SetBinContent(12,hPre->GetBinContent(13)+hPre->GetBinContent(14)
				+hPre->GetBinContent(15)+hPre->GetBinContent(16));
    hPre_Rebin->SetBinError(12,hPre->GetBinError(13)+hPre->GetBinError(14)
				+hPre->GetBinError(15)+hPre->GetBinError(16));
    */
    hPre_Rebin->Print("all");
    hExp_Rebin->Print("all");
    hPre   = hPre_Rebin;
    hExp   = hExp_Rebin;
    hExpTT = hExpTT_Rebin;
    hExpWJ = hExpWJ_Rebin;
  } //end of rebin==1
  //AR, 170815-Defines bin boundaries for HT
  /* 
 if (histname=="HT"){
    Double_t HT_bins[14]={0.,100.,300.,500.,700.,900.,1100.,1300.,1500.,1700.,1900.,2100.,2300.,2500.};
    TH1 *hExpTT_Rebin = hExpTT->Rebin(13,"hExpTT_Rebin",HT_bins);
    TH1 *hExpWJ_Rebin = hExpWJ->Rebin(13,"hExpWJ_Rebin",HT_bins);
    TH1 *hPre_Rebin   = hPre->Rebin(13,"hPre_Rebin",HT_bins);
    TH1 *hExp_Rebin   = hExp->Rebin(13,"hExp_Rebin",HT_bins);
    hPre_Rebin->Print("all");
    hExp_Rebin->Print("all");
  */
  /*
    hExp_Rebin->SetBinContent(11,hExp->GetBinContent(11)+hExp->GetBinContent(12));
    hExp_Rebin->SetBinError(11,hExp->GetBinError(11)+hExp->GetBinError(12));
    hExp_Rebin->SetBinContent(12,hExp->GetBinContent(13)+hExp->GetBinContent(14)
				+hExp->GetBinContent(15)+hExp->GetBinContent(16));
    hExp_Rebin->SetBinError(12,hExp->GetBinError(13)+hExp->GetBinError(14)
				+hExp->GetBinError(15)+hExp->GetBinError(16));
    hPre_Rebin->SetBinContent(11,hPre->GetBinContent(11)+hPre->GetBinContent(12));
    hPre_Rebin->SetBinError(11,hPre->GetBinError(11)+hPre->GetBinError(12));
    hPre_Rebin->SetBinContent(12,hPre->GetBinContent(13)+hPre->GetBinContent(14)
				+hPre->GetBinContent(15)+hPre->GetBinContent(16));
    hPre_Rebin->SetBinError(12,hPre->GetBinError(13)+hPre->GetBinError(14)
				+hPre->GetBinError(15)+hPre->GetBinError(16));
  */
    //* AR, 170815-TTbar,Wjet, TTbar+Wjet+ST, data prediction are rebinned as given below.
  /*
    hPre_Rebin->Print("all");
    hExp_Rebin->Print("all");
    hPre   = hPre_Rebin;
    hExp   = hExp_Rebin;
    hExpTT = hExpTT_Rebin;
    hExpWJ = hExpWJ_Rebin;
  }
*/
  if (!skipRare)hExpRare->Print("all");  //* AR, 170815-Not relevent for us
  if (!skipRare)ExpStack->Add(hExpRare); //* AR, 170815-Not relevent for us
  //* AR, 170815-ExpStack was empty stack into which ST,Wjet,TTbar are added as that is the sequence we want
  if (!skipSingleTop) ExpStack->Add(hExpT);
  ExpStack->Add(hExpWJ);
  ExpStack->Add(hExpTT);
  //* AR, 170815-Defines canvas properties for HT,MHT,Njet and Nbjet distributions.
  double xlatex, ylatex;
  if(histname=="MuonPt"){
    xtext_top = 1800.;
    //y_legend  = 2000.;
    ymax_top = 30000.;
    ymin_top = 0.15;
    xmax = 400.;
    //if (cutname=="delphi") xmax = 700.;
    xmin = 0;
	xlatex=686.927;ylatex=13.61134;
    //sprintf(xtitlename,"#slash{H}_{T} [GeV]");
    sprintf(xtitlename,"MuonPt[GeV/c]");
    sprintf(ytitlename,"Events /(20 GeV/c)");
    gPad->SetLogy();
  }

  if(histname=="MHT"){
    xtext_top = 1800.;
    //y_legend  = 2000.;
    ymax_top = 30000.;
    ymin_top = 0.15;
    xmax = 1000.;
    //if (cutname=="delphi") xmax = 700.;
    xmin = 0;
	xlatex=686.927;ylatex=13.61134;
    //sprintf(xtitlename,"#slash{H}_{T} [GeV]");
    sprintf(xtitlename,"H_{T}^{miss} [GeV]");
    sprintf(ytitlename,"Events / 50 GeV");
    gPad->SetLogy();
  }
  if(histname=="MHT2"){
    //xtext_top = 1800.;
    //xtext_top = 1800.;
    //y_legend  = 2000.;
    ymax_top = 10000.;
    ymin_top = 0.15;
    xmax = 1000.;
    //if (cutname=="delphi") xmax = 700.;
    //xmax = 700.;
    //xmax = 500.;
    xmin = 200;
    //sprintf(xtitlename,"#slash{H}_{T} [GeV]");
    sprintf(xtitlename,"H_{T}^{miss} [GeV]");
    sprintf(ytitlename,"Events / bin");
    gPad->SetLogy();
  }
  if(histname=="HT"){
    xtext_top = 2200.;
    //y_legend  = 2000.;
    ymax_top = 30000.;
    ymin_top = 0.15;
    xmax = 2500.;
    //if (cutname=="delphi") xmax = 1500.;
    xmin = 0;
	xlatex=1466.973;ylatex=14.62075;
    sprintf(xtitlename,"H_{T} [GeV]");
    sprintf(ytitlename,"Events / 100 GeV");
    gPad->SetLogy();
  }
  if(histname=="HT2"){
    xtext_top = 1800.;
    //y_legend  = 2000.;
    ymax_top = 3000.;
    ymin_top = 0.15;
    xmax = 2000.;
    //if (cutname=="delphi") xmax = 1500.;
    //xmax = 1200.;
    xmin = 200;
    sprintf(xtitlename,"H_{T} [GeV]");
    sprintf(ytitlename,"Events / bin");
    gPad->SetLogy();
  }
  if(histname=="Nbjets"){
    xtext_top = 1800.;
    //y_legend  = 2000.;
    ymax_top = 100000.;
    ymin_top = 0.15;
    xmax = 5.;
    xmin = 0;
	xlatex=3.406299;ylatex=70.30534;
    sprintf(xtitlename,"N_{b}");
    sprintf(ytitlename,"Events");
    gPad->SetLogy();
  }
  if(histname=="Njets"){
    xtext_top = 1800.;
    //y_legend  = 2000.;
    ymax_top = 100000.;
    ymin_top = 0.15;
    xmax = 12.;
    xmin = 2;
	xlatex=9.190936;ylatex=71.94065;
    sprintf(xtitlename,"N_{jets}");
    sprintf(ytitlename,"Events");
    gPad->SetLogy();
  }
  if(histname=="DelPhi1" || histname=="DelPhi2" || histname=="DelPhi3" ){
    xtext_top = 1800.;
    //y_legend  = 2000.;
    //ymax_top = 10.;
    ymax_top = 800.;
    ymin_top = 0.0;
    xmax = 3.2;
    xmin = 0.;
    if (histname=="DelPhi1") sprintf(xtitlename,"#Delta#phi(MHT,jet1) [rad]");
    if (histname=="DelPhi2") sprintf(xtitlename,"#Delta#phi(MHT,jet2) [rad]");
    if (histname=="DelPhi3") sprintf(xtitlename,"#Delta#phi(MHT,jet3) [rad]");
    sprintf(ytitlename,"Events");
    //gPad->SetLogy();
  }

  // Actually draw plots
  //* AR, 170815-Draws data prediction
  hPre->GetXaxis()->SetTitleSize(0.06);
  hPre->GetXaxis()->SetTitleOffset(1.9);
  hPre->GetXaxis()->SetTitleFont(42);
  hPre->GetXaxis()->SetLabelSize(0.04);
  hPre->GetXaxis()->SetLabelOffset(1.9);
  //hPre->GetYaxis()->SetLabelFont(42);
  //hPre->GetYaxis()->SetLabelOffset(0.007);
  //hPre->GetYaxis()->SetLabelSize(0.04);
  hPre->GetYaxis()->SetTitleSize(0.06);
  hPre->GetYaxis()->SetTitleOffset(0.8);
  hPre->GetYaxis()->SetTitleFont(42);
  hPre->GetYaxis()->SetLabelSize(0.04);

  hPre->GetXaxis()->SetTitle(xtitlename);
  hPre->GetYaxis()->SetTitle(ytitlename);
  hPre->SetLineColor(1);
  hPre->SetMaximum(ymax_top);
  hPre->SetMinimum(ymin_top);
  hPre->GetXaxis()->SetRangeUser(xmin,xmax);
  hPre->SetTitle("");
  hPre->DrawCopy("e");
  //* AR, 170815-Draws MC expectation stack
  ExpStack->Draw("same hist");

  hPre->DrawCopy("e same");

  sprintf(tempname,"Data: 2017 data");
  catLeg1->AddEntry(hPre,tempname);
  //sprintf(tempname,"#tau_{h} MC expectation from t#bar{t}");
  sprintf(tempname,"MC: t#bar{t}");
  catLeg1->AddEntry(hExpTT,tempname,"f");
  //sprintf(tempname,"#tau_{h} MC expectation from W + jets");
  sprintf(tempname,"MC: W + jets");
  catLeg1->AddEntry(hExpWJ,tempname,"f");  
  if (!skipSingleTop) {
    //sprintf(tempname,"#tau_{h} MC expectation from single top");
    sprintf(tempname,"MC: single top");
    catLeg1->AddEntry(hExpT,tempname,"f");
  }
  if (!skipRare){
    sprintf(tempname,"MC: Other");
    catLeg1->AddEntry(hExpRare,tempname,"f");
  }
  catLeg1->Draw();
  
  {
    //void CMS_lumi( canvas_up, iPeriod, iPos );   // writing the lumi information and the CMS "logo"
  }
  canvas->Update();
  canvas->RedrawAxis();
  //KH canvas->GetFrame()->Draw(); 

  sprintf(tempname,"CMS Preliminary, %.3f fb^{-1}, #sqrt{s} = 13 TeV",lumi);
  TLatex * ttext = new TLatex(xmin, ymax_top*1.3,tempname);
  ttext->SetTextFont(42);
  ttext->SetTextSize(0.050);
  ttext->SetTextAlign(11);
  //  ttext->Draw();

  //TLatex *   tex = new TLatex(xlatex,ylatex,"arXiv:1602.06581");
  // tex->SetTextColor(4);
  // tex->SetTextFont(61);
  // tex->SetTextSize(0.0375);
  // tex->SetLineColor(4);
  // tex->SetLineWidth(2);
  // //tex->Draw();
   
  gPad->RedrawAxis(); 
  
  //
  // Bottom ratio plot
  //
  // ----------

  //KH -- flip the numerator and denominator
  //* AR, 170815-hPreOverExp is initially same as data prediction
  TH1D * hPreOverExp = (TH1D*) hPre->Clone();
  //* AR, 170815-lowPredictionCutOff is setting up minimum non-zero value the ratio can have.
  for (int ibin=0;ibin<hPreOverExp->GetNbinsX();ibin++){
    if (hPreOverExp->GetBinContent(ibin+1)<lowPredictionCutOff){
      hPreOverExp->SetBinContent(ibin+1,0.);
      hPreOverExp->SetBinError(ibin+1,0.);
    }
  }
  //* AR, 170815-We divide hPreOverExp by expectation here
  hPreOverExp->Divide(hExp);
  /*
    hPre->Print("all");
    hPreTT->Print("all");
    hPreWJ12->Print("all");
    hPreWJ24->Print("all");
    hPreWJ46->Print("all");
    hPreWJ6I->Print("all");
    hExp->Print("all");
  */
  hPreOverExp->Print("all");

  // draw bottom figure
  canvas_dw->cd();
  // font size
  hPreOverExp->GetXaxis()->SetLabelSize(font_size_dw);
  hPreOverExp->GetXaxis()->SetTitleSize(font_size_dw);
  hPreOverExp->GetYaxis()->SetLabelSize(font_size_dw);
  hPreOverExp->GetYaxis()->SetTitleSize(font_size_dw);
  
  //
  // Common to all bottom plots
  //
  sprintf(ytitlename,"Data / MC");
  hPreOverExp->GetYaxis()->SetTitle(ytitlename);
  hPreOverExp->SetMaximum(2.65);
  hPreOverExp->SetMinimum(0.0);

  //
  // Specific to each bottom plot
  //
  //sprintf(xtitlename,"search bin");
  hPreOverExp->GetXaxis()->SetRangeUser(xmin,xmax);
  TLine *tline = new TLine(xmin,1.,xmax,1.);
  
  // Setting style
  //hPreOverExp->SetMaximum(1.4);
  //hPreOverExp->GetXaxis()->SetLabelFont(42);
  hPreOverExp->GetXaxis()->SetLabelOffset(0.007);
  hPreOverExp->GetXaxis()->SetLabelSize(0.12);
  hPreOverExp->GetXaxis()->SetTitleSize(0.16);
  hPreOverExp->GetXaxis()->SetTitleOffset(0.80);
  hPreOverExp->GetXaxis()->SetTitleFont(42);
  //hPreOverExp->GetYaxis()->SetLabelFont(42);
  hPreOverExp->GetYaxis()->SetLabelOffset(0.007);
  hPreOverExp->GetYaxis()->SetLabelSize(0.12);
  hPreOverExp->GetYaxis()->SetTitleSize(0.16);
  hPreOverExp->GetYaxis()->SetTitleOffset(0.3);
  hPreOverExp->GetYaxis()->SetTitleFont(42);
  hPreOverExp->GetXaxis()->SetTickSize(0.08);

  hPreOverExp->SetTitle("");
  hPreOverExp->Draw();
  tline->SetLineStyle(2);
  tline->Draw();

  hPreOverExp->Print("all");
  
  //* AR, 170815-normalize=false
  if (normalize) sprintf(tempname,"DataPreVsMCExp_%s_%s_normalize_Plot.png",histname.c_str(),PDname.c_str());
  //* AR, 170815-Relevent for us
  else           sprintf(tempname,"DataPreVsMCExp_%s_%s_Plot.png",histname.c_str(),PDname.c_str());
  canvas->Print(tempname);
  if (normalize) sprintf(tempname,"DataPreVsMCExp_%s_%s_normalize_Plot.pdf",histname.c_str(),PDname.c_str());
  //* AR, 170815-Relevent for us
  else           sprintf(tempname,"DataPreVsMCExp_%s_%s_Plot.pdf",histname.c_str(),PDname.c_str());
  canvas->Print(tempname);
  
}
