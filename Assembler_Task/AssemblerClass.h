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
	void setSymbol(string symName) {
		this->symbol = symName;
	}
	void setType(char type) {
		this->wordType = type;
	}
	void setLC(int loCnt) {
		this->lc = loCnt;
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

// �� �ν�Ʈ������ ������ �����ϴ� Ŭ����
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
//��ɾ� ���� �� �о �����͸� �����ϴ� Ŭ����
class sentence
{
public:
	string label;
	string _operator;
	string operand [3];
};

//�Լ� ����
FILE* File_Open_Read(const char*);  //���� ����
FILE* File_Open_Write(const char* name);
string binaryToHex(const char* binaryStr);
int readLine(string sen, instruction* insTable, sentence&);
int passFrist(int ,char* , instruction* , symbolTable* , sentence& ,int* );
int passSecond(int, char*, instruction*, symbolTable*, sentence&, registerClass*);
void Initialize(instruction*, registerClass*);
int btoi(char* );

extern const char* reigsterArr[];	//�������� ����
extern const char* instructionArr[];	//��ɾ� ����
extern const char* directivesArr[];	//���þ� ����

static int maxInst;	//���ǵ� ��ɾ� ���̺��� ����