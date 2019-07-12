#include <iostream>
#include <string>
#include <iomanip> //For formatting
#include <fstream> //For File I/O
#include <sstream> //Necessary for string manipulation in runTransaction
using namespace std;

//Global constants representing the price of each item.  Needed for running both reports and transactions
const double su6191Price = 10.99, su6192Price = 12.99, su6193Price = 14.99, taxRate = 0.0825;

//Function prototypes
int promptForMenu(string);
void printReportValuesToScreen(int, int, int, double, double, double, double, double, double, bool);
void extractValuesFromFile(ifstream&, int&, int&, int&);
void calculateValueVariables(double&, double&, double&, int, int, int);
void calculateTotalsAndTaxes(double, double, double, double&, double&, double&);
void runTransaction(int, string);
void runDailyReport(string, string, string);
void runMonthlyReport(string, string);
void runYearlyReport(string);
bool fileExists(string);
void printToFile(string, string, int, int, int);

int main() {
	//Sentinel variable to end while statement when user inputs 0 at menu
	bool endExecution = false;

	//Number of transactions that have been done today.
	int numOfTxs = 0;

	//Variable to hold the user's response when prompted for the report date range.  Used
	//in every switch case except 1 and 0.
	string userResp;

	//Get date in string format
	char currentDate[10];
	_strdate_s(currentDate);
	string date(currentDate);

	//Sets all floats to display with exactly 2 decimal places.  Works for both currency and tax rate
	cout << fixed << setprecision(2) << showpoint;


	while (!endExecution) {
		//This switch uses promptForMenu() to get a integer representing what the user would like to do.  If it is a daily, monthly, or yearly report it generates the file name and
		//passes it to runReport().  If it is a sale then it calls runTransaction() to handle the sale and adds one to the number of sales that have been completed.  Otherwise, it trips the
		//sentential variable to end execution
		switch (promptForMenu(date)) {
		case 1:
			runTransaction(numOfTxs, date);
			numOfTxs++;
			break;
		case 2:
			//Prompts user for the date to run the report and stores it.  Passes runReport the type of report and the generated file name.
			cout << "Enter the date to run the report for.  Format: mm/dd/yy" << endl;
			cin >> userResp;
			runDailyReport(userResp.substr(6, 2), userResp.substr(0, 2), userResp.substr(3, 2));
			break;
		case 3:
			//Prompts user for the month to run the report and stores it.  Passes runReport the type of report and the generated file name.
			cout << "Enter the month to run the report for.  Format: mm/yy" << endl;
			cin >> userResp;
			runMonthlyReport(userResp.substr(3, 2), userResp.substr(0, 2));
			break;
		case 4:
			//Prompts user for the year to run the report and stores it.  Passes runReport the type of report and the generated file name.
			cout << "Enter the year to run the report for.  Format: yy" << endl;
			cin >> userResp;
			runYearlyReport(userResp);
			break;
		case 0:
			//Set sentinel variable to end the while loop and break;
			endExecution = true;
			break;
		}
	}
}

//Displays menu prompt and returns an integer representing user response to be switched off of.
int promptForMenu(string date) {
	//Variable to be returned to main
	int userResponse;

	//Clear the screen before displaying menu.
	system("cls");
	
	//Prints menu prompt
	cout << "SALE SUMMER 2019 PRODUCTS - DANIEL SAPP" << endl << setw(13) << "Today: " << date << endl
		<< "1.\tSale Product" << endl << "2.\tEnding day sale report" << endl << "3.\tEnding month sale report" << endl << "4.\tEnding year sale report"
		<< endl << "0.\tExit" << endl;

	//Stores user input in an int and returns it.
	cin >> userResponse;
	return userResponse;
}

