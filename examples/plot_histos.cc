void setTCanvasNicev1(TCanvas *can0){
  
  
  can0->SetFillColor(0);
  can0->SetBorderMode(0);
  can0->SetBorderSize(2);
  can0->SetTickx(1);
  can0->SetTicky(1);
  can0->SetLeftMargin(0.15);
  can0->SetRightMargin(0.05);
  can0->SetTopMargin(0.05);
  can0->SetBottomMargin(0.13);
  can0->SetFrameFillStyle(0);
  can0->SetFrameBorderMode(0);
  can0->SetFrameFillStyle(0);
  can0->SetFrameBorderMode(0);
}


void setTLegendNice(TLegend *leg){
  leg->SetTextFont(62);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(1001);
  leg->SetShadowColor(0);
}



void plot_twoHist1General(TH1 *hhtmp1, TH1 *hhtmp2,  const  char *xtitle, const char *ytitle,float xmin, float xmax, int logy, float ymin, float kymax, const char *leg1Name, const char *leg2Name, const char *dirName, const char *gifName , float text_x, float text_y, const char *texName, int drawflag = 1,float legx1=0.55,float legy1=0.75,float legx2 = 0.9,float legy2 =0.9){
  
  TH1 *hh[2] = {hhtmp1, hhtmp2};
  
  if( hh[0]->Integral() <=0 || hh[1]->Integral() <=0){
    cout<<"empty histogram " << hh[0]->Integral() <<" "<< hh[1]->Integral() <<endl;
    exit(1);
  }
  
  if(drawflag==9){///normalized to unit
    float s = hh[0]->Integral();
    hh[0]->Scale(1.0/s);
    s = hh[1]->Integral();
    hh[1]->Scale(1.0/s);
  }
  
  
  TCanvas *can0 = new TCanvas("can0","c000",200,10,550,500);
  can0->SetLogy(logy);
  setTCanvasNicev1(can0);
  
  float ymax = hh[0]->GetMaximum()> hh[1]->GetMaximum() ?  hh[0]->GetMaximum() : hh[1]->GetMaximum();
  
  
  hh[1]->GetYaxis()->SetRangeUser(ymin,ymax*kymax);
  hh[0]->GetYaxis()->SetRangeUser(ymin,ymax*kymax);
  hh[1]->GetXaxis()->SetRangeUser(xmin,xmax);
  hh[0]->GetXaxis()->SetRangeUser(xmin,xmax);
  
  hh[1]->GetXaxis()->SetTitle(xtitle);
  hh[1]->GetYaxis()->SetTitle(ytitle);
  hh[0]->GetXaxis()->SetTitle(xtitle);
  hh[0]->GetYaxis()->SetTitle(ytitle);
  

  hh[1]->SetLineWidth(3);
  hh[0]->SetLineWidth(3);
  hh[1]->SetMarkerSize(1.5);
  
  
  hh[1]->SetLineColor(kRed);
  hh[1]->SetMarkerColor(kRed);
  hh[0]->SetLineColor(kBlue);
  hh[0]->SetMarkerColor(kBlue);
  
  if(drawflag==1){
    hh[1]->Draw("hist"); //MC
    hh[0]->Draw("esame"); // data
  }
  if(drawflag==9){
    hh[0]->Draw("hist");
    hh[1]->Draw("histsame"); 
  }
  

  TLegend *leg1 = new TLegend(legx1,legy1,legx2,legy2,NULL,"brNDC");
  setTLegendNice(leg1); 

  if(drawflag==1){
    leg1->AddEntry(hh[0],leg1Name,"p");
    leg1->AddEntry(hh[1],leg2Name,"l");
  }
  if(drawflag==9){
    leg1->AddEntry(hh[0],leg1Name,"l");
    leg1->AddEntry(hh[1],leg2Name,"l");
  }



  
  leg1->Draw();
  
  
  if( text_x >0 && text_y >0){
    TLatex *   tex = new TLatex(text_x, text_y, texName);
    tex->SetNDC();
    tex->SetTextSize(0.04);
    tex->SetLineWidth(2);
    tex->Draw();
  }
  
  
  TString histName = TString(Form("%s/%s.gif",dirName,gifName));
  can0->Print(histName);
  histName = TString(Form("%s/%s.pdf",dirName,gifName));
  can0->Print(histName);
  histName = TString(Form("%s/%s.C",dirName,gifName));
  can0->Print(histName);

  
  
}

