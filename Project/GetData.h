#pragma once
#include<map>
#include <stdio.h>
#include <string> 
#include <iostream>
#include <sstream>  
#include <vector>
#include <locale>
#include <iomanip>
#include "curl.h"
#include<fstream>
#include"Group.h"

using namespace std;

#ifndef GetData
#define GetData


void GetStockPool(Group& Beat, Group& Meet, Group& Miss, string* fileList);
size_t write_data(void *ptr, int size, int nmemb, FILE *stream);
void *myrealloc(void *ptr, size_t size);
size_t write_data2(void *ptr, size_t size, size_t nmemb, void *data);
string getTimeinSeconds(string Time);
int GetStockPrice(Group& StockList);
int GetSPYPrice(Index& Spy);
#endif 