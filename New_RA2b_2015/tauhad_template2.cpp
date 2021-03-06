///////////////////////////
#include "bootstrapUtils.h"
#include "Events.h"
#include "Selection.h"
#include "utils.h"
#include "LeptonEfficiency.h"
#include "LeptonAcceptance.h"
#include "utils2.h"
#include "Lepton_Selection.h"
#include "ISRCorrector.h"
#include "BTagCorrector.h"
#include "EventListFilter.h"

#include "TTree.h"
#include <cmath>
#include <string>
#include <vector>
#include <map>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include "TFile.h"
#include "TChain.h"
#include "TH1.h"
#include "TProfile.h"
#include "TVector2.h"
#include "TVector3.h"
#include "TRandom3.h"

using namespace std;

  class histClass{
    double * a;
    TH1D * b_hist;

    public:

    void fill(int Nhists, double * eveinfarr_, TH1D * hist_){
      a = eveinfarr_;
      b_hist=hist_;
      (*b_hist).Fill(*a);
      for(int i=1; i<=Nhists ; i++){
        (*(b_hist+i)).Fill(*(a+i),*a);
      }
    }
  };

  // Prototype a function // See AN-15-003 for more info.
  double DeltaT(unsigned int i, vector<TVector3> Jet3vec );

  int main(int argc, char *argv[]){
    /////////////////////////////////////
    if (argc != 6)
    {
      std::cout << "Please enter something like: ./run \"filelist_WJets_PU20bx25_100_200.txt\" \"WJets_PU20bx25_100_200\" \"Results\" \"00\" \"0\" " << std::endl;
      return EXIT_FAILURE;
    }
    //get the inputs from user
    const string InRootList = argv[1];
    const string subSampleKey = argv[2];
    const string Outdir = argv[3];
    const string inputnumber = argv[4];
    const string verbosity = argv[5];
    //////////////////////////////////////
    int verbose = atoi(verbosity.c_str());

    //some varaibles
    char filenames[500];
    vector<string> filesVec;
    ifstream fin(InRootList.c_str());
    TChain *sample_AUX = new TChain("TreeMaker2/PreSelection");

    char tempname[200];
    char histname[200];
    vector<TH1D > vec, vec_search;
    map<int, string> eventType;
    map<string , vector<TH1D> > cut_histvec_map, cut_histvec_map_search;
    map<string, map<string , vector<TH1D> > > map_map , map_map_evt, map_map_search , map_map_evt_search;
    map<string, histClass> histobjmap;
    histClass histObj;
    int TauResponse_nBins=4;
    int binx = -1;
    vector<TH1*> vec_resp, vec_resp_x,vec_resp_y,vec_respUp,vec_respDown;
    vector<TH2*> vec_resp_xy;
    vector<double> vec_recoMuMTW;
    vector<double> vec_MTActivity;
    vector<TLorentzVector> vec_recoMuon4vec;
    vector<TVector3> vec_recoElec3vec;
    TVector3 temp3vec;
    TLorentzVector temp4vec;
    double muPt;
    double muEta;
    double muPhi;
    double muE;
    double muMtW=-1.;
    double simTauJetPt,simTauJetPt_x,simTauJetPt_y,simTauJetPt_xy;
    double simTauJetEta;
    double simTauJetPhi,simTauJetPhi_xy;

    Double_t ht_bins[15] = {
      0., 100.,200.,300.,400.,500.,600.,700.,800.,900.,
      1000.,1200.,1500.,2000.,5000.};
    Double_t mht_bins[13] = {0., 50.,100.,150.,200.,250.,300.,350.,400.,500.,
                             700.,1000.,5000.};

    //build a vector of histograms
    TH1D weight_hist = TH1D("weight", "Weight Distribution", 5,0,5);
    vec.push_back(weight_hist);
    TH1D StatError_hist = TH1D("StatError", "#events with stat error", 1,0,2);
    StatError_hist.Sumw2();
    vec.push_back(StatError_hist);
    TH1D RA2HT_hist = TH1D("HT","HT Distribution",50,0,5000);
    RA2HT_hist.Sumw2();
    vec.push_back(RA2HT_hist);
    TH1D RA2HT2_hist = TH1D("HT2","HT2 Distribution",14,ht_bins);
    RA2HT2_hist.Sumw2();
    vec.push_back(RA2HT2_hist);
    TH1D RA2origHT_hist = TH1D("origHT","origHT Distribution",50,0,5000);
    RA2origHT_hist.Sumw2();
    vec.push_back(RA2origHT_hist);
    TH1D RA2MHT_hist = TH1D("MHT","MHT Distribution",100,0,5000);
    RA2MHT_hist.Sumw2();    
    vec.push_back(RA2MHT_hist);
    TH1D RA2MHT2_hist = TH1D("MHT2","MHT Distribution",12,mht_bins);
    RA2MHT2_hist.Sumw2();
    vec.push_back(RA2MHT2_hist);
    TH1D RA2origMHT_hist = TH1D("origMHT","origMHT Distribution",100,0,5000);
    RA2origMHT_hist.Sumw2();
    vec.push_back(RA2origMHT_hist);
    TH1D RA2MET_hist = TH1D("MET","MET Distribution",100,0,5000);
    RA2MET_hist.Sumw2();
    vec.push_back(RA2MET_hist);
    TH1D RA2origMET_hist = TH1D("origMET","origMET Distribution",100,0,5000);
    RA2origMET_hist.Sumw2();
    vec.push_back(RA2origMET_hist);
    TH1D RA2DelPhiN_hist = TH1D("DelPhiN","DelPhiN Distribution",20,0,20);
    RA2DelPhiN_hist.Sumw2();
    vec.push_back(RA2DelPhiN_hist);
    TH1D RA2DelPhi1_hist = TH1D("DelPhi1","DelPhi1 Distribution",50,0,5);
    RA2DelPhi1_hist.Sumw2();
    vec.push_back(RA2DelPhi1_hist);
    TH1D RA2DelPhi2_hist = TH1D("DelPhi2","DelPhi2 Distribution",50,0,5);
    RA2DelPhi2_hist.Sumw2();
    vec.push_back(RA2DelPhi2_hist);
    TH1D RA2DelPhi3_hist = TH1D("DelPhi3","DelPhi3 Distribution",50,0,5);
    RA2DelPhi3_hist.Sumw2();
    vec.push_back(RA2DelPhi3_hist);
    TH1D RA2DelPhi4_hist = TH1D("DelPhi4","DelPhi4 Distribution",50,0,5);
    RA2DelPhi4_hist.Sumw2();
    vec.push_back(RA2DelPhi4_hist);
    TH1D RA2NJet_hist = TH1D("NJet","Number of Jets Distribution",20,0,20);
    RA2NJet_hist.Sumw2();
    vec.push_back(RA2NJet_hist);
    TH1D RA2NBtag_hist = TH1D("NBtag","Number of Btag Distribution",20,0,20);
    RA2NBtag_hist.Sumw2();
    vec.push_back(RA2NBtag_hist);
    TH1D RA2MuonPt_hist = TH1D("MuonPt","Pt of muon Distribution",80,0,400);
    RA2MuonPt_hist.Sumw2();
    vec.push_back(RA2MuonPt_hist);
    TH1D RA2MuonEta_hist = TH1D("MuonEta","Eta of muon Distribution",50,-5,5);
    RA2MuonEta_hist.Sumw2();
    vec.push_back(RA2MuonEta_hist);
    TH1D RA2MuonPhi_hist = TH1D("MuonPhi","Phi of muon Distribution",35,-3.5,3.5);
    RA2MuonPhi_hist.Sumw2();
    vec.push_back(RA2MuonPhi_hist);
    TH1D simTauJetPt_hist = TH1D("TauJetPt","Pt of simulated tau Jet",80,0,400);
    simTauJetPt_hist.Sumw2();
    vec.push_back(simTauJetPt_hist);
    TH1D simTauJetPhi_hist = TH1D("TauJetPhi","Phi of simulated tau Jet",140,-3.5,3.5);
    simTauJetPhi_hist.Sumw2();
    vec.push_back(simTauJetPhi_hist);
    TH1D TauJet_MHT_delPhi_hist = TH1D("TauJet_MHT_delPhi","dletaPhi of tau Jet vs MHT",350,0,3.5);
    TauJet_MHT_delPhi_hist.Sumw2();
    vec.push_back(TauJet_MHT_delPhi_hist);

    int Nhists=((int)(vec.size())-1);//-1 is because weight shouldn't be counted.

    // Introduce cutflow histogram to monior event yields for early preselection
    TH1D* cutflow_preselection = new TH1D("cutflow_preselection","cutflow_preselectoion",
                                         11,0.,11.);
    cutflow_preselection->GetXaxis()->SetBinLabel(1,"All Events");
    cutflow_preselection->GetXaxis()->SetBinLabel(2,"Sample based gen-selection");
    cutflow_preselection->GetXaxis()->SetBinLabel(3,"HBHEIsoNoiseFilter");
    cutflow_preselection->GetXaxis()->SetBinLabel(4,"eeBadScFilter");
    cutflow_preselection->GetXaxis()->SetBinLabel(5,"HBHENoiseFilter");   
    cutflow_preselection->GetXaxis()->SetBinLabel(6,"GoodVtx"); 
    cutflow_preselection->GetXaxis()->SetBinLabel(7,"JetID Cleaning");
    cutflow_preselection->GetXaxis()->SetBinLabel(8,"1-lepton");
    cutflow_preselection->GetXaxis()->SetBinLabel(9,"Lepton vetoes");
    cutflow_preselection->GetXaxis()->SetBinLabel(10,"Preselection");
    cutflow_preselection->GetXaxis()->SetBinLabel(11,"Presel with weight");

    // Introduce search bin histogram
    map<string,int> binMap_mht_nj = utils2::BinMap_mht_nj();

    // Introduce the bins for IsoTrk
    map<string,int> binMap_ForIso = utils2::BinMap_ForIso(); 
    int totNbins_ForIso=binMap_ForIso.size();

    // Introduce search bin histogram
    map<string,int> binMap = utils2::BinMap_NoB();
    int totNbins=binMap.size();
    TH1* searchH = new TH1D("searchH","search bin histogram",totNbins_ForIso,1,totNbins_ForIso+1);
    searchH->Sumw2();
    // Make another hist to be filled during bootstrapping
    TH1 * searchH_evt = static_cast<TH1D*>(searchH->Clone("searchH_evt")); 
    TH1* searchH_lowDphi = new TH1D("searchH_lowDphi","search bin histogram",totNbins_ForIso,1,totNbins_ForIso+1);
    searchH_lowDphi->Sumw2();
    // Make another hist to be filled during bootstrapping
    TH1 * searchH_evt_lowDphi = static_cast<TH1D*>(searchH_lowDphi->Clone("searchH_evt_lowDphi"));

    // Introduce QCD histogram
    map<string,int> binMap_QCD = utils2::BinMap_QCD();
    int totNbins_QCD=binMap_QCD.size();
    TH1* QCD_Low = new TH1D("QCD_Low","QCD bin histogram",totNbins_QCD,1,totNbins_QCD+1);
    QCD_Low->Sumw2();
    // Make another hist to be filled during bootstrapping
    TH1 * QCD_Low_evt = static_cast<TH1D*>(QCD_Low->Clone("QCD_Low_evt"));
    TH1* QCD_Up = new TH1D("QCD_Up","QCD bin histogram",totNbins_QCD,1,totNbins_QCD+1);
    QCD_Up->Sumw2();
    // Make another hist to be filled during bootstrapping
    TH1 * QCD_Up_evt = static_cast<TH1D*>(QCD_Up->Clone("QCD_Up_evt"));
 

    // Introduce search bin histogram with bTag bins
    map<string,int> binMap_b = utils2::BinMap();
    int totNbins_b=binMap_b.size();
    TH1* searchH_b = new TH1D("searchH_b","search bin histogram",totNbins_b,1,totNbins_b+1);
    searchH_b->Sumw2();  
    TH1 * searchH_b_evt = static_cast<TH1D*>(searchH_b->Clone("searchH_b_evt"));

    TH1* searchH_b_noWeight = new TH1D("searchH_b_noWeight","search bin histogram",totNbins_b,1,totNbins_b+1);
    searchH_b_noWeight->Sumw2();
    TH1 * searchH_b_noWeight_evt = static_cast<TH1D*>(searchH_b_noWeight->Clone("searchH_b_noWeight_evt"));

    //KH-Feb2016-starts
    // Search binning - correlation
    TH2* hSearchBinCorr = new TH2D("hSearchBinCorr","hSearchBinCorr",
           totNbins_b,0.5,totNbins_b+0.5,totNbins_b,0.5,totNbins_b+0.5);
    hSearchBinCorr->Sumw2();  

    // HT,MHT,njet,nb search binning
    double SearchBinHT[4] = {500.,800.,1200.,1500.};
    TH1* hSearchBinHT = new TH1D("hSearchBinHT","hSearchBinHT",3,SearchBinHT);
    hSearchBinHT->Sumw2();  
    TH1 * hSearchBinHT_evt = static_cast<TH1D*>(hSearchBinHT->Clone("hSearchBinHT_evt"));

    double SearchBinMHT[4] = {200.,500.,750.,1200.};
    TH1* hSearchBinMHT = new TH1D("hSearchBinMHT","hSearchBinMHT",3,SearchBinMHT);
    hSearchBinMHT->Sumw2();  
    TH1 * hSearchBinMHT_evt = static_cast<TH1D*>(hSearchBinMHT->Clone("hSearchBinMHT_evt"));

    double SearchBinNjets[4] = {3.5,6.5,8.5,12.5};
    TH1* hSearchBinNjets = new TH1D("hSearchBinNjets","hSearchBinNjets",3,SearchBinNjets);
    hSearchBinNjets->Sumw2();  
    TH1 * hSearchBinNjets_evt = static_cast<TH1D*>(hSearchBinNjets->Clone("hSearchBinNjets_evt"));

    double SearchBinNb[5] = {-0.5,0.5,1.5,2.5,3.5};
    TH1* hSearchBinNb = new TH1D("hSearchBinNb","hSearchBinNb",4,SearchBinNb);
    hSearchBinNb->Sumw2();  
    TH1 * hSearchBinNb_evt = static_cast<TH1D*>(hSearchBinNb->Clone("hSearchBinNb_evt"));

    // HT,MHT,njet,nb search binning - correlation
    TH2* hSearchBinCorrHT = new TH2D("hSearchBinCorrHT","hSearchBinCorrHT",3,SearchBinHT,3,SearchBinHT);
    hSearchBinCorrHT->Sumw2();  

    TH2* hSearchBinCorrMHT = new TH2D("hSearchBinCorrMHT","hSearchBinCorrMHT",3,SearchBinMHT,3,SearchBinMHT);
    hSearchBinCorrMHT->Sumw2();  

    TH2* hSearchBinCorrNjets = new TH2D("hSearchBinCorrNjets","hSearchBinCorrNjets",3,SearchBinNjets,3,SearchBinNjets);
    hSearchBinCorrNjets->Sumw2();  

    TH2* hSearchBinCorrNb = new TH2D("hSearchBinCorrNb","hSearchBinCorrNb",4,SearchBinNb,4,SearchBinNb);
    hSearchBinCorrNb->Sumw2();  
    //KH-Feb2016-ends

    ////     

    // vector of search and QCD histograms
    TH1D searchH_ = TH1D("searchH_","search bin histogram",totNbins,1,totNbins+1); 
    searchH_.Sumw2();
    vec_search.push_back(searchH_);
    TH1D searchH_b_ = TH1D("searchH_b_","search bin histogram",totNbins_b,1,totNbins_b+1);
    searchH_b_.Sumw2();
    vec_search.push_back(searchH_b_);
    TH1D QCD_ = TH1D("QCD_","QCD bin histogram",totNbins_QCD,1,totNbins_QCD+1);
    QCD_.Sumw2();
    vec_search.push_back(QCD_);


    // some histograms for our presentations
    map<string,int> binMap_HTMHT = utils2::BinMap_HTMHT();
    int totNbins_HTMHT=binMap_HTMHT.size();
    TH1D* hPredHTMHT0b = new TH1D("hPredHTMHT0b", ";HTMHT Box;Events / Bin", totNbins_HTMHT, 1, totNbins_HTMHT+1);
    hPredHTMHT0b->Sumw2();
    TH1D* hPredHTMHT0b_evt = static_cast<TH1D*>(hPredHTMHT0b->Clone("hPredHTMHT0b_evt"));
    //
    TH1D* hPredHTMHTwb = new TH1D("hPredHTMHTwb", ";HTMHT Box;Events / Bin", totNbins_HTMHT, 1, totNbins_HTMHT+1);
    hPredHTMHTwb->Sumw2();
    TH1D* hPredHTMHTwb_evt = static_cast<TH1D*>(hPredHTMHTwb->Clone("hPredHTMHTwb_evt"));
    //
    Double_t njetbins[4] = {3.5, 6.5, 8.5, 11.5};
    TH1D* hPredNJetBins = new TH1D("hPredNJetBins", ";N_{jets} (p_{T} > 30 GeV);Events / Bin", 3, njetbins);
    hPredNJetBins->Sumw2();
    TH1D* hPredNJetBins_evt = static_cast<TH1D*>(hPredNJetBins->Clone("hPredNJetBins_evt"));
    //
    TH1D* hPredNbBins = new TH1D("hPredNbBins", ";N_{b-jets} (p_{T} > 30 GeV);Events / Bin", 4, -0.5, 3.5);
    hPredNbBins->Sumw2();
    TH1D* hPredNbBins_evt = static_cast<TH1D*>(hPredNbBins->Clone("hPredNbBins_evt"));
    //


    // Studying event weight
    TH2 * hWeightForSearchBin  = new TH2D("hWeightForSearchBin", "Weight vs. SearchBin",100.,0.,2.,totNbins_b,1,totNbins_b+1);
    hWeightForSearchBin->Sumw2(); 
    TH2 * hWeight2ForSearchBin = new TH2D("hWeight2ForSearchBin","Weight vs. SearchBin",100.,0.,2.,totNbins_b,1,totNbins_b+1);
    hWeight2ForSearchBin->Sumw2(); 
    TH2 * hWeight3ForSearchBin = new TH2D("hWeight3ForSearchBin","Weight vs. SearchBin",100.,0.,2.,totNbins_b,1,totNbins_b+1);
    hWeight3ForSearchBin->Sumw2(); 
    TH2 * hWeight4ForSearchBin = new TH2D("hWeight4ForSearchBin","Weight vs. SearchBin",100.,0.,2.,totNbins_b,1,totNbins_b+1);
    hWeight4ForSearchBin->Sumw2(); 

    TProfile * prWeightForSearchBin  = new TProfile("prWeightForSearchBin", "Weight vs. SearchBin",totNbins_b,1,totNbins_b+1,0.,2.,"s");
    prWeightForSearchBin->Sumw2(); 
    TProfile * prWeight2ForSearchBin = new TProfile("prWeight2ForSearchBin","Weight vs. SearchBin",totNbins_b,1,totNbins_b+1,0.,2.,"s");
    prWeight2ForSearchBin->Sumw2(); 
    TProfile * prWeight3ForSearchBin = new TProfile("prWeight3ForSearchBin","Weight vs. SearchBin",totNbins_b,1,totNbins_b+1,0.,2.,"s");
    prWeight3ForSearchBin->Sumw2(); 
    TProfile * prWeight4ForSearchBin = new TProfile("prWeight4ForSearchBin","Weight vs. SearchBin",totNbins_b,1,totNbins_b+1,0.,2.,"s");
    prWeight4ForSearchBin->Sumw2(); 
    double weightEffAcc;       // weight from lepton efficiency and acceptance
    double weightEffAccForEvt; // + isotrack efficiency

    // Determine correlation between original and recalculated variables
    TH2 * hCorSearch = new TH2D("hCorSearch","original vs. recalculated SearchBin",totNbins,1,totNbins+1,totNbins,1,totNbins+1);
    hCorSearch->Sumw2(); TH2 * hCorSearch_evt = static_cast<TH2D*>(hCorSearch->Clone("hCorSearch_evt"));
    TH2 * hCorHT = new TH2D("hCorHT","original vs. recalculated HT",50,0,5000,50,0,5000);
    hCorHT->Sumw2(); TH2 * hCorHT_evt = static_cast<TH2D*>(hCorHT->Clone("hCorHT_evt"));
    TH2 * hCorMHT = new TH2D("hCorMHT","original vs. recalculated MHT",100,0,5000,100,0,5000);
    hCorMHT->Sumw2(); TH2 * hCorMHT_evt = static_cast<TH2D*>(hCorMHT->Clone("hCorMHT_evt"));
    TH2 * hCorNJet = new TH2D("hCorNJet","original vs. recalculated NJet",20,0,20,20,0,20);
    hCorNJet->Sumw2(); TH2 * hCorNJet_evt = static_cast<TH2D*>(hCorNJet->Clone("hCorNJet_evt"));
    TH2 * hCorNBtag = new TH2D("hCorNBtag","original vs. recalculated NBtag",20,0,20,20,0,20);
    hCorNBtag->Sumw2(); TH2 * hCorNBtag_evt = static_cast<TH2D*>(hCorNBtag->Clone("hCorNBtag_evt"));

    // Determine correlation between original and recalculated variables + nB info
    TH2 * hCorSearch_noW = new TH2D("hCorSearch_noW","original vs. recalculated SearchBin",totNbins,1,totNbins+1,totNbins,1,totNbins+1);
    hCorSearch_noW->Sumw2(); TH2 * hCorSearch_noW_evt = static_cast<TH2D*>(hCorSearch->Clone("hCorSearch_noW_evt"));
    TH2 * hCorHT_noW = new TH2D("hCorHT_noW","original vs. recalculated HT",50,0,5000,50,0,5000);
    hCorHT_noW->Sumw2(); TH2 * hCorHT_noW_evt = static_cast<TH2D*>(hCorHT->Clone("hCorHT_noW_evt"));
    TH2 * hCorMHT_noW = new TH2D("hCorMHT_noW","original vs. recalculated MHT",100,0,5000,100,0,5000);
    hCorMHT_noW->Sumw2(); TH2 * hCorMHT_noW_evt = static_cast<TH2D*>(hCorMHT->Clone("hCorMHT_noW_evt"));
    TH2 * hCorNJet_noW = new TH2D("hCorNJet_noW","original vs. recalculated NJet",20,0,20,20,0,20);
    hCorNJet_noW->Sumw2(); TH2 * hCorNJet_noW_evt = static_cast<TH2D*>(hCorNJet->Clone("hCorNJet_noW_evt"));
    TH2 * hCorNBtag_noW = new TH2D("hCorNBtag_noW","original vs. recalculated NBtag",20,0,20,20,0,20);
    hCorNBtag_noW->Sumw2(); TH2 * hCorNBtag_noW_evt = static_cast<TH2D*>(hCorNBtag->Clone("hCorNBtag_noW_evt"));

    // Determine correlation between original and recalculated variables + nB info
    TH2 * hCorSearch_b = new TH2D("hCorSearch_b","original vs. recalculated SearchBin",totNbins_b,1,totNbins_b+1,totNbins_b,1,totNbins_b+1);
    hCorSearch_b->Sumw2(); TH2 * hCorSearch_b_evt = static_cast<TH2D*>(hCorSearch_b->Clone("hCorSearch_b_evt"));
    TH2 * hCorSearch_b_noW = new TH2D("hCorSearch_b_noW","original vs. recalculated SearchBin",totNbins_b,1,totNbins_b+1,totNbins_b,1,totNbins_b+1);
    hCorSearch_b_noW->Sumw2(); TH2 * hCorSearch_b_noW_evt = static_cast<TH2D*>(hCorSearch_b_noW->Clone("hCorSearch_b_noW_evt"));

    // Checking weights in each bin
    TH1 * hMaxWeight_HT   = new TH1D("hMaxWeight_HT",  "hMaxWeight_HT",  50,0,5000);
    TH1 * hMaxWeight_HT2  = new TH1D("hMaxWeight_HT2", "hMaxWeight_HT2", 14,ht_bins);
    TH1 * hMaxWeight_MHT  = new TH1D("hMaxWeight_MHT", "hMaxWeight_MHT", 100,0,5000);
    TH1 * hMaxWeight_MHT2 = new TH1D("hMaxWeight_MHT2","hMaxWeight_MHT2",12,mht_bins);
    TH1 * hMaxWeight_NJet   = new TH1D("hMaxWeight_NJet",  "hMaxWeight_NJet",  20,0,20);
    TH1 * hMaxWeight_NBtag  = new TH1D("hMaxWeight_NBtag", "hMaxWeight_NBtag", 20,0,20);

    // calculate iso efficiencies
    TH1* IsoElec_all = new TH1D("IsoElec_all","Isolated electron efficiency -- all ",totNbins,1,totNbins+1);
    IsoElec_all->Sumw2();
    TH1* IsoElec_pass = new TH1D("IsoElec_pass","Isolated electron efficiency -- pass ",totNbins,1,totNbins+1);
    IsoElec_pass->Sumw2();

    TH1* IsoMu_all = new TH1D("IsoMu_all","Isolated muon efficiency -- all ",totNbins,1,totNbins+1);
    IsoMu_all->Sumw2();
    TH1* IsoMu_pass = new TH1D("IsoMu_pass","Isolated muon efficiency -- pass ",totNbins,1,totNbins+1);
    IsoMu_pass->Sumw2();

    TH1* IsoPion_all = new TH1D("IsoPion_all","Isolated pion efficiency -- all ",totNbins,1,totNbins+1);
    IsoPion_all->Sumw2();
    TH1* IsoPion_pass = new TH1D("IsoPion_pass","Isolated pion efficiency -- pass ",totNbins,1,totNbins+1);
    IsoPion_pass->Sumw2();


    // The tau response templates
    Utils * utils = new Utils();

    bool fastsim=false;
    TFile * signalPileUp, *IsrFile,*skimfile;
    TH1* puhist,*h_isr, * h_genpt; 
    ISRCorrector isrcorr;
    BTagCorrector btagcorr;
    if(subSampleKey.find("fast")!=string::npos){
      cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n fastsim Monte Carlo \n "; 
      fastsim=true;
      signalPileUp = new TFile("TauHad/PileupHistograms_1104.root","R");
      puhist=(TH1*)signalPileUp->Get("pu_weights_central");
      IsrFile = new TFile("TauHad/ISRWeights.root","R");
      h_isr = (TH1*)IsrFile->Get("isr_weights_central");
      if(subSampleKey.find("T2tt_170_1")==string::npos &&
         subSampleKey.find("T2tt_172_1")==string::npos &&
         subSampleKey.find("T2tt_173_1")==string::npos
        )sample_AUX = new TChain("tree");
      
      vector<string> skimInput = utils->skimInput(subSampleKey); 
      if(skimInput.size()!=5){
        cout<<"Something is wrong with the naming of the skim file.\nUse an input like T1bbbb_mMother-1500_mLSP-800_fast \n";
        return 2;
      }
      //
      sprintf(tempname,
      //"/data3/store/user/hatake/ntuples/SusyRA2Analysis2015/Skims/Run2ProductionV4/scan/tree_SLmLoose/tree_%s_%s_%s_fast.root",
      "/data3/store/user/hatake/ntuples/SusyRA2Analysis2015/Skims/Run2ProductionV5/scan/tree_SLm/tree_%s_%s_%s_fast.root",
      skimInput[1].c_str(),skimInput[2].c_str(),skimInput[3].c_str());
      //
      skimfile = new TFile(tempname,"R");
      if(!skimfile->IsOpen()){
        cout << " \n\n first attempt to find the skim file failed. Trying to find it ... \n\n";
        sprintf(tempname,
        "/data3/store/user/borzou/ntuples/SusyRA2Analysis2015/Skims/Run2ProductionV5/scan/tree_SLm/tree_%s_%s_%s_fast.root",
        skimInput[1].c_str(),skimInput[2].c_str(),skimInput[3].c_str());
      }
      skimfile = new TFile(tempname,"R");
      if(!skimfile->IsOpen()){cout << "skim file is not open \n " ;return 2;} 
      else cout << " skimfile: " << tempname << endl;
      h_genpt = (TH1*)skimfile->Get("GenPt");
      isrcorr.SetWeights(h_isr,h_genpt);
      //PDG ID for gluino
      isrcorr.SetMother(1000021);
      //
    }
   

    ///read the file names from the .txt files and load them to a vector.
    while(fin.getline(filenames, 500) ){filesVec.push_back(filenames);}
    cout<< "\nProcessing " << subSampleKey << " ... " << endl;
    for(unsigned int in=0; in<filesVec.size(); in++){ sample_AUX->Add(filesVec.at(in).c_str()); }

    // Interface to the event content
    Events * evt = new Events(sample_AUX, subSampleKey,verbose);
    
    double fastsimWeight =1.0;
    if(subSampleKey.find("fast")!=string::npos){
      //if(filesVec.size()!=1){cout << " 1 skim file only \n"; return 2;}
      //
      //
      btagcorr.SetEffs(skimfile);
      btagcorr.SetCalib("CSVv2_mod.csv");
      btagcorr.SetFastSim(true);
      //btagcorr.SetDebug(true);
      btagcorr.SetCalibFastSim("CSV_13TEV_Combined_20_11_2015.csv");
  
      // determine the weight of fast sim signal
      sprintf(tempname,"SkimSampleXSections.txt");
      if(subSampleKey.find("T2tt")!=string::npos)sprintf(tempname,"SkimSampleXSections_t2tt.txt");
      fstream XSfile(tempname, std::fstream::in);
      string line;
      int GluinoMass =0;
      double SampleXS=0.0, XS=0.0;
      while (getline(XSfile, line)){
        if (line.empty()) continue;
        if (line[0] == '#') continue;
        stringstream ss; ss << line;
        ss >>  GluinoMass >> XS ;
        if(GluinoMass== atoi(utils->skimInput(subSampleKey)[2].c_str())){
          if(subSampleKey.find("T2tt")!=string::npos)printf(" \n \n STOP mass: %d XSection: %g \n",GluinoMass,XS);
          else printf(" \n \n Gluino mass: %d XSection: %g \n",GluinoMass,XS);
          SampleXS=XS;
          break;
        }
      } 
      XSfile.close();
      int TotNEve = evt->TotNEve();
      if(subSampleKey.find("T2tt_170_1_2bdfast")!=string::npos)TotNEve=1932026;
      else if(subSampleKey.find("T2tt_170_1_fast")!=string::npos)TotNEve=1931165;
      else if(subSampleKey.find("T2tt_172_1_fast")!=string::npos)TotNEve=1890447;
      else if(subSampleKey.find("T2tt_173_1_fast")!=string::npos)TotNEve=1912169;
      fastsimWeight = (3000 * SampleXS)/TotNEve;
      printf(" Luminosity 3000/pb fastsimWeight: %g \n",fastsimWeight);
    }

    // --- Analyse the events --------------------------------------------

    // Check consistancy
    bool isData;
    if(subSampleKey.find("Data")!=string::npos || subSampleKey.find("data")!=string::npos)isData=true;
    else isData=false;
    // 
    if( isData != evt->DataBool_() ){
      cout << " Your second input indicates code will run on Data/MC. \n Change the value of the DataBool in Events.cpp \n ";
      return 2;
    }


    // Get a pointer to the Selection class
    Selection * sel = new Selection();

    // For each selection, cut, make a vector containing the same histograms as those in vec
    for(int i=0; i<(int) sel->cutName().size();i++){
      cut_histvec_map[sel->cutName()[i]]=vec;
      cut_histvec_map_search[sel->cutName()[i]]=vec_search;
    }


    bool StudyErrorPropag = false;
    map<int,string> UncerLoop;
    // Define different event categories
    if(subSampleKey.find("templatePlus")!=string::npos)UncerLoop[0]="templatePlus";
    else if(subSampleKey.find("templateMinus")!=string::npos)UncerLoop[0]="templateMinus";
    else if(subSampleKey.find("MTSelPlus")!=string::npos)UncerLoop[0]="MTSelPlus";
    else if(subSampleKey.find("MTSelMinus")!=string::npos)UncerLoop[0]="MTSelMinus";
    else UncerLoop[0]="main";
    
 
    // get the acceptance systematics
    TFile * AccSysfile, * AccSysfile2;
    TH1 * hAccSysMax, * hAccSysMax_lowDphi, * hScaleAccSysMax, * hScaleAccSysMax_lowDphi;
    TH1 * hAccSysMin, * hAccSysMin_lowDphi, * hScaleAccSysMin, * hScaleAccSysMin_lowDphi;

    TFile * MuonIdFile, * MuonIsoFile;
    TH2F * hMuIdSF, * hMuIsoSF;
    eventType[0]="allEvents";
    if(StudyErrorPropag){
      MuonIdFile = TFile::Open("TauHad/MuonIDScaleFactor.root","READ");
      MuonIsoFile = TFile::Open("TauHad/MuonIsoScaleFactor.root","READ");
      hMuIdSF = (TH2F*) MuonIdFile->Get("pt_abseta_PLOT_pair_probeMultiplicity_bin0_&_tag_combRelIsoPF04dBeta_bin0_&_tag_pt_bin0_&_tag_IsoMu20_pass")->Clone();
      hMuIsoSF = (TH2F*) MuonIsoFile->Get("pt_abseta_PLOT_pair_probeMultiplicity_bin0_&_tag_combRelIsoPF04dBeta_bin0_&_tag_pt_bin0_&_PF_pass_&_tag_IsoMu20_pass")->Clone();

/*      
      AccSysfile = TFile::Open("TauHad/Elog408_AcceptanceSystematicsFromPDF_AllSamples.root","READ");
      AccSysfile2 = TFile::Open("TauHad/Elog408_AcceptanceSystematicsFromScale_AllSamples.root","READ");
      hAccSysMax = (TH1*) AccSysfile->Get("hAccSysMax")->Clone();
      hAccSysMax_lowDphi = (TH1*) AccSysfile->Get("hAccSysMax_lowDphi")->Clone();
      hScaleAccSysMax = (TH1*) AccSysfile2->Get("hScaleAccSysMax")->Clone();
      hScaleAccSysMax_lowDphi = (TH1*) AccSysfile2->Get("hScaleAccSysMax_lowDphi")->Clone();
      hAccSysMin = (TH1*) AccSysfile->Get("hAccSysMin")->Clone();
      hAccSysMin_lowDphi = (TH1*) AccSysfile->Get("hAccSysMin_lowDphi")->Clone();
      hScaleAccSysMin = (TH1*) AccSysfile2->Get("hScaleAccSysMin")->Clone();
      hScaleAccSysMin_lowDphi = (TH1*) AccSysfile2->Get("hScaleAccSysMin_lowDphi")->Clone();
*/
      //////////////////////////
      eventType[1]="RecoSysPlus";
      eventType[2]="RecoSysMinus";
      eventType[3]="IsoSysPlus";
      eventType[4]="IsoSysMinus";
/*
      eventType[1]="BMistagPlus";
      eventType[2]="BMistagMinus";
      eventType[7]="MuRecoIsoPlus";
      eventType[8]="MuRecoIsoMinus";
      eventType[9]="AccSysPlus";
      eventType[10]="AccSysMinus";
      eventType[11]="ScaleAccSysPlus";
      eventType[12]="ScaleAccSysMinus";
*/
      //
      //eventType[5]="IsoPlus";
      //eventType[6]="IsoMinus";
      //eventType[7]="MTPlus";
      //eventType[8]="MTMinus";
      //eventType[9]="MuFromTauPlus";
      //eventType[10]="MuFromTauMinus";
      //eventType[13]="BMistag_statPlus";
      //eventType[14]="BMistag_statMinus";  
      //eventType[15]="Tau_BrRatio_Plus";
      //eventType[16]="Tau_BrRatio_Minus";
      //eventType[17]="DileptonPlus";
      //eventType[18]="DileptonMinus";
      //eventType[19]="AccPlus";
      //eventType[20]="AccMinus";

    }
    // weights are different for different eventType
    map<string,double> totWeightMap, totWeightMap_lowDphi;
    double dummyWeight=1.;
    for(int i=0;i < eventType.size();i++){
      totWeightMap[eventType[i]]=dummyWeight;
      totWeightMap_lowDphi[eventType[i]]=dummyWeight;
    }

    //initialize a map between string and maps. copy the map of histvecs into each
    for(int i=0; i< eventType.size();i++){
      map_map[eventType[i]]=cut_histvec_map;
      map_map_search[eventType[i]]=cut_histvec_map_search;
    }
    // Make another hist to be filled during bootstrapping
    map_map_evt=map_map;
    map_map_evt_search=map_map_search;

    //initialize histobjmap
    for(map<string , vector<TH1D> >::iterator it=cut_histvec_map.begin(); it!=cut_histvec_map.end();it++){
      histobjmap[it->first]=histObj;
    }

    // Open some files and get the histograms ........................................//


    // Rate of bTagged tau jet
//    TFile * bRateFile = new TFile("TauHad/TauBtaggedRate_TTbar_Elog195.root","R");
//    TFile * bRateFile = new TFile("TauHad/Stack/TauBtaggedRate_WJet_stacked_Elog269.root","R");
    TFile * bRateFile = new TFile("TauHad/Stack/TauBtaggedRate_WJet_stacked_Elog282.root","R");
    cout << " \n\n\n\n\n WJet mistag rate is being applied \n\n\n \n\n\n " ;

    sprintf(histname,"TauBtaggedRate");
    TH1D * bRateHist = (TH1D * ) bRateFile->Get(histname)->Clone();

    // Probability of muon coming from Tau
    //TFile * Prob_Tau_mu_file = new TFile("TauHad2/Probability_Tau_mu_TTbar_Elog242.root","R");
    //TFile * Prob_Tau_mu_file = new TFile("TauHad2/Stack/Probability_Tau_mu_stacked_Elog329.root","R");
    //TFile * Prob_Tau_mu_file = new TFile("TauHad2/Stack/Elog377_Probability_Tau_mu_stacked.root","R");
    TFile * Prob_Tau_mu_file = new TFile("TauHad2/Stack/Elog401_Probability_Tau_mu_stacked.root","R");
    sprintf(histname,"hProb_Tau_mu");
    TH1D * hProb_Tau_mu =(TH1D *) Prob_Tau_mu_file->Get(histname)->Clone();
    sprintf(histname,"hProb_Tau_mu_lowDelphi");
    TH1D * hProb_Tau_mu_lowDelphi =(TH1D *) Prob_Tau_mu_file->Get(histname)->Clone();

    // Acceptance and efficiencies
    TFile * MuEffAcc_file = new TFile("LostLepton/LostLepton2_MuonEfficienciesFromTTbar_Elog212.root","R");

    //TFile * MuAcc_file = new TFile("TauHad/LostLepton2_MuonEfficienciesFromTTbar_Elog213.root","R");
    //TFile * MuAcc_file = new TFile("TauHad/Stack/Elog387_LostLepton2_MuonEfficienciesFromstacked.root","R");
    //TFile * MuAcc_file = new TFile("TauHad/Stack/Elog398_LostLepton2_MuonEfficienciesFromstacked.root","R");
    TFile * MuAcc_file = new TFile("TauHad/Stack/Elog401_LostLepton2_MuonEfficienciesFromstacked.root","R");

    sprintf(histname,"hAcc");
    TH1D * hAcc =(TH1D *) MuAcc_file->Get(histname)->Clone();
    TH1D * hAcc_lowDphi =(TH1D *) MuAcc_file->Get("hAcc_lowDphi")->Clone();
    TH1D * hEff =(TH1D *) MuEffAcc_file->Get("hEff")->Clone();

//    TFile * MuIsoEff_Arne = new TFile("TauHad/Efficiencies_Arne.root","R");
    TFile * MuIsoEff_Arne = new TFile("TauHad/New_Efficiencies_Arne.root","R");
    TH2F *hMuRecoPTActivity_Arne = (TH2F*)MuIsoEff_Arne->Get("Efficiencies/MuRecoActivityPT/MuRecoActivityPT");
    TH2F *hMuIsoPTActivity_Arne = (TH2F*)MuIsoEff_Arne->Get("Efficiencies/MuIsoActivityPT/MuIsoActivityPT");


    // Get IsoTrk efficiencies
    //TFile * IsoEffFile = new TFile("TauHad/Stack/IsoEfficiencies_stacked_Elog325.root","R");
    //TFile * IsoEffFile = new TFile("TauHad/Stack/Elog381_IsoEfficiencies_stacked.root","R");
    //TFile * IsoEffFile = new TFile("TauHad/Stack/Elog398_IsoEfficiencies_stacked.root","R");
    TFile * IsoEffFile = new TFile("TauHad/Stack/Elog401_IsoEfficiencies_stacked.root","R");
    TH1D * hIsoEff =(TH1D *) IsoEffFile->Get("IsoEff")->Clone();
    TH1D * hIsoEff_lowDphi =(TH1D *) IsoEffFile->Get("IsoEff_lowDphi")->Clone();

    TFile * IsoEffFile2 = new TFile("TauHad/IsoEfficiencies_TTbar_Elog271.root","R");
    TH1D * hIsoEff2 =(TH1D *) IsoEffFile2->Get("IsoEff2")->Clone();    

    // Get MT efficiency that is calculated here in this code
    //TFile * MtFile = new TFile("TauHad2/MtEff_TTbar_Elog227.root","R");
    //TFile * MtFile = new TFile("TauHad2/Elog335_MtEff.root","R");
    //TFile * MtFile = new TFile("TauHad2/Elog377_MtEff.root","R");
    TFile * MtFile = new TFile("TauHad2/Elog401_MtEff.root","R");
    TH1D * hMT = (TH1D *) MtFile->Get("MtCutEff")->Clone();
    TH1D * hMT_lowDphi = (TH1D *) MtFile->Get("MtCutEff_lowDphi")->Clone();


    // Inroduce two histogram to understand the probability of a muon coming from tau.
    // and also those reco mu that does not match a gen mu
    TH1D * hAll_mu = new TH1D("hAll_mu","mu from W -- search bin",totNbins_ForIso,1,totNbins_ForIso+1);
    hAll_mu->Sumw2();
    TH1D * hNonW_mu = new TH1D("hNonW_mu","mu from Tau -- search bin",totNbins_ForIso,1,totNbins_ForIso+1);
    hNonW_mu->Sumw2();
    TH1D * hAll_mu_lowDelphi = new TH1D("hAll_mu_lowDelphi","mu from W -- search bin",totNbins_ForIso,1,totNbins_ForIso+1);
    hAll_mu_lowDelphi->Sumw2();
    TH1D * hNonW_mu_lowDelphi = new TH1D("hNonW_mu_lowDelphi","mu from Tau -- search bin",totNbins_ForIso,1,totNbins_ForIso+1);
    hNonW_mu_lowDelphi->Sumw2();

    // calculate the trigger efficiency 
    TH1D * trig_all = new TH1D("trig_all"," trigger all -- search bin",totNbins,1,totNbins+1);
    trig_all->Sumw2();
    TH1D * trig_pass = new TH1D("trig_pass"," trigger pass -- search bin",totNbins,1,totNbins+1);
    trig_pass->Sumw2();


    // Use Ahmad's tau template
    TFile * resp_file_temp = new TFile("TauHad/Stack/Elog371_HadTau_TauResponseTemplates_stacked.root","R");
    //TFile * resp_file = new TFile("TauHad/Stack/HadTau_TauResponseTemplates_stacked_Elog327.root","R");
    TFile * resp_file = new TFile("TauHad/Stack/Elog404_WithJECUpDown_HadTau_TauResponseTemplates_stacked.root","R");
    for(int i=0; i<TauResponse_nBins; i++){
      sprintf(histname,"hTauResp_%d",i);
      vec_resp.push_back( (TH1D*) resp_file->Get( histname )->Clone() );
      if(subSampleKey.find("template")!=string::npos){
        sprintf(histname,"hTauResp_%d_Up",i);
        vec_respUp.push_back( (TH1D*) resp_file->Get( histname )->Clone() );
        sprintf(histname,"hTauResp_%d_Down",i);
        vec_respDown.push_back( (TH1D*) resp_file->Get( histname )->Clone() );
      }
      sprintf(histname,"hTauResp_%d_xy",i);
      vec_resp_xy.push_back( (TH2D*) resp_file->Get( histname )->Clone() );

    }

    TH2D * h2tau_phi = (TH2D*) resp_file_temp->Get("tau_GenJetPhi")->Clone();


    // Use Rishi's tau template 
    //TFile * resp_file_Rishi = new TFile("TauHad/HadTau_TauResponseTemplates_GenTau_Matching04.root","R");
    //TFile * resp_file_Rishi = new TFile("TauHad/template_singletaugun_match04_74x_v01.root","R");
    TFile * resp_file_Rishi = new TFile("TauHad/template_singletaugun_match04_74x_v02.root","R");
    for(int i=0; i<TauResponse_nBins; i++){
      sprintf(histname,"hTauResp_%d",i);
    //  vec_resp.push_back( (TH1D*) resp_file_Rishi->Get( histname )->Clone() );
    }



    // muMtW Histogram
    TH1D * muMtWHist = new TH1D("muMtW"," MT distribution of muon",40,0,200);

    // Two histograms to find rate of btagged(mistagged) tau jets as a function of pT. 
    TH1D * B_rate_all = new TH1D("B_rate_all","Pt of all matched tau jets",utils->NMuPtBins(),0,utils->MaxMuPt());
    B_rate_all->Sumw2();
    TH1D * B_rate_tagged = new TH1D("B_rate_tagged","Pt of bTagged tau jets",utils->NMuPtBins(),0,utils->MaxMuPt());
    B_rate_tagged->Sumw2();


    // see how often gen mu doesn't match reco mu
    int GenRecMu_all=0,GenRecMu_fail=0;

    // see how often there are two leptons in the the event
    int nCleanEve=0;
    TH1D * dilepton_all = new TH1D("dilepton_all"," dilepton all -- search bin",totNbins,1,totNbins+1);
    dilepton_all->Sumw2();
    TH1D * dilepton_pass = new TH1D("dilepton_pass"," dilepton pass -- search bin",totNbins,1,totNbins+1);
    dilepton_pass->Sumw2();

    TH1D * dilepton_all_lowDphi = new TH1D("dilepton_all_lowDphi"," dilepton all -- search bin",totNbins,1,totNbins+1);
    dilepton_all_lowDphi->Sumw2();
    TH1D * dilepton_pass_lowDphi = new TH1D("dilepton_pass_lowDphi"," dilepton pass -- search bin",totNbins,1,totNbins+1);
    dilepton_pass_lowDphi->Sumw2();



    // how often a muon does not match a jet
    double muBin[]={0,20,40,60,80,100,1000};
    int muNbin = sizeof(muBin)/sizeof(muBin[0]) -1 ; 
    TH1 * MuJet_all  = new TH1D("MuJet_all","mu match jet vs. pT -- all",muNbin,muBin);
    TH1 * MuJet_fail = new TH1D("MuJet_fail","mu match jet vs. pT -- fail",muNbin,muBin);

    // Determine which model to work with
    int TauHadModel=utils2::TauHadModel;    

    if(TauHadModel!=0 && TauHadModel!=1 && TauHadModel!=2 && TauHadModel!=3 && TauHadModel!=4){
      cout << " The model is not recognized! Please check utils2.h \n ";
      return 1;
    }

    // bootstrap or not ?
    int nLoops;  
    int nBtagsForHadTau;
    if(utils2::bootstrap){nLoops=50;nBtagsForHadTau=2;}
    else {nLoops=1;nBtagsForHadTau=1;}
    
    if(!utils2::bootstrap && StudyErrorPropag)cout << " propagation of errors are not handled right when bootstrap is off :( .\n Turn it on or fix me please :) . \n";


    int sampletype=-1;
    if(subSampleKey.find("TTbar_Inclusive")!=string::npos)sampletype=0; //TTbar_Inclusive
    else if(subSampleKey.find("TTbar_Tbar_SingleLep")!=string::npos || subSampleKey.find("TTbar_T_SingleLep")!=string::npos)sampletype=1;
    else if(subSampleKey.find("TTbar_DiLept")!=string::npos)sampletype=2;  
    else if(subSampleKey.find("TTbar_HT_600_800")!=string::npos)sampletype=3;
    else if(subSampleKey.find("TTbar_HT_800_1200")!=string::npos)sampletype=4;
    else if(subSampleKey.find("TTbar_HT_1200_2500")!=string::npos)sampletype=5;
    else if(subSampleKey.find("TTbar_HT_2500_Inf")!=string::npos)sampletype=6;
    else if(subSampleKey.find("TTbar")!=string::npos){
      cout << " TT sample is not known. Please check the second input \n " ;
      return 2;  
    }

    //initialize a met filter for data
    EventListFilter filter("csc2015.txt");

    double eventWeight = 1.0;
    int eventN=0;
    while( evt->loadNext() ){
      eventN++;


      eventWeight = evt->weight();
      //eventWeight = evt->weight()/evt->puweight();
      //if(subSampleKey.find("TTbar_Tbar_SingleLep")!=string::npos)eventWeight = 2.984e-06;
      //if(subSampleKey.find("TTbar_DiLept")!=string::npos)eventWeight = 2.84141e-06;




      //if(eventN>2000)break;
      cutflow_preselection->Fill(0.,eventWeight); // keep track of all events processed
      
      if(!evt->DataBool_()){
      
        if(sampletype==0){
          if(evt->gen_ht()>600||evt->GenElecPtVec_().size()>0||evt->GenMuPtVec_().size()>0||evt->GenTauPtVec_().size()>0)continue;
        }

        if(sampletype==1){
          if(evt->gen_ht()>600)continue;
        }

        if(sampletype==2){
          if(evt->gen_ht()>600)continue;
        }
        
      }


      // apply filter on bad jets if fastsim is on
      if(fastsim && evt->Cut())continue;
     
      cutflow_preselection->Fill(1.,eventWeight);
      if( !fastsim && evt->HBHEIsoNoiseFilter_()==0)continue;
      cutflow_preselection->Fill(2.,eventWeight);
      if( !fastsim && evt->eeBadScFilter_()==0)continue;
      cutflow_preselection->Fill(3.,eventWeight);
      if( !fastsim && evt->HBHENoiseFilter_()==0)continue;
      if(evt->DataBool_() && !fastsim && !filter.CheckEvent(evt->Runnum(),evt->LumiBlocknum(),evt->Evtnum()))continue;
      cutflow_preselection->Fill(4.,eventWeight);
      if(!(evt->NVtx_() >0))continue;
      cutflow_preselection->Fill(5.,eventWeight); 
      // Through out an event that contains HTjets with bad id
      if( !fastsim && evt->JetId()==0)continue;
      cutflow_preselection->Fill(6.,eventWeight); // events passing JetID event cleaning

      nCleanEve++;

      // Trigger check
      bool trigPass=false;
      if(isData){
        string triggerNameToBeUsed = "HLT_Mu15_IsoVVVL_PFHT350_v";
        if (!evt->DataBool_()) triggerNameToBeUsed = "HLT_Mu15_IsoVVVL_PFHT400_v";
        bool trigfound=false;
        if(verbose!=0)
          cout << "############################\n "; 

        for(int i=0; i< evt->TriggerNames_().size(); i++){ 
          if(verbose!=0){
            cout << evt->TriggerNames_().at(i) << endl; 
            cout << " Pass: " << evt->PassTrigger_().at(i) << " \n+\n";
          }

          string trigStr;
          if(!isData)trigStr="PFHT400";
          if(isData)trigStr="PFHT350";
          sprintf(tempname,"HLT_Mu15_IsoVVVL_%s_v",trigStr.c_str());
          //if( evt->TriggerNames_().at(i).find(triggerNameToBeUsed) != string::npos ){          
          if( evt->TriggerNames_().at(i).find(tempname) != string::npos ){
          //if( evt->TriggerNames_().at(i).find("HLT_Mu50_v") != string::npos ){
          //if( evt->TriggerNames_().at(i).find("HLT_Mu15_IsoVVVL_PFHT600_v2") != string::npos ){



  //        if( evt->TriggerNames_().at(i).find(tempname) != string::npos
  //            || evt->TriggerNames_().at(i).find("HLT_Mu50_v") != string::npos
  //          ){
  /*
          if( evt->TriggerNames_().at(i).find(tempname) != string::npos
              || evt->TriggerNames_().at(i).find("HLT_Mu50_v") != string::npos
              || evt->TriggerNames_().at(i).find("HLT_Mu15_IsoVVVL_PFHT600_v2") != string::npos
            ){
  */
            trigfound=true; 
            if(evt->PassTrigger_().at(i)==1)trigPass=true;
          }

        }

        if(!trigfound ){
          cout << " ####\n ####\n trigger was not found \n ####\n ";
        }
        if(eventN < 100 )cout<< "A temporary selection is in effect \n\n\nA temporary selection is in effect \n\n\nA temporary selection is in effect ";
        if(!trigPass)continue;
      }
    
 
      // to study some of the uncertainties we need to make some changes from
      // the very beginning and observe how that propagates
      
      if(subSampleKey.find("template")!=string::npos  && (eventN < 10 || eventN % 100000 ==0))cout << " calculating templatePlus/Minus \n ";
      if(subSampleKey.find("MTSel")!=string::npos  && (eventN < 10 || eventN % 100000 ==0))cout << " calculating MTSelPlus/Minus \n ";
      for(int iuncer=0; iuncer<UncerLoop.size() ;iuncer++){



        /////////////////////////////////////////////////////////////////////////////////////
        // Select the control sample:
        // - select events with exactly one well-reconstructed, isolated muon
        // Use the muon to predict the energy deposits of the
        // hadronically decaying tau:
        // - scale the muon pt by a random factor drawn from the
        // tau-reponse template to simulate the tau measurement
        // - use the simulated tau-pt to predict HT, MHT, and N(jets)

        if(verbose!=0)printf("@@@@@@@@@@@@@@@@@@@@@@@@ \n eventN: %d \n ",eventN);

        // select muons with pt>20. eta<2.1 relIso<.2
        vec_recoMuMTW.clear(); 
        vec_recoMuon4vec.clear();
        vec_MTActivity.clear();  

  vector<int> MuFromTauVec;//Ahmad33
  MuFromTauVec.clear();//Ahmad33



        // Consistancy check
        if(isData==true && TauHadModel<3){
          cout << "Only TauHadModel>=3 is valid for Data \n ";
          return 2;
        }
        
        if(TauHadModel>=3){
          for(int i=0; i< evt->MuPtVec_().size(); i++){ // Ahmad33
            double pt=evt->MuPtVec_().at(i); // Ahmad33
            double eta=evt->MuEtaVec_().at(i); // Ahmad33
            double phi=evt->MuPhiVec_().at(i); // Ahmad33
            double energy=evt->MuEVec_().at(i); // Ahmad33
            double activity_ = evt->MTActivityVec_().at(i);
            double mu_mt_w =utils->calcMT(pt,phi,evt->met(),evt->metphi());  
            if(UncerLoop[iuncer]=="MTSelPlus")mu_mt_w =utils->calcMT(pt,phi,evt->met()*1.3,evt->metphi());
            if(UncerLoop[iuncer]=="MTSelMinus")mu_mt_w =utils->calcMT(pt,phi,evt->met()*0.7,evt->metphi());
            if( pt> LeptonAcceptance::muonPtMin()  && fabs(eta)< LeptonAcceptance::muonEtaMax()  ){
              if(verbose==2)printf(" \n Muons: \n pt: %g eta: %g phi: %g \n ",pt,eta,phi);
              temp4vec.SetPtEtaPhiE(pt,eta,phi,energy);
              if(utils2::applyMT){
                if(mu_mt_w < 100. ){vec_recoMuon4vec.push_back(temp4vec);vec_MTActivity.push_back(activity_);}
              }
              else {vec_recoMuon4vec.push_back(temp4vec);vec_MTActivity.push_back(activity_);}
              vec_recoMuMTW.push_back(mu_mt_w); 
            }
          }

        }
        else{
          for(int i=0; i< evt->GenMuPtVec_().size(); i++){ // Ahmad33
            double pt=evt->GenMuPtVec_().at(i); // Ahmad33
            double eta=evt->GenMuEtaVec_().at(i); // Ahmad33
            double phi=evt->GenMuPhiVec_().at(i); // Ahmad33
            double energy=evt->GenMuEVec_().at(i); // Ahmad33
            double activity_ = evt->GenMTActivityVec_().at(i);
            double mu_mt_w =utils->calcMT(pt,phi,evt->met(),evt->metphi());
            if( pt> LeptonAcceptance::muonPtMin()  && fabs(eta)< LeptonAcceptance::muonEtaMax()  ){
              if(verbose==2)printf(" \n Muons: \n pt: %g eta: %g phi: %g \n ",pt,eta,phi);
              temp4vec.SetPtEtaPhiE(pt,eta,phi,energy);
              if(utils2::applyMT){
                if(mu_mt_w < 100. ){vec_recoMuon4vec.push_back(temp4vec);vec_MTActivity.push_back(activity_);}
              }
              else {vec_recoMuon4vec.push_back(temp4vec);vec_MTActivity.push_back(activity_);}
              MuFromTauVec.push_back(evt->GenMuFromTauVec_()[i]);//Ahmad33
              vec_recoMuMTW.push_back(mu_mt_w); 
            }
          }
        }

        ///select electrons with pt>10. eta<2.5 relIso<.2
        vec_recoElec3vec.clear();


  /* Ahmad33
        for(int i=0; i< evt->ElecPtVec_().size(); i++){
          double pt=evt->ElecPtVec_().at(i);
          double eta=evt->ElecEtaVec_().at(i);
          double phi=evt->ElecPhiVec_().at(i);
          // double mu_mt_w =muonsMtw->at(i);  ????
  //        if( pt>10. && fabs(eta)< 2.5 ){   // These are applied at the treemaker level. Also,
            // we suppose to use supercluster eta. While here for the cut, we are using gsf. 
  Ahmad33 */


        if(TauHadModel < 2){
          for(int i=0; i< evt->GenElecPtVec_().size(); i++){// Ahmad33
            double pt=evt->GenElecPtVec_().at(i); // Ahmad33
            double eta=evt->GenElecEtaVec_().at(i); // Ahmad33 
            double phi=evt->GenElecPhiVec_().at(i); // Ahmad33

              if(verbose==2)printf(" \n Electrons: \n pt: %g eta: %g phi: %g \n ",pt,eta,phi);
              temp3vec.SetPtEtaPhi(pt,eta,phi);
              vec_recoElec3vec.push_back(temp3vec);

    //        }

          }
        }

        
        if(vec_recoMuMTW.size()>0)muMtW = vec_recoMuMTW[0]; 
        muMtWHist->Fill(muMtW,eventWeight);

        if(verbose==1)printf(" \n **************************************** \n #Muons: %d #Electrons: %d \n ****************************** \n ",vec_recoMuon4vec.size(),vec_recoElec3vec.size());

        //if( template_nMuons == 1 && template_nElectrons == 0 ) {
        bool pass1=false;
        if(TauHadModel>=2){if( vec_recoMuon4vec.size() == 1)pass1=true;}
        else {if( vec_recoMuon4vec.size() == 1 && vec_recoElec3vec.size() == 0 )pass1=true;} // recoElec is realy GenElec here

        if(pass1){

          muPt = vec_recoMuon4vec[0].Pt();
          muEta = vec_recoMuon4vec[0].Eta();
          muPhi = vec_recoMuon4vec[0].Phi();
          muE  = vec_recoMuon4vec[0].E();

  //        double activity= utils->MuActivity(muEta,muPhi,evt->JetsPtVec_(),evt->JetsEtaVec_(),evt->JetsPhiVec_(),evt->Jets_chargedEmEnergyFraction_(),evt->Jets_chargedHadronEnergyFraction_());
          double activity=vec_MTActivity[0]; 
          if(verbose!=0)cout << " activity: " << activity << endl;




          cutflow_preselection->Fill(7.,eventWeight); // 1-mu selection

   
        bool pass1_1=false;
        // for veto we have a lepton collection with softer requirements. pT > 10 not 20 and eta < 2.4 not 2.1 and also there is no 
        // mT cut applied. 
        if(TauHadModel>=2){if(evt->MuPtVec_().size()>1 || evt->ElecPtVec_().size()>0)pass1_1=true;}
        else{ if(evt->GenMuPtVec_().size()>1 || evt->GenElecPtVec_().size()>0)pass1_1=true;}
        if(pass1_1)continue;


        cutflow_preselection->Fill(8.,eventWeight); // Lepton vetos



          // The muon we are using is already part of a jet. (Note: the muon is isolated by 0.2 but jet is much wider.) And,
          // its momentum is used in HT and MHT calculation. We need to subtract this momentum and add the contribution from the simulated tau jet.

          //Identify the jet containing the muon
          const double deltaRMax = 0.4; // 0.4 is delR of jet
          int JetIdx=-1;
          if(verbose!=0 && utils->findMatchedObject(JetIdx,muEta,muPhi,evt->JetsPtVec_(), evt->JetsEtaVec_(), evt->JetsPhiVec_(),deltaRMax,verbose) ){
            printf(" \n **************************************** \n JetIdx: %d \n ",JetIdx);
          }

          GenRecMu_all++;
          // If muon does not match a GenMuon, drop the event. We do this by applying some corrections 
          int GenMuIdx=-1;
          if(!isData && !utils->findMatchedObject(GenMuIdx,muEta,muPhi,evt->GenMuPtVec_(),evt->GenMuEtaVec_(),evt->GenMuPhiVec_(),deltaRMax,verbose)){
            GenRecMu_fail++;
            if(evt->DataBool_()==false && eventN < 100){
              printf(" Warning! There is no Gen Muon \n ");
              printf("@@@@@@@@@@@@@@@@@@\n eventN: %d \n MuPt: %g MuEta: %g MuPhi: %g \n ",eventN,muPt,muEta,muPhi);
            }
            
          }

          // start of bootstrapping ( if is on ) 
          for(int l=1; l<=nLoops;l++){
            
            // Get random number from tau-response template
            // or if bootstrap is on read the whole template
            // The template is chosen according to the muon pt
            double scale, scaleUp, scaleDown;
            if(utils2::bootstrap){
              scale = utils->GetBinValue(muPt,vec_resp,l );
              if(subSampleKey.find("template")!=string::npos){
                scaleUp = utils->GetBinValue(muPt,vec_respUp,l );
                scaleDown = utils->GetBinValue(muPt,vec_respDown,l );
              }
              if(UncerLoop[iuncer]=="templatePlus")scale=scaleUp;       
              else if(UncerLoop[iuncer]=="templateMinus")scale=scaleDown;
            }
            else scale = utils->getRandom(muPt,vec_resp );
            Double_t scale_x=0,scale_y=0;
            utils->getRandom2(muPt,vec_resp_xy,scale_x,scale_y );

      
            simTauJetPt = scale * muPt;
            simTauJetEta = muEta;
            simTauJetPhi = muPhi;

            double simTauJetPt_x = scale_x * muPt;  
            double simTauJetPt_y = scale_y * muPt;
            simTauJetPhi_xy = muPhi + TMath::ATan2(simTauJetPt_y,simTauJetPt_x);
            simTauJetPt_xy = sqrt( pow(simTauJetPt_x,2)+pow(simTauJetPt_y,2) ); 
            if(subSampleKey.find("templatePlus")!=string::npos && scale > 2.5)binx = utils->tau_phi_GetBinX(2.49); 
            else binx = utils->tau_phi_GetBinX(scale);
            // when bootstapping we work with 1D template. 
            // It was good if we could use 2D ( we are short in time now ) 
            if(utils2::bootstrap){
              double phi_genTau_tauJet=0.;
              if(binx!=0){
                if(verbose!=0)cout << "deltaPhi: " << h2tau_phi->ProjectionY("angularTemplate",binx,binx,"")->GetRandom() << endl;
                phi_genTau_tauJet=h2tau_phi->ProjectionY("angularTemplate",binx,binx,"")->GetRandom();    
              }
              simTauJetPhi_xy=simTauJetPhi + phi_genTau_tauJet ;
              simTauJetPt_xy=simTauJetPt; 
            }


            if(verbose!=0)printf(" \n ######### \n scale_x: %g scale_y: %g \n simTauJetPt_x: %g simTauJetPt_y: %g \n simTauJetPt_xy: %g simTauJetPt: %g \n simTauJetPhi_xy: %g simTauJetPhi: %g \n",scale_x,scale_y,simTauJetPt_x,simTauJetPt_y,simTauJetPt_xy,simTauJetPt,simTauJetPhi_xy,simTauJetPhi);


    //######################################################################

            // 3Vec of muon and scaledMu 
            TVector3 SimTauJet3Vec,NewTauJet3Vec,Muon3Vec;
            double NewTauJetPt=0.0;
            SimTauJet3Vec.SetPtEtaPhi(simTauJetPt_xy,simTauJetEta,simTauJetPhi_xy);
            Muon3Vec.SetPtEtaPhi(muPt,muEta,muPhi);

            // New ht and mht 
            vector<TVector3> HT3JetVec,MHT3JetVec;
            HT3JetVec.clear();
            MHT3JetVec.clear();
            TVector3 temp3Vec;
            int slimJetIdx=-1;
            MuJet_all->Fill(muPt,eventWeight);
            utils->findMatchedObject(slimJetIdx,muEta,muPhi,evt->slimJetPtVec_(),evt->slimJetEtaVec_(), evt->slimJetPhiVec_(),deltaRMax,verbose);
/*
            printf("############# \n mu ==> pT: %g eta: %g phi: %g \n ",muPt,muEta,muPhi);
            printf(" pT: %g eta: %g phi: %g delR: %g \n "
                  ,evt->JetsPtVec_()[slimJetIdx],evt->JetsEtaVec_()[slimJetIdx],evt->JetsPhiVec_()[slimJetIdx]
                  ,sqrt( pow((muEta - evt->slimJetEtaVec_()[slimJetIdx]),2.) + pow((utils->deltaPhi(muPhi,evt->slimJetPhiVec_()[slimJetIdx])),2.) ));
            if()
*/
            int jetIdx=-1;
            if(slimJetIdx!=-1 && utils->findMatchedObject(jetIdx,evt->slimJetEtaVec_()[slimJetIdx],evt->slimJetPhiVec_()[slimJetIdx],
            evt->JetsPtVec_(),evt->JetsEtaVec_(),evt->JetsPhiVec_(),0.1,verbose) &&
            evt->Jets_muonMultiplicity_()[jetIdx]==1)
            {
              double muFrac = evt->Jets_muonEnergyFraction_()[jetIdx];
              double muPtModified = (evt->JetsEVec_()[jetIdx]*muFrac/muE)*muPt;
              //printf(" mu: ==> PtModified: %g Pt: %g muPtModified/muPt: %g \n ",muPtModified,muPt,muPtModified/muPt); 
              if(muPtModified/muPt < 2.)Muon3Vec.SetPtEtaPhi(muPtModified,muEta,muPhi);
            }


            // If there is no match, add the tau jet as a new one
            if(slimJetIdx==-1){

              printf("############# \n mu ==> pT: %g eta: %g phi: %g \n ",muPt,muEta,muPhi);
              for(int ij=0; ij< evt->JetsPtVec_().size(); ij++){
                double dphi = utils->deltaPhi(muPhi,evt->JetsPhiVec_()[ij]);
                double deta = muEta - evt->JetsEtaVec_()[ij];
                int mumult = evt->Jets_muonMultiplicity_()[ij];
                if(ij+1 <= evt->JetsPtVec_().size())
                printf(" indx: %d pT: %g eta: %g phi: %g delR: %g muMultiplicity: %d \n "
                        ,ij,evt->JetsPtVec_()[ij],evt->JetsEtaVec_()[ij],evt->JetsPhiVec_()[ij],sqrt( deta*deta + dphi*dphi ), mumult);
                
              }

              MuJet_fail->Fill(muPt,eventWeight);
              NewTauJet3Vec=SimTauJet3Vec;
              NewTauJetPt = NewTauJet3Vec.Pt();
              if(NewTauJet3Vec.Pt()>30. && fabs(NewTauJet3Vec.Eta())<2.4)HT3JetVec.push_back(NewTauJet3Vec);
              if(NewTauJet3Vec.Pt()>30. && fabs(NewTauJet3Vec.Eta())<5.)MHT3JetVec.push_back(NewTauJet3Vec);
            }
            for(int i=0;i<evt->slimJetPtVec_().size();i++){
              if(i!=slimJetIdx){
                temp3Vec.SetPtEtaPhi(evt->slimJetPtVec_()[i],evt->slimJetEtaVec_()[i],evt->slimJetPhiVec_()[i]);
                if(evt->slimJetPtVec_()[i]>30. && fabs(evt->slimJetEtaVec_()[i])<2.4)HT3JetVec.push_back(temp3Vec);
                if(evt->slimJetPtVec_()[i]>30. && fabs(evt->slimJetEtaVec_()[i])<5.)MHT3JetVec.push_back(temp3Vec);
              }
              else if(i==slimJetIdx){
                temp3Vec.SetPtEtaPhi(evt->slimJetPtVec_()[i],evt->slimJetEtaVec_()[i],evt->slimJetPhiVec_()[i]);
                NewTauJet3Vec=temp3Vec-Muon3Vec+SimTauJet3Vec;
                NewTauJetPt = NewTauJet3Vec.Pt();
                if(NewTauJet3Vec.Pt()>30. && fabs(NewTauJet3Vec.Eta())<2.4)HT3JetVec.push_back(NewTauJet3Vec);
                if(NewTauJet3Vec.Pt()>30. && fabs(NewTauJet3Vec.Eta())<5.)MHT3JetVec.push_back(NewTauJet3Vec);
              }
              
            }

            // Order the HT3JetVec and MHT3JetVec based on their pT
            HT3JetVec = utils->Order_the_Vec(HT3JetVec); 
            MHT3JetVec = utils->Order_the_Vec(MHT3JetVec);


            double newHT=0,newMHT=0,newMHTPhi=-1;
            TVector3 newMHT3Vec;
            for(int i=0;i<HT3JetVec.size();i++){
              newHT+=HT3JetVec[i].Pt();
            }        
            for(int i=0;i<MHT3JetVec.size();i++){
              newMHT3Vec-=MHT3JetVec[i];
            }        
            newMHT=newMHT3Vec.Pt();
            newMHTPhi=newMHT3Vec.Phi();

            if(verbose==1)printf("newHT: %g newMHT: %g newMHTPhi: %g \n ",newHT,newMHT,newMHTPhi);

    //######################################################################

            // Calculate muon's btag rate 
            if(!utils2::bootstrap){
              JetIdx=-1;
              utils->findMatchedObject(JetIdx,muEta,muPhi,evt->JetsPtVec_(),evt->JetsEtaVec_(), evt->JetsPhiVec_(),deltaRMax,verbose);  
              B_rate_all->Fill(NewTauJetPt,eventWeight);
              if(JetIdx!=-1 && evt->csvVec()[JetIdx]>evt->csv_()){
                B_rate_tagged->Fill(NewTauJetPt,eventWeight);
              }
            }

            // start of btag on/off for had tau jets ( if is on ) 
            for(int m=0; m<nBtagsForHadTau;m++){

              
              // New #b
              double NewNB=evt->nBtags();
              // get the rate of tau jet mistagging as a function of pT.
              double bRateError_stat, bRatePlus_stat, bRateMinus_stat;
              double bRate =bRateHist->GetBinContent(bRateHist->GetXaxis()->FindBin(NewTauJet3Vec.Pt()));
              bRateError_stat = bRateHist->GetBinError(bRateHist->GetXaxis()->FindBin(NewTauJet3Vec.Pt()));
              if(bRate==0)bRate=0.0000000000000000001;
              bRatePlus_stat=bRate+bRateError_stat;
              bRateMinus_stat=bRate-bRateError_stat;

              //KH20150617
              double Prob_Btag = 1.;
              double Prob_Btag_Plus = 1., Prob_Btag_Plus_stat = 1.;
              double Prob_Btag_Minus = 1., Prob_Btag_Minus_stat = 1.;
              if(utils2::bootstrap){
                double bRate_Plus = bRate + bRate * .5;
                double bRate_Minus = bRate - bRate * .5;
                if (m==0){
                   Prob_Btag=(1.-bRate); // had tau not b-tagged
                   Prob_Btag_Plus=(1. - bRate_Plus);
                   Prob_Btag_Minus=(1. - bRate_Minus);

                   Prob_Btag_Plus_stat=(1. - bRatePlus_stat);
                   Prob_Btag_Minus_stat=(1. - bRateMinus_stat);
                }
                else if (m==1){ 
                  NewNB++;
                  Prob_Btag=bRate; // had tau b-tagged
                  Prob_Btag_Plus=bRate_Plus;
                  Prob_Btag_Minus=bRate_Minus;

                  Prob_Btag_Plus_stat=bRatePlus_stat;
                  Prob_Btag_Minus_stat=bRateMinus_stat;
                }               
                //std::cout << m << " " << bRate << " " << Prob_Btag << std::endl;
              }
              else {
                // get a random number between 0 and 1
                TRandom3 * ran = new TRandom3(0);
                double rn = ran->Rndm();
                // If statistics is high enough, in bRate*100 % of cases the random number is smaller than bRate. 
                if(rn < bRate )NewNB++; 
                delete ran;
              }


              if(verbose==2){
                double directHT=0,directMHTX=0,directMHTY=0,directMHT=0;
                printf(" ========\n Jets section: \n ");
                for(int i=0;i<evt->slimJetPtVec_().size();i++){
                  printf(" jet#: %d pt: %g eta: %g phi: %g JetID: %d \n ",i+1,evt->slimJetPtVec_()[i],evt->slimJetEtaVec_()[i],evt->slimJetPhiVec_()[i],evt->slimJetID_()[i]);
                  if(evt->slimJetPtVec_()[i]>30. && fabs(evt->slimJetEtaVec_()[i]) < 5.){
                    directMHTX-=evt->slimJetPtVec_()[i]*cos(evt->slimJetPhiVec_()[i]);
                    directMHTY-=evt->slimJetPtVec_()[i]*sin(evt->slimJetPhiVec_()[i]);
                  }
                  if(evt->slimJetPtVec_()[i]>30. && fabs(evt->slimJetEtaVec_()[i]) < 2.4){
                    directHT+=evt->slimJetPtVec_()[i];
                  }

                }
                printf("========== \n");
                for(int i=0;i<evt->JetsPtVec_().size();i++){
                  printf(" jet#: %d pt: %g eta: %g phi: %g \n ",i+1,evt->JetsPtVec_()[i],evt->JetsEtaVec_()[i],evt->JetsPhiVec_()[i]);
                }
                printf("========== \n");
                directMHT = pow( (directMHTX*directMHTX+directMHTY*directMHTY) ,.5);
                printf("directHT: %g directMHT: %g \n",directHT,directMHT);
                if((int) directHT != (int) evt->ht() )cout << " Warning in HT calc. \n " ;
                if((int) directMHT != (int) evt->mht())cout << " Warning in MHT calc. \n " ;
              }

                   
                  
              //New MET
              double metX = evt->met()*cos(evt->metphi())-simTauJetPt_xy*cos(simTauJetPhi_xy)+muPt*cos(muPhi);///the minus sign is because of Mht definition.
              double metY = evt->met()*sin(evt->metphi())-simTauJetPt_xy*cos(simTauJetPhi_xy)+muPt*sin(muPhi);

              if(verbose==1)printf("############ \n metX: %g, metY: %g \n",metX,metY);
              if(verbose==1)printf("evt->met: %g, evt->metphi: %g,muPt: %g simTauJetPt: %g, simTauJetPhi_xy: %g \n",evt->met(),evt->metphi(),muPt,simTauJetPt,simTauJetPhi_xy);

              double newMet = sqrt(pow(metX,2)+pow(metY,2));
              double newMetphi=-99.;
              newMetphi=TMath::ATan2(metY,metX);

              if(verbose==1)printf("\n evt->ht(): %g evt->mht(): %g, evt->mhtphi(): %g \n ",evt->ht(),evt->mht(),evt->mhtphi());
              if(verbose==1)printf("\n newMet: %g, newMetphi: %g \n ", newMet,newMetphi);

              // New minDelPhi_N
              double dpnhat[3]; 
              unsigned int goodcount=0;
              for(unsigned int i=0; i< HT3JetVec.size();i++){
                if(goodcount<3 && HT3JetVec[i].Pt() > 30. && fabs( HT3JetVec[i].Eta() ) < 5. ){ 
                  float dphi=std::abs(TVector2::Phi_mpi_pi(HT3JetVec[i].Phi() - newMetphi));
                  float dT=DeltaT(i,HT3JetVec);
                  if(dT/newMet>=1.0)dpnhat[goodcount]=dphi/(TMath::Pi()/2.0);
                  else dpnhat[goodcount]=dphi/asin(dT/newMet);
                  ++goodcount;
                }
              }// end loop over jets
              float mindpn=9999;
              for(int i=0; i<3; ++i){
                if(mindpn>fabs(dpnhat[i]))mindpn=fabs(dpnhat[i]);
              } 



      //#############################################################
              //New #Jet

              int newNJet = HT3JetVec.size(); 
              if(verbose==1)printf("newNJet: %d \n ",newNJet);

      //#############################################################

              // If the jet is dropped, Nbtag should stay the same. Since the muon jet is not btagged, dropping it should not change #b. 
              if( (int) HT3JetVec.size() < (int) evt->nJets() )NewNB=evt->nBtags(); 

              // if muon jet is dropped and muon is btagged, #original b shoud reduce by 1
              // if muon jet is dropped but muon is not btagged, #b shoud stat the same as original one(no increase).
              // if muon jet is not dropped but is btagged, #b shoud stat the same as original one(no increase).
              JetIdx=-1;
              utils->findMatchedObject(JetIdx,muEta,muPhi,evt->JetsPtVec_(),evt->JetsEtaVec_(), evt->JetsPhiVec_(),deltaRMax,verbose);  
              if( (int) HT3JetVec.size() < (int) evt->nJets() ){
                if(JetIdx!=-1 && evt->csvVec()[JetIdx]>evt->csv_())NewNB=evt->nBtags()-1;
                else NewNB=evt->nBtags(); 
              }
              else if(JetIdx!=-1 && evt->csvVec()[JetIdx]>evt->csv_())NewNB=evt->nBtags();


              // New dphi1, dphi2, and dphi3
              double newDphi1=-99.,newDphi2=-99.,newDphi3=-99.,newDphi4=-99.;
              //first order the jets based on their pT
              double p1=0,p2=0,p3=0,p4=0;
              int i1=-1,i2=-1,i3=-1,i4=-1;
              for(int i=0; i < HT3JetVec.size(); i++ ){
                if(HT3JetVec[i].Pt()>p1){
                  p1=HT3JetVec[i].Pt();
                  i1=i;
                }
              }
              for(int i=0; i < HT3JetVec.size(); i++ ){
                if(i==i1)continue;
                if(HT3JetVec[i].Pt()>p2){
                  p2=HT3JetVec[i].Pt();
                  i2=i;
                }
              }
              for(int i=0; i < HT3JetVec.size(); i++ ){
                if(i==i1 || i==i2)continue;
                if(HT3JetVec[i].Pt()>p3){
                  p3=HT3JetVec[i].Pt();
                  i3=i;
                }
              }
              for(int i=0; i < HT3JetVec.size(); i++ ){
                if(i==i1 || i==i2 || i==i3)continue;
                if(HT3JetVec[i].Pt()>p4){
                  p4=HT3JetVec[i].Pt();
                  i4=i;
                }
              }

              if(i1!=-1)newDphi1=fabs(TVector2::Phi_mpi_pi(HT3JetVec[i1].Phi() - newMHTPhi ));
              if(i2!=-1)newDphi2=fabs(TVector2::Phi_mpi_pi(HT3JetVec[i2].Phi() - newMHTPhi ));
              if(i3!=-1)newDphi3=fabs(TVector2::Phi_mpi_pi(HT3JetVec[i3].Phi() - newMHTPhi ));
              if(i4!=-1)newDphi4=fabs(TVector2::Phi_mpi_pi(HT3JetVec[i4].Phi() - newMHTPhi ));
             
              double simTauJetPhi_ForPlotting=-99.0;
              double tauJet_mht_dlePhi_forPlotting=-99.0;
              if( NewTauJetPt > 30.){
                simTauJetPhi_ForPlotting = simTauJetPhi_xy; 
                tauJet_mht_dlePhi_forPlotting = fabs(TVector2::Phi_mpi_pi( simTauJetPhi_ForPlotting - newMHTPhi ));
                //printf("phi(tau,mht): %g tauJetPt: %g GenTauPt: %g \n ",tauJet_mht_dlePhi_forPlotting,simTauJetPt_xy,muPt);

              }

 
              if(verbose!=0){
                printf("newDphi1: %g newDphi2: %g newDphi3: %g newDphi4: %g\n ",newDphi1,newDphi2,newDphi3,newDphi4);
                printf("i1: %d p1: %g i2: %d p2: %g i3: %d p3: %g i4: %d p4: %g \n ",i1,p1,i2,p2,i3,p3,i4,p4);
                for(int i=0; i < HT3JetVec.size(); i++){
                  printf("i: %d HT3JetVec[i].Pt(): %g \n ",i,HT3JetVec[i].Pt());
                }
              }


              // get the effieciencies and acceptance
              // if baseline cuts on the main variables are passed then calculate the efficiencies otherwise simply take 0.75 as the efficiency.
              double Eff,Eff_Arne,Reco_error_Arne, Iso_error_Arne, Eff_ArnePlus, Eff_ArneMinus;


              // Here Eff is not a good naming. What this really mean is efficiency and also isolation together
              Eff_Arne=hMuRecoPTActivity_Arne->GetBinContent(hMuRecoPTActivity_Arne->GetXaxis()->FindBin(activity),hMuRecoPTActivity_Arne->GetYaxis()->FindBin(muPt));
              Reco_error_Arne = hMuRecoPTActivity_Arne->GetBinError(hMuRecoPTActivity_Arne->GetXaxis()->FindBin(activity),hMuRecoPTActivity_Arne->GetYaxis()->FindBin(muPt));
              Eff_Arne*=hMuIsoPTActivity_Arne->GetBinContent(hMuIsoPTActivity_Arne->GetXaxis()->FindBin(activity),hMuIsoPTActivity_Arne->GetYaxis()->FindBin(muPt));
              Iso_error_Arne = hMuIsoPTActivity_Arne->GetBinError(hMuIsoPTActivity_Arne->GetXaxis()->FindBin(activity),hMuIsoPTActivity_Arne->GetYaxis()->FindBin(muPt));


              if(sel->ht_500(newHT) && sel->mht_200(newMHT) && sel->Njet_4(newNJet)){
                // Eff = hEff->GetBinContent(binMap_b[utils2::findBin(newNJet,NewNB,newHT,newMHT)]);
                Eff = hEff->GetBinContent(binMap[utils2::findBin_NoB(newNJet,newHT,newMHT)]); 
              }else{
                Eff=0.75;
              }

              // if baseline cuts on the main variables are passed then calculate the acceptance otherwise simply take 0.9 as the acceptance.
              double Acc, AccError, AccPlus, AccMinus, Acc_lowDphi, Acc_lowDphiError, AccPlus_lowDphi, AccMinus_lowDphi;
              double AccSysPlus, AccSysMinus, AccSysPlus_lowDphi, AccSysMinus_lowDphi, ScaleAccSysPlus, ScaleAccSysMinus, ScaleAccSysPlus_lowDphi, ScaleAccSysMinus_lowDphi;
              double IsoSFUp, IsoSFDw, IdSFUp,IdSFDw;

              if(sel->ht_500(newHT) && sel->mht_200(newMHT) && sel->Njet_4(newNJet)){
                // Acc = hAcc->GetBinContent(binMap_b[utils2::findBin_b(newNJet,NewNB,newHT,newMHT)]);
                // Acc = hAcc->GetBinContent(binMap[utils2::findBin_NoB(newNJet,newHT,newMHT)]);
                Acc = hAcc->GetBinContent(binMap_ForIso[utils2::findBin_ForIso(newNJet,newHT,newMHT)]);
                AccError = hAcc->GetBinError(binMap_ForIso[utils2::findBin_ForIso(newNJet,newHT,newMHT)]);
                Acc_lowDphi = hAcc_lowDphi->GetBinContent(binMap_ForIso[utils2::findBin_ForIso(newNJet,newHT,newMHT)]); 
                Acc_lowDphiError = hAcc_lowDphi->GetBinError(binMap_ForIso[utils2::findBin_ForIso(newNJet,newHT,newMHT)]);
                // use original ht mht njet to get acc. Becaue mht is different in 1mu event than hadronic event 
                // Or use recomputed ht mht ... when making Acc. 
                //Acc = hAcc->GetBinContent(binMap_ForIso[utils2::findBin_ForIso(evt->nJets(),evt->ht(),evt->mht())]);
              }else{
                Acc=0.9;
                Acc_lowDphi=0.9;
              }


              if(verbose==2 && sel->ht_500(newHT) && sel->mht_200(newMHT) && sel->Njet_4(newNJet))printf("Eff: %g Acc: %g njet: %d nbtag: %d ht: %g mht: %g binN: %d \n ",Eff,Acc, newNJet,evt->nBtags(),newHT,newMHT, binMap_ForIso[utils2::findBin_ForIso(newNJet,newHT,newMHT)]);
              if(verbose==2 && sel->ht_500(newHT) && sel->mht_200(newMHT) && sel->Njet_4(newNJet))printf("Eff_Arne: %g \n" ,Eff_Arne);

              if(Acc==0){Acc=0.9;cout << " Warning! Acc==0 \n ";}
              if(Acc_lowDphi==0)Acc_lowDphi=0.9;
              if(Eff==0)Eff=0.75;
              if(Eff_Arne==0){Eff_Arne=0.75;cout << " Warning! Eff_Arne==0 \n ";}

              AccPlus = Acc+AccError;
              AccMinus = Acc-AccError;
              AccPlus_lowDphi = Acc_lowDphi+Acc_lowDphiError;
              AccMinus_lowDphi= Acc_lowDphi-Acc_lowDphiError;
              if(StudyErrorPropag){
                double tempval=hMuIsoSF->GetBinContent(hMuIsoSF->GetXaxis()->FindBin(muPt),hMuIsoSF->GetYaxis()->FindBin(fabs(muEta)));
                double tempvalErr=fabs(hMuIsoSF->GetBinError(hMuIsoSF->GetXaxis()->FindBin(muPt),hMuIsoSF->GetYaxis()->FindBin(fabs(muEta))));
                if(muPt>=120.){ // sometimes pT can be higher than 120. Eta is laways less than 2.1 so we are fine.
                  tempval=hMuIsoSF->GetBinContent(hMuIsoSF->GetXaxis()->FindBin(119.99),hMuIsoSF->GetYaxis()->FindBin(fabs(muEta)));
                  tempvalErr=fabs(hMuIsoSF->GetBinError(hMuIsoSF->GetXaxis()->FindBin(119.99),hMuIsoSF->GetYaxis()->FindBin(fabs(muEta))));
                }
                //printf(" ####\n muPt: %g muEta: %g \n err1: %2.3f err2: %2.3f \n",muPt,muEta,fabs(1.-tempval),pow((pow(tempvalErr,2.)+pow(0.01*tempval,2.)),0.5)); 
                tempval=max( fabs(1.-tempval) , pow((pow(tempvalErr,2.)+pow(0.01*tempval,2.)),0.5) );
                //printf(" err: %2.3f \n",tempval);
                IsoSFUp=1.+tempval;
                IsoSFDw=1.-tempval;
                
                tempval=0.;
                tempvalErr=0.;
                tempval=hMuIdSF->GetBinContent(hMuIdSF->GetXaxis()->FindBin(muPt),hMuIdSF->GetYaxis()->FindBin(fabs(muEta)));
                tempvalErr=hMuIdSF->GetBinError(hMuIdSF->GetXaxis()->FindBin(muPt),hMuIdSF->GetYaxis()->FindBin(fabs(muEta)));
                if(muPt>=120.){
                  tempval=hMuIdSF->GetBinContent(hMuIdSF->GetXaxis()->FindBin(119.99),hMuIdSF->GetYaxis()->FindBin(fabs(muEta)));
                  tempvalErr=hMuIdSF->GetBinError(hMuIdSF->GetXaxis()->FindBin(119.99),hMuIdSF->GetYaxis()->FindBin(fabs(muEta)));
                }
                //printf(" ####\n muPt: %g muEta: %g \n err1: %2.3f err2: %2.3f \n",muPt,muEta,fabs(1.-tempval),pow((pow(tempvalErr,2.)+pow(0.01*tempval,2.)),0.5));
                tempval=max( fabs(1.-tempval) , pow((pow(tempvalErr,2.)+pow(0.01*tempval,2.)),0.5) );
                //printf(" err: %2.3f \n",tempval);
                IdSFUp=1.+tempval;
                IdSFDw=1.-tempval;
                //printf(" muPt: %g muEta: %g IsoSFUp: %g IsoSFDw: %g IdSFUp: %g IdSFDw: %g \n",muPt,muEta,IsoSFUp,IsoSFDw,IdSFUp,IdSFDw);
                
/*
                AccSysPlus = Acc + hAccSysMax->GetBinContent(binMap_ForIso[utils2::findBin_ForIso(newNJet,newHT,newMHT)]);
                AccSysMinus = Acc - hAccSysMin->GetBinContent(binMap_ForIso[utils2::findBin_ForIso(newNJet,newHT,newMHT)]);
                AccSysPlus_lowDphi = Acc_lowDphi + hAccSysMax_lowDphi->GetBinContent(binMap_ForIso[utils2::findBin_ForIso(newNJet,newHT,newMHT)]);
                AccSysMinus_lowDphi = Acc_lowDphi - hAccSysMin_lowDphi->GetBinContent(binMap_ForIso[utils2::findBin_ForIso(newNJet,newHT,newMHT)]);
                ScaleAccSysPlus = Acc + hScaleAccSysMax->GetBinContent(binMap_ForIso[utils2::findBin_ForIso(newNJet,newHT,newMHT)]);
                ScaleAccSysMinus = Acc - hScaleAccSysMin->GetBinContent(binMap_ForIso[utils2::findBin_ForIso(newNJet,newHT,newMHT)]);
                ScaleAccSysPlus_lowDphi = Acc_lowDphi + hScaleAccSysMax_lowDphi->GetBinContent(binMap_ForIso[utils2::findBin_ForIso(newNJet,newHT,newMHT)]);
                ScaleAccSysMinus_lowDphi = Acc_lowDphi - hScaleAccSysMin_lowDphi->GetBinContent(binMap_ForIso[utils2::findBin_ForIso(newNJet,newHT,newMHT)]);
*/
              }
              Eff_ArnePlus = Eff_Arne + (Reco_error_Arne + Iso_error_Arne); 
              Eff_ArneMinus = Eff_Arne - (Reco_error_Arne + Iso_error_Arne);

              // Not all the muons are coming from W. Some of them are coming from Tau which should not be considered in our estimation.
              double Prob_Tau_muError, Prob_Tau_muPlus, Prob_Tau_muMinus, Prob_Tau_muError_lowDelphi, Prob_Tau_muPlus_lowDelphi, Prob_Tau_muMinus_lowDelphi;
              double Prob_Tau_mu = hProb_Tau_mu->GetBinContent(binMap_ForIso[utils2::findBin_ForIso(newNJet,newHT,newMHT)]);
              Prob_Tau_muError = hProb_Tau_mu->GetBinError(binMap_ForIso[utils2::findBin_ForIso(newNJet,newHT,newMHT)]);
              double Prob_Tau_mu_lowDelphi = hProb_Tau_mu_lowDelphi->GetBinContent(binMap_ForIso[utils2::findBin_ForIso(newNJet,newHT,newMHT)]);
              Prob_Tau_muError_lowDelphi = hProb_Tau_mu_lowDelphi->GetBinError(binMap_ForIso[utils2::findBin_ForIso(newNJet,newHT,newMHT)]);
              Prob_Tau_muPlus=Prob_Tau_mu+Prob_Tau_muError;
              Prob_Tau_muMinus=Prob_Tau_mu-Prob_Tau_muError;
              Prob_Tau_muPlus_lowDelphi=Prob_Tau_mu_lowDelphi+Prob_Tau_muError_lowDelphi;
              Prob_Tau_muMinus_lowDelphi=Prob_Tau_mu_lowDelphi-Prob_Tau_muError_lowDelphi;
      //Ahmad33
              if(TauHadModel<4){Acc=1.;Acc_lowDphi=1.; }
      //Ahmad33
              if(TauHadModel<3)Eff_Arne=1.; 
      //Ahmad33
              if(TauHadModel < 1)Prob_Tau_mu=0; 

              double totWeight=( eventWeight )*1*0.64*(1/(Acc*Eff_Arne))*(1-Prob_Tau_mu);//the 0.64 is because only 64% of tau's decay hadronically. Here 0.9 is acceptance and 0.75 is efficiencies of both reconstruction and isolation.
              double totWeight_lowDphi=eventWeight*1*0.64*(1/(Acc_lowDphi*Eff_Arne))*(1-Prob_Tau_mu_lowDelphi);
              // dilepton contamination
              if(TauHadModel>=3){
                if(utils2::IsoTrkModel==0){totWeight*= 1./1.02;totWeight_lowDphi*= 1./1.02;}
              }

              // if fastsim
              vector<double> prob;
              if(fastsim){
                totWeight *= fastsimWeight*0.99; // 0.99 is the jet id efficiency correction. 
                double puWeight = 
		  puhist->GetBinContent(puhist->GetXaxis()->FindBin(min(evt->TrueNumInteractions_(),puhist->GetBinLowEdge(puhist->GetNbinsX()+1))));
		//puhist->GetBinContent(puhist->GetXaxis()->FindBin(min(evt->NVtx_(),(int)puhist->GetBinLowEdge(puhist->GetNbinsX()+1))));
                totWeight*= puWeight ;
                //
                double isrWeight = isrcorr.GetCorrection(evt->genParticles_(),evt->genParticles_PDGid_());
                totWeight*=isrWeight;
                //
                prob = btagcorr.GetCorrections(evt->JetsLorVec_(),evt->Jets_partonFlavor_(),evt->HTJetsMask_());
		//
		if (totWeight>1. && l==1 && m==0) {
		  printf("puWeight=%8.1f, isrWeight=%8.1f, nvtx=%8d, trueNint=%5.1f, Nint=%5d\n",
			 puWeight,isrWeight,evt->NVtx_(),evt->TrueNumInteractions_(),evt->NumInteractions_());
		}
              }

              weightEffAcc = totWeight;

              // if bootstrap is on weigh the events such that 
              // the total number of events remains the same.
              // That means the sum over bootstrapWeights = 1
              // Our templates are made such that area under them = 1

              if(utils2::bootstrap){
                double bootstrapWeight = utils->GetBinContent(muPt,vec_resp,l) * utils->GetBinWidth(muPt,vec_resp,l);
                if(UncerLoop[iuncer]=="templatePlus"){
                  bootstrapWeight = utils->GetBinContent(muPt,vec_respUp,l) * utils->GetBinWidth(muPt,vec_respUp,l);
                }
                else if(UncerLoop[iuncer]=="templateMinus"){
                  bootstrapWeight = utils->GetBinContent(muPt,vec_respDown,l) * utils->GetBinWidth(muPt,vec_respDown,l);
                }
                totWeight*=bootstrapWeight;
                totWeight*=Prob_Btag;
                totWeight_lowDphi*=bootstrapWeight;
                totWeight_lowDphi*=Prob_Btag;
              }


              int binNum_MT = binMap_ForIso[utils2::findBin_ForIso(newNJet,newHT,newMHT).c_str()];
              double mtWeightError, mtWeightPlus, mtWeightMinus, mtWeightError_lowDphi, mtWeightPlus_lowDphi, mtWeightMinus_lowDphi;
              double mtWeight = hMT->GetBinContent(binNum_MT);
              mtWeightError = hMT->GetBinError(binNum_MT);
              double mtWeight_lowDphi = hMT_lowDphi->GetBinContent(binNum_MT);
              mtWeightError_lowDphi = hMT_lowDphi->GetBinError(binNum_MT);


              // Apply MT efficiency
              if(utils2::applyMT){

                if(mtWeight==0){mtWeight=0.9;cout << " Warning! mtWeight==0 \n ";}
                if(mtWeight_lowDphi==0)mtWeight_lowDphi=0.9;

                mtWeightPlus=mtWeight+mtWeightError;
                mtWeightMinus=mtWeight-mtWeightError;
                mtWeightPlus_lowDphi=mtWeight_lowDphi+mtWeightError_lowDphi;
                mtWeightMinus_lowDphi=mtWeight_lowDphi-mtWeightError_lowDphi;

                if(!utils2::CalcMT){totWeight/= mtWeight;totWeight_lowDphi/= mtWeight_lowDphi;weightEffAcc/= mtWeight;}
                else if(eventN < 100000 ) cout<< "warning! MT is not being applied. Turn off CalcMT in utils2\n";

              }

              if (l==1 && m==0){                 // Fill this only once per event l=[1,nLoops] m=[0,1]
                cutflow_preselection->Fill(9.,eventWeight); // All preselection
              }
              cutflow_preselection->Fill(10.,totWeight); // All preselection

              double IsoTrkWeight,IsoTrkWeightError,IsoTrkWeightPlus,IsoTrkWeightMinus, IsoTrkWeight_lowDphi,IsoTrkWeightError_lowDphi,IsoTrkWeightPlus_lowDphi,IsoTrkWeightMinus_lowDphi;
              bool PassIso2=false;
              double searchWeight = totWeight;
              // applyIsoTrk here 
              if(utils2::applyIsoTrk){

                // Determine which Iso model is chosen
                // and apply the neccessary cuts 
                if(utils2::IsoTrkModel==0)PassIso2=true;
                else if(utils2::IsoTrkModel==1){
                  int nIsoElec=0, nIsoMu=0, nIsoPion=0;
                  int IsoElecIdx=-1,IsoMuIdx=-1,IsoPionIdx=-1;
                  for(int k=0;k<evt->IsoElecPtVec_().size();k++){
                    if(utils->findMatchedObject(IsoElecIdx,muEta,muPhi,evt->IsoElecPtVec_(),evt->IsoElecEtaVec_(),evt->IsoElecPhiVec_(),0.1,verbose))
                      continue;
                    nIsoElec++;
                  }
                  for(int k=0;k<evt->IsoMuPtVec_().size();k++){
                    if(utils->findMatchedObject(IsoMuIdx,muEta,muPhi,evt->IsoMuPtVec_(),evt->IsoMuEtaVec_(),evt->IsoMuPhiVec_(),0.1,verbose))
                      continue;
                    nIsoMu++;
                  }
                  for(int k=0;k<evt->IsoPionPtVec_().size();k++){
                    if(utils->findMatchedObject(IsoPionIdx,muEta,muPhi,evt->IsoPionPtVec_(),evt->IsoPionEtaVec_(),evt->IsoPionPhiVec_(),0.1,verbose))
                      continue;
                    nIsoPion++;
                  }
                  if(nIsoElec==0&&nIsoMu==0&&nIsoPion==0)PassIso2=true; 
                }

                int binNum = binMap_ForIso[utils2::findBin_ForIso(newNJet,newHT,newMHT).c_str()];
                if(utils2::IsoTrkModel==0){
                  IsoTrkWeight = hIsoEff->GetBinContent(binNum);
                  IsoTrkWeightError= hIsoEff->GetBinError(binNum);
                  if(IsoTrkWeight==0){IsoTrkWeight=0.6; cout << "Warning! IsoTrkWeight==0 \n ";}
                }
                else if(utils2::IsoTrkModel==1){
                  IsoTrkWeight = hIsoEff2->GetBinContent(binNum);
                  if(IsoTrkWeight==0)IsoTrkWeight=0.95;
                }
                else{
                  cout << "unknown IsoTrkModel \n ";
                  return 2;
                }
                IsoTrkWeightPlus = IsoTrkWeight + IsoTrkWeightError;
                IsoTrkWeightMinus = IsoTrkWeight - IsoTrkWeightError;

                IsoTrkWeight_lowDphi = hIsoEff_lowDphi->GetBinContent(binNum);
                IsoTrkWeightError_lowDphi = hIsoEff_lowDphi->GetBinError(binNum);
                if(IsoTrkWeight_lowDphi==0)IsoTrkWeight_lowDphi=0.6;
                IsoTrkWeightPlus_lowDphi = IsoTrkWeight_lowDphi + IsoTrkWeightError_lowDphi;
                IsoTrkWeightMinus_lowDphi = IsoTrkWeight_lowDphi - IsoTrkWeightError_lowDphi;

                searchWeight = totWeight*IsoTrkWeight;
                weightEffAccForEvt = weightEffAcc*IsoTrkWeight;

              }
              else{  // utils2::applyIsoTrk
                searchWeight = totWeight; 
                PassIso2=true;
              }



              //We would like to know whether in low/delPhi region
              //muon is from non-W sources? This info is especially
              //needed when working out MT efficiency
              bool isMuFromNonW = false;
              if(GenMuIdx<0)isMuFromNonW=true;
              else if(evt->GenMuFromTauVec_()[GenMuIdx]==1)isMuFromNonW=true; 
              bool Pass_MuMomForMT = false;
              if(utils2::CalcMT){
                if(!isMuFromNonW)Pass_MuMomForMT=true;
              }
              else Pass_MuMomForMT=true;
            

              // Apply low delta phi region
              if(sel->ht_500(newHT) && sel->mht_200(newMHT) && (newDphi1<=0.5 || newDphi2<=0.5 || newDphi3<=0.3 || newDphi4<=0.3) && sel->Njet_4(newNJet)  ){
                if(!utils2::bootstrap){
                  // Non W muons calculation
                  if(!isData){
                    hAll_mu_lowDelphi->Fill(binMap_ForIso[utils2::findBin_ForIso(newNJet,newHT,newMHT).c_str()],eventWeight);
                    if(GenMuIdx<0)hNonW_mu_lowDelphi->Fill(binMap_ForIso[utils2::findBin_ForIso(newNJet,newHT,newMHT).c_str()],eventWeight);
                    else if(evt->GenMuFromTauVec_()[GenMuIdx]==1)hNonW_mu_lowDelphi->Fill(binMap_ForIso[utils2::findBin_ForIso(newNJet,newHT,newMHT).c_str()],eventWeight);
                  }
                }
              }


              // Apply baseline cuts
              if(sel->ht_500(newHT) && sel->mht_200(newMHT) && newDphi1>0.5 && newDphi2>0.5 && newDphi3>0.3 && newDphi4>0.3 && sel->Njet_4(newNJet) ){

                if(!utils2::bootstrap){
                  // The followings doesn't make sense if bootstrap is on!
                  IsoElec_all->Fill( binMap[utils2::findBin_NoB(newNJet,newHT,newMHT).c_str()],eventWeight);
                  if(evt->nIsoElec()==0)IsoElec_pass->Fill( binMap[utils2::findBin_NoB(newNJet,newHT,newMHT).c_str()],eventWeight);
                  IsoMu_all->Fill( binMap[utils2::findBin_NoB(newNJet,newHT,newMHT).c_str()],eventWeight);
                  if(evt->nIsoMu()==0)IsoMu_pass->Fill( binMap[utils2::findBin_NoB(newNJet,newHT,newMHT).c_str()],eventWeight);
                  IsoPion_all->Fill( binMap[utils2::findBin_NoB(newNJet,newHT,newMHT).c_str()],eventWeight);
                  if(evt->nIsoPion()==0)IsoPion_pass->Fill( binMap[utils2::findBin_NoB(newNJet,newHT,newMHT).c_str()],eventWeight);
                  
                  // Non W muons calculation
                  if(!isData){ 
                    hAll_mu->Fill(binMap_ForIso[utils2::findBin_ForIso(newNJet,newHT,newMHT).c_str()],eventWeight);
                    if(GenMuIdx<0)hNonW_mu->Fill(binMap_ForIso[utils2::findBin_ForIso(newNJet,newHT,newMHT).c_str()],eventWeight);
                    else if(evt->GenMuFromTauVec_()[GenMuIdx]==1)hNonW_mu->Fill(binMap_ForIso[utils2::findBin_ForIso(newNJet,newHT,newMHT).c_str()],eventWeight);
                  }

                  // calculate trigger efficiency 
                  trig_all->Fill(binMap[utils2::findBin_NoB(newNJet,newHT,newMHT).c_str()],eventWeight);
                  if(trigPass)trig_pass->Fill(binMap[utils2::findBin_NoB(newNJet,newHT,newMHT).c_str()],eventWeight); 
                }



                if(PassIso2){

                  // to see the dileptonic contamination
                  if(!isData && !utils2::bootstrap){
                    dilepton_all->Fill(binMap[utils2::findBin_NoB(newNJet,newHT,newMHT).c_str()],eventWeight);
                    if(evt->GenMuPtVec_().size()>1 || evt->GenElecPtVec_().size()>0)dilepton_pass->Fill(binMap[utils2::findBin_NoB(newNJet,newHT,newMHT).c_str()],eventWeight);
                  }

                  // Fill Search bin histogram 
                  // the if condition will be effective only if 
                  // MTCalc is on and mu is from nonW mom
                  // otherwise it always pass
                  if(Pass_MuMomForMT)searchH_evt->Fill( binMap_ForIso[utils2::findBin_ForIso(newNJet,newHT,newMHT).c_str()],searchWeight);
                  if(fastsim){
                    for(int iii=0;iii< prob.size();iii++){
                      searchH_b_evt->Fill( binMap_b[utils2::findBin(newNJet,NewNB,newHT,newMHT).c_str()],searchWeight*prob[iii]);
                      // Fill QCD histograms
                      QCD_Up_evt->Fill( binMap_QCD[utils2::findBin_QCD(newNJet,NewNB,newHT,newMHT).c_str()],searchWeight*prob[iii]);
                    }
                  }
                  else{

        searchH_b_evt->Fill( binMap_b[utils2::findBin(newNJet,NewNB,newHT,newMHT).c_str()],searchWeight);

        //KH-Feb2016-starts
        double newHT_tmp,newMHT_tmp,newNJet_tmp,NewNB_tmp;
        newHT_tmp=newHT; newMHT_tmp=newMHT; newNJet_tmp=newNJet; NewNB_tmp=NewNB;
                    if (newHT_tmp>1500.) newHT_tmp=1499.;
        if (newMHT_tmp>1200.) newMHT_tmp=1199.;
        if (newNJet_tmp>=13) newNJet_tmp=12;
        if (NewNB_tmp>=4) NewNB_tmp=3.;
        hSearchBinHT_evt->Fill( newHT_tmp , searchWeight);
        hSearchBinMHT_evt->Fill( newMHT_tmp , searchWeight);
        hSearchBinNjets_evt->Fill( newNJet_tmp , searchWeight);
        hSearchBinNb_evt->Fill( NewNB_tmp , searchWeight);
        //KH-Feb2016-ends

        // Fill QCD histograms
        QCD_Up_evt->Fill( binMap_QCD[utils2::findBin_QCD(newNJet,NewNB,newHT,newMHT).c_str()],searchWeight);
                  }
                  if(NewNB==0)hPredHTMHT0b_evt->Fill( binMap_HTMHT[utils2::findBin_HTMHT(newHT,newMHT).c_str()],searchWeight);  
                  if(NewNB >0)hPredHTMHTwb_evt->Fill( binMap_HTMHT[utils2::findBin_HTMHT(newHT,newMHT).c_str()],searchWeight);
                  hPredNJetBins_evt->Fill(newNJet,searchWeight);
                  hPredNbBins_evt->Fill( NewNB,searchWeight);


                  
                  // Fill correlation histograms
                  hCorSearch_evt->Fill(binMap[utils2::findBin_NoB(evt->nJets(),evt->ht(),evt->mht()).c_str()],binMap[utils2::findBin_NoB(newNJet,newHT,newMHT).c_str()],searchWeight);
                  hCorHT_evt->Fill(evt->ht(),newHT,searchWeight);
                  hCorMHT_evt->Fill(evt->mht(),newMHT,searchWeight);
                  hCorNJet_evt->Fill(evt->nJets(),newNJet,searchWeight);
                  hCorNBtag_evt->Fill(evt->nBtags(),NewNB,searchWeight);

                  hCorSearch_b_evt->Fill(binMap_b[utils2::findBin(evt->nJets(),evt->nBtags(),evt->ht(),evt->mht()).c_str()],binMap_b[utils2::findBin(newNJet,NewNB,newHT,newMHT).c_str()],searchWeight);

                  hCorSearch_noW_evt->Fill(binMap[utils2::findBin_NoB(evt->nJets(),evt->ht(),evt->mht()).c_str()],binMap[utils2::findBin_NoB(newNJet,newHT,newMHT).c_str()]);
                  hCorHT_noW_evt->Fill(evt->ht(),newHT);
                  hCorMHT_noW_evt->Fill(evt->mht(),newMHT);
                  hCorNJet_noW_evt->Fill(evt->nJets(),newNJet);
                  hCorNBtag_noW_evt->Fill(evt->nBtags(),NewNB);
                  searchH_b_noWeight_evt->Fill( binMap_b[utils2::findBin(newNJet,NewNB,newHT,newMHT).c_str()]);
                  hCorSearch_b_noW_evt->Fill(binMap_b[utils2::findBin(evt->nJets(),evt->nBtags(),evt->ht(),evt->mht()).c_str()],binMap_b[utils2::findBin(newNJet,NewNB,newHT,newMHT).c_str()]);

                } // passIso2

              }   // baseline cut


              // Fill QCD histogram
              // Fill the histogram in the inverted delta phi region
              if(sel->ht_500(newHT) && sel->mht_200(newMHT) && sel->Njet_4(newNJet) && (newDphi1<=0.5 || newDphi2<=0.5 || newDphi3<=0.3 || newDphi4<=0.3)  ){
                double searchWeight = totWeight/(1-Prob_Tau_mu)*(1-Prob_Tau_mu_lowDelphi)*mtWeight/mtWeight_lowDphi;

                // applyIsoTrk here 
                if(utils2::applyIsoTrk){
                  searchWeight = totWeight/(1-Prob_Tau_mu)*(1-Prob_Tau_mu_lowDelphi)*IsoTrkWeight_lowDphi*mtWeight/mtWeight_lowDphi*Acc/Acc_lowDphi;
                }
                else searchWeight = totWeight/(1-Prob_Tau_mu)*(1-Prob_Tau_mu_lowDelphi)*mtWeight/mtWeight_lowDphi*Acc/Acc_lowDphi;


                if(PassIso2){

                  // to see the dileptonic contamination
                  if(!isData && !utils2::bootstrap){
                    dilepton_all_lowDphi->Fill(binMap[utils2::findBin_NoB(newNJet,newHT,newMHT).c_str()],eventWeight);
                    if(evt->GenMuPtVec_().size()>1 || evt->GenElecPtVec_().size()>0)dilepton_pass_lowDphi->Fill(binMap[utils2::findBin_NoB(newNJet,newHT,newMHT).c_str()],eventWeight);
                  }

                  // Fill Search bin histogram
                  // the if condition will be effective only if
                  // MTCalc is on and mu is from nonW mom
                  // otherwise it always pass
                  if(Pass_MuMomForMT)searchH_evt_lowDphi->Fill( binMap_ForIso[utils2::findBin_ForIso(newNJet,newHT,newMHT).c_str()],searchWeight);
                  // Fill QCD histograms
                  if(fastsim){
                    for(int iii=0;iii< prob.size();iii++){
                      QCD_Low_evt->Fill(binMap_QCD[utils2::findBin_QCD(newNJet,NewNB,newHT,newMHT).c_str()],searchWeight*prob[0]);
                    }
                  }
                  else QCD_Low_evt->Fill( binMap_QCD[utils2::findBin_QCD(newNJet,NewNB,newHT,newMHT).c_str()],searchWeight);
                }

              }


              //load totWeightMap
                // no error propagation
                totWeightMap["allEvents"]=totWeight;
                totWeightMap_lowDphi["allEvents"]=totWeight_lowDphi;
                if(StudyErrorPropag){
                  // b mistag error propagation
                  totWeightMap["BMistagPlus"]=totWeight;
                  totWeightMap["BMistagMinus"]=totWeight;
                  if(utils2::bootstrap){
                    totWeightMap["BMistagPlus"]/=Prob_Btag;// Prob_Btag was multiplied before. This is to cancel it.
                    totWeightMap["BMistagPlus"]*=Prob_Btag_Plus;
                    totWeightMap["BMistagMinus"]/=Prob_Btag;// Prob_Btag was multiplied before. This is to cancel it.
                    totWeightMap["BMistagMinus"]*=Prob_Btag_Minus;
                  }
                  totWeightMap_lowDphi["BMistagPlus"]=totWeight_lowDphi;
                  totWeightMap_lowDphi["BMistagMinus"]=totWeight_lowDphi;
                  if(utils2::bootstrap){
                    totWeightMap_lowDphi["BMistagPlus"]/=Prob_Btag;// Prob_Btag was multiplied before. This is to cancel it.
                    totWeightMap_lowDphi["BMistagPlus"]*=Prob_Btag_Plus;
                    totWeightMap_lowDphi["BMistagMinus"]/=Prob_Btag;// Prob_Btag was multiplied before. This is to cancel it.
                    totWeightMap_lowDphi["BMistagMinus"]*=Prob_Btag_Minus;
                  }
                  // Acc 
                  totWeightMap["AccPlus"]=totWeight*Acc/AccPlus;
                  totWeightMap["AccMinus"]=totWeight*Acc/AccMinus;
                  totWeightMap_lowDphi["AccPlus"]=totWeight_lowDphi*Acc_lowDphi/AccPlus_lowDphi;
                  totWeightMap_lowDphi["AccMinus"]=totWeight_lowDphi*Acc_lowDphi/AccMinus_lowDphi;         
                  // Acc Systematics
                  totWeightMap["AccSysPlus"]=totWeight*Acc/AccSysPlus;
                  totWeightMap["AccSysMinus"]=totWeight*Acc/AccSysMinus;
                  totWeightMap_lowDphi["AccSysPlus"]=totWeight_lowDphi*Acc_lowDphi/AccSysPlus_lowDphi;
                  totWeightMap_lowDphi["AccSysMinus"]=totWeight_lowDphi*Acc_lowDphi/AccSysMinus_lowDphi; 
                  totWeightMap["ScaleAccSysPlus"]=totWeight*Acc/ScaleAccSysPlus;
                  totWeightMap["ScaleAccSysMinus"]=totWeight*Acc/ScaleAccSysMinus;
                  totWeightMap_lowDphi["ScaleAccSysPlus"]=totWeight_lowDphi*Acc_lowDphi/ScaleAccSysPlus_lowDphi;
                  totWeightMap_lowDphi["ScaleAccSysMinus"]=totWeight_lowDphi*Acc_lowDphi/ScaleAccSysMinus_lowDphi;
                  // Iso
                  totWeightMap["IsoPlus"]=totWeight;  
                  totWeightMap["IsoMinus"]=totWeight;
                  totWeightMap_lowDphi["IsoPlus"]=totWeight_lowDphi;
                  totWeightMap_lowDphi["IsoMinus"]=totWeight_lowDphi;
                  // MT
                  totWeightMap["MTPlus"]=totWeight*mtWeight/mtWeightPlus;
                  totWeightMap["MTMinus"]=totWeight*mtWeight/mtWeightMinus;
                  totWeightMap_lowDphi["MTPlus"]=totWeight_lowDphi*mtWeight_lowDphi/mtWeightPlus_lowDphi;
                  totWeightMap_lowDphi["MTMinus"]=totWeight_lowDphi*mtWeight_lowDphi/mtWeightMinus_lowDphi;
                  // MuFromTau
                  totWeightMap["MuFromTauPlus"]=totWeight/(1-Prob_Tau_mu)*(1-Prob_Tau_muPlus);
                  totWeightMap["MuFromTauMinus"]=totWeight/(1-Prob_Tau_mu)*(1-Prob_Tau_muMinus);
                  totWeightMap_lowDphi["MuFromTauPlus"]=totWeight_lowDphi/(1-Prob_Tau_mu_lowDelphi)*(1-Prob_Tau_muPlus_lowDelphi);
                  totWeightMap_lowDphi["MuFromTauMinus"]=totWeight_lowDphi/(1-Prob_Tau_mu_lowDelphi)*(1-Prob_Tau_muMinus_lowDelphi);
                  // Iso & Reco effieciency (From Arne)
                  totWeightMap["MuRecoIsoPlus"]=totWeight*Eff_Arne/Eff_ArnePlus;
                  totWeightMap["MuRecoIsoMinus"]=totWeight*Eff_Arne/Eff_ArneMinus;
                  totWeightMap_lowDphi["MuRecoIsoPlus"]=totWeight_lowDphi*Eff_Arne/Eff_ArnePlus;
                  totWeightMap_lowDphi["MuRecoIsoMinus"]=totWeight_lowDphi*Eff_Arne/Eff_ArneMinus;
                  // Mistag Rate
                  totWeightMap["BMistag_statPlus"]=totWeight;
                  totWeightMap["BMistag_statMinus"]=totWeight;
                  if(utils2::bootstrap){
                    totWeightMap["BMistagPlus"]/=Prob_Btag;// Prob_Btag was multiplied before. This is to cancel it.
                    totWeightMap["BMistagPlus"]*=Prob_Btag_Plus_stat;
                    totWeightMap["BMistagMinus"]/=Prob_Btag;// Prob_Btag was multiplied before. This is to cancel it.
                    totWeightMap["BMistagMinus"]*=Prob_Btag_Minus_stat;
                  }
                  totWeightMap_lowDphi["BMistag_statPlus"]=totWeight_lowDphi;
                  totWeightMap_lowDphi["BMistag_statMinus"]=totWeight_lowDphi;
                  if(utils2::bootstrap){
                    totWeightMap_lowDphi["BMistag_statPlus"]/=Prob_Btag;// Prob_Btag was multiplied before. This is to cancel it.
                    totWeightMap_lowDphi["BMistag_statPlus"]*=Prob_Btag_Plus_stat;
                    totWeightMap_lowDphi["BMistag_statMinus"]/=Prob_Btag;// Prob_Btag was multiplied before. This is to cancel it.
                    totWeightMap_lowDphi["BMistag_statMinus"]*=Prob_Btag_Minus_stat;
                  }
                  // Tau branching ratio
                  totWeightMap["Tau_BrRatio_Plus"]=totWeight/0.64*0.65;
                  totWeightMap["Tau_BrRatio_Minus"]=totWeight/0.64*0.63;   
                  totWeightMap_lowDphi["Tau_BrRatio_Plus"]=totWeight_lowDphi/0.64*0.65;
                  totWeightMap_lowDphi["Tau_BrRatio_Minus"]=totWeight_lowDphi/0.64*0.63;
                  // Dileptonic
                  if(utils2::IsoTrkModel==0){
                    totWeightMap["DileptonPlus"]=  totWeight* 1.02/1.04;
                    totWeightMap["DileptonMinus"]=  totWeight* 1.02/1.0;
                    totWeightMap_lowDphi["DileptonPlus"]=totWeight_lowDphi* 1.02/1.04;
                    totWeightMap_lowDphi["DileptonMinus"]=totWeight_lowDphi* 1.02/1.0;
                  }
                  else{
                    totWeightMap["DileptonPlus"]=totWeight;
                    totWeightMap["DileptonMinus"]=totWeight;
                    totWeightMap_lowDphi["DileptonPlus"]=totWeight_lowDphi;
                    totWeightMap_lowDphi["DileptonMinus"]=totWeight_lowDphi;
                  }
                  // Reco & Iso systetmatics
                  totWeightMap["RecoSysPlus"]=totWeight/IdSFUp;// this is Eff_Arne/1.1*Eff_Arne
                  totWeightMap["RecoSysMinus"]=totWeight/IdSFDw;
                  totWeightMap_lowDphi["RecoSysPlus"]=totWeight_lowDphi/IdSFUp;
                  totWeightMap_lowDphi["RecoSysMinus"]=totWeight_lowDphi/IdSFDw;
                  totWeightMap["IsoSysPlus"]=totWeight/IsoSFUp;// this is Eff_Arne/1.1*Eff_Arne
                  totWeightMap["IsoSysMinus"]=totWeight/IsoSFDw;
                  totWeightMap_lowDphi["IsoSysPlus"]=totWeight_lowDphi/IsoSFUp;
                  totWeightMap_lowDphi["IsoSysMinus"]=totWeight_lowDphi/IsoSFDw;


                }


              //build and array that contains the quantities we need a histogram for. Here order is important and must be the same as RA2nocutvec
              double eveinfvec[] = {totWeight, 1. , newHT, newHT, evt->ht(), newMHT,newMHT, evt->mht()
                                   ,newMet, evt->met(), mindpn,newDphi1, newDphi2, newDphi3, newDphi4
                                   ,(double) newNJet, (double)NewNB, muPt
                                   ,muEta, muPhi, simTauJetPt_xy,simTauJetPhi_ForPlotting,tauJet_mht_dlePhi_forPlotting};

              bool pass0=false;
              if(TauHadModel >= 1)pass0=true;
              else if(MuFromTauVec[0]==0)pass0=true; // Ahmad33
              if(pass0){

                //loop over all the different backgrounds: "allEvents", "Wlv", "Zvv"
                for(map<string, map<string , vector<TH1D> > >::iterator itt=map_map_evt.begin(); itt!=map_map_evt.end();itt++){//this will be terminated after the cuts
                  eveinfvec[0] = totWeightMap[itt->first];
                  ////determine what type of background should pass
                  if(true){//we can apply extra selection for each eventType here

                    //Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts

                    //////loop over cut names and fill the histograms
                    for(map<string , vector<TH1D> >::iterator ite=cut_histvec_map.begin(); ite!=cut_histvec_map.end();ite++){
                
                      //To save time we only write few directories when calcing not main events
                      //if(subSampleKey.find("template")!=string::npos && (ite->first!="delphi" && ite->first!="low_Dphi" ))continue; 

                      //To save cpu we neglect some of early selections
                      if(ite->first=="PreSel" || ite->first=="nolep" || ite->first=="ht_500" 
                          || ite->first=="Njet_4" || ite->first=="isoElec" || ite->first=="isoMu" || ite->first=="nolep"
                        )continue;

                      // Apply IsoTrkVeto after PreSel, nolep, Njet_4, ht_500 and mht_200
                      if(ite->first!="PreSel" && ite->first!="nolep"&&ite->first!="ht_500"&&ite->first!="mht_200"&&ite->first!="Njet_4" 
                         && ite->first!="mht_500"&& ite->first!="delphi_NoIso" && ite->first!="J46_HT5001200_MHT500750" && utils2::applyIsoTrk){            
                        if(!PassIso2)continue;

                        if(itt->first=="IsoPlus") eveinfvec[0] = totWeightMap[itt->first]*IsoTrkWeightPlus;
                        else if(itt->first=="IsoMinus") eveinfvec[0] = totWeightMap[itt->first]*IsoTrkWeightMinus;
                        else eveinfvec[0] = totWeightMap[itt->first]*IsoTrkWeight; 
             
                      } 
                      else eveinfvec[0] = totWeightMap[itt->first];

                      if(ite->first=="low_Dphi"){
                        if(utils2::applyIsoTrk){
                          if(itt->first=="IsoPlus")eveinfvec[0] = totWeightMap_lowDphi[itt->first]*IsoTrkWeightPlus_lowDphi;
                          else if(itt->first=="IsoMinus")eveinfvec[0] = totWeightMap_lowDphi[itt->first]*IsoTrkWeightMinus_lowDphi;
                          else eveinfvec[0] = totWeightMap_lowDphi[itt->first]*IsoTrkWeight_lowDphi;
                        }
                        else eveinfvec[0] = totWeightMap_lowDphi[itt->first];
                      }

                      if(ite->first=="isoPion"){
                        if(utils2::applyIsoTrk){
                          eveinfvec[0] = totWeightMap[itt->first]*IsoTrkWeight;
                          if(newDphi1<=0.5 || newDphi2<=0.5 || newDphi3<=0.3 || newDphi4<=0.3)eveinfvec[0] = totWeightMap_lowDphi[itt->first]*IsoTrkWeight_lowDphi;
                        }
                        else{
                          eveinfvec[0] = totWeightMap[itt->first];
                          if(newDphi1<=0.5 || newDphi2<=0.5 || newDphi3<=0.3 || newDphi4<=0.3)eveinfvec[0] = totWeightMap_lowDphi[itt->first];
                        }
                      }
                      if(ite->first=="mht_200" || ite->first=="mht_500" || ite->first=="J46_HT5001200_MHT500750"){
                        eveinfvec[0] = totWeightMap[itt->first];
                        if(newDphi1<=0.5 || newDphi2<=0.5 || newDphi3<=0.3 || newDphi4<=0.3)eveinfvec[0] = totWeightMap_lowDphi[itt->first];
                      }

                      if(sel->checkcut_HadTau(ite->first,newHT,newMHT,newDphi1,newDphi2,newDphi3,newDphi4,newNJet,NewNB,evt->nLeptons(),evt->nIsoElec(),evt->nIsoMu(),evt->nIsoPion())==true){

                        histobjmap[ite->first].fill(Nhists,&eveinfvec[0] ,&itt->second[ite->first][0]);

                        map_map_evt_search[itt->first][ite->first][0].Fill(binMap[utils2::findBin_NoB(newNJet,newHT,newMHT).c_str()],eveinfvec[0]);//searchH_
                        map_map_evt_search[itt->first][ite->first][1].Fill( binMap_b[utils2::findBin(newNJet,NewNB,newHT,newMHT).c_str()],eveinfvec[0]);//searchH_b_
                        map_map_evt_search[itt->first][ite->first][2].Fill( binMap_QCD[utils2::findBin_QCD(newNJet,NewNB,newHT,newMHT).c_str()],eveinfvec[0]);  // QCD_
                        
                        
                      }

                    }//end of loop over cut names

                    ////EndOfCuts//EndOfCuts//EndOfCuts//EndOfCuts//EndOfCuts//EndOfCuts//EndOfCuts//EndOfCuts//EndOfCuts//EndOfCuts
                    
                  }//end of bg_type determination
                }//end of loop over all the different backgrounds: "allEvents", "Wlv", "Zvv"
              } //Ahmad33
            } // end of Btag bootstrap
          } // end of bootstrapping

        } // End if exactly one muon

        
        // Studying weights
        for (int ibin=0; ibin<searchH_b_evt->GetNbinsX()+2; ibin++){
          if (searchH_b_evt->GetBinContent(ibin)>0.){
            double fullWeight = searchH_b_evt->GetBinContent(ibin);
            hWeightForSearchBin->Fill(weightEffAccForEvt,ibin,fullWeight);
            hWeight2ForSearchBin->Fill(weightEffAccForEvt,ibin);
            hWeight3ForSearchBin->Fill(fullWeight,ibin,fullWeight);
            hWeight4ForSearchBin->Fill(fullWeight,ibin);
            prWeightForSearchBin->Fill(ibin,weightEffAccForEvt,fullWeight);
            prWeight2ForSearchBin->Fill(ibin,weightEffAccForEvt);
            prWeight3ForSearchBin->Fill(ibin,fullWeight,fullWeight);
            prWeight4ForSearchBin->Fill(ibin,fullWeight);
          }
        }

  //KH-Feb2016-starts - add histograms for correlation studies
  //These lines have to come before HistogramFillForEventTH1,
  //because HistogramFillForEventTH1 resets XXXX_evt histograms
        bootstrapUtils::HistogramFillForEventCorrelation(hSearchBinCorr, searchH_b_evt);
        bootstrapUtils::HistogramFillForEventCorrelation(hSearchBinCorrHT, hSearchBinHT_evt);
        bootstrapUtils::HistogramFillForEventCorrelation(hSearchBinCorrMHT, hSearchBinMHT_evt);
        bootstrapUtils::HistogramFillForEventCorrelation(hSearchBinCorrNjets, hSearchBinNjets_evt);
        bootstrapUtils::HistogramFillForEventCorrelation(hSearchBinCorrNb, hSearchBinNb_evt);
  //KH-Feb2016-ends

        // Correct the uncertainties
        bootstrapUtils::HistogramFillForEventTH1(searchH, searchH_evt);
        bootstrapUtils::HistogramFillForEventTH1(searchH_lowDphi, searchH_evt_lowDphi);
        bootstrapUtils::HistogramFillForEventTH1(QCD_Up, QCD_Up_evt);
        bootstrapUtils::HistogramFillForEventTH1(QCD_Low, QCD_Low_evt);
        bootstrapUtils::HistogramFillForEventTH1(searchH_b, searchH_b_noWeight, searchH_b_evt, searchH_b_noWeight_evt);
        bootstrapUtils::HistogramFillForEventTH1(hPredHTMHT0b, hPredHTMHT0b_evt);
        bootstrapUtils::HistogramFillForEventTH1(hPredHTMHTwb, hPredHTMHTwb_evt);
        bootstrapUtils::HistogramFillForEventTH1(hPredNJetBins, hPredNJetBins_evt);
        bootstrapUtils::HistogramFillForEventTH1(hPredNbBins, hPredNbBins_evt);

  //KH-Feb2016-starts - add new histograms
        bootstrapUtils::HistogramFillForEventTH1(hSearchBinHT, hSearchBinHT_evt);
        bootstrapUtils::HistogramFillForEventTH1(hSearchBinMHT, hSearchBinMHT_evt);
        bootstrapUtils::HistogramFillForEventTH1(hSearchBinNjets, hSearchBinNjets_evt);
        bootstrapUtils::HistogramFillForEventTH1(hSearchBinNb, hSearchBinNb_evt);
  //KH-Feb2016-ends

  // for correlation studies
        bootstrapUtils::HistogramFillForEventTH2(hCorSearch_b, hCorSearch_b_noW, hCorSearch_b_evt, hCorSearch_b_noW_evt);
        bootstrapUtils::HistogramFillForEventTH2(hCorHT,    hCorHT_noW,    hCorHT_evt,    hCorHT_noW_evt);
        bootstrapUtils::HistogramFillForEventTH2(hCorMHT,   hCorMHT_noW,   hCorMHT_evt,   hCorMHT_noW_evt);
        bootstrapUtils::HistogramFillForEventTH2(hCorNJet,  hCorNJet_noW,  hCorNJet_evt,  hCorNJet_noW_evt);
        bootstrapUtils::HistogramFillForEventTH2(hCorNBtag, hCorNBtag_noW, hCorNBtag_evt, hCorNBtag_noW_evt);

        // Correct the uncertainties
        // Loop over differnt event types
        for(map<string, map<string , vector<TH1D> > >::iterator itt=map_map.begin(); itt!=map_map.end();itt++){
          // Loop over different cuts
          for(map<string , vector<TH1D> >::iterator it=itt->second.begin(); it!=itt->second.end();it++){
            //correct search hists first
            int nHist_search = map_map_search[itt->first][it->first].size();
            // Loop over different search histograms
            for(int ii=0; ii<nHist_search; ii++){
              bootstrapUtils::HistogramFillForEventTH1(&map_map_search[itt->first][it->first][ii],&map_map_evt_search[itt->first][it->first][ii]);
            }
            int nHist = it->second.size();
            // Loop over different histograms
            for(int ii=0; ii<nHist; ii++){

              //KH--- for MaxWeight hisotgrams --- can be improved... starts
              //std::cout << it->first << " " << itt->first << std::endl;
              if (it->first=="delphi" && itt->first=="allEvents"){
                if (isData){
                if (ii==1){ // HT
                  if (map_map_evt[itt->first][it->first][ii].GetSumOfWeights()>0.25){
                    printf("run,event: %10d,%10d, Njet,Nbtag,HT,MHT: %8d,%8d,%10.1f,%10.1f, muon pt,eta,phi=%6.1f,%6.1f,%6.1f\n",
                           evt->Runnum(),evt->Evtnum(),evt->nJets(),evt->nBtags(),evt->ht(),evt->mht(),
                           muPt,muEta,muPhi);
                  }
                }
                }
                if (ii==1){ // HT
                  //map_map_evt[itt->first][it->first][ii].Print();
                  for (int ibin=0;ibin<map_map_evt[itt->first][it->first][ii].GetNbinsX()+2;ibin++) {
                    if (hMaxWeight_HT->GetBinContent(ibin)<map_map_evt[itt->first][it->first][ii].GetBinContent(ibin)) 
                      hMaxWeight_HT->SetBinContent(ibin,map_map_evt[itt->first][it->first][ii].GetBinContent(ibin));
                  }
                }
                if (ii==2){ // HT2
                  //map_map_evt[itt->first][it->first][ii].Print();
                  for (int ibin=0;ibin<map_map_evt[itt->first][it->first][ii].GetNbinsX()+2;ibin++) {
                    if (hMaxWeight_HT2->GetBinContent(ibin)<map_map_evt[itt->first][it->first][ii].GetBinContent(ibin)) 
                      hMaxWeight_HT2->SetBinContent(ibin,map_map_evt[itt->first][it->first][ii].GetBinContent(ibin));
                  }
                }
                if (ii==3){ // MHT
                  //map_map_evt[itt->first][it->first][ii].Print();
                  for (int ibin=0;ibin<map_map_evt[itt->first][it->first][ii].GetNbinsX()+2;ibin++) {
                    if (hMaxWeight_MHT->GetBinContent(ibin)<map_map_evt[itt->first][it->first][ii].GetBinContent(ibin)) 
                      hMaxWeight_MHT->SetBinContent(ibin,map_map_evt[itt->first][it->first][ii].GetBinContent(ibin));
                  }
                }
                if (ii==4){ //MHT2
                  //map_map_evt[itt->first][it->first][ii].Print();
                  for (int ibin=0;ibin<map_map_evt[itt->first][it->first][ii].GetNbinsX()+2;ibin++) {
                    if (hMaxWeight_MHT2->GetBinContent(ibin)<map_map_evt[itt->first][it->first][ii].GetBinContent(ibin)) 
                      hMaxWeight_MHT2->SetBinContent(ibin,map_map_evt[itt->first][it->first][ii].GetBinContent(ibin));
                  }
                }
                if (ii==10){ // NJet
                  //map_map_evt[itt->first][it->first][ii].Print();
                  for (int ibin=0;ibin<map_map_evt[itt->first][it->first][ii].GetNbinsX()+2;ibin++) {
                    if (hMaxWeight_NJet->GetBinContent(ibin)<map_map_evt[itt->first][it->first][ii].GetBinContent(ibin)) 
                      hMaxWeight_NJet->SetBinContent(ibin,map_map_evt[itt->first][it->first][ii].GetBinContent(ibin));
                  }
                }
                if (ii==11){ // NBtag
                  //map_map_evt[itt->first][it->first][ii].Print();
                  for (int ibin=0;ibin<map_map_evt[itt->first][it->first][ii].GetNbinsX()+2;ibin++) {
                    if (hMaxWeight_NBtag->GetBinContent(ibin)<map_map_evt[itt->first][it->first][ii].GetBinContent(ibin)) 
                      hMaxWeight_NBtag->SetBinContent(ibin,map_map_evt[itt->first][it->first][ii].GetBinContent(ibin));
                  }
                }
              }
              //KH--ends

              bootstrapUtils::HistogramFillForEventTH1( &it->second[ii] , &map_map_evt[itt->first][it->first][ii] );
              
            } // nHist
          }   // 
        }     // map_map

      } // loop for uncertainties
    } // end of loop over events


    double GenRecMu_rate = (double)GenRecMu_fail /((double)GenRecMu_all);
    printf("GenRecMu_all: %d GenRecMu_fail: %d fail rate: %g \n ",GenRecMu_all,GenRecMu_fail,GenRecMu_rate);
      

    if(!utils2::bootstrap){

      // Calculate tau mistagged(btagged) rate
      TH1D * TauBtaggedRate = static_cast<TH1D*>(B_rate_tagged->Clone("TauBtaggedRate"));
      TauBtaggedRate->Divide(B_rate_tagged,B_rate_all,1,1,"B");
      // Write the histogram 
      sprintf(tempname,"%s/TauBtaggedRate_%s_%s.root",Outdir.c_str(),subSampleKey.c_str(),inputnumber.c_str());
      TFile btagfile(tempname,"RECREATE");
      TauBtaggedRate->Write();
      B_rate_tagged->Write();
      B_rate_all->Write();
      btagfile.Close();

      // Compute iso efficiencies
      TH1* IsoElecEff = static_cast<TH1*>(IsoElec_pass->Clone("IsoElecEff"));
      IsoElecEff->Divide(IsoElec_pass,IsoElec_all,1,1,"B");
      TH1* IsoMuEff = static_cast<TH1*>(IsoMu_pass->Clone("IsoMuEff"));
      IsoMuEff->Divide(IsoMu_pass,IsoMu_all,1,1,"B");
      TH1* IsoPionEff = static_cast<TH1*>(IsoPion_pass->Clone("IsoPionEff"));
      IsoPionEff->Divide(IsoPion_pass,IsoPion_all,1,1,"B");
      sprintf(tempname,"%s/IsoEfficiencies_%s_%s.root",Outdir.c_str(),subSampleKey.c_str(),inputnumber.c_str());
      TFile outFile3(tempname,"RECREATE");
      IsoElecEff->Write();
      IsoElec_pass->Write();
      IsoElec_all->Write();
      IsoMuEff->Write();
      IsoMu_pass->Write();
      IsoMu_all->Write();
      IsoPionEff->Write();
      IsoPion_pass->Write();
      IsoPion_all->Write();
      outFile3.Close();

      // Calculate dilepton contamination
      TH1* dilepton_rate = static_cast<TH1*>(dilepton_pass->Clone("dilepton_rate"));
      dilepton_rate->Divide(dilepton_pass,dilepton_all,1,1,"B");
      TH1* dilepton_rate_lowDphi = static_cast<TH1*>(dilepton_pass_lowDphi->Clone("dilepton_rate_lowDphi"));
      dilepton_rate_lowDphi->Divide(dilepton_pass_lowDphi,dilepton_all_lowDphi,1,1,"B");
      sprintf(tempname,"%s/DileptonRate_%s_%s.root",Outdir.c_str(),subSampleKey.c_str(),inputnumber.c_str());
      TFile DiFile(tempname,"RECREATE");
      dilepton_rate->Write();
      dilepton_all->Write();
      dilepton_pass->Write();
      dilepton_rate_lowDphi->Write();
      dilepton_all_lowDphi->Write();
      dilepton_pass_lowDphi->Write();
      DiFile.Close();
    

      // Calculate probability of finding non-W muons
      TH1* hNonWMuProb = static_cast<TH1*>(hNonW_mu->Clone("hProb_Tau_mu"));
      hNonWMuProb->Divide(hNonW_mu,hAll_mu,1,1,"B");
      TH1* hNonWMuProb_lowDelphi = static_cast<TH1*>(hNonW_mu_lowDelphi->Clone("hProb_Tau_mu_lowDelphi"));
      hNonWMuProb_lowDelphi->Divide(hNonW_mu_lowDelphi,hAll_mu_lowDelphi,1,1,"B");
      sprintf(tempname,"%s/Probability_Tau_mu_%s_%s.root",Outdir.c_str(),subSampleKey.c_str(),inputnumber.c_str());
      TFile muProbFile(tempname,"RECREATE");     
      hNonWMuProb->Write();
      hAll_mu->Write();
      hNonW_mu->Write();
      hNonWMuProb_lowDelphi->Write();
      hAll_mu_lowDelphi->Write();
      hNonW_mu_lowDelphi->Write();
      muProbFile.Close();

      // Calculate trigger efficiency 
      TH1* trigEff = static_cast<TH1*>(trig_pass->Clone("trigEff"));
      trigEff->Divide(trig_pass,trig_all,1,1,"B");
      sprintf(tempname,"%s/TriggerEff_%s_%s.root",Outdir.c_str(),subSampleKey.c_str(),inputnumber.c_str());
      TFile trigFile(tempname,"RECREATE");
      trigEff->Write();
      trig_all->Write();
      trig_pass->Write();
      trigFile.Close();
      
    }

    // calculate muon_jet match failure and write the histograms
    TH1D * MuJet_rate = static_cast<TH1D*>(MuJet_fail->Clone("MuJet_rate"));
    MuJet_rate->Divide(MuJet_fail,MuJet_all,1,1,"B");
    //Write
    sprintf(tempname,"%s/MuJetMatchRate_%s_%s.root",Outdir.c_str(),subSampleKey.c_str(),inputnumber.c_str());
    TFile MuJetfile(tempname,"RECREATE");
    MuJet_rate->Write();
    MuJet_fail->Write();
    MuJet_all->Write();
    MuJetfile.Close();

    // open a file to write the histograms
    sprintf(tempname,"%s/HadTauEstimation_%s_%s.root",Outdir.c_str(),subSampleKey.c_str(),inputnumber.c_str());
    TFile *resFile = new TFile(tempname, "RECREATE");
    muMtWHist->Write();
    cutflow_preselection->Write();
    searchH->Write();
    searchH_lowDphi->Write();
    QCD_Up->Write();
    QCD_Low->Write();
    searchH_b->Write();
    searchH_b_noWeight->Write();
    hPredHTMHT0b->Write();
    hPredHTMHTwb->Write();
    hPredNJetBins->Write();
    hPredNbBins->Write();
    hCorSearch->Write();
    hCorSearch_b->Write();
    hCorHT->Write();
    hCorMHT->Write();
    hCorNJet->Write();
    hCorNBtag->Write();
    hCorSearch_noW->Write();
    hCorSearch_b_noW->Write();
    hCorHT_noW->Write();
    hCorMHT_noW->Write();
    hCorNJet_noW->Write();
    hCorNBtag_noW->Write();
    hWeightForSearchBin->Write(); 
    hWeight2ForSearchBin->Write(); 
    hWeight3ForSearchBin->Write(); 
    hWeight4ForSearchBin->Write(); 
    prWeightForSearchBin->Write(); 
    prWeight2ForSearchBin->Write(); 
    prWeight3ForSearchBin->Write(); 
    prWeight4ForSearchBin->Write(); 
    hMaxWeight_HT->Write();
    hMaxWeight_HT2->Write();
    hMaxWeight_MHT->Write();
    hMaxWeight_MHT2->Write();
    hMaxWeight_NJet->Write();
    hMaxWeight_NBtag->Write();

    //KH-Feb2016-starts
    hSearchBinHT->Write();
    hSearchBinMHT->Write();
    hSearchBinNjets->Write();
    hSearchBinNb->Write();
    hSearchBinCorr->Write();
    hSearchBinCorrHT->Write();
    hSearchBinCorrMHT->Write();
    hSearchBinCorrNjets->Write();
    hSearchBinCorrNb->Write();
    //KH-Feb2016-ends

    TDirectory *cdtoitt;
    TDirectory *cdtoit;
    // Loop over different event categories (e.g. "All events, Wlnu, Zll, Zvv, etc")
    for(int iet=0;iet<(int)eventType.size();iet++){
      for(map<string, map<string , vector<TH1D> > >::iterator itt=map_map.begin(); itt!=map_map.end();itt++){
        if (eventType[iet]==itt->first){
          //KH
          ////std::cout << (itt->first).c_str() << std::endl;
          cdtoitt = resFile->mkdir((itt->first).c_str());
          cdtoitt->cd();
          for(int i=0; i< (int)sel->cutName().size();i++){
            for(map<string , vector<TH1D> >::iterator it=itt->second.begin(); it!=itt->second.end();it++){
              //To save time we only write few directories when calcing not main events
              //if(subSampleKey.find("template")!=string::npos && (it->first!="delphi" && it->first!="low_Dphi" ))continue;

              //To save cpu we neglect some of early selections
              if(it->first=="PreSel" || it->first=="nolep" || it->first=="ht_500" 
                 || it->first=="Njet_4" || it->first=="isoElec" || it->first=="isoMu" || it->first=="nolep"
                )continue;

              if (sel->cutName()[i]==it->first){
                cdtoit = cdtoitt->mkdir((it->first).c_str());
                cdtoit->cd();
                int nHist = it->second.size();
                for(int ii=0; ii<nHist; ii++){//since we only have 4 type of histograms
                  sprintf(tempname,"%s_%s_%s",it->second[ii].GetName(),(it->first).c_str(),(itt->first).c_str());
                  it->second[ii].Write(tempname);
                }
                int nHist_search = map_map_search[itt->first][it->first].size();
                for(int ii=0; ii<nHist_search; ii++){
                  sprintf(tempname,"%s_%s_%s",map_map_search[itt->first][it->first][ii].GetName(),(it->first).c_str(),(itt->first).c_str());
                  map_map_search[itt->first][it->first][ii].Write();
                }
                cdtoitt->cd();
              }
            }
          }
        }
      }
    }

    delete resp_file;

  } // end of main

  double DeltaT(unsigned int i, vector<TVector3> Jet3vec ){

      double deltaT=0;
      float jres=0.1;
      double sum=0;
      double Jpx_i= Jet3vec[i].Pt()*cos(Jet3vec[i].Phi());
      double Jpy_i= Jet3vec[i].Pt()*sin(Jet3vec[i].Phi());

      for(unsigned int j=0; j< Jet3vec.size(); ++j){
          if(j==i)continue;
          double Jpx_j= Jet3vec[j].Pt()*cos(Jet3vec[j].Phi());
          double Jpy_j= Jet3vec[j].Pt()*sin(Jet3vec[j].Phi());

          sum=sum+(Jpx_i*Jpy_j-Jpx_j*Jpy_i) * (Jpx_i*Jpy_j-Jpx_j*Jpy_i);
      }
      deltaT=jres*sqrt(sum)/Jet3vec[i].Pt();

      return deltaT;
  }

  