void plot_oneHist1General(TH1 *hhtmp1,const  char *xtitle, const char *ytitle,float xmin, float xmax, int logy, float ymin, float kymax, char *dirName, const char *gifName , float text_x, float text_y, const char *texName, int drawflag=1){
  if(drawflag==9){///normalized to unit
    float s = hhtmp1->Integral();
    hhtmp1->Scale(1.0/s);
  }
  
  
  TCanvas *can0 = new TCanvas("can0","c000",200,10,550,500);
  can0->SetLogy(logy);
  setTCanvasNicev1(can0);
  
  float ymax = hhtmp1->GetMaximum();
  hhtmp1->GetYaxis()->SetRangeUser(ymin,ymax*kymax);
  hhtmp1->GetXaxis()->SetRangeUser(xmin,xmax);
  hhtmp1->GetXaxis()->SetTitle(xtitle);
  hhtmp1->GetYaxis()->SetTitle(ytitle);
  hhtmp1->SetLineWidth(3);
  hhtmp1->SetLineColor(kBlue);
  hhtmp1->SetMarkerColor(kBlue);
  
  if(drawflag==1){
    hhtmp1->Draw("hist");
  }
  if(drawflag==9){
    hhtmp1->Draw();
  }
  
  
  if( text_x >0 && text_y >0){
    TLatex *   tex = new TLatex(text_x, text_y, texName);
    tex->SetNDC();
    tex->SetTextSize(0.04);
    tex->SetLineWidth(2);
    tex->Draw();
  }
  
  
  TString histName = TString(Form("%s/%s.gif",dirName,gifName));
  can0->Print(histName);
  histName = TString(Form("%s/%s.pdf",dirName,gifName));
  can0->Print(histName);
  histName = TString(Form("%s/%s.C",dirName,gifName));
  can0->Print(histName);

  
  
}

