 
foreach m ( 600 650 700 750 800 850 900 950 1000 1050 1100 1150 1200 1250 1300 1350 1400 1450 1500 1550 1600 1650 1700 1750 1800 1850 1900 1950 2000 2050 2100 2150 2200 2250 2300 )

foreach d ( 1 100 200 300 400 450 475 500 525 550 575 600 625 650 675 700 725 750 775 800 825 850 875 900 925 950 975 1000 1025 1050 1075 1100 1125 1150 1175 1200 1225 1250 1275 1300 1325 1350 1375 1400 1425 1450 1475 1500 1525 1550 1575 1600 ) 

if ( -e /eos/uscms/store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV12/scan/T5qqqqVV_mMother-${m}_mLSP-${d}_fast.root ) then
  echo  root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV12/scan/T5qqqqVV_mMother-${m}_mLSP-${d}_fast.root  


  echo root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV12/scan/T5qqqqVV_mMother-${m}_mLSP-${d}_fast.root > RA2bin_T5qqqqVV_${m}_${d}_fast
#else
  #echo 'file does not exist'
endif

end
end