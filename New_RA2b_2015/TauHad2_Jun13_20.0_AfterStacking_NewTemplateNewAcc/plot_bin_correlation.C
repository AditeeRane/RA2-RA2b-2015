
{ 

  gROOT->LoadMacro("tdrstyle.C");
  setTDRStyle();
  
  TFile *_file0 = TFile::Open("HadTauEstimation_TTbar_.root");  
  _file0->ls();

  //
  //--- MHT ---
  //-----------
  TCanvas * cMHT = new TCanvas("cMHT", "cMHT", 1000, 800);
  gPad->SetGrid();
  hCorMHT_noW->GetXaxis()->SetRange(1,20);
  hCorMHT_noW->GetYaxis()->SetRange(1,20);
  hCorMHT_noW->GetXaxis()->SetTitle("MHT (original) [GeV]");
  hCorMHT_noW->GetYaxis()->SetTitle("MHT (re-calculated) [GeV]");
  hCorMHT_noW->GetXaxis()->SetTitleSize(0.05);
  hCorMHT_noW->GetYaxis()->SetTitleSize(0.05);
  hCorMHT_noW->GetXaxis()->SetTitleOffset(1.2);
  hCorMHT_noW->GetYaxis()->SetTitleOffset(1.2);
  hCorMHT_noW->Draw("text");

  TLine *tMHT1 = new TLine(200,225,975,1000);
  tMHT1->SetLineWidth(2); tMHT1->Draw();
  TLine *tMHT2 = new TLine(225,200,1000,975);
  tMHT2->SetLineWidth(2); tMHT2->Draw();
  TLine *tMHT3 = new TLine(200,200,200,225);
  tMHT3->SetLineWidth(2); tMHT3->Draw();
  TLine *tMHT4 = new TLine(200,200,225,200);
  tMHT4->SetLineWidth(2); tMHT4->Draw();

  TLine *tMHT11 = new TLine(0,200,1000,200);
  tMHT11->SetLineWidth(2); tMHT11->SetLineColor(2); tMHT11->Draw();
  TLine *tMHT11a = new TLine(200,200,200,500.);
  tMHT11a->SetLineWidth(2); tMHT11a->SetLineColor(2); tMHT11a->SetLineStyle(2); tMHT11a->Draw();
  TLine *tMHT11b = new TLine(500,200,500,500.);
  tMHT11b->SetLineWidth(2); tMHT11b->SetLineColor(2); tMHT11b->SetLineStyle(2); tMHT11b->Draw();

  TLine *tMHT12 = new TLine(0,500,1000,500);
  tMHT12->SetLineColor(2); tMHT12->Draw();
  TLine *tMHT12a = new TLine(500,500,500,750.);
  tMHT12a->SetLineWidth(2); tMHT12a->SetLineColor(2); tMHT12a->SetLineStyle(2); tMHT12a->Draw();
  TLine *tMHT12b = new TLine(750,500,750,750.);
  tMHT12b->SetLineWidth(2); tMHT12b->SetLineColor(2); tMHT12b->SetLineStyle(2); tMHT12b->Draw();

  TLine *tMHT13 = new TLine(0,750,1000,750);
  tMHT13->SetLineColor(2); tMHT13->Draw();
  TLine *tMHT13a = new TLine(750,750,750,1000.);
  tMHT13a->SetLineWidth(2); tMHT13a->SetLineColor(2); tMHT13a->SetLineStyle(2); tMHT13a->Draw();
  TLine *tMHT13b = new TLine(1000,750,1000,1000.);
  tMHT13b->SetLineWidth(2); tMHT13b->SetLineColor(2); tMHT13b->SetLineStyle(2); tMHT13b->Draw();

  int nMHT1low=0,nMHT1mid=0,nMHT1hi=0;
  int nMHT2low=0,nMHT2mid=0,nMHT2hi=0;
  int nMHT3low=0,nMHT3mid=0,nMHT3hi=0;
  for (int ibin=0; ibin<22;ibin++){
    for (int jbin=0; jbin<22;jbin++){
      Int_t gbin = hCorMHT_noW->GetBin(ibin,jbin);
      Int_t ent  = hCorMHT_noW->GetBinContent(gbin);
      Double_t xbin = hCorMHT_noW->GetXaxis()->GetBinCenter(ibin);
      Double_t ybin = hCorMHT_noW->GetYaxis()->GetBinCenter(jbin);
      //printf("x=%4.0f(%3d), y=%4.0f(%3d), gbin=%6d, ent=%6d\n",xbin,ibin,ybin,jbin,gbin,ent);
      if (ybin>200. && ybin<500.){
	if (xbin<200.)      nMHT1low += ent;
	else if (xbin<500.) nMHT1mid += ent;
	else                nMHT1hi  += ent;
      }
      if (ybin>500. && ybin<750.){
	if (xbin<500.)      nMHT2low += ent;
	else if (xbin<750.) nMHT2mid += ent;
	else                nMHT2hi  += ent;
      }
      if (ybin>750. && ybin<1000.){
	if (xbin<750.)       nMHT3low += ent;
	else if (xbin<1000.) nMHT3mid += ent;
	else                 nMHT3hi  += ent;
      }
    }
  }
  TString cmsText; TLatex l; l.SetTextAlign(22); l.SetTextSize(0.03); l.SetTextColor(2); l.SetTextFont(72);
  cmsText = std::to_string(nMHT1low); l.DrawLatex(100.,170.,cmsText);
  cmsText = std::to_string(nMHT1mid); l.DrawLatex(350.,170.,cmsText);
  cmsText = std::to_string(nMHT1hi);  l.DrawLatex(600.,170.,cmsText);
  cmsText = std::to_string(nMHT2low); l.DrawLatex(460.,720.,cmsText);
  cmsText = std::to_string(nMHT2mid); l.DrawLatex(550.,720.,cmsText);
  cmsText = std::to_string(nMHT2hi);  l.DrawLatex(800.,720.,cmsText);
  cmsText = std::to_string(nMHT3low); l.DrawLatex(700.,900.,cmsText);
  cmsText = std::to_string(nMHT3mid); l.DrawLatex(800.,900.,cmsText);

  cMHT->SaveAs("MHT_correlation.pdf");
  cMHT->SaveAs("MHT_correlation.gif");

  //
  //--- HT ---
  //-----------
  TCanvas * cHT = new TCanvas("cHT", "cHT", 1000, 800);
  gPad->SetGrid();
  hCorHT_noW->GetXaxis()->SetRange(1,20);
  hCorHT_noW->GetYaxis()->SetRange(1,20);
  hCorHT_noW->GetXaxis()->SetTitle("HT (original) [GeV]");
  hCorHT_noW->GetYaxis()->SetTitle("HT (re-calculated) [GeV]");
  hCorHT_noW->GetXaxis()->SetTitleSize(0.05);
  hCorHT_noW->GetYaxis()->SetTitleSize(0.05);
  hCorHT_noW->GetXaxis()->SetTitleOffset(1.2);
  hCorHT_noW->GetYaxis()->SetTitleOffset(1.2);
  hCorHT_noW->Draw("text");

  TLine *tHT1 = new TLine(500,550,1950,2000);
  tHT1->SetLineWidth(2); tHT1->Draw();
  TLine *tHT2 = new TLine(550,500,2000,1950);
  tHT2->SetLineWidth(2); tHT2->Draw();
  TLine *tHT3 = new TLine(500,500,500,550);
  tHT3->SetLineWidth(2); tHT3->Draw();
  TLine *tHT4 = new TLine(500,500,550,500);
  tHT4->SetLineWidth(2); tHT4->Draw();

  TLine *tHT11 = new TLine(0,500,2000,500);
  tHT11->SetLineWidth(2); tHT11->SetLineColor(2); tHT11->Draw();
  TLine *tHT11a = new TLine(500,500,500,800.);
  tHT11a->SetLineWidth(2); tHT11a->SetLineColor(2); tHT11a->SetLineStyle(2); tHT11a->Draw();
  TLine *tHT11b = new TLine(800,500,800,800.);
  tHT11b->SetLineWidth(2); tHT11b->SetLineColor(2); tHT11b->SetLineStyle(2); tHT11b->Draw();

  TLine *tHT12 = new TLine(0,800,2000,800);
  tHT12->SetLineColor(2); tHT12->Draw();
  TLine *tHT12a = new TLine(800,800,800,1200.);
  tHT12a->SetLineWidth(2); tHT12a->SetLineColor(2); tHT12a->SetLineStyle(2); tHT12a->Draw();
  TLine *tHT12b = new TLine(1200,800,1200,1200.);
  tHT12b->SetLineWidth(2); tHT12b->SetLineColor(2); tHT12b->SetLineStyle(2); tHT12b->Draw();

  TLine *tHT13 = new TLine(0,1200,2000,1200);
  tHT13->SetLineColor(2); tHT13->Draw();
  TLine *tHT13a = new TLine(1200,1200,1200,2000.);
  tHT13a->SetLineWidth(2); tHT13a->SetLineColor(2); tHT13a->SetLineStyle(2); tHT13a->Draw();
  TLine *tHT13b = new TLine(2000,1200,2000,2000.);
  tHT13b->SetLineWidth(2); tHT13b->SetLineColor(2); tHT13b->SetLineStyle(2); tHT13b->Draw();

  int nHT1low=0,nHT1mid=0,nHT1hi=0;
  int nHT2low=0,nHT2mid=0,nHT2hi=0;
  int nHT3low=0,nHT3mid=0,nHT3hi=0;
  for (int ibin=0; ibin<22;ibin++){
    for (int jbin=0; jbin<22;jbin++){
      Int_t gbin = hCorHT_noW->GetBin(ibin,jbin);
      Int_t ent  = hCorHT_noW->GetBinContent(gbin);
      Double_t xbin = hCorHT_noW->GetXaxis()->GetBinCenter(ibin);
      Double_t ybin = hCorHT_noW->GetYaxis()->GetBinCenter(jbin);
      //printf("x=%4.0f(%3d), y=%4.0f(%3d), gbin=%6d, ent=%6d\n",xbin,ibin,ybin,jbin,gbin,ent);
      if (ybin>500. && ybin<800.){
	if (xbin<500.)      nHT1low += ent;
	else if (xbin<800.) nHT1mid += ent;
	else                nHT1hi  += ent;
      }
      if (ybin>800. && ybin<1200.){
	if (xbin<800.)       nHT2low += ent;
	else if (xbin<1200.) nHT2mid += ent;
	else                 nHT2hi  += ent;
      }
      if (ybin>1200. && ybin<2000.){
	if (xbin<1200.)      nHT3low += ent;
	else if (xbin<2000.) nHT3mid += ent;
	else                 nHT3hi  += ent;
      }
    }
  }
  //TString cmsText; TLatex l; l.SetTextAlign(22); l.SetTextSize(0.03); l.SetTextColor(2); l.SetTextFont(72);
  cmsText = std::to_string(nHT1low); l.DrawLatex(400.,470.,cmsText);
  cmsText = std::to_string(nHT1mid); l.DrawLatex(650.,470.,cmsText);
  cmsText = std::to_string(nHT1hi);  l.DrawLatex(900.,470.,cmsText);
  cmsText = std::to_string(nHT2low); l.DrawLatex(750.,1100.,cmsText);
  cmsText = std::to_string(nHT2mid); l.DrawLatex(1000.,1100.,cmsText);
  cmsText = std::to_string(nHT2hi);  l.DrawLatex(1300.,1100.,cmsText);
  cmsText = std::to_string(nHT3low); l.DrawLatex(1000.,1900.,cmsText);
  cmsText = std::to_string(nHT3mid); l.DrawLatex(1300.,1900.,cmsText);

  cHT->SaveAs("HT_correlation.pdf");
  cHT->SaveAs("HT_correlation.gif");

  //
  //--- nJets ---
  //-----------
  TCanvas * cNJet = new TCanvas("cNJet", "cNJet", 1000, 800);
  gPad->SetGrid();
  hCorNJet_noW->GetXaxis()->SetRange(1,15);
  hCorNJet_noW->GetYaxis()->SetRange(1,15);
  hCorNJet_noW->GetXaxis()->SetTitle("Number of jets (original) [GeV]");
  hCorNJet_noW->GetYaxis()->SetTitle("Number of jets (re-calculated) [GeV]");
  hCorNJet_noW->GetXaxis()->SetTitleSize(0.05);
  hCorNJet_noW->GetYaxis()->SetTitleSize(0.05);
  hCorNJet_noW->GetXaxis()->SetTitleOffset(1.2);
  hCorNJet_noW->GetYaxis()->SetTitleOffset(1.2);
  hCorNJet_noW->Draw("text");

  TLine *tNJet1 = new TLine(4,4.5,14.5,15);
  tNJet1->SetLineWidth(2); tNJet1->Draw();
  TLine *tNJet2 = new TLine(4.5,4,15,14.5);
  tNJet2->SetLineWidth(2); tNJet2->Draw();
  TLine *tNJet3 = new TLine(4,4,4,4.5);
  tNJet3->SetLineWidth(2); tNJet3->Draw();
  TLine *tNJet4 = new TLine(4,4,4.5,4);
  tNJet4->SetLineWidth(2); tNJet4->Draw();

  TLine *tNJet11 = new TLine(0,4,15,4);
  tNJet11->SetLineWidth(2); tNJet11->SetLineColor(2); tNJet11->Draw();
  TLine *tNJet11a = new TLine(4,4,4,7);
  tNJet11a->SetLineWidth(2); tNJet11a->SetLineColor(2); tNJet11a->SetLineStyle(2); tNJet11a->Draw();
  TLine *tNJet11b = new TLine(7,4,7,7);
  tNJet11b->SetLineWidth(2); tNJet11b->SetLineColor(2); tNJet11b->SetLineStyle(2); tNJet11b->Draw();

  TLine *tNJet12 = new TLine(0,7,15,7);
  tNJet12->SetLineColor(2); tNJet12->Draw();
  TLine *tNJet12a = new TLine(7,7,7,9);
  tNJet12a->SetLineWidth(2); tNJet12a->SetLineColor(2); tNJet12a->SetLineStyle(2); tNJet12a->Draw();
  TLine *tNJet12b = new TLine(9,7,9,9);
  tNJet12b->SetLineWidth(2); tNJet12b->SetLineColor(2); tNJet12b->SetLineStyle(2); tNJet12b->Draw();

  TLine *tNJet13 = new TLine(0,9,15,9);
  tNJet13->SetLineColor(2); tNJet13->Draw();
  TLine *tNJet13a = new TLine(9,9,9,15);
  tNJet13a->SetLineWidth(2); tNJet13a->SetLineColor(2); tNJet13a->SetLineStyle(2); tNJet13a->Draw();
  TLine *tNJet13b = new TLine(15,9,15,15);
  tNJet13b->SetLineWidth(2); tNJet13b->SetLineColor(2); tNJet13b->SetLineStyle(2); tNJet13b->Draw();

  /*
  int nNJet1low=0,nNJet1mid=0,nNJet1hi=0;
  int nNJet2low=0,nNJet2mid=0,nNJet2hi=0;
  int nNJet3low=0,nNJet3mid=0,nNJet3hi=0;
  for (int ibin=0; ibin<20;ibin++){
    for (int jbin=0; jbin<20;jbin++){
      Int_t gbin = hCorNJet_noW->GetBin(ibin,jbin);
      Int_t ent  = hCorNJet_noW->GetBinContent(gbin);
      Double_t xbin = hCorNJet_noW->GetXaxis()->GetBinCenter(ibin);
      Double_t ybin = hCorNJet_noW->GetYaxis()->GetBinCenter(jbin);
      //printf("x=%4.0f(%3d), y=%4.0f(%3d), gbin=%6d, ent=%6d\n",xbin,ibin,ybin,jbin,gbin,ent);
      if (ybin>200. && ybin<500.){
	if (xbin<200.)      nNJet1low += ent;
	else if (xbin<500.) nNJet1mid += ent;
	else                nNJet1hi  += ent;
      }
      if (ybin>500. && ybin<750.){
	if (xbin<500.)      nNJet2low += ent;
	else if (xbin<750.) nNJet2mid += ent;
	else                nNJet2hi  += ent;
      }
      if (ybin>750. && ybin<1000.){
	if (xbin<750.)       nNJet3low += ent;
	else if (xbin<1000.) nNJet3mid += ent;
	else                 nNJet3hi  += ent;
      }
    }
  }
  //TString cmsText; TLatex l; l.SetTextAlign(22); l.SetTextSize(0.03); l.SetTextColor(2); l.SetTextFont(72);
  cmsText = std::to_string(nNJet1low); l.DrawLatex(100.,170.,cmsText);
  cmsText = std::to_string(nNJet1mid); l.DrawLatex(350.,170.,cmsText);
  cmsText = std::to_string(nNJet1hi);  l.DrawLatex(600.,170.,cmsText);
  cmsText = std::to_string(nNJet2low); l.DrawLatex(460.,720.,cmsText);
  cmsText = std::to_string(nNJet2mid); l.DrawLatex(550.,720.,cmsText);
  cmsText = std::to_string(nNJet2hi);  l.DrawLatex(800.,720.,cmsText);
  cmsText = std::to_string(nNJet3low); l.DrawLatex(700.,900.,cmsText);
  cmsText = std::to_string(nNJet3mid); l.DrawLatex(800.,900.,cmsText);
  */

  cNJet->SaveAs("NJet_correlation.pdf");
  cNJet->SaveAs("NJet_correlation.gif");
  
  //
  //--- nBtags ---
  //-----------
  TCanvas * cNBtag = new TCanvas("cNBtag", "cNBtag", 1000, 800);
  gPad->SetGrid();
  hCorNBtag_noW->GetXaxis()->SetRange(1,10);
  hCorNBtag_noW->GetYaxis()->SetRange(1,10);
  hCorNBtag_noW->GetXaxis()->SetTitle("Number of btags (original) [GeV]");
  hCorNBtag_noW->GetYaxis()->SetTitle("Number of btags (re-calculated) [GeV]");
  hCorNBtag_noW->GetXaxis()->SetTitleSize(0.05);
  hCorNBtag_noW->GetYaxis()->SetTitleSize(0.05);
  hCorNBtag_noW->GetXaxis()->SetTitleOffset(1.2);
  hCorNBtag_noW->GetYaxis()->SetTitleOffset(1.2);
  hCorNBtag_noW->Draw("text");

  TLine *tNBtag1 = new TLine(0,0.5,9.5,10);
  tNBtag1->SetLineWidth(2); tNBtag1->Draw();
  TLine *tNBtag2 = new TLine(0.5,0,10,9.5);
  tNBtag2->SetLineWidth(2); tNBtag2->Draw();
  TLine *tNBtag3 = new TLine(0,0,0,0.5);
  tNBtag3->SetLineWidth(2); tNBtag3->Draw();
  TLine *tNBtag4 = new TLine(0,0,0.5,0);
  tNBtag4->SetLineWidth(2); tNBtag4->Draw();

  /*
  TLine *tNBtag11 = new TLine(0,4,10,4);
  tNBtag11->SetLineWidth(2); tNBtag11->SetLineColor(2); tNBtag11->Draw();
  TLine *tNBtag11a = new TLine(4,4,4,7);
  tNBtag11a->SetLineWidth(2); tNBtag11a->SetLineColor(2); tNBtag11a->SetLineStyle(2); tNBtag11a->Draw();
  TLine *tNBtag11b = new TLine(7,4,7,7);
  tNBtag11b->SetLineWidth(2); tNBtag11b->SetLineColor(2); tNBtag11b->SetLineStyle(2); tNBtag11b->Draw();

  TLine *tNBtag12 = new TLine(0,7,15,7);
  tNBtag12->SetLineColor(2); tNBtag12->Draw();
  TLine *tNBtag12a = new TLine(7,7,7,9);
  tNBtag12a->SetLineWidth(2); tNBtag12a->SetLineColor(2); tNBtag12a->SetLineStyle(2); tNBtag12a->Draw();
  TLine *tNBtag12b = new TLine(9,7,9,9);
  tNBtag12b->SetLineWidth(2); tNBtag12b->SetLineColor(2); tNBtag12b->SetLineStyle(2); tNBtag12b->Draw();

  TLine *tNBtag13 = new TLine(0,9,15,9);
  tNBtag13->SetLineColor(2); tNBtag13->Draw();
  TLine *tNBtag13a = new TLine(9,9,9,15);
  tNBtag13a->SetLineWidth(2); tNBtag13a->SetLineColor(2); tNBtag13a->SetLineStyle(2); tNBtag13a->Draw();
  TLine *tNBtag13b = new TLine(15,9,15,15);
  tNBtag13b->SetLineWidth(2); tNBtag13b->SetLineColor(2); tNBtag13b->SetLineStyle(2); tNBtag13b->Draw();
  */

  /*
  int nNBtag1low=0,nNBtag1mid=0,nNBtag1hi=0;
  int nNBtag2low=0,nNBtag2mid=0,nNBtag2hi=0;
  int nNBtag3low=0,nNBtag3mid=0,nNBtag3hi=0;
  for (int ibin=0; ibin<20;ibin++){
    for (int jbin=0; jbin<20;jbin++){
      Int_t gbin = hCorNBtag_noW->GetBin(ibin,jbin);
      Int_t ent  = hCorNBtag_noW->GetBinContent(gbin);
      Double_t xbin = hCorNBtag_noW->GetXaxis()->GetBinCenter(ibin);
      Double_t ybin = hCorNBtag_noW->GetYaxis()->GetBinCenter(jbin);
      //printf("x=%4.0f(%3d), y=%4.0f(%3d), gbin=%6d, ent=%6d\n",xbin,ibin,ybin,jbin,gbin,ent);
      if (ybin>200. && ybin<500.){
	if (xbin<200.)      nNBtag1low += ent;
	else if (xbin<500.) nNBtag1mid += ent;
	else                nNBtag1hi  += ent;
      }
      if (ybin>500. && ybin<750.){
	if (xbin<500.)      nNBtag2low += ent;
	else if (xbin<750.) nNBtag2mid += ent;
	else                nNBtag2hi  += ent;
      }
      if (ybin>750. && ybin<1000.){
	if (xbin<750.)       nNBtag3low += ent;
	else if (xbin<1000.) nNBtag3mid += ent;
	else                 nNBtag3hi  += ent;
      }
    }
  }
  //TString cmsText; TLatex l; l.SetTextAlign(22); l.SetTextSize(0.03); l.SetTextColor(2); l.SetTextFont(72);
  cmsText = std::to_string(nNBtag1low); l.DrawLatex(100.,170.,cmsText);
  cmsText = std::to_string(nNBtag1mid); l.DrawLatex(350.,170.,cmsText);
  cmsText = std::to_string(nNBtag1hi);  l.DrawLatex(600.,170.,cmsText);
  cmsText = std::to_string(nNBtag2low); l.DrawLatex(460.,720.,cmsText);
  cmsText = std::to_string(nNBtag2mid); l.DrawLatex(550.,720.,cmsText);
  cmsText = std::to_string(nNBtag2hi);  l.DrawLatex(800.,720.,cmsText);
  cmsText = std::to_string(nNBtag3low); l.DrawLatex(700.,900.,cmsText);
  cmsText = std::to_string(nNBtag3mid); l.DrawLatex(800.,900.,cmsText);
  */

  cNBtag->SaveAs("NBtag_correlation.pdf");
  cNBtag->SaveAs("NBtag_correlation.gif");
  
  
}
