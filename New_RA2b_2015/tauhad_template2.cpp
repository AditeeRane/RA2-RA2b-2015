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

#include "TTreeCache.h"
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
#include "TGraphAsymmErrors.h"
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
    char tempname2[200];
    char tempname3[200];
    char tempname4[200];
    char tempname5[200];
    char histname[200];
    char prefix[200];
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

    Double_t ht_bins[15] = { 0., 100.,200.,300.,400.,500.,600.,700.,800.,900.,
			     1000.,1200.,1500.,2000.,5000.};
    Double_t mht_bins[13] = {0., 50.,100.,150.,200.,250.,300.,350.,400.,500.,
                             700.,1000.,5000.};

    sprintf(prefix,"root://cmseos.fnal.gov/");
    //sprintf(prefix,"/data3/");

    
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
    cutflow_preselection->GetXaxis()->SetBinLabel(3,"HBHE(Iso)NoiseFilter");
    cutflow_preselection->GetXaxis()->SetBinLabel(4,"eeBadScFilter");
    cutflow_preselection->GetXaxis()->SetBinLabel(5,"CSCTightHalo/EcalDeadCellTriggerPrimitive");   
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

    // Introduce the bins for Acc
    map<string,int> binMap_ForAcc = utils2::BinMap_ForAcc();
    int totNbins_ForAcc=binMap_ForAcc.size();

    // Introduce search bin histogram
    map<string,int> binMap = utils2::BinMap_NoB();
    int totNbins=binMap.size();
    TH1* searchH = new TH1D("searchH","search bin histogram",totNbins,1,totNbins+1);
    TH1* searchH_nb_njet2 = new TH1D("searchH_nb_njet2","search bin histogram _nb_njet2",4,-0.5,3.5);
    TH1* searchH_nb_njet34 = new TH1D("searchH_nb_njet34","search bin histogram _nb_njet34",4,-0.5,3.5);
    TH1* searchH_nb_njet56 = new TH1D("searchH_nb_njet56","search bin histogram _nb_njet56",4,-0.5,3.5);
    TH1* searchH_nb_njet78 = new TH1D("searchH_nb_njet78","search bin histogram _nb_njet78",4,-0.5,3.5);
    TH1* searchH_nb_njet9 = new TH1D("searchH_nb_njet9","search bin histogram _nb_njet9",4,-0.5,3.5);


    searchH->Sumw2();
    searchH_nb_njet2->Sumw2();
    searchH_nb_njet34->Sumw2();
    searchH_nb_njet56->Sumw2();
    searchH_nb_njet78->Sumw2();    
    searchH_nb_njet9->Sumw2();

    TH1* MuonCS_Njets= new TH1D("MuonCS_Njets","Njet distribution for muon CS event",10,2,12);
    TH1* MuonCS_Nbjets= new TH1D("MuonCS_Nbjets","Nbjet distribution for muon CS event",5,0,5);
    TH1* MuonCS_HT= new TH1D("MuonCS_HT","HT distribution for muon CS event",20,0,2500);
    TH1* MuonCS_MHT= new TH1D("MuonCS_MHT","MHT distribution for muon CS event",20,0,1000);
    TH1* MuonCS_MuonPt= new TH1D("MuonCS_MuonPt","MuonPt distribution for muon CS event",20,0,400);
    TH1* MuonCS_MuonEta= new TH1D("MuonCS_MuonEta","MuonEta distribution for muon CS event",32,-3.2,3.2);
    TH1* MuonCS_MuonPhi= new TH1D("MuonCS_MuonPhi","MuonPhi distribution for muon CS event",32,-3.2,3.2);
    TH1* Denom_MuonPt= new TH1D("Denom_MuonPt","MuonPt distribution for events passing MET triggers",40,0,400);
    TH1* Num_MuonPt= new TH1D("Num_MuonPt","MuonPt distribution for events passing MET+Mu24 triggers",40,0,400);
    TH1* NumMuonsPerEvt= new TH1D("NumMuonsPerEvt","Number of muons per evt",5,0,5);
    TH1* JetOne_Pt= new TH1D("JetOne_Pt","Pt distribution for leading JetOne",20,0,400);
    TH1* JetOne_Eta= new TH1D("JetOne_Eta","Eta distribution for leading JetOne",32,-3.2,3.2);
    TH1* JetOne_Phi= new TH1D("JetOne_Phi","Phi distribution for leading JetOne",32,-3.2,3.2);
    TH1* JetTwo_Pt= new TH1D("JetTwo_Pt","Pt distribution for leading JetTwo",20,0,400);
    TH1* JetTwo_Eta= new TH1D("JetTwo_Eta","Eta distribution for leading JetTwo",32,-3.2,3.2);
    TH1* JetTwo_Phi= new TH1D("JetTwo_Phi","Phi distribution for leading JetTwo",32,-3.2,3.2);
    TH1* JetThree_Pt= new TH1D("JetThree_Pt","Pt distribution for leading JetThree",20,0,400);
    TH1* JetThree_Eta= new TH1D("JetThree_Eta","Eta distribution for leading JetThree",32,-3.2,3.2);
    TH1* JetThree_Phi= new TH1D("JetThree_Phi","Phi distribution for leading JetThree",32,-3.2,3.2);

    MuonCS_Njets->Sumw2();
    MuonCS_Nbjets->Sumw2();
    MuonCS_HT->Sumw2();
    MuonCS_MHT->Sumw2();
    MuonCS_MuonPt->Sumw2();
    MuonCS_MuonEta->Sumw2();
    MuonCS_MuonPhi->Sumw2();
    JetOne_Pt->Sumw2();
    JetOne_Eta->Sumw2();
    JetOne_Phi->Sumw2();
    JetTwo_Pt->Sumw2();
    JetTwo_Eta->Sumw2();
    JetTwo_Phi->Sumw2();
    JetThree_Pt->Sumw2();
    JetThree_Eta->Sumw2();
    JetThree_Phi->Sumw2();
    Num_MuonPt->Sumw2();
    Denom_MuonPt->Sumw2();
    NumMuonsPerEvt->Sumw2();
