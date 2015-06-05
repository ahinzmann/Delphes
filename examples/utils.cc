


int getJetPartonIndex(float eta, float phi, TClonesArray *branchParticle){

  Double_t PartondRmin  = 0.3;
  Double_t PartonPTMin = 10;
  Double_t PartonEtaMax  = 999999; 

  int ind = -1; 
  int pdgCodeMax = -1; 
  for(int j = 0; j < branchParticle->GetEntriesFast(); ++j){
    GenParticle * particle = (GenParticle*)branchParticle->At(j);
    if(particle->PT < PartonPTMin || TMath::Abs(particle->Eta) > PartonEtaMax) continue;
    
    if(particle->M1 <=1) continue; ///from proton
    
    float dr = GetDeltaR(eta,phi,  particle->Eta, particle->Phi);
    // Get jet flavor
    int pdgCode = TMath::Abs(particle->PID);
    if(pdgCode != 21 && pdgCode > 5) continue;
    if(pdgCode == 21) pdgCode = 0;

    if( dr < PartondRmin){
      if(pdgCodeMax < pdgCode){
	pdgCodeMax = pdgCode;
	ind = j; 
      }
    }
  }
  
  return ind; 
  
}

vector<int> jetTrackAssociationDR(float eta, float phi, TClonesArray *branchTrack){
  
  //float dRmin = 0.4; 
  //float ptmin = 0.1; 
  float dRmin = 0.5; 
  float ptmin = 1; 

  vector<int> ind; 
  for(int k = 0; k < branchTrack->GetEntriesFast(); ++k) {
    Track *track = (Track*)branchTrack->At(k);
    if(track->PT < ptmin ) continue;
    float dr = GetDeltaR(eta,phi,  track->Eta, track->Phi);
    if( dr< dRmin){
      ind.push_back(k);
    }
  }
  return ind;
}


double ComputeTrackIP(Track *track) {
  double  Bz = 3.8; //magnetic field
  const Double_t c_light = 2.99792458E8; //c

  Double_t r;
  Double_t x_c, y_c, r_c, phi_c, phi_0, phi;
  Double_t rcu, rc2, dxy, xd, yd;

  r = track->PT / (track->Charge * Bz) * 1.0E9/c_light; // helix radius in [m]
  phi_0 = track->Phi;

  double x = track->X * 0.001; 
  double y = track->Y * 0.001; 
  
  // 2. helix axis coordinates
  x_c = x + r*TMath::Sin(phi_0);
  y_c = y - r*TMath::Cos(phi_0);
  r_c = TMath::Hypot(x_c, y_c);
  phi_c = TMath::ATan2(y_c, x_c);
  phi = phi_c;
  if(x_c < 0.0) phi += TMath::Pi();

  rcu = TMath::Abs(r);
  rc2 = r_c*r_c;

  // calculate coordinates of closest approach to track circle in transverse plane xd, yd, zd
  xd = x_c*x_c*x_c - x_c*rcu*r_c + x_c*y_c*y_c;
  xd  = ( rc2 > 0.0 ) ? xd / rc2 : -999;
  yd  = y_c*(-rcu*r_c + rc2);
  yd  = ( rc2 > 0.0 ) ? yd / rc2 : -999;
  //  zd  = z + (TMath::Sqrt(xd*xd+yd*yd) - TMath::Sqrt(x*x+y*y))*pz/pt;

  // calculate impact paramater
  //ang_mom = (xd*py - yd*px);
  //dxy = ang_mom/pt;
  dxy = xd * sin(track->Phi) - yd * cos(track->Phi); 
  
  double dxy0 = - track->X * sin(track->Phi) + track->Y * cos(track->Phi);
  //cout<<"dxy " << dxy <<" "<<dxy0<<endl;
  
  //return dxy;
  return dxy0/10; //now in cm

}

float track_dxyres[3][16];///condition, 1-2,2-5,5-20,>20

