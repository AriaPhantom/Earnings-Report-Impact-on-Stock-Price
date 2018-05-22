#pragma once
#ifndef Stock_h
#define Stock_h

#include<string>
#include<vector>
#include<map>
#include<iostream>
#include"ExcelDriver\ExcelDriver.hpp"
#include"ExcelDriver\stdafx.h"
#include"ExcelDriver\BitsAndPieces\StringConversions.hpp"
#include"ExcelDriver\ExcelMechanisms.hpp"
#include"ExcelDriver\VectorsAndMatrices\NumericMatrix.cpp"
#include"ExcelDriver\ExceptionClasses\DatasimException.hpp"

#define DayNum 91
#define SPYRange 357

using namespace std;

//Base class, used for store common info for both SPY and stocks
class Index
{
protected:
	string Ticker; //The tick for Stock or SPY
	string StartTime; //store the date we pull data from
	string EndTime; // Store the date we pull data to


public:
	vector<string> Date; //vector for store all trading date
	vector<double> Price; //vector for store stock price
	vector<double> DailyReturn; //vector for store daily return

	Index() : Ticker("0"), StartTime("0"), EndTime("0") {} // default constructor 
	Index(string ticker, string startdate, string enddate) :Ticker(ticker), StartTime(startdate), EndTime(enddate) {} //assigment constructr

	void CalDailyReturn();// calculate the DailyReturn from Price

	void SetTicker(string name);
	void SetStartTime(char* source);
	void SetEndTime(char* source);
	
	string GetTicker() const;
	string GetStartTime() const;
	string GetEndTime() const;
};

//A derived class based on Index to store more specific information about a stock
class Stock : public Index
{

public:
	string Est_EPS; //Estimated EPS
	string Act_EPS; //Actual EPS
	string Act_to_Est; //(Act_EPS - Est_EPS) / Est_EPS * 100
	vector<double> AbnormalReturn; //vector for storing abnormal return

	Stock() :Index() { Act_EPS = "0"; Est_EPS = "0"; Act_to_Est = "0"; }// default constructor
	Stock(string ticker, string startdate, string enddate, string act_eps, string est_eps, string act_to_est)// assignment constructor
		:Index(ticker, startdate, enddate) { Act_EPS = act_eps; Est_EPS = est_eps; Act_to_Est = act_to_est; }

	void ExportData() const; //print data in excel

	int check() const; // find if there is any zero price for a stock

	void CalAbnormalReturn(Index& benchmark); //Calculate the abnormal return

	friend ostream& operator<<(ostream& os, const Stock& source); // operator overloading for << 
};
#endif