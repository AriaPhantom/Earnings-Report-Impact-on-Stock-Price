#pragma once
#ifndef Group_h
#define Group_h
#include"Stock.h"
#include<map>
#define SampleNum 30
using namespace std;


class Group
{
public:
	map<string, Stock> Content;
	vector<string> Ticker;
	vector<double> AAR;
	vector<double> CAAR;

	void CalAbnormalReturn(Index& benchmark);
	void CalAAR();
	void CalCAAR();
};

void BootStrap(Group& StockPool, Index& benchmark, vector<double>& AAR_, vector<double>& CAAR_);




#endif 
