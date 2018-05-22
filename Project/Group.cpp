#include"Group.h"
#include"Stock.h"
#include"VectorOperator.h"
#include<algorithm>
#include<numeric>
#include<time.h>
using namespace std;

void BootStrap(Group& StockPool, Index& benchmark, vector<double>& AAR_, vector<double>& CAAR_)
{
	Group Sample;
	vector<int> index_(StockPool.Ticker.size()); // create a vector of the number of indexes of beat, meet or miss
	iota(index_.begin(), index_.end(), 0); //fill an iterator range with successively incremented values.
	srand(time(NULL)); // random state
	random_shuffle(index_.begin(), index_.end()); //shuffle the values in index_
	for (unsigned int i = 0; i < SampleNum; i++) //find the first SampleNum shuffled indexes and find corresponding stocks
	{

		map<string, Stock>::iterator itr = StockPool.Content.find(StockPool.Ticker[index_[i]]);
		if (itr != StockPool.Content.end())
			Sample.Content.insert(pair<string, Stock>(StockPool.Ticker[i], itr->second));
	}

	Sample.CalAbnormalReturn(benchmark);
	Sample.CalAAR();
	Sample.CalCAAR();
	AAR_ = AAR_ + Sample.AAR;
	CAAR_ = CAAR_ + Sample.CAAR;

}

void Group::CalAAR()
{
	Vect W(DayNum, 0); // create a vector of double with all zero elements
	map<string, Stock>::iterator it;
	for (it = Content.begin(); it != Content.end(); it++)
		W = W + it->second.AbnormalReturn; // overloaded vector + operator
	W = W / SampleNum; // overloaded vector  / operator
	AAR = W;
}

void Group::CalCAAR()
{
	CAAR = Cumsum(AAR); //function to calculate CAAR for vectors
}


void Group::CalAbnormalReturn(Index& benchmark)
{
	for (map<string, Stock>::iterator itr = Content.begin(); itr != Content.end(); )
	{
		itr->second.CalDailyReturn(); // calculate daily return of each stock first
		itr->second.CalAbnormalReturn(benchmark); // invoke CalAbnormalReturn in Class Stock
		itr++;
	}

}