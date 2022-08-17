

//recursive tree search for mothers of muons. Stops as soon as one is found and returns true

bool has_ohf_mother(TParticle* particle,TClonesArray* particles, Int_t &id, int &iterations, int &second_iterations){
    int mother1_idx = particle->GetMother(1);
    int mother2_idx = particle->GetMother(2);
    int max_depth = 30000;
    iterations++;

    if(iterations >= max_depth){
        cout << "Too much! I can't take it! \n";
        return false;
    }

    if(mother1_idx >= 0){
        TParticle* mother1 = (TParticle*) particles->At(mother1_idx);

        int mother1_id = mother1->GetPdgCode();
        if(mother1_id == 13 || mother1_id == -13){
            if(iterations == 1){
                cout << "recursion! " << iterations << '\n';
            }
            cout << "Mother id (1)= " << mother1_id << '\n';
            has_ohf_mother(mother1,particles,id,iterations,second_iterations);
        }
        for(int i = 0; i < light_d_mesons_length; i++){
            if(mother1_id == light_d_mesons[i]){
                id = mother1_idx;
                cout << "OHF mother found! " << mother1_id << '\n';
                return true;
            }
        }
    }

    if(mother2_idx >= 0){
        TParticle* mother2 = (TParticle*) particles->At(mother2_idx);

        int mother2_id = mother2->GetPdgCode();
        if(mother2_id == 13 || mother2_id == -13){
            if(second_iterations == 1){
                cout << "second recursion! " << second_iterations << '\n';
            }
            cout << "Mother id (2)= " << mother2_id << '\n';
            has_ohf_mother(mother2,particles,id,iterations,second_iterations);
        }

        for(int i = 0; i < light_d_mesons_length; i++){
            if(mother2_id == light_d_mesons[i]){
                id = mother2_idx;
                cout << "OHF mother found! " << mother2_id << '\n';
                return true;
            }
        }
    }

    return false;

    
}