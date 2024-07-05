#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <cstdlib>
#include <iomanip>
#include "random.h"
#include "individual.h"

#ifndef __Population__
#define __Population__

class Population
{
private:
    int _population_dim, _ind_size;
    int _sim_type, _generations, _actual_gen;

    double _p; //Selection power (r^p)

    //MUTATION PROBABILITIES :
    double _PPP; //probability of pair permutation
    double _PSMC; //probability of shift of +n m contiguous
    double _PPMC; //probability of permutation among m contiguous
    double _PIO; //probability  of inversion of the order

    //CROSSOVER PROBABILITY:
    double _PCROSS;

    double _length_ave;


    std::vector<Individual> _individuals;
    std::vector<double> _x_city;
    std::vector<double> _y_city;
    Random _rnd;          

public:
    void initialize(int continent);  
    void check_function();
    void sorting();
    void love(int ind1, int ind2);
    void compute_all_length();
    void mutate();
    void crossover();

    void selection();

    void print(int continent);

    int get_dim();
    int get_ind_dim();
    int get_generations();

    void modify_ind(std::vector<int>mod, int pos);

    Individual get_individual(int pos);


};

void completePath(std::vector<int>& child, const std::vector<int>& parent, int cutPoint);
    

#endif