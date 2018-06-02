# Earnings Report Impact on Stock Price

ExcelDriver and libcurl needed

Programming Requirements:

	a. Use liburl to retrieve historical price data from Yahoo Finance: A function retrieves
	the adjusted close prices for selected S & P 500 stocks and SPY into memory.   
	b. Create a set of classes such as class for stock to handle EPS estimate and price
	information.  	 
	c. Use member functions or independent functions for all calculation. Overload a few
	arithmetic operators for vector/matrix.  
	d. The stocks and their corresponding price information for each group should be
	stored in a STL map, with stock symbol as its keys.  
	e. The AAR and CAAR for 3 groups are presented in a matrix. The row of the matrix is
	the group#, matrix columns are for AAR and CAAR.   
	f. Use Excel Driver (strongly suggested, but gnuplot could be an alternative for Apple
	computers) to show the CAAR from all 3 groups in one graph.     

• Your program should be able to:  

	a. Retrieve historical price data for all selected stocks. Parse the retrieved data
	b. using STL stack.  
	c. Calculate AAR and CAAR for each group  
	d. Populate the stock maps and AAR/CAAR matrix.  
	e. Show an Excel graph with CAAR for all 3 groups.  
	
• Your program should have a menu of 5 options:  

	a. Retrieve historical price data for all stocks.  
	b. Pull information for one stock from one group.  
	c. Show AAR or CAAR for one group.  
	d. Show the Excel graph with CAAR for all 3 groups.  
	e. Exit your program  
	
Calculation Details:  

	1. From Bloomberg terminal, sort all the stocks from S & P 500 into 3 categories
	according to their 4th quarter 2017 earnings releases into 3 group:
		a. 1st group of stocks beat EPS Estimate, such as 5% to 10% above the estimate.  
		b. 2nd group of stocks meet EPS Estimate, such as 5% to 10% around the estimate.  
		c. 3rd group of stock miss EPS Estimate, such as 5% to 10% below the estimate.  
		
	2. Define as day “zero” for a stock the day the earning is released.
	
	3. Implement Bootstrapping:
	
		a. Sampling the stocks by randomly selecting 60 stocks from each group, total 180 stocks.  
		b. Use libcurl lib to retrieve 121 days of historical prices for each stock and
		SPY around the date of earning release (You could enhance our class
		example for this purpose).  
		c. For each stock calculate the daily returns Rit for 60 days before the day 
		“zero” and 60 days after : t = -60, -59,…-1, 0, 1,…, 59, 60:
		R_it = (Price_t – Price_{t-1})/ Price_{t-1}
		d. Calculate the corresponding daily return Rmt for SPY for the same days.  
		e. Define abnormal returns as the difference AR_it= R_it–R_mt.  
		f. Calculate average daily abnormal returns for each group of stocks for all 120
		reference days.  
		g. Cumulate the returns on the first T days to CAAR.  
		h. Repeat steps a to g 30 times and calculate the average CAAR for 30 samplings  
		
	4. Generate an Excel chart show the averaged CAAR of all three groups, and discuss
	the impact the earning releases on their stock prices. Is there any conclusion you
	could draw from your project?
