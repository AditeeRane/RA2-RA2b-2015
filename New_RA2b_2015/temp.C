{
TFile *_file0 = TFile::Open("TauHad/Stack/LostLepton2_MuonEfficienciesFromstacked.root");
TCanvas *c= new TCanvas("c","c",600,600);
gStyle->SetOptStat(0);
leg = new TLegend(0.1,0.7,0.48,0.9);
acc0b=(TH1D*) _file0->Get("hAcc_0b_")->Clone();
accnon0b=(TH1D*) _file0->Get("hAcc_non0b_")->Clone();
acc0b->SetLineColor(1);
accnon0b->SetLineColor(2);
acc0b->Draw();
accnon0b->Draw("same");
leg->AddEntry(acc0b,"0b","l");
leg->AddEntry(accnon0b,">0b","l");
leg->Draw();
c->Print("AcceptancePerB.pdf");





}
