

#include <vector>
#include <cstdio>

using namespace std;




  // Main function
  plot_ScaledMuPt_genTauJet(){

    // Don't draw the stat box
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



    Float_t legendX1 = .40;
    Float_t legendX2 = .80;
    Float_t legendY1 = .70; //.65;
    Float_t legendY2 = .85;
    TLegend* catLeg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);
    catLeg1->SetTextSize(0.032);
    catLeg1->SetTextFont(42);
    catLeg1->SetBorderSize(0);



    // Scaled Mu Pt
    
    
    TFile * MuPt_file = new TFile("HadTauEstimation_TTbar_.root","R");        
    sprintf(tempname,"allEvents/delphi/simTauJetPt_delphi_allEvents");
    TH1D * simTauJetPtHist = (TH1D*) MuPt_file->Get( tempname )->Clone();   
    simTauJetPtHist->SetLineColor(4);
    simTauJetPtHist->GetXaxis()->SetTitle("P_{T}");
    simTauJetPtHist->GetYaxis()->SetTitleOffset(1.5);
    simTauJetPtHist->GetYaxis()->SetTitle("Events");
    simTauJetPtHist->SetTitleOffset(1.5);
    simTauJetPtHist->SetTitle("#tau jet vs. scaled #mu");
//    simTauJetPtHist->SetMaximum(5000);
    simTauJetPtHist->Draw("same hist");
    sprintf(tempname,"Predicted #tau Jet(using template)");
    catLeg1->AddEntry(simTauJetPtHist,tempname,"l");


    //................//


    // Tau Jet Pt

    TFile * resp_file = new TFile("../TauHad/HadTau_TauResponseTemplates_TTbar_.root","R");
    sprintf(tempname,"tauJetPtHist");
    TH1D * tauJetPtHist = (TH1D*) resp_file->Get( tempname )->Clone();
    tauJetPtHist->SetLineColor(2);
    tauJetPtHist->GetXaxis()->SetTitle("P_{T}");
    tauJetPtHist->GetYaxis()->SetTitleOffset(1.5);
    tauJetPtHist->GetYaxis()->SetTitle("Events");
    tauJetPtHist->SetTitleOffset(1.5);
    tauJetPtHist->SetTitle("#tau vs. scaled #mu");
    tauJetPtHist->Draw("same hist");
    sprintf(tempname,"#tau Jet");
    catLeg1->AddEntry(tauJetPtHist,tempname,"l");





    catLeg1->Draw();
    
    c1->Print("simTauJet_vs_TauJet.png"); 

  }
