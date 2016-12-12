#include "Selection.h"  

  // Constructor
  Selection::Selection(){

    applyIsoTrk = true;

    //initialize a map between string=cutnames and histvecs. copy one histvec into all of them. The histograms, though, will be filled differently.
    //cutname[0]="PreSel";cutname[1]="nolep";cutname[2]="Njet_base";cutname[3]="ht_base";
    cutname[0]="delphi";
    cutname[1]="low_Dphi";

    //    cutname[0]="mht_base";
    //cutname[5]="isoMu";cutname[6]="isoElec";
    //cutname[1]="isoPion";
    //cutname[2]="delphi";
    //cutname[3]="NJet2";
    //cutname[4]="NJet34";
    //cutname[5]="NJet56";
    //cutname[6]="NJet7up";
    //cutname[7]="NJet34_MHT350";
    //cutname[8]="NJet56_MHT350";
    //cutname[9]="delphi_Only2Jet";
    //cutname[10]="isotrk_Only2Jet";
    //cutname[11]="mht_Only2Jet";
    //cutname[9]="low_Dphi";
    //cutname[10]="delphi_NoIso"; cutname[11]="mht_500";
    //cutname[12]="J46_HT5001200_MHT500750";
    //cutname[13]="Njet_9";
  }

  bool Selection::ElecIsoTrk(int nElecIso_){
    if(!applyIsoTrk) return true;
    else if(nElecIso_ ==0)return true; return false;
  }


  bool Selection::MuIsoTrk(int nMuIso_){
    if(!applyIsoTrk) return true;
    else if(nMuIso_ ==0)return true; return false;
  }


  bool Selection::PionIsoTrk(int nPionIso_){
    if(!applyIsoTrk) return true;
    else if(nPionIso_ ==0)return true; return false;
  }


  ///apply the cuts here


  bool Selection::checkcut(string ss,double ht_,double mht_,double dphi0,double dphi1,double dphi2,double dphi3,int nJets_,int nBtags_,int nLeptons_,int nElecIso_,int nMuIso_,int nPionIso_){



    if(ss== cutname[0]){if(nolep( nLeptons_)&&Njet_base( nJets_)&&ht_base( ht_)&&mht_base( mht_)&&MuIsoTrk( nMuIso_)&&ElecIsoTrk(nElecIso_)&&PionIsoTrk(nPionIso_)&&dphi(nJets_, dphi0,dphi1,dphi2,dphi3))return true;}

    if(ss== cutname[1]){if(nolep( nLeptons_)&&Njet_base( nJets_)&&ht_base( ht_)&&mht_base( mht_)&&MuIsoTrk( nMuIso_)&&ElecIsoTrk(nElecIso_)&&PionIsoTrk(nPionIso_)&&low_dphi(nJets_, dphi0,dphi1,dphi2,dphi3))return true;}





    //if(ss== cutname[0])return true;
    //if(ss== cutname[1]){if(nolep( nLeptons_))return true;}
    //if(ss== cutname[2]){if(nolep( nLeptons_)&&Njet_base(nJets_))return true;}
    //if(ss== cutname[3]){if(nolep( nLeptons_)&&Njet_base(nJets_) &&ht_base( ht_))return true;}
    //if(ss== cutname[0]){if(nolep( nLeptons_)&&Njet_base( nJets_)&&ht_base( ht_)&&mht_base( mht_))return true;}
    //if(ss== cutname[5]){if(nolep( nLeptons_)&&Njet_base( nJets_)&&ht_base( ht_)&&mht_base( mht_)&&MuIsoTrk( nMuIso_))return true;}
    //if(ss== cutname[6]){if(nolep( nLeptons_)&&Njet_base( nJets_)&&ht_base( ht_)&&mht_base( mht_)&&MuIsoTrk( nMuIso_)&&ElecIsoTrk(nElecIso_))return true;}
    //if(ss== cutname[1]){if(nolep( nLeptons_)&&Njet_base( nJets_)&&ht_base( ht_)&&mht_base( mht_)&&MuIsoTrk( nMuIso_)&&ElecIsoTrk(nElecIso_)&&PionIsoTrk(nPionIso_))return true;}
// if(ss== cutname[2]){if(nolep( nLeptons_)&&Njet_base( nJets_)&&ht_base( ht_)&&mht_base( mht_)&&MuIsoTrk( nMuIso_)&&ElecIsoTrk(nElecIso_)&&PionIsoTrk(nPionIso_)&&dphi(nJets_, dphi0,dphi1,dphi2,dphi3))return true;}
//
// if(ss== cutname[3]){if(nolep( nLeptons_)&&ht_base( ht_)&&mht_base( mht_)&&MuIsoTrk( nMuIso_)&&ElecIsoTrk(nElecIso_)&&PionIsoTrk(nPionIso_)&&Njet_2( nJets_))return true;}
// if(ss== cutname[4]){if(nolep( nLeptons_)&&ht_base( ht_)&&mht_base( mht_)&&MuIsoTrk( nMuIso_)&&ElecIsoTrk(nElecIso_)&&PionIsoTrk(nPionIso_)&&Njet_3_4( nJets_))return true;}
// if(ss== cutname[5]){if(nolep( nLeptons_)&&ht_base( ht_)&&mht_base( mht_)&&MuIsoTrk( nMuIso_)&&ElecIsoTrk(nElecIso_)&&PionIsoTrk(nPionIso_)&&Njet_5_6( nJets_))return true;}
// if(ss== cutname[6]){if(nolep( nLeptons_)&&ht_base( ht_)&&mht_base( mht_)&&MuIsoTrk( nMuIso_)&&ElecIsoTrk(nElecIso_)&&PionIsoTrk(nPionIso_)&&Njet_7up( nJets_))return true;}
//
// if(ss== cutname[7]){if(nolep( nLeptons_)&&ht_base( ht_)&&mht_350( mht_)&&MuIsoTrk( nMuIso_)&&ElecIsoTrk(nElecIso_)&&PionIsoTrk(nPionIso_)&&Njet_3_4( nJets_))return true;}
// if(ss== cutname[8]){if(nolep( nLeptons_)&&ht_base( ht_)&&mht_350( mht_)&&MuIsoTrk( nMuIso_)&&ElecIsoTrk(nElecIso_)&&PionIsoTrk(nPionIso_)&&Njet_5_6( nJets_))return true;}
//
    /*
    if(ss== cutname[9]){if(nolep( nLeptons_)&&Njet_2( nJets_)&&ht_500( ht_)&&mht_200( mht_)&&MuIsoTrk( nMuIso_)&&ElecIsoTrk(nElecIso_)&&PionIsoTrk(nPionIso_)&&dphi(nJets_, dphi0,dphi1,dphi2,dphi3))return true;}
    if(ss== cutname[10]){if(nolep( nLeptons_)&&Njet_2( nJets_)&&ht_500( ht_)&&mht_200( mht_)&&MuIsoTrk( nMuIso_)&&ElecIsoTrk(nElecIso_)&&PionIsoTrk(nPionIso_))return true;}  
  if(ss== cutname[11]){if(nolep( nLeptons_)&&Njet_2( nJets_)&&ht_500( ht_)&& mht_200( mht_))return true;}
    */

    //if(ss== cutname[9]){if(nolep( nLeptons_)&&Njet_4( nJets_)&&ht_500( ht_)&&mht_200( mht_)&&MuIsoTrk( nMuIso_)&&ElecIsoTrk(nElecIso_)&&PionIsoTrk(nPionIso_)&&low_dphi(nJets_, dphi0,dphi1,dphi2,dphi3))return true;}
    //if(ss== cutname[10]){if(nolep( nLeptons_)&&Njet_4( nJets_)&&ht_500( ht_)&&mht_200( mht_)&&dphi(nJets_, dphi0,dphi1,dphi2,dphi3))return true;}
    //if(ss== cutname[11]){if(nolep( nLeptons_)&&Njet_4( nJets_)&&ht_500( ht_)&&mht_500( mht_))return true;}
    //if(ss== cutname[12]){if(nolep( nLeptons_)&&Njet_4_6( nJets_)&&ht_500_1200( ht_)&&mht_500_750( mht_))return true;}
    //if(ss== cutname[13]){if(nolep( nLeptons_)&&Njet_9( nJets_)&&ht_500( ht_)&&mht_200( mht_)&&MuIsoTrk( nMuIso_)&&ElecIsoTrk(nElecIso_)&&PionIsoTrk(nPionIso_)&&dphi(nJets_, dphi0,dphi1,dphi2,dphi3))return true;}
  return false;
  }

  bool Selection::checkcut_HadTau(string ss,double ht_,double mht_,double dphi0,double dphi1,double dphi2,double dphi3,int nJets_,int nBtags_,int nLeptons_,int nElecIso_,int nMuIso_,int nPionIso_){
    if(ss== cutname[0]){if(Njet_base( nJets_)&&ht_base( ht_)&&mht_base( mht_)&&dphi(nJets_, dphi0,dphi1,dphi2,dphi3))return true;}
    if(ss== cutname[1]){if(Njet_base( nJets_)&&ht_base( ht_)&&mht_base( mht_)&&low_dphi(nJets_, dphi0,dphi1,dphi2,dphi3))return true;}
    //if(ss== cutname[0])return true;
    //if(ss== cutname[1]){return true;}
    //if(ss== cutname[2]){if(Njet_base( nJets_))return true;}
    //if(ss== cutname[3]){if(Njet_base( nJets_)&&ht_base( ht_))return true;}
    //    if(ss== cutname[0]){if(Njet_base( nJets_)&&ht_base( ht_)&&mht_base( mht_))return true;}
    //if(ss== cutname[5]){if(Njet_base( nJets_)&&ht_base( ht_)&&mht_base( mht_))return true;}
    //if(ss== cutname[6]){if(Njet_base( nJets_)&&ht_base( ht_)&&mht_base( mht_))return true;}
    //if(ss== cutname[1]){if(Njet_base( nJets_)&&ht_base( ht_)&&mht_base( mht_))return true;}
    //if(ss== cutname[2]){if(Njet_base( nJets_)&&ht_base( ht_)&&mht_base( mht_)&&dphi(nJets_, dphi0,dphi1,dphi2,dphi3))return true;}
    //if(ss== cutname[3]){if(ht_base( ht_)&&mht_base( mht_)&&Njet_2( nJets_))return true;}
    //if(ss== cutname[4]){if(ht_base( ht_)&&mht_base( mht_)&&Njet_3_4( nJets_))return true;}
    //if(ss== cutname[5]){if(ht_base( ht_)&&mht_base( mht_)&&Njet_5_6( nJets_))return true;}
    //if(ss== cutname[6]){if(ht_base( ht_)&&mht_base( mht_)&&Njet_7up( nJets_))return true;}
    //
    //if(ss== cutname[7]){if(ht_base( ht_)&&mht_350( mht_)&&Njet_3_4( nJets_))return true;}
    //if(ss== cutname[8]){if(ht_base( ht_)&&mht_350( mht_)&&Njet_5_6( nJets_))return true;}
    ////if(ss== cutname[9]){if(Njet_2( nJets_)&&ht_500( ht_)&&mht_200( mht_)&&dphi(nJets_, dphi0,dphi1,dphi2,dphi3))return true;}
    //if(ss== cutname[10]){if(Njet_2( nJets_)&&ht_500( ht_)&&mht_200( mht_))return true;}
    //if(ss== cutname[11]){if(Njet_2( nJets_)&&ht_500( ht_)&&mht_200( mht_))return true;}

    //if(ss== cutname[9]){if(Njet_4( nJets_)&&ht_500( ht_)&&mht_200( mht_)&&low_dphi(nJets_, dphi0,dphi1,dphi2,dphi3))return true;}
    //if(ss== cutname[10]){if(Njet_4( nJets_)&&ht_500( ht_)&&mht_200( mht_)&&dphi(nJets_, dphi0,dphi1,dphi2,dphi3))return true;}
    //if(ss== cutname[11]){if(Njet_4( nJets_)&&ht_500( ht_)&&mht_500( mht_))return true;}
    //if(ss== cutname[12]){if(Njet_4_6( nJets_)&&ht_500_1200( ht_)&&mht_500_750( mht_))return true;}
    //if(ss== cutname[13]){if(Njet_9( nJets_)&&ht_500( ht_)&&mht_200( mht_)&&dphi(nJets_, dphi0,dphi1,dphi2,dphi3))return true;}

  return false;
  }


  map<int, string> Selection::cutName() const {return cutname;}
 
  bool Selection::applyIsoTrk_() const {return applyIsoTrk;}
