 
foreach m ( 150 167 175 183 192 200 208 217 225 233 242 250 258 267 275 283 292 300 308 317 325 333 342 350 358 367 375 383 392 400 408 417 425 433 442 450 458 467 475 483 492 500 508 517 525 533 542 550 558 567 575 583 592 600 608 617 625 633 642 650 658 667 675 683 692 700 708 717 725 733 742 750 758 767 775 783 792 800 808 817 825 833 850 875 900 925 950 1000 1050 1100 1150 1200 )

foreach d ( 1 25 50 63 75 88 100 113 125 138 150 163 175 188 200 213 225 238 250 263 275 288 300 313 325 338 350 363 375 388 400 413 425 438 450 463 475 488 500 513 525 538 550 563 575 588 600 613 625 638 650 ) 

if ( -e /eos/uscms/store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV12/scan/T2tt_mMother-${m}_mLSP-${d}_fast.root ) then
  echo  root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV12/scan/T2tt_mMother-${m}_mLSP-${d}_fast.root  


  echo root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV12/scan/T2tt_mMother-${m}_mLSP-${d}_fast.root > RA2bin_T2tt_${m}_${d}_fast
#else
  #echo 'file does not exist'
endif

end
end