///maximum 4 hists
void plot_multiHist1General(const  char *xtitle, const char *ytitle,float xmin, float xmax, int logy, float ymin, float kymax, const char *leg1Name, const char *leg2Name, const char *leg3Name, const char *leg4Name, const char *dirName, const char *gifName , float text_x, float text_y, const char *texName, TH1 *hhtmp1 =0, TH1 *hhtmp2 = 0, TH1 * hhtmp3 = 0 ,TH1 *hhtmp4 = 0, int drawflag = 1,float legx1=0.55,float legy1=0.75,float legx2 = 0.9,float legy2 =0.9){

  gStyle->SetOptStat(0);
  
  
  cout<<"zz " <<endl; 

  TH1 *hh[10] = {hhtmp1, hhtmp2,hhtmp3, hhtmp4};
  
  if( hh[0]->Integral() <=0 || hh[1]->Integral() <=0 ){
    cout<<"empty histogram " << hh[0]->Integral() <<" "<< hh[1]->Integral() <<endl;
    exit(1);
  }

  cout<<"zz1 " <<endl; 
  
  if(drawflag==9){///normalized to unit
    float s = hh[0]->Integral();
    hh[0]->Scale(1.0/s);
    s = hh[1]->Integral();
    hh[1]->Scale(1.0/s);
    
    if(hh[2]!=0){
      s = hh[2]->Integral();
      hh[2]->Scale(1.0/s);
    }
    if(hh[3]!=0){
      s = hh[3]->Integral();
      hh[3]->Scale(1.0/s);
    }
  }
  
  cout<<"dd " <<endl; 
  
  TCanvas *can0 = new TCanvas("can0","c000",200,10,550,500);
  can0->SetLogy(logy);
  setTCanvasNicev1(can0);
  
  float ymax = hh[0]->GetMaximum()> hh[1]->GetMaximum() ?  hh[0]->GetMaximum() : hh[1]->GetMaximum();
  if(hh[2]!=0){
    ymax = ymax > hh[2]->GetMaximum()? ymax: hh[2]->GetMaximum();
  }
  if(hh[3]!=0){
    ymax = ymax > hh[3]->GetMaximum()? ymax: hh[3]->GetMaximum();
  }
  
  
  hh[1]->GetYaxis()->SetRangeUser(ymin,ymax*kymax);
  hh[0]->GetYaxis()->SetRangeUser(ymin,ymax*kymax);
  hh[1]->GetXaxis()->SetRangeUser(xmin,xmax);
  hh[0]->GetXaxis()->SetRangeUser(xmin,xmax);
  
  hh[1]->GetXaxis()->SetTitle(xtitle);
  hh[1]->GetYaxis()->SetTitle(ytitle);
  hh[0]->GetXaxis()->SetTitle(xtitle);
  hh[0]->GetYaxis()->SetTitle(ytitle);
  

  hh[1]->SetLineWidth(3);
  hh[0]->SetLineWidth(3);

  hh[1]->SetMarkerSize(1.5);
  hh[1]->SetLineColor(kRed);
  hh[1]->SetMarkerColor(kRed);

  hh[0]->SetLineColor(kBlue);
  hh[0]->SetMarkerColor(kBlue);
  
  if(hh[2]!=0){
    hh[2]->SetLineWidth(3);
    hh[2]->SetLineColor(kCyan+3);
    hh[2]->SetMarkerColor(kCyan+3);
    hh[2]->SetMarkerSize(1.5);
    hh[2]->SetMarkerStyle(25);
  }
  
  if(hh[3]!=0){
    hh[3]->SetLineWidth(3);
  }
  

  
  if(drawflag==1){
    hh[0]->Draw("hist"); //MC
    hh[1]->Draw("esame"); // data
    if(hh[2]!=0) hh[2]->Draw("esame");
    if(hh[3]!=0) hh[3]->Draw("esame"); 
    

  }
  if(drawflag==9){
    hh[0]->Draw("hist");
    hh[1]->Draw("histsame"); 

    if(hh[2]!=0) hh[2]->Draw("histsame");
    if(hh[3]!=0) hh[3]->Draw("histsame"); 
    
  }

  cout<<"xx " <<endl;

  TLegend *leg1 = new TLegend(legx1,legy1,legx2,legy2,NULL,"brNDC");
  setTLegendNice(leg1); 

  if(drawflag==1){
    leg1->AddEntry(hh[0],leg1Name,"l");
    leg1->AddEntry(hh[1],leg2Name,"pl");
    if(hh[2]!=0) leg1->AddEntry(hh[2],leg3Name,"lp");
    if(hh[3]!=0) leg1->AddEntry(hh[3],leg4Name,"lp");
  }
  if(drawflag==9){
    leg1->AddEntry(hh[0],leg1Name,"l");
    leg1->AddEntry(hh[1],leg2Name,"l");
    if(hh[2]!=0)  leg1->AddEntry(hh[2],leg3Name,"l");
    if(hh[3]!=0)  leg1->AddEntry(hh[3],leg4Name,"l");
  }



  
  leg1->Draw();
  
  
  if( text_x >0 && text_y >0){
    TLatex *   tex = new TLatex(text_x, text_y, texName);
    tex->SetNDC();
    tex->SetTextSize(0.04);
    tex->SetLineWidth(2);
    tex->Draw();
  }
  

    
  TString histName = TString(Form("%s/%s.gif",dirName,gifName));
  can0->Print(histName);
  histName = TString(Form("%s/%s.pdf",dirName,gifName));
  can0->Print(histName);
  histName = TString(Form("%s/%s.C",dirName,gifName));
  can0->Print(histName);
  
  
}


