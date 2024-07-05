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


    int M=100, N=10000, N_hit=0, iter=100;
    double sum=0, k=double(N)/double(M);
    ofstream outfile("Chi2.dat");
    std::vector<double> Chi2;

    for(int p=0; p<iter; p++){
        sum = 0;
        for (int i = 0; i < M; ++i) {
            N_hit = 0;
            // Loop all'interno di ogni blocco
            for (int j = 0; j < N; ++j) {
                double x = rnd.Rannyu();
                double down= 1/double(M)*double(i), up=1/double(M)*double(i+1);
                if(x > down && x < up) N_hit+=1;
            }
            // Calcolo delle medie per il blocco corrente
            sum += pow(N_hit - k, 2);
        }
        Chi2.push_back(sum/k);
    }
    for(int i=0; i<iter; i++) outfile <<Chi2[i] <<endl;
// Salvataggio del seme del generatore di numeri casuali
    rnd.SaveSeed();
    outfile.close();
    Chi2.clear();

    return 0;
}