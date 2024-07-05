#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <algorithm>
#include "random.h"

#ifndef __Individual__
#define __Individual__

class Individual
{
private:
    std::vector<int> _sequence;
    int _size;
    double _length;
public:
    Individual(int size){
        _size=size;
        for (int i = 0; i <_size; i++) {
            _sequence.push_back(i);
            _sequence[i]+=1;
        }
    } 

    //MUTATION
    void pair_perm(int p1, int p2);
    void shift_Mcontiguous(int m, int n, int initial);
    void perm_Mcontiguous(int m, int n, int initial);
    void inv_order();



    void print(); //for debugging 
    void compute_length(std::vector<double> x_city, std::vector<double> y_city);
    void modify(std::vector<int>& mod); 

    int get_element(int position);
    double get_length() {return _length;}
};


#endif