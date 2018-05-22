#include "VectorOperator.h"

Vect operator+(const Vect& W, const Vect& V) //adds two vectors
{
	size_t d = W.size();
	Vect M(d);
	for (unsigned int i = 0; i<d; i++) M[i] = W[i] + V[i];
	return M;
}

Vect operator-(const Vect& W, const Vect& V)//subtracts one vector from another
{
	size_t d = W.size();
	Vect M(d);
	for (unsigned int i = 0; i<d; i++) M[i] = W[i] - V[i];
	return M;
}

Vect& operator /= (Vect& W, const int& a)//divides each element of the vector by some integer and stores inside the same vector
{
	size_t d = W.size();
	for (unsigned int i = 0; i<d; i++) W[i] = W[i] / a;
	return W;
}

Vect operator / (const Vect& W, const int& a)//divides each element of the vector by some integer and stores inside other vector
{
	size_t d = W.size();
	Vect M(d);
	for (unsigned int i = 0; i<d; i++) M[i] = W[i] / a;
	return M;
}

Vect Cumsum(const Vect& W)//calculates the cumulative sum of the elements of a vector and stores them inside another vector; this is used to calculate CAAR, by summing the elements of AAR
{
	Vect V(W.size());
	V[0] = W[0];
	for (unsigned int i = 1; i<W.size(); i++)
		V[i] = V[i - 1] + W[i];
	return V;
}
vector<vector<vector<double>>> SetMatrix(Vect  BeatAAR, Vect  BeatCAAR, Vect  MeetAAR, Vect  MeetCAAR, Vect  MissAAR, Vect  MissCAAR)//this function stores the AAR and CAAR vectors for 3 groups inside a matrix
{
	vector<vector<vector<double>>> ReturnMatrix;
	ReturnMatrix.resize(3); //creates 3 rows
	for (int i = 0; i<3; i++)
		ReturnMatrix[i].resize(2); //each row has 2 columns
	ReturnMatrix[0][0] = BeatAAR; //cell [0][0] contains vector BeatAAR
	ReturnMatrix[1][0] = MeetAAR;
	ReturnMatrix[2][0] = MissAAR;
	ReturnMatrix[0][1] = BeatCAAR;
	ReturnMatrix[1][1] = MeetCAAR;
	ReturnMatrix[2][1] = MissCAAR;
	return ReturnMatrix;
}
vector<vector<string>> SetTitle()//this function is used to name Excel worksheets
{
	vector<vector<string>> title;
	title.resize(2); //creates 2 rows
	for (int i = 0; i<2; i++)
		title[i].resize(3);  //each row has 3 columns
	title[0][0] = "Beat AAR";  //cell [0][0] contains string "Beat AAR"
	title[0][1] = "Meet AAR";
	title[0][2] = "Miss AAR";
	title[1][0] = "Beat CAAR";
	title[1][1] = "Meet CAAR";
	title[1][2] = "Miss CAAR";
	return title;
};