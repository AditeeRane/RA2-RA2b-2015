
namespace utils2{

  // Determine which Iso trk veto to work with
  // 0: IsoTrk effeiciency is calculated from expection code
  //    and is applied in the prediction code as weight
  // 1: The veto stays the same as 0, for the expectation code.
  //    From the expectation code, We calculte how many times 
  //    the tau-jet(and only the leading tau jet) gets vetoed 
  //    by the isoTrks.
  //    In the prediction code, we veto the events if the
  //    leading muon does not match any of the available trks.
  //    alos, we will apply the efficiency that was calculated
  //    and talked about above as weight.
  int IsoTrkModel=0;

  // Determine which model to work with
  // 0: The most simple model
  // 1: 0 but muon's mother ( W or tau ) is determined using hists not generator info.
  // 2: 1 but for veto(in the prediction code) reco objects are used and not gen ones 
  // 3: 2 but in prediction everything reco. Efficiency used
  // 4: 3 but Acceptance added
  int TauHadModel=4;

  // Do the bootstrapping?
  bool bootstrap = true; // default true

  // btagSF model
  bool btagSF=true;

  bool applyMT = true; // default true
  // We calculate MTEff in the same code where it is also being used. 
  // So, it doesn't make sense to apply MT weight when calculating it.
  // To calc. MT eff. first turn off applyMT and applyIsoTrk but turn on bootstrap and 
  // CalcMT. Save the search bin histogram. Next turn on applyMT and run again. Divide 
  // the two search bin histograms, after and before applyMT. 
  // At the end, turn off CalcMT.
  bool CalcMT = false; // default false

  bool applyTrig = false;

  bool applyIsoTrk =true; // default true

  bool genHTMHT=false; // default false
  //###############################################################################################################

  bool addSys=true;
  // get the total # of events for normalization purposes
  int TotNEve(string subSampleKey){
    int NEve=-1;
    if(subSampleKey.find("TTbar_DiLept")!=string::npos)NEve=5470162;
    else if(subSampleKey.find("TTbar_HT_600_800")!=string::npos)NEve=4837326;
    else if(subSampleKey.find("TTbar_HT_800_1200")!=string::npos)NEve=3273714;
    else if(subSampleKey.find("TTbar_HT_1200_2500")!=string::npos)NEve=970912;
    else if(subSampleKey.find("TTbar_HT_2500_Inf")!=string::npos)NEve=467189;
    else if(subSampleKey.find("TTbar_Inclusive")!=string::npos)NEve=11299872;
    else if(subSampleKey.find("TTbar_T_SingleLep")!=string::npos)NEve=10145239;
    else if(subSampleKey.find("TTbar_Tbar_SingleLep")!=string::npos)NEve=11619522; //
    else if(subSampleKey.find("WJet_HT_100_200")!=string::npos)NEve=6334833;
    else if(subSampleKey.find("WJet_HT_200_400")!=string::npos)NEve=4156939;
    else if(subSampleKey.find("WJet_HT_400_600")!=string::npos)NEve=1728593;
    else if(subSampleKey.find("WJet_HT_600_800")!=string::npos)NEve=3077541;
    else if(subSampleKey.find("WJet_HT_800_1200")!=string::npos)NEve=1173001;
    else if(subSampleKey.find("WJet_HT_1200_2500")!=string::npos)NEve=210017;
    else if(subSampleKey.find("WJet_HT_2500_Inf")!=string::npos)NEve=253036; //
    else if(subSampleKey.find("ST_tW_antitop")!=string::npos)NEve=703454;
    else if(subSampleKey.find("ST_tW_top")!=string::npos)NEve=995600;
    else if(subSampleKey.find("ST_t_top")!=string::npos)NEve=2745000;
    else if(subSampleKey.find("ST_t_antitop")!=string::npos)NEve=1595900; //
    else if(subSampleKey.find("ZJet_HT_100_200")!=string::npos)NEve=5075985;
    else if(subSampleKey.find("ZJet_HT_200_400")!=string::npos)NEve=4865021;
    else if(subSampleKey.find("ZJet_HT_400_600")!=string::npos)NEve=891556;
    else if(subSampleKey.find("ZJet_HT_600_Inf")!=string::npos)NEve=963090; //
    else if(subSampleKey.find("QCD_HT_200_300")!=string::npos)NEve=18675969;
    else if(subSampleKey.find("QCD_HT_300_500")!=string::npos)NEve=18477055;
    else if(subSampleKey.find("QCD_HT_500_700")!=string::npos)NEve=14978442;
    else if(subSampleKey.find("QCD_HT_700_1000")!=string::npos)NEve=13922489;
    else if(subSampleKey.find("QCD_HT_1000_1500")!=string::npos)NEve=4914035;
    else if(subSampleKey.find("QCD_HT_1500_2000")!=string::npos)NEve=3673364;
    else if(subSampleKey.find("QCD_HT_2000_Inf")!=string::npos)NEve=1912529; //
    else if(subSampleKey.find("T1bbbb_1000_900")!=string::npos)NEve=142674;
    else if(subSampleKey.find("T1bbbb_1500_100")!=string::npos)NEve=52613;
    else if(subSampleKey.find("T1qqqq_1000_800")!=string::npos)NEve=95354;
    else if(subSampleKey.find("T1tttt_1200_800")!=string::npos)NEve=147194;
    else if(subSampleKey.find("T1tttt_1500_100")!=string::npos)NEve=103140;
    else cout << " there is no # events for the given smaple. The weight is not correct now! \n " << endl;
    cout << " Please make sure that total # events are up to date! \n " << endl;

    return NEve;
  }

  // Logical filename to physical filename
  string LFNtoPFN(string PFN){
    string prefix="root://cmseos.fnal.gov/";
    return prefix+PFN;
  }
  
