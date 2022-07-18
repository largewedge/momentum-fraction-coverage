#include "hist.h"
#include "TStyle.h"

void analysis(string name,string parton_string,string experiment) {
	TFile *f = new TFile("root_files/OHF.root");

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

	entries = HEAVY_TREE->GetEntries();
	//gStyle->SetLimits(0.001,1.,"x");

	float pT_bins[5] = {5,6,8,10,16};
	TCanvas* pT_bin_canvas = new TCanvas("canvas","pT bin canvas" );

	pt_histos pT_cut_hist = pt_histos(" pT ",parton_string,pT_bin_canvas,pT_bins,0);

	int counter;
	float max_eta;

	if(experiment == "PHENIX"){
		max_eta = 0.35;
	}else if(experiment == "sPHENIX"){
		max_eta = 1.1;
	}

	for(int i = 0; i<entries; i++) {

		HEAVY_TREE->GetEntry(i);

		std::vector<float> pT_in = *pT;
		std::vector<float> eta_in = *eta;

		if(eta->size() == 0) continue;
		
		for(int id = 0; id < eta_in.size() ; id++){
			//pseudorapidity cut
			//std::vector<float> eta_in = *eta;
			if(eta_in[id] > max_eta or eta_in[id] < - max_eta) continue;



			pT_cut_hist.heavy_filler(pdg1,pdg2,x1,x2,pT_in[id]);


		}




	}

	//cout << counter << '\n';



	pT_cut_hist.display(1, 1e-3,"joint",name);	

}
