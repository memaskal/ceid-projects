#pragma once

#include "Company.h"

class BinCont {
public:

	BinCont();
	BinCont(int, Company*);

	int getKey() const;
	Company* getVal() const;
	int compare(int) const;
	bool operator() (BinCont*, BinCont*) const;

private:
	int id;
	Company* value;
};

