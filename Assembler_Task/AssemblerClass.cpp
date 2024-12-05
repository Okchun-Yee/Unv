#define _CRT_SECURE_NO_WARNINGS
#include "AssemblerClass.h"
#include <iostream>
#include <string>
#include <ctype.h>  //strtok 함수
#include <iomanip> // for std::setw
#include <cctype>  //isalpha 함수
#include <bitset>
#include <sstream>

using namespace std;

const char* reigsterArr[] = {
	"AX", "BX", "CX", "DX",  // 16-bit 일반 레지스터들
	"AL", "AH", "BL", "BH", "CL", "CH", "DL", "DH", 
	"SI", "DI", 
	NULL // 8-bit 하위 레지스터들
};

const char* instructionArr[] = {
	"MOV", "ADD", "DIV", "INC", "DEC", "CMP", "SUB", "INT", // 어셈블러 명령어
	"JA", "JAE", "JB", "JBE", "JC", "JE", "JNE", "JZ", "JZE", "JMP" ,NULL // 조건분기 명령어
};
const char* directivesArr[] = {
	"DW", "DB",
	"SEGMENT", "ASSUME", "PROC", "MACRO", "ENDS", "ENDP", "ENDM","END", //어셈블러 지시어
};

FILE* File_Open_Read(const char* name) {
	FILE* fp;
	fp = fopen(name, "r");
	// 파일 확인
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
	// 파일 확인
	if (fp == NULL) {
		printf("%s : WriteFile Open False\n", name);
		return NULL;
	}
	else {
		printf("%s : WriteFile Open True\n", name);
		return fp;
	}
}

void Initialize(instruction* insTable, registerClass* regTable) {
	int cnt = 0;
	FILE* regi = File_Open_Read("reg_tbl.txt");
	char temp1[3], temp2[4];
	//레지스터 테이블 작성
	cout << "---------------------------------" << endl;
	while (fscanf(regi, "%s %s", temp1, temp2) == 2) {
		regTable[cnt].setRegClass(string(temp1), string(temp2));
		cout << regTable[cnt].getRegName() << " " << regTable[cnt].getRegNum()<<"\t";
		cnt++;
		if ((cnt % 5 == 0) && cnt != 0) cout << endl;
		if (cnt >= 20) break;
	}
	cout << "---------------------------------" << endl;
	FILE* inst = File_Open_Read("inst_tbl.txt");
	cnt = 1;
	//명령어 테이블 작성
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
	/*for (int i = 1; i < cnt; i++) {
		cout
			<< "---------------------------------" << endl;
			<< left << setw(10) << insTable[i].instruct
			<< setw(5) << insTable[i].dest
			<< setw(5) << insTable[i].sour
			<< setw(10) << insTable[i].word_type
			<< setw(5) << insTable[i].ins_code
			<< setw(5) << insTable[i].ins_len
			<< setw(12) << insTable[i].mod_reg
			<< "---------------------------------" << endl;
	}*/

	fclose(regi); fclose(inst);
}

//피연산자가 레지스터인지, 심벌인지 구분
int Analyze(string operand) {
	int cnt = 0;

	// operand[0]이 숫자인지 확인, 숫자라면 immediate 어드레스 모드(0)를 반환
	if (isdigit(operand[0]) || (operand[0] == '-' && isdigit(operand[1]))) {
		return 0; // Immediate
	}

	// 레지스터 배열에 대해 비교, 배열 끝(NULL)을 만날 때까지 반복
	while (reigsterArr[cnt] != NULL) {
		if (operand == string(reigsterArr[cnt]))  // operand가 regist[i]와 같다면
		{
			if (cnt < 4) return 1;	//첫 번째 그룹 (AX, BX, CX, DX)
			else return 2;			//두 번째 그룹 (AL, BL, CL, DL, AH, BH, CH, DH)
		}
		else cnt++;  // 다음 레지스터로 이동
	}

	return 3;  // 그 외의 경우 (레지스터가 아닌 경우)는 3을 반환
}

