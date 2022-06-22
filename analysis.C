#include "hist.h"
#include "TStyle.h"

void analysis() {
	TFile *f = new TFile("direct_gammas.root");
	TTree *PHENIX = (TTree*)f->Get("PHENIX");

	Float_t pT1, pT2,pT_gamma;
	Float_t eta1,eta2,eta_gamma;
	Int_t pdg1,pdg2;
	Float_t x1,x2;
	std::vector<float> *gamma_pT1 = new std::vector<float>();
	std::vector<float> *gamma_pT2 = new std::vector<float>();
	std::vector<float> *gamma_eta1 = new std::vector<float>();
	std::vector<float> *gamma_eta2 = new std::vector<float>();

	PHENIX->SetBranchAddress("pT1",&pT1);
	PHENIX->SetBranchAddress("pT2",&pT2);
	PHENIX->SetBranchAddress("pT_gamma",&pT_gamma);
	PHENIX->SetBranchAddress("eta1",&eta1);
	PHENIX->SetBranchAddress("eta2",&eta2);
	PHENIX->SetBranchAddress("eta_gamma",&eta_gamma);
	PHENIX->SetBranchAddress("pdg1",&pdg1);
	PHENIX->SetBranchAddress("pdg2",&pdg2);
	PHENIX->SetBranchAddress("x1",&x1);
	PHENIX->SetBranchAddress("x2",&x2);
	PHENIX->SetBranchAddress("gammas1",&gamma_pT1);
	PHENIX->SetBranchAddress("gammas2",&gamma_pT2);
	PHENIX->SetBranchAddress("gamma_eta1",&gamma_eta1);
	PHENIX->SetBranchAddress("gamma_eta2",&gamma_eta2);

	int entries = PHENIX->GetEntries();

	//gStyle->SetLimits(0.001,1.,"x");

	//TH1F* mom_frac = new TH1F("Momentum Fraction", "Momentum Fraction",100,0,1);

	frac_hists all_pT = frac_hists("all pT");
	//frac_hists pT56 = frac_hists("5 GeV < pT < 6 GeV");
	TH1F* eta_gamma_hist = new TH1F("eta","eta",100,-5,5);

	float pT_bins[5] = {5,6,8,10,16};
	TCanvas* pT_bin_canvas = new TCanvas("canvas","pT bin canvas" );

	pt_histos pT_cut_hist = pt_histos("binned by pT","udt",pT_bin_canvas,pT_bins,0);

	int counter;

	for(int i = 0; i<entries; i++) {
		PHENIX->GetEntry(i);
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

		pT_cut_hist.filler(pdg1,pdg2,x1,x2,*gamma_pT1,*gamma_pT2,counter);







	}

	cout << counter << '\n';



	//mom_frac->Draw();
//	all_pT.show("total");
	//
//	all_pT.draw_multiple("tudgqe");
	//pT56.draw_multiple("tdguqe");

//	TCanvas* c3 = new TCanvas();
//	eta_gamma_hist->Draw("c3");
	pT_cut_hist.display(1, 1e-3);	

//	pT_cut_hist.up_histos[1]->Draw();
}