  // get the skimFileName
  string skimFileName(string subSampleKey){
    string skimPath="/store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV10/tree_SLm/";
    string skimName="tree_TTJets_SingleLeptFromT.root";
    if(subSampleKey.find("TTbar_DiLept")!=string::npos)skimName="tree_TTJets_DiLept.root";
    else if(subSampleKey.find("TTbar_HT_600_800")!=string::npos)skimName="tree_TTJets_HT-600to800.root";
    else if(subSampleKey.find("TTbar_HT_800_1200")!=string::npos)skimName="tree_TTJets_HT-800to1200.root";
    else if(subSampleKey.find("TTbar_HT_1200_2500")!=string::npos)skimName="tree_TTJets_HT-1200to2500.root";
    else if(subSampleKey.find("TTbar_HT_2500_Inf")!=string::npos)skimName="tree_TTJets_HT-2500toInf.root";
    else if(subSampleKey.find("TTbar_Inclusive")!=string::npos)skimName="tree_TTJets.root";
    else if(subSampleKey.find("TTbar_T_SingleLep")!=string::npos)skimName="tree_TTJets_SingleLeptFromT.root";
    else if(subSampleKey.find("TTbar_Tbar_SingleLep")!=string::npos)skimName="tree_TTJets_SingleLeptFromTbar.root"; //
    else if(subSampleKey.find("WJet_HT_100_200")!=string::npos)skimName="tree_WJetsToLNu_HT-100to200.root";
    else if(subSampleKey.find("WJet_HT_200_400")!=string::npos)skimName="tree_WJetsToLNu_HT-200to400.root";
    else if(subSampleKey.find("WJet_HT_400_600")!=string::npos)skimName="tree_WJetsToLNu_HT-400to600.root";
    else if(subSampleKey.find("WJet_HT_600_800")!=string::npos)skimName="tree_WJetsToLNu_HT-600to800.root";
    else if(subSampleKey.find("WJet_HT_800_1200")!=string::npos)skimName="tree_WJetsToLNu_HT-800to1200.root";
    else if(subSampleKey.find("WJet_HT_1200_2500")!=string::npos)skimName="tree_WJetsToLNu_HT-1200to2500.root";
    else if(subSampleKey.find("WJet_HT_2500_Inf")!=string::npos)skimName="tree_WJetsToLNu_HT-2500toInf.root"; //
    else if(subSampleKey.find("ST_tW_antitop")!=string::npos)skimName="tree_ST_tW_antitop.root";
    else if(subSampleKey.find("ST_tW_top")!=string::npos)skimName="tree_ST_tW_top.root";
    else if(subSampleKey.find("ST_t_top")!=string::npos)skimName="tree_ST_t-channel_top.root";
    else if(subSampleKey.find("ST_t_antitop")!=string::npos)skimName="tree_ST_t-channel_antitop.root"; //
    else if(subSampleKey.find("s_channel")!=string::npos)skimName="tree_ST_s-channel.root"; //
    //else if(subSampleKey.find("ZJet_HT_100_200")!=string::npos);
    //else if(subSampleKey.find("ZJet_HT_200_400")!=string::npos);
    //else if(subSampleKey.find("ZJet_HT_400_600")!=string::npos);
    //else if(subSampleKey.find("ZJet_HT_600_Inf")!=string::npos); //
    //else if(subSampleKey.find("QCD_HT_200_300")!=string::npos);
    //else if(subSampleKey.find("QCD_HT_300_500")!=string::npos);
    //else if(subSampleKey.find("QCD_HT_500_700")!=string::npos);
    //else if(subSampleKey.find("QCD_HT_700_1000")!=string::npos);
    //else if(subSampleKey.find("QCD_HT_1000_1500")!=string::npos);
    //else if(subSampleKey.find("QCD_HT_1500_2000")!=string::npos);
    //else if(subSampleKey.find("QCD_HT_2000_Inf")!=string::npos); //
    //else if(subSampleKey.find("T1bbbb_1000_900")!=string::npos);
    //else if(subSampleKey.find("T1bbbb_1500_100")!=string::npos);
    //else if(subSampleKey.find("T1qqqq_1000_800")!=string::npos);
    //else if(subSampleKey.find("T1tttt_1200_800")!=string::npos);
    //else if(subSampleKey.find("T1tttt_1500_100")!=string::npos);
    else cout << " there is no skim file for the given smaple. The weight is not correct now! \n " << endl;

    return skimPath+skimName;
  }

//###############################################################################################################
// Full search bins

  // find appropriate bin number for the given (Njet,Nbtag,ht,mht)
      std::string findBin(int njet,int nbtag,double ht,double mht){

        std::ostringstream binS;
        int bNjet, bNbtag, bHtMht;
        if(njet==2)bNjet=1;else if(njet >= 3 && njet <=4)bNjet=2;else if(njet >= 5 && njet <=6)bNjet=3;else if(njet >= 7 && njet <=8)bNjet=4;else if(njet >= 9)bNjet=5; else bNjet=9;

        if(nbtag == 0)bNbtag=1;else if(nbtag==1)bNbtag=2;else if(nbtag == 2)bNbtag=3;else if(nbtag >= 3)bNbtag=4;else bNbtag=9;
	if(njet<7){
	  if(ht >= 300 && ht <500 && mht>=300 && mht<350)bHtMht=1;else if(ht >= 500 && ht <1000 && mht>=300 && mht<350)bHtMht=2;else if(ht >= 1000 && mht>=300 && mht<350)bHtMht=3;
	  else if(ht >= 350 && ht <500 && mht>=350 && mht<500)bHtMht=4;else if(ht >=500 && ht<1000 && mht>=350 && mht<500)bHtMht=5;else if(ht >=1000 && mht>=350 && mht<500)bHtMht=6;
	  else if(ht >= 500 && ht <1000 && mht>=500 && mht<750)bHtMht=7;else if(ht >= 1000 && mht>=500 && mht<750)bHtMht=8;
	  else if(ht >= 750 && ht<1500 && mht>=750)bHtMht=9;
	  else if(ht >= 1500 && mht>=750)bHtMht=10;
	  else bHtMht=99;
	}
	else if(njet>=7){
	  if(ht >= 500 && ht <1000 && mht>=300 && mht<350)bHtMht=1;else if(ht >= 1000 && mht>=300 && mht<350)bHtMht=2;
	  else if(ht >=500 && ht<1000 && mht>=350 && mht<500)bHtMht=3;else if(ht >=1000 && mht>=350 && mht<500)bHtMht=4;
	  else if(ht >= 500 && ht <1000 && mht>=500 && mht<750)bHtMht=5;else if(ht >= 1000 && mht>=500 && mht<750)bHtMht=6;
	  else if(ht >= 750 && ht<1500 && mht>=750)bHtMht=7;
	  else if(ht >= 1500 && mht>=750)bHtMht=8;
	  else bHtMht=99;
	}
	//else bHtMht=99;
        binS << 1000*bNjet+100*bNbtag+bHtMht ;

        return binS.str();
      }

