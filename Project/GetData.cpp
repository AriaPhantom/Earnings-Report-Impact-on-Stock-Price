#include"GetData.h"
#include"Stock.h"
#include <sstream>
using namespace std;
template <class Type>

Type stringToNum(const string& str)// function type to change from string to other type, (double in our case)
{
	istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}

void GetStockPool(Group& Beat, Group& Meet, Group& Miss, string* file) //function to sort the stocks into Beat, Meet, and Miss
{

	Group StockPool[3];   //3 objects of the class Group to store each stock's data
	string ticker, start, end, act_eps, est_eps, act_to_est; //variables to store the ticker, start, etc information of each stock



	ifstream infile(file[0]);  //read a file
	while (infile.good())   //if we can read the file...
	{

		getline(infile, ticker, '\t');//store the data till the first tab inside ticker variable
		getline(infile, start, '\t');//store the data from the first tab till the second tab inside start variable
		getline(infile, end, '\t');
		getline(infile, act_eps, '\t');
		getline(infile, est_eps, '\t');
		getline(infile, act_to_est, '\n');//go to the next line


		if (stringToNum<double>(act_to_est) >= 5.0)//convert the string act_to_est into type double, then if it is greater than 5%, do
		{
			StockPool[0].Content.insert(pair<string, Stock>(ticker, Stock(ticker, start, end, act_eps, est_eps, act_to_est)));//store each stock's data inside a Content map, which is part of a Stockpool[0] Group
			StockPool[0].Ticker.push_back(ticker); //store each stock's ticker inside a Ticker vector, which is also part of a Stockpool[0] Group
		}
		else if (stringToNum<double>(act_to_est) <= -5.0)
		{
			StockPool[2].Content.insert(pair<string, Stock>(ticker, Stock(ticker, start, end, act_eps, est_eps, act_to_est)));
			StockPool[2].Ticker.push_back(ticker);
		}
		else
		{
			StockPool[1].Content.insert(pair<string, Stock>(ticker, Stock(ticker, start, end, act_eps, est_eps, act_to_est)));
			StockPool[1].Ticker.push_back(ticker);
		}

	}

	Beat = StockPool[0];  //Define Group StockPool[0] as a Group Beat
	Meet = StockPool[1];
	Miss = StockPool[2];
}

struct MemoryStruct
{
	char *memory;
	size_t size;
};

size_t write_data(void *ptr, int size, int nmemb, FILE *stream)
{
	size_t written;
	written = fwrite(ptr, size, nmemb, stream);
	return written;
}

void *myrealloc(void *ptr, size_t size)
{
	/* There might be a realloc() out there that doesn't like reallocing
	NULL pointers, so we take care of it here */
	if (ptr)
		return realloc(ptr, size);
	else
		return malloc(size);
}

size_t write_data2(void *ptr, size_t size, size_t nmemb, void *data)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)data;

	mem->memory = (char *)myrealloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory) {
		memcpy(&(mem->memory[mem->size]), ptr, realsize);
		mem->size += realsize;
		mem->memory[mem->size] = 0;
	}
	return realsize;
}

string getTimeinSeconds(string Time)
{
	std::tm t = { 0 };
	std::istringstream ssTime(Time);
	char time[100];
	memset(time, 0, 100);
	if (ssTime >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S"))
	{
		//cout << std::put_time(&t, "%c %Z") << "\n"<< std::mktime(&t) << "\n";
		sprintf(time, "%lld", mktime(&t));
		return string(time);
	}
	else
	{
		cout << "Parse failed\n";
		return "";
	}
}
// Return the error message if occurred.
int GetStockPrice(Group& StockList)
{
	struct MemoryStruct data;
	data.memory = NULL;
	data.size = 0;
	//file pointer to create file that store the data  
	FILE *fp;
	//declaration of an object CURL 
	CURL *handle;
	//result of the whole process 
	CURLcode result;
	curl_global_init(CURL_GLOBAL_ALL);//set up the program environment that libcurl needs
	handle = curl_easy_init();//curl_easy_init() returns a CURL easy handle
	if (handle)
	{
		string sCookies, sCrumb;
		if (sCookies.length() == 0 || sCrumb.length() == 0)
		{
			//curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history?p=AMZN");
			curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history");
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
			curl_easy_setopt(handle, CURLOPT_COOKIEJAR, "cookies.txt");
			curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "cookies.txt");

			curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
			curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);

			result = curl_easy_perform(handle);

			if (result != CURLE_OK)
			{
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
				return 1;
			}

			char cKey[] = "CrumbStore\":{\"crumb\":\"";
			char *ptr1 = strstr(data.memory, cKey);
			char *ptr2 = ptr1 + strlen(cKey);
			char *ptr3 = strstr(ptr2, "\"}");
			if (ptr3 != NULL)
				*ptr3 = NULL;
			sCrumb = ptr2;
			fp = fopen("cookies.txt", "r");
			char cCookies[100];
			if (fp) {
				while (fscanf(fp, "%s", cCookies) != EOF);
				fclose(fp);
			}
			else
				cerr << "cookies.txt does not exists" << endl;
			sCookies = cCookies;
			free(data.memory);
			data.memory = NULL;
			data.size = 0;
		}
		map<string, Stock>::iterator itr = StockList.Content.begin();
		while (true)
		{
			if (itr == StockList.Content.end())
				break;

			// Input the start time and end time for certain stock.
			string Tail = "T16:00:00";
			string startTime = getTimeinSeconds(itr->second.GetStartTime() + Tail);
			string endTime = getTimeinSeconds(itr->second.GetEndTime() + Tail);

			string urlA = "https://query1.finance.yahoo.com/v7/finance/download/";
			string symbol = itr->second.GetTicker();
			string urlB = "?period1=";
			string urlC = "&period2=";
			string urlD = "&interval=1d&events=history&crumb=";
			string url = urlA + symbol + urlB + startTime + urlC + endTime + urlD + sCrumb;
			const char * cURL = url.c_str();
			const char * cookies = sCookies.c_str();
			curl_easy_setopt(handle, CURLOPT_COOKIE, cookies);   // Only needed for 1st stock
			curl_easy_setopt(handle, CURLOPT_URL, cURL);
			stringstream Info;
			struct MemoryStruct temp_ptr;
			temp_ptr.memory = NULL;
			temp_ptr.size = 0;
			string temp_line;

			curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
			curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&temp_ptr);
			result = curl_easy_perform(handle);

			if (result != CURLE_OK)
			{
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
				return 1;
			}
			Info.str(temp_ptr.memory);
			getline(Info, temp_line);// Pop out the first line.
			for (int j = 0; j < DayNum + 1; j++)
			{
				getline(Info, temp_line, ',');//Date
				itr->second.Date.push_back(temp_line.c_str());
				getline(Info, temp_line, ',');//Open
				getline(Info, temp_line, ',');//High
				getline(Info, temp_line, ',');//Low
				getline(Info, temp_line, ',');//Close
				getline(Info, temp_line, ',');//Adj close
											  //push back price
				itr->second.Price.push_back(atof(temp_line.c_str()));
				getline(Info, temp_line, '\n');//Volume
			}
			free(temp_ptr.memory);
			temp_ptr.size = 0;
			itr++;
		}

	}
	else
	{
		fprintf(stderr, "Curl init failed!\n");
		return 1;
	}

	curl_easy_cleanup(handle);

	curl_global_cleanup();
	return 0;
}

