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

double Get_call_price(double k, double S, double r, double T){
    if(S-k > 0) return exp(-1*r*T)*(S-k);
    else return 0;
    }
double Get_put_price(double k, double S, double r, double T){
    if(S-k < 0) return exp(-1*r*T)*(k-S);
    else return 0;
    }
double Get_Si(double vol, double S0, double r, double Zi, double t1, double t2){
    return S0*exp((r-vol*vol/2)*(t1-t2) + Zi*vol*sqrt(t1-t2));
    }

//funzioni per ottenere numeri random con distribuzione
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

    int M=1E7, N=100; 
    double S0=100, T=1, K=100, r=0.1, vol = 0.25, L=M/N;
    vector<ofstream> outfile(4);
    outfile[0].open("DirectlyCall.dat");
    outfile[1].open("DirectlyPUT.dat");
    outfile[2].open("DiscretizedCall.dat");
    outfile[3].open("DiscretizedPUT.dat");

    vector<double> sum(4);
    vector<double> ave(4);
    vector<double> ave2(4);
    vector<double> sum_prog(4);
    vector<double> su2_prog(4);
    vector<double> avg_prog(4);
    vector<double> avg2_prog(4);
    vector<double> S_i(2);
        
    for (int i = 0; i < N; ++i) {
        sum[0]=0;
        sum[1]=0;
        sum[2]=0;
        sum[3]=0;
        // Loop all'interno di ogni blocco
        for (int j = 0; j < L; ++j) {
            double Zi = rnd.Gauss(0, 1);
            S_i[0] = Get_Si(vol, S0, r, Zi, T, 0);
            for(int k=0; k<N; k++) {
                double up = T/double(N)*k+1;
                double down = T/double(N)*k;
                S_i[1] = Get_Si(vol, S0, r, Zi, up, down);
            }
            sum[0] += Get_call_price(K, S_i[0], r, T);
            sum[1] += Get_put_price(K, S_i[0], r, T);
            sum[2] += Get_call_price(K, S_i[1], r, T);
            sum[3] += Get_put_price(K, S_i[1], r, T);
        }
        
        // Calcolo delle medie per il blocco corrente
        for(int k=0; k<4; k++){
            ave[k] = sum[k]/L;
            ave2[k] = pow(ave[k], 2);
            sum_prog[k] += ave[k];
            su2_prog[k] += ave2[k];
            avg_prog[k] = sum_prog[k] / (i+1);
            avg2_prog[k] = su2_prog[k] / (i+1);
            outfile[k] <<i <<" " <<avg_prog[k] <<" " <<error(avg_prog[k], avg2_prog[k], i) <<endl;
        }
    }
    rnd.SaveSeed();
    outfile[0].close();
    outfile[1].close();
    outfile[2].close();
    outfile[3].close();

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