//Handles the full process of inputting a transaction, outputting the receipt, and outputting transaction to "daySale_YYMMDD.txt"
void runTransaction(int numOfTxs, string date) {
	int su6191SoldInTx, su6192SoldInTx, su6193SoldInTx;
	double su6191Value, su6192Value, su6193Value, subTotal, tax, total, amountPaid;

	//Prompt the user for the number of each item sold and store the result in the proper "sold" variable.
	cout << "Enter the number of of SU6191 sold" << endl;
	cin >> su6191SoldInTx;
	cout << "Enter the number of SU6192 sold" << endl;
	cin >> su6192SoldInTx;
	cout << "Enter the number of SU6193 sold" << endl;
	cin >> su6193SoldInTx;
	cout << "Enter the amount the customer paid" << endl;
	cin >> amountPaid;
	

	//Calculate values as itemPrice * itemsSold
	calculateValueVariables(su6191Value, su6192Value, su6193Value, su6191SoldInTx, su6192SoldInTx, su6193SoldInTx);

	//Calculate subTotal as the sum of all Value variables, tax as taxRate * subTotal, and total as subTotal + tax
	calculateTotalsAndTaxes(su6191Value, su6192Value, su6193Value, subTotal, tax, total);

	//Output receipt header
	cout << "........................................................" << endl << "\t    RECEIPT - SALE SU0619 PRODUCT" << endl
		<< "........................................................" << endl << "Date:" << setw(44) << right << date << endl << left << setw(45) << "Sale transaction:" << setfill('0') << setw(4) << right << (numOfTxs + 1)
		<< setfill(' ') << endl;
		
	//Print the data input formatted as a receipt
	printReportValuesToScreen(su6191SoldInTx, su6192SoldInTx, su6193SoldInTx, su6191Value, su6192Value, su6193Value, subTotal, tax, total, true);
	cout << setw(41) << left << "Amount paid:" << setw(8) << right << amountPaid << endl << setw(41) << left << "Balance:" << setw(8) << right << (amountPaid - total) << endl << endl;

	//Keep output on screen until the user presses any key
	system("pause");

	//Generates the Transaction ID as DD#### where #### is numOfTxs+1 with leading zeros to take exactly 4 spaces
	string firstColumn;
	stringstream firstColumnAsStream;
	firstColumnAsStream << date.substr(3, 2) << setw(4) << setfill('0') << right << (numOfTxs + 1);
	firstColumn = firstColumnAsStream.str();

	//Output "DD#### su6191SoldInTx su6192SoldInTx su6193SoldInTx" to "daySale_YYMMDD.txt"
	printToFile("daySale_" + date.substr(6, 2) + date.substr(0, 2) + date.substr(3, 2) + ".txt", firstColumn, su6191Sold, su6192Sold, su6193Sold);
}

//Handles the full process of displaying a end-of-day report and outputting results to "monthSale_MMYY.txt"
void runDailyReport(string year, string month, string day) {
	//Variables to hold the number of each item sold today, and the money made from each of those categoes of sales
	int su6191SoldInDay = 0, su6192SoldInDay = 0, su6193SoldInDay = 0;
	double su6191Value, su6192Value, su6193Value, subTotal, tax, total;

	//Attempt to open the report file
	ifstream inputStream;
	inputStream.open("daySale_" + year + month + day + ".txt");

	//If the daySale file representing the requested day doesn't exist then output an error message, create an output stream on the monthSale file it would fall
	//under, and add a line with "dayNum 0 0 0" representing no sales that day.  Wait for user input before returning to menu.
	if (!inputStream) {
		cout << "The file doesn't exist" << endl;
		printToFile("monthSale_" + year + month + ".txt", day, 0, 0, 0);
		system("pause");
		return;
	}

	//Fill the su####Sold, su####Value, subTotal, tax, and total values based on information extracted from the input stream.
	extractValuesFromFile(inputStream, su6191SoldInDay, su6192SoldInDay, su6193SoldInDay);
	calculateValueVariables(su6191Value, su6192Value, su6193Value, su6191SoldInDay, su6192SoldInDay, su6193SoldInDay);
	calculateTotalsAndTaxes(su6191Value, su6192Value, su6193Value, subTotal, tax, total);

	//stuff to do here
	
	//Prints the header for the report
	cout << "\tSALE SU619 PRODUCTS - " << month << "/" << day << "/" << year << endl;
	
	//Print the data extracted from the file to the screen.
	printReportValuesToScreen(su6191SoldInDay, su6192SoldInDay, su6193SoldInDay, su6191Value, su6192Value, su6193Value, subTotal, tax, total, false);

	//Keep output on screen until the user presses any key
	system("pause");
	
	//Output "DD su6191SoldInDay su6192SoldInDay su6193SoldInDay" to "monthSale_YYMM.txt"
	printToFile("monthSale_" + year + month + ".txt", day, su6191SoldInDay, su6192SoldInDay, su6193SoldInDay);
}

