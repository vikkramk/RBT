#include <iostream>
#include <fstream>
#include <cstdlib>
#include "rbt.h"
using namespace std;

int main() {
	RBTree tree;
	string command;
	
	bool done = false;
	while (!done) {
		cout << "ENTER ADD, REMOVE, SEARCH, ADDFILE, PRINT, OR QUIT: " << flush;
		cin >> command;
		
		if (command == "ADD") {
			int num;
			cout << "ENTER NUMBER: " << flush;
			cin >> num;
			tree.add(num);
			cout << "ADDED" << endl;
		}
		else if (command == "REMOVE") {
			int num;
			cout << "ENTER NUMBER: " << flush;
			cin >> num;
			cout << (tree.remove(num) ? "REMOVED" : "NOT IN TREE") << endl;
		}
		else if (command == "SEARCH") {
			int num;
			cout << "ENTER NUMBER: " << flush;
			cin >> num;
			cout << (tree.inTree(num) ? "IN TREE" : "NOT IN TREE") << endl;
		}
		else if (command == "ADDFILE") {
			string filename;
			cout << "ENTER FILENAME: " << flush;
			cin >> filename;
			
			ifstream numfile;

			numfile.open(filename.c_str());
			if (!numfile.good()) {
				cout << "NO SUCH FILE" << endl;
				continue;
			}
					
			bool done = false;
			string token;
			while (std::getline(numfile, token, ',')) {
				int num = atoi(token.c_str());
				tree.add(num);
			}
			
			cout << "ADDED" << endl;
		}
		else if (command == "PRINT") {
			cout << tree.toString() << endl;
		}
		else if (command == "QUIT") {
			done = true;
		}
		else {
			cout << "INVALID COMMAND" << endl;
		}
	}

	return 0;
}