//          ./Analysis delphes_output.root



//remember to add your plots here
struct MyPlots
{
  TH1 *hJetBDeltaR;
  TH1 *hJetTrackDeltaR;
  TH1 *hJetEta;
  TH1 *hBJetEta;
  TH1 *hParticleEta;
  TH1 *hBJetIP;
  TH1 *hLightJetIP; 
  TH1 *hCJetIP;  
};

//------------------------------------------------------------------------------
class ExRootResult;
class ExRootTreeReader;
//------------------------------------------------------------------------------

//Function to compute dR
double DeltaR(TVector3 candidate1, TVector3 candidate2) 
{
  Double_t deta = candidate1.Eta()-candidate2.Eta();
  Double_t dphi = candidate1.Phi()-candidate2.Phi();
  return sqrt( deta*deta+dphi*dphi );
}
//------------------------------------------------------------------------------

//Function to compute the impact parameter
double ComputeIP(Track *LeadingTrack) {
  
  Int_t Bz = 3.8; //magnetic field
  const Double_t c_light = 2.99792458E8; //c
  
  Double_t r;
  Double_t x_c, y_c, r_c, phi_c, phi_0, phi;
  Double_t rcu, rc2, dxy, xd, yd;
  
  
  TVector3 vLeadingTrack;
  
  
  vLeadingTrack.SetPtEtaPhi(LeadingTrack->PT, LeadingTrack->Eta, LeadingTrack->Phi);
  
  r = LeadingTrack->PT / (LeadingTrack->Charge * Bz) * 1.0E9/c_light; // helix radius in [m]
  phi_0 = TMath::ATan2(vLeadingTrack.Py(), vLeadingTrack.Px()); // [rad] in [-pi, pi]
  
  // 2. helix axis coordinates
  x_c = LeadingTrack->X + r*TMath::Sin(phi_0);
  y_c = LeadingTrack->Y - r*TMath::Cos(phi_0);
  r_c = TMath::Hypot(x_c, y_c);
  phi_c = TMath::ATan2(y_c, x_c);
  phi = phi_c;
  if(x_c < 0.0) phi += TMath::Pi();
  
  rcu = TMath::Abs(r);
  rc2 = r_c*r_c;
  
  // calculate coordinates of closest approach to track circle in transverse plane xd, yd, zd
  xd = x_c*x_c*x_c - x_c*rcu*r_c + x_c*y_c*y_c;
  xd = (rc2 > 0.0) ? xd / rc2 : -999;
  yd = y_c*(-rcu*r_c + rc2);
  yd = (rc2 > 0.0) ? yd / rc2 : -999;
  //     zd = z + (TMath::Sqrt(xd*xd + yd*yd) - TMath::Sqrt(x*x + y*y))*pz/pt;

  /// dxy parameter. (This is the transverse impact parameter w.r.t. to (0,0,0) ONLY if refPoint is close to (0,0,0): \
  //see parametrization definition above for details). See also function dxy(myBeamSpot) below.
  //double dxy() const { return ( - vx() * py() + vy() * px() ) / pt(); }
  
  // calculate impact paramater

  //dxy = (xd*vLeadingTrack.Py() - yd*vLeadingTrack.Px())/LeadingTrack->PT;

  dxy = (- LeadingTrack->X *vLeadingTrack.Py() + LeadingTrack->Y  *vLeadingTrack.Px())/LeadingTrack->PT;
  
  return dxy;
}

//------------------------------------------------------------------------------

