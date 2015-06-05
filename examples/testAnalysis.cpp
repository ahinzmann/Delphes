#include "/shome/yangyong/rootheader.h"

#include "classes/DelphesClasses.h"
#include "ExRootAnalysis/ExRootTreeReader.h"
#include "ExRootAnalysis/ExRootTreeWriter.h"
#include "ExRootAnalysis/ExRootTreeBranch.h"
#include "ExRootAnalysis/ExRootResult.h"
#include "ExRootAnalysis/ExRootUtilities.h"


#include "/shome/yangyong/backup/th3f.cc"

#include "/shome/yangyong/backup/phyUtils.cc"

#include "examples/utils.cc"


using namespace std;

int main(int argc, char *argv[]){
  char *appName = "testAnalysis";

  if(argc != 4) {
    cout << " Usage : " << appName << " input_file" << endl;
    //cout << " Usage : " << appName << " input_file output_file" << endl;
    cout << " input_file - input file in ROOT format ('Delphes' tree) doSmearDxy(1) condition(0) " << endl;
    return 1;
  }

  
  int doSmearDxy = atoi(argv[2]);
  int condition = atoi(argv[3]);

  cout<<" doSmearDxy /  condition " <<doSmearDxy <<" "<< condition <<endl;

  gROOT->SetBatch();

  int appargc = 1;
  char *appargv[] = {appName};
  TApplication app(appName, &appargc, appargv);

  TString inputFile(argv[1]);

  //TString outputFile(argv[2]);
  TString outputFile(argv[1]);
  
  cout<<"run analysis.." <<endl;

  
  gSystem->Load("libDelphes");
  TChain *chain = new TChain("Delphes");

  //  chain->Add(inputFile);
  chain->Add("dcap://t3se01.psi.ch:22125//pnfs/psi.ch/cms/trivcat/store/user/yangyong//data/Delphes/31Jul2014/HH_bbWW/HH_bbWW_1_0to5k.root");
  chain->Add("dcap://t3se01.psi.ch:22125//pnfs/psi.ch/cms/trivcat/store/user/yangyong//data/Delphes/31Jul2014/HH_bbWW/HH_bbWW_1_5to10k.root");
  
  
  ExRootTreeReader *treeReader = new ExRootTreeReader(chain);
  //Branches
  TClonesArray *branchParticle = treeReader->UseBranch("Particle");
  TClonesArray *branchJet = treeReader->UseBranch("Jet");
  //  TClonesArray *branchEFlowTrack = treeReader->UseBranch("EFlowTrack");
  //TClonesArray *branchTrack = treeReader->UseBranch("Track");
  TClonesArray *branchTrack = treeReader->UseBranch("EFlowTrack");
  
  TClonesArray *branchGenParticles = treeReader->UseBranch("GenParticles");
  

  Long64_t allEntries = treeReader->GetEntries();
  Long64_t entry;

  TFile *fnew = new TFile(Form("testAnalysis_smear%d_cond%d.root",doSmearDxy,condition),"recreate");
  makeTH1F("jetFlavour",35,-10,25);
  makeTH1F("jetPtoGenPt",50,0,5);
  
  makeTH1F("bjet_ntrk",50,0,50);
  makeTH1F("cjet_ntrk",50,0,50);
  makeTH1F("ljet_ntrk",50,0,50);

  makeTH1F("bjet_dxy_highestpt",400,-0.1,0.1);
  makeTH1F("cjet_dxy_highestpt",400,-0.1,0.1);
  makeTH1F("ljet_dxy_highestpt",400,-0.1,0.1);

  makeTH1F("bjet_dxy_highest",400,-0.1,0.1);
  makeTH1F("cjet_dxy_highest",400,-0.1,0.1);
  makeTH1F("ljet_dxy_highest",400,-0.1,0.1);

  makeTH1F("bjet_dxy_2ndhighest",400,-0.1,0.1);
  makeTH1F("cjet_dxy_2ndhighest",400,-0.1,0.1);
  makeTH1F("ljet_dxy_2ndhighest",400,-0.1,0.1);


  makeTH1F("bjet_pttrk_dxyhighest",100,0,100);

  makeTH1F("trk_vx_dtrue",100,-1,1);
  makeTH1F("trk_vy_dtrue",100,-1,1);

  makeTH1F("d0_prtcl",400,-2,2);

  
  cout << "** Reading " << allEntries << " events" << endl;

  set_track_dxyres();
  grand = new TRandom3(12345);



  //  allEntries = 10;
  
  // Loop over events
  for(entry = 0; entry < allEntries; ++entry){
    if( entry % 1000==0) cout<<" entry " << entry <<endl;
    // Load selected branches with data from specified event
    treeReader->ReadEntry(entry);
        
    // for(int k = 0; k < branchTrack->GetEntriesFast(); ++k) {
    //   Track *track = (Track*)branchTrack->At(k);
    //   int indjmin = -1; 
    //   float drmin = 0.1; 
    //   for(int j=0; j< branchGenParticles->GetEntriesFast(); j++){
    // 	GenParticle *ptcl = (GenParticle*)branchGenParticles->At(j);
    // 	float dr = GetDeltaR(track->Eta,track->Phi, ptcl->Eta, ptcl->Phi);
    // 	if(dr < drmin){
    // 	  drmin = dr; 
    // 	  indjmin = j; 
    // 	}
    //   }
    //   GenParticle *ptcl = (GenParticle*)branchGenParticles->At(indjmin);
    //   cout<<"track " << track->PT<<" "<<track->Eta<<" "<<track->Phi<<" ptcl " << ptcl->PT<<" "<<ptcl->Eta<<" "<<ptcl->Phi<<" drmin " << drmin <<" vx/vy/vz "<< track->X <<" "<<track->Y <<" "<<track->Z<<" "<< ptcl->X<<" "<<ptcl->Y <<" "<<ptcl->Z <<endl;
    // }    
    // continue;
    ////track pt is smeared, eta/phi are the same as gen particle, x/y/z are the same.


    for(int j=0; j< branchGenParticles->GetEntriesFast(); j++){                                                                                                                   
      GenParticle *ptcl = (GenParticle*)branchGenParticles->At(j);      
      if( ptcl->Charge==0) continue; 
      if( ptcl->PT <1) continue; 

      float d0 = - ptcl->X/10 * sin(ptcl->Phi) + ptcl->Y/10 * cos(ptcl->Phi);
      fillTH1F("d0_prtcl",d0);
      
    }

    
    // Loop over all jets in event
    for(int i = 0; i < branchJet->GetEntriesFast(); ++i){
      Jet *jet = (Jet*) branchJet->At(i);
      //cout<<"jet " << jet->PT <<endl;
      if(condition%10==0)
         if (jet->PT<30 || fabs(jet->Eta)>2.0) continue; 
      if(condition%10==1)
         if (jet->PT<30 || fabs(jet->Eta)<2.0 || fabs(jet->Eta)>2.5) continue; 
      if(condition%10==2)
         if (jet->PT<30 || fabs(jet->Eta)<2.5 || fabs(jet->Eta)>3.0) continue; 
      //int getJetPartonIndex(float eta, float phi, TClonesArray *branchParticle){
      int ind_gen = getJetPartonIndex(jet->Eta, jet->Phi, branchParticle);
      bool isbjet = false;
      bool iscjet = false; 
      bool isljet = false; 
      if(ind_gen>=0){
	GenParticle * particle = (GenParticle*)branchParticle->At(ind_gen);
	fillTH1F("jetFlavour",particle->PID);
	fillTH1F("jetPtoGenPt",jet->PT/particle->PT);
	if (abs(particle->PID)==5) isbjet = true;
	else if( abs(particle->PID)==4) iscjet = true;
	else isljet = true; 
      }else{
	fillTH1F("jetFlavour",0);
      }
      ///jet-track matching
      vector<int> indtrk = jetTrackAssociationDR(jet->Eta, jet->Phi,branchTrack);
      int ntrk = int(indtrk.size());
      
      if(ind_gen>=0){
	if (isbjet){
	  fillTH1F("bjet_ntrk", ntrk);
	}else if(iscjet) {
	  fillTH1F("cjet_ntrk", ntrk);
	}else if(isljet){
	  fillTH1F("ljet_ntrk", ntrk);
	}
	
      }

      if(ntrk>0){
	///highest dxy 
	///highest pt 
	float ip_highestpt = 0;
	float ip_highest = 0; 
	int ind_highestip = 0; 
	float highestpt = 0; 
	vector<pair<float,int> > allips; 
	for(int j=0; j< ntrk; j++){
	  int indt = indtrk[j];
	  Track *trk = (Track*)branchTrack->At(indt);
	  float ip = ComputeTrackIP(trk);
	  //	  cout<<" ip " <<ip << endl; 
	  if(doSmearDxy) ip = smeareddxy(trk->PT, trk->Eta, ip, condition/10);
	  //cout<<" ip_smeared " << ip <<endl;
	  allips.push_back( make_pair(ip,indt));

	  if (fabs(ip) > ip_highest){
	    ip_highest = fabs(ip);
	    ind_highestip = indt; 
	  }
	  if( trk->PT > highestpt){
	    highestpt = trk->PT; 
	    ip_highestpt = ip; 
	  }
	}

	Track *trk = (Track*)branchTrack->At(ind_highestip);//highest pt

	sort(allips.begin(),allips.end(),sort_descabs);
	float ip = allips[0].first; 

	if( ind_highestip != allips[0].second){
	  cout<<"wrong ind_highestip " << ind_highestip <<" "<<  allips[0].second <<endl; 
	}
	float ip2nd = allips[0].first; 
	if( int(allips.size()) >1){
	  ip2nd = allips[1].first;
	}

	if(isbjet) {
	  fillTH1F("bjet_dxy_2ndhighest",ip2nd);
	  fillTH1F("bjet_dxy_highestpt",ip_highestpt);
	  fillTH1F("bjet_dxy_highest",ip);
	  fillTH1F("bjet_pttrk_dxyhighest",trk->PT);
	}else if(iscjet){
	  fillTH1F("cjet_dxy_2ndhighest",ip2nd);
	  fillTH1F("cjet_dxy_highestpt",ip_highestpt);
	  fillTH1F("cjet_dxy_highest",ip);
	}else if(isljet){
	  fillTH1F("ljet_dxy_2ndhighest",ip2nd);
	  fillTH1F("ljet_dxy_highestpt",ip_highestpt);
	  fillTH1F("ljet_dxy_highest",ip);
	}
	
      }
      
      
    }
    

  }
  
  


  fnew->Write();
  fnew->Close();

}


