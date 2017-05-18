#ifndef RBT_H
#define RBT_H

#include <string>

#define BLACK 0
#define RED 1
#define DOUBLE_BLACK 2

struct RBTNode {
	RBTNode(int data);
	RBTNode();
	bool isNULL();
	void setNULL();
	RBTNode* getSibling();
	RBTNode* getGrandparent();
	RBTNode* getUncle();	
	int getColor();
	void setRight(RBTNode* child);
	void setLeft(RBTNode* child);
	
	int data;
	int color;
	RBTNode *parent, *left, *right;
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
		void rotateRight(RBTNode* node);
		void rotateLeft(RBTNode* node);
		void deleteLineNode(RBTNode* node);
		void removalRestructure(RBTNode* node);
		
		RBTNode* root;
};

#endif