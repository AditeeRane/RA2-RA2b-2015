#include <vector>
#include <cstdio>
#include <string> 

using namespace std;

Plot_searchBin_Data(string sample="stacked",string histname="searchH",int choice=1){

  ///////////////////////////////////////////////////////////////////////////////////////////
  ////Some cosmetic work for official documents.
  //
  // Set basic style
  //
  gROOT->LoadMacro("tdrstyle.C");
  setTDRStyle();
  gStyle->SetPalette(1) ; // for better color output
  gROOT->LoadMacro("CMS_lumi_v2.C");

  int W = 600;
  int H = 600;
  int H_ref = 600;
  int W_ref = 800;
  float T = 0.08*H_ref;
  float B = 0.12*H_ref;
  float L = 0.12*W_ref;
  float R = 0.08*W_ref;

  ///////////////////////////////////////////////////////////////////////////////////////////
  //
  // More specific style set, opening input files etc

  gStyle->SetOptStat(0);  ///to avoid the stat. on the plots
  char tempname[200];
  char xtitlename[200];
  char ytitlename[200];

  if(sample.find("stack")==string::npos)sprintf(tempname,"TauHad/GenInfo_HadTauEstimation_%s.root",sample.c_str());
  else sprintf(tempname,"TauHad/Stack/GenInfo_HadTauEstimation_%s.root",sample.c_str());
//cout << "warning:\n Warning \n \n  using elog195 for pre and  exp \n \n ";
  TFile * GenFile = new TFile(tempname,"R");
  sprintf(tempname,"TauHad2/Elog378_HadTauEstimation_data_SingleMuon_v15cd_.root");
  TFile * EstFile = new TFile(tempname,"R");

  //
  // Define legend
  //
  Float_t legendX1 = .55; //.50;
  Float_t legendX2 = .85; //.70;
  Float_t legendY1 = .75; //.65;
  Float_t legendY2 = .90;

  TLegend* catLeg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);
  catLeg1->SetTextSize(0.032);
  catLeg1->SetTextFont(42);

  catLeg1->SetTextSize(0.042);
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
  canvas->SetTopMargin( T/H );
  canvas->SetBottomMargin( B/H );
  canvas->SetTickx(0);
  canvas->SetTicky(0);

  canvas->Divide(1, 2);

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
  canvas_dw->SetBottomMargin(0.25);
  
  // set top margin 0 for bottom figure
  canvas_dw->SetTopMargin(0);
  
  // draw top figure
  canvas_up->cd();

  TH1D * GenHist, * EstHist,* thist;
  TH1D * histTemplate;
  THStack * tempstack;

  double HT_x_max=2500.;
  double HT_x_min=400.;
  double MHT_x_max=1000.;
  double NJet_x_max=15.;
  double NBtag_x_max=4.;
  double search_x_max=19.;
  double search_x_min=1.;


  sprintf(tempname,"%s",histname.c_str());
  if(sample.find("stacked")!=string::npos){
   
    EstHist=(TH1D*)EstFile->Get(tempname)->Clone();
    tempstack=(THStack*)GenFile->Get(tempname)->Clone();   
    GenHist=(TH1D*) tempstack->GetStack()->Last();
    
    EstHistD=(TH1D*) EstFile->Get(tempname)->Clone();
    tempstack=(THStack*)GenFile->Get(tempname)->Clone();
    GenHistD=(TH1D*) tempstack->GetStack()->Last();
    
  }
  else{
    EstHist=(TH1D*) EstFile->Get(tempname)->Clone();
    GenHist=(TH1D*) GenFile->Get(tempname)->Clone();
    EstHistD=(TH1D*) EstFile->Get(tempname)->Clone();
    GenHistD=(TH1D*) GenFile->Get(tempname)->Clone();
  }

  GenHist->SetLineColor(2);
  EstHist->SetLineColor(4);
  //GenHist->GetXaxis()->SetLabelFont(42);
  //GenHist->GetXaxis()->SetLabelOffset(0.007);
  //GenHist->GetXaxis()->SetLabelSize(0.04);
  GenHist->GetXaxis()->SetTitleSize(0.05);
  GenHist->GetXaxis()->SetTitleOffset(0.9);
  GenHist->GetXaxis()->SetTitleFont(42);
  //GenHist->GetYaxis()->SetLabelFont(42);
  //GenHist->GetYaxis()->SetLabelOffset(0.007);
  //GenHist->GetYaxis()->SetLabelSize(0.04);
  GenHist->GetYaxis()->SetTitleSize(0.05);
  GenHist->GetYaxis()->SetTitleOffset(1.25);
  GenHist->GetYaxis()->SetTitleFont(42);

  //EstHist->GetXaxis()->SetLabelFont(42);
  //EstHist->GetXaxis()->SetLabelOffset(0.007);
  //EstHist->GetXaxis()->SetLabelSize(0.04);
  EstHist->GetXaxis()->SetTitleSize(0.05);
  EstHist->GetXaxis()->SetTitleOffset(0.9);
  EstHist->GetXaxis()->SetTitleFont(42);
  //EstHist->GetYaxis()->SetLabelFont(42);
  //EstHist->GetYaxis()->SetLabelOffset(0.007);
  //EstHist->GetYaxis()->SetLabelSize(0.04);
  EstHist->GetYaxis()->SetTitleSize(0.05);
  EstHist->GetYaxis()->SetTitleOffset(1.25);
  EstHist->GetYaxis()->SetTitleFont(42);
  sprintf(xtitlename,"search bins");
  sprintf(ytitlename,"Events");
  gPad->SetLogy();
  GenHist->SetMaximum(200000);
  GenHist->SetMinimum(0.1);
  GenHist->GetXaxis()->SetRangeUser(search_x_min,search_x_max);

  GenHist->SetFillStyle(3004);
  GenHist->SetFillColor(kGreen-3);
  GenHist->SetTitle("");
  GenHist->Draw("e2");
  //KH GenHist->Draw("same");
  EstHist->Draw("same");

  TH1D * GenHist_Clone = static_cast<TH1D*>(GenHist->Clone("denominator"));

  sprintf(tempname,"#tau_{hadronic} BG");
  catLeg1->AddEntry(GenHist,tempname,"l");
  sprintf(tempname,"Data");
  catLeg1->AddEntry(EstHist,tempname,"l");
  GenHist->GetXaxis()->SetTitle(xtitlename);
  GenHist->GetYaxis()->SetTitle(ytitlename);
  catLeg1->Draw();

  TText * ttext = new TText(10. , 1700. , "4 fb-1");
  //  ttext->Draw();
  //
  // Bottom ratio plot
  //
  // ----------

  if(choice==0){
      //KH -- flip the numerator and denominator
      EstHistD->Divide(GenHistD);

      // draw bottom figure
      canvas_dw->cd();
      // font size
      EstHistD->GetXaxis()->SetLabelSize(font_size_dw);
      EstHistD->GetXaxis()->SetTitleSize(font_size_dw);
      EstHistD->GetYaxis()->SetLabelSize(font_size_dw);
      EstHistD->GetYaxis()->SetTitleSize(font_size_dw);
      
      //
      // Common to all bottom plots
      //
      sprintf(ytitlename,"Estimate / #tau_{had} BG");
      EstHistD->SetMaximum(2.65);
      EstHistD->SetMinimum(0.0);

      //
      // Specific to each bottom plot
      //
      sprintf(xtitlename,"search bin");
      EstHistD->GetXaxis()->SetRangeUser(search_x_min,search_x_max);
      TLine *tline = new TLine(search_x_min,1.,search_x_max,1.);

      // Setting style
      //EstHistD->SetMaximum(1.4);
      //EstHistD->GetXaxis()->SetLabelFont(42);
      //EstHistD->GetXaxis()->SetLabelOffset(0.007);
      //EstHistD->GetXaxis()->SetLabelSize(0.04);
      EstHistD->GetXaxis()->SetTitleSize(0.12);
      EstHistD->GetXaxis()->SetTitleOffset(0.9);
      EstHistD->GetXaxis()->SetTitleFont(42);
      //EstHistD->GetYaxis()->SetLabelFont(42);
      //EstHistD->GetYaxis()->SetLabelOffset(0.007);
      //EstHistD->GetYaxis()->SetLabelSize(0.04);
      EstHistD->GetYaxis()->SetTitleSize(0.13);
      EstHistD->GetYaxis()->SetTitleOffset(0.5);
      EstHistD->GetYaxis()->SetTitleFont(42);

      EstHistD->GetXaxis()->SetTitle(xtitlename);
      EstHistD->GetYaxis()->SetTitle(ytitlename);

      EstHistD->SetTitle("");
      EstHistD->Draw();
      tline->SetLineStyle(2);
      tline->Draw();
  }

  if(choice==1){

    /*
      TH1D * denominator = static_cast<TH1D*>(EstHist->Clone("denominator"));
      EstHistD->Add(GenHistD,-1);
      denominator->Divide(EstHistD,GenHistD,1,1,"");
    */
      TH1D * denominator = static_cast<TH1D*>(EstHist->Clone("denominator"));
      //EstHistD->Add(GenHistD,-1);
      denominator->Divide(GenHistD,EstHistD,1,1,"");

      // draw bottom figure
      canvas_dw->cd();
      // font size
      denominator->GetXaxis()->SetLabelSize(font_size_dw);
      denominator->GetXaxis()->SetTitleSize(font_size_dw);
      denominator->GetYaxis()->SetLabelSize(font_size_dw);
      denominator->GetYaxis()->SetTitleSize(font_size_dw);
      
      TLine *tline = new TLine(search_x_min,1.,search_x_max,1.);

      //
      // Common to all bottom plots
      //
      //sprintf(ytitlename,"#frac{Estimate - #tau_{had} BG}{#tau_{had} BG} ");
      sprintf(ytitlename,"#frac{Expectation}{Prediction} ");
      denominator->SetMaximum(2.65);
      denominator->SetMinimum(0.0);

      //
      // Specific to each bottom plot
      //
      // Setting style
      //denominator->SetMaximum(1.4);
      //denominator->GetXaxis()->SetLabelFont(42);
      //denominator->GetXaxis()->SetLabelOffset(0.007);
      //denominator->GetXaxis()->SetLabelSize(0.04);
      denominator->GetXaxis()->SetTitleSize(0.12);
      denominator->GetXaxis()->SetTitleOffset(0.9);
      denominator->GetXaxis()->SetTitleFont(42);
      //denominator->GetYaxis()->SetLabelFont(42);
      //denominator->GetYaxis()->SetLabelOffset(0.007);
      //denominator->GetYaxis()->SetLabelSize(0.04);
      denominator->GetYaxis()->SetTitleSize(0.13);
      denominator->GetYaxis()->SetTitleOffset(0.5);
      denominator->GetYaxis()->SetTitleFont(42);

      denominator->GetXaxis()->SetTitle(xtitlename);
      denominator->GetYaxis()->SetTitle(ytitlename);

      denominator->SetTitle("");
      denominator->Draw();
      tline->SetLineStyle(2);
      tline->Draw();
  }

  sprintf(tempname,"%s_SearchBin_Plot.png",sample.c_str());
  canvas->Print(tempname);

}
