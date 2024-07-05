#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <cstdlib>
#include <iomanip>
#include "random.h"
#include "system.h"

using namespace std;

void System::initialize(){

  int seed[4];
    int p1, p2;
    ifstream Primes("../INPUT/Primes");
    if (Primes.is_open()){
        Primes >> p1 >> p2 ;
    } else cerr << "PROBLEM: Unable to open Primes" << endl;
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
    if( property == "MU" ){
      input >> _mu;
    } else if( property == "SIGMA" ){
      input >> _sigma;
    } else if( property == "DELTA" ){
      input >> _delta;
    }else if( property == "NSTEPS" ){
      input >> _metro_steps;
    }
    else if( property == "NBLOCKS" ){
      input >> _nblocks;
    }
    else if( property == "BETA_STEPS" ){
      input >> _beta_steps;
    }
    else if( property == "INITIAL_POINT" ){
      input >> _initial_pt;
    }
    else if( property == "DELTA_MU" ){
      input >> _delta_mu;
    }
    else if( property == "DELTA_SIGMA" ){
      input >> _delta_sigma;
    }
    else if( property == "SA_STEPS" ){
      input >> _SA_steps;
    }
    else if( property == "X_START_BETA" ){
      input >> _x_start;
    }
    else if( property == "D_X_BETA" ){
      input >> _d_x;
    }
  }
   input.close();
  // _naccepted = 0;
  // ofstream coutp("../OUTPUT/Hvalues.dat");
  // coutp << "#     STEP:  TEMP:  H:   H:ERROR:" << endl;
  // coutp.close();

  // ofstream coutm("../OUTPUT/mu.dat");
  // coutm << "#   STEP:  TEMP:  MU:" << endl;
  // coutm.close();

  // ofstream couts("../OUTPUT/sigma.dat");
  // couts << "#   STEP:  TEMP:  SIGMA:" << endl;
  // couts.close();

  //LOOP PER DARE UN VALORE INIZIALE AD <H> 
  double x = _initial_pt;
  for(int i=0; i < _nblocks; i++){ //loop over blocks
        for(int j=0; j < _metro_steps; j++){ //loop over steps in a block
          x = metro(x, _mu, _sigma);
          _block_av += get_energy(x, _mu, _sigma) + V(x);
        }
        averages(i+1);
      }
  reset_averages();
  _H = _H_TRY;
  _err_H = _err_H_TRY;
  modify_beta();
  return;
}

double System::psi_mod(double x, double mu, double sigma) {
        double psi = exp(-pow(x - mu, 2) / (2 * sigma * sigma)) + exp(-pow(x + mu, 2) / (2 * sigma * sigma));
        return pow(psi, 2);
    }

double System::propose(double x_old){
        double x = _rnd.Rannyu(-1 * _delta, _delta);
        return x_old + x;
    }

void System::propose_mu(){
        double x = _rnd.Rannyu(-1 * _delta_mu, _delta_mu);
        _proposed_mu =  _mu + x;
    }

void System::propose_sigma(){
        double x = _rnd.Rannyu(-1 * _delta_sigma, _delta_sigma);
        _proposed_sigma =  _sigma + x;
    }

double System::V(double x) {
        return pow(x, 4) - 2.5 * pow(x, 2);
    }

double System::get_energy(double x, double mu, double sigma){
        double E = 0.5/(pow(sigma,2)) - 0.5/(pow(sigma, 4))*(x*x + mu*mu) + x*mu/(pow(sigma, 4))*tanh(x*mu/(sigma*sigma)); 
        return E + V(x);
}

double System::boltzmann(double x){
        return exp(-1*_beta*x);
}

double System::metro(double x, double mu, double sigma){
        double new_x = propose(x);
        double rait = psi_mod(new_x, mu, sigma)/psi_mod(x, mu, sigma);
        double test = _rnd.Rannyu();
        if(rait>1) return new_x;
        else if(test < rait) return new_x; 
        else return x;
}

void System::metro_SA(){
  if(_H_TRY < _H) {
    _H = _H_TRY;
    _err_H = _err_H_TRY;
    _mu = _proposed_mu;
    _sigma = _proposed_sigma;
  }
  else if(_rnd.Rannyu() < boltzmann(_H_TRY)/boltzmann(_H)){
    _H = _H_TRY;
    _err_H = _err_H_TRY;
    _mu = _proposed_mu;
    _sigma = _proposed_sigma;
  } 
}

void System::measure_H(double x){
      propose_mu();
      propose_sigma();
      for(int i=0; i < _nblocks; i++){ //loop over blocks
        for(int j=0; j < _metro_steps; j++){ //loop over steps in a block
          x = metro(x, _proposed_mu, _proposed_sigma);
          _block_av += get_energy(x, _proposed_mu, _proposed_sigma);
        }
        averages(i+1);
      }
    reset_averages();
}

void System :: plot(int step){

  ofstream coutf;
  coutf.open("../OUTPUT/Hvalues.dat",ios::app);
  coutf  << setw(12) << step 
  << setw(12) << 1/_beta
  << setw(12) << _H
  << setw(12) << _err_H << endl;
  coutf.close();

  coutf.open("../OUTPUT/mu.dat",ios::app);
  coutf << setw(12) << step << setw(12) << 1/_beta << setw(12) <<_mu << endl;
  coutf.close();

  coutf.open("../OUTPUT/sigma.dat",ios::app);
  coutf << setw(12) << step << setw(12) << 1/_beta << setw(12) <<_sigma << endl;
  coutf.close();
  
  return;
}

void System :: averages(int blk){
  _average     = _block_av / double(_metro_steps);
  _global_av  += _average;
  _global_av2 += _average*_average; 

  if(blk == _nblocks){
    _H_TRY = _global_av/double(blk);
    _err_H_TRY = error(_global_av, _global_av2, blk);
  }
  _block_av = 0;  
  return;
}

void System :: reset_averages(){
  _average    = 0;
  _global_av  = 0;
  _global_av2 = 0; 
}


double System :: error(double acc, double acc2, int blk){
  if(blk <= 1) return 0.0;
  else return sqrt( fabs(acc2/double(blk) - pow( acc/double(blk) ,2) )/double(blk) );
}

int System::get_metro_steps(){
  return _metro_steps;
}

int System::get_beta_step(){
  return _beta_steps;
}

int System::get_SA_steps(){
  return _SA_steps;
}

double System::get_initial_pt(){
  return _initial_pt;
}

void System::modify_beta(){
  _x_start += _d_x;
  _beta = sqrt(_x_start + 3.7)*exp(-0.2*_x_start);
  _beta = 1/_beta;
}