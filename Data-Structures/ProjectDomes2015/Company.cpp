#include "Company.h"

/**
*  Destructor
**/
Company::~Company() {
	for (size_t i = 0; i < employees.size(); ++i) {
		delete employees[i];
	}
	employees.clear();
}


/**
*  Inserts a new employee to the company
**/
void Company::insertEmployee(Employee *newEmployee) {
	++numberOfEmployees;
	employees.push_back(newEmployee);
}


/**
*  << Operator overloading
**/
ostream &operator<<(ostream &output, const Company *comp) {

	unsigned short subConst = 40;
	string subSum = (comp->summary.length() > subConst) ?
		comp->summary.substr(0, subConst) + "..." :
		comp->summary;

	output << "========================================" << endl;
	output << "ID:       " << comp->id << endl;
	output << "Title:    " << comp->title << endl;
	output << "Summary:  " << subSum << endl;
	output << "Emp/yees: " << comp->numberOfEmployees << endl;

	for (size_t i = 0; i < comp->numberOfEmployees; ++i) {
		output << " #" << (i + 1) << "  " << comp->employees[i] << endl;
	}

	return output;
}
