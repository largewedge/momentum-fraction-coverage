#include "hist.h"
#include "TStyle.h"

void analysis(string name,string parton_string,string experiment) {
	TFile *f = new TFile("root_files/direct_gammas.root");

	TTree *GAMMA_TREE = (TTree*)f->Get("GAMMA_TREE");

	Float_t pT1, pT2,pT_gamma;
	Float_t eta1,eta2,eta_gamma;
	Int_t pdg1,pdg2;
	Float_t x1,x2;
	std::vector<float> *gamma_pT1 = new std::vector<float>();
	std::vector<float> *gamma_pT2 = new std::vector<float>();
	std::vector<float> *gamma_eta1 = new std::vector<float>();
	std::vector<float> *gamma_eta2 = new std::vector<float>();
	int entries;


	GAMMA_TREE->SetBranchAddress("pT1",&pT1);
	GAMMA_TREE->SetBranchAddress("pT2",&pT2);
	GAMMA_TREE->SetBranchAddress("pT_gamma",&pT_gamma);
	GAMMA_TREE->SetBranchAddress("eta1",&eta1);
	GAMMA_TREE->SetBranchAddress("eta2",&eta2);
	GAMMA_TREE->SetBranchAddress("eta_gamma",&eta_gamma);
	GAMMA_TREE->SetBranchAddress("pdg1",&pdg1);
	GAMMA_TREE->SetBranchAddress("pdg2",&pdg2);
	GAMMA_TREE->SetBranchAddress("x1",&x1);
	GAMMA_TREE->SetBranchAddress("x2",&x2);
	GAMMA_TREE->SetBranchAddress("gammas1",&gamma_pT1);
	GAMMA_TREE->SetBranchAddress("gammas2",&gamma_pT2);
	GAMMA_TREE->SetBranchAddress("gamma_eta1",&gamma_eta1);
	GAMMA_TREE->SetBranchAddress("gamma_eta2",&gamma_eta2);

	entries = GAMMA_TREE->GetEntries();
	//gStyle->SetLimits(0.001,1.,"x");

	//TH1F* mom_frac = new TH1F("Momentum Fraction", "Momentum Fraction",100,0,1);

	frac_hists all_pT = frac_hists("all pT");
	//frac_hists pT56 = frac_hists("5 GeV < pT < 6 GeV");
	TH1F* eta_gamma_hist = new TH1F("eta","eta",100,-5,5);

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

		GAMMA_TREE->GetEntry(i);

		//mom_frac->Fill(x1);
		//
		all_pT.filler(pdg1,x1);
		all_pT.filler(pdg2,x2);
		//pT56.filler(pdg1,x1);
		//pT56.filler(pdg2,x2);
		for(int i = 0; i<gamma_eta1->size(); i++) {
			eta_gamma_hist->Fill((*gamma_eta1)[i]);
		}for(int i = 0; i < gamma_eta2->size();i++) {
			eta_gamma_hist->Fill((*gamma_eta2)[i]);
		}


		pT_cut_hist.filler(pdg1,pdg2,x1,x2,*gamma_pT1,*gamma_pT2, *gamma_eta1,*gamma_eta2,counter,max_eta);







	}

	cout << counter << '\n';



	//mom_frac->Draw();
//	all_pT.show("total");
	//
//	all_pT.draw_multiple("tudgqe");
	//pT56.draw_multiple("tdguqe");

//	TCanvas* c3 = new TCanvas();
//	eta_gamma_hist->Draw("c3");
	pT_cut_hist.display(1, 1e-3,"joint",name);	

//	pT_cut_hist.up_histos[1]->Draw();
}