  // A map is needed between strings like "132" or "2143" that specify the searc bins
  // (see findBin fundtion above) and an integer that can take from 1 to 108 (# of search bins)
      std::map <std::string,int> BinMap(){
	std::cout<<"-------------------BinMap function getting printed-----------------"<<std::endl;
	int binN=0;
	//	string binString[174]={"nJet0_nB0_MHT0_HT0","nJet0_nB0_MHT0_HT1","nJet0_nB0_MHT0_HT2","nJet0_nB0_MHT0_HT3","nJet0_nB0_MHT0_HT0","nJet0_nB0_MHT0_HT0","nJet0_nB0_MHT0_HT0","nJet0_nB0_MHT0_HT0","nJet0_nB0_MHT0_HT0","nJet0_nB0_MHT0_HT0","nJet0_nB0_MHT0_HT0","nJet0_nB0_MHT0_HT0","nJet0_nB0_MHT0_HT0","nJet0_nB0_MHT0_HT0","nJet0_nB0_MHT0_HT0","nJet0_nB0_MHT0_HT0","nJet0_nB0_MHT0_HT0","nJet0_nB0_MHT0_HT0","nJet0_nB0_MHT0_HT0","nJet0_nB0_MHT0_HT0","nJet0_nB0_MHT0_HT0","nJet0_nB0_MHT0_HT0","nJet0_nB0_MHT0_HT0"};
      int Max_bNbtag=0;
      int Max_bHtMht=0;
      int nJet=0;
      int nB=0;
      int HT=0;
      int MHT=0;
      char cname[500];
      std::map <std::string , int> binMap;
      //std::map <std::string ,std::string> binMap;
      for(int bNjet=1; bNjet<=5;  bNjet++){
	nJet=bNjet-1;
	if(bNjet ==1)Max_bNbtag=3;
	else Max_bNbtag=4;
	if(bNjet >3)Max_bHtMht=8;
	else Max_bHtMht=10;
        for(int bNbtag=1; bNbtag<=Max_bNbtag; bNbtag++){
	  nB=bNbtag-1;MHT=0;
          for(int bHtMht=1; bHtMht<=Max_bHtMht; bHtMht++){
	    HT=bHtMht-1;
	    std::ostringstream binS;
	    std::ostringstream newbinS;
	    binS << 1000*bNjet+100*bNbtag+bHtMht;
	    binN++;
	    binMap[binS.str()]=binN;
	      //    std::cout << "binString: " << binS.str() << " corresponing with binNumber: " <<binN << std::endl;
	      sprintf(cname, "nJet%i_nB%i_MHT%i_HT%i", nJet,nB,MHT,HT);
	      std::cout << "binString: " << cname << " corresponing with binNumber: " <<binN << std::endl;
	      if(Max_bHtMht==10 && (HT==2 || HT==5 || HT==7))MHT++;
	      else if(Max_bHtMht==8 && (HT==1 || HT==3 || HT==5))MHT++;
	      //  binS << cname;
	      //binMap[binS.str()]=cname;
          }
        }
      }

      std::cout<<"-------------------BinMap function ends here-----------------"<<std::endl;
    return binMap;
  }

//############################################################################################
// Search bins integrated over nbtags

  // find appropriate bin number for the given (Njet,ht,mht) (and no Btag)

      std::string findBin_NoB(int njet,double ht,double mht){
        std::ostringstream binS;
        int bNjet, bHtMht;
        if(CalcMT){
	  if(njet == 2)bNjet=1;
          else if(njet == 3)bNjet=2;
          else if(njet == 4)bNjet=3;
          else if(njet == 5)bNjet=4;
          else if(njet == 6)bNjet=5;
          else if(njet >= 7 && njet <=8)bNjet=6;
          else if(njet >= 9)bNjet=7;
          else bNjet=9;

          if(ht >= 300 && ht <500 && mht>=250 && mht<300)bHtMht=1;
          else if(ht >= 500 && ht <1000 && mht>=250 && mht<300)bHtMht=2;
          else if(ht >= 1000 && mht>=250 && mht<300)bHtMht=3;
          else if(ht >= 300 && ht <500 && mht>=300 && mht<350)bHtMht=4;
          else if(ht >= 500 && ht <1000 && mht>=300 && mht<350)bHtMht=5;
          else if(ht >= 1000 && mht>=300 && mht<350)bHtMht=6;
          else if(ht >= 350 && ht <500 && mht>=350 && mht<500)bHtMht=7;
          else if(ht >=500 && ht<1000 && mht>=350 && mht<500)bHtMht=8;
          else if(ht >=1000 && mht>=350 && mht<500)bHtMht=9;
          else if(ht >= 500 && ht <1000 && mht>=500 && mht<750)bHtMht=10;
          else if(ht >= 1000 && mht>=500 && mht<750)bHtMht=11;
          else if(ht >= 750 && mht>=750)bHtMht=12;
          else bHtMht=99;
        }else{

          if(njet == 2)bNjet=1;else if(njet >= 3 && njet <=4)bNjet=2;else if(njet >= 5 && njet <=6)bNjet=3;else if(njet >= 7 && njet <=8)bNjet=4;else if(njet >= 9)bNjet=5; else bNjet=9;

          if(ht >= 300 && ht <500 && mht>=300 && mht<350)bHtMht=1;else if(ht >= 500 && ht <1000 && mht>=300 && mht<350)bHtMht=2;else if(ht >= 1000 && mht>=300 && mht<350)bHtMht=3;
          else if(ht >= 350 && ht <500 && mht>=350 && mht<500)bHtMht=4;else if(ht >=500 && ht<1000 && mht>=350 && mht<500)bHtMht=5;else if(ht >=1000 && mht>=350 && mht<500)bHtMht=6;
          else if(ht >= 500 && ht <1000 && mht>=500 && mht<750)bHtMht=7;else if(ht >= 1000 && mht>=500 && mht<750)bHtMht=8;
          else if(ht >= 750 && ht<1500 && mht>=750)bHtMht=9;
          else if(ht >= 1500 && mht>=750)bHtMht=10;
          else bHtMht=99;

        }

        binS << 100*bNjet+bHtMht;

        return binS.str();
      }


