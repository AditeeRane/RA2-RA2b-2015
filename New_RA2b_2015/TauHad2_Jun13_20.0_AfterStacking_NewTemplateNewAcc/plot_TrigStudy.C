#include <map>

using namespace std;

void plot_TrigStudy(string histName="MHT", string trigName="HLT_Mu15_IsoVVVL_PFHT400_PFMET70_v1"){

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
  gStyle->SetPalette(1) ; // for better color output

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

  canvas->Divide(1, 2);
  //
  // Define pads
  //
  TPad* canvas_up = (TPad*) canvas->GetListOfPrimitives()->FindObject("canvas_1");
  TPad* canvas_dw = (TPad*) canvas->GetListOfPrimitives()->FindObject("canvas_2");
 
  // define the size
  double up_height     = 0.8;  // please tune so that the upper figures size will meet your requirement
  double dw_correction = 1.30; // please tune so that the smaller canvas size will work in your environment
  double font_size_dw  = 0.15;  // please tune the font size parameter for bottom figure
  double dw_height    = (1. - up_height) * dw_correction;
  double dw_height_offset = - 0.04; // KH, added to put the bottom one closer to the top panel
  double x_min=0.0, x_max=.97;
  // set pad size
  canvas_up->SetPad(0., .25, 0.97, 1.);

  canvas_dw->SetPad(0., 0.07, 0.97, 0.25);
  canvas_up->SetFrameFillColor(0);
  canvas_up->SetFillColor(0);
  canvas_dw->SetFillColor(0);
  canvas_dw->SetFrameFillColor(0);
  canvas_dw->SetBottomMargin(0.3);
  
  // set top margin 0 for bottom figure
  canvas_dw->SetTopMargin(0);
  


  
  Float_t legendX1 = .15; //.50;
  Float_t legendX2 = .50; //.70;
  Float_t legendY1 = .7; //.65;
  Float_t legendY2 = .85;
  TLegend* catLeg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);
  catLeg1->SetTextSize(0.042);
  catLeg1->SetTextFont(42);
  catLeg1->SetFillColor(0);
  catLeg1->SetLineColor(0);
  catLeg1->SetBorderSize(0);


  TH1D * temp_tt, * temp_wj, * noTrigH;
  
/*
  // From W+jets - predictions
  TFile * file_WJet_mu = new TFile("Stack/TriggerEff_WJet_stacked.root","R");  
  temp_wj_muH = (TH1D*)file_WJet_mu->Get("TrigEff")->Clone();
  TH1D * wj_muH = new TH1D("wj_muH","WJet -- Tau mistag",temp_wj_muH->GetNbinsX(),temp_wj_muH->GetXaxis()->GetXmin(),temp_wj_muH->GetXaxis()->GetXmax());
  for(int ibin=0;ibin<wj_muH->GetNbinsX()+2;ibin++){
    double con = (double)temp_wj_muH->GetBinContent(ibin);
    double err = (double)temp_wj_muH->GetBinError(ibin);
    cout << "con: " << con << " err: " << err << endl;
    wj_muH->SetBinContent(ibin,con);
    wj_muH->SetBinError(ibin,err);
  }
*/