#define MAX_INS 1 
int readLine(string sen, instruction* insTable, sentence& line) {
	register int k = maxInst;
	int wordP = 0, index = 0;
	string op[5];
	for (int i = 0; sen[wordP] != '\n' && sen[wordP] != '\0'; i++) {
		while (sen[wordP] == ' ' || sen[wordP] == '\t' || sen[wordP] == ',' || sen[wordP] == ';') wordP++;
		if (index < 5) {  // index가 op 배열의 범위를 초과하지 않도록 제한
			for (; sen[wordP] != ' ' && sen[wordP] != '\t' && sen[wordP] != '\n' && sen[wordP] != ',' && sen[wordP] != '\0' && sen[wordP] != ';'; wordP++) {
				op[index] += sen[wordP]; // 단어를 op[index]에 추가
			}
			index++;  // 단어를 다 읽은 후에 index 증가
		}
	}

	int cnt = 0;
	for (cnt = 0; instructionArr[cnt] != NULL;cnt++) {
		if (op[0] == instructionArr[cnt]) {
			line.label = "\0";
			line._operator = op[0];
			//cout <<cnt<<" | " << index << endl;
			for (int i = 1; i < index; i++) {
				line.operand[i - 1] = op[i];
			}
			break;
		}
	}
	if ((cnt == size(instructionArr) - 1) && op[0] != "\n") {
		line.label = op[0];
		line._operator = op[1];
		for (int i = 0; i < 2; i++)line.operand[i] = op[i + 2];
	}
	index = 0;
	strcpy(insTable[0].instruct, op[index].c_str());

	switch (Analyze(op[index + 1]))
	{
	case 0: strcpy(insTable[0].dest, "I");
		break;
	case 1: strcpy(insTable[0].dest, "R");
		strcpy(insTable[0].word_type, "W");
		break;
	case 2: strcpy(insTable[0].dest, "R");
		strcpy(insTable[0].word_type, "B");
		break;
	case 3: strcpy(insTable[0].dest, "M");
		break;
	}
	switch (Analyze(op[index + 2]))
	{
	case 0: strcpy(insTable[0].sour, "I");
		break;
	case 1: strcpy(insTable[0].sour, "R");
		strcpy(insTable[0].word_type, "W");
		break;
	case 2: strcpy(insTable[0].sour, "R");
		strcpy(insTable[0].word_type, "B");
		break;
	case 3: strcpy(insTable[0].sour, "M");
		break;
	}
	while (strcmp(insTable[k].instruct, insTable[0].instruct)
		|| strcmp(insTable[k].dest, insTable[0].dest)
		|| strcmp(insTable[k].sour, insTable[0].sour)
		|| strcmp(insTable[k].word_type, insTable[0].word_type)) {
		k--;
	}
	/*cout << k << "\t"
		<< insTable[k].instruct << "\t\t"
		<< insTable[k].dest << "\t"
		<< insTable[k].sour << "\t"
		<< insTable[k].word_type << "\t\t"
		<< "Match" <<  endl;*/
	/*cout 
		<< "Lable\t"
		<< line.label << "\t"
		<< line._operator << "\t"
		<< line.operand[0] << "\t"
		<< line.operand[1] << "\t" << endl;*/
	return k; // k == 0 해당 문자열은 심벌이다.
}


int passFrist(int loCnt, char* buffer, instruction* insTable, symbolTable* symTable, sentence& line, int* indexCnt) {
	int i = readLine(buffer, insTable, line);
	int cnt = 0;

	/*cout
		<< "Lable\t"<<i<<"\t"
		<< line.label << "\t"
		<< line._operator << "\t"
		<< line.operand[0] << "\t"
		<< line.operand[1] << "\t" << endl;*/
	/*cout << i << "\t"
		<< insTable[i].instruct << "\t\t"
		<< insTable[i].dest << "\t"
		<< insTable[i].sour << "\t"
		<< insTable[i].word_type << "\t\t" << endl;*/

	if (i != 0)
	{
		printf("%04X:\t%s", loCnt, buffer);
		loCnt += atoi(insTable[i].ins_len);
	}
	else {
		if (line._operator == "DW") symTable[*indexCnt].setType('W');
		else if (line._operator == "DB") symTable[*indexCnt].setType('B');
		symTable[*indexCnt].setSymbol(line.label);
		symTable[*indexCnt].setLC(loCnt);
		symTable[*indexCnt].data = line.operand[0];
		
		printf("%04X:\t%s", loCnt, buffer);
		if (symTable[*indexCnt].getType() == 'W') loCnt += 2;
		else if (symTable[*indexCnt].getType() == 'B') loCnt += 1;
		/*cout << "symbolName \t" << *indexCnt<<"\t"
			<< symTable[*indexCnt].getSymbol() << "\t"
			<< symTable[*indexCnt].getType() << "\t"
			<< symTable[*indexCnt].getLc() << "\t"
			<< endl;*/
		if (symTable[*indexCnt].getSymbol() != "\0") {
			//printf("%04X: %s", loCnt, buffer);
			*indexCnt += 1;
			loCnt += 1;
		}
		cnt++;
	}
	return loCnt;
}

