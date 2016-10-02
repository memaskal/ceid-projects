#pragma once

#include "DataHandler.h"
#include "Observer.h"
#include "Testable.h"
#include "Company.h"

#include <list>
using std::list;

class LinearSearches : public Observer, public Testable {
public:

	LinearSearches(DataHandler*);

	void import();
	void insert();
	void remove();
	void drop();

	Company* searchID(int);
	list<Company*> searchName(string);
};


