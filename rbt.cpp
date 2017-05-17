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


//Get the parent of a node based on data
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

RBTNode* RBTree::getSibling(int data) {
	RBTNode* p = getParent(data);
	
	if (p) {
		return p->left->data == data ? p->right : p->left;
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


//Remove a node following BST rules
void RBTree::BSTremove(int data) {
	RBTNode* node = getNode(data);
	
	//Nonexistant
	if (!node) {
		return;
	}
	
	//Delete a leaf node
	if (!node->left && !node->right) {
		if (node != root) {
			RBTNode* parent = getParent(data);
			node == parent->left ? parent->left = NULL : parent->right = NULL;
		}
		delete node;
	}
	
	//Delete a node with two children
	if (node->left && node->right) {
		//Find inorder successor (minimum of right subtree)
		RBTNode* parent, *successor = node->right;
		while (successor->left)
			successor = successor->left;
		
		int successordata = successor->data;
		BSTremove(successor->data);
		
		//Replace data with successor
		node->data = successordata;
	}
	
	//Delete a node with 1 child
	RBTNode* child = node->left ? node->left : node->right;
	//Move values to this node and delete child
	node->data = child->data;
	node->right = child->right;
	node->left = child->left;
	delete child;
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

bool RBTree::remove(int data) {
	//Hold the node
	RBTNode* node = getNode(data);
	
	//No such node
	if (!node) {
		return false;
	}
	
	bool nodecolor = node->color;
	
	//Internal node
	if (node->right && node->left) {
		std::cout << "internal node" << endl;
		//Get successor and parent
		RBTNode* successor = node->right, *sparent;
		while (successor->left) successor = successor->left;
		sparent = getParent(successor->data);
		
		//Store the value
		int successorval = successor->data;
		
		//Delete the successor
		deleteLineNode(successor);
		
		//Put value of successor in node
		node->data = successorval;
	}
	
	//Line node
	else {
		deleteLineNode(node);
	}
	
	return true;
}

//Delete a node with one or two children
void RBTree::deleteLineNode(RBTNode* node) {
	std::cout << "line node " << node->data << endl;
	
	//Get child (either the non-null child or null)
	RBTNode* child = node->left ? node->left : node->right;
	
	//Replace node with child (relink through child)
	RBTNode* parent = getParent(node->data);
	if (parent) {
		parent->left == node ? parent->left = child : parent->right = child;
	}
	else {	//Root
		root = child;
	}
	
	//Node is red, nothing to do
	if (node->color == RED) {}
	
	//Node is black, child is red
	else if (node->color == BLACK && child && child->color == RED) {
		std::cout << "just coloring" << endl;
		child->color = BLACK;
	}
	
	//Node to delete is black and child is black
	else {
		if (parent)
			removalRestructure(parent);
		//std::cout << toString() << endl;
		removalRestructure(child);
	}
}

//Restructuring the tree for use in removal of node
void RBTree::removalRestructure(RBTNode* node) {
	std::cout << "restructure" << endl;
	
	bool done = false;
	int step = 1;
	RBTNode* fnode = node, *fparent, *fsibling;	//Nodes in focus
	
	while (!done) {
		fparent = getParent(node->data);
		fsibling = getSibling(node->data);
		
		if (step == 1) {
			//Case 1: it's the root, else go on
			if (fnode == root)
				done == true;
			else
				step = 2;
		}
		else if (step == 2) {
			//Case 2, sibling is red
			if (fsibling && fsibling->color == RED) {
				//Switch colors
				int temp = fsibling->color;
				fsibling->color = fparent->color;
				fparent->color = temp;
				
				//Rotate away from side node is on
				if (node == fparent->left)
					rotateLeft(fparent->data);
				else
					rotateRight(fparent->data);
			}
			
			//Continue
			step = 3;
		}
		else if (step == 3) {
			//Case 3, parent, sibling, and siblings children are black
			if (fparent->color == BLACK && 
			fsibling->color == BLACK && 
			(!fsibling->left || fsibling->left->color == BLACK) &&
			(!fsibling->right || fsibling->left->color == BLACK)) {
				fsibling->color = RED;
				//Restructure from parent starting at step 1
				fnode = fparent;
				step = 1;
			}
			//Or try step 4
			else
				step = 4;
		}
		else if (step == 4) {
			//Case 4, parent is red, sibling and sibling's children black
			if (fparent->color == RED &&
			fsibling->color == BLACK &&
			(!fsibling->left || fsibling->left->color == BLACK) &&
			(!fsibling->right || fsibling->right->color == BLACK)) {
				fsibling->color = RED;
				fparent->color = BLACK;
				done = true;
			}
			//Or try step 5
			else
				step = 5;
		}
		else if (step == 5) {
			//Case 5, sibling is black right child, sibling right black, sibling left red
			if (fsibling->color == BLACK) {
				if ((fnode == fparent->left) &&
				(!fsibling->right || fsibling->right->color == BLACK) &&
				(fsibling->left && fsibling->left->color == RED)) {
					fsibling->color = RED;
					fsibling->left->color = BLACK;
					rotateRight(fsibling->data);
				}
				//Flipped
				else if ((fnode == fparent->right) &&
				(!fsibling->left || fsibling->left->color == BLACK) &&
				(fsibling->right && fsibling->right->color == RED)) {
					fsibling->color = RED;
					fsibling->right->color = BLACK;
					rotateLeft(fsibling->data);
				}
			}
			//Go to step 6
			step = 6;
		}
		else if (step == 6) {
			fsibling->color = fparent->color;
			fparent->color = BLACK;
			
			if (fnode == fparent->left) {
				fsibling->right->color = BLACK;
				rotateLeft(fparent->data);
			}
			else {
				fsibling->left->color = BLACK;
				rotateRight(fparent->data);
			}
		}
	}
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