int GetSPYPrice(Index& Spy)
{

	struct MemoryStruct data;
	data.memory = NULL;
	data.size = 0;
	//file pointer to create file that store the data  
	FILE *fp;
	CURL *handle;
	CURLcode result;
	//curl_global_init(CURL_GLOBAL_ALL);
	handle = curl_easy_init();
	if (handle)
	{
		string sCookies, sCrumb;
		if (sCookies.length() == 0 || sCrumb.length() == 0)
		{
			//curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history?p=AMZN");
			curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history");
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
			curl_easy_setopt(handle, CURLOPT_COOKIEJAR, "cookies.txt");
			curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "cookies.txt");
			curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
			curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);
			result = curl_easy_perform(handle);
			if (result != CURLE_OK)
			{
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
				return 1;
			}
			char cKey[] = "CrumbStore\":{\"crumb\":\"";
			char *ptr1 = strstr(data.memory, cKey);
			char *ptr2 = ptr1 + strlen(cKey);
			char *ptr3 = strstr(ptr2, "\"}");
			if (ptr3 != NULL)
				*ptr3 = NULL;
			sCrumb = ptr2;
			fp = fopen("cookies.txt", "r");
			char cCookies[100];
			if (fp) {
				while (fscanf(fp, "%s", cCookies) != EOF);
				fclose(fp);
			}
			else
				cerr << "cookies.txt does not exists" << endl;
			sCookies = cCookies;
			free(data.memory);
			data.memory = NULL;
			data.size = 0;
		}
		// Input the start time and end time for certain stock.
		string Tail = "T16:00:00";
		string startTime = getTimeinSeconds(Spy.GetStartTime() + Tail);
		string endTime = getTimeinSeconds(Spy.GetEndTime() + Tail);
		string urlA = "https://query1.finance.yahoo.com/v7/finance/download/";
		string symbol = Spy.GetTicker();
		string urlB = "?period1=";
		string urlC = "&period2=";
		string urlD = "&interval=1d&events=history&crumb=";
		string url = urlA + symbol + urlB + startTime + urlC + endTime + urlD + sCrumb;
		const char * cURL = url.c_str();
		const char * cookies = sCookies.c_str();
		curl_easy_setopt(handle, CURLOPT_COOKIE, cookies);   // Only needed for 1st stock
		curl_easy_setopt(handle, CURLOPT_URL, cURL);

		stringstream Info;
		struct MemoryStruct temp_ptr;
		temp_ptr.memory = NULL;
		temp_ptr.size = 0;
		string temp_line;

		curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
		curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&temp_ptr);
		result = curl_easy_perform(handle);

		if (result != CURLE_OK)
		{
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
			return 1;
		}

		Info.str(temp_ptr.memory);

		getline(Info, temp_line);// Pop out the first line.
		for (int j = 0; j < SPYRange; j++)
		{
			getline(Info, temp_line, ',');//Date
			string date = temp_line.c_str(); // date
			getline(Info, temp_line, ',');//Open
			getline(Info, temp_line, ',');//High
			getline(Info, temp_line, ',');//Low
			getline(Info, temp_line, ',');//Close
			getline(Info, temp_line, ',');//Adj close				   
			double price = atof(temp_line.c_str());//price
			getline(Info, temp_line, '\n');//Volume
			Spy.Date.push_back(date);//push back date
			Spy.Price.push_back(price);//push back price

		}
		free(temp_ptr.memory);
		temp_ptr.size = 0;

	}
	else
	{
		fprintf(stderr, "Curl init failed!\n");
		return 1;
	}

	curl_easy_cleanup(handle);

	curl_global_cleanup();
	return 0;
}
