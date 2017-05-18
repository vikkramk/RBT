#include "rbt.h"
#include <cstring>
#include <queue>
#include <sstream>

#include <iostream>

using namespace std;

//RBTNode constructor
RBTNode::RBTNode(int data) {
	this->data = data;
	color = RED;
	parent = left = right = NULL;
	
}

RBTNode::RBTNode() {
	this->setNULL();
	parent = left = right = NULL;
}

//Check for NULL node
bool RBTNode::isNULL() {
	return this->data == 0;
}

//Set the node NULL
void RBTNode::setNULL() {
	this->data = 0;
	this->color = BLACK;
}

//Get node's sibling
RBTNode* RBTNode::getSibling() {
	if (this->parent) {
		if (this->parent->left == this)
			return this->parent->right;
		else
			return this->parent->left;
	}
	else
		return NULL;
}

//Get node's grandparent
RBTNode* RBTNode::getGrandparent() {
	if (this->parent) {
		return this->parent->parent;
	}
	else
		return NULL;
}

//Get node's uncle
RBTNode* RBTNode::getUncle() {
	if (this->parent) {
		return this->parent->getSibling();
	}
	else
		return NULL;
}

//Set right child
void RBTNode::setRight(RBTNode* child) {
	this->right = child;
	child->parent = this;
}

//Set left child
void RBTNode::setLeft(RBTNode* child) {
	this->left = child;
	child->parent = this;
}

//Is this node a left child
bool RBTNode::isLeft() {
	if (this->parent) {
		return this->parent->left == this;
	}
	
	return false;
}

//Is this node a left child
bool RBTNode::isRight() {
	if (this->parent) {
		return this->parent->right == this;
	}
	
	return false;
}

//RBTree constructor
RBTree::RBTree() {
	root = NULL;
}

//Get a node address based on data
RBTNode* RBTree::getNode(int data) {
	std::queue<RBTNode*> todo;
	todo.push(root);
	
	while (todo.size() != 0) {
		RBTNode* node = todo.front();
		todo.pop();
		
		if (node->data == data)
			return node;
		
		else {
			if (node->left != NULL)
				todo.push(node->left);
			if (node->right != NULL)
				todo.push(node->right);
		}
	}
	
	return NULL;
}

void RBTree::rotateRight(RBTNode* node) {
	RBTNode *parent, *leftchild;
	
	if (node == NULL || node->isNULL() || node->left->isNULL())
		return;
	
	parent = node->parent;
	
	leftchild = node->left;
	node->setLeft(leftchild->right);
	leftchild->setRight(node);
	
	if (parent != NULL) {
		if (parent->right == node)
			parent->setRight(leftchild);
		else
			parent->setLeft(leftchild);
	}
	else {
		leftchild->parent = NULL;
		root = leftchild;
	}
}

void RBTree::rotateLeft(RBTNode* node) {
	RBTNode *parent, *rightchild;
	
	if (node == NULL || node->isNULL() || node->right->isNULL())
		return;
	
	std::cout << "left rotating" << endl;
	
	parent = node->parent;
	
	rightchild = node->right;
	node->setRight(rightchild->left);
	rightchild->setLeft(node);
	
	if (parent != NULL) {
		if (parent->right == node)
			parent->setRight(rightchild);
		else
		parent->setLeft(rightchild);
	}
	else {
		rightchild->parent = NULL;
		root = rightchild;
	}
}

//Add a node following BST rules
void RBTree::BSTadd(int data) {
	if (root == NULL) {
		root = new RBTNode(data);
		root->color = RED;
		root->setLeft(new RBTNode());
		root->setRight(new RBTNode());
		return;
	}
	
	if (inTree(data)) {
		return;
	}
	
	bool added = false;
	RBTNode* current = root;
	while (!added) {
		if (data < current->data) {
			if (current->left->isNULL()) {
				current->left->data = data;
				current->left->color = RED;
				current->left->setLeft(new RBTNode());
				current->left->setRight(new RBTNode());
				added = true;
			}
			else
				current = current->left;
		}
		else {
			if (current->right->isNULL()) {
				current->right->data = data;
				current->right->color = RED;
				current->right->setLeft(new RBTNode());
				current->right->setRight(new RBTNode());
				added = true;
			}
			else
				current = current->right;
		}
	}
}

void RBTree::add(int data) {
	RBTNode* node;
	bool inserted = false;
	
	/*Step 1*/
	//Standard BST insertion
	BSTadd(data);
	
	std::cout << toString() << endl;
	
	//Get the node for the new data, set to red
	node = getNode(data);
	node->color = RED;
	
	while (!inserted) {
		RBTNode* parent = node->parent;
	
		std:: cout << "hey" << node->data << endl;
	
		std::cout << toString() << endl;
	
		/*Step 2*/
		//If the node is root, set to black
		if (node == root) {
			node->color = BLACK;
			inserted = true;
		}
		
		/*Step 3*/
		//If parent not black (is red)
		else if (parent->color != BLACK) {
			//Get relatives
			RBTNode* uncle = node->getUncle();
			RBTNode* grandparent = node->getGrandparent();
			
			/*Step 3a*/
			//If uncle is red
			if (uncle && uncle->color == RED) {
				//Set parent and uncle to black
				parent->color = uncle->color = BLACK;
				grandparent->color = RED;
				node = grandparent;
				//Leave to do steps 2 and 3 again with new focus node (the grandparent)
			}
			
			/*Step 3b*/
			//If uncle is black (or null)
			else {
				//Case i (left left)
				if (grandparent->left == parent && parent->left == node) {
					std::cout << "case 1" << endl;
					
					rotateRight(grandparent);
					
					bool temp = grandparent->color;
					grandparent->color = parent->color;
					parent->color = temp;
					
					inserted = true;
				}
				//Case ii (left right)
				else if (grandparent->left == parent && parent->right == node) {
					std::cout << "case 2" << endl;
					rotateLeft(parent);
					
					//Leave to do left left case on next iteration
					node = parent; //Set to old parent
					
					std::cout << "umm" << endl;
				}
				//Case iii (right right)
				else if (grandparent->right == parent && parent->right == node) {
					std::cout << "case 3" << endl;
					rotateLeft(grandparent);
					
					bool temp = grandparent->color;
					grandparent->color = parent->color;
					parent->color = temp;
					
					inserted = true;
				}
				//Case iv (right left)
				else if (grandparent->right == parent && parent->left == node) {
					std::cout << "case 4" << endl;
					rotateRight(parent);
					//Leave to do right right case on next iteration
					node = node->right;
				}
			}
		}
		
		//Otherwise nothing more to do
		else {
			std::cout << "done" << endl;
			inserted = true;
		}
	}
	
}

