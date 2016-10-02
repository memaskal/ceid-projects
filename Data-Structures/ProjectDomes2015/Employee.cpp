#include "Employee.h"

/**
*  Constructor
**/
Employee::Employee(string fn, string ln) {
	firstName = fn;
	lastName = ln;
}


/**
*  << Operator overloading
**/
ostream &operator << (ostream &output, const Employee *emp) {

	output << "Name : " + emp->firstName + " " + emp->lastName;
	return output;
}
