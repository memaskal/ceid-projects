#include "BinarySearches.h"

#include <algorithm>
#include <cmath>

/**
*  Constructor, attaches objserver to subject
**/
BinarySearches::BinarySearches(DataHandler *data) {
	subject = data;
	subject->attach(this);
	isSorted = true;
}


/**
*  Make sure to dealocate heap memory
**/
BinarySearches::~BinarySearches() {
	drop();
}


/**
*  Insert the new entrie to our table
**/
void BinarySearches::insert() {
	isSorted = false;
	int index = subject->getInsIndex();
	entries.push_back(new BinCont(subject->companies[index]->id, subject->companies[index]));
}


/**
*  Sorts all the entries, if needed
**/
void BinarySearches::sortEntries() {

	if (!isSorted) {
		sort(entries.begin(), entries.end(), BinCont());
		isSorted = true;
	}
}


/**
*  Removes element using binary search
**/
void BinarySearches::remove() {
	
	int deleteIndex = searchBinary(subject->companies[subject->getDelIndex()]->id);
	if (deleteIndex >= 0) {
		delete entries[deleteIndex];
		entries.erase(entries.begin() + deleteIndex);
	}
}


/**
*  Imports a bulk of data to the structure
**/
void BinarySearches::import() {
	isSorted = false;
	entries.reserve(subject->getCapacity());
	size_t size = subject->getSize();
	for (size_t i = 0; i < size; ++i) {
		entries.push_back(new BinCont(subject->companies[i]->id, subject->companies[i]));
	}
}


/**
*  Delete all entries
**/
void BinarySearches::drop() {
	isSorted = true;
	size_t size = entries.size();
	for (size_t i = 0; i < size; ++i) {
		delete entries[i];
	}
	entries.clear();
}


/**
*  Calls the appropriate search algorithm
**/
Company* BinarySearches::searchID(int id, bool isBin) {
	int index = ((isBin) ? searchBinary(id) : searchInter(id));
	if (index >= 0) {
		return entries[index]->getVal();
	}
	return NULL;
}


/**
*  Binary Search to entries, by ID
**/
int BinarySearches::searchBinary(int id) {

	size_t size = entries.size();
	int result = -1;

	#if TEST_MODE_ON 
		int counter = 0;
		resetCounter("BinSearch");
	#endif

	if (size > 0) {

		// Sort entries if needed
		sortEntries();

		int next, left = 0 , right = size - 1;
		int comparison;

		while (left <= right) {
			next = (left + right) / 2;
			comparison = entries[next]->compare(id);

			#if TEST_MODE_ON 
				++counter;
			#endif;

			if (comparison > 0) {
				right = next - 1;
			}
			else if (comparison < 0) {
				left = next + 1;
			}
			else {
				result = next;
				break;
			}
		}
	}
	#if TEST_MODE_ON 
		inCounter("BinSearch", counter);
	#endif
	return result;
}


/**
*  Interpolation Search to entries, by ID
**/
int BinarySearches::searchInter(int id) {

	int size = entries.size();
	int result = -1;

	#if TEST_MODE_ON 
		int counter = 0;
		resetCounter("IntSearch");
	#endif
	
	if (size > 0) {
		// Sort entries if needed
		sortEntries();

		int next, i, j, comparison, sqrs;
		int left = 0, right = size - 1;
		int maxIndex = right;

		if (entries[right]->compare(id) < 0 || entries[left]->compare(id) > 0)  {
			#if TEST_MODE_ON 
				inCounter("IntSearch", 2);
			#endif
			return -1;
		}

		do {
			// Linear search in a small sized input
			if (size < 4) {
				for (i = left; i <= right; ++i) {
					if (entries[i]->compare(id) == 0) {
						result = i;
						break;
					}
				}
				#if TEST_MODE_ON 
					inCounter("IntSearch", counter + i - left + 1);
				#endif
				return result;
			}

			// Interpolation step, keys unique no divide by zero check
			next = left + (int)ceil(size*((float)id - entries[left]->getKey()) / (entries[right]->getKey() - entries[left]->getKey()));
			if (next < 0) {
				next = 0;
			}
			else if (next > maxIndex) {
				next = maxIndex;
			}

			i = -1;			
			// perform sqrt(size) steps
			sqrs = (int) sqrt(size);
			comparison = entries[next]->compare(id);
			if (comparison < 0) {

				if (next < 1) ++i;
				do { 
					++i;
					j = next + i * sqrs - 1;
				} while (j <= maxIndex && entries[j]->compare(id) < 0);

				right = (j + 1 > maxIndex) ? maxIndex : j + 1;
				left = right - sqrs;
			
			} else if (comparison > 0) {

				if (next == maxIndex) ++i;
				do {
					++i;
					j = next - i * sqrs + 1;
				} while (j >= 0 && entries[j]->compare(id) > 0);

				left = (j < 1) ? 0 : j - 1;
				right = left + sqrs;
			}
			else {
				result = next;
			}

			#if TEST_MODE_ON 
				counter += i + 1;
			#endif
			size = right - left + 1;

		} while ( result == -1);
	} 
	#if TEST_MODE_ON 
		inCounter("IntSearch", counter);
	#endif
	return result;
}