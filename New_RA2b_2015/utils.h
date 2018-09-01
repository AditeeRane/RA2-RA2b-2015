#include "TH1.h"
#include "TH2.h"
#include "TVector2.h"
#include "TVector3.h"
#include <cstdio>
#include <iostream>
#include "TString.h"
#include "TGraphAsymmErrors.h"
#include <cmath>
#include <string>
#include <vector>
#include "TLorentzVector.h"
using namespace std;

class Utils{

    int TauResponse_nBins;

public:


    Utils();

    void TauResponse_checkPtBin(unsigned int ptBin);

    TString TauResponse_name(unsigned int ptBin);
    TString NJetVsPt_name(unsigned int ptBin);
    TString TauPt_name(unsigned int ptBin);
    TString RecoMuPt_name(unsigned int ptBin);
    TString GenMuPt_name(unsigned int ptBin);

    TString TauPtEta_name(unsigned int ptBin);

    double deltaPhi(double phi1, double phi2);

    double deltaR(double eta1, double eta2, double phi1, double phi2);

    bool findMatchedObject(int &matchedObjIdx,double genTauEta, double genTauPhi, vector<double> PtVec,vector<double> EtaVec, vector<double> PhiVec,double deltaRMax, int ver);

    bool RelPt(double genTauPt, double IsoPt,double relPt);

    int TauResponse_nBins_();

    unsigned int TauResponse_ptBin(double pt);

    double getRandom(double muPt_,vector<TH1*> vec_resp);

    double MuActivity(double muEta, double muPhi, vector<double> JetPtVec,vector<double> JetEtaVec, vector<double> JetPhiVec,vector<double> JetChargedEmEnergyFraction, vector<double> JetChargedHadronEnergyFraction);

    vector<TVector3> Order_the_Vec(vector<TVector3> vec);
    vector<double> Order_the_Vec(vector<TVector3> vec,vector<double> vecTwo);
    vector<bool> Order_the_Vec(vector<TVector3> vec,vector<bool> vecTwo);
    vector<int> Order_the_Vec(vector<TVector3> vec,vector<int> vecTwo);
    vector<TLorentzVector> Order_the_Vec(vector<TVector3> vec,vector<TLorentzVector> vecTwo);
    void getRandom2(double muPt_,vector<TH2*> vec_resp_xy,Double_t & scale_x,Double_t & scale_y);
    
    double GetBinValue(double muPt_,vector<TH1*> vec_resp, int l );

    TH1D * GetBinxProj(TH2D * h2tau_phi, int binx);   
    double GetTotProjArea(TH1D * ProjY);

    double GetBinProjArea(TH1D * ProjY, int p);

    double GetBinContent(double muPt_,vector<TH1*> vec_resp, int l );
    
    double GetBinWidth(double muPt_,vector<TH1*> vec_resp, int l );

    double calcMT(const double objPt, const double objPhi, const double met, const double metphi ) ;

    int MaxMuPt();
    int NMuPtBins();   
 
    double tau_Phi_lowX();
    double tau_Phi_lowY();
    double tau_Phi_upX();
    double tau_Phi_upY();
    double tau_Phi_nbinX();
    double tau_Phi_nbinY();

    int tau_phi_GetBinX(double x);

    vector<string> skimInput(string mom);
};

