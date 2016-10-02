#include "AVLSearches.h"
#define max(a,b) (((a)>(b))?(a):(b))

/**
*  Constructor, attaches objserver to subject
**/
AVLSearches::AVLSearches(DataHandler *data) {
	subject = data;
	subject->attach(this);
	size = 0;
	root = NULL;
}


/**
*  Destroys the structure
**/
AVLSearches::~AVLSearches() {
	deleteTree(root);
}


/**
*  Deletes all the AVL structure
**/
void AVLSearches::drop() {
	deleteTree(root);
	root = NULL;
}


/**
*  Visits every node in the tree and deletes
*  them recursively
**/
void AVLSearches::deleteTree(Node *node) {
	if (node != NULL) {
		deleteTree(node->left);
		deleteTree(node->right);
		delete node;
	}
}


/**
*  Need to be implemented 
**/
void AVLSearches::import() {
	size_t size = subject->getSize();
	for (size_t i = 0; i < size; ++i) {
		root = insert(root, new Node(subject->companies[i]->id, subject->companies[i]));
	}
	this->size = size;
}


/**
*  Inserts new node to the AVL tree
**/
void AVLSearches::insert() {
	int index = subject->getInsIndex();
	root = insert(root, new Node(subject->companies[index]->id, subject->companies[index]));
	++size;
}


/**
*  Removes node from AVL
**/
void AVLSearches::remove() {

	if (size == 0) {
		return;
	}
	int index = subject->getDelIndex();
	root = remove(root, subject->companies[index]->id);
	--size;
}


/**
*  Binary search using AVL Trees
**/
Company* AVLSearches::searchID(int id) {

	Node *currNode = root;
	Company *result = NULL;

	#if TEST_MODE_ON 
		int counter = 0;
		resetCounter("AvlSearch");
	#endif

	while (currNode != NULL) {

		#if TEST_MODE_ON 
			++counter;
		#endif;

		int comparison = currNode->compare(id);
		if (comparison < 0) {
			currNode = currNode->right;
		}
		else if (comparison > 0) {
			currNode = currNode->left;
		}
		else {
			result = currNode->getVal();
			break;
		}
	}
	#if TEST_MODE_ON 
		inCounter("AvlSearch", counter);
	#endif
	return result;
}


/**
*  Returns the height of a node
**/
inline int AVLSearches::height(Node* node) const {
	if (node != NULL) {
		return node->height;
	}
	return 0;
}


/**
*  Calculates the height Difference of a subtree
**/
inline int AVLSearches::heightDiff(Node *node) const {
	return height(node->right) - height(node->left);
}


/**
*  Updates the height property of the node
**/
inline void AVLSearches::updateHeight(Node* node) const {
	node->height = 1 + max(height(node->left), height(node->right));
}


/**
*  Performs a right rotation arround critical 
*  point node
**/
Node* AVLSearches::rrotation(Node *node) {
	Node *temp = node->left;
	node->left = temp->right;
	temp->right = node;
	updateHeight(node);
	updateHeight(temp);
	return temp;
}


/**
*  Performs a left rotation arround critical
*  point node
**/
Node* AVLSearches::lrotation(Node* node) {
	Node *temp = node->right;
	node->right = temp->left;
	temp->left = node;
	updateHeight(node);
	updateHeight(temp);
	return temp;
}


/**
*  Balancing AVL tree
**/
Node *AVLSearches::balance(Node *node) {

	updateHeight(node);
	int bal_factor = heightDiff(node);
	if (bal_factor > 1) {
		if (heightDiff(node->right) < 0) {
			node->right = rrotation(node->right);
		}
		return lrotation(node);
	}
	else if (bal_factor < -1) {
		if (heightDiff(node->left) > 0) {
			node->left = lrotation(node->left);
		}
		return rrotation(node);
	}
	return node;
}


/**
*  Insert Node to the AVL tree
**/
Node *AVLSearches::insert(Node *node, Node *newNode) {

	if (node == NULL) {
		return newNode;
	}
	else if (node->compare(*newNode)) {
		node->left = insert(node->left, newNode);
	}
	else {
		node->right = insert(node->right, newNode);
	}
	return balance(node);
}


/**
*  Removes ndoe from AVL tree
**/
Node *AVLSearches::remove(Node *node, int key) {

	if (node == NULL) {
		return NULL;
	}

	int comparison = node->compare(key);
	if (comparison > 0) {
		node->left = remove(node->left, key);
	}
	else if (comparison < 0) {
		node->right = remove(node->right, key);
	}
	else {
		Node* lnode = node->left;
		Node* rnode = node->right;
		delete node;

		if (rnode == NULL) {
			return lnode;
		}

		Node *min = findmin(rnode);
		min->right = removemin(rnode);
		min->left = lnode;

		return balance(min);
	}
	return balance(node);
}


/**
*  Finds the minimum node in the tree
**/
Node* AVLSearches::findmin(Node* node) const {
	Node *currNode = node;
	while (currNode->left != NULL) {
		currNode = currNode->left;
	}
	return currNode;
}


/**
*  Removes the min node of the tree
**/
Node* AVLSearches::removemin(Node *node) {
	if (node->left == NULL) {
		return node->right;
	}
	node->left = removemin(node->left);
	return balance(node);
}