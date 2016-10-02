#include "LinearSearches.h"

/**
*  Constructor, attaches objserver to subject
**/
LinearSearches::LinearSearches(DataHandler *data) {
	subject = data;
	//subject->attach(this);
}

/**
*  We are not adding any extra functionality to these
*  methods since we use the original collecection without
*  any changes
**/
void LinearSearches::import() {}
void LinearSearches::insert() {}
void LinearSearches::remove() {}
void LinearSearches::drop() {}


/**
*  Linear Search to entries, by ID
**/
Company* LinearSearches::searchID(int id) {

	Company *result = NULL;
	size_t i, size = subject->companies.size();
	
	for (i = 0; i < size; ++i) {
		if (subject->companies[i]->id == id) {
			result = subject->companies[i];
			break;
		}
	}
	#if TEST_MODE_ON 
		resetCounter("LinSearch");
		inCounter("LinSearch", i);
	#endif;
	return result;
}


/**
*  Linear Search to entries, by employees' surname
**/
list<Company*> LinearSearches::searchName(string surname)  {

	list<Company*> Comp;
	#if TEST_MODE_ON 
		resetCounter("nameSearch");
		int counter = 0;
	#endif;

	vector<Company*>::iterator company = subject->companies.begin();
	for (; company != subject->companies.end(); ++company) {
		vector<Employee*>::iterator employee = (*company)->employees.begin();
		for (; employee != (*company)->employees.end(); ++employee) {
			#if TEST_MODE_ON 
				++counter;
			#endif;
			if ((*employee)->lastName == surname) {
				Comp.push_back(*company);
				break;
			}
		}
	}
	
	#if TEST_MODE_ON 
		inCounter("nameSearch", counter * surname.length());
	#endif;
	return Comp;
}