#include <iostream>
#include <vector>
#include "individual.h"
#include "population.h"

using namespace std;

int main (int argc, char *argv[]){
    Population pop;

    //Starting procedure
    pop.initialize();
    pop.compute_all_length();
    pop.sorting();

    for(int i=0; i<pop.get_generations(); i++){
      pop.selection();
      pop.crossover();
      pop.mutate();
      pop.check_function();
      pop.compute_all_length();
      pop.sorting();
      pop.print();
    }
    

  return 0;
}