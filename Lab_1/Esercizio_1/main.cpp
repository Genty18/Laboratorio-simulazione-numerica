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

// Funzione per calcolare l'errore statistico
double error(double AV, double AV2, int n) {
    if (n == 0)
        return 0;
    else
        return sqrt((AV2 - pow(AV, 2)) / n);
}

int main() {
    // Numero totale di lanci
    int M = 100000;
    // Numero di blocchi
    int N = 100;
    // Numero di lanci per blocco
    int L = M / N;

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

    // Variabili per il calcolo delle medie
    double r, ave, av2, sum_prog = 0, su2_prog = 0, err_prog;
     // Variabili per il calcolo della deviazione
    double sigma = 0, sigmaq = 0, sigma_prog = 0, sigmaq_prog = 0, err2_prog;

    // File di output
    ofstream outfile("Media.dat");
    ofstream outfile2("Deviazione.dat");
    // Loop sui blocchi
    for (int i = 0; i < N; ++i) {
        double sum1 = 0;
        double sum2 = 0;
        // Loop all'interno di ogni blocco
        for (int j = 0; j < L; j++) {
            // Generazione di un numero casuale
            r = rnd.Rannyu();
            // Accumulo dei valori per il calcolo delle medie
            sum1 += r;
            //Accumulo valori sigma
            sum2 += pow(r - 0.5, 2);
        }
        // Calcolo delle medie per il blocco corrente
        ave = sum1 / L;
        sigma = sum2 / L;
        av2 = pow(ave, 2);
        sigmaq = pow(sigma, 2);

        // Accumulo delle medie
        sum_prog += ave;
        su2_prog += av2;
        sigma_prog += sigma;
        sigmaq_prog += sigmaq;

        // Calcolo delle medie progressive
        double avg_prog = sum_prog / (i + 1);
        double avg2_prog = su2_prog / (i + 1);

        // Calcolo delle deviazioni progressive
        double sigmaM_prog = sigma_prog / (i + 1);
        double sigmaqM_prog = sigmaq_prog / (i + 1);

        // Calcolo dell'errore statistico
        err_prog = error(avg_prog, avg2_prog, i);
        err2_prog = error(sigmaM_prog, sigmaqM_prog, i);

        // Scrittura dei risultati sul file di output
        outfile << i * L << " " << avg_prog - 0.5 << " " << err_prog << endl;
        outfile2 << i * L << " " << sigmaM_prog - 1/12 << " " << err2_prog << endl;
    }

    // Chiusura del file di output
    outfile.close();
    outfile2.close();

    rnd.SaveSeed();

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
