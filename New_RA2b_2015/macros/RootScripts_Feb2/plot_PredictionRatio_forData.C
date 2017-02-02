//void plot_Acceptance_forICHEP2016(std::string elogForPlot=""){
void  plot_PredictionRatio_forData(string histname="searchH_b",string elog="ARElog89",string variable="MinMuPt",string numerator="MinMuPtForHighHT25",string denominator="MinMuPtForHighHT20"){  
  //
  ///////////////////////////////////////////////////////////////////////////////////////////
  ////Some cosmetic work for official documents. 
  //  gROOT->LoadMacro("tdrstyle.C");
  //setTDRStyle();
  //gROOT->LoadMacro("CMS_lumi_v2.C");
  
  char tempname[200];
  char tempname2[200];
  char tempNum[200];
  char tempDen[200];
  char tempRatio[200];
  char tempnameMod[200];
 
 int W = 1200;
  int H = 600;
  int H_ref = 600;
  int W_ref = 800;
  float T = 0.08*H_ref;
  float B = 0.12*H_ref;
  float L = 0.12*W_ref;
  float R = 0.04*W_ref;
  
  double ymax=2.;
  double ymin=-1.;
  double ytext=0.3;
  double xmin=0.;  
  TCanvas* c1 = new TCanvas("name","name",800,600);
  //c1->SetFillColor(0);
  //c1->SetBorderMode(0);
  //c1->SetFrameFillStyle(0);
  //c1->SetFrameBorderMode(0);
  //c1->SetLeftMargin( L/W );
  //c1->SetRightMargin( R/W );
  //c1->SetTopMargin( T/H );
  //c1->SetBottomMargin( B/H );
  //c1->SetTickx(0);
  //c1->SetTicky(0);

  gStyle->SetOptStat(000000);
  gStyle->SetErrorX(0.5); 
  
  Float_t legendX1 = .60; //.50;
  Float_t legendX2 = .90; //.70;
  Float_t legendY1 = .60; //.65;
  Float_t legendY2 = .90;
//  TLegend* catLeg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);
//  catLeg1->SetTextSize(0.042);
//  catLeg1->SetTextFont(42);
//  catLeg1->SetFillColor(0);
//  catLeg1->SetLineColor(0);
//  catLeg1->SetBorderSize(0);
//
  //
  
  //TFile *file = new TFile("LostLepton2_MuonEfficienciesFromTTbar_Elog213.root","R"); 
  //sprintf(tempname,"TauHad/Stack/%sLostLepton2_MuonEfficienciesFromstacked.root",elogForPlot.c_str());
  //  sprintf(tempname,"TauHad/Stack/%sLostLepton2_MuonEfficienciesFromstacked_LowStatBinNotcorrected.root",elogForPlot.c_str());
  //TFile *file   = new TFile(tempname,"R");
  sprintf(tempNum,"TauHad2/%s_HadTauEstimation_data_SingleMuon_V10_%s.root",elog.c_str(),numerator.c_str());
  TFile *file   = new TFile(tempNum,"R");
  // printf("Opened %s\n",tempNum);
  std::cout<<" opened first file "<<endl;
  sprintf(tempDen,"TauHad2/%s_HadTauEstimation_data_SingleMuon_V10_%s.root",elog.c_str(),denominator.c_str());
  TFile *file2   = new TFile(tempDen,"R");
  //  printf("Opened %s\n",tempDen);
  std::cout<<" opened second file "<<endl;
  sprintf(tempRatio,"TauHad2/%s_Ratio_HadTauEstimation_data_SingleMuon_V10_%s.root",elog.c_str(),variable.c_str());
  TFile *file3   = new TFile(tempRatio,"RECREATE");


  sprintf(tempname,"%s",histname.c_str());

  TH1D* histNum = (TH1D*)file->Get(tempname);
  // tempstack=(THStack*)EstFile->Get(tempname)->Clone();
  //stackNum=(THStack*)file->Get(tempname)->Clone();
  //  EstHist_input=(TH1D*) tempstack->GetStack()->Last();
  //histNum=(TH1D*) stackNum->GetStack()->Last();
  int nbins=histNum->GetNbinsX();
  std::cout<<"Num_nbins "<<nbins<<endl;
  TH1D* histDen = (TH1D*)file2->Get(tempname);
  //  TH1D *histDen = (TH1D*)file2->Get(tempname)->Clone();
  //stackDen=(THStack*)file2->Get(tempname)->Clone();
  //histDen=(TH1D*) stackDen->GetStack()->Last();
  std::cout<<" histDen "<< tempname <<endl;
  std::cout<<"Den_nbins "<<histDen->GetNbinsX()<<endl; 
  //histRatio = (TH1D*)file2->Get(tempname)->Clone();
  //stackRatio=(THStack*)file2->Get(tempname)->Clone();
  TH1D* histRatio=new TH1D("histRatio","histRatio",nbins,1,nbins);
  //  std::cout<<" histRatio "<< tempname <<endl;
  //std::cout<<"Ratio_nbins "<<histRatio->GetNbinsX()<<endl;
 
  for(int j=1;j<=nbins;j++){
    //std::cout<<"..........................."<<endl;
    double ratio=-5;

    double ratioErr=-5;    //std::cout<<" j "<<j<<" num "<< histNum->GetBinContent(j) <<endl;
    //std::cout<<" j "<<j<<" den "<< histDen->GetBinContent(j) <<endl; 
    if(histDen->GetBinContent(j)>0){
      ratio=histNum->GetBinContent(j)/histDen->GetBinContent(j);
      ratioErr=histNum->GetBinError(j)/histDen->GetBinError(j);
    }
    histRatio->SetBinContent(j,ratio);
    //    histRatio->SetBinError(j, 0);
      std::cout<<" Bin "<< j<< " Numerator(j) "<< histNum->GetBinContent(j) <<" Denominator(j) "<<histDen->GetBinContent(j)<<" NumeratorErr(j) "<< histNum->GetBinError(j) <<" DenominatorErr(j) "<<histDen->GetBinError(j)<<" Ratio(j) "<< ratio<<" RatiErrorCalculated "<< ratioErr<<" RatioErr(j) "<<histRatio->GetBinError(j)<< endl;    
      //histRatio->SetBinError(j,ratioErr);    
      //if(binvalue!=0)
    
    
    
  }
  
  

  //  histRatio->SetLineColor(1);
  //histRatio->SetLineWidth(3);
  //histRatio->SetStats(kFALSE);

  histRatio->SetTitle("");
  //histRatio->GetXaxis()->SetRangeUser(-1,2);

  histRatio->SetMaximum(ymax);
  histRatio->SetMinimum(ymin);
  //histRatio->GetXaxis()->SetLabelFont(42);
  //histRatio->GetXaxis()->SetLabelOffset(0.007);
  //histRatio->GetXaxis()->SetLabelSize(0.04);
  //histRatio->GetXaxis()->SetTitleSize(0.04);
  //histRatio->GetXaxis()->SetTitleOffset(1.0);
  //histRatio->GetXaxis()->SetTitleFont(42);
  //histRatio->GetYaxis()->SetLabelFont(42);
  ////histRatio->GetYaxis()->SetLabelOffset(0.007);
  //histRatio->GetYaxis()->SetLabelSize(0.04);
  //histRatio->GetYaxis()->SetTitleSize(0.04);
  ////histRatio->GetYaxis()->SetTitleOffset(1.20);
  //histRatio->GetYaxis()->SetTitleFont(42);
  //
  histRatio->GetYaxis()->SetTitle("#frac{MuPt>25}{MuPt>20} ");
  histRatio->GetXaxis()->SetTitle("Bin");
  //histRatio->SetMarkerStyle(20);
  histRatio->Draw();
  /*
  TLatex * ttext1 = new TLatex(6.5 , ytext , " N_{jets} = 3");
  //TLatex * ttext1 = new TLatex(5.5 , ytext , " N_{jets} = 3");                                                                                                        
  ttext1->SetTextFont(42);
  ttext1->SetTextSize(0.04);
  ttext1->SetTextAlign(22);
  ttext1->Draw();

  //TLatex * ttext1 = new TLatex(16.5 , ytext , " N_{jets} = 3");                                                                                                       
  TLatex * ttext2 = new TLatex(18.5 , ytext , " N_{jets} = 4");
  ttext2->SetTextFont(42);
  ttext2->SetTextSize(0.04);
  ttext2->SetTextAlign(22);
  ttext2->Draw();

  //TLatex * ttext1 = new TLatex(27.5 , ytext , " N_{jets} = 3");                                                                                                       
  TLatex * ttext3 = new TLatex(30.5 , ytext , " N_{jets} = 5");
  ttext3->SetTextFont(42);
  ttext3->SetTextSize(0.04);
  ttext3->SetTextAlign(22);
  ttext3->Draw();

  //TLatex * ttext1 = new TLatex(38.5 , ytext , " N_{jets} = 3");                                                                                                       
  TLatex * ttext4 = new TLatex(42.5 , ytext , " N_{jets} = 6");
  ttext4->SetTextFont(42);
  ttext4->SetTextSize(0.04);
  ttext4->SetTextAlign(22);
  ttext4->Draw();

  //TLatex * ttext5 = new TLatex(46.5 , ytext , "7 #leq N_{jets} #leq 8");                                                                                             
  TLatex * ttext5 = new TLatex(54.5 , ytext , "7 #leq N_{jets} #leq 8");
  ttext5->SetTextFont(42);
  ttext5->SetTextSize(0.04);
  ttext5->SetTextAlign(22);
  ttext5->Draw();

  //TLatex * ttext6 = new TLatex(52.5 , ytext , "N_{jets} #geq 9");                                                                                                     
  TLatex * ttext6 = new TLatex(66.5 , ytext , "N_{jets} #geq 9");
  ttext6->SetTextFont(42);
  ttext6->SetTextSize(0.04);
  ttext6->SetTextAlign(22);
  ttext6->Draw();


  TLine *tline_1 = new TLine(13,ymin,13,ymax);
  tline_1->SetLineStyle(2);
  tline_1->Draw();

  TLine *tline_2 = new TLine(25,ymin,25,ymax);
  tline_2->SetLineStyle(2);
  tline_2->Draw();
  
  TLine *tline_3 = new TLine(37,ymin,37,ymax);
  tline_3->SetLineStyle(2);
  tline_3->Draw();

  TLine *tline_4 = new TLine(49,ymin,49,ymax);
  tline_4->SetLineStyle(2);
  tline_4->Draw();
  
  TLine *tline_5 = new TLine(61,ymin,61,ymax);
  tline_5->SetLineStyle(2);
  tline_5->Draw();
*/
  
  c1->Print("PredictionRatio.png");
  c1->Print("PredictionRatio.pdf");

  file3->cd();
  histRatio->Write();

}