int btoi(char* dig)
{
	int ret = 0;
	int length = strlen(dig);  // 문자열 길이 한번만 계산
	for (int i = 0; i < length; i++) {
		if (dig[i] == '1') {
			ret += (1 << (length - i - 1));  // 2의 거듭제곱을 비트 연산으로 처리
		}
	}
	return ret;
}

int passSecond(int loCnt, char* buffer, instruction* insTable, symbolTable* symTable, sentence& line, registerClass* regTable) {
	int i = readLine(buffer, insTable, line); // 현재 라인을 읽어 명령어 정보를 가져옴
	int cnt = 0;
	char* numptr;
	int deNum = 0;
	//printf("%04X:\t%s", loCnt, buffer);
	if (i != 0) { // 명령어 테이블에서 매칭된 경우
		printf("%04X:\t", loCnt);

		// 명령어 코드 출력
		printf("%s\t", insTable[i].ins_code);

		// 목적지 피연산자 처리
		if (strcmp(insTable[i].dest, "R") == 0) { // 목적지가 레지스터일 경우
			for (int j = 0; regTable[j].getRegName() != ""; j++) {
				if (line.operand[0] == regTable[j].getRegName()) { // operand[0]과 일치하는 레지스터를 찾음
					// 레지스터 번호를 mod_reg에 변경
					char* modRegPos = strchr(insTable[i].mod_reg, '?');
					if (modRegPos) {
						strncpy(modRegPos, regTable[j].getRegNum().c_str(), 3); // 레지스터 번호로 교체
					}
					//printf("%s\t", insTable[i].mod_reg);
					break;
				}
			}
		}
		else if (strcmp(insTable[i].dest, "M") == 0) { // 메모리 주소인 경우
			bool check = false;
			for (int j = 0; symTable[j].getSymbol() != "\0"; j++) {
				if (symTable[j].getSymbol() == line.operand[0]) {
					deNum = stoi(symTable[j].data);
					printf("%02X\t", deNum & 0xFF);
				}
				check = true;
			}
			if(!check) printf("%-10s ", line.operand[1].c_str());
		}
		else if (strcmp(insTable[i].dest, "I") == 0) { // 즉시값인 경우
			// 10진수 문자열을 변환
			deNum = strtol(line.operand[0].c_str(), &numptr, 10);

			// 16진수로 출력 (16비트 범위 내)
			printf("%02X\t", deNum & 0xFF);
		}

		// 소스 피연산자 처리
		if (strcmp(insTable[i].sour, "R") == 0) { // 소스가 레지스터일 경우
			for (int j = 0; regTable[j].getRegName() != ""; j++) {
				if (line.operand[1] == regTable[j].getRegName()) { // operand[1]과 일치하는 레지스터를 찾음
					// 레지스터 번호를 mod_reg에 변경
					char* modRegPos = strchr(insTable[i].mod_reg, '?');
					if (modRegPos) {
						strncpy(modRegPos, regTable[j].getRegNum().c_str(), 3); // 레지스터 번호로 교체
					}
					//printf("%s\t", insTable[i].mod_reg);
					break;
				}
			}
		}
		else if (strcmp(insTable[i].sour, "M") == 0) { // 메모리 주소인 경우
			bool check = false;
			for (int j = 0; symTable[j].getSymbol() != "\0"; j++) {
				if (symTable[j].getSymbol() == line.operand[1]) {
					deNum = stoi(symTable[j].data);
					printf("%02X\t", deNum & 0xFF);
				}
				check = true;
			}
			if(!check) printf("%-10s ", line.operand[1].c_str());
		}
		else if (strcmp(insTable[i].sour, "I") == 0) { // 즉시값인 경우
			// 10진수 문자열을 변환
			deNum = strtol(line.operand[1].c_str(), &numptr, 10);

			// 16진수로 출력 (16비트 범위 내)
			printf("%02X\t", deNum & 0xFF);
		}
		deNum = strtol(insTable[i].mod_reg, &numptr, 2);
		printf("%02X\t", deNum & 0xFF);
		//cout << insTable[i].mod_reg;
		printf("\n");
		loCnt += atoi(insTable[i].ins_len); // 다음 주소로 이동
	}
	else { // 명령어가 아닌 심볼 테이블에 등록된 경우
		for (int j = 0; j < cnt; j++) {
			if (line.label == symTable[j].getSymbol()) {
				//printf("%04X:\t", symTable[j].getLc());
				if (symTable[j].getType() == 'W') {
				}
				else if (symTable[j].getType() == 'B') {
				}
				break;
			}
		}
	}
	return loCnt;
}


