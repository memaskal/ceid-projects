#include <iostream>
#include <iomanip>

#include "Helper.h"
#include "Company.h"
#include "DataHandler.h"
#include "AVLSearches.h"
#include "TRIESearches.h"
#include "LinearSearches.h"
#include "BinarySearches.h"

#include <list>
using namespace std;

// Defining some Globals
int	  ALLOC_FACTOR = 10;
char* DEFAULT_FILE = "small.csv";
char* inputFile = NULL;


// Initializing our objects
DataHandler data = DataHandler();
LinearSearches	linSearch(&data);
BinarySearches	binSearch(&data);
TRIESearches   trieSearch(&data);
AVLSearches		avlSearch(&data);



/**
*  Loads Data from input file
**/
void loadSubMenu() {

	char resp;
	string buf;

	cout << "\n============== Load file ==============\n" << endl;
	if (data.getSize()) {
		cout << "Loading from file will discard all your changes." << endl;
		resp = 'n';
		do {
			cout << "Continue? y/n ";
			cin >> resp;

			// flush input
			cin.clear();
			getline(cin, buf);

		} while (resp != 'y' && resp != 'n');
		if (resp == 'n') {
			return;
		}
	}
	cout << "Loading ..." << endl;
	data.loadFromFile();

	if (data.getFileError() == DataHandler::NOERROR) {
		cout << "Data loaded successfully!" << endl;
	}
	else {
		cout << data.getFileErrorMsg() << endl;
	}
}


/**
*  Saves changes to file
**/
void saveSubMenu() {

	cout << "\n============== Save file ==============\n" << endl;
	data.writeToFile();
	if (data.getFileError() == DataHandler::NOERROR) {
		cout << "Data saved successfully!" << endl;
	}
	else {
		cout << data.getFileErrorMsg() << endl;
	}
}


/**
*  Adds a new company
**/
void addSubMenu() {

	int numEmp;
	char resp;
	string buf, fname, lname;
	Company *newCmp = new Company();

	cout << "\n=========== Insert Company ============\n" << endl;
	newCmp->id = 0;
	do {
		cout << "Please enter an id: ";
		cin >> newCmp->id;

		// flush input
		cin.clear();
		getline(cin, buf);

	} while (newCmp->id <= 0);

	cout << "Please enter a title: ";
	getline(cin, buf);
	newCmp->title = buf.substr(0, Company::TITLE_LEN);

	cout << "Please enter a summary: ";
	getline(cin, buf);
	newCmp->summary = buf.substr(0, Company::SUMRY_LEN);

	numEmp = 0;
	do {
		cout << "Please enter the number of employees: ";
		cin >> numEmp;
	} while (numEmp <= 0);

	cout << "Please enter their name in firstaname lastname format" << endl;
	for (int i = 0; i < numEmp; ++i) {
		cout << "Emp #" << (i + 1) << " ";
		cin >> fname >> lname;
		newCmp->insertEmployee(
			new Employee(fname.substr(0, Employee::FNAME_LEN),
			lname.substr(0, Employee::LNAME_LEN))
		);
	}
	cout << "Press 'n' to discard changes : ";
	cin >> resp;

	if (resp == 'n') {
		delete newCmp;
		cout << "New Company was not saved !!!" << endl;
	}
	else {
		data.insertCompany(newCmp);
		cout << "New Company was added successfuly" << endl;
	}
}


/**
*  Searches and removes companies
**/
void delSubMenu() {

	int searchID, method;
	string buf;

	while (true) {
		method = -1;
		cout << "\n============ Delete By ID ============\n" << endl;
		cout << "1. Delete by id" << endl;
		cout << "2. Exit sub menu\n" << endl;
		cout << "Option : ";
		cin >> method;

		cin.clear();
		getline(cin, buf);

		if (method == 1) {
			do {
				searchID = -1;
				cout << "Please insert an id : ";
				cin >> searchID;

				// Flush the input
				cin.clear();
				getline(cin, buf);

			} while (searchID < 1);

			if (data.removeCompany(searchID)) {
				cout << "The company with id : " << searchID << " deleted successfuly" << endl;
			}
			else {
				cout << "A company with this id could not be found!!!" << endl;
			}
		}
		else if (method == 2) {
			return;
		}
		else {
			cout << "\n\nWrong Input, please try again!\n\n";
		}
	}
}


