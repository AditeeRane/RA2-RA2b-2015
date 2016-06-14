
void plot_TauTemplate_SingleTemplate(int i,int icomp=3, string input=""){

  //
  // icomp=0: only show own results
  //       1: show also Koushik's results
  //
  
  //
  ///////////////////////////////////////////////////////////////////////////////////////////
  ////Some cosmetic work for official documents. 
  gROOT->LoadMacro("tdrstyle.C");
  setTDRStyle();
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
  Float_t legendY2 = .90;
  TLegend* catLeg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);
  catLeg1->SetTextSize(0.042);
  catLeg1->SetTextFont(42);
  catLeg1->SetFillColor(0);
  catLeg1->SetLineColor(0);
  catLeg1->SetBorderSize(0);

  //
  
//  TFile *file_08TeV         = new TFile("tauTemplatesRes_8TeV.root","R"); 
//  TFile *file_13TeV         = new TFile("HadTau_TauResponseTemplates_stacked.root","R"); 
  sprintf(tempname,"HadTau_TauResponseTemplates_TTbar_%s.root",input.c_str());
  TFile *file_13TeV         = new TFile(tempname,"R");
  TFile *file_13TeV_NoLowPtJet = new TFile("../../TauHad/HadTau_TauResponseTemplates_TTbar_.root","R"); 
  TFile *file_13TeV_TTbar   = new TFile(tempname,"R");

  TFile *file_WJet         = new TFile("Stack/HadTau_TauResponseTemplates_WJet_stacked.root","R");
  TFile *file_TauGan       = new TFile("ROOTsOfTTbar/HadTau_TauResponseTemplates_GenTau_Matching04.root","R");


  TH1D * thist;
  catLeg1->SetHeader("p_{T}(#tau^{had}) [GeV]");

    sprintf(tempname,"hTauResp_%d",i);
    thist = (TH1D*)file_13TeV->Get(tempname)->Clone();
    sprintf(tempname2,"hTauResp_%d_AB",i);
    thist->SetName(tempname2);
    thist->SetLineColor(4);
    thist->SetFillColor(0);
    thist->SetLineWidth(3);

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
      thist->GetXaxis()->SetTitle("p_{T}(visible)/p_{T}(generated_{#tau})");
      thist->GetYaxis()->SetTitle("Probability");

    }

    thist->Draw("same,hist");
    if(i==0)sprintf(tempname,"20 - 30");
    if(i==1)sprintf(tempname,"30 - 50");
    if(i==2)sprintf(tempname,"50 - 100");
    if(i==3)sprintf(tempname,">100");
    catLeg1->AddEntry(thist,tempname,"l");
  catLeg1->Draw();

  TH1D * thist_WJet;
  if (icomp==3){
    sprintf(tempname,"hTauResp_%d",i);
    thist_WJet = (TH1D*)file_WJet->Get(tempname)->Clone();
    if(thist_WJet->Integral("width") > 0.){
     thist_WJet->Scale(1./thist_WJet->Integral("width")); 
    }
    sprintf(tempname2,"hTauResp_%d_WJet",i);
    thist_WJet->SetName(tempname2);
    thist_WJet->SetLineColor(2);
    thist_WJet->SetFillColor(0);
    thist_WJet->SetLineWidth(3);
    thist_WJet->SetLineStyle(3);
    thist_WJet->Draw("same,hist");
    if(i==0)sprintf(tempname,"20 - 30: WJet");
    if(i==1)sprintf(tempname,"30 - 50: WJet");
    if(i==2)sprintf(tempname,"50 - 100:WJet");
    if(i==3)sprintf(tempname,">100: WJet");
    catLeg1->AddEntry(thist_WJet,tempname,"l");
  }

  TH1D * thist_TauGan;
  if (icomp==3){
    sprintf(tempname,"hTauResp_%d",i);
    thist_TauGan = (TH1D*)file_TauGan->Get(tempname)->Clone();
    if(thist_TauGan->Integral("width") > 0.){
     thist_TauGan->Scale(1./thist_TauGan->Integral("width"));
    }
    sprintf(tempname2,"hTauResp_%d_TauGan",i);
    thist_TauGan->SetName(tempname2);
    thist_TauGan->SetLineColor(6);
    thist_TauGan->SetFillColor(0);
    thist_TauGan->SetLineWidth(3);
    thist_TauGan->SetLineStyle(3);
    thist_TauGan->Draw("same,hist");
    if(i==0)sprintf(tempname,"20 - 30: TauGan");
    if(i==1)sprintf(tempname,"30 - 50: TauGan");
    if(i==2)sprintf(tempname,"50 - 100:TauGan");
    if(i==3)sprintf(tempname,">100: TauGan");
    catLeg1->AddEntry(thist_TauGan,tempname,"l");
  }




  TH1D * thist_Template_x;
  if (icomp==4){
    sprintf(tempname,"hTauResp_%d_x",i);
    thist_Template_x = (TH1D*)file_13TeV->Get(tempname)->Clone();
    if(thist_Template_x->Integral("width") > 0.){
     thist_Template_x->Scale(1./thist_Template_x->Integral("width"));
    }
    sprintf(tempname2,"hTauResp_%d_X",i);
    thist_Template_x->SetName(tempname2);
    thist_Template_x->SetLineColor(1);
    thist_Template_x->SetLineWidth(3);
    thist_Template_x->SetLineStyle(8);
    thist_Template_x->Draw("same,hist");
    sprintf(tempname,"template_X");
    catLeg1->AddEntry(thist_Template_x,tempname,"l");
  }


  TH1D * thist_Template_y;
  if (icomp==4){
    sprintf(tempname,"hTauResp_%d_y",i);
    thist_Template_y = (TH1D*)file_13TeV->Get(tempname)->Clone();
    if(thist_Template_y->Integral("width") > 0.){
     thist_Template_y->Scale(1./thist_Template_y->Integral("width"));
    }
    sprintf(tempname2,"hTauResp_%d_Y",i);
    thist_Template_y->SetName(tempname2);
    thist_Template_y->SetLineColor(2);
    thist_Template_y->SetLineWidth(3);
    thist_Template_y->SetLineStyle(8);
    thist_Template_y->Draw("same,hist");
    sprintf(tempname,"template_Y");
    catLeg1->AddEntry(thist_Template_y,tempname,"l");
  }



  catLeg1->Draw();  

  sprintf(tempname,"Response%d_TTbar_%s.png",i,input.c_str());
  c1->Print(tempname);

}
