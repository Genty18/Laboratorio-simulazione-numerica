#include "random.h"

#ifndef __System__
#define __System__

class System
{
private:
    double _mu;
    double _sigma;
    double _beta;
    double _delta;
    double _initial_pt;
    double _range_histo;
    int _naccepted, _nattempts;
    int _nsteps, _nblocks, _histo_bins;
    double _block_av;
    double _average;
    double _global_av;
    double _global_av2; 

    std::vector<int> _histo;
    Random _rnd;          

public:
    void initialize();  

    double psi_mod(double x);
    double propose(double x_old);
    double V(double x);
    double get_energy(double x);
    double boltzmann(double x);
    void measure_H(double x);
    double error(double acc, double acc2, int blk);

    void averages(int blk);
    void histo(double x);
    int get_nsteps();
    int get_nblk();
    double get_initial_pt();

    double metro(double x);
};


#endif