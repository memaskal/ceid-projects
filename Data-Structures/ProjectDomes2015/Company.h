#pragma once

#include "Employee.h"

#include <vector>
#include <string>
#include <iostream>

using std::ostream;
using std::vector;
using std::string;
using std::endl;

class Company {
public:
	static int const TITLE_LEN = 256;
	static int const SUMRY_LEN = 2048;

	~Company();

	friend ostream &operator<<(ostream &, const Company*);
	void insertEmployee(Employee*);

	int id;
	string title;
	string summary;
	size_t numberOfEmployees;
	vector<Employee*> employees;
};

