#include "logbin.h"

class frac_hists{

	public:
		TH1F* total;
		TH1F* up;
		TH1F* down;
		TH1F* aup;
		TH1F* adown;
		TH1F* gluon;
		frac_hists(char* name) {
			//hacky C way to concatenate two char*'s
			char buf[100];
			strcpy(buf,(char*)"Total Momentum Fraction ");
			strcat(buf,name);
			total = new TH1F(buf,buf,100,0,1);

			strcpy(buf,(char*)"q_{u} Momentum Fraction ");
			strcat(buf,name);
			up = new TH1F(buf,buf,100,0,1);

			strcpy(buf,(char*)"q_{d} Momentum Fraction ");
			strcat(buf,name);
			down = new TH1F(buf,buf,100,0,1);

			strcpy(buf,(char*)"#overline{q_{u}} Momentum Fraction ");
			strcat(buf,name);
			aup = new TH1F(buf,buf,100,0,1);

			strcpy(buf,(char*)"#overline{q_{d}} Momentum Fraction ");
			strcat(buf,name);
			adown = new TH1F(buf,buf,100,0,1);

			strcpy(buf,(char*)"g Momentum Fraction ");
			strcat(buf,name);
			gluon = new TH1F(buf,buf,100,0,1);

		}

		void filler(int id,float data) {
			total->Fill(data);
			if(id == 2) {
				up->Fill(data);
			}if(id == 1){
				down->Fill(data);
			}if(id == -1) {
				adown->Fill(data);
			}if(id == -2){
				aup->Fill(data);
			}if(id == 21) {
				gluon->Fill(data);
			}
		}

		void show(string parton){
			TCanvas* c1 = new TCanvas();
			auto legend = new TLegend(0.1,0.7,0.48,0.9);
			if(parton == "total" or parton == "all"){
				total->SetLineColor(4);
				legend->AddEntry(total,"Total Parton Momentum Fraction","f");
				total->Draw();	
			}
			if(parton == "up" or parton == "all"){
				up->SetLineColor(6);
				legend->AddEntry(up, "q_{u} Momentum Fraction","f");
				up->Draw("SAME");
			}if(parton == "down" or parton == "all"){
				down->SetLineColor(7);
				legend->AddEntry(down, "q_{d} Momentum Fraction","f");
				down->Draw("SAME");
			}if(parton == "aup" or parton == "all"){
				aup->SetLineColor(8);
				legend->AddEntry(aup, "#bar{q_{u}} Momentum Fraction","f");
				aup->Draw("SAME");
			}if(parton == "adown" or parton == "all"){
				adown->SetLineColor(9);
				legend->AddEntry(adown, "#bar{q_{d}} Momentum Fraction","f");
				adown->Draw("SAME");
			}if(parton == "gluon" or parton == "all"){
				gluon->SetLineColor(3);
				legend->AddEntry(gluon,"g Momentum Fraction","f");
				gluon->Draw("SAME");
			}
			c1->SetLogx();
			c1->SetLogy();
			

			legend->Draw("SAME");
		}

		void draw_multiple(string partons) {
			TCanvas* c2 = new TCanvas();
			auto legend1 = new TLegend(0.1,0.7,0.48,0.9);

			for(int i = 0; i<partons.length(); i++) {
				if(partons.at(i) == 't') {	
					total->SetLineColor(4);
					legend1->AddEntry(total, "total Momentum Fraction","f");
					total->Draw("SAME");
				}
				if(partons.at(i) == 'u') {
					up->SetLineColor(6);
					legend1->AddEntry(up, "q_{u} Momentum Fraction","f");
					up->Draw("SAME");
				}if(partons.at(i) == 'd') {
					down->SetLineColor(7);
					legend1->AddEntry(down, "q_{d} Momentum Fraction", "f");
					down->Draw("SAME");
				}if(partons.at(i) == 'q') {
					aup->SetLineColor(8);
					legend1->AddEntry(aup, "#bar{q_{u}} Momentum Fraction", "f");
					aup->Draw("SAME");
				}if(partons.at(i) == 'e') {
					adown->SetLineColor(9);
					legend1->AddEntry(adown, "#bar{q_{d}} Momentum Fraction", "f");
					adown->Draw("SAME");
				}if(partons.at(i) == 'g') {
					gluon->SetLineColor(3);
					legend1->AddEntry(gluon, "g Momentum Fraction","f");
					gluon->Draw("SAME");
				}
			}

			legend1->Draw("SAME");
			c2->SetLogx();
			c2->SetLogy();
			
		
		}
};