void set_track_dxyres(){

  // C TP
  track_dxyres[0][0]=0.0055;
  track_dxyres[0][1]=0.0033;
  track_dxyres[0][2]=0.0020;
  track_dxyres[0][3]=0.0012;

  // I TP
  track_dxyres[0][4]=0.00713;
  track_dxyres[0][5]=0.00401;
  track_dxyres[0][6]=0.00220;
  track_dxyres[0][7]=0.00126;

  // F TP
  track_dxyres[0][8]=0.01059;
  track_dxyres[0][9]=0.00581;
  track_dxyres[0][10]=0.00287;
  track_dxyres[0][11]=0.00142;

  // VF TP
  track_dxyres[0][12]=0.01397;
  track_dxyres[0][13]=0.00796;
  track_dxyres[0][14]=0.00449;
  track_dxyres[0][15]=0.00282;

  /*
  track_dxyres[1][0]=0.0055;
  track_dxyres[1][1]=0.0033;
  track_dxyres[1][2]=0.0020;
  track_dxyres[1][3]=0.0012;


  track_dxyres[2][0]=0.0056;
  track_dxyres[2][1]=0.0034;
  track_dxyres[2][2]=0.0020;
  track_dxyres[2][3]=0.0012;


  track_dxyres[3][0]=0.0053;
  track_dxyres[3][1]=0.0029;
  track_dxyres[3][2]=0.0015;
  track_dxyres[3][3]=0.0008;

  track_dxyres[4][0]=0.0051;
  track_dxyres[4][1]=0.0029;
  track_dxyres[4][2]=0.0015;
  track_dxyres[4][3]=0.0008;


  track_dxyres[5][0]=0.0061;
  track_dxyres[5][1]=0.0033;
  track_dxyres[5][2]=0.0017;
  track_dxyres[5][3]=0.0008;


  track_dxyres[6][0]=0.0049;
  track_dxyres[6][1]=0.0027;
  track_dxyres[6][2]=0.0014;
  track_dxyres[6][3]=0.0008;


  track_dxyres[7][0]=0.0061;
  track_dxyres[7][1]=0.0033;
  track_dxyres[7][2]=0.0017;
  track_dxyres[7][3]=0.0008;

  */
  // C Strawman5
  track_dxyres[1][0]=0.0051;
  track_dxyres[1][1]=0.0031;
  track_dxyres[1][2]=0.0018;
  track_dxyres[1][3]=0.0011;

  // I Strawman5
  track_dxyres[1][4]=0.00650;
  track_dxyres[1][5]=0.00370;
  track_dxyres[1][6]=0.00206;
  track_dxyres[1][7]=0.00118;

  // F Strawman5
  track_dxyres[1][8]=0.00961;
  track_dxyres[1][9]=0.00533;
  track_dxyres[1][10]=0.00273;
  track_dxyres[1][11]=0.00137;

  // VF Strawman5
  track_dxyres[1][12]=0.01406;
  track_dxyres[1][13]=0.00748;
  track_dxyres[1][14]=0.00352;
  track_dxyres[1][15]=0.00180;

/*
  track_dxyres[9][0]=0.0047;
  track_dxyres[9][1]=0.0026;
  track_dxyres[9][2]=0.0014;
  track_dxyres[9][3]=0.0008;
*/
  // C Strawman5 2 layers of new pixels
  track_dxyres[2][0]=0.0047;
  track_dxyres[2][1]=0.0026;
  track_dxyres[2][2]=0.0014;
  track_dxyres[2][3]=0.0008;

  // I Strawman5 2 layers of new pixels
  track_dxyres[2][4]=0.00604;
  track_dxyres[2][5]=0.00331;
  track_dxyres[2][6]=0.00166;
  track_dxyres[2][7]=0.00082;

  // F Strawman5 2 layers of new pixels
  track_dxyres[2][8]=0.00894;
  track_dxyres[2][9]=0.00488;
  track_dxyres[2][10]=0.00232;
  track_dxyres[2][11]=0.00102;

  // VF Strawman5 2 layers of new pixels
  track_dxyres[2][12]=0.01367;
  track_dxyres[2][13]=0.00717;
  track_dxyres[2][14]=0.00322;
  track_dxyres[2][15]=0.00135;

/*
  track_dxyres[11][0]=0.0079;
  track_dxyres[11][1]=0.0045;
  track_dxyres[11][2]=0.0025;
  track_dxyres[11][3]=0.0014;

*/  
  

}


TRandom3 *grand; 
float smeareddxy(float pt, float eta, float dxy0,int condition=0){ ////input cm
  
  int ptbin = -1; 
  if(pt>1&&pt<2){
    ptbin = 0; 
  }else if(pt<5){
    ptbin = 1; 
  }else if(pt<20){
    ptbin = 2;
  }else {
    ptbin = 3; 
  }

  if(ptbin<0){
    cout<<" smeareddxy ptbin " << ptbin <<" "<<pt<<endl; 
    exit(1);
  }
  
  int etabin = -1; 
  if(fabs(eta)<0.8){
    etabin = 0; 
  }else if(fabs(eta)<1.6){
    etabin = 1; 
  }else if(fabs(eta)<2.4){
    etabin = 2;
  }else {
    //<3.2
    etabin = 3; 
  }

  if(etabin<0){
    cout<<" smeareddxy etabin " << etabin <<" "<<eta<<endl; 
    exit(1);
  }
  
  float res = track_dxyres[condition][ptbin+4*etabin];
  float dxy = grand->Gaus(dxy0,res);
  ///  cout<<"dxy " << dxy0 <<" "<<dxy<<endl;

  return dxy;
    
}



