#include "TSystem.h"
#include "TH1F.h"
#include "TClonesArray.h"
#include "TPythia8.h"
#include "TParticle.h"
#include "TDatabasePDG.h"
#include "TCanvas.h"
#include <math.h>
#include "TFile.h"
#include "TTree.h"
#include "TStyle.h"
//#include "find_mothers.h"
#include "particle_ids.h"

void sim(Int_t nev = 10000000, Int_t ndeb = 5){

	gSystem->Load("libEG");
	gSystem->Load("libEGPythia8");
	gStyle->SetOptFit(1111);

	//array of particles
	TClonesArray* particles = new TClonesArray("TParticle",1000);

	TPythia8* pythia8 = new TPythia8();

	pythia8->ReadString("HardQCD:hardccbar = on");
	pythia8->ReadString("HardQCD:hardbbbar = on");
	pythia8->ReadString("Random:setSeed = on");

	pythia8->ReadString("Random:seed = 42");
	pythia8->ReadString("PhaseSpace:pTHatMin = 4");
	pythia8->ReadString("PhaseSpace:pTHatMax = 16.5");
	//pythia8->ReadString("211:mayDecay = off");
	//pythia8->ReadString("421:mayDecay = off");
	Double_t b_energy = 200;
	Float_t north_max_eta = 2.4;
	Float_t north_min_eta = 1.2;
	Float_t south_max_eta = -1.2;
	Float_t south_min_eta = -2.2;
	pythia8->Initialize(2212,2212,b_energy);

	std::vector<float> pT;
	std::vector<float> eta;
	std::vector<double> px;
	std::vector<double> py;
	std::vector<double> pz;
	std::vector<double> e;
	std::vector<Int_t> pdg_out;
	Float_t x1,x2,pT1,pT2;
	Int_t pdg1,pdg2;
	std::vector<Int_t> mother_id;
	std::vector<float> muon_eta;
	std::vector<float> muon_pt;

	TTree *HEAVY_TREE = new TTree("HEAVY_TREE","");

	HEAVY_TREE->Branch("pT","std::vector<float>",&pT);
	HEAVY_TREE->Branch("eta","std::vector<float>",&eta);
	HEAVY_TREE->Branch("px","std::vector<double>",&px);
	HEAVY_TREE->Branch("py","std::vector<double>",&py);
	HEAVY_TREE->Branch("pz","std::vector<double>",&pz);
	HEAVY_TREE->Branch("e","std::vector<double>",&e);
	HEAVY_TREE->Branch("x1",&x1,"x1/F");
	HEAVY_TREE->Branch("x2",&x2,"x2/F");
	HEAVY_TREE->Branch("pdg1",&pdg1,"pdg1/I");
	HEAVY_TREE->Branch("pdg2",&pdg2,"pdg2/I");
	HEAVY_TREE->Branch("pT1",&pT1,"pT1/F");
	HEAVY_TREE->Branch("pT2",&pT2,"pT2/F");
	HEAVY_TREE->Branch("pdg_out","std::vector<Int_t>",&pdg_out);
	HEAVY_TREE->Branch("mother_id","std::vector<Int_t>",&mother_id);
	HEAVY_TREE->Branch("muon_eta","std::vector<float>",&muon_eta);
	HEAVY_TREE->Branch("muon_pt","std::vector<float>",&muon_pt);

	//an array of all of the particle ids for charmed and bottom hadrons that we care about
	Int_t pids[] = {411,421,413,423,415,425,431,433,435,511,521,513,523,515,525,531,533,535,541,543,545};

	Int_t charmed_baryons[] = {4122,4222,4212,4112,4224,4214,4114,4232,4132,4322,4312,4324,4314,4332,4334,4412,4422,4414,4424,4432,4434,4444};
	Int_t bottom_baryons[] = {5122,5112,5212,5222,5114,5214,5224,5132,5232,5312,5322,5314,5324,5332,5334,5142,5242,5412,5422,5414,5424,5342,5432,5434,5442,5444,5512,5522,5514,5524,5532,5534,5542,5544,5554};
	//Int_t pids[] = {211};
	int pids_length = sizeof(pids) / sizeof(pids[0]);
	int charm_length = sizeof(charmed_baryons) / sizeof(charmed_baryons[0]);
	int bottom_length = sizeof(bottom_baryons) / sizeof(bottom_baryons[0]);

	//turn off decays for all particles of interest so we can treat them as final state particles

	/*
	for(int i = 0; i < pids_length; i++){
		const char* input = (char*)(std::to_string(pids[i]) + ":mayDecay = off").c_str();
		pythia8->ReadString(input);
	}
	*/	

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


		int heavy_seen = 0;

		bool has_muon = false;

		for (Int_t ip = 4; ip < np; ip++) {
			TParticle* part = (TParticle*) particles->At(ip);
			Int_t ist = part->GetStatusCode();
			Int_t pdg = part->GetPdgCode();

			//if(ist <= 0) continue;

			


			for(int i = 0; i < light_d_mesons_length; i++){
				if(pdg == light_d_mesons[i] || pdg == - light_d_mesons[i]){

					if(part->GetLastDaughter() >= np || part->GetLastDaughter() < 0) continue;
					if(part->GetDaughter(1) >= 0){
		
						for(int j = part->GetFirstDaughter(); j <= part->GetLastDaughter();j++){
							TParticle* daughter = (TParticle*) particles->At(j);
							int daughterid = daughter->GetPdgCode();
							//cout << daughterid << '\n';
							if(daughterid == 13 || daughterid == -13){
								int muon_status = daughter->GetStatusCode();

								if(muon_status <= 0){
									cout << "Non-final state muon?\n";
									continue;
								}


								//apply pseudorapidity cuts
								float eta_p = daughter->Eta();

								if((eta_p > north_max_eta || eta_p < north_min_eta) && (eta_p > south_max_eta || eta_p < south_min_eta)) continue;

								eta.push_back(eta_p);
								pT.push_back(daughter->Pt());
								px.push_back(daughter->Px());
								py.push_back(daughter->Py());
								pz.push_back(daughter->Pz());
								e.push_back(daughter->Energy());
								mother_id.push_back(light_d_mesons[i]);
								
								
								has_muon = true;
								//muon_eta.push_back(daughter->Eta());
								//muon_pt.push_back(daughter->Pt());
								//cout << "Muon seen!" << '\n';
								continue;
							}
						} 
					}
				}
			}
	
				

				
	
				
			/*
			pT.push_back(part->Pt());
			eta.push_back(eta_p);
			px.push_back(part->Px());
			py.push_back(part->Py());
			pz.push_back(part->Pz());
			e.push_back(part->Energy());
			pdg_out.push_back(pdg);
			if(part->GetMother(1) >= 0){
				TParticle* mother = (TParticle*) particles->At(part->GetMother(1));
				mother_id.push_back(mother->GetPdgCode());
			}
			*/
			//heavy_seen = 1;
		}

/*
			for(int i = 0; i < charm_length; i++){
				if(pdg == charmed_baryons[i] or pdg == - charmed_baryons[i]){
					pT.push_back(part->Pt());
					eta.push_back(eta_p);
					px.push_back(part->Px());
					py.push_back(part->Py());
					pz.push_back(part->Pz());
					e.push_back(part->Energy());
					pdg_out.push_back(pdg);
					if(part->GetMother(1) >= 0){
						TParticle* mother = (TParticle*) particles->At(part->GetMother(1));
						mother_id.push_back(mother->GetPdgCode());
					}

					heavy_seen = 1;


				}
			}

			for(int i = 0; i < bottom_length; i++){
				if(pdg == bottom_baryons[i] or pdg == - bottom_baryons[i]){
					pT.push_back(part->Pt());
					eta.push_back(eta_p);
					px.push_back(part->Px());
					py.push_back(part->Py());
					pz.push_back(part->Pz());
					e.push_back(part->Energy());
					pdg_out.push_back(pdg);
					if(part->GetMother(1) >= 0){
						TParticle* mother = (TParticle*) particles->At(part->GetMother(1));
						mother_id.push_back(mother->GetPdgCode());
					}

					heavy_seen = 1;


				}
			}
*/

		
		if(has_muon){
			HEAVY_TREE->Fill();
			//We have to clear our vectors, otherwise they grow without bound and the computer runs out of memory
			pT.resize(0);
			eta.resize(0);
			px.resize(0);
			py.resize(0);
			pz.resize(0);
			e.resize(0);
			pdg_out.resize(0);
			mother_id.resize(0);
			muon_eta.resize(0);
		}

	}


	TFile *dir_gamma = new TFile("root_files/OHF_muons.root","CREATE");
	HEAVY_TREE->Write();

}
