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
//#include "splitString.h"
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
    char tempname6[200];
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
    vector<double> vec_recoEleMTW;
    vector<TLorentzVector> vec_recoElectron4vec;

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

    TH2 * hAcc_All=new TH2D("hAcc_All","hAcc_All",400,0,400,50,-2.5,2.5);
    TH2 * hAcc_Pass=new TH2D("hAcc_Pass","hAcc_Pass",400,0,400,50,-2.5,2.5);
    hAcc_All->Sumw2();
    hAcc_Pass->Sumw2();

    //sprintf(prefix,"/data3/");
    int GetMuSize=0;
    int GetEleSize=0;
    TH2 * hMT_All=new TH2D("hMT_All","hMT_All",400,0,400,50,-2.5,2.5);
    TH2 * hMT_Pass=new TH2D("hMT_Pass","hMT_Pass",400,0,400,50,-2.5,2.5);
    hMT_All->Sumw2();
    hMT_Pass->Sumw2();

    TH2 * hMu_All=new TH2D("hMu_All","hMu_All",400,0,400,50,-2.5,2.5);
    TH2 * hMu_NonW=new TH2D("hMu_NonW","hMu_NonW",400,0,400,50,-2.5,2.5);

    hMu_All->Sumw2(); 
    hMu_NonW->Sumw2();

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
    cutflow_preselection->GetXaxis()->SetBinLabel(4,"ecalBadCalibFilter");
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

    TH1 * HT_HEMiss_Phi_2Pt38_2Pt72 =new TH1D("HT_HEMiss_Phi_2Pt38_2Pt72","HT_HEMiss_Phi_2Pt38_2Pt72", 50,0,1000);
    //    TH1 * MHT_HEMiss_Phi_2Pt38_2Pt72 =new TH1D("MHT_HEMiss_Phi_2Pt38_2Pt72","MHT_HEMiss_Phi_2Pt38_2Pt72", 60,0,3000);
    //TH1 * MHT_HEMiss_Phi_2Pt38_2Pt72 =new TH1D("MHT_HEMiss_Phi_2Pt38_2Pt72","MHT_HEMiss_Phi_2Pt38_2Pt72", 60,0,3000);
    TH1 * NJet_HEMiss_Phi_2Pt38_2Pt72 =new TH1D("NJet_HEMiss_Phi_2Pt38_2Pt72","NJet_HEMiss_Phi_2Pt38_2Pt72", 60,0,3000);

    std::map<int,int>  EventNRunN;
    std::map<int,int> EventNLumiN;
    std::map<int,std::map<int,int>> EventNRunNLumiN;
    std::map<int,string> LumiNRootFile;
    std::map<std::pair<std::pair<int,int>, int>,double> EventNRunNLumiNMET;
    std::map<std::pair<double,double>,double> MuEtaEleEtaMET;
    std::map<std::pair<double,double>,double> HTMHTMET;
    std::map<std::pair<std::pair<std::pair<int,int>, int>,std::pair<std::pair<std::pair<double,double>,std::pair<std::pair<double,double>,std::pair<double,double>>>,std::pair<std::pair<double,double>,std::pair<std::pair<double,double>,std::pair<double,double>>>>>,double> FinalMap;

    vector<int> EventNHEMiss;
    vector<int> RunNHEMiss;
    vector<int> LumiBlockNHEMiss; 
    vector<double> MuPtHEMiss;
    vector<double> MuEtaHEMiss; 
    vector<double> MuPhiHEMiss; 
    vector<double> ElePtHEMiss;
    vector<double> EleEtaHEMiss; 
    vector<double> ElePhiHEMiss; 
    vector<int> NJetHEMiss;
    vector<int> NBTagHEMiss;
    vector<double>HTHEMiss;
    vector<double>MHTHEMiss;
    vector<double>METHEMiss;
    vector<double>MHTPhiHEMiss;
    vector<double>DPhiOneHEMiss;
    //map<string,string> binMap_b = utils2::BinMap();
    int totNbins_b=binMap_b.size();
    TH1* searchH_b = new TH1D("searchH_b","search bin histogram",totNbins_b,1,totNbins_b+1);
    searchH_b->Sumw2();
    TH1 * h_HT_SearchStat = new TH1D("h_HT_SearchStat","h_HT_SearchStat",50, 0, 10000);
    h_HT_SearchStat->Sumw2();
    TH1 * h_MHT_SearchStat = new TH1D("h_MHT_SearchStat","h_MHT_SearchStat",60, 0, 3000);
    h_MHT_SearchStat->Sumw2(); 
    TH1 * h_MET_SearchStat = new TH1D("h_MET_SearchStat","h_MET_SearchStat",60, 0, 3000);
    h_MET_SearchStat->Sumw2(); 
    TH1 * h_NJet_SearchStat = new TH1D("h_NJet_SearchStat","h_NJet_SearchStat",20,0,20);
    h_NJet_SearchStat->Sumw2();
    TH1 * h_NBTag_SearchStat = new TH1D("h_NBTag_SearchStat","h_NBTag_SearchStat",10,0,10);
    h_NBTag_SearchStat->Sumw2();
    TH2 * h_HT_SearchStat_TwoD = new TH2D("h_HT_SearchStat_TwoD","h_HT_SearchStat_TwoD",50, 0, 1000,50, 0, 1000);

    TH2 * h_MHT_SearchStat_TwoD = new TH2D("h_MHT_SearchStat_TwoD","h_MHT_SearchStat_TwoD",60, 0, 3000,60, 0, 3000);
    TH2 * h_MET_SearchStat_TwoD = new TH2D("h_MET_SearchStat_TwoD","h_MET_SearchStat_TwoD",60, 0, 3000,60, 0, 3000);
    TH2 * h_NJet_SearchStat_TwoD = new TH2D("h_NJet_SearchStat_TwoD","h_NJet_SearchStat_TwoD",20, 0, 20,20, 0, 20);
    TH2 * h_NBTag_SearchStat_TwoD = new TH2D("h_NBTag_SearchStat_TwoD","h_NBTag_SearchStat_TwoD",10, 0, 10,10, 0, 10);

    TH1 * MuonCS_pt=new TH1D("MuonCS_pt","MuonCS_pt",100,0.0,1000.0); 
    TH1 * MuonCS_eta=new TH1D("MuonCS_eta","MuonCS_eta",100,-5.0,5.0);
    TH1 * MuonCS_phi=new TH1D("MuonCS_phi","MuonCS_phi",70,-3.5,3.5); 
    MuonCS_pt->Sumw2();
    MuonCS_eta->Sumw2();
    MuonCS_phi->Sumw2();

    TH2 * MuonCS_TwoD_pt=new TH2D("MuonCS_TwoD_pt","MuonCS_TwoD_pt",100,0.0,1000.0,100,0.0,1000.0); 
    TH2 * MuonCS_TwoD_eta=new TH2D("MuonCS_TwoD_eta","MuonCS_TwoD_eta",100,-5.0,5.0,100,-5.0,5.0);
    TH2 * MuonCS_TwoD_phi=new TH2D("MuonCS_TwoD_phi","MuonCS_TwoD_phi",70,-3.5,3.5,70,-3.5,3.5); 


    TH1 * ElectronCS_pt=new TH1D("ElectronCS_pt","ElectronCS_pt",100,0.0,1000.0); 
    TH1 * ElectronCS_eta=new TH1D("ElectronCS_eta","ElectronCS_eta",100,-5.0,5.0);
    TH1 * ElectronCS_phi=new TH1D("ElectronCS_phi","ElectronCS_phi",70,-3.5,3.5); 
    ElectronCS_pt->Sumw2();
    ElectronCS_eta->Sumw2();
    ElectronCS_phi->Sumw2();

    TH2 * ElectronCS_TwoD_pt=new TH2D("ElectronCS_TwoD_pt","ElectronCS_TwoD_pt",100,0.0,1000.0,100,0.0,1000.0); 
    TH2 * ElectronCS_TwoD_eta=new TH2D("ElectronCS_TwoD_eta","ElectronCS_TwoD_eta",100,-5.0,5.0,100,-5.0,5.0);
    TH2 * ElectronCS_TwoD_phi=new TH2D("ElectronCS_TwoD_phi","ElectronCS_TwoD_phi",70,-3.5,3.5,70,-3.5,3.5); 

    TH1 * j1_pt=new TH1D("j1_pt","j1_pt",100,0.0,1000.0);
    TH1 * j2_pt=new TH1D("j2_pt","j2_pt",100,0.0,1000.0);
    TH1 * j3_pt=new TH1D("j3_pt","j3_pt",100,0.0,1000.0);
    TH1 * j4_pt=new TH1D("j4_pt","j4_pt",100,0.0,1000.0);
    TH1 * j_pt_inclusive=new TH1D("j_pt_inclusive","j_pt_inclusive",100,0.0,1000.0);
    j1_pt->Sumw2(); 
    j2_pt->Sumw2();
    j3_pt->Sumw2();
    j4_pt->Sumw2();
    j_pt_inclusive->Sumw2();


    TH1 * j1_eta=new TH1D("j1_eta","j1_eta",100,-5.0,5.0);
    TH1 * j2_eta=new TH1D("j2_eta","j2_eta",100,-5.0,5.0);
    TH1 * j3_eta=new TH1D("j3_eta","j3_eta",100,-5.0,5.0);
    TH1 * j4_eta=new TH1D("j4_eta","j4_eta",100,-5.0,5.0);
    TH1 * j_eta_inclusive=new TH1D("j_eta_inclusive","j_eta_inclusive",100,-5.0,5.0);

    j1_eta->Sumw2(); 
    j2_eta->Sumw2();
    j3_eta->Sumw2();
    j4_eta->Sumw2();
    j_eta_inclusive->Sumw2();
    TH1 * j1_phi=new TH1D("j1_phi","j1_phi",70,-3.5,3.5);
    TH1 * j2_phi=new TH1D("j2_phi","j2_phi",70,-3.5,3.5);
    TH1 * j3_phi=new TH1D("j3_phi","j3_phi",70,-3.5,3.5);
    TH1 * j4_phi=new TH1D("j4_phi","j4_phi",70,-3.5,3.5);
    TH1 * j_phi_inclusive=new TH1D("j_phi_inclusive","j_phi_inclusive",70,-3.5,3.5);
    j_phi_inclusive->Sumw2();
    j1_phi->Sumw2(); 
    j2_phi->Sumw2();
    j3_phi->Sumw2();
    j4_phi->Sumw2();


    TH1 * DeltaPhi_j1MHT_SearchStat =  new TH1D("DeltaPhi_j1MHT_SearchStat","DeltaPhi_j1MHT_SearchStat",50,0,5);
    TH1 * DeltaPhi_j2MHT_SearchStat =  new TH1D("DeltaPhi_j2MHT_SearchStat","DeltaPhi_j2MHT_SearchStat",50,0,5);
    TH1 * DeltaPhi_j3MHT_SearchStat =  new TH1D("DeltaPhi_j3MHT_SearchStat","DeltaPhi_j3MHT_SearchStat",50,0,5);
    TH1 * DeltaPhi_j4MHT_SearchStat =  new TH1D("DeltaPhi_j4MHT_SearchStat","DeltaPhi_j4MHT_SearchStat",50,0,5);
    DeltaPhi_j1MHT_SearchStat->Sumw2();
    DeltaPhi_j2MHT_SearchStat->Sumw2();
    DeltaPhi_j3MHT_SearchStat->Sumw2();
    DeltaPhi_j4MHT_SearchStat->Sumw2();

    TH2 * DeltaPhi_TwoD_j1MHT_SearchStat =  new TH2D("DeltaPhi_TwoD_j1MHT_SearchStat","DeltaPhi_TwoD_j1MHT_SearchStat",50,0,5,50,0,5);
    //    TH2 * DeltaPhi_TwoD_j1MHT_SearchStat =  new TH2D("DeltaPhi_TwoD_j1MHT_SearchStat","DeltaPhi_TwoD_j1MHT_SearchStat",140,-7,7,140,-7,7);

    TH2 * MHTPhi_TwoD_SearchStat =  new TH2D("MHTPhi_TwoD_SearchStat","MHTPhi_TwoD_SearchStat",70,-3.5,3.5,70,-3.5,3.5);

    TH1 * DeltaPhi_j1MET_SearchStat =  new TH1D("DeltaPhi_j1MET_SearchStat","DeltaPhi_j1MET_SearchStat",50,0,5);
    TH1 * DeltaPhi_j2MET_SearchStat =  new TH1D("DeltaPhi_j2MET_SearchStat","DeltaPhi_j2MET_SearchStat",50,0,5);
    TH1 * DeltaPhi_j3MET_SearchStat =  new TH1D("DeltaPhi_j3MET_SearchStat","DeltaPhi_j3MET_SearchStat",50,0,5);
    TH1 * DeltaPhi_j4MET_SearchStat =  new TH1D("DeltaPhi_j4MET_SearchStat","DeltaPhi_j4MET_SearchStat",50,0,5);
    DeltaPhi_j1MET_SearchStat->Sumw2();
    DeltaPhi_j2MET_SearchStat->Sumw2();
    DeltaPhi_j3MET_SearchStat->Sumw2();
    DeltaPhi_j4MET_SearchStat->Sumw2();


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
    std::cout<<" sample_AUX "<<sample_AUX<<endl;
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
    int nCleanEve=0;
    int PassHBHEIsoNoiseFilter=0;
    int PassHBHENoiseFilter=0;
    int PassecalBadCalibFilter=0;
    int PassEcalDeadCellTriggerPrimitiveFilter=0;
    int PassBadChargedCandidateFilter=0;
    int PassBadPFMuonFilter=0;
    int PassglobalSuperTightHalo2016Filter=0;
    int PassPFCaloMETRatioFilter=0;
    int PassnoMuonJet=0;
    int PassNVtx=0;
    int PassJetId=0;
    int OneLepCSEvt=0;
    int PassNoExtraEleOrMuWithIsoApplied=0;
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
    int MuonCSevt=0;
    int MuonPassAcc=0;
    int MuonFailAcc=0;
    int MuonPassmT=0;
    int ElectronCSevt=0;
    int ElectronFailAcc=0;
    int ElectronPassAcc=0;
    int ElectronPassmT=0;
    int TriggerPass=0;
    int noLepEvt=0;
    int OneEleEvt=0;
    int OneMuEvt=0;
    int HEMMapMatch=0;
    while( evt->loadNext() ){
      eventN++;
      //      std::cout<<"  **** Before Matching ****"<<" evt "<<eventN<<" run "<<evt->Runnum()<<" lumi "<<evt->LumiBlocknum()<<endl;
      double NomMuPt=-99;
      double NomMuEta=-99;
      double NomMuPhi=-99;
      double NomElePt=-99;
      double NomEleEta=-99;
      double NomElePhi=-99;
      double NomNJet=-99;
      double NomNBtag=-99;
      double NomHT=-99;
      double NomMHT=-99;
      double NomMHTPhi=-99;
      double NomDphiOne=-99;
      double NomMET=-99;
      if(utils2::UseHEMEvtMap){
	ifstream ifile("Map.txt");
	vector<int> vecEvt;
	vector<double> vecEvtDetails;
	string line;
	bool MatchEvt=false;
	int CheckEvtNum=0;
	while(ifile.getline(filenames, 500) )
	  {
	    vector<int> vecEvt;
	    char *p;
	    const char s[2] = "|";
	    int itrr=-1;
	    p = strtok (filenames,s);
	    CheckEvtNum=atoi(p);
	    if(CheckEvtNum != evt->Evtnum()){
	      // std::cout<<" evtN "<<evt->Evtnum()<<" p "<<p<<endl;
	      continue;
	    }
	    while (p!= NULL){
	      if(vecEvt.size()<3){	      
		int number= atoi(p);
		vecEvt.push_back(number);
	      }
	      else{
		double d = atof(p);
		vecEvtDetails.push_back(d);
	      }
	      p = strtok (NULL, "|");
	    }
	    //	    std::cout<<" evt "<<vecEvt[0]<<" run "<<vecEvt[1]<<" lumi "<<vecEvt[2]<<endl;
	    if(vecEvt[0] == evt->Evtnum() && vecEvt[1]==evt->Runnum() && vecEvt[2]==evt->LumiBlocknum()){
	      MatchEvt=true;
	      break;
	    } 
	  }
	if(!MatchEvt){
	  continue;
	}
	NomMuPt=vecEvtDetails[0];
	if(NomMuPt==-99) NomMuPt=900;
	NomElePt=vecEvtDetails[1];
	if(NomElePt==-99) NomElePt=900;

	NomMuEta=vecEvtDetails[2];
	if(NomMuEta==-99) NomMuEta=4.;
	NomEleEta=vecEvtDetails[3];
	if(NomEleEta==-99) NomEleEta=4.;

	NomMuPhi=vecEvtDetails[4];
	if(NomMuPhi==-99) NomMuPhi=3.4;
	NomElePhi=vecEvtDetails[5];
	if(NomElePhi==-99) NomElePhi=3.4;

	NomNJet=vecEvtDetails[6];
	NomNBtag=vecEvtDetails[7];
	NomHT=vecEvtDetails[8];
	NomMHT=vecEvtDetails[9];
	NomMHTPhi=vecEvtDetails[10];
	NomDphiOne=vecEvtDetails[11];
	NomMET=vecEvtDetails[12];
	HEMMapMatch++;
	//	std::cout<<"  ** matched found **"<<" evt "<<evt->Evtnum()<<" run "<<evt->Runnum()<<" lumi "<<evt->LumiBlocknum()<<" muPt "<<NomMuPt<<" elePt "<<NomElePt<<" muEta "<<NomMuEta<<" eleeta "<<NomEleEta<<" muPhi "<<NomMuPhi<<" elePhi "<<NomElePhi<<" njet "<<NomNJet<<" nbtag "<<NomNBtag<<" ht "<<NomHT<<" mht "<<NomMHT<<" mhtphi "<<NomMHTPhi<<" dphi1 "<<NomDphiOne<<" met "<<NomMET<<endl;
      } //end of use map





      //      std::cout<<" new event "<<endl;
      eventWeight = evt->weight();
      double puWeight = 
	puhist->GetBinContent(puhist->GetXaxis()->FindBin(min(evt->TrueNumInteractions_(),puhist->GetBinLowEdge(puhist->GetNbinsX()+1))));  
      //std::cout<<" interactions "<<evt->TrueNumInteractions_()<<" findbin "<<puhist->GetXaxis()->FindBin(evt->TrueNumInteractions_())<<" lastbin "<<puhist->GetBinLowEdge(puhist->GetNbinsX()+1)<<" puweight "<<puWeight<<endl;    
      eventWeight*=puWeight;
      
      //      std::cout<<" eventN "<<eventN<<" eventWeight "<<eventWeight<<endl;
      if(evt->DataBool_())eventWeight = 1.;
      if(evt->Evtnum()==631307 || evt->Evtnum()==2020962 || evt->Evtnum()==6046723)
	std::cout<<" evt "<<evt->Evtnum()<<" total_ele "<<evt->ElecPtVec_().size()<<" total_mu "<<evt->MuPtVec_().size()<<" ele_passIsolation "<<evt->nElectrons()<<" mu_passIsolation "<<evt->nMuons()<<endl;
      else
	continue;
      //eventWeight = evt->weight()/evt->puweight();
      //if(eventN>10000)break;
      if(eventN%5000==0)
	std::cout<<" eventN "<<eventN<<endl;
      //if(eventN>50)break;
      //std::cout<<" eventN "<<eventN<<endl;
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
      
      cutflow_preselection->Fill(1.,eventWeight);
      if( !fastsim && evt->HBHEIsoNoiseFilter_()==0)continue;
      PassHBHEIsoNoiseFilter++;
      if( !fastsim && evt->HBHENoiseFilter_()==0)continue;
      PassHBHENoiseFilter++;
      cutflow_preselection->Fill(2.,eventWeight);
      if( !fastsim && evt->ecalBadCalibFilter_()==0)continue;
      PassecalBadCalibFilter++;
      cutflow_preselection->Fill(3.,eventWeight);
      //if(evt->DataBool_() && !fastsim && !filter.CheckEvent(evt->Runnum(),evt->LumiBlocknum(),evt->Evtnum()))continue;
      //if( !fastsim && evt->CSCTightHaloFilter_()==0)continue;
      if( !fastsim && evt->EcalDeadCellTriggerPrimitiveFilter_()==0)continue;
      PassEcalDeadCellTriggerPrimitiveFilter++;
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
      if( evt->DataBool_() && evt->BadChargedCandidateFilter_()==0) continue;
      PassBadChargedCandidateFilter++;
      if( evt->DataBool_() && evt->BadPFMuonFilter_()==0) continue;
      PassBadPFMuonFilter++;
      if( evt->DataBool_() && evt->globalSuperTightHalo2016Filter_()==0) continue;
      PassglobalSuperTightHalo2016Filter++;
      if( evt->PFCaloMETRatioFilter_()==0) continue;
      PassPFCaloMETRatioFilter++;
      if( evt->noMuonJet_()==0) continue;
      PassnoMuonJet++;
      if( !evt->DataBool_() && fastsim && evt->noFakeJet_()==0) continue;

      cutflow_preselection->Fill(4.,eventWeight);
      if(!(evt->NVtx_() >0))continue;
      PassNVtx++;
      cutflow_preselection->Fill(5.,eventWeight); 
      // Through out an event that contains HTjets with bad id
      if( !fastsim && evt->JetId()==0)continue;
      PassJetId++;
      cutflow_preselection->Fill(6.,eventWeight); // events passing JetID event cleaning

      nCleanEve++;
      //      std::cout<<" nCleanEve "<<nCleanEve<<endl;
      //std::cout<<" ***CH2*** "<< " evtWeight "<<eventWeight<<endl;
      // Trigger check
      bool trigPass=false;
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
        if(verbose!=0)
          cout << "############################\n "; 

        for(int i=0; i< evt->TriggerNames_().size(); i++){ 
          if(verbose!=0){
            cout << evt->TriggerNames_().at(i) << endl; 
            cout << " Pass: " << evt->PassTrigger_().at(i) << " \n+\n";
	  }
	  //	  std::cout << "i "<<i <<evt->TriggerNames_().at(i) << endl;
          string trigStr;
          sprintf(tempname, "HLT_PFMET100_PFMHT100_IDTight_v");
          sprintf(tempname2,"HLT_PFMET110_PFMHT110_IDTight_v");
          sprintf(tempname3,"HLT_PFMET120_PFMHT120_IDTight_v");
          sprintf(tempname4,"HLT_PFMETNoMu100_PFMHTNoMu100_IDTight_v");
	  sprintf(tempname5,"HLT_PFMETNoMu110_PFMHTNoMu110_IDTight_v");
	  sprintf(tempname6,"HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_v");
	  /*
	  if (lowHTSelection){
	    sprintf(tempname,"HLT_IsoMu22_v");
	    sprintf(tempname2,"");
	  }
	  */
          if( evt->TriggerNames_().at(i).find(tempname)  != string::npos || 
	      evt->TriggerNames_().at(i).find(tempname2) != string::npos ||
	      evt->TriggerNames_().at(i).find(tempname3) != string::npos ||
	      evt->TriggerNames_().at(i).find(tempname4) != string::npos ||
	      evt->TriggerNames_().at(i).find(tempname5) != string::npos ||
	      evt->TriggerNames_().at(i).find(tempname6) != string::npos) {

            trigfound=true;
            if(evt->PassTrigger_().at(i)==1)trigPass=true;
	    //	    std::cout<<" eventN "<<eventN<<" trigger is passed "<<endl;
	    /*
	    // HighHT selection
	    if( evt->TriggerNames_().at(i).find(tempname)  != string::npos || 
		evt->TriggerNames_().at(i).find(tempname2) != string::npos ||
		evt->TriggerNames_().at(i).find(tempname3) != string::npos ||
		evt->TriggerNames_().at(i).find(tempname4) != string::npos ||
		evt->TriggerNames_().at(i).find(tempname5) != string::npos 
		){
	      if(evt->PassTrigger_().at(i)==1)trigPassHighHT=true;
	    }
	    // LowHT selection
	    if( evt->TriggerNames_().at(i).find(tempname3) != string::npos ||
		evt->TriggerNames_().at(i).find(tempname4) != string::npos ||
		evt->TriggerNames_().at(i).find(tempname5) != string::npos){
	      if(evt->PassTrigger_().at(i)==1)trigPassLowHT=true;
	    }
	    */
	  } // if for both highHT and lowHT triggers
        }   // looping over trigger information
	
        if(!trigfound ){
          cout << " ####\n ####\n trigger was not found \n ####\n ";
	  for(int i=0; i< evt->TriggerNames_().size(); i++){ 
            cout << evt->TriggerNames_().at(i) << " Pass: " << evt->PassTrigger_().at(i) << endl;
	  }
	  
        }
        //if(eventN < 100 )cout<< "A temporary selection is in effect \n\n\nA temporary selection is in effect \n\n\nA temporary selection is in effect ";
	//std::cout<<" trigPass "<<trigPass<<endl;
        if(!trigPass)continue;
      }
      TriggerPass++;
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

	vec_recoEleMTW.clear();
	vec_recoElectron4vec.clear();
	vector<int> MuFromTauVec;//Ahmad33
	MuFromTauVec.clear();//Ahmad33
	if(evt->MuPtVec_().size()==0 && evt->ElecPtVec_().size()==0)
	  noLepEvt++;
	if(evt->MuPtVec_().size()==1)
	  OneMuEvt++;
	if(evt->ElecPtVec_().size()==1)
	  OneEleEvt++;

        // Consistancy check
        if(isData==true && TauHadModel<3){
          cout << "Only TauHadModel>=3 is valid for Data \n ";
          return 2;
        }
	//	if(evt->nMuons() !=evt->MuPtVec_().size())
	//std::cout<<" nMuons "<< evt->nMuons() <<" size "<<evt->MuPtVec_().size()<<endl;
        if(TauHadModel>=3){ // Use reco-level muon
	  GetMuSize=evt->MuPtVec_().size();
	  //	  std::cout<<" GetMuSize "<<GetMuSize<<endl;
	  for(int i=0; i< evt->MuPtVec_().size(); i++){ // Ahmad33
            double pt=evt->MuPtVec_().at(i); // Ahmad33
	    //std::cout<<" pt "<<pt<<endl;
            double eta=evt->MuEtaVec_().at(i); // Ahmad33
	    //std::cout<<" eta "<<eta<<endl;
            double phi=evt->MuPhiVec_().at(i); // Ahmad33
	    //std::cout<<" phi "<<phi<<endl;
            double energy=evt->MuEVec_().at(i); // Ahmad33
	    bool passMini = evt->Muons_passIso_().at(i);
	    //	    std::cout<<" passMini "<<passMini <<endl;
	    //	    std::cout<<" eventN "<<eventN<<" mu_pt "<<pt<<" mu_eta "<<eta<<endl;
	    if(pt<20 || fabs(eta)>2.1)
	      MuonFailAcc++;

	    //std::cout<<" energy "<<energy<<endl; 
	    //            double activity_ = evt->MTActivityVec_().at(i);
	    //std::cout<<" activity_ "<<activity_<<endl;
            double mu_mt_w =utils->calcMT(pt,phi,evt->met(),evt->metphi());  
	    //std::cout<<" mu_mt_w "<<mu_mt_w<<endl;
	    if( passMini && pt> LeptonAcceptance::muonPtMin()  && fabs(eta)< LeptonAcceptance::muonEtaMax()  ){
	      MuonPassAcc++;
	      temp4vec.SetPtEtaPhiE(pt,eta,phi,energy);
              if(utils2::applyMT){
                if(mu_mt_w < 100. ){
		  MuonPassmT++;
		  vec_recoMuon4vec.push_back(temp4vec);
		  //vec_MTActivity.push_back(activity_);
		}
              }
              else {vec_recoMuon4vec.push_back(temp4vec);
		//vec_MTActivity.push_back(activity_);
	      }
              vec_recoMuMTW.push_back(mu_mt_w); 
            }
          }
	 

	  GetEleSize=evt->ElecPtVec_().size();
	  //	  if(evt->nElectrons() !=evt->ElecPtVec_().size())
	  //std::cout<<" nEles "<< evt->nElectrons() <<" size "<<evt->ElecPtVec_().size()<<endl;

	  //std::cout<<" GetEleSize "<<GetEleSize<<endl; 
	  for(int i=0; i< evt->ElecPtVec_().size(); i++){ // Ahmad33
            double pt=evt->ElecPtVec_().at(i); // Ahmad33
            double eta=evt->ElecEtaVec_().at(i); // Ahmad33
            double phi=evt->ElecPhiVec_().at(i); // Ahmad33
            double energy=evt->ElecEVec_().at(i); // Ahmad33
	    bool passMini = evt->Electrons_passIso_().at(i);
	    //            double activity_ = evt->MTActivityVec_().at(i);
            double ele_mt_w =utils->calcMT(pt,phi,evt->met(),evt->metphi());  
	    //	    std::cout<<" eventN "<<eventN<<" ele_pt "<<pt<<" ele_eta "<<eta<<endl;
	    if(pt<20 || fabs(eta)>2.1)
	      ElectronFailAcc++;
	    if( passMini && pt> LeptonAcceptance::muonPtMin()  && fabs(eta)< LeptonAcceptance::muonEtaMax()  ){
	      ElectronPassAcc++;
	      temp4vec.SetPtEtaPhiE(pt,eta,phi,energy);
              if(utils2::applyMT){
		ElectronPassmT++;
                if(ele_mt_w < 100. ){vec_recoElectron4vec.push_back(temp4vec);
		  //vec_MTActivity.push_back(activity_);
		}
              }
              else {vec_recoElectron4vec.push_back(temp4vec);
		//	vec_MTActivity.push_back(activity_);
	      }
              vec_recoEleMTW.push_back(ele_mt_w); 
            }
          }
	 
 
        }
	//std::cout<<" vec_recoMuMTW "<<vec_recoMuMTW.size()<<" vec_recoEleMTW "<<vec_recoEleMTW.size()<<endl;

	
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


        if(vec_recoMuMTW.size()>0)muMtW = vec_recoMuMTW[0]; 
        muMtWHist->Fill(muMtW,eventWeight);
	
        //if( template_nMuons == 1 && template_nElectrons == 0 ) {
        bool pass1=false;
	bool MuEvt=false;
	bool EleEvt=false;
	double GetMuPt=900;
	double GetMuEta=4.;
	double GetMuPhi=3.4;
	double GetElePt=900;
	double GetEleEta=4.;
	double GetElePhi=3.4;

        if(TauHadModel>=2){

	  if(vec_recoMuon4vec.size() == 1 && vec_recoElectron4vec.size() ==0){
	    MuEvt=true;
	    GetMuPt=vec_recoMuon4vec[0].Pt();
	    GetMuEta=vec_recoMuon4vec[0].Eta();
	    GetMuPhi=vec_recoMuon4vec[0].Phi();
	    MuonCSevt++;
	    //	    std::cout<<" eventN "<<eventN<<" this is muon event "<<endl;
	  }
	  if(vec_recoElectron4vec.size() == 1 && vec_recoMuon4vec.size() ==0){
	    EleEvt=true;
	    GetElePt=vec_recoElectron4vec[0].Pt();
	    GetEleEta=vec_recoElectron4vec[0].Eta();
	    GetElePhi=vec_recoElectron4vec[0].Phi();
	    ElectronCSevt++;
	    //std::cout<<" eventN "<<eventN<<" this is electron event "<<endl;
	  }
	  if(MuEvt || EleEvt){
	    pass1=true;
	    OneLepCSEvt++;
	    //std::cout<<" eventN "<<eventN<<" pass1 "<<pass1<<endl;
	  }
	} // Number of reco-level muon=1

	if(utils2::UseHEMEvtMap){
	  h_HT_SearchStat->Fill(evt->ht(),eventWeight);
	  h_MHT_SearchStat->Fill(evt->mht(),eventWeight);
	  h_NJet_SearchStat->Fill(evt->nJets(),eventWeight); 
	  h_NBTag_SearchStat->Fill(evt->nBtags(),eventWeight); 
	  h_MET_SearchStat->Fill(evt->met(),eventWeight);
	  
	  h_HT_SearchStat_TwoD->Fill(NomHT,evt->ht(),eventWeight);
	  h_MHT_SearchStat_TwoD->Fill(NomMHT,evt->mht(),eventWeight);
	  h_NJet_SearchStat_TwoD->Fill(NomNJet,evt->nJets(),eventWeight);
	  h_NBTag_SearchStat_TwoD->Fill(NomNBtag,evt->nBtags(),eventWeight);
	  h_MET_SearchStat_TwoD->Fill(NomMET,evt->met(),eventWeight);
	  
	  DeltaPhi_j1MHT_SearchStat->Fill(evt->deltaPhi1(),eventWeight);
	  DeltaPhi_j2MHT_SearchStat->Fill(evt->deltaPhi2(),eventWeight);
	  DeltaPhi_j3MHT_SearchStat->Fill(evt->deltaPhi3(),eventWeight);
	  DeltaPhi_j4MHT_SearchStat->Fill(evt->deltaPhi4(),eventWeight);
	  
	  DeltaPhi_TwoD_j1MHT_SearchStat->Fill(NomDphiOne,evt->deltaPhi1(),eventWeight);
	  MHTPhi_TwoD_SearchStat->Fill(NomMHTPhi,evt->mhtphi(),eventWeight);
	  
	  MuonCS_TwoD_pt->Fill(NomMuPt,GetMuPt,eventWeight);
	  MuonCS_TwoD_eta->Fill(NomMuEta,GetMuEta,eventWeight);
	  MuonCS_TwoD_phi->Fill(NomMuPhi,GetMuPhi,eventWeight);
	  
	  
	  ElectronCS_TwoD_pt->Fill(NomElePt,GetElePt,eventWeight);
	  ElectronCS_TwoD_eta->Fill(NomEleEta,GetEleEta,eventWeight);
	  ElectronCS_TwoD_phi->Fill(NomElePhi,GetElePhi,eventWeight);
	  
	  if(MuEvt){
	    MuonCS_pt->Fill(vec_recoMuon4vec[0].Pt(),eventWeight);
	    MuonCS_eta->Fill(vec_recoMuon4vec[0].Eta(),eventWeight);
	    MuonCS_phi->Fill(vec_recoMuon4vec[0].Phi(),eventWeight);
	  }
	  
	  if(EleEvt){
	    ElectronCS_pt->Fill(vec_recoElectron4vec[0].Pt(),eventWeight);
	    ElectronCS_eta->Fill(vec_recoElectron4vec[0].Eta(),eventWeight);
	    ElectronCS_phi->Fill(vec_recoElectron4vec[0].Phi(),eventWeight);
	    
	  }
	}
	
	if(utils2::UseHEMEvtMap){
	  if(evt->JetsEtaVec_().size()>0)
	    j1_pt->Fill(evt->JetsPtVec_()[0],eventWeight);
	  if(evt->JetsEtaVec_().size()>1)
	    j2_pt->Fill(evt->JetsPtVec_()[1],eventWeight);
	  if(evt->JetsEtaVec_().size()>2)
	    j3_pt->Fill(evt->JetsPtVec_()[2],eventWeight);
	  if(evt->JetsEtaVec_().size()>3)
	    j4_pt->Fill(evt->JetsPtVec_()[3],eventWeight);
	      
	  if(evt->JetsEtaVec_().size()>0)
	    j1_eta->Fill(evt->JetsEtaVec_()[0],eventWeight);
	  if(evt->JetsEtaVec_().size()>1)
	    j2_eta->Fill(evt->JetsEtaVec_()[1],eventWeight);
	  if(evt->JetsEtaVec_().size()>2)
	    j3_eta->Fill(evt->JetsEtaVec_()[2],eventWeight);
	  if(evt->JetsEtaVec_().size()>3)
	    j4_eta->Fill(evt->JetsEtaVec_()[3],eventWeight);
	  if(evt->JetsEtaVec_().size()>0)
	    j1_phi->Fill(evt->JetsPhiVec_()[0],eventWeight);
	  if(evt->JetsEtaVec_().size()>1)
	    j2_phi->Fill(evt->JetsPhiVec_()[1],eventWeight);
	  if(evt->JetsEtaVec_().size()>2)
	    j3_phi->Fill(evt->JetsPhiVec_()[2],eventWeight);
	  if(evt->JetsEtaVec_().size()>3)
	    j4_phi->Fill(evt->JetsPhiVec_()[3],eventWeight);

	  for(unsigned j = 0; j < evt->JetsPtVec_().size(); ++j){
	    if(evt->JetsPtVec_()[j] > 30. && fabs(evt->JetsEtaVec_()[j]) < 5.0){ 
	      j_pt_inclusive->Fill(evt->JetsPtVec_()[j],eventWeight);
	      j_eta_inclusive->Fill(evt->JetsEtaVec_()[j],eventWeight);
	      j_phi_inclusive->Fill(evt->JetsPhiVec_()[j],eventWeight);
	    }
	  }
	}










	//std::cout<<" eventN "<<eventN<<endl;
	if(pass1){ // pass muon selection
	  //std::cout<<" eventN "<<eventN<<" ****** seg vio****** "<<endl;
	
  /*
          muPt = vec_recoMuon4vec[0].Pt();
          muEta = vec_recoMuon4vec[0].Eta();
          muPhi = vec_recoMuon4vec[0].Phi();
          muE  = vec_recoMuon4vec[0].E();
*/
	  //double activity= utils->MuActivity(muEta,muPhi,evt->JetsPtVec_(),evt->JetsEtaVec_(),evt->JetsPhiVec_(),evt->Jets_chargedEmEnergyFraction_(),evt->Jets_chargedHadronEnergyFraction_());
	  
	  //          double activity=vec_MTActivity[0]; 

	  //          if(verbose!=0)cout << " activity: " << activity << endl;
        
	  //    cutflow_preselection->Fill(7.,eventWeight); // 1-mu selection
   
	  bool pass1_1=false;

	  // for veto we have a lepton collection with softer requirements. pT > 10 not 20 and eta < 2.4 not 2.1 and also there is no 
	  // mT cut applied. 
	  /*	  
	  if(TauHadModel>=2){if(evt->MuPtVec_().size()>1    || evt->ElecPtVec_().size()>1)pass1_1=true;}    // N(reco-muon)<=1, N(reco-ele)=0, 
	  if(pass1_1){
	    //	    std::cout<<" e/ mu > 1, skip evt "<<endl;
	    continue;
	  }

*/	  
	  if(TauHadModel>=2){
	    int nGetLepPassIso=evt->nElectrons()+evt->nMuons();
	    if(nGetLepPassIso !=1)
	      pass1_1=true;
	  }    // N(reco-muon)<=1, N(reco-ele)=0, 
	  if(pass1_1){
	    //	    std::cout<<" e/ mu > 1, skip evt "<<endl;
	    continue;
	  }
	  
	  PassNoExtraEleOrMuWithIsoApplied++;
	  //cutflow_preselection->Fill(8.,eventWeight); // Lepton vetos

          // The muon we are using is already part of a jet. (Note: the muon is isolated by 0.2 but jet is much wider.) And,
          // its momentum is used in HT and MHT calculation. We need to subtract this momentum and add the contribution from the simulated tau jet.

          //Identify the jet containing the muon
          const double deltaRMax = 0.4; // 0.4 is delR of jet

          int JetIdx=-1;
          GenRecMu_all++;

          // If muon does not match a GenMuon, drop the event. We do this by applying some corrections 
          int GenMuIdx=-1;
	  /*
          if(!isData && !utils->findMatchedObject(GenMuIdx,muEta,muPhi,evt->GenMuPtVec_(),evt->GenMuEtaVec_(),evt->GenMuPhiVec_(),deltaRMax,verbose)){
            GenRecMu_fail++;
            if(evt->DataBool_()==false && eventN < 100){
              printf(" Warning! There is no Gen Muon \n ");
              printf("@@@@@@@@@@@@@@@@@@\n eventN: %d \n MuPt: %g MuEta: %g MuPhi: %g \n ",eventN,muPt,muEta,muPhi);
            }
            
          }
*/
          // start of bootstrapping ( if is on ) 
          for(int l=1; l<=nLoops;l++){

            // start of btag on/off for had tau jets ( if is on ) 
            for(int m=0; m<nBtagsForHadTau;m++){
	     
	      //	      if( sel->ht_base(evt->ht()) && sel->mht_base(evt->mht()) && sel->Njet_base(evt->nJets()) ){
	      bool FoundHEMiss_Phi_2Pt38_2Pt72 =false;
	      bool FoundHEMiss_Phi_Minus1_Minus1Pt4=false;
	      
	      if(utils2::GetHEMEvtMap){
		    //		    std::cout<<" evt "<<evt->Evtnum()<<" run "<<evt->Runnum()<<" lumi "<<evt->LumiBlocknum()<<" eta "<<evt->JetsEtaVec_()[i]<<" phi "<<evt->JetsPhiVec_()[i]<<endl;
		    EventNHEMiss.push_back(evt->Evtnum());
		    RunNHEMiss.push_back(evt->Runnum());
		    LumiBlockNHEMiss.push_back(evt->LumiBlocknum());
		    MuPtHEMiss.push_back(GetMuPt);
		    MuEtaHEMiss.push_back(GetMuEta);
		    MuPhiHEMiss.push_back(GetMuPhi);
		    ElePtHEMiss.push_back(GetElePt);
		    EleEtaHEMiss.push_back(GetEleEta);
		    ElePhiHEMiss.push_back(GetElePhi);
		    NJetHEMiss.push_back(evt->nJets());
		    NBTagHEMiss.push_back(evt->nBtags());
		    HTHEMiss.push_back(evt->ht());
		    MHTHEMiss.push_back(evt->mht());
		    MHTPhiHEMiss.push_back(evt->mhtphi());
		    DPhiOneHEMiss.push_back(evt->deltaPhi1());
		    METHEMiss.push_back(evt->met());
		    //EventNRunN.emplace(evt->Evtnum(),evt->Runnum());
		    //*AR: 180801: creates a map of (evt, run, lumiblock)
		    
		    EventNRunN[evt->Evtnum()]=evt->Runnum();
		    EventNRunNLumiN[evt->Evtnum()][evt->Runnum()]=evt->LumiBlocknum();

		    EventNRunNLumiNMET[std::make_pair(std::make_pair(evt->Evtnum(),evt->Runnum()),evt->LumiBlocknum())]=evt->met();

		    MuEtaEleEtaMET[std::make_pair(GetMuEta,GetEleEta)]=evt->met();
		    HTMHTMET[std::make_pair(evt->ht(),evt->mht())]=evt->met();
		    
		    FinalMap[std::make_pair(std::make_pair(std::make_pair(evt->Evtnum(),evt->Runnum()),evt->LumiBlocknum()),std::make_pair(std::make_pair(std::make_pair(GetMuPt,GetElePt),std::make_pair(std::make_pair(GetMuEta,GetEleEta),std::make_pair(GetMuPhi,GetElePhi))),std::make_pair(std::make_pair(evt->nJets(),evt->nBtags()),std::make_pair(std::make_pair(evt->ht(),evt->mht()),std::make_pair(evt->mhtphi(),evt->deltaPhi1())))))]=evt->met();
		    //		    std::cout<<" seg vio "<<endl;

		    

		    //LumiNEventNRunN.emplace(evt->LumiBlocknum(),EventNRunN);
		    //		    int Runj=evt->Runnum();
		    
		    
		    //EventNLumiN[evt->Evtnum()]=evt->LumiBlocknum();
		    //		    EventNRunNLumiN[evt->Evtnum(),evt->Runnum()]=evt->LumiBlocknum();
		    //LumiNRootFile[evt->LumiBlocknum()]=InRootList.c_str();
		    
	      }

	      //	      std::cout<<" eventN "<<eventN<<" ht "<<evt->ht()<<" mht "<<evt->mht()<<" njet "<<evt->nJets()<<" btag "<<evt->nBtags()<<" met "<<evt->met()<<endl;
	      
	      //	      }
	      
	      
	      
 //#############################################################
              //New #Jet
	      //              else if(JetIdx!=-1 && evt->csvVec()[JetIdx]>evt->csv_())NewNB=evt->nBtags();
             
	      /*
	      if(newNJet>=5 && newNJet<=6 && newHT >= 500 && newHT <1000 && newMHT>=350 && newMHT<500 && m==0){
		
	//*AR-Nov8,2016-Check dR between simulated tau jet and other jets
		if(slimJetIdx!=-1){
		  double taujet_eta=NewTauJet3Vec.Eta();
		  double taujet_pt=NewTauJet3Vec.Pt(); 
		  double taujet_phi=NewTauJet3Vec.Phi();
		  //	      std::cout<<" eventN "<<eventN<<" slimJetIdx "<<slimJetIdx<<" taujet_eta "<<taujet_eta<<" taujet_phi "<<taujet_phi<<" num_slimjets "<<evt->slimJetPtVec_().size()<<endl;
		  for(int i=0;i<evt->slimJetPtVec_().size();i++){
		    if(i!=slimJetIdx){
		      double jet_eta=evt->slimJetEtaVec_()[i];
		      double jet_phi=evt->slimJetPhiVec_()[i];
		      double delta_eta_square=(taujet_eta-jet_eta)*(taujet_eta-jet_eta);
		      double delta_phi_square=(taujet_phi-jet_phi)*(taujet_phi-jet_phi);
		      double dR_taujet_jet=sqrt(delta_eta_square + delta_phi_square);
		      if(dR_taujet_jet<0.4)
			std::cout<<" eventN "<<eventN<<" l "<<l<<" tauJetIdx "<<slimJetIdx<<" taujet_pt "<<taujet_pt<<" taujet_eta "<<taujet_eta<<" taujet_phi "<<taujet_phi<<" num_slimjets "<<evt->slimJetPtVec_().size()<<" jetIdx "<<i<<" jet_eta "<<jet_eta<<" jet_phi "<<jet_phi<<" dR "<<dR_taujet_jet<<endl;
		      
			
		    }
		  }
		}
	      }	      
	      
	      
	     

	      if(newNJet>=5 && newNJet<=6 && newHT >= 350 && newHT <500 && m==0){
		//		std::cout<<" newMHT "<<newMHT<<endl;
		if(newMHT>=350 && newMHT<500){
		  std::cout<<" eventN "<<eventN<<" l"<<l<<"old jets "<<evt->nJets()<<" newNJet "<<newNJet<<" mueta "<<muEta<<" muphi "<<muPhi<<" mupt "<< muPt<<" simTauJetPt_xy "<<simTauJetPt_xy<<" simTauJetPhi_xy "<<simTauJetPhi_xy<<" dPhi "<<simTauJetPhi_xy-muPhi<<endl;
		  std::cout<<"*********Fall in bin*******"<<endl;
		}
	      }
	      */ 
	      // 
	      // newNJet,newHT,newMHT,newNB fully ready
	      // 
	      bool PassIso2=false;
              // Apply MT efficiency
              // applyIsoTrk here 
              if(utils2::applyIsoTrk){

                // Determine which Iso model is chosen
                // and apply the neccessary cuts 
                if(utils2::IsoTrkModel==0)PassIso2=true;
	
              }

        
            } // end of Btag bootstrap
          } // end of bootstrapping

        } // End if exactly one muon
	
      } // loop for uncertainties
    } // end of loop over events
    // calculate muon_jet match failure and write the histograms
    std::cout<<" end of loop over evt "<<" eventN "<<eventN<<" HEMMapMatch "<<HEMMapMatch<<" PassHBHEIsoNoiseFilter "<<PassHBHEIsoNoiseFilter<<" PassHBHENoiseFilter "<<PassHBHENoiseFilter<<" PassecalBadCalibFilter "<<PassecalBadCalibFilter<<" PassEcalDeadCellTriggerPrimitiveFilter "<<PassEcalDeadCellTriggerPrimitiveFilter<<" PassBadChargedCandidateFilter "<<PassBadChargedCandidateFilter<<" PassBadPFMuonFilter "<<PassBadPFMuonFilter<<" PassglobalSuperTightHalo2016Filter "<<PassglobalSuperTightHalo2016Filter<<" PassPFCaloMETRatioFilter "<<PassPFCaloMETRatioFilter<<" PassnoMuonJet "<<PassnoMuonJet<<" PassNVtx "<<PassNVtx<<" PassJetId "<<PassJetId<<" nCleanEve "<<nCleanEve<<" TriggerPass "<<TriggerPass<<" noLepEvt "<<noLepEvt<<" OneMuEvt "<< OneMuEvt<<" OneEleEvt "<<OneEleEvt<<" MuonsFailAcc "<< MuonFailAcc<<" MuonPassAcc "<<MuonPassAcc<<" MuonPassmT "<<MuonPassmT<<" MuonCSevt "<<MuonCSevt<<" ElectronFailAcc "<< ElectronFailAcc<<" ElectronPassAcc "<<ElectronPassAcc<<" ElectronPassmT "<<ElectronPassmT<<" ElectronCSevt "<<ElectronCSevt<<" OneLepCSEvt "<<OneLepCSEvt<<" PassNoExtraEleOrMuWithIsoApplied "<<PassNoExtraEleOrMuWithIsoApplied<<endl;
    /*
    for (map<int,int>::iterator itr=EventNRunN.begin(); itr != EventNRunN.end(); ++itr){
      //if(itr->second == itt->first)
      std::cout<<" itr->first "<<itr->first<<" itr->second "<<itr->second<<" lumi "<<endl;
    }

*/

    std::cout<<" size_EventNRunNLumiN "<<EventNRunNLumiN.size()<<endl;
    std::cout<<" size_EventNRunN "<<EventNRunN.size()<<endl;


    //*AR: 180801: Saves map(event, run, lumi) to a text file
     if(utils2::GetHEMEvtMap){
      ofstream newFile("Map.txt");

      for(std::map<std::pair<std::pair<std::pair<int,int>, int>,std::pair<std::pair<std::pair<double,double>,std::pair<std::pair<double,double>,std::pair<double,double>>>,std::pair<std::pair<double,double>,std::pair<std::pair<double,double>,std::pair<double,double>>>>>,double>::iterator itr2=FinalMap.begin(); itr2 !=FinalMap.end();itr2++){

	newFile<< itr2->first.first.first.first<<"|"<<itr2->first.first.first.second<<"|"<<itr2->first.first.second<<"|"<<itr2->first.second.first.first.first<<"|"<<itr2->first.second.first.first.second<<"|"<<itr2->first.second.first.second.first.first<<"|"<<itr2->first.second.first.second.first.second<<"|"<<itr2->first.second.first.second.second.first<<"|"<<itr2->first.second.first.second.second.second<<"|"<<itr2->first.second.second.first.first<<"|"<<itr2->first.second.second.first.second<<"|"<<itr2->first.second.second.second.first.first<<"|"<<itr2->first.second.second.second.first.second<< "|"<<itr2->first.second.second.second.second.first<<"|"<<itr2->first.second.second.second.second.second<<"|"<<itr2->second<<endl;
       
      }


     newFile.close();
     std::cout<<" now print vector elements "<<endl;
     /*
     for(int i=0;i<EventNHEMiss.size();i++){
       std::cout<<" evt "<< EventNHEMiss[i]<<" run "<<RunNHEMiss[i]<<" lumi "<<LumiBlockNHEMiss[i]<<" muPt "<<MuPtHEMiss[i]<<" elePt "<<ElePtHEMiss[i]<<" muEta "<<MuEtaHEMiss[i]<<" eleEta "<<EleEtaHEMiss[i]<<" muPhi "<<MuPhiHEMiss[i]<<" elePhi "<<ElePhiHEMiss[i]<<" njet "<<NJetHEMiss[i]<<" btag "<<NBTagHEMiss[i]<<" ht "<<HTHEMiss[i]<<" mht "<<MHTHEMiss[i]<<" mhtphi "<<MHTPhiHEMiss[i]<<" dphi1 "<<DPhiOneHEMiss[i]<<" met "<<METHEMiss[i]<<endl;
     }
*/
     }
    /*
    for(std::map<int,std::map<int,int>>::iterator itr2 = EventNRunNLumiN.begin(); itr2 != EventNRunNLumiN.end(); itr2++){

      for (map<int,int>::iterator itr=itr2->first.begin(); itr != itr2->first.end(); ++itr){
	//if(itr->second == itt->first)
	std::cout<<" evt "<<itr->first<<" run "<<itr->second<<" lumi "<<itr2->second<<endl;
      }
      
    }
    
*/
    /*
    for(int i=0;i<EventNHEMiss_Phi_Minus1_Minus1Pt4.size();i++){
      std::cout<<"EVT "<<EventNHEMiss_Phi_Minus1_Minus1Pt4[i]<<" Run "<<RunNHEMiss_Phi_Minus1_Minus1Pt4[i]<< " lumi "<< LumiBlockNHEMiss_Phi_Minus1_Minus1Pt4[i]<<endl;
    }
*/




    // open a file to write the histograms
    sprintf(tempname,"%s/HadTauEstimation_%s_%s.root",Outdir.c_str(),subSampleKey.c_str(),inputnumber.c_str());
    TFile *resFile = new TFile(tempname, "RECREATE");
    muMtWHist->Write();
    cutflow_preselection->Write();
    searchH->Write();
    searchH_nb_njet2->Write();
    h_HT_SearchStat->Write();
    h_MHT_SearchStat->Write();
    h_MET_SearchStat->Write();
    h_NJet_SearchStat->Write();
    h_NBTag_SearchStat->Write(); 

    h_HT_SearchStat_TwoD->Write();
    h_MHT_SearchStat_TwoD->Write();
    h_MET_SearchStat_TwoD->Write();
    h_NJet_SearchStat_TwoD->Write();
    h_NBTag_SearchStat_TwoD->Write(); 

    MuonCS_pt->Write();
    MuonCS_eta->Write();
    MuonCS_phi->Write();

    MuonCS_TwoD_pt->Write();
    MuonCS_TwoD_eta->Write();
    MuonCS_TwoD_phi->Write();

    //    EventNHEMiss->Write();
    ElectronCS_TwoD_pt->Write();
    ElectronCS_TwoD_eta->Write();
    ElectronCS_TwoD_phi->Write();

    //    EventNRunN.Write();
    j1_pt->Write();
    j2_pt->Write();
    j3_pt->Write();
    j4_pt->Write();
    j_pt_inclusive->Write();

    j1_eta->Write();
    j2_eta->Write();
    j3_eta->Write();
    j4_eta->Write();
    j_eta_inclusive->Write();
    j1_phi->Write();
    j2_phi->Write();
    j3_phi->Write();
    j4_phi->Write();
    j_phi_inclusive->Write();

    DeltaPhi_j1MHT_SearchStat->Write();
    DeltaPhi_j2MHT_SearchStat->Write();
    DeltaPhi_j3MHT_SearchStat->Write();
    DeltaPhi_j4MHT_SearchStat->Write();


    DeltaPhi_TwoD_j1MHT_SearchStat->Write(); 
    MHTPhi_TwoD_SearchStat->Write();

    DeltaPhi_j1MET_SearchStat->Write();
    DeltaPhi_j2MET_SearchStat->Write();
    DeltaPhi_j3MET_SearchStat->Write();
    DeltaPhi_j4MET_SearchStat->Write();


    // Loop over different event categories (e.g. "All events, Wlnu, Zll, Zvv, etc")
   
    
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

  