//Handles the full process of displaying a end-of-month report and outputting results to "yearSale_YY.txt"
void runMonthlyReport(string year, string month) {
	//Variables to hold the number of each item sold this month, and the money made from each of those sales
	int su6191SoldInMonth = 0, su6192SoldInMonth = 0, su6193SoldInMonth = 0;
	double su6191Value, su6192Value, su6193Value, subTotal, tax, total;

	//Attempt to open the report file
	ifstream inputStream;
	inputStream.open("monthSale_" + year + month + ".txt");

	//If the monthSale file representing the requested month doesn't exist then output an error message, create an output stream on the yearSale file it would fall
	//under, and add a line with "monthNum 0 0 0" representing no sales that month.  Wait for user input before returning to menu.
	if (!inputStream) {
		cout << "The file doesn't exist" << endl;
		printToFile("yearSale_" + year + ".txt", month, 0, 0, 0);
		system("pause");
		return;
	}

	//Fill the su####Sold, su####Value, subTotal, tax, and total values based on information extracted from the input stream.
	extractValuesFromFile(inputStream, su6191SoldInMonth, su6192SoldInMonth, su6193SoldInMonth);
	calculateValueVariables(su6191Value, su6192Value, su6193Value, su6191SoldInMonth, su6192SoldInMonth, su6193SoldInMonth);
	calculateTotalsAndTaxes(su6191Value, su6192Value, su6193Value, subTotal, tax, total);
	
	//Prints the header for the report
	cout << "\tSALE SU619 PRODUCTS - " << month << "/" << year << endl;
	
	//Print the data extracted from the file to the screen.
	printReportValuesToScreen(su6191SoldInMonth, su6192SoldInMonth, su6193SoldInMonth, su6191Value, su6192Value, su6193Value, subTotal, tax, total, false);

	//Keep output on screen until the user presses any key
	system("pause");

	//TODO: COMMENTME
	printToFile("yearSale_" + year + ".txt", month, su6191SoldInMonth, su6192SoldInMonth, su6193SoldInMonth);
}

//Handles the full process of displaying a end-of-year report
void runYearlyReport(string year) {
	//Variables to hold the number of each item sold, and the money made from each of those sales
	int su6191Sold = 0, su6192Sold = 0, su6193Sold = 0;
	double su6191Value, su6192Value, su6193Value, subTotal, tax, total;

	//Attempt to open the report file
	ifstream inputStream;
	inputStream.open("yearSale_" + year + ".txt");

	if (!inputStream) {
		cout << "The file does not exist" << endl;
		return;
	}

	//Fill the su####Sold, su####Value, subTotal, tax, and total values based on information extracted from the input stream.
	extractValuesFromFile(inputStream, su6191Sold, su6192Sold, su6193Sold);
	calculateValueVariables(su6191Value, su6192Value, su6193Value, su6191Sold, su6192Sold, su6193Sold);
	calculateTotalsAndTaxes(su6191Value, su6192Value, su6193Value, subTotal, tax, total);
	
	//Prints the header for the report
	cout << "\tSALE SU619 PRODUCTS - 20" << year << endl;
	
	//Print the data extracted from the file to the screen.
	printReportValuesToScreen(su6191Sold, su6192Sold, su6193Sold, su6191Value, su6192Value, su6193Value, subTotal, tax, total, false);

	//Keep output on screen until the user presses any key
	system("pause");
}

