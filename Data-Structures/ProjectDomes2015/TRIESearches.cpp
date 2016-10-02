#include "TRIESearches.h"

/**
*  Constructor, attaches objserver to subject
**/
TRIESearches::TRIESearches(DataHandler *data) {
	subject = data;
	subject->attach(this);
	root = new TRIECont();
}


/**
*  Destructor, deletes the trie and de-allocate
*  the heap memory
**/
TRIESearches::~TRIESearches() {
	deleteTRIE(root);
	delete root;
}


/**
*  Inserts the new names to the trie struct
**/
void TRIESearches::insert() {
	int index = subject->getInsIndex();
	vector<Employee*> employees = subject->companies[index]->employees;
	for (size_t i = 0; i < employees.size(); ++i) {
		insert(employees[i]->lastName.c_str(), subject->companies[index]);
	}
}


/**
*  Removes the deleted employees from the trie
**/
void TRIESearches::remove() {
	int index = subject->getDelIndex();
	vector<Employee*> employees = subject->companies[index]->employees;
	for (size_t i = 0; i < employees.size(); ++i) {
		removeValue(root, employees[i]->lastName.c_str(), subject->companies[index]);
	}
}


/**
*  No need to implement capacity for the trie
**/
void TRIESearches::import() {
	size_t size = subject->getSize();
	for (size_t i = 0; i < size; ++i) {
		vector<Employee*> employees = subject->companies[i]->employees;
		size_t empSize = employees.size();
		for (size_t j = 0; j < empSize; ++j) {
			insert(employees[j]->lastName.c_str(), subject->companies[i]);
		}
	}	
}


/**
*  Deletes all the entries in the trie
**/
void TRIESearches::drop() {
	deleteTRIE(root);

	root->children.clear();
	root->value.clear();
}


/**
*  Given a node in the trie deletes all of its
*  contents
**/
void TRIESearches::deleteTRIE(TRIECont *currNode) {
	map<char, TRIECont*>::iterator it;
	for (it = currNode->children.begin(); it != currNode->children.end(); ++it) {
		deleteTRIE(it->second);
		delete it->second;
	}
}


/**
*  Search the trie for all the vallues the input string
**/
list<Company*> TRIESearches::searchName(string name) {

	TRIECont *currLevel = root;
	list<Company*> result;

	map<char, TRIECont*>::iterator found;
	const char *input = name.c_str();	

	#if TEST_MODE_ON 
		resetCounter("nameSearch");
		int counter = 1;
	#endif;

	if (input != '\0') {
		while (*input != '\0') {
			#if TEST_MODE_ON 
				++counter;
			#endif;

			found = currLevel->children.find(*input);
			if (found == currLevel->children.end()) {
				break;
			}
			else {
				currLevel = found->second;
			}
			++input;
		}
		result = currLevel->value;
	}

	#if TEST_MODE_ON 
		inCounter("nameSearch", counter);
	#endif;
	return result;
}


/**
*  Inserts a new value to the trie given an input string
**/
void TRIESearches::insert(const char *input, Company* val) {

	if (*input == NULL || *input == '\0') {
		return;
	}

	TRIECont *currLevel = root;
	map<char, TRIECont*>::iterator found;

	while (*input != '\0') {

		found = currLevel->children.find(*input);
		if (found == currLevel->children.end()) {
			currLevel = currLevel->children[*input] = new TRIECont();
		}
		else {
			currLevel = found->second;
		}
		++input;
	}
	currLevel->value.push_back(val);
}


/**
*  Deletes value from trie
**/
int TRIESearches::removeValue(TRIECont* curNode, const char *input, Company* val) {

	if (*input == '\0' || input == NULL) {
		return 0;
	}

	map<char, TRIECont*>::iterator found = curNode->children.find(*input);
	if (found != curNode->children.end()) {

		int result = removeValue(found->second, ++input, val);
		size_t valSize = found->second->value.size();
		size_t childSize = found->second->children.size();

		// No action needed, return
		if (result == -1 || result == 1 && valSize) {
			return -1;
		}				

		// The node has children or multiple values, remove only our value
		if (childSize || valSize > 1) {
			if (result == 0) {
				found->second->value.remove(val);
			}
			return -1;
		}

		// Delete the nodes as well
		delete found->second;
		curNode->children.erase(found);
		return 1;
	}
	return -1;
}
