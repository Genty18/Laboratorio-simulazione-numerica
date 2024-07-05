#include <iostream>
#include "system.h"

using namespace std;

int main (int argc, char *argv[]){

  System SYS;
  SYS.initialize();
  double x = SYS.get_initial_pt();
  int step = 0;
  for(int k=0; k<SYS.get_beta_step(); k++){
    for(int j=0; j < SYS.get_SA_steps(); j++){ //loop over steps 
      SYS.measure_H(x);
      SYS.metro_SA();
      step++;
      SYS.plot(step);
    }
    SYS.modify_beta();
  }
  return 0;
}