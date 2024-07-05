/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <string>
#include "random.h"
#include <random>

using namespace std;


//funzioni per ottenere numeri random con distribuzione
double Get_random_exp(double lambda, Random &rnd) {
    double y = rnd.Rannyu();
    return -1/lambda*(log(1-y));
}
double Get_random_CL(double gamma, double mu,  Random &rnd) {
    double y = rnd.Rannyu();
    return gamma*tan(M_PI*(y-0.5)) + mu;
}



int main() {
    // Generatore di numeri casuali
   Random rnd;
   int seed[4];
   int p1, p2;
   ifstream Primes("Primes");
   if (Primes.is_open()){
      Primes >> p1 >> p2 ;
   } else cerr << "PROBLEM: Unable to open Primes" << endl;
   Primes.close();

   ifstream input("seed.in");
   string property;
   if (input.is_open()){
      while ( !input.eof() ){
         input >> property;
         if( property == "RANDOMSEED" ){
            input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
            rnd.SetRandom(seed,p1,p2);
         }
      }
      input.close();
   } else cerr << "PROBLEM: Unable to open seed.in" << endl;



// Definizioni
int M = 10000;
vector<int> N = {1, 2, 10, 100};
double Sn_u = 0, Sn_exp = 0, Sn_cl = 0;
std::vector<std::ofstream> outfile(N.size());

// Ciclo sui diversi valori di N
for(int k=0; k<int(N.size()); k++){
   // Apertura del file di output per ogni N
   outfile[k].open("DatiN" + std::to_string(N[k]) + ".dat");

   // Ciclo su M esperimenti
   for(int i=0; i<M; i++){
      // Reset delle somme
      Sn_u = 0;
      Sn_cl = 0;
      Sn_exp = 0;

      // Calcolo delle somme per ogni esperimento
      for(int j=0; j<N[k]; j++){
         Sn_u += rnd.Rannyu()*5 + 1; // Somma di numeri uniformemente distribuiti
         Sn_exp += Get_random_exp(1, rnd); // Somma di numeri distribuiti esponenzialmente
         Sn_cl += Get_random_CL(1, 0, rnd); // Somma di numeri distribuiti secondo Cauchy-Lorentz
      }

      // Scrittura dei risultati nel file di output
      outfile[k] << Sn_u/N[k] <<" " << Sn_exp/N[k] <<" " << Sn_cl/N[k] << endl;
   }

   // Chiusura del file di output
   outfile[k].close();
}

// Salvataggio del seme del generatore di numeri casuali
rnd.SaveSeed();

N.clear();
outfile.clear();

    return 0;
}



/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
