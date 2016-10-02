#pragma once

#define TEST_MODE_ON 1

#include <map>
#include <string>
using std::string;
using std::map;

class Testable {
public:
	Testable();
	int getCounter(const string);

protected:
	void resetCounter(const string);
	bool inCounter(const string, size_t);

private:
	map<string, size_t> counters;
};