void plot_oneTGraph(TGraph *hhtmp1, const char *xtitle, const char *ytitle,float xmin, float xmax, int logy,float ymin, float ymax,  int drawFlag, const char *dirName,const char *gifName , float text_x, float text_y, const char *texName,
		    float text_x1 = 0 , float text_y1 = 0 , const char *texName1 = ""
		    ){

  TCanvas *can0 = new TCanvas("can0","c000",200,10,550,500);
  setTCanvasNicev1(can0);


  if(logy==1){
    can0->SetLogy(1);
  }else if(logy==2){
    can0->SetLogx(1);
  }



  hhtmp1->GetYaxis()->SetRangeUser(ymin,ymax);
  hhtmp1->GetXaxis()->SetRangeUser(xmin,xmax);
  hhtmp1->GetXaxis()->SetTitle(xtitle);
  hhtmp1->GetYaxis()->SetTitle(ytitle);
  hhtmp1->SetLineWidth(3);
  hhtmp1->SetMarkerSize(1.5);

  hhtmp1->SetMarkerStyle(20);
  if(drawFlag==1){
    hhtmp1->Draw("ap");
  }
  if(drawFlag==2){
    hhtmp1->Draw("apl");
  }
  if(drawFlag==3){
    hhtmp1->Draw("apc");
  }
  
  if( text_x >0 && text_y >0){
    TLatex *   tex = new TLatex(text_x, text_y, texName);
    tex->SetNDC();
    tex->SetTextSize(0.04);
    tex->Draw();
  }
  
  if( text_x1 >0 && text_y1 >0){
    TLatex *   tex = new TLatex(text_x1, text_y1, texName1);
    tex->SetNDC();
    tex->SetTextSize(0.04);
    tex->Draw();
  }

      
  TString histName = TString(Form("%s/%s.gif",dirName,gifName));
  can0->Print(histName);
  histName = TString(Form("%s/%s.pdf",dirName,gifName));
  can0->Print(histName);
  histName = TString(Form("%s/%s.C",dirName,gifName));
  can0->Print(histName);

}

void plot_twoTGraph(TGraph *hhtmp1, TGraph *hhtmp2, const char *xtitle, const char *ytitle,float xmin, float xmax, int logy,float ymin, float ymax, const char *leg1Name, const char *leg2Name,int drawFlag, const char *dirName,const char *gifName , float text_x, float text_y, const char *texName,float legx1=0.55,float legy1=0.75,float legx2 = 0.9,float legy2 =0.9){
  TCanvas *can0 = new TCanvas("can0","c000",200,10,550,500);
  setTCanvasNicev1(can0);

  if(logy==1){
    can0->SetLogy(1);
  }else if(logy==2){
    can0->SetLogx(1);
  }
  


  hhtmp1->GetYaxis()->SetRangeUser(ymin,ymax);
  hhtmp1->GetXaxis()->SetRangeUser(xmin,xmax);
  hhtmp1->GetXaxis()->SetTitle(xtitle);
  hhtmp1->GetYaxis()->SetTitle(ytitle);
  hhtmp1->SetLineWidth(3);
  hhtmp1->SetMarkerSize(1.5);
  hhtmp1->SetMarkerStyle(20);
  
  hhtmp2->SetLineWidth(3);
  hhtmp2->SetMarkerSize(1.5);
  hhtmp2->SetMarkerStyle(21);		       
  hhtmp2->SetMarkerColor(kBlue);
  hhtmp2->SetLineColor(kBlue);
  
  
  if(drawFlag==1){
    hhtmp1->Draw("ap");
    hhtmp2->Draw("p");
  }
  if(drawFlag==2){
    hhtmp1->Draw("apl");
    hhtmp2->Draw("pl");
  }
  if(drawFlag==3){
    hhtmp1->Draw("apc");
    hhtmp2->Draw("pc");
  }


  
  if( text_x >0 && text_y >0){
    TLatex *   tex = new TLatex(text_x, text_y, texName);
    tex->SetNDC();
    tex->SetTextSize(0.04);
    tex->Draw();
  }

  
  TLegend *leg1 = new TLegend(legx1,legy1,legx2,legy2,NULL,"brNDC");
  setTLegendNice(leg1);
  if(drawFlag==1){
    leg1->AddEntry(hhtmp1,leg1Name,"p");
    leg1->AddEntry(hhtmp2,leg2Name,"p");
  }
  if(drawFlag==2){
    leg1->AddEntry(hhtmp1,leg1Name,"l");
    leg1->AddEntry(hhtmp2,leg2Name,"l");
  }
  if(drawFlag==3){
    leg1->AddEntry(hhtmp1,leg1Name,"pl");
    leg1->AddEntry(hhtmp2,leg2Name,"pl");
  }


  leg1->Draw();


      
  TString histName = TString(Form("%s/%s.gif",dirName,gifName));
  can0->Print(histName);
  histName = TString(Form("%s/%s.pdf",dirName,gifName));
  can0->Print(histName);
  histName = TString(Form("%s/%s.C",dirName,gifName));
  can0->Print(histName);

}