/*

Parton chars:
up quark = u
down quark = d
antiup quark = a
antidown quark = p
total (count all partons) = t
gluon = g


*/

class pt_histos{
	TCanvas* canvas;
	int both;
	string partons;


	const static int max_pT_bins = 6;
	const static int max_partons = 6;

	float pT_bins[max_pT_bins] = {};

	//gStyle->GetXaxis()->SetLimits(1e-3,1);
	
	char possible_partons[max_partons] = {'u','d','a','p','t','g'};
	


	public:
		TH1F *histos[max_pT_bins][max_partons];

		pt_histos(char* name,string partons_in, TCanvas* canvas_in,float pT_bins_in[], int show_both_hist) {
			partons = partons_in;
			canvas = canvas_in;
			both = show_both_hist;


			for(int i = 0; i< max_pT_bins - 1;i++){
				pT_bins[i] = *(pT_bins_in + i);
			}

			for(int i = 0; i<max_pT_bins - 1; i++) {
				for(int j = 0; j < partons.length(); j++){
					char parton = partons[j];
					float pT_bin_right = pT_bins[i+1];
					float pT_bin_left = pT_bins[i];
					
					create_new_histogram(pT_bin_right,pT_bin_left,histos[i][j],name,j);
					

				}
			}
			






		}

		void create_new_histogram(float right_bound_in, float left_bound_in,TH1F* &histogram,char* name,int parton_idx){
			char buf[100];
			char disp_buf[100];

			const char* left_bound = std::to_string(left_bound_in).c_str();

			const char* right_bound = std::to_string(right_bound_in).c_str();


			char parton = partons[parton_idx];
			if(parton == 't'){
				strcpy(buf,(char*)"Total ");
			}
			else if(parton == 'u'){
				strcpy(buf,(char*)"q_{u} Momentum Fraction ");
			} if(parton == 'd'){
				strcpy(buf,(char*)"q_{d} Momentum Fraction ");
			} if(parton == 'a'){
				strcpy(buf,(char*)"#bar{q_{u}} Momentum Fraction ");
			} if(parton == 'p'){
				strcpy(buf,(char*)"#bar{q_{d}} Momentum Fraction ");
			} if(parton == 'g') {
				strcpy(buf,(char*)"g Momentum Fraction");
			}
			
			strcat(buf,name);
			strcat(buf,left_bound);
			strcat(buf,(char*)" -  ");
			strcat(buf,right_bound);


			//generate the name that will actually be displayed on the canvas
			strcpy(disp_buf,(char*)"pT ");
			strcat(disp_buf,left_bound);
			strcat(disp_buf," GeV - ");
			strcat(disp_buf,right_bound);
			strcat(disp_buf," GeV");


			histogram = new TH1F(buf,disp_buf,100,-3,0);

			BinLogX(histogram);


		}


		void bin_filler(std::vector<float> pT_gamma1,std::vector<float> pT_gamma2, std::vector<float> eta1,std::vector<float> eta2,float pT_bin_right, float pT_bin_left, TH1F *histo, float data,float max_eta){
	
			if(pT_gamma1.size() == 0 && pT_gamma2.size() == 0) {
				return 0;
			}
			for(int i = 0; i < pT_gamma1.size();i++){
			
				if(pT_gamma1[i] < pT_bin_right and pT_gamma1[i] > pT_bin_left and (eta1[i] < max_eta and eta1[i] > - max_eta)){
					histo->Fill(data);

				}
			}
			for(int i = 0; i < pT_gamma2.size();i++){
				if(pT_gamma2[i] < pT_bin_right and pT_gamma2[i] > pT_bin_left and (eta2[i] < max_eta and eta2[i] > - max_eta)){
					histo->Fill(data);
				}
			}


		}



