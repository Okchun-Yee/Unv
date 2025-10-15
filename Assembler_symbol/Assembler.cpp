#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <ctype.h>  //strtok �Լ�
#include <map>  //��ŷ��� ���̺귯��
#include <cctype>  //isalpha �Լ�
#include "Assembler.h"

using namespace std;

#define SIZE 1024 //1KB

map<string, const char*> label_map;  // ���̺��� ������ ��ųʸ�

const char* registers[] = {
    "AX", "BX", "CX", "DX",  // 16-bit �Ϲ� �������͵�
    "SI", "DI", "SP", "BP",  // �ε���, ���÷���, ���� ������, ���̽� ������
    "IP", "FLAGS",  // ���α׷� ī����, �÷��� ��������
    "CS", "DS", "SS", "ES", "FS", "GS",  // ���׸�Ʈ �������͵�
};
const char* byteReigsters[] = {
    "AL", "AH", "BL", "BH", "CL", "CH", "DL", "DH",  // 8-bit ���� �������͵�
};
const char* instruc[] = {
    "MOV", "ADD", "DIV", "INC", "DEC", "CMP", "SUB", "INT", // ����� ��ɾ�
    "JA", "JAE", "JB", "JBE", "JC", "JE", "JNE", "JZ", "JZE",  // ���Ǻб� ��ɾ�
    "DW", "DB"  // ����
};

// ��ųʸ��� ���̺��� �����ϴ� �Լ�
void storeLabel(const char* label, const char* line_number) {
    // ���̺��� ':' ����
    string clean_label = label;
    if (clean_label.back() == ':') {
        clean_label.pop_back();  // ':' ����
    }

    // ���̺��� �̹� map�� �������� ������ �߰�
    if (label_map.find(clean_label) == label_map.end()) {
        label_map[clean_label] = line_number;  // ���̺�� �ش� �� ��ȣ�� ����
    }
}

// ���ڿ����� �ܾ �����Ͽ� ���̺��� ã�� ��ųʸ��� �����ϴ� �Լ�
int counter(FILE* fp) {
    char buffer[SIZE];  // �ӽ÷� ���ڿ� ���� ����

    int word_cnt = 0;   //IP �ּ� Ž����

    char* pre_token = NULL; //���� token
    int pre_word_cnt = 0;

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {  // ���ۿ� ���ڿ� ��ü�� ����
        int instruction_length = sizeof(instruc) / sizeof(instruc[0]);  //��ɾ� �迭 ����
        bool check = true;

        // ���ڿ��� ������ �������� �и�
        char* token = strtok(buffer, "\t\n,+[] ");
        while (token != NULL) {
            // ���̺� ã��: ':'�� �������� Ȯ���ϰ� ':' ����
            if (token[strlen(token) - 1] == ':') {
                token[strlen(token) - 1] = '\0';  // ':' ����
                //cout << token << "\t" << "sym" << endl;
                storeLabel(token, "sym");  // ���̺�� �ش� �� ��ȣ ����
            }
            //���� ����� ã�� ���� ��ū�� DW�� DB�Ͻ� �� ���� ��ū�� ���� ����ζ�� �Ǵ�
            else if (strcmp(token, "DB") == 0 || strcmp(token, "DW") == 0) {
                //cout << token << "\t" << "sym" << endl;
                storeLabel(pre_token, "sym");
            }

            // ��ɾ����� Ȯ��
            bool is_instruction = false;
            for (int i = 0; i < instruction_length; i++) {
                if (strcmp(token, instruc[i]) == 0) {
                    storeLabel(token, "op");
                    //cout << token << "\t" << "op" << endl;
                    is_instruction = true;
                    word_cnt++;
                    break;
                }
            }
            //��ɾ �ƴ� ��� �������� Ȯ��
            if (!is_instruction) {
                find_Registers(token, &word_cnt); 
            }
            pre_token = token;  //���� ��� ����
            pre_word_cnt = word_cnt;   //���� ��� �ּҹ�ȣ ����
            token = strtok(NULL, "\t\n,+[] ");  // ���� ��ū
        }
        word_cnt++;
    }
    return 1;
}

// �������� Ȯ��
void find_Registers(char* token, int* cnt) {
    //�������� Ȯ�� �迭�� ����
    int register_count = sizeof(registers) / sizeof(registers[0]);
    int byteReigster_count = sizeof(byteReigsters) / sizeof(byteReigsters[0]);
    bool check = false;

    for (int i = 0; i < register_count; i++) {
        if (strcmp(token, registers[i]) == 0) {
            check = true;
            storeLabel(token, "reg16");
            //cout << token << "\t" << "reg16" << endl;
            break;
        }
        else {
            for (int j = 0; j < byteReigster_count; j++) {
                if (strcmp(token, byteReigsters[j]) == 0) {
                    check = true;
                    storeLabel(token, "reg8");
                    //cout << token << "\t" << "reg8" << endl;
                    break;
                }
            }
        }
    }
    // ��ɾ �ƴϰ� �������͵� �ƴ� ��� + ù���ڰ� ���ڰ� �ƴѰ�� = ���, IP �ּ� ����
    if (!check&&!isalpha(token[0])) {
        //cout << token << "\t" << "num" << endl;
        storeLabel(token, "num");
        (*cnt)++;
    }
}

// ���̺��� �ش� �� ��ȣ�� ġȯ�ϴ� �Լ�
void replaceLabel(FILE* inputFile, FILE* outputFile) {
    char buffer[SIZE];  // ���Ͽ��� �� �پ� ���� ����
    while (fgets(buffer, sizeof(buffer), inputFile)) {
        char* token = strtok(buffer, "\t\n,+[] ");  // �����ڷ� ���ڿ��� �и�
        while (token != NULL) {
            // ���̺� ó��: ':'�� ������ ��ū�� Ȯ���ϰ� ':' ����
            if (token[strlen(token) - 1] == ':') {
                // ':' ����
                token[strlen(token) - 1] = '\0';
            }

            // ���̺��� ã����, map���� �ش� �� ��ȣ�� ġȯ
            if (label_map.find(token) != label_map.end()) {
                const char* line_number = label_map[token];
                fprintf(outputFile, "%s ", line_number);  // ���̺��� ���ڷ� ġȯ
            }
            else {
                fprintf(outputFile, "%s ", token);  // ���̺��� �ƴϸ� �״�� ���
            }
            token = strtok(NULL, "\t\n,+[] ");  // ���� ��ū
        }
        fprintf(outputFile, "\n");
    }
}