void BookHistograms(ExRootResult *result, MyPlots *plots)
{
  
  THStack *stack;
  TLegend *legend;
// TPaveText *comment;
  
  // AddHist1D(name, title, xlabel, ylabel, nxbins, xmin, xmax
  plots->hJetBDeltaR = result->AddHist1D(
    "hJetBDeltaR", "#DeltaR_{jet, quark}",
    "#DeltaR_{jet, quark}", "number of jets",
      100, 0., 0.6);
  
  plots->hJetTrackDeltaR = result->AddHist1D(
    "hJetTrackDeltaR", "#DeltaR_{jet, track}",
    "R^{jet} - R^{track})", "number of jets",
     100, 0., 0.6);
  
  plots->hJetEta = result->AddHist1D(
    "jetEta", "Jet Eta",
    "Jet eta", "number of jets",
    100, -2.5, 2.5);
  
  plots->hBJetEta = result->AddHist1D(
    "BjetEta", "BJet Eta",
    "BJet eta", "number of jets",
    100, -2.5, 2.5);
  
  plots->hParticleEta = result->AddHist1D(
    "ParticleEta", "Particle Eta",
    "Particle eta","number of jets",
    100, -2.5, 2.5);
  
  plots->hBJetIP = result->AddHist1D(
    "hBJetIP", "b-jet track IP",
    "b-jet track IP","number of tracks",
    //100, -25., 25.);
    100, -0.5, 0.5);
  
  plots->hCJetIP = result->AddHist1D(
    "hCJetIP", "c-jet track IP",
    "c-jet track IP","number of tracks",
    //100, -25., 25.);
    100, -0.5, 0.5);
  
  plots->hLightJetIP = result->AddHist1D(
    "hLightJetIP", "Light jet track IP",
    "Light jet track IP","number of tracks",
    //100, -25., 25.);  
    100, -0.5, 0.5);  
  
  plots->hBJetIP->SetLineColor(kRed);
  plots->hLightJetIP->SetLineColor(kBlue);
  plots->hLightJetIP->SetLineColor(kGreen);
  
  // book 1 stack of 2 histograms
  
  stack = result->AddHistStack("Impact parameter", "IP for b and light");
  stack->Add(plots->hBJetIP);
  stack->Add(plots->hLightJetIP);
  stack->Add(plots->hCJetIP);
  
  // book legend for stack of 2 histograms
  
  legend = result->AddLegend(0.72, 0.86, 0.98, 0.98);
  legend->AddEntry(plots->hBJetIP, "b", "l");
  legend->AddEntry(plots->hCJetIP, "c ", "l");
  legend->AddEntry(plots->hLightJetIP, "light ", "l");
  
  
  // attach legend to stack (legend will be printed over stack in .eps file)
  
  result->Attach(stack, legend);
}