//Remove a node from the tree
bool RBTree::remove(int data) {
	RBTNode* node = getNode(data);
	
	//No such node
	if (!inTree(data)) {
		return false;
	}
	
	//Step 1, deal with internal nodes
	if (!node->left->isNULL() && !node->right->isNULL()) {
		//Find in-order successor
		RBTNode* successor = node->right;
		while (!successor->left->isNULL())
			successor = successor->left;
		
		//Replace data
		node->data = successor->data;
		
		node = successor;	//Set to remove successor
	}
	
	//Remove node
	//Get the child (the one that is not NULL, or a NULL if both are)
	//Delete the other child
	RBTNode* child;
	if (node->left) {
		child = node->left;
		delete node->right;
	}
	else {
		child = node->right;
		delete node->left;
	}
	
	if (node->parent->left == node)
		node->parent->setLeft(child);
	else
		node->parent->setRight(child);
	
	std::cout << "chopped\n" << toString() << endl;
	
	//If either node or child is red, color the replacing child red
	if (node->color == RED) {
		std::cout << "that was easy" << endl;
		child->color = BLACK;
	}
	else {
		child->color = DOUBLE_BLACK;
	}
		
	//Set the node to start restructuring on
	RBTNode* current = child;
	
	//Eliminate double black
	while (current->color == DOUBLE_BLACK && current != root) {
		RBTNode* sibling = current->getSibling();
		
		cout << toString() << endl;
		cout << sibling->data << sibling->parent->data << sibling->parent->left->data << sibling->parent->right->data << endl;
		
		if (sibling->color == BLACK) {
			if (current->isLeft()) {
				//Case 1: black sibling, red sibling's child
				if (sibling->right->color == RED) {
					rotateLeft(current->parent);
					current->color = BLACK;
				}
				else if (sibling->left->color == RED) {
					rotateRight(sibling);
					rotateLeft(current->parent);
					current->color = BLACK;
				}
				
				//Case 2: black sibling, black sibling's children
				else if (current->parent->color == RED) {
					current->parent->color = BLACK;
					sibling->color = RED;
					current->color = BLACK;
				}
				else {
					sibling->color = RED;
					current->parent->color = DOUBLE_BLACK;
					current->color = BLACK;
					current = current->parent;
				}
			}
			else {
				//Case 1: black sibling, red sibling's child
				if (sibling->left->color == RED) {
					rotateRight(current->parent);
					current->color = BLACK;
				}
				else if (sibling->right->color == RED) {
					rotateLeft(sibling);
					rotateRight(current->parent);
					current->color = BLACK;
				}
				
				//Case 2: black sibling, black sibling's children
				else if (current->parent->color == RED) {
					current->parent->color = BLACK;
					sibling->color = RED;
					current->color = BLACK;
				}
				else {
					sibling->color = RED;
					current->parent->color = DOUBLE_BLACK;
					current->color = BLACK;
					current = current->parent;
				}
			}
		}
		
		//Case 3: sibling is red
		else if (sibling->color == RED) {
			//Rotate to move the sibling up
			if (current->isLeft())
				rotateLeft(current->parent);
			else
				rotateRight(current->parent);
			current->parent->color = RED;
			current->getGrandparent()->color = BLACK;
		}
		
		//If root, make single black
		if (current == root) {
			current->color = BLACK;
		}
	}
	
	delete node;	//Delete the node
	
	return true;	//Success
}

bool RBTree::inTree(int data) {
	return getNode(data) != NULL;
}

std::string RBTree::toString() {
	std::string string;
	
	std::queue<RBTNode*> todo;	//Nodes to look through
	todo.push(root);
	
	bool moreonlevel = true;
	while (moreonlevel) {	//Keep going while more to look through
		int levelCount = todo.size();	//Number of nodes in this level of the tree
		moreonlevel = false;
		while (levelCount > 0) {	//While more nodes in this level
			RBTNode* p = todo.front();	//Take out a node
			todo.pop();
			if (p == NULL || p->isNULL()) {	//Display empty spaces
				string.append("- ");
				todo.push(NULL);	//Null left
				todo.push(NULL);	//Null right
			}
			else {
				moreonlevel = true;
				std::stringstream data;
				data << p->data;
				string.append(data.str() + (p->color == RED ? "R " : "B "));	//Display it
				todo.push(p->left);	//Push left child
				todo.push(p->right);	//Push right child
			}
			levelCount--;	//Decrement number left in level
		}
		string.append("\n");	//Newline to end level
	}
	
	return string;
}