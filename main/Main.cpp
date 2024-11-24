#include "playfair.h"
#include <iostream>
#include <string>

int main() {
	Playfair key("assasinator", "q/z");
	//key.showTable();
	string afterKey = key.makeEncryption("whatiskey");
	cout << "whatiskey - > " << afterKey;
	return 0;
}