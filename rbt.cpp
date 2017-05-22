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
	
	if (root == NULL)
		return NULL;
	
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
	if (root == NULL || root->isNULL()) {
		if (root) {
			delete root;
		}
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
	
	//Get the node for the new data, set to red
	node = getNode(data);
	node->color = RED;
	
	while (!inserted) {
		RBTNode* parent = node->parent;
		
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
					
					rotateRight(grandparent);
					
					bool temp = grandparent->color;
					grandparent->color = parent->color;
					parent->color = temp;
					
					inserted = true;
				}
				//Case ii (left right)
				else if (grandparent->left == parent && parent->right == node) {
					rotateLeft(parent);
					
					//Leave to do left left case on next iteration
					node = parent; //Set to old parent
					
				}
				//Case iii (right right)
				else if (grandparent->right == parent && parent->right == node) {
					rotateLeft(grandparent);
					
					bool temp = grandparent->color;
					grandparent->color = parent->color;
					parent->color = temp;
					
					inserted = true;
				}
				//Case iv (right left)
				else if (grandparent->right == parent && parent->left == node) {
					rotateRight(parent);
					//Leave to do right right case on next iteration
					node = node->right;
				}
			}
		}
		
		//Otherwise nothing more to do
		else {
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
	if (!node->left->isNULL()) {
		child = node->left;
		delete node->right;
	}
	else {
		child = node->right;
		delete node->left;
	}
	
	if (node == root) {
		root = child;
	}
	else {
		if (node->isLeft())
			node->parent->setLeft(child);
		else
			node->parent->setRight(child);
	}
	
	//If the removed node was black
	if (node->color == BLACK ) {
		//Just recolor if the replacing child is red
		if (child->color == RED) {
			child->color = BLACK;
		}
		//Else run the cases
		else {
			//Set the node to start restructuring on
			RBTNode* current = child;
			bool done = false;
			int step = 1;	//Start on step 1
			
			//Run steps until done
			while (!done) {
				RBTNode* sibling = current->getSibling();
				RBTNode* parent = current->parent;
			
				//Step 1
				if (step == 1) {
					//Done if reached root
					if (current == root)
						done = true;
					//Else go on
					else
						step = 2;
				}
				//Step 2
				else if (step == 2) {
					//Sibling red
					if (sibling->color == RED) {
						parent->color = RED;
						sibling->color = BLACK;
						if (current->isLeft())
							rotateLeft(parent);
						else
							rotateRight(parent);
					}
					//Go on
					step = 3;
				}
				else if (step == 3) {
					//Parent, sibling, and sibling's children black
					if ((parent->color == BLACK) &&
						(sibling->color == BLACK) &&
						(sibling->left->color == BLACK) &&
						(sibling->right->color == BLACK)) {
						sibling->color = RED;
						//Go back to start on parent
						current = parent;
						step = 1;
					}
					//Else go on
					else
						step = 4;
				}
				else if (step == 4) {
					//Parent red, sibling and sibling's children black
					if ((parent->color == RED) &&
						(sibling->color == BLACK) &&
						(sibling->left->color == BLACK) &&
						(sibling->right->color == BLACK)) {
						sibling->color = RED;
						parent->color = BLACK;
						done = true;
					}
					//Else go on
					else
						step = 5;
				}
				else if (step == 5) {
					//Sibling black with one black and one red child
					if (sibling->color == BLACK) {
						if (current->isLeft() &&
							sibling->right->color == BLACK &&
							sibling->left->color == RED) {
								sibling->color = RED;
								sibling->left->color = BLACK;
								rotateRight(sibling);
						}
						else if (current->isRight() &&
							sibling->left->color == BLACK &&
							sibling->right->color == RED) {
								sibling->color = RED;
								sibling->right->color = BLACK;
								rotateLeft(sibling);
						}
					}
					//Go on
					step = 6;
				}
				else if (step == 6) {
					//Flip sibling and parent colors
					sibling->color = parent->color;
					parent->color = BLACK;
					
					//Rotate and recolor
					if (current->isLeft()) {
						sibling->right->color = BLACK;
						rotateLeft(parent);
					}
					else {
						sibling->left->color = BLACK;
						rotateRight(parent);
					}
					
					done = true;
				}
			}
		}
	}
	
	//Remove the node
	delete node;
	
	return true;
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