  // A map is needed between strings like "15" or "24" that specify the search bins ( without Btag)
  // (see findBin fundtion above) and an integer that can take from 1 to 108 (# of search bins)
  std::map <std::string,int> BinMap_NoB(){
    cout << " --------------- Beginning of _NoB binning ----------------" ;
    int binN=0;
    std::map <std::string , int> binMap_NoB;
    if(CalcMT){
      for(int bNjet=1; bNjet<=7;  bNjet++){
          for(int bHtMht=1; bHtMht<=12; bHtMht++){
              std::ostringstream binS;
              binS << 100*bNjet+bHtMht;
              binN++;
              binMap_NoB[binS.str()]=binN;
              std::cout << "binString: " << binS.str() << " corresponing with binNumber: " <<binN << std::endl;
          }
      }
    }else{
      for(int bNjet=1; bNjet<=5;  bNjet++){
          for(int bHtMht=1; bHtMht<=10; bHtMht++){
              std::ostringstream binS;
              binS << 100*bNjet+bHtMht;
              binN++;
              binMap_NoB[binS.str()]=binN;
              std::cout << "binString: " << binS.str() << " corresponing with binNumber: " <<binN << std::endl;
          }
      }

    }
    cout << " --------------- End of _NoB binning ----------------" ;
    return binMap_NoB;
  }



//############################################################################################
// Bins defined by Njet and mht only
//

  // find appropriate bin number for the given (Njet,mht) (and no Btag)

      std::string findBin_mht_nj(int njet,double mht){
        std::ostringstream binS;
        int bNjet, bMht;
        if(njet == 4)bNjet=1;else if(njet == 5)bNjet=2;else if(njet == 6)bNjet=3;
        else if(njet >= 7 && njet <=8)bNjet=4;else if(njet >= 9)bNjet=5;else bNjet=9;
        if(mht>=200 && mht<500)bMht=1;else if(mht>=500 && mht<750)bMht=2;else if(mht>=750)bMht=3; else bMht=9;
        binS << 10*bNjet+bMht;

        return binS.str();
      }


  // A map is needed between strings like "15" or "24" that specify the search bins ( without Btag)
  // (see findBin fundtion above) and an integer that can take from 1 to 108 (# of search bins)
  std::map <std::string,int> BinMap_mht_nj(){
    std::cout<<"-------------------BinMap_mht_nj function getting printed-----------------"<<std::endl;

      int binN=0;
      std::map <std::string , int> binMap_mht_nj;
      for(int bNjet=1; bNjet<=5;  bNjet++){
          for(int bMht=1; bMht<=3; bMht++){
              std::ostringstream binS;
              binS << 10*bNjet+bMht;
              binN++;
              binMap_mht_nj[binS.str()]=binN;
              std::cout << "binString: " << binS.str() << " corresponing with binNumber: " <<binN << std::endl;
          }
      }
    return binMap_mht_nj;
  }

//############################################################################################
// Bins defined by Njet, HT, MHT (no b-tag) used for Iso-track veto parametrization
//

  // find appropriate bin number for the given (Njet,ht,mht) (and no Btag)

      std::string findBin_ForIso(int njet,double ht,double mht){
        std::ostringstream binS;
        int bNjet, bHtMht;
	if(njet == 2)bNjet=1;
        else if(njet == 3)bNjet=2;
        else if(njet == 4)bNjet=3;
        else if(njet == 5)bNjet=4;
        else if(njet == 6)bNjet=5;
        else if(njet >= 7 && njet <=8)bNjet=6;
        else if(njet >= 9)bNjet=7;
        else bNjet=9;

        if(ht >= 300 && ht <500 && mht>=250 && mht<300)bHtMht=1;
        else if(ht >= 500 && ht <1000 && mht>=250 && mht<300)bHtMht=2;
        else if(ht >= 1000 && mht>=250 && mht<300)bHtMht=3;
        else if(ht >= 300 && ht <500 && mht>=300 && mht<350)bHtMht=4;
        else if(ht >= 500 && ht <1000 && mht>=300 && mht<350)bHtMht=5;
        else if(ht >= 1000 && mht>=300 && mht<350)bHtMht=6;
        else if(ht >= 350 && ht <500 && mht>=350 && mht<500)bHtMht=7;
        else if(ht >=500 && ht<1000 && mht>=350 && mht<500)bHtMht=8;
        else if(ht >=1000 && mht>=350 && mht<500)bHtMht=9;
        else if(ht >= 500 && ht <1000 && mht>=500 && mht<750)bHtMht=10;
        else if(ht >= 1000 && mht>=500 && mht<750)bHtMht=11;
        else if(ht >= 750 && mht>=750)bHtMht=12;
        else bHtMht=99;


        binS << 100*bNjet+bHtMht;

        return binS.str();
      }


  // A map is needed between strings like "15" or "24" that specify the search bins ( without Btag)
  // (see findBin fundtion above) and an integer that can take from 1 to 108 (# of search bins)
  std::map <std::string,int> BinMap_ForIso(){
    cout << " --------------- Beginning of _ForIso binning ----------------" ;
    int binN=0;
    std::map <std::string , int> binMap_ForIso;
    for(int bNjet=1; bNjet<=7;  bNjet++){
        for(int bHtMht=1; bHtMht<=12; bHtMht++){
            std::ostringstream binS;
            binS << 100*bNjet+bHtMht;
            binN++;
            binMap_ForIso[binS.str()]=binN;
            std::cout << "binString: " << binS.str() << " corresponing with binNumber: " <<binN << std::endl;
        }
    }
    cout << " --------------- End of _ForIso binning ----------------" ;
    return binMap_ForIso;
  }

//############################################################################################
// Bins defined by Njet, HT, MHT (no b-tag) used for Iso-track veto parametrization
//

  // find appropriate bin number for the given (Njet,ht,mht) (and no Btag)

