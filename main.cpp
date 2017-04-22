#include <iostream>
#include <fstream>
#include "rbt.h"
using namespace std;

int main(int argc, char* argv[]) {
	RBTree tree;
	
	//Manual input of numbers
	if (argc == 1) {
		bool done = false;
		while (!done) {
			int input;
			cout << "Enter next number to add (0 to quit): " << flush;
			cin >> input;
			if (input == 0)
				done = true;
			else {
				tree.add(input);
			}
		}
	}
	
	//File input
	else if (argc == 2) {
		ifstream numfile;

    numfile.open(argv[1]);
    if (!numfile.good()) {
      cout << "Useage: " << argv[0] << " <filepath>" << endl;
      return 1;
    }
				
		bool done = false;
		while (!done) {
			int input;
			numfile >> input;
			
			if (input == 0)
				done = true;
			else {
				tree.add(input);
			}
		}
	}
	
	else {
		cout << "Useage: " << argv[0] << " <filepath>" << endl;
    return 1;
	}
	
	//Print tree
	cout << tree.toString() << endl;

	return 0;
}