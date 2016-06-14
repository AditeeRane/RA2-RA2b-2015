#ifndef SELECTION_H
#define SELECTION_H

#include <map>
#include <string>
//
using namespace std;

class Selection {

  bool applyIsoTrk;
  bool addTrig;
  map<int, string> cutname;
  
public:


  // constructor 
  Selection();

  map<int, string> cutName() const;

  //define different cuts here
  bool ht_500(double ht_){if(ht_>=300) return true; return false;}
  bool ht_500_800(double ht_){if(ht_>=500 && ht_<800) return true; return false;}
  bool ht_500_1200(double ht_){if(ht_>=500 && ht_<1200)return true; return false;}
  bool ht_800_1200(double ht_){if(ht_>=800 && ht_<1200)return true; return false;}
  bool ht_800(double ht_){if(ht_>=800)return true; return false;}
  bool ht_1200(double ht_){if(ht_>=1200)return true; return false;}
  bool mht_200(double mht_){if(mht_>=250)return true; return false;}
  bool mht_500(double mht_){if(mht_>=500)return true; return false;}
  bool mht_200_500(double mht_){if(mht_>=200 && mht_<500)return true; return false;}
  bool mht_500_750(double mht_){if(mht_>=500 && mht_<750)return true; return false;}
  bool mht_750(double mht_){if(mht_>=750)return true; return false;}

  bool dphi(int njet,double dPhi0, double dPhi1, double dPhi2, double dPhi3){
    if(njet>=4 && dPhi0>0.5 && dPhi1>0.5 && dPhi2>0.3 && dPhi3>0.3)return true; 
    if(njet==3 && dPhi0>0.5 && dPhi1>0.5 && dPhi2>0.3)return true;
    if(njet==2 && dPhi0>0.5 && dPhi1>0.5)return true;
    return false;
  }
  bool low_dphi(int njet,double dPhi0, double dPhi1, double dPhi2, double dPhi3){
    if(njet>=4 && !(dPhi0>0.5 && dPhi1>0.5 && dPhi2>0.3 && dPhi3>0.3))return true;
    if(njet==3 && !(dPhi0>0.5 && dPhi1>0.5 && dPhi2>0.3))return true;
    if(njet==2 && !(dPhi0>0.5 && dPhi1>0.5))return true;
    return false;
  }

//  bool dphi(double minDeltaPhiN_){if(minDeltaPhiN_ > 6.)return true; return false;}
//  bool dphi(double minDeltaPhiN_){if(minDeltaPhiN_ > 4.)return true; return false;} // Ahmad33



  bool nolep(int nLeptons_){if(nLeptons_==0)return true; return false;}
  bool Njet_4(int nJets_){if(nJets_ >= 3)return true; return false;}
  bool Njet_2(int nJets_){if(nJets_ == 2)return true; return false;}
  bool Njet_4_6(int nJets_){if(nJets_ >= 4 && nJets_ <= 6)return true; return false;}
  bool Njet_7_8(int nJets_){if(nJets_ >= 7 && nJets_ <= 8)return true; return false;}
  bool Njet_9(int nJets_){if(nJets_ >= 9)return true; return false;}
  bool btag_0(int nBtags_){if(nBtags_ == 0)return true; return false;}
  bool btag_1(int nBtags_){if(nBtags_ == 1)return true; return false;}
  bool btag_2(int nBtags_){if(nBtags_ == 2)return true; return false;}
  bool btag_3(int nBtags_){if(nBtags_ >= 3)return true; return false;}
//  bool isoTrk(int nIso_){if(nIso_ ==0)return true; return false;}



  bool ElecIsoTrk(int nElecIso_);
  bool MuIsoTrk(int nMuIso_);
  bool PionIsoTrk(int nPionIso_);


//  bool checkcut(string ss,double ht_,double mht_,double minDeltaPhiN_,int nJets_,int nBtags_,int nLeptons_,int nElecIso_,int nMuIso_,int nPionIso_);
//  bool checkcut_HadTau(string ss,double ht_,double mht_,double minDeltaPhiN_,int nJets_,int nBtags_,int nLeptons_,int nElecIso_,int nMuIso_,int nPionIso_ );
  bool checkcut(string ss,double ht_,double mht_,double dphi0,double dphi1,double dphi2,double dphi3,int nJets_,int nBtags_,int nLeptons_,int nElecIso_,int nMuIso_,int nPionIso_);
  bool checkcut_HadTau(string ss,double ht_,double mht_,double dphi0,double dphi1,double dphi2,double dphi3,int nJets_,int nBtags_,int nLeptons_,int nElecIso_,int nMuIso_,int nPionIso_ );

  bool applyIsoTrk_() const ;

};



#endif