// Make another hist to be filled during bootstrapping
    TH1 * searchH_evt = static_cast<TH1D*>(searchH->Clone("searchH_evt")); 
    TH1* searchH_lowDphi = new TH1D("searchH_lowDphi","search bin histogram",totNbins,1,totNbins+1);
    TH1* searchH_nb_njet2_lowDphi = new TH1D("searchH_nb_njet2_lowDphi","search bin histogram _nb_njet2",4,-0.5,3.5);
    TH1* searchH_nb_njet34_lowDphi = new TH1D("searchH_nb_njet34_lowDphi","search bin histogram _nb_njet34",4,-0.5,3.5);
    TH1* searchH_nb_njet56_lowDphi = new TH1D("searchH_nb_njet56_lowDphi","search bin histogram _nb_njet56",4,-0.5,3.5);
    TH1* searchH_nb_njet78_lowDphi = new TH1D("searchH_nb_njet78_lowDphi","search bin histogram _nb_njet78_lowDphi",4,-0.5,3.5);
    TH1* searchH_nb_njet9_lowDphi = new TH1D("searchH_nb_njet9_lowDphi","search bin histogram _nb_njet9",4,-0.5,3.5);


    searchH_lowDphi->Sumw2();
    searchH_nb_njet2_lowDphi->Sumw2();
    searchH_nb_njet34_lowDphi->Sumw2();
    searchH_nb_njet56_lowDphi->Sumw2();
    searchH_nb_njet78_lowDphi->Sumw2();    
    searchH_nb_njet9_lowDphi->Sumw2();

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
    //map<string,string> binMap_b = utils2::BinMap();
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

    TH1* MuPt_NJ34 = new TH1D("MuPt_NJ34","MuPtDistribution_HTMHT=[350,500]",150,0.,150.);
    TH1* MuEta_NJ34 = new TH1D("MuEta_NJ34","MuEtaDistribution_HTMHT=[350,500]",50,-2.5,2.5);
    TH1* MuPhi_NJ34 = new TH1D("MuPhi_NJ34","MuPhiDistribution_HTMHT=[350,500]",80,-4.,4.);

    TH1* MuPt_NJ56 = new TH1D("MuPt_NJ56","MuPtDistribution_HTMHT=[350,500]",150,0.,150.);
    TH1* MuEta_NJ56 = new TH1D("MuEta_NJ56","MuEtaDistribution_HTMHT=[350,500]",50,-2.5,2.5);
    TH1* MuPhi_NJ56 = new TH1D("MuPhi_NJ56","MuPhiDistribution_HTMHT=[350,500]",80,-4.,4.);

    // *AR, Mar3,2017-Objective is to check impact of PU reweighting on efficiencies.
    TFile * pileupFile = new TFile("Inputs/PileupHistograms_0121_69p2mb_pm4p6.root","R");
    TH1D * puhist = (TH1D*)pileupFile->Get("pu_weights_down");


    // The tau response templates
    Utils * utils = new Utils();

    bool fastsim=false;
    TFile *IsrFile,*skimfile;
    TH1 *h_isr, *h_genpt, *h_njetsisr; 
    ISRCorrector isrcorr;
    BTagCorrector btagcorr;
    if(subSampleKey.find("fast")!=string::npos){
      cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n fastsim Monte Carlo \n "; 
      fastsim=true;
      //signalPileUp = new TFile("Inputs/PileupHistograms_0121_69p2mb_pm4p6.root","R");
      //puhist=(TH1*)signalPileUp->Get("pu_weights_down");

      IsrFile = new TFile("Inputs/ISRWeights.root","R");
      h_isr = (TH1*)IsrFile->Get("isr_weights_central");

      sample_AUX = new TChain("tree");      
      vector<string> skimInput = utils->skimInput(subSampleKey); 
      std::cout << skimInput.size() << std::endl;
      std::cout << " " << skimInput[0].c_str() << " " << skimInput[1].c_str() << " " << skimInput[2].c_str() << " " << skimInput[3].c_str()  << " " << skimInput[4].c_str() << std::endl;
      if(skimInput.size()!=5){
        cout<<"Something is wrong with the naming of the skim file.\nUse an input like T1bbbb_mMother-1500_mLSP-800_fast \n";
        return 2;
      }
      //
      sprintf(tempname,
      "%s/store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV12/scan/tree_SLm/tree_%s_%s_%s_fast.root",
      prefix,skimInput[1].c_str(),skimInput[2].c_str(),skimInput[3].c_str());
      //
      skimfile = TFile::Open(tempname,"R");
      if(!skimfile->IsOpen()){
        cout << " \n\n first attempt to find the skim file failed. Trying to find it ... \n\n";
        sprintf(tempname,
        "%s/store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV12/scan/tree_SLm/tree_%s_%s_%s_fast.root",
	prefix,skimInput[1].c_str(),skimInput[2].c_str(),skimInput[3].c_str());
      }
      skimfile = TFile::Open(tempname,"R");
      if(!skimfile->IsOpen()){cout << "skim file is not open \n " ;return 2;} 
      else cout << " skimfile: " << tempname << endl;
      h_njetsisr = (TH1*)skimfile->Get("NJetsISR");
      isrcorr.SetWeights(h_isr,h_njetsisr);
      //
    }//end of fastsim

    ///read the file names from the .txt files and load them to a vector.
    while(fin.getline(filenames, 500) ){filesVec.push_back(filenames);}
    cout<< "\nProcessing " << subSampleKey << " ... " << endl;

    for(unsigned int in=0; in<filesVec.size(); in++){ 
      sample_AUX->Add(filesVec.at(in).c_str()); 
      //cout<<" filename "<< filesVec.at(in).c_str()<<std::endl;
    }
    //sample_AUX->SetCacheSize(20*1024*1024);
    //TTreeCache::SetLearnEntries(1);

    // Interface to the event content
    Events * evt = new Events(sample_AUX, subSampleKey,verbose);
    
    double fastsimWeight =1.0;
    if(subSampleKey.find("fast")!=string::npos){
      //f(filesVec.size()!=1){cout << " 1 skim file only \n"; return 2;}
      //
      btagcorr.SetEffs(skimfile);
      btagcorr.SetCalib("btag/CSVv2_ichep.csv");
      btagcorr.SetFastSim(true);
      //btagcorr.SetDebug(true);
      btagcorr.SetCalibFastSim("btag/CSV_13TEV_Combined_14_7_2016.csv");

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
      //std::cout << "SampleXS" << SampleXS << std::endl;
      SampleXS = 1.; // Let's use the weight (XS) stored in ntuples
      fastsimWeight = (3000. * SampleXS)/TotNEve;
      //fastsimWeight = (3000. * SampleXS)/TotNEve;
      printf(" Luminosity 3000/pb TotNEve: %d fastsimWeight: %g \n",TotNEve,fastsimWeight);
    }
    /*  
  else if (!evt->DataBool_()){  // fullsim
 
      std::cout << subSampleKey << std::endl;
      string skimName;
      if      (subSampleKey=="TTJets_T_SingleLep")    skimName = "tree_TTJets_SingleLeptFromT.root";
      else if (subSampleKey=="TTJets_Tbar_SingleLep") skimName = "tree_TTJets_SingleLeptFromTbar.root";
      else if (subSampleKey=="TTJets_Inclusive")      skimName = "tree_TTJets.root";
      else if (subSampleKey=="TTJets_DiLept")         skimName = "tree_TTJets_DiLept.root";
      else if (subSampleKey=="TTJets_HT_600_800")     skimName = "tree_TTJets_HT_600_800.root";
      else if (subSampleKey=="TTJets_HT_800_1200")    skimName = "tree_TTJets_HT_800_1200.root";
      else if (subSampleKey=="TTJets_HT_1200_2500")   skimName = "tree_TTJets_HT_1200_2500.root";
      else if (subSampleKey=="TTJets_HT_2500_Inf")    skimName = "tree_TTJets_HT_2500_Inf.root";
      else                                            skimName = "tree_TTJets_SingleLeptFromT.root"; // avoid crash
      sprintf(tempname,
	      "%s/store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV10/tree_SLm/%s",
	      prefix,skimName.c_str());
      skimfile = TFile::Open(tempname,"R");
      if(!skimfile->IsOpen()){cout << "skim file is not open \n " ;return 2;} 
      else cout << " skimfile: " << tempname << endl;
      btagcorr.SetEffs(skimfile);
      btagcorr.SetCalib("btag/CSVv2_ichep.csv");      
    }
*/

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

    eventType[0]="allEvents";
    if(StudyErrorPropag){
      
      //////////////////////////
      eventType[1]="RecoSysPlus";   // Muon Reco/ID SF uncertainty
      eventType[2]="RecoSysMinus";  //
      eventType[3]="IsoSysPlus";    // Muon Iso SF uncertainty
      eventType[4]="IsoSysMinus";   //

      //      eventType[5]="BMistagPlus";
      // eventType[6]="BMistagMinus";
      //eventType[7]="MuRecoIsoPlus";
      //eventType[8]="MuRecoIsoMinus";
 
/*
      eventType[1]="BMistagPlus";   // Bmistag efficiency
      eventType[2]="BMistagMinus";  //
      eventType[7]="MuRecoIsoPlus"; // Muon MC-derived efficiency stat uncertainty
      eventType[8]="MuRecoIsoMinus";//
      eventType[9]="AccSysPlus";    // Acc uncertainty due to PDF
      eventType[10]="AccSysMinus";  //
      eventType[11]="ScaleAccSysPlus";  // Acc uncertanty due to scale
      eventType[12]="ScaleAccSysMinus"; //
*/

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
    TFile * bRateFile = new TFile("Inputs/ARElog115_TauBtaggedRate_WJet_stacked.root","R");
    //*AR,Oct14,2016-Instead of getting b mistag rate of tau directly we will take the difference between tau and mu mistag rates from WJet for reassignment of Nb bins
    //    TFile * bRateFile = new TFile("Inputs/hist_bRateDiff_TauVsMu_08Dec.root","R");
    //TFile * bRateFile = new TFile("Inputs/hist_AvebRateDiff_TauVsMu.root","R");
    cout << " \n\n\n\n\n WJet mistag rate is being applied \n\n\n \n\n\n " ;

    sprintf(histname,"TauBtaggedRate");
    //sprintf(histname,"hAveDiff");
    //sprintf(histname,"hDiff");
    TH1D * bRateHist = (TH1D * ) bRateFile->Get(histname)->Clone();

    // Probability of muon coming from Tau
    TFile * Prob_Tau_mu_file = new TFile("Inputs/ARElog116_Probability_Tau_mu_stacked.root","R");
    sprintf(histname,"hProb_Tau_mu");
    TH1D * hProb_Tau_mu =(TH1D *) Prob_Tau_mu_file->Get(histname)->Clone();
    sprintf(histname,"hProb_Tau_mu_nb_njet2");
    TH1D * hProb_Tau_mu_nb_njet2 =(TH1D *) Prob_Tau_mu_file->Get(histname)->Clone();
    sprintf(histname,"hProb_Tau_mu_nb_njet34");
    TH1D * hProb_Tau_mu_nb_njet34 =(TH1D *) Prob_Tau_mu_file->Get(histname)->Clone();
    sprintf(histname,"hProb_Tau_mu_nb_njet56");
    TH1D * hProb_Tau_mu_nb_njet56 =(TH1D *) Prob_Tau_mu_file->Get(histname)->Clone();
    sprintf(histname,"hProb_Tau_mu_nb_njet78");
    TH1D * hProb_Tau_mu_nb_njet78 =(TH1D *) Prob_Tau_mu_file->Get(histname)->Clone();
    sprintf(histname,"hProb_Tau_mu_nb_njet9");
    TH1D * hProb_Tau_mu_nb_njet9 =(TH1D *) Prob_Tau_mu_file->Get(histname)->Clone();
    
    sprintf(histname,"hProb_Tau_mu_lowDelphi");
    TH1D * hProb_Tau_mu_lowDelphi =(TH1D *) Prob_Tau_mu_file->Get(histname)->Clone();
    sprintf(histname,"hProb_Tau_mu_nb_njet2_lowDelphi");
    TH1D * hProb_Tau_mu_nb_njet2_lowDelphi =(TH1D *) Prob_Tau_mu_file->Get(histname)->Clone();
    sprintf(histname,"hProb_Tau_mu_nb_njet34_lowDelphi");
    TH1D * hProb_Tau_mu_nb_njet34_lowDelphi =(TH1D *) Prob_Tau_mu_file->Get(histname)->Clone();
    sprintf(histname,"hProb_Tau_mu_nb_njet56_lowDelphi");
    TH1D * hProb_Tau_mu_nb_njet56_lowDelphi =(TH1D *) Prob_Tau_mu_file->Get(histname)->Clone();
    sprintf(histname,"hProb_Tau_mu_nb_njet78_lowDelphi");
    TH1D * hProb_Tau_mu_nb_njet78_lowDelphi =(TH1D *) Prob_Tau_mu_file->Get(histname)->Clone();
    sprintf(histname,"hProb_Tau_mu_nb_njet9_lowDelphi");
    TH1D * hProb_Tau_mu_nb_njet9_lowDelphi =(TH1D *) Prob_Tau_mu_file->Get(histname)->Clone();


    // Acceptance and efficiencies
    //    TFile * MuAcc_file = new TFile("Inputs/ARElog63_LostLepton2_MuonEfficienciesFromstacked.root","R");
    TFile * MuAcc_file = new TFile("Inputs/ARElog115_LostLepton2_MuonEfficienciesFromstacked.root","R");
    sprintf(histname,"hAcc");
    TH1D * hAcc =(TH1D *) MuAcc_file->Get(histname)->Clone();
    
    sprintf(histname,"hAcc_nb_njet2");
    TH1D * hAcc_NbNjet2 =(TH1D *) MuAcc_file->Get(histname)->Clone();
    sprintf(histname,"hAcc_nb_njet34");
    TH1D * hAcc_NbNjet34 =(TH1D *) MuAcc_file->Get(histname)->Clone();
    sprintf(histname,"hAcc_nb_njet56");
    TH1D * hAcc_NbNjet56 =(TH1D *) MuAcc_file->Get(histname)->Clone();
    sprintf(histname,"hAcc_nb_njet78");
    TH1D * hAcc_NbNjet78 =(TH1D *) MuAcc_file->Get(histname)->Clone();
    sprintf(histname,"hAcc_nb_njet9");
    TH1D * hAcc_NbNjet9 =(TH1D *) MuAcc_file->Get(histname)->Clone();

    //    TH1D * hAcc_0b =(TH1D *) MuAcc_file->Get("hAcc_0b_")->Clone();
    //    TH1D * hAcc_non0b =(TH1D *) MuAcc_file->Get("hAcc_non0b_")->Clone();
    TH1D * hAcc_lowDphi =(TH1D *) MuAcc_file->Get("hAcc_lowDphi")->Clone();
    sprintf(histname,"hAcc_nb_njet2_lowDphi");
    TH1D * hAcc_NbNjet2_lowDphi =(TH1D *) MuAcc_file->Get(histname)->Clone();
    sprintf(histname,"hAcc_nb_njet34_lowDphi");
    TH1D * hAcc_NbNjet34_lowDphi =(TH1D *) MuAcc_file->Get(histname)->Clone();
    sprintf(histname,"hAcc_nb_njet56_lowDphi");
    TH1D * hAcc_NbNjet56_lowDphi =(TH1D *) MuAcc_file->Get(histname)->Clone();
    sprintf(histname,"hAcc_nb_njet78_lowDphi");
    TH1D * hAcc_NbNjet78_lowDphi =(TH1D *) MuAcc_file->Get(histname)->Clone();
    sprintf(histname,"hAcc_nb_njet9_lowDphi");
    TH1D * hAcc_NbNjet9_lowDphi =(TH1D *) MuAcc_file->Get(histname)->Clone();
  
    //
    // Reco and ISO MC efficiencies
    TFile * MuIsoEff_Arne = new TFile("Inputs/Efficiencies_Simon_v12.root","R");
    TH2F *hMuRecoPTEta_Arne = (TH2F*)MuIsoEff_Arne->Get("Efficiencies/MuRecoPTEta/MuRecoPTEta");
    TH2F *hMuIsoPTActivity_Arne = (TH2F*)MuIsoEff_Arne->Get("Efficiencies/MuIsoActivityPT/MuIsoActivityPT");

    // Data/MC scale factors
    //*AR,Feb21,2017-Moved to Moriond scale factor
    TFile * MediumID_SF = new TFile("Inputs/TnP_NUM_MediumID_DENOM_generalTracks_VAR_map_pt_eta.root","R");
    TH2F *hMuIDSF = (TH2F*)MediumID_SF->Get("SF");

    TFile * GenTrack_SF = new TFile("Inputs/Tracking_EfficienciesAndSF_BCDEFGH.root","R");
    TGraphAsymmErrors *hMuTrkHighPtSF = (TGraphAsymmErrors*)GenTrack_SF->Get("ratio_eff_eta3_dr030e030_corr");
    TGraphAsymmErrors *hMuTrkLowPtSF = (TGraphAsymmErrors*)GenTrack_SF->Get("ratio_eff_eta3_tk0_dr030e030_corr");

    TFile * MediumIso_SF = new TFile("Inputs/TnP_NUM_MiniIsoTight_DENOM_MediumID_VAR_map_pt_eta.root","R");
    TH2F *hMuIsoSF = (TH2F*)MediumIso_SF->Get("SF");

    // Get IsoTrk (veto) efficiencies
    TFile * IsoEffFile = new TFile("Inputs/ARElog115_IsoEfficiencies_stacked.root","R");
    //    TFile * IsoEffFile = new TFile("Inputs/KHElog420_modifiedIsoEfficiencies_stacked.root","R");
    TH1D * hIsoEff =(TH1D *) IsoEffFile->Get("IsoEff")->Clone();
    TH1D * hIsoEff_lowDphi =(TH1D *) IsoEffFile->Get("IsoEff_lowDphi")->Clone();

    TH1D * hIsoEff_NbNjet2 =(TH1D *) IsoEffFile->Get("IsoEff_NbNjet2")->Clone();
    TH1D * hIsoEff_NbNjet2_lowDphi =(TH1D *) IsoEffFile->Get("IsoEff_NbNjet2_lowDphi")->Clone();

    TH1D * hIsoEff_NbNjet34 =(TH1D *) IsoEffFile->Get("IsoEff_NbNjet34")->Clone();
    TH1D * hIsoEff_NbNjet34_lowDphi =(TH1D *) IsoEffFile->Get("IsoEff_NbNjet34_lowDphi")->Clone();
    
    TH1D * hIsoEff_NbNjet56 =(TH1D *) IsoEffFile->Get("IsoEff_NbNjet56")->Clone();
    TH1D * hIsoEff_NbNjet56_lowDphi =(TH1D *) IsoEffFile->Get("IsoEff_NbNjet56_lowDphi")->Clone();
    
    TH1D * hIsoEff_NbNjet78 =(TH1D *) IsoEffFile->Get("IsoEff_NbNjet78")->Clone();
    TH1D * hIsoEff_NbNjet78_lowDphi =(TH1D *) IsoEffFile->Get("IsoEff_NbNjet78_lowDphi")->Clone();
    
    TH1D * hIsoEff_NbNjet9 =(TH1D *) IsoEffFile->Get("IsoEff_NbNjet9")->Clone();
    TH1D * hIsoEff_NbNjet9_lowDphi =(TH1D *) IsoEffFile->Get("IsoEff_NbNjet9_lowDphi")->Clone();
    TFile * IsoEffFile2 = new TFile("Inputs/IsoEfficiencies_TTbar_Elog271.root","R");
    TH1D * hIsoEff2 =(TH1D *) IsoEffFile2->Get("IsoEff2")->Clone();    

    // Get MT efficiency that is calculated here in this code
    //TFile * MtFile = new TFile("Inputs/Elog401_MtEff.root","R");
    //std::cout<<" MTFile is read "<<std::endl;
    //    TFile * MtFile = new TFile("Inputs/Elog433_MtEff.root","R");

    TFile * MtFile = new TFile("Inputs/ARElog116_MtEff.root","R");
    TH1D * hMT = (TH1D *) MtFile->Get("MtCutEff")->Clone();
    //TH1D * hMT_lowDphi = (TH1D *) MtFile->Get("MtCutEff_lowDphi")->Clone();
    TH1D * hMT_lowDphi = (TH1D *) MtFile->Get("MtCutEff_lowDphi")->Clone();


    // Inroduce two histogram to understand the probability of a muon coming from tau.
    // and also those reco mu that does not match a gen mu
    TH1D * hAll_mu = new TH1D("hAll_mu","mu from W -- search bin",totNbins_ForIso,1,totNbins_ForIso+1);
    TH1* hAll_mu_nb_njet2 = new TH1D("hAll_mu_nb_njet2","mu from W -- search bin nb_njet2",4,-0.5,3.5);
    TH1* hAll_mu_nb_njet34 = new TH1D("hAll_mu_nb_njet34","mu from W -- search bin nb_njet34",4,-0.5,3.5);
    TH1* hAll_mu_nb_njet56 = new TH1D("hAll_mu_nb_njet56","mu from W -- search bin nb_njet56",4,-0.5,3.5);
    TH1* hAll_mu_nb_njet78 = new TH1D("hAll_mu_nb_njet78","mu from W -- search bin nb_njet78",4,-0.5,3.5);
    TH1* hAll_mu_nb_njet9 = new TH1D("hAll_mu_nb_njet9","mu from W -- search bin nb_njet9",4,-0.5,3.5);
  
    hAll_mu->Sumw2();
    hAll_mu_nb_njet2->Sumw2();
    hAll_mu_nb_njet34->Sumw2();
    hAll_mu_nb_njet56->Sumw2();
    hAll_mu_nb_njet78->Sumw2();
    hAll_mu_nb_njet9->Sumw2();

    TH1D * hNonW_mu = new TH1D("hNonW_mu","mu from Tau -- search bin",totNbins_ForIso,1,totNbins_ForIso+1);
    TH1* hNonW_mu_nb_njet2 = new TH1D("hNonW_mu_nb_njet2","mu from Tau -- search bin nb_njet2",4,-0.5,3.5);
    TH1* hNonW_mu_nb_njet34 = new TH1D("hNonW_mu_nb_njet34","mu from Tau -- search bin nb_njet34",4,-0.5,3.5);
    TH1* hNonW_mu_nb_njet56 = new TH1D("hNonW_mu_nb_njet56","mu from Tau -- search bin nb_njet56",4,-0.5,3.5);
    TH1* hNonW_mu_nb_njet78 = new TH1D("hNonW_mu_nb_njet78","mu from Tau -- search bin nb_njet78",4,-0.5,3.5);
    TH1* hNonW_mu_nb_njet9 = new TH1D("hNonW_mu_nb_njet9","mu from Tau -- search bin nb_njet9",4,-0.5,3.5);

    hNonW_mu->Sumw2();
    hNonW_mu_nb_njet2->Sumw2();
    hNonW_mu_nb_njet34->Sumw2();
    hNonW_mu_nb_njet56->Sumw2();
    hNonW_mu_nb_njet78->Sumw2();
    hNonW_mu_nb_njet9->Sumw2();

    TH1D * hAll_mu_lowDelphi = new TH1D("hAll_mu_lowDelphi","mu from W -- search bin",totNbins_ForIso,1,totNbins_ForIso+1);
    TH1* hAll_mu_nb_njet2_lowDelphi = new TH1D("hAll_mu_nb_njet2_lowDelphi","mu from W -- search bin nb_njet2",4,-0.5,3.5);
    TH1* hAll_mu_nb_njet34_lowDelphi = new TH1D("hAll_mu_nb_njet34_lowDelphi","mu from W -- search bin nb_njet34",4,-0.5,3.5);
    TH1* hAll_mu_nb_njet56_lowDelphi = new TH1D("hAll_mu_nb_njet56_lowDelphi","mu from W -- search bin nb_njet56",4,-0.5,3.5);
    TH1* hAll_mu_nb_njet78_lowDelphi = new TH1D("hAll_mu_nb_njet78_lowDelphi","mu from W -- search bin nb_njet78",4,-0.5,3.5);
    TH1* hAll_mu_nb_njet9_lowDelphi = new TH1D("hAll_mu_nb_njet9_lowDelphi","mu from W -- search bin nb_njet9",4,-0.5,3.5);

    hAll_mu_lowDelphi->Sumw2();
    hAll_mu_nb_njet2_lowDelphi->Sumw2();
    hAll_mu_nb_njet34_lowDelphi->Sumw2();
    hAll_mu_nb_njet56_lowDelphi->Sumw2();
    hAll_mu_nb_njet78_lowDelphi->Sumw2();
    hAll_mu_nb_njet9_lowDelphi->Sumw2();

    TH1D * hNonW_mu_lowDelphi = new TH1D("hNonW_mu_lowDelphi","mu from Tau -- search bin",totNbins_ForIso,1,totNbins_ForIso+1);
    TH1* hNonW_mu_nb_njet2_lowDelphi = new TH1D("hNonW_mu_nb_njet2_lowDelphi","mu from Tau -- search bin nb_njet2",4,-0.5,3.5);
    TH1* hNonW_mu_nb_njet34_lowDelphi = new TH1D("hNonW_mu_nb_njet34_lowDelphi","mu from Tau -- search bin nb_njet34",4,-0.5,3.5);
    TH1* hNonW_mu_nb_njet56_lowDelphi = new TH1D("hNonW_mu_nb_njet56_lowDelphi","mu from Tau -- search bin nb_njet56",4,-0.5,3.5);
    TH1* hNonW_mu_nb_njet78_lowDelphi = new TH1D("hNonW_mu_nb_njet78_lowDelphi","mu from Tau -- search bin nb_njet78",4,-0.5,3.5);
    TH1* hNonW_mu_nb_njet9_lowDelphi = new TH1D("hNonW_mu_nb_njet9_lowDelphi","mu from Tau -- search bin nb_njet9",4,-0.5,3.5);

    hNonW_mu_lowDelphi->Sumw2();
    hNonW_mu_nb_njet2_lowDelphi->Sumw2();
    hNonW_mu_nb_njet34_lowDelphi->Sumw2();
    hNonW_mu_nb_njet56_lowDelphi->Sumw2();
    hNonW_mu_nb_njet78_lowDelphi->Sumw2();
    hNonW_mu_nb_njet9_lowDelphi->Sumw2();

    // calculate the trigger efficiency 
    TH1D * trig_all = new TH1D("trig_all"," trigger all -- search bin",totNbins,1,totNbins+1);
    trig_all->Sumw2();
    TH1D * trig_pass = new TH1D("trig_pass"," trigger pass -- search bin",totNbins,1,totNbins+1);
    trig_pass->Sumw2();
    //*AR,Oct4,2016-Using taugun templates instead of MC templates
    TFile * resp_file_taugun = new TFile("Inputs/hist_taugun_aditee.root","R");
    // Use Ahmad's tau template
    TFile * resp_file_temp = new TFile("Inputs/Elog371_HadTau_TauResponseTemplates_stacked.root","R");
    TFile * resp_file = new TFile("Inputs/Elog433_HadTau_TauResponseTemplates_stacked.root","R");
    TFile * resp_file_PhiVsEta = new TFile("Inputs/ARElog90_HadTau_TauResponseTemplates_stacked.root","R");
    
    for(int i=0; i<TauResponse_nBins; i++){
      sprintf(histname,"hTauResp_%d",i);
      //    vec_resp.push_back( (TH1D*) resp_file->Get( histname )->Clone() );
      vec_resp.push_back( (TH1D*) resp_file_taugun->Get( histname )->Clone() );
      if(subSampleKey.find("template")!=string::npos){
        sprintf(histname,"hTauResp_%d_Up",i);
        vec_respUp.push_back( (TH1D*) resp_file_taugun->Get( histname )->Clone() );
        sprintf(histname,"hTauResp_%d_Down",i);
        vec_respDown.push_back( (TH1D*) resp_file_taugun->Get( histname )->Clone() );
      }
      sprintf(histname,"hTauResp_%d_xy",i);
      vec_resp_xy.push_back( (TH2D*) resp_file->Get( histname )->Clone() );

    }

    //TH2D * h2tau_phi = (TH2D*) resp_file_temp->Get("tau_GenJetPhi")->Clone();
    //*AR,Oct12,2016-Using Wgun template to get dPhi distribution
    TH2D * h2tau_phi = (TH2D*) resp_file_taugun->Get("tau_GenJetPhi")->Clone();
    //TH2D * h2tau_phivseta = (TH2D*) resp_file_PhiVsEta->Get("tau_GenJetPhiVsEta")->Clone();

    // Use Rishi's tau template 
    TFile * resp_file_Rishi = new TFile("Inputs/template_singletaugun_match04_74x_v02.root","R");
    for(int i=0; i<TauResponse_nBins; i++){
      sprintf(histname,"hTauResp_%d",i);
    //  vec_resp.push_back( (TH1D*) resp_file_Rishi->Get( histname )->Clone() );
    }

    // Use Aditee's muon pt cut correction
    TFile *fileMuonPtMinCorr  = new TFile("Inputs/ARElog116_Ratio_HadTauEstimation_stacked_MinMuPt.root","R");
    TH1D * histMuonPtMinCorr = (TH1D*) fileMuonPtMinCorr->Get("searchH_b")->Clone();

    // muMtW Histogram
    TH1D * muMtWHist = new TH1D("muMtW"," MT distribution of muon",40,0,200);

    // Two histograms to find rate of btagged(mistagged) tau jets as a function of pT. 
    TH1D * B_rate_all = new TH1D("B_rate_all","Pt of all matched tau jets",utils->NMuPtBins(),0,utils->MaxMuPt());
    B_rate_all->Sumw2();
    TH1D * B_rate_tagged = new TH1D("B_rate_tagged","Pt of bTagged tau jets",utils->NMuPtBins(),0,utils->MaxMuPt());
    B_rate_tagged->Sumw2();

    TH1D * B_Mu_rate_all = new TH1D("B_Mu_rate_all","Pt of all matched muon jets",utils->NMuPtBins(),0,utils->MaxMuPt());
    B_Mu_rate_all->Sumw2();
    TH1D * B_Mu_rate_tagged = new TH1D("B_Mu_rate_tagged","Pt of bTagged muon jets",utils->NMuPtBins(),0,utils->MaxMuPt());
    B_Mu_rate_tagged->Sumw2();

    // see how often gen mu doesn't match reco mu
    int GenRecMu_all=0,GenRecMu_fail=0;

    // see how often there are two leptons in the the event
    int nCleanEve=0,nClean_BadCharged=0,nClean_BadPFMuon=0,nClean_globalTight=0,nClean_PFCaloMET=0,nClean_noMuonJet=0,nClean_noFakeJet=0,nClean_JetId=0;
    int nPassOR=0,nPass_HBHEIsoNoise=0,nPass_HBHENoise=0,nPass_eeBadSc=0,nPass_EcalDeadCell=0,nPass_BadCharged=0,nPass_BadPFMuon=0,nPass_globalTight=0,nPass_PFCaloMET=0,nPass_noMuonJet=0,nPass_noFakeJet=0,nPass_JetId=0;

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
    int trigIndex=-1;
    double eventWeight = 1.0;
    int eventN=0;
    int FailTrigMuons=0;
    int leadJ1Idx=-99,leadJ2Idx=-99,leadJ3Idx=-99;
    double leadJ1Pt=-99.,leadJ2Pt=-99.,leadJ3Pt=-99.;
    double leadJ1Eta=-9999.,leadJ2Eta=-9999.,leadJ3Eta=-9999.;
    double leadJ1Phi=-9999.,leadJ2Phi=-9999.,leadJ3Phi=-9999.;
    
    while( evt->loadNext() ){
      eventN++;

      eventWeight = evt->weight();
      double puWeight = 
	puhist->GetBinContent(puhist->GetXaxis()->FindBin(min(evt->TrueNumInteractions_(),puhist->GetBinLowEdge(puhist->GetNbinsX()+1))));  
      //std::cout<<" interactions "<<evt->TrueNumInteractions_()<<" findbin "<<puhist->GetXaxis()->FindBin(evt->TrueNumInteractions_())<<" lastbin "<<puhist->GetBinLowEdge(puhist->GetNbinsX()+1)<<" puweight "<<puWeight<<endl;    
      eventWeight*=puWeight;
      
      //      std::cout<<" eventN "<<eventN<<" eventWeight "<<eventWeight<<endl;
      if(evt->DataBool_())eventWeight = 1.;
      //eventWeight = evt->weight()/evt->puweight();
      //      if(eventN>20000)break;
      //if(eventN>50)break;
      //std::cout<<" eventN "<<eventN<<endl;
      /*    
      cutflow_preselection->Fill(0.,eventWeight); // keep track of all events processed

      // Meant to combine different ttbar samples exclusively
      if(!evt->DataBool_()){
      
        if(sampletype==0){ // TTbar_Inclusive, use only for all-hadronic && HT<600 GeV
          if(evt->gen_ht()>600||evt->GenElecPtVec_().size()>0||evt->GenMuPtVec_().size()>0||evt->GenTauPtVec_().size()>0)continue;
        }

        if(sampletype==1){ // TTbar_Tbar_SingleLep, use only for 1-lepton ttbar && HT<600 GeV
          if(evt->gen_ht()>600)continue; 
        }

        if(sampletype==2){ // TTbar_DiLept, use only for 1-lepton ttbar && HT<600 GeV
          if(evt->gen_ht()>600)continue;
        }
        
      }
      


      //*AR,170823- These were added to get efficiency of different filters in 2017 data.
      if( !fastsim && evt->HBHEIsoNoiseFilter_()!=0)nPass_HBHEIsoNoise++;
      if( !fastsim && evt->HBHENoiseFilter_()!=0)nPass_HBHENoise++;
      if( !fastsim && evt->eeBadScFilter_()!=0)nPass_eeBadSc++;
      if( !fastsim && evt->EcalDeadCellTriggerPrimitiveFilter_()!=0)nPass_EcalDeadCell++;
      if( evt->DataBool_() && evt->BadChargedCandidateFilter_()!=0)nPass_BadCharged++;
      if( evt->DataBool_() && evt->BadPFMuonFilter_()!=0)nPass_BadPFMuon++;
      if( evt->DataBool_() && evt->globalTightHalo2016Filter_()!=0)nPass_globalTight++;
      if( evt->PFCaloMETRatioFilter_()!=0) nPass_PFCaloMET++;
      if( evt->noMuonJet_()!=0)nPass_noMuonJet++;
      if( !fastsim && evt->JetId()!=0)nPass_JetId++;
      if(evt->DataBool_() && (evt->HBHEIsoNoiseFilter_()!=0 || evt->HBHENoiseFilter_()!=0 || evt->eeBadScFilter_()!=0 || evt->EcalDeadCellTriggerPrimitiveFilter_()!=0 || evt->BadChargedCandidateFilter_()!=0 || evt->BadPFMuonFilter_()!=0 || evt->globalTightHalo2016Filter_()!=0 || evt->PFCaloMETRatioFilter_()!=0 || evt->noMuonJet_()!=0 || evt->JetId()!=0)) nPassOR++;

      //	std::cout<<" eventN "<<eventN<<" nPass_HBHEIsoNoise "<<nPass_HBHEIsoNoise<<" nPass_HBHENoise "<<nPass_HBHENoise<<" nPass_eeBadSc "<<nPass_eeBadSc<<" nPass_EcalDeadCell "<<nPass_EcalDeadCell<<" nPass_BadCharged "<<nPass_BadCharged<<" nPass_BadPFMuon "<<nPass_BadPFMuon<<" nPass_globalTight "<<nPass_globalTight<<" nPass_PFCaloMET "<<nPass_PFCaloMET<<" nPass_noMuonJet "<<nPass_noMuonJet<<" nPass_JetId "<<nPass_JetId<<" nPassOR "<<nPassOR<<endl;
      
      cutflow_preselection->Fill(1.,eventWeight);
      if( !fastsim && evt->HBHEIsoNoiseFilter_()==0)continue;
      if( !fastsim && evt->HBHENoiseFilter_()==0)continue;
      cutflow_preselection->Fill(2.,eventWeight);
      if( !fastsim && evt->eeBadScFilter_()==0)continue;
      cutflow_preselection->Fill(3.,eventWeight);
      //if(evt->DataBool_() && !fastsim && !filter.CheckEvent(evt->Runnum(),evt->LumiBlocknum(),evt->Evtnum()))continue;
      //if( !fastsim && evt->CSCTightHaloFilter_()==0)continue;
      if( !fastsim && evt->EcalDeadCellTriggerPrimitiveFilter_()==0)continue;
      
      //added on July 12, 2016
      /*
      if( !fastsim && (evt->BadChargedCandidateFilter_()==0 || evt->BadPFMuonFilter_()==0 
		       || evt->PFCaloMETRatioFilter_()==0 
		       || evt->globalTightHalo2016Filter_()==0 || evt->noMuonJet_()==0) ) {
	if(evt->MuPtVec_().size() >= 1 && evt->met() > 100.0) {
	  std::cout << "run:lumi:event " << evt->Runnum() 
		    << ":"<<evt->LumiBlocknum()<<":"<<evt->Evtnum()
		    << " pfmet " << evt->met() << " muPt,eta,phi " << evt->MuPtVec_().at(0) 
		    << " " << evt->MuEtaVec_().at(0) << " " << evt->MuPhiVec_().at(0) 
		    << " BadChargedCandidateFilter " << evt->BadChargedCandidateFilter_()
		    << " BadPFMuonFilter " << evt->BadPFMuonFilter_()
		  << " PFCaloMETRatioFilter " << evt->PFCaloMETRatioFilter_()
		    << " globalTightHalo2016Filter " << evt->globalTightHalo2016Filter_()
		    << " noMuonJet " << evt->noMuonJet_()
		  << std::endl;
	}
      }
      */
      /*
      if( evt->DataBool_() && evt->BadChargedCandidateFilter_()==0)continue;
      nClean_BadCharged++;
      if( evt->DataBool_() && evt->BadPFMuonFilter_()==0)continue;
      nClean_BadPFMuon++;
      if( evt->DataBool_() && evt->globalTightHalo2016Filter_()==0)continue;
      nClean_globalTight++;
      if( evt->PFCaloMETRatioFilter_()==0) continue;
      nClean_PFCaloMET++;
      if( evt->noMuonJet_()==0) continue;
      nClean_noMuonJet++;
      if( !evt->DataBool_() && fastsim && evt->noFakeJet_()==0) continue;
      nClean_noFakeJet++;
      cutflow_preselection->Fill(4.,eventWeight);
      if(!(evt->NVtx_() >0))continue;
      cutflow_preselection->Fill(5.,eventWeight); 
      // Through out an event that contains HTjets with bad id
      if( !fastsim && evt->JetId()==0)continue;
      nClean_JetId++;
      cutflow_preselection->Fill(6.,eventWeight); // events passing JetID event cleaning
*/
      nCleanEve++;
      //std::cout<<" eventN "<<eventN<<" nClean_BadCharged "<<nClean_BadCharged<<" nClean_BadPFMuon "<<nClean_BadPFMuon<<" nClean_globalTight "<<nClean_globalTight<<" nClean_PFCaloMET "<<nClean_PFCaloMET<<" nClean_noMuonJet "<<nClean_noMuonJet<<" nClean_noFakeJet "<<nClean_noFakeJet<<" nClean_JetId "<<nClean_JetId<<" nCleanEve "<<nCleanEve<<endl;

      

      //std::cout<<" ***CH2*** "<< " evtWeight "<<eventWeight<<endl;
      // Trigger check
      bool trigPass=false;
      bool trigPassNum=false;
      bool trigPassDen=false; 
      bool trigPassLowHT=false;
      bool trigPassHighHT=false;
      //if(isData && 2==1){
      if(isData){
	//std::cout<<" eventN "<<eventN<<endl;
	/*
        string triggerNameToBeUsed = "HLT_Mu15_IsoVVVL_PFHT350_v";
        if (!evt->DataBool_()) triggerNameToBeUsed = "HLT_Mu15_IsoVVVL_PFHT400_v";
	*/
        bool trigfound=false;
	bool trigfoundNum=false;
	bool trigfoundDen=false;
	if(verbose!=0)
          cout << "############################\n "; 

        for(int i=0; i< evt->TriggerNames_().size(); i++){ 
          if(verbose!=0){
            cout << evt->TriggerNames_().at(i) << endl; 
            cout << " Pass: " << evt->PassTrigger_().at(i) << " \n+\n";
	  }
	 
          string trigStr;
          sprintf(tempname, "HLT_IsoMu24_eta2p1_v");
          sprintf(tempname2,"HLT_PFMET120_PFMHT120_IDTight_v");
          sprintf(tempname3,"HLT_PFMET120_PFMHT120_IDTight_v");
          sprintf(tempname4,"HLT_PFMET120_PFMHT120_IDTight_v");
	  sprintf(tempname5,"HLT_PFMET120_PFMHT120_IDTight_v");

	  if(evt->TriggerNames_().at(i).find(tempname) != string::npos
	     || evt->TriggerNames_().at(i).find(tempname2) != string::npos
	     || evt->TriggerNames_().at(i).find(tempname3) != string::npos
	     || evt->TriggerNames_().at(i).find(tempname4) != string::npos
	     || evt->TriggerNames_().at(i).find(tempname5) != string::npos)
	    trigfoundNum=true;
	  if(evt->TriggerNames_().at(i).find(tempname2) != string::npos
	     || evt->TriggerNames_().at(i).find(tempname3) != string::npos
	     || evt->TriggerNames_().at(i).find(tempname4) != string::npos
	     || evt->TriggerNames_().at(i).find(tempname5) != string::npos)
	    {
	      if(evt->PassTrigger_().at(i)==1)
		trigPassDen=true;
	    }
	  if(evt->TriggerNames_().at(i).find(tempname) != string::npos){
	    if(evt->PassTrigger_().at(i)==1)
	      trigPassNum=true;
	  }
	  
	  /*
	    if (lowHTSelection){
	    sprintf(tempname,"HLT_IsoMu22_v");
	    sprintf(tempname2,"");
	    }
	  */
          if( evt->TriggerNames_().at(i).find(tempname)  != string::npos  
	      // evt->TriggerNames_().at(i).find(tempname2) != string::npos ||
	      //	      evt->TriggerNames_().at(i).find(tempname3) != string::npos
	      //evt->TriggerNames_().at(i).find(tempname4) != string::npos ||
	      //evt->TriggerNames_().at(i).find(tempname5) != string::npos
	      ) {

            trigfound=true;
            if(evt->PassTrigger_().at(i)==1){
	      trigPass=true;
	      trigIndex=i;
	    }
	    // HighHT selection
	    if( evt->TriggerNames_().at(i).find(tempname)  != string::npos  
		//evt->TriggerNames_().at(i).find(tempname2) != string::npos ||
		//		evt->TriggerNames_().at(i).find(tempname3) != string::npos
		//evt->TriggerNames_().at(i).find(tempname4) != string::npos ||
		//evt->TriggerNames_().at(i).find(tempname5) != string::npos 
		){
	      if(evt->PassTrigger_().at(i)==1)trigPassHighHT=true;
	    }
	    // LowHT selection
	    if( evt->TriggerNames_().at(i).find(tempname3) != string::npos 
		//evt->TriggerNames_().at(i).find(tempname4) != string::npos ||
		//evt->TriggerNames_().at(i).find(tempname5) != string::npos
		){
	      if(evt->PassTrigger_().at(i)==1)trigPassLowHT=true;
	    }
	    
	  } // if for both highHT and lowHT triggers
        }   // looping over trigger information
	
        if(!trigfound ){
          cout << " ####\n ####\n trigger was not found \n ####\n ";
	  for(int i=0; i< evt->TriggerNames_().size(); i++){ 
            cout << evt->TriggerNames_().at(i) << " Pass: " << evt->PassTrigger_().at(i) << endl;
	  }
	  
	}
	if(!trigfoundNum)	  
	  std::cout<<" eventN "<<eventN<<" Event did not find either of the triggers "<<endl;
	
        //if(eventN < 100 )cout<< "A temporary selection is in effect \n\n\nA temporary selection is in effect \n\n\nA temporary selection is in effect ";
	//std::cout<<" trigPass "<<trigPass<<endl;
	/*		
        if(!trigPass){
	  for(int i=0; i< evt->MuPtVec_().size(); i++){
	    double pt_FailTrigMuon=evt->MuPtVec_().at(i);
	  }
	  continue;
	}
*/
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
        
        if(TauHadModel>=3){ // Use reco-level muon
	  if(evt->met()>200 && evt->nJets()>=1)
	    NumMuonsPerEvt->Fill(evt->MuPtVec_().size(),eventWeight); 
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
        else{ // Use gen-level muon
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

        if(TauHadModel < 2){ // Use gen-level electron
          for(int i=0; i< evt->GenElecPtVec_().size(); i++){// Ahmad33
            double pt=evt->GenElecPtVec_().at(i); // Ahmad33
            double eta=evt->GenElecEtaVec_().at(i); // Ahmad33 
            double phi=evt->GenElecPhiVec_().at(i); // Ahmad33

              if(verbose==2)printf(" \n Electrons: \n pt: %g eta: %g phi: %g \n ",pt,eta,phi);
              temp3vec.SetPtEtaPhi(pt,eta,phi);
              vec_recoElec3vec.push_back(temp3vec);
          }
        }

        if(vec_recoMuMTW.size()>0)muMtW = vec_recoMuMTW[0]; 
        muMtWHist->Fill(muMtW,eventWeight);

        if(verbose==1)printf(" \n **************************************** \n #Muons: %d #Electrons: %d \n ****************************** \n ",vec_recoMuon4vec.size(),vec_recoElec3vec.size());

        //if( template_nMuons == 1 && template_nElectrons == 0 ) {
        bool pass1=false;
        if(TauHadModel>=2){if( vec_recoMuon4vec.size() == 1)pass1=true;} // Number of reco-level muon=1
        else {if( vec_recoMuon4vec.size() == 1 && vec_recoElec3vec.size() == 0 )pass1=true;} // recoElec is realy GenElec here

        if(pass1){ // pass muon selection

          muPt = vec_recoMuon4vec[0].Pt();
          muEta = vec_recoMuon4vec[0].Eta();
          muPhi = vec_recoMuon4vec[0].Phi();
          muE  = vec_recoMuon4vec[0].E();

	  //double activity= utils->MuActivity(muEta,muPhi,evt->JetsPtVec_(),evt->JetsEtaVec_(),evt->JetsPhiVec_(),evt->Jets_chargedEmEnergyFraction_(),evt->Jets_chargedHadronEnergyFraction_());
          double activity=vec_MTActivity[0]; 
          if(verbose!=0)cout << " activity: " << activity << endl;

          cutflow_preselection->Fill(7.,eventWeight); // 1-mu selection
   
	  bool pass1_1=false;
	  // for veto we have a lepton collection with softer requirements. pT > 10 not 20 and eta < 2.4 not 2.1 and also there is no 
	  // mT cut applied. 
	  if(TauHadModel>=2){if(evt->MuPtVec_().size()>1    || evt->ElecPtVec_().size()>0)pass1_1=true;}    // N(reco-muon)<=1, N(reco-ele)=0, 
	  else{              if(evt->GenMuPtVec_().size()>1 || evt->GenElecPtVec_().size()>0)pass1_1=true;}
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
	  //std::cout<<" HT "<<evt->ht()<<" MHT "<<evt->mht()<<" njets "<<evt->nJets()<<endl;
	  double trigEffCorr_CS=1.;
	  /*
	  if (isData){
	    if (evt->ht()<500.){
	      if (muPt<LeptonAcceptance::muonPtMinLowHT()) trigEffCorr_CS=0.;
	      else {
		//if (trigPassLowHT) trigEffCorr_CS=1./0.824;
		// updated on July 12, 2016, Eff provided by Manuel
		if (trigPassLowHT) {
		  if(muPt>25.0 && muPt<=30) trigEffCorr_CS=1./0.787;
		  else if(muPt>30.0 && muPt<=50)trigEffCorr_CS=1./0.843;
		  else trigEffCorr_CS=1./0.908;
		  
		}
		else trigEffCorr_CS=0.;
	      }
	    } else {
	      if (trigPassHighHT) trigEffCorr_CS=1./0.949;
	      else                trigEffCorr_CS=0.;
	    }
	  }
*/
	  if(isData){
	    if(evt->met()>200 && evt->nJets()>=1){
	      if(trigPassDen){
		//	      std::cout<<" eventN passed "<<eventN<<" pt "<<muPt<<" trigPassNum "<<trigPassNum<<endl;
		Denom_MuonPt->Fill(muPt,eventWeight);
		if(trigPassNum)
		  Num_MuonPt->Fill(muPt,eventWeight);
	      }
	    }
	  }
	  if(sel->ht_500(evt->ht()) && sel->mht_200(evt->mht()) && sel->dphi(evt->nJets(),evt->deltaPhi1(),evt->deltaPhi2(),evt->deltaPhi3(),evt->deltaPhi4()) && sel->Njet_4(evt->nJets())){
	    double corr_eventWeight=eventWeight*trigEffCorr_CS;
	    MuonCS_Njets->Fill(evt->nJets(),eventWeight);
	    MuonCS_Nbjets->Fill(evt->nBtags(),eventWeight);
	    MuonCS_HT->Fill(evt->ht(),eventWeight);
	    MuonCS_MHT->Fill(evt->mht(),eventWeight);
	    MuonCS_MuonPt->Fill(muPt,eventWeight);
	    MuonCS_MuonEta->Fill(muEta,eventWeight);
	    MuonCS_MuonPhi->Fill(muPhi,eventWeight);
	   
	    if(evt->JetsPtVec_().size()>0){
	      JetOne_Pt->Fill(evt->JetsPtVec_()[0],eventWeight);
	      JetOne_Eta->Fill(evt->JetsEtaVec_()[0],eventWeight);
	      JetOne_Phi->Fill(evt->JetsPhiVec_()[0],eventWeight);
	    }
	    if(evt->JetsPtVec_().size()>1){
	      JetTwo_Pt->Fill(evt->JetsPtVec_()[1],eventWeight);
	      JetTwo_Eta->Fill(evt->JetsEtaVec_()[1],eventWeight);
	      JetTwo_Phi->Fill(evt->JetsPhiVec_()[1],eventWeight);
	    }
	    if(evt->JetsPtVec_().size()>2){
	      JetThree_Pt->Fill(evt->JetsPtVec_()[2],eventWeight);
	      JetThree_Eta->Fill(evt->JetsEtaVec_()[2],eventWeight);
	      JetThree_Phi->Fill(evt->JetsPhiVec_()[2],eventWeight);
	    }

	  }
	    //	    if(isData)
	    //std::cout<<" trigIndex "<< trigIndex<<" name "<<evt->TriggerNames_().at(trigIndex)<<endl;
	  
	  
          // start of bootstrapping ( if is on ) 
          
	  
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
      //*AR, Oct13,2016-Calculate mu jet mistagged(btagged) rate
      TH1D * MuBtaggedRate = static_cast<TH1D*>(B_Mu_rate_tagged->Clone("MuBtaggedRate"));
      MuBtaggedRate->Divide(B_Mu_rate_tagged,B_Mu_rate_all,1,1,"B");
      // Write the histogram 
      sprintf(tempname,"%s/TauBtaggedRate_%s_%s.root",Outdir.c_str(),subSampleKey.c_str(),inputnumber.c_str());
      TFile btagfile(tempname,"RECREATE");
      TauBtaggedRate->Write();
      B_rate_tagged->Write();
      B_rate_all->Write();
      MuBtaggedRate->Write();
      B_Mu_rate_tagged->Write();
      B_Mu_rate_all->Write();
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
      TH1* hNonWMuProb_nb_njet2 = static_cast<TH1*>(hNonW_mu_nb_njet2->Clone("hProb_Tau_mu_nb_njet2"));
      hNonWMuProb_nb_njet2->Divide(hNonW_mu_nb_njet2,hAll_mu_nb_njet2,1,1,"B");
      if (hNonW_mu_nb_njet2->GetSumOfWeights()>0.) hNonWMuProb_nb_njet2->Scale(hAll_mu_nb_njet2->GetSumOfWeights()/hNonW_mu_nb_njet2->GetSumOfWeights());
      TH1* hNonWMuProb_nb_njet34 = static_cast<TH1*>(hNonW_mu_nb_njet34->Clone("hProb_Tau_mu_nb_njet34"));
      hNonWMuProb_nb_njet34->Divide(hNonW_mu_nb_njet34,hAll_mu_nb_njet34,1,1,"B");
      if (hNonW_mu_nb_njet34->GetSumOfWeights()>0.) hNonWMuProb_nb_njet34->Scale(hAll_mu_nb_njet34->GetSumOfWeights()/hNonW_mu_nb_njet34->GetSumOfWeights());
      TH1* hNonWMuProb_nb_njet56 = static_cast<TH1*>(hNonW_mu_nb_njet56->Clone("hProb_Tau_mu_nb_njet56"));
      hNonWMuProb_nb_njet56->Divide(hNonW_mu_nb_njet56,hAll_mu_nb_njet56,1,1,"B");
      if (hNonW_mu_nb_njet56->GetSumOfWeights()>0.) hNonWMuProb_nb_njet56->Scale(hAll_mu_nb_njet56->GetSumOfWeights()/hNonW_mu_nb_njet56->GetSumOfWeights());
      TH1* hNonWMuProb_nb_njet78 = static_cast<TH1*>(hNonW_mu_nb_njet78->Clone("hProb_Tau_mu_nb_njet78"));
      hNonWMuProb_nb_njet78->Divide(hNonW_mu_nb_njet78,hAll_mu_nb_njet78,1,1,"B");
      if (hNonW_mu_nb_njet78->GetSumOfWeights()>0.) hNonWMuProb_nb_njet78->Scale(hAll_mu_nb_njet78->GetSumOfWeights()/hNonW_mu_nb_njet78->GetSumOfWeights());
      TH1* hNonWMuProb_nb_njet9 = static_cast<TH1*>(hNonW_mu_nb_njet9->Clone("hProb_Tau_mu_nb_njet9"));
      hNonWMuProb_nb_njet9->Divide(hNonW_mu_nb_njet9,hAll_mu_nb_njet9,1,1,"B");
      if (hNonW_mu_nb_njet9->GetSumOfWeights()>0.) hNonWMuProb_nb_njet9->Scale(hAll_mu_nb_njet9->GetSumOfWeights()/hNonW_mu_nb_njet9->GetSumOfWeights());



      TH1* hNonWMuProb_lowDelphi = static_cast<TH1*>(hNonW_mu_lowDelphi->Clone("hProb_Tau_mu_lowDelphi"));
      hNonWMuProb_lowDelphi->Divide(hNonW_mu_lowDelphi,hAll_mu_lowDelphi,1,1,"B");
      TH1* hNonWMuProb_nb_njet2_lowDelphi = static_cast<TH1*>(hNonW_mu_nb_njet2_lowDelphi->Clone("hProb_Tau_mu_nb_njet2_lowDelphi"));
      hNonWMuProb_nb_njet2_lowDelphi->Divide(hNonW_mu_nb_njet2_lowDelphi,hAll_mu_nb_njet2_lowDelphi,1,1,"B");
      if (hNonW_mu_nb_njet2_lowDelphi->GetSumOfWeights()>0.) hNonWMuProb_nb_njet2_lowDelphi->Scale(hAll_mu_nb_njet2_lowDelphi->GetSumOfWeights()/hNonW_mu_nb_njet2_lowDelphi->GetSumOfWeights());

      TH1* hNonWMuProb_nb_njet34_lowDelphi = static_cast<TH1*>(hNonW_mu_nb_njet34_lowDelphi->Clone("hProb_Tau_mu_nb_njet34_lowDelphi"));
      hNonWMuProb_nb_njet34_lowDelphi->Divide(hNonW_mu_nb_njet34_lowDelphi,hAll_mu_nb_njet34_lowDelphi,1,1,"B");
      if (hNonW_mu_nb_njet34_lowDelphi->GetSumOfWeights()>0.) hNonWMuProb_nb_njet34_lowDelphi->Scale(hAll_mu_nb_njet34_lowDelphi->GetSumOfWeights()/hNonW_mu_nb_njet34_lowDelphi->GetSumOfWeights());

      TH1* hNonWMuProb_nb_njet56_lowDelphi = static_cast<TH1*>(hNonW_mu_nb_njet56_lowDelphi->Clone("hProb_Tau_mu_nb_njet56_lowDelphi"));
      hNonWMuProb_nb_njet56_lowDelphi->Divide(hNonW_mu_nb_njet56_lowDelphi,hAll_mu_nb_njet56_lowDelphi,1,1,"B");
      if (hNonW_mu_nb_njet56_lowDelphi->GetSumOfWeights()>0.) hNonWMuProb_nb_njet56_lowDelphi->Scale(hAll_mu_nb_njet56_lowDelphi->GetSumOfWeights()/hNonW_mu_nb_njet56_lowDelphi->GetSumOfWeights());

      TH1* hNonWMuProb_nb_njet78_lowDelphi = static_cast<TH1*>(hNonW_mu_nb_njet78_lowDelphi->Clone("hProb_Tau_mu_nb_njet78_lowDelphi"));
      hNonWMuProb_nb_njet78_lowDelphi->Divide(hNonW_mu_nb_njet78_lowDelphi,hAll_mu_nb_njet78_lowDelphi,1,1,"B");
      if (hNonW_mu_nb_njet78_lowDelphi->GetSumOfWeights()>0.) hNonWMuProb_nb_njet78_lowDelphi->Scale(hAll_mu_nb_njet78_lowDelphi->GetSumOfWeights()/hNonW_mu_nb_njet78_lowDelphi->GetSumOfWeights());

      TH1* hNonWMuProb_nb_njet9_lowDelphi = static_cast<TH1*>(hNonW_mu_nb_njet9_lowDelphi->Clone("hProb_Tau_mu_nb_njet9_lowDelphi"));
      hNonWMuProb_nb_njet9_lowDelphi->Divide(hNonW_mu_nb_njet9_lowDelphi,hAll_mu_nb_njet9_lowDelphi,1,1,"B");
      if (hNonW_mu_nb_njet9_lowDelphi->GetSumOfWeights()>0.) hNonWMuProb_nb_njet9_lowDelphi->Scale(hAll_mu_nb_njet9_lowDelphi->GetSumOfWeights()/hNonW_mu_nb_njet9_lowDelphi->GetSumOfWeights());

      sprintf(tempname,"%s/Probability_Tau_mu_%s_%s.root",Outdir.c_str(),subSampleKey.c_str(),inputnumber.c_str());
      TFile muProbFile(tempname,"RECREATE");     
      hNonWMuProb->Write();
      hNonWMuProb_nb_njet2->Write();
      hNonWMuProb_nb_njet34->Write();
      hNonWMuProb_nb_njet56->Write();
      hNonWMuProb_nb_njet78->Write();
      hNonWMuProb_nb_njet9->Write();

      hAll_mu->Write();
      hAll_mu_nb_njet2->Write();
      hAll_mu_nb_njet34->Write();
      hAll_mu_nb_njet56->Write();
      hAll_mu_nb_njet78->Write();
      hAll_mu_nb_njet9->Write();

      hNonW_mu->Write();
      hNonW_mu_nb_njet2->Write();
      hNonW_mu_nb_njet34->Write();
      hNonW_mu_nb_njet56->Write();
      hNonW_mu_nb_njet78->Write();
      hNonW_mu_nb_njet9->Write();

      hNonWMuProb_lowDelphi->Write();
      hNonWMuProb_nb_njet2_lowDelphi->Write();
      hNonWMuProb_nb_njet34_lowDelphi->Write();
      hNonWMuProb_nb_njet56_lowDelphi->Write();
      hNonWMuProb_nb_njet78_lowDelphi->Write();
      hNonWMuProb_nb_njet9_lowDelphi->Write();
      hAll_mu_lowDelphi->Write();
      hAll_mu_nb_njet2_lowDelphi->Write();
      hAll_mu_nb_njet34_lowDelphi->Write();
      hAll_mu_nb_njet56_lowDelphi->Write();
      hAll_mu_nb_njet78_lowDelphi->Write();
      hAll_mu_nb_njet9_lowDelphi->Write();
      hNonW_mu_lowDelphi->Write();
      hNonW_mu_nb_njet2_lowDelphi->Write();
      hNonW_mu_nb_njet34_lowDelphi->Write();
      hNonW_mu_nb_njet56_lowDelphi->Write();
      hNonW_mu_nb_njet78_lowDelphi->Write();
      hNonW_mu_nb_njet9_lowDelphi->Write();
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
    MuonCS_Njets->Write();
    MuonCS_Nbjets->Write();
    MuonCS_HT->Write();
    MuonCS_MHT->Write();
    MuonCS_MuonPt->Write();
    MuonCS_MuonEta->Write();
    MuonCS_MuonPhi->Write();
    JetOne_Pt->Write();
    JetOne_Eta->Write();
    JetOne_Phi->Write();
    JetTwo_Pt->Write();
    JetTwo_Eta->Write();
    JetTwo_Phi->Write();
    JetThree_Pt->Write();
    JetThree_Eta->Write();
    JetThree_Phi->Write();
    NumMuonsPerEvt->Write();
    Num_MuonPt->Write();
    Denom_MuonPt->Write();
    searchH->Write();
    searchH_nb_njet2->Write();
    searchH_nb_njet34->Write();
    searchH_nb_njet56->Write();
    searchH_nb_njet78->Write();
    searchH_nb_njet9->Write();
    searchH_lowDphi->Write();
    searchH_nb_njet2_lowDphi->Write();
    searchH_nb_njet34_lowDphi->Write();
    searchH_nb_njet56_lowDphi->Write();
    searchH_nb_njet78_lowDphi->Write();
    searchH_nb_njet9_lowDphi->Write();

    QCD_Up->Write();
    QCD_Low->Write();
    searchH_b->Write();
    searchH_b_noWeight->Write();
    MuPt_NJ34->Write();
    MuEta_NJ34->Write();
    MuPhi_NJ34->Write();
    MuPt_NJ56->Write();
    MuEta_NJ56->Write();
    MuPhi_NJ56->Write();
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
        /*
              if(it->first=="PreSel" || it->first=="nolep" || it->first=="ht_500" 
                 || it->first=="Njet_4" || it->first=="isoElec" || it->first=="isoMu" || it->first=="nolep"
                )continue;
*/
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

  


