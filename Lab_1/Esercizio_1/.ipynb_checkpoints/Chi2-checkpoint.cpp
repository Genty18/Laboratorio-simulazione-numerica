#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <string>
#include "random.h"
#include <random>

using namespace std;


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


    int M=100; N=10000, N_hit=0, iter=100;
    double I=0, sum=0, ave=0, ave2=0, sum_prog=0, sum_prog2=0, K = double(N)/double(M);
    ofstream outfile("Chi2.dat");

    for(int p=0; p<iter; p++){
        sum = 0;
        for (int i = 0; i < M; ++i) {
            N_hit = 0;
            // Loop all'interno di ogni blocco
            for (int j = 0; j < N; ++j) {
                double x = rnd.Rannyu();
                if(x > (1/M*i) && x < (1/M*(i+1))) N_hit+=1;
            }
            // Calcolo delle medie per il blocco corrente
            sum += pow(N_hit - k, 2);
        }
        Chi2.push_back(sum/k);
    }

    for(int i=0; i<iter; i+) outfile <<Chi2[i] <<endl;
// Salvataggio del seme del generatore di numeri casuali
    rnd.SaveSeed();
    outfile.close();

    return 0;
}