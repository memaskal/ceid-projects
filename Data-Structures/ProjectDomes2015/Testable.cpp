#include "Testable.h"

/**
*  Constructor
**/
Testable::Testable() {
	counters = map<string, size_t>();
}


/**
*  Searches for a counter and increment it by value
**/
bool Testable::inCounter(const string name, size_t values) {
	map<string, size_t>::iterator found = counters.find(name);
	if (found != counters.end()) {
		found->second += values;
		return true;
	}
	return false;
}


/**
*  Adds a new counter
**/
void Testable::resetCounter(const string name) {
	counters[name] = 0;
}


/**
*  Returns the value of a counter if it exists
**/
int Testable::getCounter(const string name) {
	map<string, size_t>::iterator found = counters.find(name);
	if (found != counters.end()) {
		return found->second;
	}
	return 0;
}