      std::string findBin_ForAcc(int njet,double ht,double mht){
        std::ostringstream binS;
        int bNjet, bHtMht;
	if(njet == 2)bNjet=1;
        else if(njet == 3)bNjet=2;
        else if(njet == 4)bNjet=3;
        else if(njet == 5)bNjet=4;
        else if(njet == 6)bNjet=5;
        else if(njet >= 7 && njet <=8)bNjet=6;
        else if(njet >= 9)bNjet=7;
        else bNjet=9;

        if(ht >= 300 && ht <500 && mht>=250 && mht<300)bHtMht=1;
        else if(ht >= 500 && ht <1000 && mht>=250 && mht<300)bHtMht=2;
        else if(ht >= 1000 && mht>=250 && mht<300)bHtMht=3;
        else if(ht >= 300 && ht <500 && mht>=300 && mht<350)bHtMht=4;
        else if(ht >= 500 && ht <1000 && mht>=300 && mht<350)bHtMht=5;
        else if(ht >= 1000 && mht>=300 && mht<350)bHtMht=6;
        else if(ht >= 350 && ht <500 && mht>=350 && mht<500)bHtMht=7;
        else if(ht >=500 && ht<1000 && mht>=350 && mht<500)bHtMht=8;
        else if(ht >=1000 && mht>=350 && mht<500)bHtMht=9;
        else if(ht >= 500 && ht <1000 && mht>=500 && mht<750)bHtMht=10;
        else if(ht >= 1000 && mht>=500 && mht<750)bHtMht=11;
        else if(ht >= 750 && mht>=750)bHtMht=12;
        else bHtMht=99;

        binS << 100*bNjet+bHtMht;

        return binS.str();
      }


  // A map is needed between strings like "15" or "24" that specify the search bins ( without Btag)
  // (see findBin fundtion above) and an integer that can take from 1 to 108 (# of search bins)
  std::map <std::string,int> BinMap_ForAcc(){
      std::cout<<"-------------------BinMap_ForAcc function getting printed-----------------"<<std::endl;
      int binN=0;
      std::map <std::string , int> binMap_ForAcc;
      for(int bNjet=1; bNjet<=7;  bNjet++){
          for(int bHtMht=1; bHtMht<=12; bHtMht++){
              std::ostringstream binS;
              binS << 100*bNjet+bHtMht;
              binN++;
              binMap_ForAcc[binS.str()]=binN;
              std::cout << "binString: " << binS.str() << " corresponing with binNumber: " <<binN << std::endl;
          }
      }
    std::cout<<"-------------------end of BinMap_ForAcc function -----------------"<<std::endl;
    return binMap_ForAcc;
  }



//############################################################################################
// QCD binnings
//
  // find appropriate bin number for the given (Njet,Nbtag,ht,mht)
      std::string findBin_QCD(int njet,int nbtag,double ht,double mht){

        std::ostringstream binS;
        int bNjet, bNbtag, bHtMht;
        if(njet == 2)bNjet=1;else if(njet >= 3 && njet <=4)bNjet=2;else if(njet >= 5 && njet <=6)bNjet=3;else if(njet >= 7 && njet <=8)bNjet=4;else if(njet >= 9)bNjet=5; else bNjet=9;

        if(nbtag == 0)bNbtag=1;else if(nbtag==1)bNbtag=2;else if(nbtag == 2)bNbtag=3;else if(nbtag >= 3)bNbtag=4;else bNbtag=9;
	if(njet<7){
	  if(ht >= 300 && ht <500 && mht>=250 && mht<300)bHtMht=1;else if(ht >= 500 && ht <1000 && mht>=250 && mht<300)bHtMht=2;else if(ht >= 1000 && mht>=250 && mht<300)bHtMht=3;
	  else if(ht >= 300 && ht <500 && mht>=300 && mht<350)bHtMht=4;else if(ht >= 500 && ht <1000 && mht>=300 && mht<350)bHtMht=5;else if(ht >= 1000 && mht>=300 && mht<350)bHtMht=6;
	  else if(ht >= 350 && ht <500 && mht>=350 && mht<500)bHtMht=7;else if(ht >=500 && ht<1000 && mht>=350 && mht<500)bHtMht=8;else if(ht >=1000 && mht>=350 && mht<500)bHtMht=9;
	  else if(ht >= 500 && ht <1000 && mht>=500 && mht<750)bHtMht=10;else if(ht >= 1000 && mht>=500 && mht<750)bHtMht=11;
	  else if(ht >= 750 && ht<1500 && mht>=750)bHtMht=12;
	  else if(ht >= 1500 && mht>=750)bHtMht=13;
	  else bHtMht=99;
	}
	else if(njet>=7){
	  if(ht >= 300 && ht <500 && mht>=250 && mht<300)bHtMht=1;else if(ht >= 500 && ht <1000 && mht>=250 && mht<300)bHtMht=2;else if(ht >= 1000 && mht>=250 && mht<300)bHtMht=3;
	  else if(ht >= 500 && ht <1000 && mht>=300 && mht<350)bHtMht=4;else if(ht >= 1000 && mht>=300 && mht<350)bHtMht=5;
	  else if(ht >=500 && ht<1000 && mht>=350 && mht<500)bHtMht=6;else if(ht >=1000 && mht>=350 && mht<500)bHtMht=7;
	  else if(ht >= 500 && ht <1000 && mht>=500 && mht<750)bHtMht=8;else if(ht >= 1000 && mht>=500 && mht<750)bHtMht=9;
	  else if(ht >= 750 && ht<1500 && mht>=750)bHtMht=10;
	  else if(ht >= 1500 && mht>=750)bHtMht=11;
	  else bHtMht=99;
	}

        binS << 1000*bNjet+100*bNbtag+bHtMht ;

        return binS.str();
      }

  // A map is needed between strings like "132" or "2143" that specify the searc bins
  // (see findBin fundtion above) and an integer that can take from 1 to 108 (# of search bins)
  std::map <std::string,int> BinMap_QCD(){
    std::cout<<"-------------------BinMap_QCD function getting printed-----------------"<<std::endl;
      int binN=0;
      int Max_bNbtag=0;
      int Max_bHtMht=0;
      std::map <std::string , int> binMap;
      for(int bNjet=1; bNjet<=5;  bNjet++){
	if(bNjet ==1)Max_bNbtag=3;
	else Max_bNbtag=4;
	if(bNjet>3)Max_bHtMht=11;
	else Max_bHtMht=13;
        for(int bNbtag=1; bNbtag<=Max_bNbtag; bNbtag++){
          for(int bHtMht=1; bHtMht<=Max_bHtMht; bHtMht++){
              std::ostringstream binS;
              binS << 1000*bNjet+100*bNbtag+bHtMht;
              binN++;
              binMap[binS.str()]=binN;
              std::cout << "binString: " << binS.str() << " corresponing with binNumber: " <<binN << std::endl;
          }
        }
      }

      std::cout<<"-------------------BinMap_QCD function ends here-----------------"<<std::endl;
    return binMap;
  }

//############################################################################################
// HTMHT bins inclusive in #jet and #b
//
  // find appropriate bin number for the given (Njet,ht,mht) (and no Btag)