/*    
    double XDw=0.0, XUp = 5000. , maxVal=2500.;
    if(histName=="MHT"){
      XUp = 500. , maxVal=1200.;
    }
    else if(histName=="HT"){
      XUp = 1000. , maxVal=700.;
    }
    else if(histName=="MET"){
      XUp = 500. , maxVal=1000.;
    }

  
    // From ttbar - predictions
    TFile * file_TTbar_mu = new TFile("TriggerStudy_TTbar_.root","R");
    map<string,vector<TH1D>> trigMap;
    vector<TH1D> trigVec;
    trigMap["NoTrig"]=trigVec;
    trigMap["HLT_PFHT350_PFMET120_NoiseCleaned_v1"]=trigVec;
    trigMap["HLT_Mu15_IsoVVVL_PFHT400_PFMET70_v1"]=trigVec;
    trigMap["HLT_Mu45_eta2p1_v"]=trigVec;
    trigMap["HLT_Mu50_eta2p1_v"]=trigVec;
    trigMap["HLT_Mu50_v"]=trigVec;
    trigMap["HLT_Mu55_v"]=trigVec;

    sprintf(tempname,"NoTrig/%s",histName.c_str());
    noTrigH = (TH1D*)file_TTbar_mu->Get(tempname)->Clone();
    
    int new_Nbin = (XUp-XDw)/(noTrigH->GetXaxis()->GetXmax() - noTrigH->GetXaxis()->GetXmin())*noTrigH->GetNbinsX();
    // A histogram with shorter x range to be used in efficiency calculation
    TH1D noTrigH2("noTrigH","hist with no trigger applied",new_Nbin,XDw,XUp);
    for(int ibin=0;ibin<noTrigH2.GetNbinsX();ibin++){
      double con = (double)noTrigH->GetBinContent(ibin);
      //double err = (double)noTrigH->GetBinError(ibin);
      noTrigH2.SetBinContent(ibin,con);
      //noTrigH2.SetBinError(ibin,err);
    }

    // font size
    noTrigH2.GetXaxis()->SetLabelSize(font_size_dw);
    noTrigH2.GetXaxis()->SetTitleSize(font_size_dw);
    noTrigH2.GetYaxis()->SetLabelSize(font_size_dw);
    noTrigH2.GetYaxis()->SetTitleSize(font_size_dw);


    for(double x=0.;x<XUp;x+=XUp/new_Nbin){
      printf(" x: %g bin1: %d bin2: %d \n ",x,noTrigH->FindBin(x),noTrigH2.FindBin(x));
     
    }

    int i=0;
    for(map<string,vector<TH1D>>::iterator it=trigMap.begin(); it!=trigMap.end();it++){
      i++;
      sprintf(tempname,"%s/%s",it->first.c_str(),histName.c_str());
*/
      sprintf(tempname,"%s/%s",trigname.c_str(),histName.c_str());
      temp_tt = (TH1D*)file_TTbar_mu->Get(tempname)->Clone();

      //  catLeg1->SetHeader("Prob. of #mu from #tau ");

      //...........................................................................//
      // TTbar ....................................................................//
      //...........................................................................//


      temp_tt->GetXaxis()->SetRangeUser(XDw,XUp);
      temp_tt->SetMaximum(maxVal);
      temp_tt->SetTitle("");
      
      temp_tt->GetXaxis()->SetLabelFont(42);
      temp_tt->GetXaxis()->SetLabelOffset(0.007);
      temp_tt->GetXaxis()->SetLabelSize(0.04);
      temp_tt->GetXaxis()->SetTitleSize(0.05);
      temp_tt->GetXaxis()->SetTitleOffset(0.9);
      temp_tt->GetXaxis()->SetTitleFont(42);
      temp_tt->GetYaxis()->SetLabelFont(42);
      temp_tt->GetYaxis()->SetLabelOffset(0.007);
      temp_tt->GetYaxis()->SetLabelSize(0.04);
      temp_tt->GetYaxis()->SetTitleSize(0.05);
      temp_tt->GetYaxis()->SetTitleOffset(1.0);
      temp_tt->GetYaxis()->SetTitleFont(42);
      sprintf(tempname,"%s",histName.c_str());
      temp_tt->GetXaxis()->SetTitle(tempname);
      temp_tt->GetYaxis()->SetTitle("#event");
      temp_tt->SetLineColor(i);

      // draw top figure
      canvas_up->cd();
      temp_tt->Draw("same, P");

     
      // TLegend
      sprintf(tempname,"%s",it->first.c_str());
      catLeg1->AddEntry(temp_tt,tempname,"l");    

      // draw bottom figure 
      canvas_dw->cd();
    
    
      TH1D * denominator = static_cast<TH1D*>(temp_tt->Clone("denominator"));

      // font size
      denominator->GetXaxis()->SetLabelSize(font_size_dw);
      denominator->GetXaxis()->SetTitleSize(font_size_dw);
      denominator->GetYaxis()->SetLabelSize(font_size_dw);
      denominator->GetYaxis()->SetTitleSize(font_size_dw);

      
      TH1D temp_tt2("temp_tt2","hist with no trigger applied",new_Nbin,XDw,XUp);
      for(int ibin=0;ibin<temp_tt2.GetNbinsX();ibin++){
        double con = (double)temp_tt->GetBinContent(ibin);
        //double err = (double)temp_tt->GetBinError(ibin);
        temp_tt2.SetBinContent(ibin,con);
        //temp_tt2.SetBinError(ibin,err);
      }
      

      TEfficiency * pEff = new TEfficiency(temp_tt2,noTrigH2);

      denominator->Divide(noTrigH);
      //denominator->GetYaxis()->SetTitle("efficiency");
      denominator->SetMaximum(2.);
      denominator->SetMinimum(-2.);
  
      TH1D * effH = static_cast<TH1D*>(pEff->Clone("effH"));    
      effH->SetLineColor(i);      
/*
      effH->GetXaxis()->SetLabelSize(font_size_dw);
      effH->GetXaxis()->SetTitleSize(font_size_dw);
      effH->GetYaxis()->SetLabelSize(font_size_dw);
      effH->GetYaxis()->SetTitleSize(font_size_dw);
*/

//      if(it->first!="NoTrig") denominator->Draw("same");
//      if(it->first!="NoTrig") pEff->Draw();
      if(it->first!="NoTrig") effH->Draw("AP,same");

      TLine *tline = new TLine(XDw,1.,XUp,1.);
      tline->SetLineStyle(2);
//      tline->Draw(); 
      

    }

    canvas_up->cd();
    catLeg1->Draw();
    sprintf(tempname,"TrigStudy_%s.png",histName.c_str());
    canvas->Print(tempname);
}
