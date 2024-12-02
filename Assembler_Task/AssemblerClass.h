#pragma once
#include <string>

using namespace std;

class registerClass {
	string reg_name;
	string reg_num;
public:
	void setRegClass(string regName, string regNum) {
		this->reg_name = regName;
		this->reg_num = regNum;
	}
	string getRegName() {
		return this->reg_name;
	}
	string getRegNum() {
		return this->reg_num;
	}
};

class symbolTable {
	string symbol;
	char wordType;
	int lc;
public:
	string data;
	symbolTable(string symbolName, char dataType, int lc, string symData) {
		this->symbol = symbolName;
		this->wordType = dataType;
		this->lc = lc;
		data = symData;
	}

	string getSymbol() {
		return this->symbol;
	}
	char getType() {
		return this->wordType;
	}
	int getLc() {
		return this->lc;
	}
};

// 각 인스트럭션의 정보를 보관하는 클래스
class instruction 
{
public:
	char instruct[6];
	char dest[2];
	char sour[2];
	char word_type[2];
	char ins_code[3];
	char ins_len[2];
	char mod_reg[9];
}; 
//명령어 한줄 씩 읽어서 데이터를 저장하는 클래스
class sentence
{
public:
	string label;
	string _operator;
	string operand [3];
};

//함수 선언
FILE* File_Open_Read(const char*);  //파일 오픈
FILE* File_Open_Write(const char* name);

void passFrist(FILE* fp);
void Initialize(instruction* );

extern const char* reigsterArr[];	//레지스터 정의
extern const char* instructionArr[];	//명령어 정의
extern const char* directivesArr[];	//지시어 정의

static int maxInst;	//정의된 명령어 테이블의 개수