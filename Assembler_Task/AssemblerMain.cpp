#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "AssemblerClass.h"
using namespace std;
int main(void) {
	FILE* string_Mfp = NULL;
	//const char* file_name = "symbol.txt";  //열 파일 지정
	//string_Mfp = File_Open_Read(file_name);  //파일 열기 Part
	instruction* insTable = new instruction[128];
	Initialize(insTable);

	return 0;
}