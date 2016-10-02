#pragma once

#include "DataHandler.h"

class Observer {
public:
	virtual void import() = 0;
	virtual void insert() = 0;
	virtual void remove() = 0;
	virtual void drop() = 0;
protected:
	DataHandler *subject;
};