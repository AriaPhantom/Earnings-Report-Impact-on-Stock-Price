#ifndef VectorOperator_h
#define VectorOperator_h
#include <vector>
#include"Group.h"
using namespace std;

typedef vector<double> Vect;


Vect operator+(const Vect& W, const Vect& V);


Vect operator-(const Vect& W, const Vect& V);

Vect operator / (const Vect& W, const int& a);

Vect& operator/= (Vect& W, const int& a);


Vect Cumsum(const Vect& W);

vector<vector<vector<double>>> SetMatrix(Vect  BeatAAR, Vect  BeatCAAR, Vect  MeetAAR, Vect  MeetCAAR, Vect  MissAAR, Vect  MissCAAR);

vector<vector<string>> SetTitle();

#endif 