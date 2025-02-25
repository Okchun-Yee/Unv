﻿#define _CRT_SECURE_NO_WARNINGS

#include "LinkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>

#define SIZE 20 //파일 크기
#define logB(x, base) log(x)/log(base)  //이진 탐색 함수 최대 시간
void string_file_search(char* arr[], const char*, int);
//콘솔창 지우기 : 1초 딜레이
void clear() {
    Sleep(1000);
    system("cls");
}

//파일 열기
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
void String_File_Sort(FILE* fp) {
    //파일에서 문자열을 읽어서 arr 포인터 배열에 저장
    char buffer[SIZE * 10];
    char* arr[SIZE];
    int index = 0;

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // 쉼표, 공백, 개행 문자를 기준으로 문자열 분리
        char* token = strtok(buffer, ", \n");
        while (token != NULL && index < SIZE) {
            // 문자열 크기만큼 동적 메모리 할당
            arr[index] = (char*)malloc(strlen(token) + 1);
            if (arr[index] == NULL) {
                printf("메모리 할당 실패!\n");
                // 할당된 메모리를 해제하고 종료
                for (int i = 0; i < index; i++) {
                    free(arr[i]);
                }
                return;
            }
            strcpy(arr[index], token);  // 토큰을 복사하여 저장
            index++;
            token = strtok(NULL, ", \n");
        }
    }
    for (int i = 0; i < index; i++) printf("before %d : %s\n", i, arr[i]);
    printf("====================================\n");
    //문자열 정렬 (사전순서)
    for (int i = 0; i < index - 1; i++) {
        for (int j = i + 1; j < index; j++) {
            //'strcmp'로 사전식 순서 변경하기
            if (strcmp(*(arr + i), *(arr + j)) > 0) {
                //포인터 직접교환
                char* temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
    // 배열에 저장된 단어 출력
    for (int i = 0; i < index; i++) printf("after %d : %s\n", i, arr[i]);

    string_file_search(arr, "Journey", index);
    // 동적 메모리 해제
    for (int i = 0; i < index; i++) {
        free(arr[i]);
    }
    return;
}
//Read Int FILE & Bubble Sort
void Int_File_sort(FILE* fp, ListNode** sp) {
    //=========파일 데이터 저장==========
    int num[SIZE] = { 0 };
    for (int i = 0; i < SIZE; i++)
        fscanf_s(fp, "%d", &num[i]);
    for (int i = 0; i < SIZE; i++) printf("input [%3d] : %d\n", i, num[i]);

    //=========버블 정렬==========
    int tmp = 0;
    for (int i = 0; i < SIZE; i++)
        for (int j = i; j < SIZE; j++)
            if (num[i] < num[j]) {
                tmp = num[j];
                num[j] = num[i];
                num[i] = tmp;
            }
    fclose(fp);
    printf("input 파일 닫음.\n");

    //정렬된 배열을 리스트에 삽입
    for (int i = 0; i < SIZE; i++) {
        *sp = insert_first(*sp, num[i]);
    }
}
//연결리스트 이진 탐색
int binary_search(ListNode* sp) {
    // 리스트 크기 판독
    int count = count_node(sp);

    int* parr = (int*)malloc(sizeof(int) * count); // 동적 메모리 할당 입력받은 사이즈 * int 크기만큼 
    if (parr == NULL) {
        printf("동적 메모리 할당 실패 !\n");
    }
    // 이 할당받은 메모리 시작 주소를 포인터에 대입
    else {
        int i = 0;
        for (ListNode* p = sp; p != NULL; p = p->link) {
            //리스트의 정렬된 데이터를 배열에 삽입
            parr[i++] = p->data;
        }
    }
    //for (int i = 0; i < count; i++) printf("file [%3d] : %d\n", i, *(parr + i));  // 동적 메모리에 저장된 값 출력

    //binary search key값 입력
    int key = 0;
    printf("KEY : ");
    scanf("%d", &key);
    if (key == 0) return 0;

    //binary search start
    int arr_index = count / 2;
    int top = count;
    int bottom = 0;
    //리스트에 key값이 없을 경우 종료 조건 = 이진 탐색 최대 시간
    int cnt = 0;

    int number = 0;
    while (true) {
        //탐색 실패 종료 조건
        if (arr_index < 0 || arr_index > count || cnt > logB(count, 2)) {
            printf("탐색 실패. 값이 없습니다.%d : %d\n", parr[arr_index], arr_index);
            break;
        }
        printf("NUM : %d = [%2d]\n", parr[arr_index], arr_index);
        Sleep(500);

        if (key > parr[arr_index]) {
            bottom = arr_index;
            arr_index = (arr_index + top) / 2;
            cnt++;
        }
        else if (key < parr[arr_index]) {
            top = arr_index;
            arr_index = (arr_index + bottom) / 2;
            cnt++;
        }
        else if (key == parr[arr_index]) {
            printf("SUCCESS : %d\nSUCCESS : %d번째\nSUCCESS : %dCycle.\n", parr[arr_index], arr_index + 1, cnt);
            number = parr[arr_index];
            //window_clear();
            //찾은 경우 = 해당 숫자를 배열에서 빼고 배열 한칸씩 당기기
            break;
        }
    }
    free(parr); // 사용 완료한 동적메모리 할당 해제
    return number;
}

void string_file_search(char* parr[], const char* key,int index) {
    int bottom = 0;
    int arr_index = index / 2;
    int top = index-1;
    int cnt = 1;

    while (true) {
        if (arr_index < 0 || arr_index > index || cnt > logB(index, 2)) {
            printf("탐색 실패. 값이 없습니다.%s : %d\n", parr[arr_index], arr_index);
            break;
        }
        printf("NUM : %s = [index : %2d]\n", parr[arr_index], arr_index);
        Sleep(500);

        if (strcmp(parr[arr_index], key) == 1) {
            top = arr_index;
            arr_index = (arr_index + bottom) / 2;
            cnt++;        }
        else if(strcmp(parr[arr_index], key) == -1) {
            bottom = arr_index;
            arr_index = (arr_index + top) / 2;
            cnt++;        }
        else if(strcmp(parr[arr_index], key) == 0) {
            printf("SUCCESS : %s\nSUCCESS : %d번째\nSUCCESS : %dCycle.\n", parr[arr_index], arr_index + 1, cnt);
            break;
        }
    }
}

int main(void)
{
    //INT FILEL
    ListNode* head = NULL;
    FILE* int_Mfp;
    
    const char* int_file_name = "input.txt"; //열 파일 지정
    int_Mfp = File_Open(int_file_name);    //파일 열기 Part
    Int_File_sort(int_Mfp, &head);
    
    //이진 탐색 & 탐색값 삭제하기
    while (head != NULL) {
        clear();
        //삭제할 값 지정하기
        print_list(head);
        int key = binary_search(head);
        if (key == 0) return 0;
        head = find_and_del(head, key);
    }

    free(head); //연결리스트 해제
    fclose(int_Mfp);    //정수 파일 닫기

    //STRING FILE
    FILE* string_Mfp = NULL;
    //const char* string_file_name = "input.txt";  //열 파일 지정
   //string_Mfp = File_Open(string_file_name);  //파일 열기 Part

    //파일 공백 검사
    if (string_Mfp != NULL) {
        //NULL이 아닐시 실행
        String_File_Sort(string_Mfp);
        fclose(string_Mfp);  // 파일 닫기
    }
    return 0;
}