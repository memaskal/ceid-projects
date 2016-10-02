#pragma once

#include <string>
#include <iostream>
using std::string;
using std::ostream;

class Employee {
public:
	static int const FNAME_LEN = 56;
	static int const LNAME_LEN = 56;

	Employee(string, string);
	friend ostream &operator<<(ostream &, const Employee*);

	string firstName;
	string lastName;
};

