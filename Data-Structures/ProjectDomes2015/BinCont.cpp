#include "BinCont.h"

/**
*  Default constructor
**/
BinCont::BinCont() {}

/**
*  Cponstructs the container
**/
BinCont::BinCont(int id, Company* value) {
	this->id = id;
	this->value = value;
}


/**
*  Returns containers key val
**/
int BinCont::getKey() const {
	return id;
}


/**
*  Returns containers data val
**/
Company* BinCont::getVal() const {
	return value;
}


/**
*  Operator overloading, performing comparison for 
*  sorting with algorithm's sort
**/
bool BinCont::operator() (BinCont *c1, BinCont *c2) const {
	return c1->getKey() < c2->getKey();
}


/**
*  Compares containers value with an id
**/
int BinCont::compare(int key) const {
	return id - key;
}