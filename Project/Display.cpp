#include"Display.h"
#include"Stock.h"
#include"VectorOperator.h"
#include "Colors.h"

using namespace std;
// 2 columns, 3 rows for result matrix, containing beat/meet/miss AAR then beat/meet/miss CAAR
void Menu(const Group& Beat, const Group& Meet, const Group& Miss, const vector<vector<vector<double>>>& ResultMatrix)
{
	// string because when did Int, the code kept crashing when we put in the wrong data (ex, fdasdf instead of 2)
	string c = "0";
	// setting a checkpoint to return to 
CheckPoint:
	{
		// primary menu feature, using colors to make it pretty
		cout << greencolor << " --------------------------------------------------------------------" << endl;
		cout << " |Make sure to use capital letters for the stock selection and      |" << endl;
		cout << " |enter a number when asked for a number                            |" << endl;
		cout << " --------------------------------------------------------------------" << endl << defcolor;
		cout << " *                                                                  *" << endl;
		cout << " --------------------------------------------------------------------" << endl;
		cout << " |Would you like to...                                              |" << endl;
		cout << redcolor << " |1) Obtain data on a single stock from one group?                  |" << endl;
		cout << greencolor << " |2) Show the AAR and CAAR for the entirety of one group?           |" << endl;
		cout << bluecolor << " |3) Plot the Excel chart for CAAR of all 3 groups?                 |" << endl;
		cout << lredcolor << " |4) Quit?                                                          |" << endl << defcolor;
		cout << " --------------------------------------------------------------------" << endl;
		cout << endl;
		cout << "Which menu option would you like to see? Please enter the corresponding number: "; cin >> c;
		// checking to see what option was entered, and then executing corresponding function
		if (c.compare(string("1")) == 0)
		{
			ShowStockInfo(Beat, Meet, Miss);
			cout << endl; cout << endl; cout << endl;
			goto CheckPoint;
		}
		else if (c.compare(string("2")) == 0)
		{
			ShowStat(ResultMatrix);
			cout << endl; cout << endl; cout << endl;
			goto CheckPoint;
		}
		else if (c.compare(string("3")) == 0)
		{
			ShowPic(ResultMatrix);
			cout << endl; cout << endl; cout << endl;
			goto CheckPoint;
		}
		else if (c.compare(string("4")) == 0)
			cout << "Quit." << endl;
		else
		{ // not allowing you to enter an invalid number, it will restart back to checkpoint
			cout << "Please enter a valid number." << endl;
			cout << endl; cout << endl; cout << endl;
			goto CheckPoint;
		}

		cout << endl; cout << endl; cout << endl;
	}
}
// function to display information for a particular stock
void ShowStockInfo(const Group& Beat, const Group& Meet, const Group& Miss)
{
	cout << "You choose 1." << endl;
	string sticker; // enter the ticker you're interested in
	cout << "Enter the ticker: "; cin >> sticker;;
	// map with constant iterator to only read, can't write. itr then goes through the content (map containing the ticker and stock data) of the beat group to search for the ticker. 
	// if the itr does not reach the end, meaning it finds the correct ticker, it starts the exportdata function from Stock.cpp, which generates the excel matrix with 
	// labels for tabs price history and information 
	// if it does reach the end, it goes to next if statement to repeat the same steps through 
	map<string, Stock>::const_iterator itr1 = Beat.Content.find(sticker);
	if (itr1 != Beat.Content.end())
	{
		itr1->second.ExportData();
		cout << sticker << " information has been exported to Excel" << endl;
	}

	map<string, Stock>::const_iterator itr2 = Meet.Content.find(sticker);
	if (itr2 != Meet.Content.end())
	{
		itr2->second.ExportData();
		cout << sticker << " information has been exported to Excel" << endl;
	}

	map<string, Stock>::const_iterator itr3 = Miss.Content.find(sticker);
	if (itr3 != Miss.Content.end())
	{
		itr3->second.ExportData();
		cout << sticker << " information has been exported to Excel" << endl;
	}

}
// function to display AAR/CAAR for beat/meet/miss
// vector/vector/vector because we need a matrix where each cell is a vector
// ResultMatrix is 2 columns 3 rows of Beat/Meet/Miss AAR, then Beat/Meet/Miss CAAR
void ShowStat(const vector<vector<vector<double>>>& ResultMatrix)
{
	cout << "You choose 2." << endl;
	int indicator1, indicator2, a, b;
	cout << "1.Show AAR." << endl;
	cout << "2.Show CAAR." << endl;
	cin >> indicator2; // indicator2 says if we want AAR or CAAR
	if (indicator2 != 1 && indicator2 != 2)
		cout << "Enter a valid number please." << endl;
	cout << "1.Show group Beat." << endl;
	cout << "2.Show group Meet." << endl;
	cout << "3.Show group Miss." << endl;
	cin >> indicator1; //indicator1 tells us if we want beat, meet or miss group
	if (indicator1 != 1 && indicator1 != 2 && indicator1 != 3)
		cout << "Enter a valid number please." << endl;
	NumericMatrix<double, long> Data(DayNum, 1); // creates matrix from exceldriver of row type double, column type long, named Data, of 91 row, 1 columns
	b = indicator1 - 1; // subtracting 1 to make it consisent with how we've defined them in the past, such as in vectoroperator.cpp (ex. [0][0],[0][1], etc)
	a = indicator2 - 1;
	for (long i = 1; i <= DayNum; i++) // goes from 1 to 91, 1 by 1, setting the values of the cells to the correct data. Such as [0][0] is beatAAR, [0][1] is BeatCAAR, etc.
		Data(i, 1) = ResultMatrix[b][a][i - 1];  // which matrix it returns is in VectorOperator
	list<string> inforow;  // create two lists of type string for the names of columns and row
	list<string> infocol;
	if (indicator2 == 1) // insert the correct column and row names depending on the indicator values
	{
		infocol.push_back("AAR");
	}
	else
	{
		infocol.push_back("CAAR");
	}
	for (int i = 0; i < DayNum; i++) // name what day it is, such like day 0, day 1, day 2, etc.
	{

		inforow.push_back("Day " + to_string(i - 60));
	}

	vector<vector<string>>title; // create a matrix of strings to store the title name
	title = SetTitle(); // SetTitle comes from VectorOperator and names the tab name based on the dimensions
	ExcelDriver& excel = ExcelDriver::Instance(); // create instance of excel
	excel.MakeVisible(true); // default is visible, so make it visible
	excel.AddMatrix(title[a][b], Data, inforow, infocol); // generate the matrix in excel with the correct label based on the title values, data(includes the corresponding CAAR or AAR data for beat/meet/miss
	if (indicator2 == 1 && indicator1 == 1) // simply indicating to the user which option was chosen and that the data has been exported to excel
	{
		cout << "AAR Information for Beat group has been exported to Excel" << endl;
	}
	else if (indicator2 == 1 && indicator1 == 2)
	{
		cout << "AAR Information for Meet group has been exported to Excel" << endl;
	}
	else if (indicator2 == 1 && indicator1 == 3)
	{
		cout << "AAR Information for Miss group has been exported to Excel" << endl;
	}
	else if (indicator2 == 2 && indicator1 == 1) {
		cout << "CAAR information for Beat group has been exported to Excel" << endl;
	}
	else if (indicator2 == 2 && indicator1 == 2) {
		cout << "CAAR information for Meet group has been exported to Excel" << endl;
	}
	else if (indicator2 == 2 && indicator1 == 3) {
		cout << "CAAR information for Miss group has been exported to Excel" << endl;
	}
}
// function to display the chart of CAAR
// vector/vector/vector because we need a matrix where each cell is a vector
// ResultMatrix is 2 columns 3 rows of Beat/Meet/Miss AAR, then Beat/Meet/Miss CAAR
void ShowPic(const vector<vector<vector<double>>>& ResultMatrix) // uses the same as ShowStat of just the resultmatrix. const because we don't want it to change
{
	cout << "You choose 3." << endl;
	Vector<double, long> CAARx1(DayNum); // creating vectors using class form exceldriver (capital V)...name CAARx1 91 units long
	Vector<double, long> CAARx2(DayNum); // 
	Vector<double, long> CAARx3(DayNum);
	Vector<double, long> CAARy1(DayNum);
	Vector<double, long> CAARy2(DayNum);
	Vector<double, long> CAARy3(DayNum);
	CAARx1[CAARx1.MinIndex()] = -60.0;  // sets start of vector at -60
	CAARx2[CAARx2.MinIndex()] = -60.0;
	CAARx3[CAARx3.MinIndex()] = -60.0;
	for (long i = CAARx1.MinIndex() + 1; i <= CAARx1.MaxIndex(); i++) // counting up from -60, start, to the max, which is 91
	{
		CAARx1[i] = CAARx1[i - 1] + 1; // each time it counts up and adds 1, from -60 to 30. this is simply the values of the x axis
		CAARx2[i] = CAARx2[i - 1] + 1;
		CAARx3[i] = CAARx3[i - 1] + 1;
	}

	for (long i = CAARy1.MinIndex(); i <= CAARy1.MaxIndex(); i++) // from start of CAAR, which is zero, to end of CAAR, adding 1 each time, till 90
	{
		CAARy1[i] = ResultMatrix[0][1][i - 1]; // this line copies the specific value in the position i-1 to the CAARy1 exceldriver vector. for Beat
		CAARy2[i] = ResultMatrix[1][1][i - 1]; // for Meet 
		CAARy3[i] = ResultMatrix[2][1][i - 1]; // for Miss 
	}

	list<std::string> labels; // list of type string named labels
	list<Vector<double, long> > yyy; // list of excel driver vectors named yyy
	labels.push_back("Beat"); // puts the name Beat to the end of the labels list, which is the column name 
	labels.push_back("Meet");
	labels.push_back("Miss");

	yyy.push_back(CAARy1); // puts the entire vector from the of beat values into the yyy list
	yyy.push_back(CAARy2); // meet values
	yyy.push_back(CAARy3); // miss values
	cout << endl;
	cout << "CAAR Information for all 3 groups has been exported to Excel" << endl;
	printInExcel(CAARx1, labels, yyy, "CAAR", "Days", "Abnormal return"); // uses exceldriver function to create the chart by printing the data into excel and then graphing it

}