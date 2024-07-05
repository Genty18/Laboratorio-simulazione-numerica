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
double get_random_axis(int low, int high, Random &rnd){
    double x = rnd.Rannyu();
    return std::floor(low+(high - low)*x);
}
int get_random_sign(Random &rnd){
    double sign = std::floor(1+2*rnd.Rannyu());
    if(sign == 1) return 1;
    else return -1;
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


    int M=10000, N=100, lenght_rw=100, low=1, high=4;
    double L=M/N, x=0, y=0, z=0, a=1, random=0;

    ofstream outfile("dataDISC.dat");

    std::vector<double> r_i(lenght_rw);
    std::vector<double> r_ave_i(lenght_rw);
    std::vector<double> r_ave2_i(lenght_rw);
    std::vector<double> sum_prog(lenght_rw);
    std::vector<double> su2_prog(lenght_rw);
    std::vector<double> avg_prog(lenght_rw);
    std::vector<double> avg2_prog(lenght_rw);
    std::vector<double> err(lenght_rw);

    for (int i = 0; i < N; ++i) {
        // Loop all'interno di ogni blocco
        for (int j = 0; j < L; j++) {
            x=0;
            y=0;
            z=0;
            for(int k=0; k<lenght_rw; k++){
                a=abs(a);
                a *= get_random_sign(rnd);
                random = get_random_axis(low, high, rnd);
                if(random==1) x+=a;
                if(random==2) y+=a;
                else z+=a;
                r_i[k] += x*x + y*y + z*z;
            }
        }

        for(int k=0; k<lenght_rw; k++){
            r_ave_i[k] = sqrt(r_i[k]/L);
            r_ave2_i[k] = pow(r_ave_i[k], 2);
            r_i[k] = 0;
            sum_prog[k] += r_ave_i[k];
            su2_prog[k] += r_ave2_i[k];
            avg_prog[k] = sum_prog[k]/(i+1);
            avg2_prog[k] = su2_prog[k]/(i+1);
            err[k] = error(avg_prog[k], avg2_prog[k], i);
        } 
    }

    for(int i=0; i <lenght_rw; i++) outfile <<i+1 <<" " <<avg_prog[i] <<" " <<err[i] <<endl; 

// Salvataggio del seme del generatore di numeri casuali
    rnd.SaveSeed();
    outfile.close();
    r_i.clear();
    r_ave_i.clear();
    r_ave2_i.clear();
    sum_prog.clear();
    su2_prog.clear();
    avg_prog.clear();
    avg2_prog.clear();
    err.clear();
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
