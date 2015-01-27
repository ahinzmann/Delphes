#include "examples/otheader.h"

#include "examples/plot_histos.cc"


void get_eff_IPcut(TH1 *htmp,float cut, float &eff, float &effErr){ //cut has to be the bin edge
  
  //int nbins = htmp->GetNbinsX();
  int b1 = htmp->FindFixBin(-cut+0.00001);
  int b2 = htmp->FindFixBin(cut-0.00001);
  
  if (htmp->Integral()>= 0){
    //    cout<<"cut " << cut <<" "<<b1 <<" "<<b2 <<" " <<  htmp->Integral(b1,b2)<<" "<< htmp->Integral()<<endl;
    eff =  1- htmp->Integral(b1,b2)/htmp->Integral();
    effErr = sqrt( eff*(1-eff)/htmp->Integral());
  }else{
    eff = 0; 
    effErr = 0; 
  }

  
}


void check_Eff(){
  
  TFile *f1 =new TFile("out1.root","read");
  
  TH1F *hb = (TH1F*)f1->Get("hBJetIP");
  TH1F *hc = (TH1F*)f1->Get("hCJetIP");
  TH1F *hl = (TH1F*)f1->Get("hLightJetIP");

  TFile *fnew = new TFile("check_Eff.root","recreate");
    

  float binwidth = hb->GetBinWidth(1);
  int nbins = hb->GetNbinsX();
  float eff; 
  float effE; 
  float cuts[200];
  float cutsErr[200];
  float effb[200];
  float effbErr[200];
  float effc[200];
  float effcErr[200];
  float effl[200];
  float efflErr[200];
  int n = 0; 
  for(int b=1; b<= nbins/2; b++){
    float cut = binwidth *b; 

    if(n<200){
      get_eff_IPcut(hb,cut,eff,effE);
      effb[n] = eff; 
      effbErr[n] = effE; 
      //      cout<<"n " << n <<" "<< effb[n]<<"+/-" << effbErr[n] <<endl;

      get_eff_IPcut(hc,cut,eff,effE);
      effc[n] = eff;
      effcErr[n] = effE;
      
      get_eff_IPcut(hl,cut,eff,effE);
      effl[n] = eff;
      efflErr[n] = effE;

      cuts[n] = cut; 
      cutsErr[n] = 0; 

      n ++; 

    }


  }

  TGraphErrors *g1 = new TGraphErrors(n, cuts,effb, cutsErr, effbErr);
  g1->Draw("ap");
  g1->Write("effb");

  TGraphErrors *g2 = new TGraphErrors(n, cuts,effc,  cutsErr, effcErr);
  g2->Draw("ap");
  g2->Write("effc");

  TGraphErrors *g3 = new TGraphErrors(n, cuts, effl, cutsErr,efflErr);
  g3->Draw("ap");
  g3->Write("effl");

  plot_threeTGraphErrors(g1,g2,g3,"d_{0} (mm)","Efficiency",0,0.02,1,0.01,1,"b-jet","c-jet","light-jet",1,".","eff",0.2,0.8,"");
  
  // plot_threeTGraphErrors(TGraphErrors *hhtmp1, TGraphErrors *hhtmp2, TGraphErrors *hhtmp3,const char *xtitle, const char *y\
  // 			 title,float xmin, float xmax, int logy,float ymin, float ymax, const char *leg1Name, const char *leg2Name,const char *leg3Name\
  // 			 ,int drawFlag, const char *dirName,const char *gifName , float text_x, float text_y, const char *texName,float legx1=0.55,floa\
  // 			 t legy1=0.75,float legx2 = 0.9,float legy2 =0.9){


  fnew->Write();
  fnew->Close();
  
    

  
}
