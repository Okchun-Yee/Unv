#define _CRT_SECURE_NO_WARNINGS

#include "LinkedList.h"
#include <stdio.h>
#include <Windows.h>
#include <ctype.h>

#define SIZE 1024 //1KB

//함수 선언
FILE* File_Open(const char*);  //파일 오픈
int counter(FILE*);
int num_counter(char*);
int schar_counter(char*);

void clear() {
	Sleep(1000);
	system("cls");
}

FILE* File_Open(const char* name) {
	FILE* fp;
	fp = fopen(name, "r");
	// 파일 확인
	if (fp == NULL) {
		printf("%s : 파일 열기 실패\n", name);
		return NULL;
	}
	else {
		printf("%s : 파일 열기 성공\n", name);
		return fp;
	}
}

//Read String FILE & Sort dictionary
int counter(FILE* fp) {
	//파일에서 문자열을 읽어서 arr 포인터 배열에 저장
	char buffer[(SIZE)*5];	//임시로 문자열 담을 버퍼	(파일크기에 맞춰서 크기 지정 5KB)

	int word_cnt = 0; // 단어 수 셀 변수
	int num_cnt = 0; // 정수의 개수
	int sword_cnt = 0;	//특무문자의 개수

	//word 구분하기 => 구분자 (, . " " ! ? \n 등등)
	while (fgets(buffer, sizeof(buffer), fp) != NULL) {	//버퍼에 문자열 전체를 담음
		// 쉼표, 공백, 개행 문자를 기준으로 문자열 분리
		sword_cnt = schar_counter(buffer);	//strtok로 특수문자 공백처리하기 전에 버퍼에 있는 문자열에서 특수문자 찾기
		//printf("* %s", buffer);
		char* token = strtok(buffer, " ,.!?\n");	//구분자 제거
		while (token != NULL) {
			word_cnt++;
			//printf("Word : %s\n", token);
			if (num_counter(token)) {	//문자열에서 숫자 찾기
				//printf("Num : %s\n", token);
				num_cnt++;
			}	//정수일 경우 증가
			token = strtok(NULL, ", \n");
		}
	}
	printf("\n=========================================================================\n");
	printf("Word : %d\nNum : %d\nSpecial Word : %d\n", word_cnt, num_cnt,sword_cnt);
	return 1;
}
// 문자열이 정수인지 확인하는 함수
int num_counter(char* str) {
	if (*str == '\0') return 0; // 빈 문자열은 정수가 아님
	if (*str >= -1 && *str <= 255) {
		while (*str != NULL) {
			if (!isdigit(*str)) return 0; // 숫자가 아닌 문자가 있으면 정수가 아님
			str++;
		}
		return 1; // 모두 숫자인 경우 정수
	}
	return -1;	//err
}

int schar_counter(char* str) {
	int char_cnt = 0;	//특수문자 셀 변수

	if (*str == '\0') return 0; // 빈 문자열 탐지
	if (*str >= -1 && *str <= 255) {	//아스키코드 범위 내에서만 동작
		while (*str != NULL) {	//문자열이 빌때까지 동작
			if ((*str >= 33 && *str <= 47) || (*str >= 58 && *str <= 64) || (*str >= 91 && *str <= 96) || (*str >= 123 && *str <= 126))	//아스키코드상의 특수문자
			{
				//printf("Special : %c\n", *str);
				char_cnt++;
			}
			str++;
		}
		return char_cnt;
	}
	return -1; //err
}

int main(void) {
	//STRING FILE
	FILE* string_Mfp = NULL;
	const char* file_name = "I have a Dream..txt";  //열 파일 지정
	string_Mfp = File_Open(file_name);  //파일 열기 Part

	//파일 공백 검사
	if (string_Mfp != NULL) {
		//NULL이 아닐시 실행
		counter(string_Mfp);
		fclose(string_Mfp);  // 파일 닫기
	}
	return 0;
}