/**
*  SubMenu for display by Id
**/
void dIDSubMenu() {

	Company* result;
	int searchID, method;
	string buf;

	while (true) {
		method = -1;
		cout << "\n============ Display By ID ============\n" << endl;
		cout << "1. Use linear search" << endl;
		cout << "2. Use binary search" << endl;
		cout << "3. Use binary interpolation search" << endl;
		cout << "4. Use avl trees for search" << endl;
		cout << "5. Exit sub menu\n" << endl;
		cout << "Option : ";
		cin >> method;

		cin.clear();
		getline(cin, buf);

		if (method > 0 && method < 5) {
			do {
				searchID = -1;
				cout << "Please insert an id : ";
				cin >> searchID;

				// Flush the input
				cin.clear();
				getline(cin, buf);

			} while (searchID < 1);

			result = NULL;
			switch (method) {
			case 1:
				result = linSearch.searchID(searchID);
				break;
			case 2:
			case 3:
				result = binSearch.searchID(searchID, (method == 2));
				break;
			case 4:
				result = avlSearch.searchID(searchID);
				break;
			}

			if (result != NULL) {
				cout << endl << result << endl;
			}
			else {
				cout << "No results found!!!" << endl;
			}
		}
		else if (method == 5) {
			return;
		}
		else {
			cout << "\n\nWrong Input, please try again!\n\n";
		}
	}
}


/**
*  Displaying all companies
**/
void dispSubMenu() {

	string buf;
	char resp;

	int size = data.getSize();
	if (size == 0) {
		cout << "There are not any companies to show !!!" << endl;
	}

	int pagination = 2;
	for (int i = 0; i < size; ++i) {

		cout << endl << data.companies[i]<< endl;

		if ((i + 1) % pagination == 0 && (i + 1) < size) {
			cout << "Type any char to continue or 'n' to stop ";
			cin >> resp;

			getline(cin, buf);
			cin.clear();

			if (resp == 'n') {
				break;
			}
		}
	}
}


/**
*  Submenu for display by surname search
**/
void SurSubMenu() {

	int method;
	list<Company*> results;
	string searchName, buf;

	while (true) {
		method = -1;
		cout << "\n========== Display By Surname ==========\n" << endl;
		cout << "1. Use linear search" << endl;
		cout << "2. Use Digital Tries" << endl;
		cout << "3. Exit sub menu\n" << endl;
		cout << "Option : ";
		cin >> method;

		cin.clear();
		getline(cin, buf);

		if (method > 0 && method < 3) {
			do {
				cout << "Please insert an surname: ";
				cin >> searchName;

				// Flush the input
				cin.clear();
				getline(cin, buf);

			} while (!searchName.length());

			switch (method) {
			case 1:
				results = linSearch.searchName(searchName);
				break;
			case 2:
				results = trieSearch.searchName(searchName);
				break;
			}

			size_t size = results.size();
			if (size) {

				char resp;
				size_t counter = 1;
				int pagination = 2;
				for (list<Company*>::iterator i = results.begin(); i != results.end(); ++i) {
					cout << (*i) << endl;

					if (counter % pagination == 0 && counter < size) {
						cout << "Type any char to continue or 'n' to stop ";
						cin >> resp;

						getline(cin, buf);
						cin.clear();

						if (resp == 'n') {
							break;
						}
					}
					++counter;
				}
			}
			else {
				cout << "No results found!!!" << endl;
			}
		}
		else if (method == 3) {
			return;
		}
		else {
			cout << "\n\nWrong Input, please try again!\n\n";
		}
	}
}