void plot_threeTGraph(TGraph *hhtmp1, TGraph *hhtmp2, TGraph *hhtmp3,const char *xtitle, const char *ytitle,float xmin, float xmax, int logy,float ymin, float ymax, const char *leg1Name, const char *leg2Name,const char *leg3Name,int drawFlag, const char *dirName,const char *gifName , float text_x, float text_y, const char *texName,float legx1=0.55,float legy1=0.75,float legx2 = 0.9,float legy2 =0.9){
  TCanvas *can0 = new TCanvas("can0","c000",200,10,550,500);
  setTCanvasNicev1(can0);
  can0->SetLogy(logy);

  hhtmp1->GetYaxis()->SetRangeUser(ymin,ymax);
  hhtmp1->GetXaxis()->SetRangeUser(xmin,xmax);
  hhtmp1->GetXaxis()->SetTitle(xtitle);
  hhtmp1->GetYaxis()->SetTitle(ytitle);
  hhtmp1->SetLineWidth(3);
  hhtmp1->SetMarkerSize(1.5);
  hhtmp1->SetMarkerStyle(20);
  
  hhtmp2->SetLineWidth(3);
  hhtmp2->SetMarkerSize(1.5);
  hhtmp2->SetMarkerStyle(21);		       
  hhtmp2->SetMarkerColor(kBlue);
  hhtmp2->SetLineColor(kBlue);
  
  hhtmp3->SetLineWidth(3);
  hhtmp3->SetMarkerSize(1.5);
  hhtmp3->SetMarkerStyle(22);		       
  hhtmp3->SetMarkerColor(kRed);
  hhtmp3->SetLineColor(kRed);
  
  if(drawFlag==1){
    hhtmp1->Draw("ap");
    hhtmp2->Draw("p");
    hhtmp3->Draw("p");
  }
  if(drawFlag==2){
    hhtmp1->Draw("apl");
    hhtmp2->Draw("pl");
    hhtmp3->Draw("pl");
  }
  
  if( text_x >0 && text_y >0){
    TLatex *   tex = new TLatex(text_x, text_y, texName);
    tex->SetNDC();
    tex->SetTextSize(0.04);
    tex->Draw();
  }

  
  TLegend *leg1 = new TLegend(legx1,legy1,legx2,legy2,NULL,"brNDC");
  setTLegendNice(leg1);
  if(drawFlag==1){
    leg1->AddEntry(hhtmp1,leg1Name,"p");
    leg1->AddEntry(hhtmp2,leg2Name,"p");
    leg1->AddEntry(hhtmp3,leg3Name,"p");
  }
  if(drawFlag==2){
    leg1->AddEntry(hhtmp1,leg1Name,"pl");
    leg1->AddEntry(hhtmp2,leg2Name,"pl");
    leg1->AddEntry(hhtmp3,leg3Name,"pl");
  }


  leg1->Draw();


      
  TString histName = TString(Form("%s/%s.gif",dirName,gifName));
  can0->Print(histName);
  histName = TString(Form("%s/%s.pdf",dirName,gifName));
  can0->Print(histName);
  histName = TString(Form("%s/%s.C",dirName,gifName));
  can0->Print(histName);

}


