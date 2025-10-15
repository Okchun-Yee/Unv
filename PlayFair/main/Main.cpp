#include "playfair.h"
#include <iostream>
#include <string>

int main() {
	Playfair key("assasinator", "q/z");
	key.showTable();
	string nomalKey = "thisistextoftaskcheckedaaaqzqqz";
	string afterKey = key.makeEncryption(nomalKey);
	cout << nomalKey << " -> " << afterKey;
	return 0;
}