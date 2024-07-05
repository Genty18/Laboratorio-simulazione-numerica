#include "population.h"


using namespace std;


struct CompareIndividuals {
    bool operator()(Individual ind1, Individual ind2) const {
        return ind1.get_length() < ind2.get_length();
    }
};

void completePath(std::vector<int>& child, const std::vector<int>& parent, int cutPoint) {
    int n = parent.size();
    int idx = cutPoint;
    for (int i = 0; i < n; ++i) {
        // Skip elements that are already in the first part of the child
        if (std::find(child.begin(), child.begin() + cutPoint, parent[i]) == child.begin() + cutPoint) {
            child[idx++] = parent[i];
        }
    }
}



void Population::initialize(int continent){

  int seed[4];
    int p1, p2;
    ifstream Primes("../INPUT/Primes");
    string line;
    //skip lines for different initial conditions in different continent
    for (int i = 0; i < continent; ++i) getline(Primes, line); 
    if (Primes.is_open()){
      Primes >> p1 >> p2;
    }else cerr << "PROBLEM: Unable to open Primes" << endl;
    Primes.close();
    ifstream input_rnd("../INPUT/seed.in");
    string property_rnd;
    if (input_rnd.is_open()){
        while ( !input_rnd.eof() ){
            input_rnd >> property_rnd;
            if( property_rnd == "RANDOMSEED" ){
                input_rnd >> seed[0] >> seed[1] >> seed[2] >> seed[3];
                _rnd.SetRandom(seed,p1,p2);
            }
        }
        input_rnd.close();
    } else cerr << "PROBLEM: Unable to open seed.in" << endl;



  ifstream input("../INPUT/input.dat"); 
  string property;
  while ( !input.eof() ){
    input >> property;
    if( property == "POPULATION_DIM" ){
      input >> _population_dim;
    }else if( property == "PAIR_PERMUTATION_PROB" ){
      input >> _PPP;
    }else if( property == "SHIFT_M_CONTIGUOUS_PROB" ){
      input >> _PSMC;
    }else if( property == "PERMUTATION_M_CONTIGUOUS_PROB" ){
      input >> _PPMC;
    }else if( property == "INVERSION_ORDER_PROB" ){
      input >> _PIO;
    }else if( property == "CROSSOVER_PROB" ){
      input >> _PCROSS;
    }else if( property == "SIM_TYPE" ){
      input >> _sim_type;
    }else if( property == "N_CITY" ){
      input >> _ind_size;
    }else if( property == "SEL_POWER" ){
      input >> _p;
    }else if( property == "GENERATIONS" ){
      input >> _generations;
    }
  }

  //Generating initial random configuration
  Individual ind(_ind_size);
  for(int i=0; i<_population_dim; i++){
    _individuals.push_back(ind);
    for(int j = 0; j<3; j++){
      int p1 = floor(_rnd.Rannyu(1, _ind_size));
      int p2 = floor(_rnd.Rannyu(1, _ind_size));
      _individuals[i].pair_perm(p1, p2);
    }
  }

  //CITY ON SQUARE
  if(_sim_type==0){
    for(int i=0; i<_ind_size; i++) {
      _x_city.push_back(_rnd.Rannyu());
      _y_city.push_back(_rnd.Rannyu());
    }
  }

  //CITY ON CIRCUMFERENCE 
  if(_sim_type==1){
    for(int i=0; i<_ind_size; i++) {
      double theta = _rnd.Rannyu(0, 2*M_PI);
      _x_city.push_back(cos(theta));
      _y_city.push_back(sin(theta));
    }
  }

  //PLOTTING THE CITY
  ofstream coutc("../OUTPUT/Cities.dat");
  coutc << "#     x:  y: " << endl;
  for(int i=0; i<_ind_size; i++) coutc <<_x_city[i] <<setw(12) <<_y_city[i] << endl;
  coutc.close();
  _actual_gen=0;
  return;
}


void Population::check_function(){
  for(int k=0; k<_population_dim; k++){
    if(_individuals[k].get_element(0)!=1) cout <<endl <<"ERROR : THE FIRST CITY DOES NOT APPEAR AT POSITION ONE";
    for (int i=1; i<_ind_size; i++){
      for (int j=1; j<_ind_size; j++){
        if(i!=j && _individuals[k].get_element(i)==_individuals[k].get_element(j)) cout <<endl <<"ERROR : CITY AT POSITION "<<i <<" APPEAR AGAIN AT POSITION " <<j <<" in the " <<k<< " individual";
      }
    }
  } 
}