void plot_threeTGraphErrors(TGraphErrors *hhtmp1, TGraphErrors *hhtmp2, TGraphErrors *hhtmp3,const char *xtitle, const char *ytitle,float xmin, float xmax, int logy,float ymin, float ymax, const char *leg1Name, const char *leg2Name,const char *leg3Name,int drawFlag, const char *dirName,const char *gifName , float text_x, float text_y, const char *texName,float legx1=0.55,float legy1=0.75,float legx2 = 0.9,float legy2 =0.9){
  TCanvas *can0 = new TCanvas("can0","c000",200,10,550,500);
  setTCanvasNicev1(can0);
  can0->SetLogy(logy);

  hhtmp1->GetYaxis()->SetRangeUser(ymin,ymax);
  hhtmp1->GetXaxis()->SetRangeUser(xmin,xmax);
  hhtmp1->GetXaxis()->SetTitle(xtitle);
  hhtmp1->GetYaxis()->SetTitle(ytitle);
  hhtmp1->SetLineWidth(3);
  hhtmp1->SetMarkerSize(1.5);
  hhtmp1->SetMarkerStyle(20);
  
  hhtmp2->SetLineWidth(3);
  hhtmp2->SetMarkerSize(1.5);
  hhtmp2->SetMarkerStyle(21);		       
  hhtmp2->SetMarkerColor(kBlue);
  hhtmp2->SetLineColor(kBlue);
  
  hhtmp3->SetLineWidth(3);
  hhtmp3->SetMarkerSize(1.5);
  hhtmp3->SetMarkerStyle(22);		       
  hhtmp3->SetMarkerColor(kRed);
  hhtmp3->SetLineColor(kRed);

  hhtmp1->GetXaxis()->SetNdivisions(505);


  if(drawFlag==1){
    hhtmp1->Draw("ap");
    hhtmp2->Draw("p");
    hhtmp3->Draw("p");
  }
  if(drawFlag==2){
    hhtmp1->Draw("apl");
    hhtmp2->Draw("pl");
    hhtmp3->Draw("pl");
  }
  
  if( text_x >0 && text_y >0){
    TLatex *   tex = new TLatex(text_x, text_y, texName);
    tex->SetNDC();
    tex->SetTextSize(0.04);
    tex->Draw();
  }

  
  TLegend *leg1 = new TLegend(legx1,legy1,legx2,legy2,NULL,"brNDC");
  setTLegendNice(leg1);
  if(drawFlag==1){
    leg1->AddEntry(hhtmp1,leg1Name,"p");
    leg1->AddEntry(hhtmp2,leg2Name,"p");
    leg1->AddEntry(hhtmp3,leg3Name,"p");
  }
  if(drawFlag==2){
    leg1->AddEntry(hhtmp1,leg1Name,"pl");
    leg1->AddEntry(hhtmp2,leg2Name,"pl");
    leg1->AddEntry(hhtmp3,leg3Name,"pl");
  }


  leg1->Draw();


      
  TString histName = TString(Form("%s/%s.gif",dirName,gifName));
  can0->Print(histName);
  histName = TString(Form("%s/%s.pdf",dirName,gifName));
  can0->Print(histName);
  histName = TString(Form("%s/%s.C",dirName,gifName));
  can0->Print(histName);

}


