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


//funzioni 
double Get_ascissa(double L, double m) {
    return abs(m*L)*sqrt(1/(1+m*m));
}
double Get_m(double p1_x,double p1_y, double p2_x, double p2_y) {
   return (p2_x - p1_x)/(p2_y - p1_y);
}
double Get_pi(double L,double d, int N_hit, int N) {
   return 2*L*N/(N_hit*d);
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



 // Dichiarazione delle variabili
double L=5, d=10, x_centro=0, av=0, av2=0, sum=0, sum2=0; 
int N_hit=0, N=100, M=1E7, N_th=M/N;

// Creazione dei vettori
vector<double> m;
vector<double> point1(2); // [0,1] = x e y dei due punti per generare una pendenza
vector<double> point2(2); // [0,1] = x e y dei due punti per generare una pendenza
vector<double> x_estremi(2); // [0,1] = ascissa estremo destro e sinistro dell'ago

// Apertura del file di output
ofstream streamout("data.dat");
ofstream outfile("testM.dat");

// Loop principale
for (int i = 0; i < N; ++i) {
    N_hit=0;
    // Loop all'interno di ogni blocco
    for (int j = 0; j < N_th; ++j) {
        // Generazione dei punti casuali
        x_centro = rnd.Rannyu(0, d);
        point1[0] = rnd.Rannyu(0, d);
        point1[1] = rnd.Rannyu(0, d);
        point2[0] = rnd.Rannyu(0, d);
        point2[1] = rnd.Rannyu(0, d);
        m.push_back(Get_m(point1[0], point1[1], point2[0], point2[1]));
        x_estremi[0] = x_centro + Get_ascissa(L/2, m[j]);
        x_estremi[1] = x_centro - Get_ascissa(L/2, m[j]);

        // Controllo se l'ago ha colpito la linea
        if(x_estremi[0]>d || x_estremi[1]<0) N_hit+=1;
    }

    // Calcolo di pi progressivi
    sum += Get_pi(L, d, N_hit, N_th);
    sum2 += pow(Get_pi(L, d, N_hit, N_th),2);      
    // Calcolo delle medie
    av = sum/(i+1);
    av2 = sum2/(i+1);

    // Scrittura dei risultati nel file di output
    streamout <<i*N_th <<" " <<av <<" " <<error(av, av2, i) <<endl;
}

for(int k=0; k<10000; k++) outfile <<m[k] <<endl;
// Salvataggio del seme del generatore di numeri casuali
rnd.SaveSeed();

//chiudo stream
streamout.close();
outfile.close();
// Pulizia dei vettori
point1.clear();
m.clear();
point2.clear();
x_estremi.clear();

return 0;

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
