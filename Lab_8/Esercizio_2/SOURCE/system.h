#include "random.h"

#ifndef __System__
#define __System__

class System
{
private:
    double _mu, _proposed_mu;
    double _sigma, _proposed_sigma;
    double _beta, _d_x, _x_start;
    double _delta, _delta_mu, _delta_sigma;
    double _initial_pt;
    int _naccepted, _nattempts;
    int _metro_steps, _SA_steps, _beta_steps, _nblocks;
    double _H, _err_H, _H_TRY, _err_H_TRY;
    double _average, _block_av, _global_av, _global_av2; 
    Random _rnd;          

public:
    void initialize();  

    double psi_mod(double x, double mu, double sigma);
    double propose(double x_old);
    void propose_mu();
    void propose_sigma();
    double V(double x);
    double get_energy(double x, double mu, double sigma);
    double boltzmann(double x);
    void measure_H(double x);
    double error(double acc, double acc2, int blk);

    void plot(int step);
    void modify_beta();

    int get_metro_steps();
    int get_beta_step();
    int get_SA_steps();
    double get_initial_pt();
    void averages(int blk);
    void reset_averages();


    double metro(double x, double mu, double sigma);
    void metro_SA();
};


#endif