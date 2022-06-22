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
class pt_histos{
	public:
		pt_histos(int bins[]){
			frac_hists bin_hists[sizeof(bins)-1];

			for (int i = 0; i<sizeof(bins)-1; i++){
				string name = string(bins[i]) + " < pT < " + string(i+1);
				bin_hists[i] = frac_hists((char*) name);
			}
		}

		void filler(int id,float data, float pT) {
			
		}

		void show(){

		}
}; 


*/



class pt_histos{
	//std::vector<TH1F*> up_histos;
	std::vector<TH1F*> down_histos;
	std::vector<TH1F*> aup_histos;
	std::vector<TH1F*> adown_histos;
	std::vector<TH1F*> gluon_histos;
	//string partons;
	//std::vector<float> pT_bins;
	TCanvas* canvas;
	int both;
	string partons;


	const static int max_pT_bins = 6;
	const static int max_partons = 6;

	float pT_bins[max_pT_bins] = {};

	//gStyle->GetXaxis()->SetLimits(1e-3,1);



	public:
		TH1F *histos[max_pT_bins][max_partons];

		pt_histos(char* name,string partons_in, TCanvas* canvas_in,float pT_bins_in[], int show_both_hist) {
			partons = partons_in;
			//pT_bins = pT_bins_in;
			canvas = canvas_in;
			both = show_both_hist;

			//TH1F *histos[pT_bins.size()][partons.length()];

			for(int i = 0; i< max_pT_bins - 1;i++){
				pT_bins[i] = *(pT_bins_in + i);
			}

			for(int i = 0; i<max_pT_bins - 1; i++) {
				for(int j = 0; j < partons.length(); j++){
					if(partons[j] == 'u'){

						char buf[100];
						const char* left_bound = std::to_string(pT_bins[i]).c_str();
						const char* right_bound = std::to_string(pT_bins[i+1]).c_str();
                                                strcpy(buf,(char*)"q_{u} Momentum Fraction ");
						strcat(buf,name);
						strcat(buf,left_bound);
						strcat(buf,(char*)" - ");
						strcat(buf,right_bound);


						histos[i][j] = new TH1F(buf,buf,100,-3,0);
						BinLogX(histos[i][j]);
						

	
					}
					if(partons[j] == 'd'){
						
						char buf[100];
						const char* left_bound = std::to_string(pT_bins[i]).c_str();
						const char* right_bound = std::to_string(pT_bins[i+1]).c_str();
						
						strcpy(buf,(char*)"q_{d} Momentum Fraction ");
						strcat(buf,name);
						strcat(buf,(char*)" - ");
						strcat(buf,right_bound);

						histos[i][j] = new TH1F(buf,buf,100,-3,0);

						BinLogX(histos[i][j]);
					}
					if(partons[j] == 't'){
						char buf[100];
						const char* left_bound = std::to_string(pT_bins[i]).c_str();
						const char* right_bound = std::to_string(pT_bins[i+1]).c_str();

						strcpy(buf,(char*)"Total Momentum Fraction ");
						strcat(buf,name);
						strcat(buf,(char*)" - ");
						strcat(buf,right_bound);

						histos[i][j] = new TH1F(buf,buf,100,-3,0);

						BinLogX(histos[i][j]);

					}
				}
			}
			






		}

