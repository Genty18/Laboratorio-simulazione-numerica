#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <cmath>
#include "system.h"

using namespace std;

int main (int argc, char *argv[]){

  System SYS;
  SYS.initialize();
  double x = SYS.get_initial_pt();
  for(int i=0; i < SYS.get_nblk(); i++){ //loop over blocks
    for(int j=0; j < SYS.get_nsteps(); j++){ //loop over steps in a block
      x = SYS.metro(x);
      SYS.measure_H(x);
    }
    SYS.averages(i+1);
  }
  return 0;
}