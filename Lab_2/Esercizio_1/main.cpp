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
double f(double x) {
    return M_PI/2. * cos(M_PI*x/2.);
}
double g(double x) {
    return M_PI/2.*cos(M_PI*x/2.)/(2.*(1-x));
}

double get_random_rho(Random &rnd){
    double y = rnd.Rannyu();
    return 1-sqrt(1-y);
}

double error(double AV, double AV2, int n) {
    if (n == 0)
        return 0;
    else
        return sqrt((AV2 - pow(AV, 2)) / n);
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


    int M=100000, N=100;
    double L=M/N;
    std::vector<ofstream> outfile(2);
    outfile[0].open("Uniform_sampling.dat");
    outfile[1].open("Importance_sampling.dat");
    std::vector<double> sum(2);
    std::vector<double> ave(2);
    std::vector<double> ave2(2);
    std::vector<double> sum_prog(2);
    std::vector<double> su2_prog(2);
    std::vector<double> avg_prog(2);
    std::vector<double> avg2_prog(2);

    for (int i = 0; i < N; ++i) {
            sum[0]=0;
            sum[1]=0;
            // Loop all'interno di ogni blocco
            for (int j = 0; j < L; ++j) {
                double x = rnd.Rannyu();
                double x2 = get_random_rho(rnd);
                sum[0] += f(x);
                sum[1] += g(x2);
            }
            // Calcolo delle medie per il blocco corrente
            for(int k=0; k<2; k++){
                ave[k] = sum[k]/L;
                ave2[k] = pow(ave[k], 2);
                sum_prog[k] += ave[k];
                su2_prog[k] += ave2[k];
                avg_prog[k] = sum_prog[k] / (i+1);
                avg2_prog[k] = su2_prog[k] / (i+1);
                outfile[k] <<i*L <<" " <<avg_prog[k] <<" " <<error(avg_prog[k], avg2_prog[k], i) <<endl;
            }
    }

// Salvataggio del seme del generatore di numeri casuali
    rnd.SaveSeed();
    outfile[0].close();
    outfile[1].close();
    sum.clear();
    ave.clear();
    ave2.clear();
    sum_prog.clear();
    su2_prog.clear();
    avg2_prog.clear();
    avg2_prog.clear();
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