//Extracts the number of each type of item sold from inputStream and changes su####Sold variables to reflect results
void extractValuesFromFile(ifstream &inputStream, int &su6191Sold, int &su6192Sold, int &su6193Sold) {
	while (true) {
		//Variables to store the number of each item sold in this transaction
		int su6191InTx = 0, su6192InTx = 0, su6193InTx = 0;

		//Ignore the transaction number
		inputStream.ignore(50, ' ');

		//Store the number of each item sold in the appropriate variables
		inputStream >> su6191InTx >> su6192InTx >> su6193InTx;

		//Add the items from this transaction into the running total of items
		su6191Sold += su6191InTx;
		su6192Sold += su6192InTx;
		su6193Sold += su6193InTx;

		//If the next character is EOF then exit loop.  Else, ignore the newline character.
		if (inputStream.peek() == EOF) {
			break;
		}
		else {
			inputStream.ignore(1);
		}
	}
}

//Calculates su####Value as su####Sold * su####Price and changes su####Value appropriately.
void calculateValueVariables(double &su6191Value, double &su6192Value, double &su6193Value, int su6191Sold, int su6192Sold, int su6193Sold) {
	su6191Value = su6191Sold * su6191Price;
	su6192Value = su6192Sold * su6192Price;
	su6193Value = su6193Sold * su6193Price;
}

//Calculates subTotal as su6191Value + su6192Value + su6193Value, tax as subTotal * taxRate, and total as tax + subTotal.  Changes subTotal, tax, and total accordingly.
void calculateTotalsAndTaxes(double su6191Value, double su6192Value, double su6193Value, double &subTotal, double &tax, double &total) {
	//Calculate subTotal as su6191Value + su6192Value + su6193Value
	subTotal = su6191Value + su6192Value + su6193Value;

	//Calculate tax as subTotal * taxRate
	tax = subTotal * taxRate;

	//Calculate total as subTotal + tax
	total = subTotal + tax;
}

//Outputs all values passed into it except for isSale in order.  Prints an extra line at the end before returning if isSale.
void printReportValuesToScreen(int su6191Sold, int su6192Sold, int su6193Sold, double su6191Value, double su6192Value, double su6193Value, double subTotal, double tax, double total, bool isSale) {
	cout << endl << "Model SU6191 (" << su6191Price << setw(12) << left << "/per unit)" << setw(8) << su6191Sold << right << setw(10) << su6191Value << left << endl
		<< "Model SU6192 (" << su6192Price << setw(12) << left << "/per unit)" << setw(8) << su6192Sold << right << setw(10) << su6192Value << left << endl
		<< "Model SU6193 (" << su6193Price << setw(12) << left << "/per unit)" << setw(8) << su6193Sold << right << setw(10) << su6193Value << endl
		<< "........................................................" << endl << "Sub total:" << setw(39)
		<< subTotal << endl << "Tax(" << (taxRate * 100) << "%)" << setw(39) << tax << endl << "Total:" << setw(43) << total << endl;

	if (isSale) {
		cout << endl;
	}
}

//Helper function to see if a file named fileName already exists
bool checkFileExistance(string fileName) {
	//Variable to hold input from stream
	char charFromStream;

	//Declare and open a ifstream on fileName
	ifstream inputStream;
	inputStream.open(fileName.c_str());

	//(Try to) get a char from inputStream
	inputStream.get(charFromStream);

	//If inputStream is still good, meaning the file exists
	if (inputStream) {
		//Put charFromStream back and return true
		inputStream.putback(charFromStream);
		return true;
	}
	//Else return false
	return false;
}

//Helper function that outputs "firstColumn secondColumn thirdColumn fourthColumn" to "fileName"
void printToFile(string fileName, string firstColumn, int secondColumn, int thirdColumn, int fourthColumn) {
	//Variable representing if fileName already exists
	bool fileExists = checkFileExistance(fileName);

	//Declare and open a ofstream on fileName
	ofstream outputStream;
	outputStream.open(fileName.c_str(), ios::app);

	//If fileExists, go to a new line from the previous entry
	if (fileExists) {
		cout << endl;
	}
	//Output "firstColumn secondColumn thirdColumn fourthColumn"
	outputStream << firstColumn << " " << secondColumn << " " << thirdColumn << " " << fourthColumn;
}