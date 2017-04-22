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
	left = right = NULL;
}

//RBTree constructor
RBTree::RBTree() {
	root = NULL;
}

//Get a node address based on value
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

//Add a node following BST rules
void RBTree::BSTadd(int data) {
	if (root == NULL) {
		root = new RBTNode(data);
		return;
	}
	
	if (getNode(data) != NULL) {
		return;
	}
	
	bool added = false;
	RBTNode* current = root;
	while (!added) {
		if (data < current->data) {
			if (current->left == NULL) {
				current->left = new RBTNode(data);
				added = true;
			}
			else
				current = current->left;
		}
		else {
			if (current->right == NULL) {
				current->right = new RBTNode(data);
				added = true;
			}
			else
				current = current->right;
		}
	}
}

//Get the parent of a node based on value
RBTNode* RBTree::getParent(int data) {
	std::queue<RBTNode*> todo;
	todo.push(root);
	
	while (todo.size() != 0) {
		RBTNode* node = todo.front();
		todo.pop();
		
		if (node->left != NULL && node->left->data == data)
			return node;
		if (node->right != NULL && node->right->data == data)
			return node;
		
		if (node->left != NULL)
			todo.push(node->left);
		if (node->right != NULL)
			todo.push(node->right);
	}
	
	return NULL;
}

RBTNode* RBTree::getGrandparent(int data) {
	RBTNode* parent = getParent(data);
	if (parent != NULL)
		return getParent(parent->data);
	return NULL;
}

RBTNode* RBTree::getUncle(int data) {
	RBTNode* gp = getGrandparent(data);
	if (gp != NULL) {
		RBTNode* parent = getParent(data);
		if (gp->left == parent)
			return gp->right;
		if (gp->right == parent)
			return gp->left;
	}
	
	return NULL;
}

void RBTree::rotateRight(int data) {
	RBTNode *node, *parent, *leftchild;

	node = getNode(data);
	
	if (node == NULL|| node->left == NULL)
		return;
	
	leftchild = node->left;
	node->left = leftchild->right;
	leftchild->right = node;
	
	parent = getParent(data);
	
	if (parent != NULL) {
		if (parent->right == node)
			parent->right = leftchild;
		else
			parent->left = leftchild;
	}
	else
		root = leftchild;
}

void RBTree::rotateLeft(int data) {
	RBTNode *node, *parent, *rightchild;

	node = getNode(data);
	
	if (node == NULL|| node->right == NULL)
		return;
	
	rightchild = node->right;
	node->right= rightchild->left;
	rightchild->left = node;
	
	parent = getParent(data);
	
	if (parent != NULL) {
		if (parent->right == node)
			parent->right = rightchild;
		else
			parent->left = rightchild;
	}
	else
		root = rightchild;
}

void RBTree::add(int data) {
	RBTNode* node;
	bool inserted = false;
	
	/*Step 1*/
	//Standard BST insertion
	BSTadd(data);
	
	//Get the node for the new data, set to red
	node = getNode(data);
	node->color = RED;
	
	while (!inserted) {
		RBTNode* parent = getParent(node->data);
	
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
			RBTNode* uncle = getUncle(node->data);
			RBTNode* grandparent = getGrandparent(node->data);
			
			/*Step 3a*/
			//If uncle is red
			if (uncle != NULL && uncle->color == RED) {
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
					
					rotateRight(grandparent->data);
					
					bool temp = grandparent->color;
					grandparent->color = parent->color;
					parent->color = temp;
					
					inserted = true;
				}
				//Case ii (left right)
				else if (grandparent->left == parent && parent->right == node) {
					
					rotateLeft(parent->data);
					//Leave to do left left case on next iteration
					node = parent;
				}
				//Case iii (right right)
				else if (grandparent->right == parent && parent->right == node) {
					
					rotateLeft(grandparent->data);
					
					bool temp = grandparent->color;
					grandparent->color = parent->color;
					parent->color = temp;
					
					inserted = true;
				}
				//Case iv (right left)
				else if (grandparent->right == parent && parent->left == node) {
					
					rotateRight(parent->data);
					//Leave to do right right case on next iteration
					node = parent;
				}
			}
		}
		
		//Otherwise nothing more to do
		else {
			inserted = true;
		}
	}
	
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
			if (p == NULL) {	//Display empty spaces
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