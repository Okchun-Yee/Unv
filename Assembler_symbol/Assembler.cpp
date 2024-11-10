#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <ctype.h>
#include "Assembler.h"

#define SIZE 1024 //1KB
using namespace std;

//Read String FILE & Sort dictionary
int counter(FILE* fp) {
	//파일에서 문자열을 읽어서 arr 포인터 배열에 저장
	char buffer[(SIZE)];	//임시로 문자열 담을 버퍼	(파일크기에 맞춰서 크기 지정 5KB)

	int word_cnt = 0; // 단어 수 셀 변수

	//word 구분하기 => 구분자 (, . " " ! ? \n 등등)
	while (fgets(buffer, sizeof(buffer), fp) != NULL) {	//버퍼에 문자열 전체를 담음
		// 설정한 구분자 기준으로 문자열 분리
		char* token = strtok(buffer, "\t\n,+[]: ");	//구분자 제거
		while (token != NULL) {
			word_cnt++;
			cout << token << endl;
			token = strtok(NULL, ", \n");
		}
	}
	return 1;
}