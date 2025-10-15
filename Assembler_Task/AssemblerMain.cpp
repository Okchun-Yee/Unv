#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "AssemblerClass.h"

#define SIZE 1024 //1KB

using namespace std;

void printSymbolTable(symbolTable* symTable) {
	cout << endl << "---------------------------------" << endl;
	cout <<"Index\tSymbol\tType\tDATA\tLC" << endl;
	for (int i = 0; symTable[i].getSymbol()!="\0"; i++) {
		cout << i << "\t"
			<< symTable[i].getSymbol() << "\t"
			<< symTable[i].getType() << "\t"
			<< symTable[i].data << "\t";
		printf("%02X\n", symTable[i].getLc());
	}
	cout << "---------------------------------" << endl;
}

int main(void) {
	FILE* fp = NULL;
	int locationCnt = 0;
	const char* file_name = "test10.asm";  //열 파일 지정
	fp = File_Open_Read(file_name);  //파일 열기 Part

	instruction* insTable = new instruction[1024];
	symbolTable* symTable = new symbolTable[1024];
	registerClass* regTable = new registerClass[20];
	sentence line;

	Initialize(insTable, regTable);

	char buffer[SIZE];
	int indexCnt = 0;
	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		//cout << buffer;
		locationCnt = passFrist(locationCnt, buffer, insTable, symTable, line, &indexCnt);
		//cout << indexCnt << endl;
	}
	printSymbolTable(symTable);
	cout << endl;
	rewind(fp); locationCnt = 0;
	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		//cout << buffer;
		locationCnt = passSecond(locationCnt, buffer, insTable, symTable, line, regTable);
	}

	delete[] insTable; delete[] symTable; delete[] regTable;
	fclose(fp);
	return 0;
}