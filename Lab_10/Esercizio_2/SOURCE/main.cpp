#include <iostream>
#include <vector>
#include "individual.h"
#include "population.h"
#include "mpi.h"
#include "random.h"

using namespace std;

int main (int argc, char *argv[]){
  int size, rank, N_mig=50, counter=0;
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Request request;
  MPI_Status status;
  
  //Starting procedure
  Population pop;
  pop.initialize(rank);
  pop.compute_all_length();
  pop.sorting();

  int to_send[pop.get_ind_dim()]; int to_recive[pop.get_ind_dim()]; int continent[2];

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
      if(rank==0){
        continent[0] = pop.get_random_number(0, size-1);
        continent[1] = pop.get_random_number(0, size-1);
        while (continent[0]==continent[1]) continent[1] = pop.get_random_number(0, size);
      }
      MPI_Bcast(continent,2,MPI_INTEGER,0, MPI_COMM_WORLD);

      if(rank==continent[0]){
        for(int j=0; j < pop.get_ind_dim(); j++) to_send[j] = pop.get_individual(0).get_element(j);
        MPI_Isend(to_send, pop.get_ind_dim(), MPI_INT, continent[1], 0, MPI_COMM_WORLD, &request);
        // Attendi che l'invio sia completato
        MPI_Wait(&request, &status);

        MPI_Irecv(to_recive, pop.get_ind_dim(), MPI_INT, continent[1], 0, MPI_COMM_WORLD, &request);
        // Attendi che la ricezione sia completata
        MPI_Wait(&request, &status);

        //associo la sequenza del continente 2 al continente 1
        vector<int> support1(pop.get_ind_dim());
        for(int k=0; k<int(support1.size()); k++) support1[k]=to_recive[k];
        pop.modify_ind(support1, 0);
      }
      else if(rank==continent[1]){
        for(int j=0; j < pop.get_ind_dim(); j++) to_send[j] = pop.get_individual(0).get_element(j);
        MPI_Isend(to_send, pop.get_ind_dim(), MPI_INT, continent[0], 0, MPI_COMM_WORLD, &request);
        // Attendi che l'invio sia completato
        MPI_Wait(&request, &status);

        MPI_Irecv(to_recive, pop.get_ind_dim(), MPI_INT, continent[0], 0, MPI_COMM_WORLD, &request);
        // Attendi che la ricezione sia completata
        MPI_Wait(&request, &status);

        vector<int> support2(pop.get_ind_dim());
        for(int k=0; k<int(support2.size()); k++) support2[k]=to_recive[k];
        pop.modify_ind(support2, 0);
      }
    }
  }
  cout<< "Continente: "<< rank << " completed! " <<endl;
  MPI_Finalize();
  return 0;
}