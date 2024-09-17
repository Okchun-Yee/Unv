#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#define SIZE 20

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

//열린 파일에서 문자열들을 읽어와서 배열에 저장
void File_Sort(FILE* fp) {
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
    for (int i = 0; i < index; i++) printf("before %d : %s\n",i, arr[i]);
    printf("====================================\n");
    //문자열 정렬 (사전순서)
    for (int i = 0; i < index - 1; i++) {
        for (int j = i + 1; j < index; j++) {
            //'strcmp'로 사전식 순서 변경하기
            if (strcmp(*(arr+i), *(arr + j)) > 0) {
                //포인터 직접교환
                char* temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
    // 배열에 저장된 단어 출력
    for (int i = 0; i < index; i++) printf("after %d : %s\n", i,arr[i]);
    // 동적 메모리 해제
    for (int i = 0; i < index; i++) {
        free(arr[i]);
    }
    return;
}

int main(void) {
    FILE* Mfp = NULL;
    const char* file_name = "input.txt";
    Mfp = File_Open(file_name);

    if (Mfp != NULL) {
        File_Sort(Mfp);
        fclose(Mfp);  // 파일 닫기
    }

    return 0;
}
