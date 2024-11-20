#include "playfair.h"
#include <iostream>
#include <string>

int main() {
	Playfair pa1("assasinator", "a/b");
	pa1.makeTable();
	pa1.showTable();
	return 0;
}