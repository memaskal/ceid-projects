#pragma once

#include "Testable.h"
#include "Observer.h"
#include "Company.h"

#include <list>
#include <map>
using std::list;
using std::map;

struct TRIECont {
	map<char, TRIECont*> children;
	list<Company*> value;
};

class TRIESearches : public Observer, public Testable {
public:
	TRIESearches(DataHandler*);
	~TRIESearches();

	void import();
	void insert();
	void remove();
	void drop();

	list<Company*> searchName(string);
	
private:
	TRIECont *root;
	
	void deleteTRIE(TRIECont*);
	void insert(const char *, Company*);
	int removeValue(TRIECont*, const char *, Company*);
};