		void filler(int id1,int id2, float data1,float data2, std::vector<float> pT_gamma1, std::vector<float> pT_gamma2, std::vector<float> eta1,std::vector<float> eta2, int &counter, float max_eta){


			int num_gammas1 = pT_gamma1.size();
			int num_gammas2 = pT_gamma2.size();
			for(int i = 0; i< partons.length(); i++){
				//if there are no final state photons, don't plot anything
				if(num_gammas1 == 0 and num_gammas2 == 0) break;

				for(int j = 0; j < max_pT_bins - 1; j++){
					char parton = partons[i];
					float pT_bin_right = pT_bins[j+1];
					float pT_bin_left = pT_bins[j];
					if(parton == 'u'){
						if(id1 == 2){
							bin_filler(pT_gamma1,pT_gamma2,eta1,eta2,pT_bin_right,pT_bin_left,histos[j][i],data1,max_eta);
						}
						if(id2 == 2) {
							bin_filler(pT_gamma1,pT_gamma2,eta1,eta2,pT_bin_right,pT_bin_left,histos[j][i],data2,max_eta);
						}
					}

					if(parton == 'd'){
						if(id1 == 1){
							bin_filler(pT_gamma1,pT_gamma2,eta1,eta2,pT_bin_right,pT_bin_left,histos[j][i],data1,max_eta);
						}
						if(id2 == 1){
							bin_filler(pT_gamma1,pT_gamma2,eta1,eta2,pT_bin_right,pT_bin_left,histos[j][i],data2,max_eta);
						}
					}
					if(parton == 't'){
						bin_filler(pT_gamma1,pT_gamma2,eta1,eta2,pT_bin_right,pT_bin_left,histos[j][i],data1,max_eta);
						bin_filler(pT_gamma1,pT_gamma2,eta1,eta2,pT_bin_right,pT_bin_left,histos[j][i],data2,max_eta);

					}

					if(parton == 'p'){
						if(id1 == -1){
							bin_filler(pT_gamma1,pT_gamma2,eta1,eta2,pT_bin_right,pT_bin_left,histos[j][i],data1,max_eta);
						}
						if(id2 == -1){
							bin_filler(pT_gamma1,pT_gamma2,eta1,eta2,pT_bin_right,pT_bin_left,histos[j][i],data2,max_eta);

						}
					}
					if(parton == 'a'){
						if(id1 == -2){
							bin_filler(pT_gamma1,pT_gamma2,eta1,eta2,pT_bin_right,pT_bin_left,histos[j][i],data1,max_eta);
						}
						if(id2 == -2){
							bin_filler(pT_gamma1,pT_gamma2,eta1,eta2,pT_bin_right,pT_bin_left,histos[j][i],data2,max_eta);
						}
					}
					if(parton == 'g'){
						if(id1 == 21){
							bin_filler(pT_gamma1,pT_gamma2,eta1,eta2,pT_bin_right,pT_bin_left,histos[j][i],data1,max_eta);
						}
						if(id2 == 21){
							bin_filler(pT_gamma1,pT_gamma2,eta1,eta2,pT_bin_right,pT_bin_left,histos[j][i],data2,max_eta);
						}			
				
					}

				}
			}
		}

		void heavy_bin_filler(float pT_bin_left,float pT_bin_right,float pT,TH1F* histo,float data){
			if(pT < pT_bin_right and pT > pT_bin_left){
				histo->Fill(data);
			}
		}


