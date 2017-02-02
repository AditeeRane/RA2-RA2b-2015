
void plot_check(string cut="mht_base", string variable="HT"){

  TFile *_file0 = TFile::Open("HadTauEstimation_PreNov21f_hadd.root");
  TFile *_file1 = TFile::Open("GenInfo_HadTauEstimation_hadd.root");
  //TFile *_file0 = TFile::Open("HadTauEstimation_TTbar_T_SingleLep_.root");
  //TFile *_file1 = TFile::Open("GenInfo_HadTauEstimation_TTbar_T_SingleLep_.root");

  char tmpname[50],tmpname2[50];
  char cname[50];
  sprintf(tmpname,"allEvents/%s/%s_%s_allEvents",cut.c_str(),variable.c_str(),cut.c_str());
  sprintf(tmpname2,"plot_%s_%s.gif",variable.c_str(),cut.c_str());
  sprintf(cname,"c_%s_%s",cut.c_str(),variable.c_str());
  TH1F* h0 = (TH1F*) _file0->Get(tmpname);
  TH1F* h1 = (TH1F*) _file1->Get(tmpname);
  gStyle->SetOptStat(1111111);
  //
  // Plotting - Tau pt
  //
  TCanvas *c1 = new TCanvas(cname,cname,800,600);
  
  h0->SetLineColor(2);
  h1->SetLineColor(4);

  h0->Scale(1.0/h0->Integral());
  h1->Scale(1.0/h1->Integral());
  h0->Draw();
  h1->Draw("sames");

  gPad->Update();

  TPaveStats *st0 = (TPaveStats*)h0->FindObject("stats");
  st0->SetLineColor(2);

  TPaveStats *st1 = (TPaveStats*)h1->FindObject("stats");
  st1->SetLineColor(4);

  st0->SetX1NDC(0.6);
  st0->SetX2NDC(0.9);
  st0->SetY1NDC(0.5);
  st0->SetY2NDC(0.7);
  st1->SetX1NDC(0.6);
  st1->SetX2NDC(0.9);
  st1->SetY1NDC(0.7);
  st1->SetY2NDC(0.9);
  st0->SetTextColor(2);
  st1->SetTextColor(4);


  TLegend* catLeg1 = new TLegend(0.5,0.3,0.8,0.4);
  catLeg1->SetTextSize(0.04);
  catLeg1->SetTextFont(42);
  catLeg1->SetLineColor(0);

  catLeg1->AddEntry(h1,"Direct from MC","l");
  catLeg1->AddEntry(h0,"Treat simulation as data","l");
  catLeg1->Draw();
  
  c1->SaveAs(tmpname2);
  
}