//------------------------------------------------------------------------------
void AnalyseEvents(ExRootTreeReader *treeReader, MyPlots *plots)
{

  //Branches
  TClonesArray *branchParticle = treeReader->UseBranch("Particle");
  TClonesArray *branchJet = treeReader->UseBranch("Jet");
  TClonesArray *branchEFlowTrack = treeReader->UseBranch("EFlowTrack");
  //   TClonesArray *branchEFlowTrack = treeReader->UseBranch("Track");
  
  //Objects 
  
  GenParticle *particle;
  
  Jet *jet;
  
  Track *track;
  Track *LeadingTrack;
  
  TVector3 vParticle; 
  TVector3 vJet;
  TVector3 vTrack;
  
  //Cuts
  Double_t pTMin  = 15;
  Double_t etaMax = 2.5;
  Double_t dRmin  = 0.5;
  Double_t PartondRmin  = 0.3;
  
  Double_t PartonPTMin = 1.0;
  Double_t PartonEtaMax  = 4.0;   
  
  
  Int_t i, j, k, pdgCode, pdgCodeMax, trackpT, trackpTMax; 
  
  Double_t dxy; //Impact Parameter
  
  Long64_t allEntries = treeReader->GetEntries();
  Long64_t entry;  
  
  cout << "** Reading " << allEntries << " events" << endl;

  // Loop over events
  //     for(entry = 0; entry < 1; ++entry)
  for(entry = 0; entry < allEntries; ++entry)
  {

    if( entry % 1000==0) cout<<" entry " << entry <<endl;

    // Load selected branches with data from specified event
    treeReader->ReadEntry(entry);
    
    // Loop over all jets in event
    for(i = 0; i < branchJet->GetEntriesFast(); ++i)
    {
      Double_t PartonJetdR = -1;
      
      pdgCodeMax = -1;
      
      jet = (Jet*) branchJet->At(i);
      
      //Apply cuts
      if(jet->PT < pTMin || TMath::Abs(jet->Eta) > etaMax) continue;
      
      //Define a jet 3-vector
      vJet.SetPtEtaPhi(jet->PT, jet->Eta, jet->Phi);
      
      plots->hJetEta->Fill(jet->Eta);
      
      
      // Loop over GenParticles
      for(j = 0; j < branchParticle->GetEntriesFast(); ++j)
      {
	particle = (GenParticle*)branchParticle->At(j);
	
	if(particle->PT < PartonPTMin || TMath::Abs(particle->Eta) > PartonEtaMax) continue;
	
	vParticle.SetPtEtaPhi(particle->PT, particle->Eta, particle->Phi);
	
	plots->hParticleEta->Fill(particle->Eta);
	
	// Get jet flavor
	pdgCode = TMath::Abs(particle->PID);
	if(pdgCode != 21 && pdgCode > 5) continue;
	if(pdgCode == 21) pdgCode = 0;	  
	
	Double_t dR = DeltaR(vParticle, vJet);
	if(dR < PartondRmin)
	{
	  if(pdgCodeMax < pdgCode){
	    PartonJetdR = dR;
	    pdgCodeMax = pdgCode;
	  }
	}
      } 
      
      if(pdgCodeMax == 0) pdgCodeMax = 21;
      if(pdgCodeMax == -1) pdgCodeMax = 0;
      
      if( (pdgCodeMax != 21 && pdgCodeMax > 5) || pdgCodeMax == 0 ) continue;
      
      // If b jet
      if(pdgCodeMax == 5)
      {	  
	plots->hJetBDeltaR->Fill(PartonJetdR);
	plots->hBJetEta->Fill(jet->Eta);
	
      }
      
      trackpTMax = -1;
      
      // Loop over tracks
      for(k = 0; k < branchEFlowTrack->GetEntriesFast(); ++k)
      {
	track = (Track*)branchEFlowTrack->At(k);
	if(track->PT < 0.1) continue;
	vTrack.SetPtEtaPhi(track->PT, track->Eta, track->Phi);
	Double_t dR = DeltaR(vTrack, vJet);
	
	// Get highest pT track in jet cone
	if(dR > dRmin)continue;
	trackpT = track->PT;
	plots->hJetTrackDeltaR->Fill(dR);
	
	if(trackpTMax < trackpT){
	  trackpTMax = trackpT;
	  LeadingTrack = track;
	}
      }
      
      
      // calculate Impact parameter
      if(LeadingTrack ) dxy = ComputeIP(LeadingTrack);
      
      // If b jet
      if(pdgCodeMax == 5)
      {	  
	plots->hBJetIP->Fill(dxy);  
	
      }
      
      // If c jet
      else if(pdgCodeMax == 4)
      {	  
	plots->hCJetIP->Fill(dxy);
      }
      
      // If light jet
      else
      {	  
	plots->hLightJetIP->Fill(dxy);
      }
      
    }	  
  }
  plots->hBJetIP->Scale(1./plots->hBJetIP->Integral());
  plots->hCJetIP->Scale(1./plots->hCJetIP->Integral());
  plots->hLightJetIP->Scale(1./plots->hLightJetIP->Integral());
  
  plots->hBJetIP->Draw();
  plots->hLightJetIP->Draw("same");
  
}

//------------------------------------------------------------------------------

void PrintHistograms(ExRootResult *result, MyPlots *plots)
{
  
  
  result->Print("png");
}

//------------------------------------------------------------------------------


void Analysis (const char *inputFile, const char *outputFile)
{
  gSystem->Load("libDelphes");
  
  TChain *chain = new TChain("Delphes");
  chain->Add(inputFile);
  
  ExRootTreeReader *treeReader = new ExRootTreeReader(chain);
  ExRootResult *result = new ExRootResult();
  
  MyPlots *plots = new MyPlots;
  
  BookHistograms(result, plots);
  
  AnalyseEvents(treeReader, plots);
  
  //  PrintHistograms(result, plots);
  
  result->Write(outputFile);
  
  cout << "** Exiting..." << endl;
  
  delete plots;
  delete result;
  delete treeReader;
  delete chain;
}