		void heavy_filler(int pdg1,int pdg2,float x1,float x2,float pT){
	
			for(int i = 0; i<max_pT_bins-1; i++){

				for(int j = 0; j < partons.length();j++){
					float pT_bin_left = pT_bins[i];
					float pT_bin_right = pT_bins[i+1];
					char parton = partons[j];




					if(parton == 'u'){
						if(pdg1 == 2){
							heavy_bin_filler(pT_bin_left,pT_bin_right,pT,histos[i][j],x1);
						}
						if(pdg2 == 2){
							heavy_bin_filler(pT_bin_left,pT_bin_right,pT,histos[i][j],x2);
						}
					}

					if(parton == 'd'){
						if(pdg1 == 1){
							heavy_bin_filler(pT_bin_left,pT_bin_right,pT,histos[i][j],x1);
						}
						if(pdg2 == 1){
							heavy_bin_filler(pT_bin_left,pT_bin_right,pT,histos[i][j],x2);
						}
					}

					if(parton == 'g'){
						if(pdg1 == 21){
							heavy_bin_filler(pT_bin_left,pT_bin_right,pT,histos[i][j],x1);
						}
						if(pdg2 == 21){
							heavy_bin_filler(pT_bin_left,pT_bin_right,pT,histos[i][j],x2);
						}
					}

					if(parton == 'p'){
						if(pdg1 == -1){
							heavy_bin_filler(pT_bin_left,pT_bin_right,pT,histos[i][j],x1);
						}
						if(pdg2 == -1){
							heavy_bin_filler(pT_bin_left,pT_bin_right,pT,histos[i][j],x2);
						}
					}
					if(parton == 'a'){
						if(pdg1 == -2){
							heavy_bin_filler(pT_bin_left,pT_bin_right,pT,histos[i][j],x1);
						}
						if(pdg2 == -2){
							heavy_bin_filler(pT_bin_left,pT_bin_right,pT,histos[i][j],x2);
						}
					}

					if(parton == 't'){
						heavy_bin_filler(pT_bin_left,pT_bin_right,pT,histos[i][j],x1);
						heavy_bin_filler(pT_bin_left,pT_bin_right,pT,histos[i][j],x2);
					}


				}
			}


		}

		void display(int split_pT_bins,float_t x_min,string normalization,string name) {
			TCanvas* pT_bin_canvas = new TCanvas("pT_bin_canvas","pT_bin_canvas");
			//pT_bin_canvas->SetLogx();
			//pT_bin_canvas->SetLogy();
			
			if(both) {
				pT_bin_canvas->Divide(2,1);

				pT_bin_canvas->cd(1);
			}

			if(split_pT_bins) {
				pT_bin_canvas->Divide(2,int((max_pT_bins -1)/ 2));
	
			}

			//auto legend = new TLegend(0.1,0.7,0.48,0.9);
			auto legend = new TLegend();
			for(int i = 0; i< max_pT_bins - 1; i++){
				int joint_integral = 0;
				for(int j = 0; j < partons.length(); j++){				
					histos[i][j]->SetLineColor(j+2);

					//for separate normalization, we normalize each histogram by its own integral
					//for joint, we sum the integrals of each partonic distribution and divide
					Double_t factor = 1;
					if(normalization == "separate"){
						histos[i][j]->Scale(factor / histos[i][j]->Integral());
					}else if(normalization == "joint"){
						if(partons[j] != 't'){
							joint_integral += histos[i][j]->Integral();
						}
					}
					if(split_pT_bins){
						pT_bin_canvas->cd(i+1);
						pT_bin_canvas->cd(i+1)->SetLogx();
						//pT_bin_canvas->cd(i+1)->SetLogy();
					}

					histos[i][j]->GetXaxis()->SetTitle("x");
					histos[i][j]->Draw("SAME HIST E1");
					histos[i][j]->GetYaxis()->SetRangeUser(0,0.03);
				}
				if(normalization == "joint"){
					Double_t factor = 1;
					for(int j = 0; j < partons.length(); j++){
						histos[i][j]->Scale(factor / joint_integral);
					}

				}
				
			}

			//legend handling
			for(int i = 0; i < partons.length(); i++){
				char legend_buf[100];
				pT_bin_canvas->cd(1);
				char parton = partons[i];
				if(parton == 't'){
					strcpy(legend_buf,(char*)"Total Momentum Fraction");
				}
				if(parton == 'u'){
					strcpy(legend_buf,(char*)"up Momentum Fraction");

				}if(parton == 'd'){
					strcpy(legend_buf,(char*)"down Momentum Fraction");

				}if(parton == 'a'){
					strcpy(legend_buf,(char*)"antiup Momentum Fraction");
				}if(parton == 'p'){
					strcpy(legend_buf,(char*)"antidown Momentum Fraction");
				}if(parton == 'g'){
					strcpy(legend_buf,(char*)"g Momentum Fraction");
				}
				legend->AddEntry(histos[0][i], legend_buf,"f");
				legend->Draw("SAME");
			}

			char name_buffer[100];
			strcpy(name_buffer,(char*)("plots/"));
			strcat(name_buffer,(char*)(name.c_str()));
			strcat(name_buffer,(char*)".pdf");

			pT_bin_canvas->SaveAs(name_buffer);
		
		}

};	



