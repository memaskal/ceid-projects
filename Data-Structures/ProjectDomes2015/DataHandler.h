#pragma once

#include "Company.h"

#include <vector>
using std::vector;

class Observer;

class DataHandler {
public:
	enum Mode {
		INSERT, REMOVE, DROP, IMPORT
	};

	enum FileError {
		WRITE, READ, NOERROR
	};

	DataHandler();
	~DataHandler();

	// Basic getters
	size_t getSize() const;
	size_t getCapacity() const;
	size_t getInsIndex() const;
	size_t getDelIndex() const;
	bool isUpadated() const;

	// Subject's methods
	void attach(Observer*);
	void loadFromFile();
	void writeToFile();

	// Subject's methods triggering observers
	void insertCompany(Company*);
	bool removeCompany(int);
	void dropAllEntries();

	// Other Methods
	void setCapacity(int);
	void setFilename(char*);
	void insertOnly(Company *);
	void setFileError(FileError);
	string getFileErrorMsg() const;
	int getFileError() const;

	// Companies data
	vector <Company*> companies;

private:
	void notifyAllObservers(Mode) const;
	vector <Observer*> observers;

	FileError fileErrorCode;
	size_t insertedIndex;
	size_t deletedIndex;
	bool dataUpdated;
	char *file;
};
