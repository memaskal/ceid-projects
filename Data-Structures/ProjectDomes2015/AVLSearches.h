#pragma once

#include "DataHandler.h"
#include "Testable.h"
#include "Observer.h"
#include "Company.h"
#include "Node.h"

	
class AVLSearches : public Observer, public Testable {
public:
	AVLSearches(DataHandler*);
	~AVLSearches();

	void import();
	void insert();
	void remove();
	void drop();

	Company* searchID(int);
	void deleteTree(Node*);

	// Inline methods, in order to speed up the operations
	inline void updateHeight(Node*) const;
	inline int heightDiff(Node*) const;
	inline int height(Node*) const;

	Node* rrotation(Node*);
	Node* lrotation(Node*);

	Node* findmin(Node*) const;
	Node* removemin(Node*);

	Node* balance(Node*);
	Node* insert(Node*, Node*);
	Node* remove(Node*, int);

	void display(Node *, int);

private:
	Node *root;
	size_t size;
};