      std::string findBin_HTMHT(double ht,double mht){
        std::ostringstream binS;
        int  bHtMht;
        if(ht >= 250 && ht <800 && mht>=200 && mht<500)bHtMht=1;else if(ht >= 800 && ht <1200 && mht>=200 && mht<500)bHtMht=2;else if(ht >= 1200 && mht>=200 && mht<500)bHtMht=3;
        else if(ht >= 250 && ht <1200 && mht>=500 && mht<750)bHtMht=4;else if(ht >=1200 && mht>=500 && mht<750)bHtMht=5;else if(ht >=800 && mht>=750)bHtMht=6; else bHtMht=9;
        binS << bHtMht;

        return binS.str();
      }


  // A map is needed between strings like "15" or "24" that specify the search bins ( without Btag)
  // (see findBin fundtion above) and an integer that can take from 1 to 108 (# of search bins)
  std::map <std::string,int> BinMap_HTMHT(){
    std::cout<<"-------------------BinMap_HTMHT function getting printed-----------------"<<std::endl;
      int binN=0;
      std::map <std::string , int> binMap_HTMHT;
      for(int bHtMht=1; bHtMht<=6; bHtMht++){
          std::ostringstream binS;
          binS << bHtMht;
          binN++;
          binMap_HTMHT[binS.str()]=binN;
          std::cout << "binString: " << binS.str() << " corresponing with binNumber: " <<binN << std::endl;
      }
    return binMap_HTMHT;
  }

  // Define the nbtag bin number (used for Nb-dependent isotrack veto correction)
  int findBin_NBtag(int nbtag){
    int nbtagbin = nbtag;
    if (nbtagbin>=4) nbtagbin=3;
    return nbtagbin; 
  }

  // Define the Njet&nbtag bin number ()
  int findBin_NJetNBtag(int njet, int nbtag){

    int nbtagbin = nbtag;
    if (nbtagbin>=4) nbtagbin=3; // nbtagbin:0,1,2,3

    int njetbin = -1;
    if (njet==2) njet=0;
    else if (njet>=3&&njet<=4) njet=1;
    else if (njet>=5&&njet<=6) njet=2;
    else if (njet>=7&&njet<=8) njet=3;
    else if (njet>=9)          njet=4;

    int NjetNbtagBin = 4*njet+nbtag;
    
    return NjetNbtagBin; 
  }

  int nMHT=5;
  double GetTriggerEffCorr(bool signal, double MHT, int statUnc=0, int systUnc=0){
    double w = 1;
    if(signal) {
      double central[nMHT] = {0.982, 0.985, 0.995, 1.00, 1.00};
      double stat_up[nMHT] = {0.004, 0.006, 0.003, 0.00, 0.00};
      double stat_dn[nMHT] = {0.005, 0.009, 0.007, 0.02, 0.02};
      double syst_up[nMHT] = {0.02, 0.02, 0.02, 0.02, 0.02}; 
      double syst_dn[nMHT] = {0.02, 0.02, 0.02, 0.02, 0.02}; 

      //bin lower edges
      double MHTbins[nMHT+1] = {250,300,350,500,750,9999};
		  
      int bin = -1;
      for(int b = 0; b < nMHT; ++b){
	if(MHT > MHTbins[b] && MHT <= MHTbins[b+1]){
	  bin = b;
	  break;
	}
      }
		  
      if(bin==-1) return 0.;
		  
      w = central[bin];
		  
      if(statUnc==1) w += stat_up[bin];
      else if(statUnc==-1) w -= stat_dn[bin];
		  
      if(systUnc==1) w += syst_up[bin];
      else if(systUnc==-1) w -= syst_dn[bin];
    }
    else {
      w = 1;
      //new SM/fakeMET effs not implemented yet
    }
		
    return w;
  }

  static std::pair<double,double> EvalSF(TH1 *hist, Double_t xVal) {
    // Dont use overflow bins!
    if(xVal < hist->GetXaxis()->GetXmin() )
      {
	//std::cout<<"SF: Warning xVal: "<<xVal<<" is smaller than minimum of histo: "<<hist->GetName()<<std::endl;
	xVal= hist->GetXaxis()->GetXmin()+0.01;
      }
    else if(xVal > hist->GetXaxis()->GetXmax() )
      {
	//std::cout<<"SF: Warning xVal: "<<xVal<<" is bigger than maximum of histo: "<<hist->GetName()<<" which is: "<<hist->GetXaxis()->GetXmax()<<std::endl;
	xVal= hist->GetXaxis()->GetXmax()-0.01;
      }
  
    int nxBin = hist->GetXaxis()->FindBin(xVal);

    if(nxBin > hist->GetNbinsX()) std::cout<<"SF: Problem in getting Efficiencies!"<<std::endl;
    if(nxBin > hist->GetNbinsX()) nxBin = hist->GetNbinsX();

    return std::make_pair(hist->GetBinContent(nxBin), hist->GetBinError(nxBin));
  }



