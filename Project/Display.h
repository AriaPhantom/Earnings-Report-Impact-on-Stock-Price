#pragma once
#include"ExcelDriver\ExcelDriver.hpp"
#include"ExcelDriver\stdafx.h"
#include"ExcelDriver\BitsAndPieces\StringConversions.hpp"
#include"ExcelDriver\ExcelMechanisms.hpp"
#include"ExcelDriver\VectorsAndMatrices\NumericMatrix.cpp"
#include"ExcelDriver\ExceptionClasses\DatasimException.hpp"
#include"Group.h"
using namespace std;

#ifndef Display
#define Display

void Menu(const Group& Beat, const Group& Meet, const Group& Miss, const vector<vector<vector<double>>>& ResultMatrix);
void ShowStockInfo(const Group& Beat, const Group& Meet, const Group& Miss);
void ShowStat(const vector<vector<vector<double>>>& ResultMatrix);
void ShowPic(const vector<vector<vector<double>>>& ResultMatrix);

#endif 