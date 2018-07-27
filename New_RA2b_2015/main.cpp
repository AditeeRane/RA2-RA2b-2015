
//Lost Lepton Efficiency and Acceptance maps
#include "Events.h"
#include "Selection2.h"
#include "TTree.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include "TChain.h"
#include <cmath>
#include <map>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include "TLorentzVector.h"
#include <stdio.h>
#include "TColor.h"
#include "TF1.h"
#include "TLegend.h"
#include "TVector3.h"
#include "TFile.h"
#include "TH1.h"
#include "TVector2.h"
#include "TCanvas.h"
#include "TH2.h"
#include "utils2.h"
#include "TGraphAsymmErrors.h"

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
  vector<TH1D > vec;
  map<int, string> eventType;
  map<string , vector<TH1D> > cut_histvec_map;
  map<string, map<string , vector<TH1D> > > map_map;
  map<string, histClass> histobjmap;
  histClass histObj;

  //build a vector of histograms
  TH1D weight_hist = TH1D("weight", "Weight Distribution", 5,0,5);
  vec.push_back(weight_hist);
  TH1D RA2HT_hist = TH1D("HT","HT Distribution",50,0,5000);
  RA2HT_hist.Sumw2();
  vec.push_back(RA2HT_hist);
  TH1D RA2MHT_hist = TH1D("MHT","MHT Distribution",100,0,5000);
  RA2MHT_hist.Sumw2();
  vec.push_back(RA2MHT_hist);
  TH1D RA2NJet_hist = TH1D("NJet","Number of Jets Distribution",20,0,20);
  RA2NJet_hist.Sumw2();
  vec.push_back(RA2NJet_hist);
  TH1D RA2NBtag_hist = TH1D("NBtag","Number of Btag Distribution",20,0,20);
  RA2NBtag_hist.Sumw2();
  vec.push_back(RA2NBtag_hist);
  int Nhists=((int)(vec.size())-1);//-1 is because weight shouldn't be counted.

  ///read the file names from the .txt files and load them to a vector.
  while(fin.getline(filenames, 500) ){filesVec.push_back(filenames);}

  cout<< "\nProcessing " << subSampleKey << " ... " << endl;

  for(unsigned int in=0; in<filesVec.size(); in++){ sample_AUX->Add(filesVec.at(in).c_str()); }

  // --- Analyse the events --------------------------------------------

  // Interface to the event content
  Events * evt = new Events(sample_AUX, subSampleKey,verbose);

  // Get a pointer to the Selection class  
  Selection2 * sel = new Selection2();

  // For each selection, cut, make a vector containing the same histograms as those in vec
  for(int i=0; i<(int) sel->cutName().size();i++){
    cut_histvec_map[sel->cutName()[i]]=vec;
  }

  // Define different event categories 
  eventType[0]="allEvents";

  //initialize a map between string and maps. copy the map of histvecs into each
  for(int i=0; i< eventType.size();i++){
    map_map[eventType[i]]=cut_histvec_map;
  }

  //initialize histobjmap
  for(map<string , vector<TH1D> >::iterator it=cut_histvec_map.begin(); it!=cut_histvec_map.end();it++){
    histobjmap[it->first]=histObj;
  }

  TH1D* TauIDhist = new TH1D("yield_tauId","Yield after tau Id",200,0.,200.);
  TH1D* TauIDhist_trk = new TH1D("yield_tauId_trk","Yield after trk veto and tau Id",200,0.,200.);
  map <int,string> idMap;
  int IdNum_=0;
  for(int iPile=0;iPile<4;iPile++){
    for(int iIso=0;iIso<4;iIso++){
      for(int iMu=0;iMu<3;iMu++){
        for(int iElec=0;iElec<4;iElec++){
          IdNum_++;
          ostringstream binS_;
          binS_ << (1+iPile)+10*(1+iIso)+100*(1+iMu)+1000*(1+iElec);
          idMap[IdNum_]=binS_.str();
          TauIDhist->GetXaxis()->SetBinLabel(IdNum_,binS_.str().c_str());
          TauIDhist_trk->GetXaxis()->SetBinLabel(IdNum_,binS_.str().c_str());
        }
      }
    }
  }



  // Introduce cutflow histogram to monior event yields for early preselection
  TH1D* cutflow_preselection = new TH1D("cutflow_preselection","cutflow_preselectoion",
                                       11,0.,11.);
  cutflow_preselection->GetXaxis()->SetBinLabel(1,"All Events");
  cutflow_preselection->GetXaxis()->SetBinLabel(2,"Sample based gen-selection");
  cutflow_preselection->GetXaxis()->SetBinLabel(3,"HBHEIsoNoiseFilter");
  cutflow_preselection->GetXaxis()->SetBinLabel(4,"ecalBadCalibFilter");
  cutflow_preselection->GetXaxis()->SetBinLabel(5,"HBHENoiseFilter");
  cutflow_preselection->GetXaxis()->SetBinLabel(6,"GoodVtx");
  cutflow_preselection->GetXaxis()->SetBinLabel(7,"JetID Cleaning");

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

  int TotNEve_ = utils2::TotNEve(subSampleKey);
  // Loop over the events (tree entries)
  int eventN=0;
  while( evt->loadNext() ){

  //if(eventN>1000)break;
    // Total weight
    //double totWeight = evt->weight()*1.;
    double totWeight = 10000.*evt->XS()/TotNEve_;
    //printf(" XS: %g NEvents: %d weight: %g \n ",evt->XS(),TotNEve_,totWeight);

    cutflow_preselection->Fill(0.,totWeight); // keep track of all events processed

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

    cutflow_preselection->Fill(1.,totWeight);
    if(evt->HBHEIsoNoiseFilter_()==0)continue;
    cutflow_preselection->Fill(2.,totWeight);
    if(evt->ecalBadCalibFilter_()==0)continue;
    cutflow_preselection->Fill(3.,totWeight);
    if(evt->HBHENoiseFilter_()==0)continue;
    cutflow_preselection->Fill(4.,totWeight);
    if(!(evt->NVtx_() >0))continue;
    cutflow_preselection->Fill(5.,totWeight);
    // Through out an event that contains HTjets with bad id
    if(evt->JetId()==0)continue;
    cutflow_preselection->Fill(6.,totWeight); // events passing JetID event cleaning


  vector<TLorentzVector> genTauJetLorVec;
  for(int i=0;i<evt->GenTauLorVec()->size();i++){
    TLorentzVector tempVec(evt->GenTauLorVec()->at(i).Px()-evt->GenTauNuLorVec()->at(i).Px(),
                           evt->GenTauLorVec()->at(i).Py()-evt->GenTauNuLorVec()->at(i).Py(),
                           evt->GenTauLorVec()->at(i).Pz()-evt->GenTauNuLorVec()->at(i).Pz(),
                           evt->GenTauLorVec()->at(i).Energy()-evt->GenTauNuLorVec()->at(i).Energy()
                          );
    genTauJetLorVec.push_back(tempVec);
  }

  if(verbose!=0){
    printf(" ############# \n Number of gen tau: %d \n ",evt->GenTauPtVec_().size());
    for(int i=0; i < genTauJetLorVec.size(); i++){
      if(evt->GenTauHadVec_()[i]==1 && genTauJetLorVec.at(i).Pt() > 18.){
        printf(" genTauJet: Pt: %g Eta: %g Phi: %g \n ",genTauJetLorVec.at(i).Pt(),genTauJetLorVec.at(i).Eta(),genTauJetLorVec.at(i).Phi());
        for(int i=0; i<evt->TauLorVec_()->size(); i++){
          printf(" \n patTau: pt: %g eta: %g phi: %g \n ",evt->TauLorVec_()->at(i).Pt(),evt->TauLorVec_()->at(i).Eta(),evt->TauLorVec_()->at(i).Phi());
          printf(" Tauid => id1: %g id2: %g id3: %g id4: %g id5: %g id6: %g id7: %g id8: %g id9: %g id10: %g id11: %g \n ",evt->tauId1()->at(i),evt->tauId2()->at(i),evt->tauId3()->at(i),evt->tauId4()->at(i),evt->tauId5()->at(i),evt->tauId6()->at(i),evt->tauId7()->at(i),evt->tauId8()->at(i),evt->tauId9()->at(i),evt->tauId10()->at(i),evt->tauId11()->at(i));
        }
      }
    }
  }

    //printf("nTau=> 2233: %d 2243: %d 2333: %d 4333: %d 1333: %d \n ",evt->nTauMap()[2233],evt->nTauMap()[2243],evt->nTauMap()[2333],evt->nTauMap()[4333],evt->nTauMap()[1333]);

    // Print out some information
    if(verbose!=0){
      printf(" ########################### \n event #: %d \n",eventN);
      printf(" ht: %g mht: %g nJets: %d nBtags: %d nIsoElec: %d nIsoMu: %d nIsoPion: %d nLeptons: %d \n ",evt->ht(),evt->mht(),evt->nJets(),evt->nBtags(),evt->nIsoElec(),evt->nIsoMu(),evt->nIsoPion(),evt->nLeptons());
      printf(" @@@@\n Jets section: \n Njets: %d \n ", evt->nJets());
      for(int i=0;i<evt->JetsPtVec_().size();i++){
        printf("jet#: %d pt: %g eta: %g phi: %g \n ",i+1,evt->JetsPtVec_()[i],evt->JetsEtaVec_()[i],evt->JetsPhiVec_()[i]);
      }
      printf(" @@@@\n Muons section: \n Nmuons: %d \n ", evt->MuPtVec_().size());
      for(int i=0;i<evt->MuPtVec_().size();i++){
        printf("Muon#: %d pt: %g eta: %g phi: %g \n ",i+1,evt->MuPtVec_()[i],evt->MuEtaVec_()[i],evt->MuPhiVec_()[i]);
      }
    }


    //printf(" mu from tau: %d elec from tau : %d hadronicTau: %d \n ", evt->GenMu_GenMuFromTau_(), evt->GenElec_GenElecFromTau_(),evt->GenTau_GenTauHad_());
    //printf(" #Mu: %d #Tau: %d \n ", evt->GenMuPtVec_().size(), evt->GenTauPtVec_().size());

    // count the number of taus for all possible combinations of tau id s
    vector<int> NtauVec(200,0);

    // apply the baseline cuts here to study the tau id s
    if(sel->nolep(evt->nLeptons())&&sel->Njet_4(evt->nJets())&&sel->ht_500(evt->ht())&&
       sel->mht_200(evt->mht())&&sel->MuIsoTrk(evt->nIsoMu())&&sel->ElecIsoTrk(evt->nIsoElec())&&
       sel->dphi(evt->deltaPhi1(),evt->deltaPhi2(),evt->deltaPhi3(),evt->deltaPhi4()))
    {
      for(int i=0; i<evt->TauLorVec_()->size(); i++){
        // 4 categories of tau id. First is anti-elec which has 3 id's. We also insert a 1 which means non of them are applied. 
        // the following 4 lines correspond whith each of the categories in the tau id. 
        int tauIdElec[4]={1,(int)evt->tauId1()->at(i),(int)evt->tauId2()->at(i),(int)evt->tauId3()->at(i)};
        int tauIdMu[3]  ={1,(int)evt->tauId4()->at(i),(int)evt->tauId5()->at(i)};
        int tauIdIso[4] ={1,(int)evt->tauId6()->at(i),(int)evt->tauId7()->at(i),(int)evt->tauId8()->at(i)};
        int tauIdPile[4]={1,(int)evt->tauId9()->at(i),(int)evt->tauId10()->at(i),(int)evt->tauId11()->at(i)};
        /*
        printf(" id1: %d id2: %d id3: %d id4: %d id5: %d id6: %d id7: %d id8: %d id9: %d id10: %d id11: %d \n",
              (int)evt->tauId1()->at(i),(int)evt->tauId2()->at(i),(int)evt->tauId3()->at(i),(int)evt->tauId4()->at(i),
              (int)evt->tauId5()->at(i),(int)evt->tauId6()->at(i),(int)evt->tauId7()->at(i),(int)evt->tauId8()->at(i),
              (int)evt->tauId9()->at(i),(int)evt->tauId10()->at(i),(int)evt->tauId11()->at(i));      
        */
        int IdNum=0;

        for(int iPile=0;iPile<(sizeof(tauIdPile)/sizeof(tauIdPile[0]));iPile++){
          for(int iIso=0;iIso<(sizeof(tauIdIso)/sizeof(tauIdIso[0]));iIso++){
            for(int iMu=0;iMu<(sizeof(tauIdMu)/sizeof(tauIdMu[0]));iMu++){
              for(int iElec=0;iElec<(sizeof(tauIdElec)/sizeof(tauIdElec[0]));iElec++){
                IdNum++;
                if(tauIdElec[iElec]==1&&tauIdMu[iMu]==1&&tauIdIso[iIso]==1&&tauIdPile[iPile]==1)NtauVec[IdNum]++;
                  //printf(" iPile: %d => %d iIso: %d => %d iMu: %d => %d iElec: %d => %d \n",iPile,tauIdPile[iPile],iIso,tauIdIso[iIso],iMu,tauIdMu[iMu],iElec,tauIdElec[iElec]);
                  //printf(" id #: %d nTau: %d \n ",IdNum,NtauVec[IdNum]);
              }
            }
          }
        }
      } 

      for(int iId=0; iId<NtauVec.size();iId++){
        //printf(" @ \n iId: %d -> %s \n ",iId,idMap[iId].c_str());
        if(NtauVec[iId]==0){
          TauIDhist->Fill(iId-1,totWeight); 
          //cout << " filled \n ";
          if(sel->PionIsoTrk(evt->nIsoPion()))TauIDhist_trk->Fill(iId-1,totWeight);
        }
      }
    }
    // Build and array that contains the quantities we need a histogram for.
    // Here order is important and must be the same as RA2nocutvec
    double eveinfvec[] = {totWeight,(double) evt->ht(),(double) evt->mht() ,(double) evt->nJets(),(double) evt->nBtags()}; //the last one gives the RA2 defined number of jets.     


    //loop over all the different backgrounds: "allEvents", "Wlv", "Zvv"
    for(map<string, map<string , vector<TH1D> > >::iterator itt=map_map.begin(); itt!=map_map.end();itt++){//this will be terminated after the cuts

      ////determine what type of background should pass
      if(itt->first=="allEvents"){//all the cuts are inside this

        //Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts//Cuts

        //////loop over cut names and fill the histograms
        for(map<string , vector<TH1D> >::iterator ite=cut_histvec_map.begin(); ite!=cut_histvec_map.end();ite++){

          if(sel->checkcut(ite->first,evt->ht(),evt->mht(),evt->deltaPhi1(),evt->deltaPhi2(),evt->deltaPhi3(),evt->deltaPhi4(),evt->nJets(),evt->nBtags(),evt->nLeptons(),evt->nIsoElec(),evt->nIsoMu(),evt->nIsoPion(),evt->nTauMap()[2233],evt->nTauMap()[2243],evt->nTauMap()[2333],evt->nTauMap()[4333],evt->nTauMap()[1333])==true){
             histobjmap[ite->first].fill(Nhists,&eveinfvec[0] ,&itt->second[ite->first][0]);
           } 
        }//end of loop over cut names

        ////EndOfCuts//EndOfCuts//EndOfCuts//EndOfCuts//EndOfCuts//EndOfCuts//EndOfCuts//EndOfCuts//EndOfCuts//EndOfCuts

      }//end of bg_type determination
    }//end of loop over all the different backgrounds: "allEvents", "Wlv", "Zvv"

    eventN++;
  } // End of loop over events


  //open a file to write the histograms
  sprintf(tempname,"%s/results_%s_%s.root",Outdir.c_str(),subSampleKey.c_str(),inputnumber.c_str());
  TFile *resFile = new TFile(tempname, "RECREATE");
  TDirectory *cdtoitt;
  TDirectory *cdtoit;

  cutflow_preselection->Write();
  TauIDhist->Write();
  TauIDhist_trk->Write();

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
            if (sel->cutName()[i]==it->first){
              cdtoit = cdtoitt->mkdir((it->first).c_str());
              cdtoit->cd();
              int nHist = it->second.size();
              for(int i=0; i<nHist; i++){//since we only have 4 type of histograms
                sprintf(tempname,"%s_%s_%s",it->second[i].GetName(),(it->first).c_str(),(itt->first).c_str());
                it->second[i].Write(tempname);
              }
              cdtoitt->cd();
            }
          }
        }
      }
    }
  }




}
