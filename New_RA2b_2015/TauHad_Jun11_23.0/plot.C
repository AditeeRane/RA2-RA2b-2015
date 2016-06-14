plot(string cutname="delphi",string histname="HT"){

gStyle->SetOptStat(0);
char tempname[200];
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

Float_t legendX1 = .70; //.50;
Float_t legendX2 = .93; //.70;
Float_t legendY1 = .70; //.65;
Float_t legendY2 = .85;
TLegend* catLeg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);
catLeg1->SetTextSize(0.032);
catLeg1->SetTextFont(42);

  TH1D * thist, * thist2, *thist3;

  // ratio of histograms before and after isoTrk veto
    TFile * file = new TFile("GenInfo_HadTauEstimation_TTbar_.root","R");
    sprintf(tempname,"allEvents/delphi/%s_delphi_allEvents",histname.c_str() );
    thist = (TH1D *) file->Get(tempname)->Clone();
    sprintf(tempname,"allEvents/iso/%s_iso_allEvents",histname.c_str() );
    thist2= (TH1D *) file->Get(tempname)->Clone();

    thist3 = static_cast<TH1D*>(thist2->Clone("Eff"));
    thist3->Divide(thist2,thist,1,1,"B");

    if(histname=="HT"){
      thist->GetXaxis()->SetRangeUser(500,2500);
      thist3->GetXaxis()->SetRangeUser(500,2500);
      thist3->SetMaximum(2);
    }
    if(histname=="MHT"){
      thist->GetXaxis()->SetRangeUser(200,1200);
      thist3->GetXaxis()->SetRangeUser(200,1200);
      thist3->SetMaximum(2);
    }
    if(histname=="NJet"){
      thist->GetXaxis()->SetRangeUser(4,20);
      thist3->GetXaxis()->SetRangeUser(4,20);
      thist3->SetMaximum(2);
    }
    if(histname=="NBtag"){
      thist->GetXaxis()->SetRangeUser(0,10);
      thist3->GetXaxis()->SetRangeUser(0,10);
      thist3->SetMaximum(2);
    }


    if(histname=="HT" || histname == "MHT")sprintf(tempname,"%s (GeV)",histname.c_str());
    else sprintf(tempname,"%s",histname.c_str());  
    thist3->GetXaxis()->SetTitle(tempname);
    thist3->GetYaxis()->SetTitle("Efficiency");
    thist3->SetTitle("(Iso+BaseLine)/BaseLine");
    thist3->Draw("hist");
    sprintf(tempname,"#frac{Iso+BaseLine}{BaseLine}"); 
    catLeg1->AddEntry(thist,tempname,"");
   // catLeg1->Draw();
    sprintf(tempname,"%s_ratioBeforeAfterIsoTrk.png",histname.c_str());
    c1->Print(tempname);



}
