

//void plot_MtEff_forAN(){
void plot_MtEff_forICHEP2016(){ 
  //
  ///////////////////////////////////////////////////////////////////////////////////////////
  ////Some cosmetic work for official documents. 
  gROOT->LoadMacro("tdrstyle.C");
  setTDRStyle();
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
  int W = 1200;
  int H = 600;
  int H_ref = 600;
  int W_ref = 800;
  float T = 0.08*H_ref;
  float B = 0.12*H_ref;
  float L = 0.12*W_ref;
  float R = 0.04*W_ref;
  
  double ymax=1.1;
  double ymin=0.2;
  double ytext=0.34;
  
  TCanvas* c1 = new TCanvas("name","name",10,10,W,H);
  c1->SetFillColor(0);
  c1->SetBorderMode(0);
  c1->SetFrameFillStyle(0);
  c1->SetFrameBorderMode(0);
  c1->SetLeftMargin( L/W );
  c1->SetRightMargin( R/W );
  c1->SetTopMargin( T/H );
  c1->SetBottomMargin( B/H );
  c1->SetTopMargin( 0.1 );
  c1->SetLeftMargin( 0.08 );
  //c1->SetTickx(0);
  //c1->SetTicky(0);

  gStyle->SetOptStat(000000);
  gStyle->SetErrorX(0.5); 
  
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
  
  //TFile * file = new TFile("TauHad2/Elog401_MtEff.root","R");
  //TFile *file = new TFile("MtEff_TTbar_Elog227.root","R"); 
  TFile * file = new TFile("TauHad2/Elog431_MtEff.root","R");

  sprintf(tempname,"MtCutEff");
  thist = (TH1D*)file->Get(tempname)->Clone();

  thist_input = static_cast<TH1D*>(thist->Clone("thist_input"));
  shift_bin(thist_input,thist);
  thist_fixed = static_cast<TH1D*>(thist->Clone("thist_fixed"));

  thist->SetLineColor(1);
  thist->SetLineStyle(1);
  thist->SetLineWidth(3);
  thist->SetStats(kFALSE);

  thist->SetTitle("");
  
  thist->SetMaximum(ymax);
  thist->SetMinimum(ymin);
  thist->GetXaxis()->SetLabelFont(42);
  thist->GetXaxis()->SetLabelOffset(0.007);
  thist->GetXaxis()->SetLabelSize(0.045);
  thist->GetXaxis()->SetTitleSize(0.06);
  thist->GetXaxis()->SetTitleOffset(0.9);
  thist->GetXaxis()->SetTitleFont(42);
  thist->GetYaxis()->SetLabelFont(42);
  thist->GetYaxis()->SetLabelOffset(0.007);
  thist->GetYaxis()->SetLabelSize(0.045);
  thist->GetYaxis()->SetTitleSize(0.06);
  thist->GetYaxis()->SetTitleOffset(0.60);
  thist->GetYaxis()->SetTitleFont(42);

  thist->GetYaxis()->SetTitle("m_{T} cut efficiency");
  thist->GetXaxis()->SetTitle("Bin number");
  thist->SetMarkerStyle(20);
  thist->Draw();

  //TLatex * ttext1 = new TLatex(6.0 , ytext , "N_{jets}=4");
  //TLatex * ttext1 = new TLatex(6.0 , ytext , "N_{jets}=4");
  TLatex * ttext1 = new TLatex(5.75 , ytext , "3#leqN_{jets}#leq4");
  ttext1->SetTextFont(42);
  ttext1->SetTextSize(0.05);
  ttext1->SetTextAlign(22);
  ttext1->Draw();

  TLatex * ttext2 = new TLatex(17.0 , ytext , "N_{jets}=5");
  ttext2->SetTextFont(42);
  ttext2->SetTextSize(0.05);
  ttext2->SetTextAlign(22);
  ttext2->Draw();

  TLatex * ttext3 = new TLatex(28.0 , ytext , "N_{jets}=6");

  ttext3->SetTextFont(42);
  ttext3->SetTextSize(0.05);
  ttext3->SetTextAlign(22);
  ttext3->Draw();

  TLatex * ttext4 = new TLatex(36.5, ytext , "7#leqN_{jets}#leq8");
  ttext4->SetTextFont(42);
  ttext4->SetTextSize(0.05);
  ttext4->SetTextAlign(22);
  ttext4->Draw();

  TLatex * ttext5 = new TLatex(42.5 , ytext , "N_{jets}#geq9");
  ttext5->SetTextFont(42);
  ttext5->SetTextSize(0.05);
  ttext5->SetTextAlign(22);
  ttext5->Draw();

  TLine *tline_1 = new TLine(11.5,ymin,11.5,ymax);
  tline_1->SetLineStyle(2);
  tline_1->Draw();

  TLine *tline_2 = new TLine(22.5,ymin,22.5,ymax);
  tline_2->SetLineStyle(2);
  tline_2->Draw();
  
  TLine *tline_3 = new TLine(33.5,ymin,33.5,ymax);
  tline_3->SetLineStyle(2);
  tline_3->Draw();
  
  TLine *tline_4 = new TLine(39.5,ymin,39.5,ymax);
  tline_4->SetLineStyle(2);
  tline_4->Draw();

  CMS_lumi( c1, iPeriod, iPos );   // writing the lumi information and the CMS "logo"

  double xlatex=0.75;
  double ylatex=0.40;
  /*
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
*/
  c1->Print("plot_MtEff.pdf");

}

void shift_bin(TH1* input, TH1* output){

  char tempname[200];  
  char temptitle[200];  
  output->SetName(tempname);
  output->SetTitle(temptitle);
  output->SetBins(input->GetNbinsX(),input->GetBinLowEdge(1)-0.5,input->GetBinLowEdge(input->GetNbinsX()+1)-0.5);
  //input->Print("all");
  //output = new TH1D(tempname,temptitle,input->GetNbinsX(),input->GetBinLowEdge(1)-0.5,input->GetBinLowEdge(input->GetNbinsX()+1)-0.5); 
  // 0: underflow
  // 1: first bin [Use the lowedge of this bin]
  // input->GetNbinsX(): highest bin 
  // input->GetNbinsX()+1: overflow bin [use the lowedge of this bin]
  //

  for (int ibin=1;ibin<=input->GetNbinsX();ibin++){
    output->SetBinContent(ibin,input->GetBinContent(ibin));    
    output->SetBinError(ibin,input->GetBinError(ibin));    
    //std::cout << input->GetBinContent(ibin) << std::endl;
  }

}

