#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <cstdlib>
#include <iomanip>
#include "random.h"
#include "system.h"

using namespace std;

void System::initialize(){

  int p1, p2; // Read from ../INPUT/Primes a pair of numbers to be used to initialize the RNG
  ifstream Primes("../INPUT/Primes");
  Primes >> p1 >> p2 ;
  Primes.close();
  int seed[4]; // Read the seed of the RNG
  ifstream Seed("../INPUT/seed.in");
  Seed >> seed[0] >> seed[1] >> seed[2] >> seed[3];
  _rnd.SetRandom(seed,p1,p2);

  ifstream input("../INPUT/input.dat"); 
  string property;
  while ( !input.eof() ){
    input >> property;
    if( property == "MU" ){
      input >> _mu;
    } else if( property == "SIGMA" ){
      input >> _sigma;
    } else if( property == "TEMP" ){
      input >> _beta;
      _beta = 1.0/_beta;
    } else if( property == "DELTA" ){
      input >> _delta;
    }else if( property == "NSTEPS" ){
      input >> _nsteps;
    }else if( property == "NBLOCKS" ){
      input >> _nblocks;
    }else if( property == "INITIAL_POINT" ){
      input >> _initial_pt;
    }else if( property == "RANGE_HISTO" ){
      input >> _range_histo;
    }else if( property == "HISTO_BINS" ){
      input >> _histo_bins;
    }
  }
  input.close();
  _naccepted = 0;
  ofstream coutp("../OUTPUT/Hvalues.dat");
  coutp << "#     BLOCK:  ACTUAL_H:  H_AVE:   H_AVE:ERROR:" << endl;
  coutp.close();
  ofstream couta("../OUTPUT/acceptance.dat"); // Set the heading line in file ../OUTPUT/acceptance.dat
  couta << "#   N_BLOCK:  ACCEPTANCE:" << endl;
  couta.close();
  ofstream couth("../OUTPUT/histo.dat"); // Set the heading line in file ../OUTPUT/histo.dat
  couth << "#   N_BLOCK:  ACCEPTANCE:" << endl;
  couth.close();

  for (int i=0; i<_histo_bins; i++) _histo.push_back(0);
  return;
}


double System::psi_mod(double x) {
        double psi = exp(-pow(x - _mu, 2) / (2 * _sigma * _sigma)) + exp(-pow(x + _mu, 2) / (2 * _sigma * _sigma));
        return pow(psi, 2);
    }

double System::propose(double x_old){
        double x = _rnd.Rannyu(-1 * _delta, _delta);
        return x_old + x;
    }

double System::V(double x) {
        return pow(x, 4) - 2.5 * pow(x, 2);
    }

double System::get_energy(double x){
        double E = 0.5/(pow(_sigma,2)) - 0.5/(pow(_sigma, 4))*(x*x + _mu*_mu) + x*_mu/(pow(_sigma, 4))*tanh(x*_mu/(_sigma*_sigma)); 
        return E + V(x);
}

double System::boltzmann(double x){
        double E = get_energy(x);
        return exp(-1*_beta*E);
}

double System::metro(double x){
        double new_x = propose(x);
        double rait = psi_mod(new_x)/psi_mod(x);
        double test = _rnd.Rannyu();
        if(rait>1) {
          _naccepted++;
          return new_x;
        }
        else if(test < rait){
          _naccepted++;
          return new_x;
        } 
        else return x;
}

void System::measure_H(double x){
        _nattempts++;
        _block_av += get_energy(x);
        if(abs(x)<_range_histo/2) _histo[(floor(x/(_range_histo/_histo_bins))) + _histo_bins/2] += 1;
}

void System :: averages(int blk){

  double average, sum_average, sum_ave2;
  vector<double> _average_histo(_histo_bins), _global_av_histo(_histo_bins), _global_av2_histo(_histo_bins);
  ofstream coutf;

  _average     = _block_av / double(_nsteps);
  _global_av  += _average;
  _global_av2 += _average * _average; 

  for(int j=0; j<_histo_bins; j++) {
    _average_histo[j] = _histo[j];
    _global_av_histo[j] +=  _average_histo[j];
    _global_av2_histo[j] +=  _average_histo[j]*_average_histo[j];
    _histo[j]=0;
  }

  coutf.open("../OUTPUT/Hvalues.dat",ios::app);
  average= _average;
  sum_average = _global_av;
  sum_ave2 = _global_av2;
  coutf  << setw(12) << blk 
  << setw(12) << average
  << setw(12) << sum_average/double(blk)
  << setw(12) << error(sum_average, sum_ave2, blk) << endl;
  coutf.close();
  _block_av = 0;

  double fraction;
  coutf.open("../OUTPUT/acceptance.dat",ios::app);
  if(_nattempts > 0) fraction = double(_naccepted)/double(_nattempts);
  else fraction = 0.0; 
  coutf << setw(12) << blk << setw(12) << fraction << endl;
  coutf.close();

  if(blk == _nblocks){
    coutf.open("../OUTPUT/histo.dat",ios::app);
    for(int k=0; k<_histo_bins; k++){
      sum_average = _global_av_histo[k]/(abs(2*_sigma*sqrt(2*M_PI)));
      sum_ave2 = _global_av2_histo[k]/pow(abs(2*_sigma*sqrt(2*M_PI)),2);
      coutf  << setw(12) << k - _histo_bins / 2
      << setw(12) << sum_average/double(blk)
      << setw(12) << error(sum_average, sum_ave2, blk) << endl;
    }
    coutf.close();
  }
  return;
}

double System :: error(double acc, double acc2, int blk){
  if(blk <= 1) return 0.0;
  else return sqrt( fabs(acc2/double(blk) - pow( acc/double(blk) ,2) )/double(blk) );
}

int System::get_nsteps(){
  return _nsteps;
}

int System::get_nblk(){
  return _nblocks;
}

double System::get_initial_pt(){
  return _initial_pt;
}