  static std::pair<double,double> EvalSF(TH2 *hist, Double_t xVal, Double_t yVal) {
    // Dont use overflow bins!
    if(xVal < hist->GetXaxis()->GetXmin() )
      {
	//std::cout<<"SF: Warning xVal: "<<xVal<<" is smaller than minimum of histo: "<<hist->GetName()<<std::endl;
	xVal= hist->GetXaxis()->GetXmin()+0.01;
      }
    else if(xVal > hist->GetXaxis()->GetXmax() )
      {
	//std::cout<<"SF: Warning xVal: "<<xVal<<" is bigger than maximum of histo: "<<hist->GetName()<<" which is: "<<hist->GetXaxis()->GetXmax()<<std::endl;
	xVal= hist->GetXaxis()->GetXmax()-0.01;
      }
  
    if(yVal < hist->GetYaxis()->GetXmin() )
      {
	//std::cout<<"SF: Warning yVal: "<<yVal<<" is smaller than minimum of histo: "<<hist->GetName()<<std::endl;
	yVal= hist->GetYaxis()->GetXmin()+0.01;
      }
    else if(yVal > hist->GetYaxis()->GetXmax() )
      {
	//std::cout<<"SF: Warning yVal: "<<yVal<<" is bigger than maximum of histo: "<<hist->GetName()<<std::endl;
	yVal= hist->GetYaxis()->GetXmax()-0.01;
      }

    int nxBin = hist->GetXaxis()->FindBin(xVal);
    int nyBin = hist->GetYaxis()->FindBin(yVal);

    if(nxBin > hist->GetNbinsX() || nyBin > hist->GetNbinsY()) std::cout<<"SF: Problem in getting Efficiencies!"<<std::endl;
    if(nxBin > hist->GetNbinsX()) nxBin = hist->GetNbinsX();
    if(nyBin > hist->GetNbinsY()) nyBin = hist->GetNbinsY();

    return std::make_pair(hist->GetBinContent(nxBin, nyBin), hist->GetBinError(nxBin, nyBin));
  }

  static double GetSF(TH1 *hist, Double_t xVal) {
    return EvalSF(hist, xVal).first;
  }


  static double GetSF(TH2 *hist, Double_t xVal, Double_t yVal) {
    return EvalSF(hist, xVal, yVal).first;
  }


  static double GetSFUnc(TH1 *hist, Double_t xVal, bool addSys) {
    // addSys: for muons, 1% systematic has to be added to total uncertainty

    std::pair<double, double> SFandUnc = EvalSF(hist, xVal);

    double SF = 0.;

    if(addSys) SF = std::max(std::abs(1-SFandUnc.first), std::sqrt(SFandUnc.second*SFandUnc.second + 0.01*SFandUnc.first*0.01*SFandUnc.first));
    else SF = std::max(std::abs(1-SFandUnc.first), SFandUnc.second);

    return SF;
  }



  static double GetSFUnc(TH2 *hist, Double_t xVal, Double_t yVal, bool addSys) {
    // addSys: for muons, 1% systematic has to be added to total uncertainty

    std::pair<double, double> SFandUnc = EvalSF(hist, xVal, yVal);

    double SF = 0.;

    if(addSys) SF = std::max(std::abs(1-SFandUnc.first), std::sqrt(SFandUnc.second*SFandUnc.second + 0.01*SFandUnc.first*0.01*SFandUnc.first));
    else SF = std::max(std::abs(1-SFandUnc.first), SFandUnc.second);

    //std::cout << std::abs(1-hist->GetBinContent(nxBin, nyBin)) << " " << std::sqrt(hist->GetBinError(nxBin, nyBin)*hist->GetBinError(nxBin, nyBin) + 0.01*hist->GetBinContent(nxBin, nyBin)*0.01*hist->GetBinContent(nxBin, nyBin)) << " " << hist->GetBinError(nxBin, nyBin)<<std::endl;

    return SF;
  }


