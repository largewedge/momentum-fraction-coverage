#include "TStyle.h"
#include "TH1F.h"
#include "particle_ids.h"
#include "TSystem.h"

void analysis() {
	TFile *f = new TFile("root_files/OHF_muons.root");

	TTree *HEAVY_TREE = (TTree*)f->Get("HEAVY_TREE");

	std::vector<float> *pT = new std::vector<float>();
	std::vector<float> *eta = new std::vector<float>();
	std::vector<double> *px = new std::vector<double>();
	std::vector<double> *py = new std::vector<double>();
	std::vector<double> *pz = new std::vector<double>();
	std::vector<double> *e = new std::vector<double>();
	std::vector<Int_t> *pdg_out = new std::vector<Int_t>();
	Float_t x1,x2,pT1,pT2;
	Int_t pdg1,pdg2;
	std::vector<Int_t> *mother_id = new std::vector<Int_t>();
	std::vector<float> *muon_pT = new std::vector<float>();
	int entries;


	HEAVY_TREE->SetBranchAddress("pT",&pT);
	HEAVY_TREE->SetBranchAddress("eta",&eta);
	HEAVY_TREE->SetBranchAddress("px",&px);
	HEAVY_TREE->SetBranchAddress("py",&py);
	HEAVY_TREE->SetBranchAddress("pz",&pz);
	HEAVY_TREE->SetBranchAddress("e",&e);
	HEAVY_TREE->SetBranchAddress("x1",&x1);
	HEAVY_TREE->SetBranchAddress("x2",&x2);
	HEAVY_TREE->SetBranchAddress("pdg1",&pdg1);
	HEAVY_TREE->SetBranchAddress("pdg2",&pdg2);
	HEAVY_TREE->SetBranchAddress("pT1",&pT1);
	HEAVY_TREE->SetBranchAddress("pT2",&pT2);
	HEAVY_TREE->SetBranchAddress("pdg_out",&pdg_out);
	HEAVY_TREE->SetBranchAddress("mother_id",&mother_id);
	HEAVY_TREE->SetBranchAddress("muon_pT",&muon_pT);

	entries = HEAVY_TREE->GetEntries();
	//gStyle->SetLimits(0.001,1.,"x");

	float pT_bins[7] = {1.25, 1.50,2.00,2.50,3.00,3.50,5.00};

	int pT_bins_length = sizeof(pT_bins) / sizeof(pT_bins[0]);


	TH1F *histos[6];

	for(int i = 0; i < pT_bins_length; i++){
		histos[i] = new TH1F(Form("hxf%d", i), Form("cut %d",i),100,0,12);
	}




	for(int i = 0; i<entries; i++) {

		HEAVY_TREE->GetEntry(i);

		std::vector<float> pT_in = *pT;
		std::vector<float> eta_in = *eta;
		std::vector<float> muon_pT_in = *muon_pT;
		std::vector<Int_t> mother_id_in = *mother_id;

		if(muon_pT_in.size() == 0) continue;
		
		for(int id = 0; id < muon_pT_in.size() ; id++){
			//pseudorapidity cut
			//std::vector<float> eta_in = *eta;
			//if(eta_in[id] > max_eta or eta_in[id] < - max_eta) continue;
			//if(eta_in[id] > north_max_eta || eta_in[id] < north_min_eta) continue;

			bool d_found = false;

			for(int j = 0; j < light_d_mesons_length; j++){
				if(mother_id_in[id] == light_b_mesons[j]){
					d_found = true;
					continue;
				}
			}

			if(!d_found) continue;

			for(int bin = 0; bin < pT_bins_length-1; bin++){
				float pT_bin_left = pT_bins[bin];
				float pT_bin_right = pT_bins[bin+1];
				
				//cout << pT_bin_left << " - " << pT_bin_right << '\n';
				if(muon_pT_in[id] > pT_bin_left and muon_pT_in[id] < pT_bin_right){
					
					histos[bin]->Fill(pT_in[id]);
				}
			}
			


		}




	}

	TCanvas* c1 = new TCanvas();
	
	for(int i = 0; i < pT_bins_length-1; i++){
		histos[i]->SetLineColor(i+1);
		histos[i]->Draw("SAME");
	}

	c1->SaveAs("plots/D_pT.pdf");
	

	//histos[0]->Draw();

	//cout << counter << '\n';




}