void Population::love(int ind1, int ind2){
    std::vector<int> mother;
    std::vector<int> father;
    
    // Define cut point
    int cutPoint = floor(_rnd.Rannyu(1, _ind_size));

    for (int i = 0; i < _ind_size; i++){
      mother.push_back(_individuals[ind1].get_element(i));
      father.push_back(_individuals[ind2].get_element(i));
    }
    
    // Create children and copy the first part
    std::vector<int> child1(mother.begin(), mother.begin() + cutPoint);
    std::vector<int> child2(father.begin(), father.begin() + cutPoint);

    // Resize children to the full length
    child1.resize(mother.size());
    child2.resize(father.size());

    // Complete the paths
    completePath(child1, father, cutPoint);
    completePath(child2, mother, cutPoint);

    _individuals[ind1].modify(child1);
    _individuals[ind2].modify(child2);
}


void Population::sorting(){
  sort(_individuals.begin(), _individuals.end(), CompareIndividuals());  
}

int Population::get_dim(){
  return _population_dim;
}

int Population::get_ind_dim(){
  return _ind_size;
}

int Population::get_generations(){
  return _generations;
}

Individual Population::get_individual(int pos){
  return _individuals[pos];
}

void Population::modify_ind(std::vector<int> mod, int pos){
  _individuals[pos].modify(mod);
}

void Population::compute_all_length(){
  for(int i=0; i<_population_dim; i++) _individuals[i].compute_length(_x_city,_y_city);
}

void Population::mutate(){
  for (int i=0; i<_population_dim; i++){
   if(_rnd.Rannyu()<_PPP) {
      int p1 = floor(_rnd.Rannyu(1, _ind_size));
      int p2 = floor(_rnd.Rannyu(1, _ind_size));
      _individuals[i].pair_perm(p1,p2);
   }
   if(_rnd.Rannyu()<_PIO) _individuals[i].inv_order();
   if (_rnd.Rannyu()<_PPMC){
      int m = floor(_rnd.Rannyu(1,_ind_size/2));
      int initial = floor(_rnd.Rannyu(1,_ind_size-m));
      int n = floor(_rnd.Rannyu(1,_ind_size/2));
      _individuals[i].perm_Mcontiguous(m,n,initial);
   }
  // //  //DA FIXARE 
  //  if (_rnd.Rannyu()<_PSMC){
  //     int initial = floor(_rnd.Rannyu(1,_ind_size));
  //     int m = floor(_rnd.Rannyu(1,_ind_size));
  //     int n = floor(_rnd.Rannyu(1,_ind_size));
  //     _individuals[i].shift_Mcontiguous(m,n,initial);
  //  }
  }

}

void Population::crossover(){
  for (int i=0; i<_population_dim-1; i++){
    if(_rnd.Rannyu()<_PCROSS) love(i, i+1);
  }
}

void Population::selection(){
  vector<Individual> copy;
  vector<int> copy_sequence(_ind_size);
  for(int i=0; i<_population_dim; i++) copy.push_back(_individuals[i]);
  double r=0;
  for(int i=0; i<_population_dim; i++){
    r = _rnd.Rannyu();
    int selected = int(_population_dim*pow(r, _p));
    for (int j=0; j<_ind_size; j++) copy_sequence[j] = copy[selected].get_element(j); 
    _individuals[i].modify(copy_sequence);
  }
}

void Population::print(int continent){
  ofstream coutf;
  string filePath;

  if(_sim_type == 0) filePath = "../OUTPUT/CONTINENT" + std::to_string(continent) + "/SQUARE/Generations.dat";
  else filePath = "../OUTPUT/CONTINENT" + std::to_string(continent) + "/CIRCUMFERENCE/Generations.dat";
  coutf.open(filePath,ios::app);
  coutf  <<setw(12) << _actual_gen << setw(12) <<_individuals[0].get_length() <<endl; //taking just the shortest distance
  coutf.close();

  _length_ave=0;
  if(_sim_type == 0) filePath = "../OUTPUT/CONTINENT" + std::to_string(continent) + "/SQUARE/Generations_ave.dat";
  else filePath = "../OUTPUT/CONTINENT" + std::to_string(continent) + "/CIRCUMFERENCE/Generations_ave.dat";
  coutf.open(filePath,ios::app);
  for(int i=0; i<int(_population_dim/2); i++) _length_ave += _individuals[i].get_length();
  coutf  <<setw(12) << _actual_gen << setw(12) <<_length_ave/double(_population_dim/2) <<endl;
  coutf.close();
  _actual_gen++;

  //printing the best path (only if that is the last generatoin)
  if(_actual_gen == _generations){
    cout <<endl <<"entrato";
    if(_sim_type == 0) filePath = "../OUTPUT/CONTINENT" + std::to_string(continent) + "/SQUARE/Sequences.dat";
    else filePath = "../OUTPUT/CONTINENT" + std::to_string(continent) + "/CIRCUMFERENCE/Sequences.dat";
    coutf.open(filePath,ios::app);
    for (int i=0; i<_ind_size; i++){
      coutf  <<_individuals[0].get_element(i) <<endl; 
    }
    coutf.close();
  }
}




