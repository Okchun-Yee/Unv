#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <ctype.h>  //strtok 함수
#include <map>  //딕셔러니 라이브러리
#include <cctype>  //isalpha 함수
#include "Assembler.h"

using namespace std;

#define SIZE 1024 //1KB

map<string, const char*> label_map;  // 레이블을 저장할 딕셔너리

const char* registers[] = {
    "AX", "BX", "CX", "DX",  // 16-bit 일반 레지스터들
    "SI", "DI", "SP", "BP",  // 인덱스, 디스플레이, 스택 포인터, 베이스 포인터
    "IP", "FLAGS",  // 프로그램 카운터, 플래그 레지스터
    "CS", "DS", "SS", "ES", "FS", "GS",  // 세그먼트 레지스터들
};
const char* byteReigsters[] = {
    "AL", "AH", "BL", "BH", "CL", "CH", "DL", "DH",  // 8-bit 하위 레지스터들
};
const char* instruc[] = {
    "MOV", "ADD", "DIV", "INC", "DEC", "CMP", "SUB", "INT", // 어셈블러 명령어
    "JA", "JAE", "JB", "JBE", "JC", "JE", "JNE",  // 조건분기 명령어
    "DW", "DB"  // 단위
};

// 딕셔너리에 레이블을 저장하는 함수
void storeLabel(const char* label, const char* line_number) {
    // 레이블에서 ':' 제거
    string clean_label = label;
    if (clean_label.back() == ':') {
        clean_label.pop_back();  // ':' 제거
    }

    // 레이블이 이미 map에 존재하지 않으면 추가
    if (label_map.find(clean_label) == label_map.end()) {
        label_map[clean_label] = line_number;  // 레이블과 해당 줄 번호를 저장
    }
}

// 문자열에서 단어를 구분하여 레이블을 찾고 딕셔너리에 저장하는 함수
int counter(FILE* fp) {
    char buffer[SIZE];  // 임시로 문자열 담을 버퍼

    int word_cnt = 0;   //IP 주소 탐색기

    char* pre_token = NULL; //이전 token
    int pre_word_cnt = 0;

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {  // 버퍼에 문자열 전체를 담음
        int instruction_length = sizeof(instruc) / sizeof(instruc[0]);  //명령어 배열 길이
        bool check = true;

        // 문자열을 구분자 기준으로 분리
        char* token = strtok(buffer, "\t\n,+[] ");
        while (token != NULL) {
            // 레이블 찾기: ':'로 끝나는지 확인하고 ':' 제거
            if (token[strlen(token) - 1] == ':') {
                token[strlen(token) - 1] = '\0';  // ':' 제거
                //cout << "Label : " << token << " " << word_cnt << endl;
                storeLabel(token, "sym");  // 레이블과 해당 줄 번호 저장
            }
            //변수 선언부 찾기 현재 토큰이 DW나 DB일시 그 이전 토큰을 변수 선언부라고 판단
            else if (strcmp(token, "DB") == 0 || strcmp(token, "DW") == 0) {
                //cout << pre_token << " " << pre_word_cnt << endl;
                storeLabel(pre_token, "sym");
            }

            // 명령어인지 확인
            bool is_instruction = false;
            for (int i = 0; i < instruction_length; i++) {
                if (strcmp(token, instruc[i]) == 0) {
                    storeLabel(token, "op");
                    is_instruction = true;
                    word_cnt++;
                    break;
                }
            }
            //명령어가 아닌 경우 레지스터 확인
            if (!is_instruction) {
                find_Registers(token, &word_cnt); 
            }
            pre_token = token;  //이전 토근 저장
            pre_word_cnt = word_cnt;   //이전 토근 주소번호 저장
            token = strtok(NULL, "\t\n,+[] ");  // 다음 토큰
        }
        word_cnt++;
    }
    return 1;
}

// 레지스터 확인
void find_Registers(char* token, int* cnt) {
    //레지스터 확인 배열의 길이
    int register_count = sizeof(registers) / sizeof(registers[0]);
    int byteReigster_count = sizeof(byteReigsters) / sizeof(byteReigsters[0]);
    bool check = false;

    for (int i = 0; i < register_count; i++) {
        if (strcmp(token, registers[i]) == 0) {
            check = true;
            for (int j = 0; j < byteReigster_count; j++) {
                if (strcmp(token, byteReigsters[i]) == 0) {
                    storeLabel(token, "reg8");
                    break;
                }
            }
            storeLabel(token, "reg16");
            break;
        }
    }
    // 명령어도 아니고 레지스터도 아닌 경우 + 첫글자가 문자가 아닌경우 = 상수, IP 주소 증가
    if (!check&&!isalpha(token[0])) {
        //cout << token << " " << *cnt << endl;
        storeLabel(token, "num");
        (*cnt)++;
    }
}

// 레이블을 해당 줄 번호로 치환하는 함수
void replaceLabel(FILE* inputFile, FILE* outputFile) {
    char buffer[SIZE];  // 파일에서 한 줄씩 읽을 버퍼
    while (fgets(buffer, sizeof(buffer), inputFile)) {
        char* token = strtok(buffer, "\t\n,+[] ");  // 구분자로 문자열을 분리
        while (token != NULL) {
            // 레이블 처리: ':'로 끝나는 토큰을 확인하고 ':' 제거
            if (token[strlen(token) - 1] == ':') {
                // ':' 제거
                token[strlen(token) - 1] = '\0';
            }

            // 레이블을 찾으면, map에서 해당 줄 번호로 치환
            if (label_map.find(token) != label_map.end()) {
                const char* line_number = label_map[token];
                fprintf(outputFile, "%s ", line_number);  // 레이블을 숫자로 치환
            }
            else {
                fprintf(outputFile, "%s ", token);  // 레이블이 아니면 그대로 출력
            }
            token = strtok(NULL, "\t\n,+[] ");  // 다음 토큰
        }
        fprintf(outputFile, "\n");
    }
}

