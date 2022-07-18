/*
Recursive tree search for final state photons

*/

void has_daughter_gamma(TClonesArray* particles,TParticle* part,std::vector<float> &gammas, std::vector<float> &eta_gamma, float max_eta) {
	int daughterid1, daughterid2;
	daughterid1 = 0;
	daughterid2 = 0;
	if(part->GetDaughter(1) <= 1000 and part->GetDaughter(1) >= 0){
		TParticle* daughter1 = (TParticle*) particles->At(part->GetDaughter(1));
		daughterid1 = daughter1->GetPdgCode();
	}if(part->GetDaughter(2) <= 1000 and part->GetDaughter(2) >= 0){
		TParticle* daughter2 = (TParticle*) particles->At(part->GetDaughter(2));
		daughterid2 = daughter2->GetPdgCode();
	}
	//if the photon has no daughter photons, is marked as final state by pythia, and meets pseudorapidity cuts, add it to the vector
	if(daughterid1 != 22 and daughterid2 != 22 and part->Eta() < max_eta and part->Eta() > -max_eta and (part->GetStatusCode() > 0)){
		gammas.push_back(part->Pt());
		eta_gamma.push_back(part->Eta());
	}
	//if the photon has daughter photons, try again until there are no daughter photons
	if(daughterid1 == 22){
		has_daughter_gamma(particles,(TParticle*) particles->At(part->GetDaughter(1)),gammas, eta_gamma,max_eta);
	}if(daughterid2 == 22){
		has_daughter_gamma(particles,(TParticle*) particles->At(part->GetDaughter(2)),gammas,eta_gamma,max_eta);
	}

}
