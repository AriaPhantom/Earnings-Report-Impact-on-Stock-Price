#include"Stock.h"
#include"VectorOperator.h"
#include<algorithm>
#include<sstream>
using namespace std;

ostream& operator<<(ostream& os, const Stock& source)
{
	os << "Ticker: " << source.Ticker << endl << "Announcement Date: " << source.Date[62]
		<< endl << "Actual EPS: " << source.Act_EPS << endl << "Estimate EPS: " << source.Est_EPS
		<< endl << "(Actual EPS - Estimate EPS) / Estimate EPS * 100: " << source.Act_to_Est << endl;
	return os;
}

void Index::SetTicker(string name)
{
	Ticker = name;
}

string Index::GetTicker() const
{
	return Ticker;
}

void Index::SetStartTime(char* source)
{
	StartTime = source;
}

string Index::GetStartTime() const
{
	return StartTime;
}

void Index::SetEndTime(char* source)
{
	EndTime = source;
}

string Index::GetEndTime() const
{
	return EndTime;
}

void Index::CalDailyReturn()
{
	int size = Price.size();
	for (int i = 0; i < size - 1; i++)// Generate DayNum-1 return based on DayNum price
	{
		DailyReturn.push_back(Price[i+1] / Price[i] - 1);
	}
}

int Stock::check() const
{
	int size = Price.size();
	for (int i = 0; i < size; i++) //go through all the price vector
	{
		if (Price[i] < 0.01) return 1;
	}
	return 0;
}


void Stock::CalAbnormalReturn(Index& benchmark)
{
	vector<double> benchmarkReturn;
	vector<string>::iterator it = std::find(benchmark.Date.begin(), benchmark.Date.end(), StartTime);// find the same trading date inside Index
	int a = std::distance(benchmark.Date.begin(), it);
	for (int i = a; i<a + DayNum; i++) //export DayNum days return since the trading date
		benchmarkReturn.push_back(benchmark.DailyReturn[i]);
	AbnormalReturn = DailyReturn - benchmarkReturn;
}

void Stock::ExportData() const
{
	NumericMatrix<double, long> matrix(92, 1);
	NumericMatrix<double, long> info(1, 4);

	list<std::string> rowlabels;//row lable
	list<std::string> columnlabels; //coolumn lable

	list<std::string> inforow; //row's information
	list<std::string> infocol; //column's information
	string day0;

	inforow.push_back(Ticker);
	infocol.push_back("Est_EPS");
	infocol.push_back("Act_EPS");
	infocol.push_back("(Act - Est) / Est * 100");
	infocol.push_back("Day0");
	day0 = Date[61];
	day0 = day0.erase(4, 1).erase(6, 1);
	stringstream s_eEPS(Est_EPS), s_aEPS(Act_EPS), s_e2sEPS(Act_to_Est), s_date(day0);
	double d_eEPS, d_aEPS, d_e2sEPS, d_date;
	s_eEPS >> d_eEPS;
	s_aEPS >> d_aEPS;
	s_date >> d_date;
	s_e2sEPS >> d_e2sEPS;
	info(1, 1) = d_eEPS;
	info(1, 2) = d_aEPS;
	info(1, 3) = d_e2sEPS;
	info(1, 4) = d_date;
	columnlabels.push_back("Price");
	for (int i = 0; i < 92; i++)
	{
		matrix(i + 1, 1) = Price[i];
		rowlabels.push_back(Date[i]);
	}
	std::string sheetName= Ticker+" Price";
	std::string sheetName1= Ticker + " Information";
	ExcelDriver& excel = ExcelDriver::Instance();
	excel.MakeVisible(true);	
	excel.AddMatrix(sheetName, matrix, rowlabels, columnlabels);
	excel.AddMatrix(sheetName1, info, inforow, infocol);
}
