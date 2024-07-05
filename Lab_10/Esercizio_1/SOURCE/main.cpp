#include <iostream>
#include <vector>
#include "individual.h"
#include "population.h"
#include "mpi.h"
#include "random.h"

using namespace std;

int main (int argc, char *argv[]){
  int size, rank, N_mig=700, counter=0;
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Request request;
  MPI_Status status;

  Random rnd;
  int seed[4];
    int p1, p2;
    ifstream Primes("../INPUT/Primes");
    if (Primes.is_open()){
      Primes >> p1 >> p2;
    }else cerr << "PROBLEM: Unable to open Primes" << endl;
    Primes.close();
    ifstream input_rnd("../INPUT/seed.in");
    string property_rnd;
    if (input_rnd.is_open()){
        while ( !input_rnd.eof() ){
            input_rnd >> property_rnd;
            if( property_rnd == "RANDOMSEED" ){
                input_rnd >> seed[0] >> seed[1] >> seed[2] >> seed[3];
                rnd.SetRandom(seed,p1,p2);
            }
        }
        input_rnd.close();
    } else cerr << "PROBLEM: Unable to open seed.in" << endl;
  
  //Starting procedure
  Population pop;
  pop.initialize(rank);
  pop.compute_all_length();
  pop.sorting();

  int to_send[pop.get_ind_dim()]; int to_recive[pop.get_ind_dim()];

  for(int i=0; i<pop.get_generations(); i++){
    pop.selection();
    pop.crossover();
    pop.mutate();
    pop.check_function();
    pop.compute_all_length();
    pop.sorting();
    pop.print(rank+1);
    counter++;
    if(counter == N_mig){
      counter = 0;
      int continent1 = int(rnd.Rannyu(0, size));
      int continent2 = int(rnd.Rannyu(0, size));
      while (continent1==continent2) continent2 = int(rnd.Rannyu(0, size));


      if(rank==continent1){
        for(int j=0; j < pop.get_ind_dim(); j++) to_send[j] = pop.get_individual(0).get_element(j);
        MPI_Isend(to_send, pop.get_ind_dim(), MPI_INT, continent2, 0, MPI_COMM_WORLD, &request);
        // Attendi che l'invio sia completato
        MPI_Wait(&request, &status);

        MPI_Irecv(to_recive, pop.get_ind_dim(), MPI_INT, continent2, 0, MPI_COMM_WORLD, &request);
        // Attendi che la ricezione sia completata
        MPI_Wait(&request, &status);

        //associo la sequenza del continente 2 al continente 1
        vector<int> support1(pop.get_ind_dim());
        for(int k=0; k<support1.size(); k++) support1[k]=to_recive[k];
        pop.modify_ind(support1, 0);
      }
      else if(rank==continent2){
        for(int j=0; j < pop.get_ind_dim(); j++) to_send[j] = pop.get_individual(0).get_element(j);
        MPI_Isend(to_send, pop.get_ind_dim(), MPI_INT, continent1, 0, MPI_COMM_WORLD, &request);
        // Attendi che l'invio sia completato
        MPI_Wait(&request, &status);

        MPI_Irecv(to_recive, pop.get_ind_dim(), MPI_INT, continent1, 0, MPI_COMM_WORLD, &request);
        // Attendi che la ricezione sia completata
        MPI_Wait(&request, &status);

        vector<int> support2(pop.get_ind_dim());
        for(int k=0; k<support2.size(); k++) support2[k]=to_recive[k];
        pop.modify_ind(support2, 0);
      }
    }
  }
  cout<< "Continente: "<< rank << " completed! " <<endl;
  MPI_Finalize();
  return 0;
}