#pragma once
#include <map>

//함수 선언
FILE* File_Open_Read(const char*);  //파일 오픈
FILE* File_Open_Write(const char* name);
int counter(FILE*);
void find_Registers(char* token, int* cnt);
void storeLabel(const char* label, int line_number);
void replaceLabel(FILE* inputFile, FILE* outputFile);

extern const char* registers[];	//레지스터 정의
extern const char* instruc[];	//명령어 정의

extern std::map<std::string, const char*> label_map;  // 레이블을 저장할 map