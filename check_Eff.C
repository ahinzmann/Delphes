#include "examples/rootheader.h"

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

  //  cout<< htmp->Integral(b1,b2) <<" "<< htmp->Integral() <<endl;
  // cout<<"eff " << eff <<" +/- " << effErr <<endl;
  // exit(1);

}


void check_Eff(TString inputfile = "testAnalysis_smear1_cond0.root", TString postfix="default"){
  
  //  TFile *f1 =new TFile("testAnalysis.root","read");

  //TFile *f1 = new TFile("testAnalysis_smear1_cond0.root","read");
  TFile *f1 = new TFile(inputfile,"read");


  TH1F *hbpt = (TH1F*)f1->Get("th1f_bjet_dxy_highestpt");
  TH1F *hcpt = (TH1F*)f1->Get("th1f_cjet_dxy_highestpt");
  TH1F *hlpt = (TH1F*)f1->Get("th1f_ljet_dxy_highestpt");
  
  TH1F *hb = (TH1F*)f1->Get("th1f_bjet_dxy_highest");
  TH1F *hc = (TH1F*)f1->Get("th1f_cjet_dxy_highest");
  TH1F *hl = (TH1F*)f1->Get("th1f_ljet_dxy_highest");
   
  TH1F *hb2 = (TH1F*)f1->Get("th1f_bjet_dxy_2ndhighest");
  TH1F *hc2 = (TH1F*)f1->Get("th1f_cjet_dxy_2ndhighest");
  TH1F *hl2 = (TH1F*)f1->Get("th1f_ljet_dxy_2ndhighest");

  TH1F *hb3 = (TH1F*)f1->Get("th1f_bjet_dxy_3rdhighest");
  TH1F *hc3 = (TH1F*)f1->Get("th1f_cjet_dxy_3rdhighest");
  TH1F *hl3 = (TH1F*)f1->Get("th1f_ljet_dxy_3rdhighest");

  TH1F *hbptdxy = (TH1F*)f1->Get("th1f_bjet_pttrk_dxyhighest");
  TH1F *hbptpt = (TH1F*)f1->Get("th1f_bjet_pttrk_highestpt");
   
  
  TFile *fnew = new TFile("check_Eff.root","recreate");

  float binwidth = hb->GetBinWidth(1);
  int nbins = hb->GetNbinsX();
  float eff; 
  float effE; 
  float cuts[1000];
  float cutsErr[1000];
  float effb[1000];
  float effbErr[1000];
  float effc[1000];
  float effcErr[1000];
  float effl[1000];
  float efflErr[1000];
  float rejl[1000];
  float rejlErr[1000];

  cout<<" nbins " << nbins <<endl;
  int n = 0; 

  for(int b=1; b<= nbins/2; b++){
    float cut = binwidth *b; 

    if(n<1000){
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

      rejl[n] = eff;
      rejlErr[n] = effE;

      cuts[n] = cut; 
      cutsErr[n] = 0; 

      n ++; 

    }


  }

  float dmin = 0.1; 
  int indmin = 0; 
  for(int j=0; j<n; j++){
    if(fabs( effb[j]-0.7)<dmin){
      dmin = fabs(effb[j]-0.7);
      indmin = j; 
    }
  }
  cout<<"working point 0.7  cut " << cuts[indmin]<<endl;
  cout<<"effb " << effb[indmin]<<"+/-"<<effbErr[indmin]<<endl;
  cout<<"effc " << effc[indmin]<<"+/-"<<effcErr[indmin]<<endl;
  cout<<"effl " << effl[indmin]<<"+/-"<<efflErr[indmin]<<endl;

  float dmin1 = 0.1; 
  int indmin1 = 0; 
  for(int j=0; j<n; j++){
    if(fabs( effl[j]-0.03)<dmin1){
      dmin1 = fabs(effl[j]-0.03);
      indmin1 = j; 
    }
  }
  cout<<"fake working point 0.03  cut " << cuts[indmin1]<<endl;
  cout<<"effb " << effb[indmin1]<<"+/-"<<effbErr[indmin1]<<endl;
  cout<<"effc " << effc[indmin1]<<"+/-"<<effcErr[indmin1]<<endl;
  cout<<"effl " << effl[indmin1]<<"+/-"<<efflErr[indmin1]<<endl;


  TGraphErrors *g0 = new TGraphErrors(n, effb, rejl, effbErr, rejlErr);
  g0->Draw("ap");
  g0->Write("effbl");

  // void plot_oneTGraphErr(TGraphError *hhtmp1, const char *xtitle, const char *ytitle,float xmin, float xmax, int logy,float ymin, float ymax,  int drawFlag, const char *dirName,const char *gifName , \
  // 			 float text_x, float text_y, const char *texName,
  // 			 float text_x1 = 0 , float text_y1 = 0 , const char *texName1 = ""
  // 			 ){


  plot_oneTGraphErr(g0,"b jet efficiency","light jet efficiency",0.3,1,1,0.001,1,1,"plots","effbl"+postfix,0.2,0.8,"");
  

  TGraphErrors *g1 = new TGraphErrors(n, cuts,effb, cutsErr, effbErr);
  g1->Draw("ap");
  g1->Write("effb");

  TGraphErrors *g2 = new TGraphErrors(n, cuts,effc,  cutsErr, effcErr);
  g2->Draw("ap");
  g2->Write("effc");

  TGraphErrors *g3 = new TGraphErrors(n, cuts, effl, cutsErr,efflErr);
  g3->Draw("ap");
  g3->Write("effl");

  plot_threeTGraphErrors(g1,g2,g3,"d_{0} (mm)","Efficiency",0,0.02,1,0.01,1,"b-jet","c-jet","light-jet",1,".","eff"+postfix,0.2,0.8,"");
  
  // plot_threeTGraphErrors(TGraphErrors *hhtmp1, TGraphErrors *hhtmp2, TGraphErrors *hhtmp3,const char *xtitle, const char *y\
  // 			 title,float xmin, float xmax, int logy,float ymin, float ymax, const char *leg1Name, const char *leg2Name,const char *leg3Name\
  // 			 ,int drawFlag, const char *dirName,const char *gifName , float text_x, float text_y, const char *texName,float legx1=0.55,floa\
  // 			 t legy1=0.75,float legx2 = 0.9,float legy2 =0.9){

  // void plot_multiHist1General(const  char *xtitle, const char *ytitle,float xmin, float xmax, int logy, float ymin, float kymax, const char *leg1Name, const char *leg2Name, const char *leg3Name, cons\
  // 			      t char *leg4Name, const char *dirName, const char *gifName , float text_x, float text_y, const char *texName, TH1 *hhtmp1 =0, TH1 *hhtmp2 = 0, TH1 * hhtmp3 = 0 ,TH1 *hhtmp4 = 0, int drawflag = 1,fl\
  // 			      oat legx1=0.55,float legy1=0.75,float legx2 = 0.9,float legy2 =0.9){

  
  hb->Rebin(2);
  hc->Rebin(2);
  hl->Rebin(2);
  plot_multiHist1General("d_{xy} (cm)","Fraction of events",-0.2,0.2,0,0,1.2,"b jet","c jet","light jet", "","plots","dxy"+postfix,0.2,0.9,"",hb,hc,hl,0,9,0.67,0.75,0.93,0.93);
  
  hb2->Rebin(2);
  hc2->Rebin(2);
  hl2->Rebin(2);
  plot_multiHist1General("d_{xy} (cm)","Fraction of events",-0.2,0.2,0,0,1.2,"b jet","c jet","light jet", "","plots","dxy2"+postfix,0.2,0.9,"",hb2,hc2,hl2,0,9,0.67,0.75,0.93,0.93);

  std::cerr << hb << std::endl;
  std::cerr << hb2 << std::endl;
  hb3->Rebin(2);
  hc3->Rebin(2);
  hl3->Rebin(2);
  plot_multiHist1General("d_{xy} (cm)","Fraction of events",-0.2,0.2,0,0,1.2,"b jet","c jet","light jet", "","plots","dxy3"+postfix,0.2,0.9,"",hb3,hc3,hl3,0,9,0.67,0.75,0.93,0.93);

  hbpt->Rebin(2);
  hcpt->Rebin(2);
  hlpt->Rebin(2);
  plot_multiHist1General("d_{xy} (cm)","Fraction of events",-0.2,0.2,0,0,1.2,"b jet","c jet","light jet", "","plots","dxypt"+postfix,0.2,0.9,"",hbpt,hcpt,hlpt,0,9,0.67,0.75,0.93,0.93);

  hbptpt->Rebin(2);
  plot_oneHist1General(hbptpt,"p_{T} (GeV)","Fraction of events",0,100,0,0,1.2,"plots","ptpt",0.2,0.9,"ptpt"+postfix);

  hbptdxy->Rebin(2);
  plot_oneHist1General(hbptdxy,"p_{T} (GeV)","Fraction of events",0,100,0,0,1.2,"plots","ptdxy",0.2,0.9,"ptdxy"+postfix);


  fnew->Write();
  fnew->Close();
  
    

  
}
