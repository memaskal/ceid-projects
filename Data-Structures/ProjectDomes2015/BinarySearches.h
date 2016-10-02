#pragma once

#include "DataHandler.h"
#include "Observer.h"
#include "Testable.h"
#include "BinCont.h"
#include "Company.h"

#include <vector>
using std::vector;


class BinarySearches : public Observer, public Testable {
public:
	BinarySearches(DataHandler*);
	~BinarySearches();

	void import();
	void insert();
	void remove();
	void drop();

	Company* searchID(int, bool);

private:
	void sortEntries();
	int searchBinary(int);
	int searchInter(int);

	vector<BinCont*> entries;
	bool isSorted;
};

