#pragma once
#include <map>

//�Լ� ����
FILE* File_Open_Read(const char*);  //���� ����
FILE* File_Open_Write(const char* name);
int counter(FILE*);
void find_Registers(char* token, int* cnt);
void storeLabel(const char* label, int line_number);
void replaceLabel(FILE* inputFile, FILE* outputFile);

extern const char* registers[];	//�������� ����
extern const char* instruc[];	//��ɾ� ����

extern std::map<std::string, const char*> label_map;  // ���̺��� ������ map