		void filler(int id1,int id2, float data1,float data2, std::vector<float> pT_gamma1, std::vector<float> pT_gamma2, int &counter){



			for(int i = 0; i< partons.length(); i++){
				//if there are no final state photons, don't plot anything
				if(pT_gamma1.size() == 0 and pT_gamma2.size() == 0) break;

				for(int j = 0; j < max_pT_bins - 1; j++){
					if(partons[i] == 'u' and (id1 == 2 || id2 == 2)) {


						for(int k = 0; k < pT_gamma1.size(); k++) {
							if(pT_gamma1[k] < pT_bins[j+1] and pT_gamma1[k] > pT_bins[j]) {
								histos[j][i]->Fill(data1);
								histos[j][i]->Fill(data2);
								if(data1 < 1e-3 || data2 < 1e-3){
									counter++;
								}
								break;
							}
						} for(int k = 0; k < pT_gamma2.size(); k++) {
							if(pT_gamma2[k] < pT_bins[j+1] and pT_gamma2[k] > pT_bins[j]) {
								histos[j][i]->Fill(data1);
								histos[j][i]->Fill(data2);


								if(data1 < 1e-3 || data2 < 1e-3) {

									counter++;
								}

								break;
							}
						}

					} if(partons[i] == 'd' and (id1 == 1 || id2 == 1)) {
						for(int k = 0; k < pT_gamma1.size(); k++){

							if(pT_gamma1[k] < pT_bins[j+1] and pT_gamma1[k] > pT_bins[j]) {
								histos[j][i]->Fill(data1);
								histos[j][i]->Fill(data2);
								break;
							}
						}
						for(int k = 0; k < pT_gamma2.size(); k++) {


							if(pT_gamma2[k] < pT_bins[j+1] and pT_gamma2[k] > pT_bins[j]) {
								histos[j][i]->Fill(data1);
								histos[j][i]->Fill(data2);
								break;
							}
						}

						
					}
				}
				
			}

		}

		void display(int split_pT_bins,float_t x_min) {
			TCanvas* pT_bin_canvas = new TCanvas("pT_bin_canvas","pT_bin_canvas");
			//pT_bin_canvas->SetLogx();
		//	pT_bin_canvas->SetLogy();
			
			if(both) {
				pT_bin_canvas->Divide(2,1);

				pT_bin_canvas->cd(1);
			}

			if(split_pT_bins) {
				pT_bin_canvas->Divide(2,int((max_pT_bins -1)/ 2));
	
			}

			auto legend = new TLegend(0.1,0.7,0.48,0.9);
			for(int i = 0; i< max_pT_bins - 1; i++){

				for(int j = 0; j < partons.length(); j++){				
					//TH1F* histogram;
//					histogram->GetXaxis()->SetRangeUser(x_min,1);
//					histogram->GetXaxis()->SetLimits(1e-3,1.);
	
					//BinLogX(histogram);


					

					if(partons[j] == 'u') {
						histos[i][j]->SetLineColor(j+1);
					}if(partons[j] == 'd'){
						histos[i][j]->SetLineColor(j+1);
					}
	
					//BinLogX(histogram);

					Double_t factor = 1;
					histos[i][j]->Scale(factor / histos[i][j]->Integral());
					if(split_pT_bins){
						pT_bin_canvas->cd(i+1);
						pT_bin_canvas->cd(i+1)->SetLogx();
						pT_bin_canvas->cd(i+1)->SetLogy();
					}

						histos[i][j]->GetXaxis()->SetTitle("x");
						histos[i][j]->Draw("SAME HIST E1");

/*
					char legend_buf[100];

					const char* left_bound_leg = std::to_string(pT_bins[i]).c_str();
					const char* right_bound_leg = std::to_string(pT_bins[i+1]).c_str();
		
					if(partons[j] == 'u'){
						strcpy(legend_buf,(char*)"q_{u} Momentum Fraction pT");}
					if(partons[j] == 'd'){
						strcpy(legend_buf,(char*)"q_{d} Momentum Fraction pT");
	
					}
					strcat(legend_buf,left_bound_leg);
					strcat(legend_buf, (char*)" - ");
					strcat(legend_buf,right_bound_leg);
				

					legend->AddEntry(histos[i][j], legend_buf,"f");
					legend->Draw("SAME");
*/
				}
				
			}
			for(int i = 0; i < partons.length(); i++){
				char legend_buf[100];
				pT_bin_canvas->cd(1);

				if(partons[i] == 'u'){
					strcpy(legend_buf,(char*)"q_{u} Momentum Fraction");

				}if(partons[i] == 'd'){
					strcpy(legend_buf,(char*)"q_{d} Momentum Fraction");

				}
				legend->AddEntry(histos[0][i], legend_buf,"f");
				legend->Draw("SAME");
			}


		
		}


};


