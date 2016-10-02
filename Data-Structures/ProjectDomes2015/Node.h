#pragma once

#include "Company.h"

class Node {
public:

	Node(int, Company*);

	Company* getVal() const;
	int getKey() const;	
	int compare(int) const;
	bool compare(Node) const;

	int height;
	Node *left;
	Node *right;

private:
	int id;
	Company* value;
};