/**
*  Main Menu displaying all available functionality
**/
void mainMenu() {

	string buf;
	int option;

	while (true) {
		cout << "============== Main Menu ==============\n" << endl;
		cout << "1. Load companies from file" << endl;
		cout << "2. Save companies to file" << endl;
		cout << "3. Add a company" << endl;
		cout << "4. Delete a company by id" << endl;
		cout << "5. Display a company by id" << endl;
		cout << "6. Display companies" << endl;
		cout << "7. Display companies by surname search" << endl;
		cout << "8. Exit app\n" << endl;
		cout << "Option : ";
		cin >> option;

		// Flush the input
		cin.clear();
		getline(cin, buf);

		switch (option) {
		case 1:
			loadSubMenu();
			break;
		case 2:
			saveSubMenu();
			break;
		case 3:
			addSubMenu();
			break;
		case 4:
			delSubMenu();
			break;
		case 5:
			dIDSubMenu();
			break;
		case 6:
			dispSubMenu();
			break;
		case 7:
			SurSubMenu();
			break;
		case 8:
			return;
		default:
			cout << "\n\nWrong Input, please try again!";
		}
		cout << "\n";
	}
}


#if TEST_MODE_ON
/**
*  Runs tests on the search algorithms
**/
void tester() {

	size_t compSize = data.getSize();
	if (compSize == 0) {
		cout << "No input supplied returning" << endl;
		return;
	}

	int maxID = 10000, testsizeIDs = 1000, testsizeNames = 10, repeats = 10;
	int counter, method, times;
	char *algorithm[] = { "Digital Tries", "Linear Search", "Binary Search", "Interpolation Search", "AVL Search" };

	list<int> searchIds = list<int>();
	list<string> searchNames = list<string>();


	// Populate the search list
	Helper::seedRand();
	for (int i = 0; i < testsizeIDs; ++i) {
		searchIds.push_back(Helper::getRandNum(0, maxID));
	}
	for (int i = 0; i < testsizeNames; ++i) {
		Company *randComp = data.companies[Helper::getRandNum(0, compSize)];
		searchNames.push_back(randComp->employees[
			Helper::getRandNum(0, randComp->numberOfEmployees)
		]->lastName);
	}

	// Id based searches
	cout.precision(10);
	cout << "ID Based Searches:" << endl;
	for (method = 1; method < 5; ++method) {
		counter = 0;
		Helper::startTimer();
		for (times = 0; times < repeats; ++times) {
			list<int>::iterator id = searchIds.begin();
			for (; id != searchIds.end(); ++id) {
				switch (method) {
				case 1:
					linSearch.searchID(*id);
					counter += linSearch.getCounter("LinSearch");
					break;
				case 2:
				case 3:
					binSearch.searchID(*id, (method == 2));
					counter += binSearch.getCounter(((method == 2) ? "BinSearch" : "IntSearch"));
					break;
				case 4:
					avlSearch.searchID(*id);
					counter += avlSearch.getCounter("AvlSearch");
					break;
				}
			}
		}
		cout << algorithm[method] << " counter : " << counter / repeats << endl;
		cout << Helper::stopTimer() / repeats << " ms" << endl;
	}

	// Name based Searches
	printf("\nSurname Based Searches\n");
	for (method = 0; method < 2; ++method) {
		counter = 0;
		Helper::startTimer();
		for (times = 0; times < repeats; ++times) {
			list<string>::iterator name = searchNames.begin();
			for (; name != searchNames.end(); ++name) {
				switch (method) {
				case 0:
					trieSearch.searchName(*name);
					counter += trieSearch.getCounter("nameSearch");
					break;
				case 1:
					linSearch.searchName(*name);
					counter += linSearch.getCounter("nameSearch");
					break;
				}
			}
		}
		cout << algorithm[method] << " counter : " << counter / repeats << endl;
		cout << Helper::stopTimer() / repeats << " ms" << endl;
	}
}
#endif;


int main(int argc, char **argv) {

	inputFile = (argc > 1) ? argv[1] : DEFAULT_FILE;
	data.setFilename(inputFile);

	// Calling the main menu
	mainMenu();
	
	// Runing all the tests after the termination
	#if TEST_MODE_ON
		cout << endl << "Runing all the tests" << endl;
		tester();
		cout << "Finished";
	#endif;
	
	getchar();
	return 0;
}