  const char* RenameBins(int x){
    const char *binString[174]={
      "nJet0_nB0_MHT0_HT0","nJet0_nB0_MHT0_HT1","nJet0_nB0_MHT0_HT2","nJet0_nB0_MHT1_HT3","nJet0_nB0_MHT1_HT4","nJet0_nB0_MHT1_HT5","nJet0_nB0_MHT2_HT6","nJet0_nB0_MHT2_HT7","nJet0_nB0_MHT3_HT8","nJet0_nB0_MHT3_HT9","nJet0_nB1_MHT0_HT0","nJet0_nB1_MHT0_HT1","nJet0_nB1_MHT0_HT2","nJet0_nB1_MHT1_HT3","nJet0_nB1_MHT1_HT4","nJet0_nB1_MHT1_HT5","nJet0_nB1_MHT2_HT6","nJet0_nB1_MHT2_HT7","nJet0_nB1_MHT3_HT8","nJet0_nB1_MHT3_HT9","nJet0_nB2_MHT0_HT0","nJet0_nB2_MHT0_HT1","nJet0_nB2_MHT0_HT2","nJet0_nB2_MHT1_HT3","nJet0_nB2_MHT1_HT4","nJet0_nB2_MHT1_HT5","nJet0_nB2_MHT2_HT6","nJet0_nB2_MHT2_HT7","nJet0_nB2_MHT3_HT8","nJet0_nB2_MHT3_HT9","nJet1_nB0_MHT0_HT0","nJet1_nB0_MHT0_HT1","nJet1_nB0_MHT0_HT2","nJet1_nB0_MHT1_HT3","nJet1_nB0_MHT1_HT4","nJet1_nB0_MHT1_HT5","nJet1_nB0_MHT2_HT6","nJet1_nB0_MHT2_HT7","nJet1_nB0_MHT3_HT8","nJet1_nB0_MHT3_HT9","nJet1_nB1_MHT0_HT0","nJet1_nB1_MHT0_HT1","nJet1_nB1_MHT0_HT2","nJet1_nB1_MHT1_HT3","nJet1_nB1_MHT1_HT4","nJet1_nB1_MHT1_HT5","nJet1_nB1_MHT2_HT6","nJet1_nB1_MHT2_HT7","nJet1_nB1_MHT3_HT8","nJet1_nB1_MHT3_HT9","nJet1_nB2_MHT0_HT0","nJet1_nB2_MHT0_HT1","nJet1_nB2_MHT0_HT2","nJet1_nB2_MHT1_HT3","nJet1_nB2_MHT1_HT4","nJet1_nB2_MHT1_HT5","nJet1_nB2_MHT2_HT6","nJet1_nB2_MHT2_HT7","nJet1_nB2_MHT3_HT8","nJet1_nB2_MHT3_HT9","nJet1_nB3_MHT0_HT0","nJet1_nB3_MHT0_HT1","nJet1_nB3_MHT0_HT2","nJet1_nB3_MHT1_HT3","nJet1_nB3_MHT1_HT4","nJet1_nB3_MHT1_HT5","nJet1_nB3_MHT2_HT6","nJet1_nB3_MHT2_HT7","nJet1_nB3_MHT3_HT8","nJet1_nB3_MHT3_HT9","nJet2_nB0_MHT0_HT0","nJet2_nB0_MHT0_HT1","nJet2_nB0_MHT0_HT2","nJet2_nB0_MHT1_HT3","nJet2_nB0_MHT1_HT4","nJet2_nB0_MHT1_HT5","nJet2_nB0_MHT2_HT6","nJet2_nB0_MHT2_HT7","nJet2_nB0_MHT3_HT8","nJet2_nB0_MHT3_HT9","nJet2_nB1_MHT0_HT0","nJet2_nB1_MHT0_HT1","nJet2_nB1_MHT0_HT2","nJet2_nB1_MHT1_HT3","nJet2_nB1_MHT1_HT4","nJet2_nB1_MHT1_HT5","nJet2_nB1_MHT2_HT6","nJet2_nB1_MHT2_HT7","nJet2_nB1_MHT3_HT8","nJet2_nB1_MHT3_HT9","nJet2_nB2_MHT0_HT0","nJet2_nB2_MHT0_HT1","nJet2_nB2_MHT0_HT2","nJet2_nB2_MHT1_HT3","nJet2_nB2_MHT1_HT4","nJet2_nB2_MHT1_HT5","nJet2_nB2_MHT2_HT6","nJet2_nB2_MHT2_HT7","nJet2_nB2_MHT3_HT8","nJet2_nB2_MHT3_HT9","nJet2_nB3_MHT0_HT0","nJet2_nB3_MHT0_HT1","nJet2_nB3_MHT0_HT2","nJet2_nB3_MHT1_HT3","nJet2_nB3_MHT1_HT4","nJet2_nB3_MHT1_HT5","nJet2_nB3_MHT2_HT6","nJet2_nB3_MHT2_HT7","nJet2_nB3_MHT3_HT8","nJet2_nB3_MHT3_HT9","nJet3_nB0_MHT0_HT0","nJet3_nB0_MHT0_HT1","nJet3_nB0_MHT1_HT2","nJet3_nB0_MHT1_HT3","nJet3_nB0_MHT2_HT4","nJet3_nB0_MHT2_HT5","nJet3_nB0_MHT3_HT6","nJet3_nB0_MHT3_HT7","nJet3_nB1_MHT0_HT0","nJet3_nB1_MHT0_HT1","nJet3_nB1_MHT1_HT2","nJet3_nB1_MHT1_HT3","nJet3_nB1_MHT2_HT4","nJet3_nB1_MHT2_HT5","nJet3_nB1_MHT3_HT6","nJet3_nB1_MHT3_HT7","nJet3_nB2_MHT0_HT0","nJet3_nB2_MHT0_HT1","nJet3_nB2_MHT1_HT2","nJet3_nB2_MHT1_HT3","nJet3_nB2_MHT2_HT4","nJet3_nB2_MHT2_HT5","nJet3_nB2_MHT3_HT6","nJet3_nB2_MHT3_HT7","nJet3_nB3_MHT0_HT0","nJet3_nB3_MHT0_HT1","nJet3_nB3_MHT1_HT2","nJet3_nB3_MHT1_HT3","nJet3_nB3_MHT2_HT4","nJet3_nB3_MHT2_HT5","nJet3_nB3_MHT3_HT6","nJet3_nB3_MHT3_HT7","nJet4_nB0_MHT0_HT0","nJet4_nB0_MHT0_HT1","nJet4_nB0_MHT1_HT2","nJet4_nB0_MHT1_HT3","nJet4_nB0_MHT2_HT4","nJet4_nB0_MHT2_HT5","nJet4_nB0_MHT3_HT6","nJet4_nB0_MHT3_HT7","nJet4_nB1_MHT0_HT0","nJet4_nB1_MHT0_HT1","nJet4_nB1_MHT1_HT2","nJet4_nB1_MHT1_HT3","nJet4_nB1_MHT2_HT4","nJet4_nB1_MHT2_HT5","nJet4_nB1_MHT3_HT6","nJet4_nB1_MHT3_HT7","nJet4_nB2_MHT0_HT0","nJet4_nB2_MHT0_HT1","nJet4_nB2_MHT1_HT2","nJet4_nB2_MHT1_HT3","nJet4_nB2_MHT2_HT4","nJet4_nB2_MHT2_HT5","nJet4_nB2_MHT3_HT6","nJet4_nB2_MHT3_HT7","nJet4_nB3_MHT0_HT0","nJet4_nB3_MHT0_HT1","nJet4_nB3_MHT1_HT2","nJet4_nB3_MHT1_HT3","nJet4_nB3_MHT2_HT4","nJet4_nB3_MHT2_HT5","nJet4_nB3_MHT3_HT6","nJet4_nB3_MHT3_HT7"};

    return binString[x-1];
  }


  static double EvtReweight(vector<double> prob,int NBtag){
    // double Reweight=0.0;
    double Loop_i=0.;
    double Loop_j=0.;
    double Loop_k=0.;
    //    double Btag_0=0.;

    if(NBtag==0){
      Loop_i=1.;
      for(int i=0;i<prob.size();i++){
	Loop_i=Loop_i*(1-prob[i]);
      }
      return Loop_i;
    }
    else if(NBtag==1){
      for(int i=0;i<prob.size();i++){
	Loop_j=prob[i];
	for(int j=0;j<prob.size();j++){
	  if(j!=i)
	    Loop_j *=(1-prob[j]);
	  else
	    continue;
	}
	Loop_i +=Loop_j;
      }
      return Loop_i;
    }
    else if(NBtag==2){
      for(int i=0;i<prob.size();i++){
	Loop_j=prob[i];
	for(int j=i+1;i<prob.size();i++){
	  Loop_k=Loop_j*prob[j];
	  for(int k=0;k<prob.size();k++){
	    if(k!=i && k!=j)
	      Loop_k *=(1-prob[k]);
	    else 
	      continue;
	  }
	}
	Loop_i +=Loop_k;
      }
      return Loop_i;
    }
    else{
      Loop_i=1-EvtReweight(prob,0)-EvtReweight(prob,1)-EvtReweight(prob,2);
      return Loop_i;
    }
  }


} // util2


  