/// Input, at lest 3 histograms last one is data. 2nd last in signal 
void plot_stack_4hist(const char *xtitle ,float xstart, float xend, const char *dirName, const char *gifName, float ymin,float ykmax, bool logy,TH1F *h1, TH1F *h2, TH1F *h3, 
		      TH1F *h4, TH1F *h5, TH1F *h6, TH1F *h7,float legx1=0.55,float legy1=0.75,float legx2 = 0.9,float legy2 =0.9, float text_x=-1, float text_y=-1, const char *texName="", float text_x1 = -1, float text_y1 = -1, 
		      const char *texName1="",bool plotdata= false){
  
  float ymax = h1->GetMaximum() > h2->GetMaximum() ? h1->GetMaximum() : h2->GetMaximum(); 
  ymax = ymax > h5->GetMaximum() ? ymax : h5->GetMaximum(); 
  
  
  TCanvas *can0 = new TCanvas("can0", "c000",205,47,550,500);
  gStyle->SetOptStat(0);
  setTCanvasNicev1(can0);
  can0->SetLogy(logy);
  
  
  THStack *hs = new THStack();
  hs->SetName("hs");
  hs->SetTitle("test stacked histograms");
  hs->SetMinimum(ymin);
  hs->SetMaximum(ymax*ykmax);
  int icol[5]= {3,2,920};
  h1->SetFillColor(kGreen+1);
  h2->SetFillColor(kCyan);

  hs->Add(h1);
  hs->Add(h2);
  
  if(h3!=0){
    h3->SetFillColor(kMagenta-5);
    hs->Add(h3);
  }
  if(h4!=0){
    h4->SetFillColor(kYellow+2);
    //h4->SetFillColor(kRed-3);
    hs->Add(h4);
  }

  cout<<"h1/h2 "<<h1->Integral()<<" "<<h2->Integral()<<endl;
  
  hs->Draw();
  hs->GetXaxis()->SetTitle(xtitle);

  string ytitle ; 
  float binwidth = h1->GetBinWidth(1);
  
  string ss = xtitle; 
  if( ss.find("GeV") != string::npos){
    ytitle = string(Form("Events / %d GeV", int(binwidth+0.001)));
  }else{
    ytitle = string(Form("Events / %2.1f", binwidth));
  }
  
  
  hs->GetYaxis()->SetTitle(ytitle.c_str());
  hs->GetXaxis()->SetRangeUser(xstart,xend);

  if(h5!=0){
    h5->SetLineWidth(3);
    h5->SetLineColor(kRed);
    h5->Draw("histsame");
  }
  if(h6!=0){ //anther signal 
    h6->SetLineWidth(3);
    h6->SetLineColor(kBlue);
    h6->Draw("histsame");
  }
  
  if(plotdata){
    h7->Draw("esame");
  }

  ////0.489011,0.6075949,0.9212454,0.9240506
  
  TLegend *leg1 = new TLegend(legx1,legy1,legx2,legy2,NULL,"brNDC");
  setTLegendNice(leg1);

  if(plotdata){
    leg1->AddEntry(h7,"Data","p");
  }
  

  leg1->AddEntry(h1,"t#bar{t}","f");
  leg1->AddEntry(h2,"EWK","f");

  if(h3!=0){
    leg1->AddEntry(h3,"Single t","f");
  }
  if(h4!=0){
    leg1->AddEntry(h4,"t#bar{t}+#gamma/Z/W","f");
  }

  //leg1->AddEntry(h5,"M_{#chi} = 100 GeV","l");
  leg1->AddEntry(h5,"Signal","l");
  if(h6!=0){
    //leg1->AddEntry(h6,"M_{#chi} = 600 GeV","l");
    leg1->AddEntry(h6,"Signal 2","l");
  }
  
  leg1->Draw();


  if( text_x >0 && text_y >0){
    TLatex *   tex = new TLatex(text_x, text_y, texName);
    tex->SetNDC();
    tex->SetTextSize(0.04);
    tex->Draw();
  }

  if( text_x1 >0 && text_y1 >0){
    TLatex *   tex = new TLatex(text_x1, text_y1, texName1);
    tex->SetNDC();
    tex->SetTextSize(0.04);
    tex->Draw();
  }
  
  

  TString histName = TString(Form("%s/%s.gif",dirName,gifName));
  can0->Print(histName);
  histName = TString(Form("%s/%s.pdf",dirName,gifName));
  can0->Print(histName);
  histName = TString(Form("%s/%s.C",dirName,gifName));
  can0->Print(histName);


  
}




