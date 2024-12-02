#define _CRT_SECURE_NO_WARNINGS
#include "AssemblerClass.h"
#include <iostream>
#include <string>
#include <ctype.h>  //strtok �Լ�
#include <iomanip> // for std::setw
#include <cctype>  //isalpha �Լ�

using namespace std;

#define SIZE 1024 //1KB

const char* reigsterArr[] = {
	"AX", "BX", "CX", "DX",  // 16-bit �Ϲ� �������͵�
	"AL", "AH", "BL", "BH", "CL", "CH", "DL", "DH", NULL // 8-bit ���� �������͵�
};

const char* instructionArr[] = {
	"MOV", "ADD", "DIV", "INC", "DEC", "CMP", "SUB", "INT", // ����� ��ɾ�
	"JA", "JAE", "JB", "JBE", "JC", "JE", "JNE", "JZ", "JZE",  // ���Ǻб� ��ɾ�
};
const char* directivesArr[] = {
	"DW", "DB",
	"SEGMENT", "ASSUME", "PROC", "MACRO", "ENDS", "ENDP", "ENDM","END", //����� ���þ�
};

FILE* File_Open_Read(const char* name) {
	FILE* fp;
	fp = fopen(name, "r");
	// ���� Ȯ��
	if (fp == NULL) {
		printf("%s : ReadFile Open False\n", name);
		return NULL;
	}
	else {
		printf("%s : ReadFile Open True\n", name);
		return fp;
	}
}

FILE* File_Open_Write(const char* name) {
	FILE* fp;
	fp = fopen(name, "w");
	// ���� Ȯ��
	if (fp == NULL) {
		printf("%s : WriteFile Open False\n", name);
		return NULL;
	}
	else {
		printf("%s : WriteFile Open True\n", name);
		return fp;
	}
}

void Initialize(instruction* insTable) {
	int cnt = 0;
	FILE* regi = File_Open_Read("reg_tbl.txt");
	registerClass* regTable = new registerClass[20];
	char temp1[3], temp2[4];
	//�������� ���̺� �ۼ�
	while (fscanf(regi, "%s %s", temp1, temp2) == 2) {
		regTable[cnt].setRegClass(string(temp1), string(temp2));
		cout << regTable[cnt].getRegName() << " " << regTable[cnt].getRegNum() << endl;
		cnt++;

		if (cnt >= 20) break;
	}

	FILE* inst = File_Open_Read("inst_tbl.txt");
	cnt = 1;
	//��ɾ� ���̺� �ۼ�
	while (fscanf(inst, "%6s%2s%2s%4s%3s%2s%9s\n",
		insTable[cnt].instruct,
		insTable[cnt].dest,
		insTable[cnt].sour,
		insTable[cnt].word_type,
		insTable[cnt].ins_code,
		insTable[cnt].ins_len,
		insTable[cnt].mod_reg) == 7)
	{
		cnt++;
	}
	maxInst = cnt - 1;
	for (int i = 1; i < cnt; i++) {
		cout
			<< left << setw(10) << insTable[i].instruct
			<< setw(5) << insTable[i].dest
			<< setw(5) << insTable[i].sour
			<< setw(10) << insTable[i].word_type
			<< setw(5) << insTable[i].ins_code
			<< setw(5) << insTable[i].ins_len
			<< setw(12) << insTable[i].mod_reg
			<< endl;
	}

	fclose(regi); fclose(inst);
}

//�ǿ����ڰ� ������������, �ɹ����� ����
int Analyze(string operand) {
	int cnt = 0;

	// operand[0]�� �������� Ȯ��, ���ڶ�� immediate ��巹�� ���(0)�� ��ȯ
	if (isdigit(operand[0])) return 0;

	// �������� �迭�� ���� ��, �迭 ��(NULL)�� ���� ������ �ݺ�
	while (reigsterArr[cnt] != NULL) {
		if (operand == string(reigsterArr[cnt]))  // operand�� regist[i]�� ���ٸ�
		{
			if (cnt < 4) return 1;	//ù ��° �׷� (AX, BX, CX, DX)
			else return 2;			//�� ��° �׷� (AL, BL, CL, DL, AH, BH, CH, DH)
		}
		else cnt++;  // ���� �������ͷ� �̵�
	}

	return 3;  // �� ���� ��� (�������Ͱ� �ƴ� ���)�� 3�� ��ȯ
}

#define MAX_INS 1 
int readLine(string sen, instruction* insTable) {
	register int k = maxInst;
	sentence line;
	int wordP = 0, index = 0;
	string op[5];
	for (int i = 0; sen[wordP] != '\n'; i++) {
		while (sen[wordP] == ' ' || sen[wordP] == '\t' || sen[wordP] == ',') wordP++;
		for (index = 0; sen[wordP] != ' ' && sen[wordP] != '\t' && sen[wordP] != '\n' && sen[wordP] != ','; index++) {
			op[index][i] = sen[wordP];
			wordP++;
		}
	}
	int cnt = 0;
	while (strcmp(instructionArr[cnt], "") != 0) {
		if (op[cnt] != instructionArr[cnt])cnt++;
		else
		{
			line._operator = op[0];
			for (int i = 1; i < index; i++) line.operand[i - 1] = op[i];
			break;
		}
	}
	if (cnt == MAX_INS) {
		line.label = op[0];
		line._operator = op[1];
		for (int i = 2; i < index; i++) line.operand[i - 2] = op[i];
	}

	if (cnt == MAX_INS)index = cnt;
	else index = 0;

	strcpy(insTable[0].instruct, op[index].c_str());

	switch (Analyze(op[index+1]))
	{
	case 0: strcpy(insTable[0].dest, "i");
		break;
	case 1: strcpy(insTable[0].dest, "r");
		strcpy(insTable[0].word_type, "w");
		break;
	case 2: strcpy(insTable[0].dest, "r");
		strcpy(insTable[0].word_type, "b");
		break;
	case 3: strcpy(insTable[0].dest, "m");
		break;
	}
	switch (Analyze(op[index+2]))
	{
	case 0: strcpy(insTable[0].sour, "i");
		break;
	case 1: strcpy(insTable[0].sour, "r");
		strcpy(insTable[0].word_type, "w");
		break;
	case 2: strcpy(insTable[0].sour, "r");
		strcpy(insTable[0].word_type, "b");
		break;
	case 3: strcpy(insTable[0].sour, "m");
		break;
	}

	while (strcmp(insTable[k].instruct, insTable[0].instruct)
		|| strcmp(insTable[k].dest, insTable[0].dest)
		|| strcmp(insTable[k].sour, insTable[0].sour)
		|| strcmp(insTable[k].word_type, insTable[0].word_type))
		k--;
	return k; // k == 0 �ش� ���ڿ��� �ɹ��̴�.
}

void passFrist(FILE* fp) {

}

