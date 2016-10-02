#include "Node.h"

/**
*  Constructor
**/
Node::Node(int id, Company* value) {
	this->id = id;
	this->value = value;

	left = right = NULL;
	height = 1;
}


/**
*  Returns the Node's key
**/
int Node::getKey() const {
	return id;
}


/**
*  Returns the Node's Value
**/
Company* Node::getVal() const {
	return value;
}


/**
*  Checks if node's key is greater 
*  than the argument's node key value
**/
bool Node::compare(Node node) const {
	return id > node.getKey();
}


/**
*  Compares the value of the Node's key with
*  the argument Node key value
**/
int Node::compare(int key) const {
	return id - key;
}

