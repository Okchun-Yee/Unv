#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "Assembler.h"

using namespace std;

FILE* File_Open_Read(const char* name) {
	FILE* fp;
	fp = fopen(name, "r");
	// 파일 확인
	if (fp == NULL) {
		printf("%s : 읽기 파일 열기 실패\n", name);
		return NULL;
	}
	else {
		printf("%s : 읽기 파일 열기 성공\n", name);
		return fp;
	}
}

FILE* File_Open_Write(const char* name) {
	FILE* fp;
	fp = fopen(name, "w");
	// 파일 확인
	if (fp == NULL) {
		printf("%s : 쓰기 파일 열기 실패\n", name);
		return NULL;
	}
	else {
		printf("%s : 쓰기 파일 열기 성공\n", name);
		return fp;
	}
}


int main(void) {
	//STRING FILE
	FILE* string_Mfp = NULL;
	const char* file_name = "symbol.txt";  //열 파일 지정
	string_Mfp = File_Open_Read(file_name);  //파일 열기 Part

	//파일 공백 검사
	if (string_Mfp != NULL) {
		//NULL이 아닐시 실행
		counter(string_Mfp);
		fclose(string_Mfp);  // 파일 닫기
	}

	string_Mfp = File_Open_Read(file_name);
	FILE* end_fp = NULL;
	file_name = "output.txt";
	end_fp = File_Open_Write(file_name);
	if (end_fp != NULL) {
		//NULL이 아닐시 실행
		replaceLabel(string_Mfp, end_fp);
		fclose(string_Mfp);  // 파일 닫기
		fclose(end_fp);  // 파일 닫기
	}

	return 0;
}