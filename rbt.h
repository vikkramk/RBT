#ifndef RBT_H
#define RBT_H

#include <string>

#define BLACK false
#define RED true

struct RBTNode {
	RBTNode(int data);
	
	int data;
	bool color;
	RBTNode *left, *right;
};

class RBTree {
	public:
		RBTree();
		void add(int data);
		std::string toString();
	
	private:
		void BSTadd(int data);
		RBTNode* getNode(int data);
		RBTNode* getParent(int data);
		RBTNode* getGrandparent(int data);
		RBTNode* getUncle(int data);
		void rotateRight(int data);
		void rotateLeft(int data);
		
		RBTNode* root;
};

#endif