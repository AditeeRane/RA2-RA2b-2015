# include TGraph.h
# include  TCanvas.h

void graph(){
  TCanvas c= new TCanvas("c1","search_Bin",200,10,700,500);
  int n=16;
  Double_t x[16]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
  Double_t y[16]={1.10302,1.05589,1.11467,1.15494,1.02888,1.00077,1.04321,0.921019,1.00569,0.993849,1.01237,1.02326,0.906839,0.815019,0.990902,0.988837};
  TGraph *g=new TGraph(16,x,y);
  g->GetXAxis()->SetTitle("Nj_Nb Bin");
  g->GetYAxis()->SetTitle("NjNb correction");
  g->Draw();
  c->Update();
  c->Modified();
}
