#ifndef RBT_Hv
#define RBT_H

#include <string>

#define BLACK 0
#define RED 1

struct RBTNode {
	RBTNode(int data);
	
	int data;
	int color;
	RBTNode *left, *right;
};

class RBTree {
	public:
		RBTree();
		void add(int data);
		bool remove(int data);
		bool inTree(int data);
		std::string toString();
	
	private:
		void BSTadd(int data);
		void BSTremove(int data);
		RBTNode* getNode(int data);
		RBTNode* getParent(int data);
		RBTNode* getGrandparent(int data);
		RBTNode* getUncle(int data);
		RBTNode* getSibling(int data);
		void rotateRight(int data);
		void rotateLeft(int data);
		void deleteLineNode(RBTNode* node);
		void removalRestructure(RBTNode* node);
		
		RBTNode* root;
};

#endif