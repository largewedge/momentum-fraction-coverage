#include "TSystem.h"
#include "TH1F.h"
#include "TClonesArray.h"
#include "TPythia8.h"
#include "TParticle.h"
#include "TDatabasePDG.h"
#include "TCanvas.h"
#include <math.h>
#include "daughter_gamma.h"

void sim(Int_t nev = 10000000, Int_t ndeb = 1){

	gSystem->Load("libEG");
	gSystem->Load("libEGPythia8");
	gStyle->SetOptFit(1111);

	//array of particles
	TClonesArray* particles = new TClonesArray("TParticle",1000);

	TPythia8* pythia8 = new TPythia8();

	pythia8->ReadString("PromptPhoton:all = on");
	pythia8->ReadString("Random:setSeed = on");

	pythia8->ReadString("Random:seed = 42");
	pythia8->ReadString("PhaseSpace:pTHatMin = 4");
	pythia8->ReadString("PhaseSpace:pTHatMax = 16.5");
	Double_t b_energy = 200;
	float max_eta_PHENIX = 0.35;
	float max_eta_sPHENIX = 1.1;
	pythia8->Initialize(2212,2212,b_energy);

	Float_t pT1, pT2,pT_gamma;
	Float_t eta1,eta2,eta_gamma; 
	Int_t pdg1,pdg2; //parton IDs
	Float_t x1,x2; //parton momentum fractions
	std::vector<float> gamma_pT1;
	std::vector<float> gamma_pT2;
	std::vector<float> gamma_eta1;
	std::vector<float> gamma_eta2;
	TTree *GAMMA_TREE = new TTree("GAMMA_TREE","");


	GAMMA_TREE->Branch("pT1",&pT1,"pT1/F");
	GAMMA_TREE->Branch("pT2",&pT2,"pT2/F");
	GAMMA_TREE->Branch("pT_gamma",&pT_gamma,"pT_gamma/F");
	GAMMA_TREE->Branch("eta1",&eta1,"eta1/F");
	GAMMA_TREE->Branch("eta2",&eta2,"eta2/F");
	GAMMA_TREE->Branch("eta_gamma",&eta_gamma,"eta_gamma/F");
	GAMMA_TREE->Branch("pdg1",&pdg1,"pdg1/I");
	GAMMA_TREE->Branch("pdg2",&pdg2,"pdg2/I");
	GAMMA_TREE->Branch("x1",&x1,"x1/F");
	GAMMA_TREE->Branch("x2",&x2,"x2/F");
	GAMMA_TREE->Branch("gammas1","std::vector<float>",&gamma_pT1);
	GAMMA_TREE->Branch("gammas2","std::vector<float>",&gamma_pT2);
	GAMMA_TREE->Branch("gamma_eta1","std::vector<float>",&gamma_eta1);
	GAMMA_TREE->Branch("gamma_eta2","std::vector<float>",&gamma_eta2);



	//Event loop
	//
	for(Int_t iev = 0; iev < nev; iev++){
		pythia8->GenerateEvent();

		if(iev < ndeb) pythia8->EventListing();

		pythia8->ImportParticles(particles,"All");

		Int_t np = particles->GetEntriesFast();

		TParticle* pro1 = (TParticle*) particles->At(0); //proton 1
		TParticle* pro2 = (TParticle*) particles->At(1); //proton 2
		TParticle* part1 = (TParticle*) particles->At(2); //scattering parton 1
		TParticle* part2 = (TParticle*) particles->At(3); //scattering parton 2

		Float_t pro1_pz = pro1->Pz();
		Float_t pro2_pz = pro2->Pz();
		Float_t part1_pz = part1->Pz();
		Float_t part2_pz = part2->Pz();

		pT1 = part1->Pt();
		pT2 = part2->Pt();
		pdg1 = part1->GetPdgCode();
		pdg2 = part2->GetPdgCode();

		x1 = part1_pz/pro1_pz;
		x2 = part2_pz/pro2_pz;

		//Particle Loop
		int gamma_seen = 0;

		for (Int_t ip = 4; ip < 7; ip++) {
			TParticle* part = (TParticle*) particles->At(ip);
			Int_t ist = part->GetStatusCode();
			Int_t pdg = part->GetPdgCode();

			//assign pseudorapidity, pt etc to tree variables temporarily, only fill tree if cuts are satisfied

			pT_gamma = part->Pt();
			if(pdg != 22) continue;


			if(gamma_seen == 0){
				gamma_seen = 1;
				gamma_pT1.resize(0);
				gamma_eta1.resize(0);
				//only save final state photons that satisfy sPHENIX eta cuts, which will also satisfy PHENIX cuts
				has_daughter_gamma(particles,part,gamma_pT1,gamma_eta1,max_eta_sPHENIX);

			}else{
				gamma_pT2.resize(0);
				gamma_eta2.resize(0);
				has_daughter_gamma(particles,part,gamma_pT2,gamma_eta2,max_eta_sPHENIX);	


			}








		}
		if(gamma_seen == 1){
			GAMMA_TREE->Fill();			
		}

	}


	TFile *dir_gamma = new TFile("root_files/direct_gammas.root","CREATE");
	GAMMA_TREE->Write();

}
