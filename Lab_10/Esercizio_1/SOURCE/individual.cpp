#include "individual.h"

using namespace std;

void Individual::pair_perm(int p1, int p2){
  if(p1 != 1 && p2!= 1){
    double support;
    support = _sequence[p1];
    _sequence[p1] = _sequence[p2];
    _sequence[p2] = support;
  }
}

void Individual::shift_Mcontiguous(int m, int n, int initial){
  if(initial!= 0){
    if(initial+m-1+n <= int(_sequence.size())){
      rotate(_sequence.begin() + initial, _sequence.begin()+initial+m, _sequence.begin()+m+initial+n);
    }
  }
}

void Individual::perm_Mcontiguous(int m, int n, int initial){
  if(initial!= 0){
    if(m < int(_sequence.size()/2)){
      if(initial+m-1+n <= int(_sequence.size())){
        std::vector<int> copy;
        for(int k=0; k < _size; k++) copy.push_back(_sequence[k]); //creating a copy
        for(int i=initial; i<initial + m-1; i++){
          _sequence[i+n] = _sequence[i];
          _sequence[i] = copy[i+n];
        }
      }
    }
  }
}

void Individual::inv_order(){
  reverse(_sequence.begin()+1, _sequence.end());
}

void Individual::modify(std::vector<int>& mod){
  for(int i=0; i<_size; i++) _sequence[i] = mod[i];
}


void Individual::print(){
  for (int i = 0; i < _size; i++)
  {
    cout <<_sequence[i] <<" ";
  }
}

void Individual::compute_length(std::vector<double> x_city, std::vector<double> y_city){
  if(_sequence.size() == x_city.size() && _sequence.size() == y_city.size()){
    double sum=0;
    for(int i=0; i<int(_sequence.size()-1); i++){
        sum += pow(x_city[_sequence[i+1]-1] - x_city[_sequence[i]-1], 2) + pow(y_city[_sequence[i+1]-1] - y_city[_sequence[i]-1], 2);
    }
    _length = sum;
  }
}

int Individual::get_element(int position){
  return _sequence[position];
 }
