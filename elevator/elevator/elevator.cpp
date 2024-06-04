#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <string.h>

#define FLOOR 10	//층수
#define MAX_STACK_SIZE 10

typedef struct {
	char  stack[MAX_STACK_SIZE]; // 승객 배열 
	int  top;
}Stack;

void get_out(Stack*);
void moveElevator(Stack*, int*, int);

//elevator 승객
char alp[FLOOR] = { 'A','B','C','D','E','F','G','H','I','J'};
int alp_index = 0;

// ===== 스택 코드의 시작 ===== 
//스택 초기화 함수
void init(Stack* s) {
	s->top = -1;
	for (int i = 0; i < MAX_STACK_SIZE; i++)
		s->stack[i] = NULL;
}

// 공백 상태 검출 함수
int is_empty(Stack* s)
{
	return (s->top == -1);
}
// 포화 상태 검출 함수
int is_full(Stack* s)
{
	return (s->top == (MAX_STACK_SIZE - 1));
}
// 삽입 함수
void push(char item, Stack* s)
{
	if (is_full(s)) {
		fprintf(stderr, "elevator is full\n");
		return;
	}
	else s->stack[++(s->top)] = item;
}
// 삭제 함수
int pop(Stack* s)
{
	if (is_empty(s)) {
		fprintf(stderr, "스택 공백 에러\n");
		exit(1);
	}
	else return s->stack[(s->top)--];
}
// 피크 함수
int peek(Stack* s)
{
	if (is_empty(s)) {
		fprintf(stderr, "스택 공백 에러\n");
		exit(1);
	}
	else return s->stack[(s->top)];
}
// ===== 스택 코드의 끝 ===== 

void clearScreen() {
	Sleep(1000);
	system("cls");
}

//stack 출력 함수
void print_stack(Stack *s) {
	//스택에 있는 데이터 전부 출력
	if (!is_empty(s)) {
		for (int i = 0;i<=s->top; i++) {
			if (s->stack[i] == NULL) break;
			printf("| %c, ", s->stack[i]);
		}
		// 현재 탑승중인 인원 출력
		printf(" | \t=PEOPLE : %2d=",(s->top)+1);
	}
	//스택에 데이터가 없을 시 빈 공간 출력
	else printf("| |");
	return;
}
//elevator 상태 출력 함수
void print_elevator(Stack* s, int in_f, int out_f) {
	printf("엘리베이터 현재 층: %d\n", in_f);
	printf("목표 층: %d\n", out_f);
	//엘리베이터 상태 출력 반복문
	for (int i = FLOOR; i > 0; i--) {
		//매개변수와 같을시 현재 층수 + 스택 출력 
		if (i == in_f) {
			printf("[%2d]F : ", i);
			print_stack(s);
		}
		//매개변수와 다를시 현재 층수만 출력
		else printf("[%2d]F : ", i);
		printf("\n");
	}
}
void moveElevator(Stack* s, int *in_f, int out_f) {
	//현재 층수가 목표 층수와 다를 경우 반복
	while (*in_f != out_f) {
		clearScreen();
		print_elevator(s,*in_f, out_f);
		Sleep(500); // 0.5초 대기
		// 현재 층수를 목표 층수까지 1씩 증가 / 감소
		if (*in_f < out_f) {
			(*in_f)++;
		}
		else {
			(*in_f)--;
		}
	}
	// 목표 층수가 0이 아닐시에 탑승인원 하차
	if (out_f != 0) get_out(s);
	clearScreen();
	//하차한 인원 출력
	print_elevator(s,*in_f, out_f);
	printf("엘리베이터가 %d층에 도착했습니다.\n", out_f);
	Sleep(1000); // 1초 대기
}
//하차 처리 POP()
void get_out(Stack* s) {
	int num = 0;
	printf("\n몇명이 내립니까? : ");
	scanf("%d", &num);
	for (int i = 0; i < num; i++) {
		pop(s);
	}
}

int main(void) {
	Stack elevator;
	init(&elevator);

	int in_f = 1, out_f = 0;	//현재 층수 = 1, 목표 층수 변수
	char choice;	// 엘리베이터 반복 여부

	//층수 설정 후 탑승, 하차 층수 결정
	do {
		clearScreen();
		print_elevator(&elevator, in_f, out_f);
		//엘리베이터 탑승 종료
		if (in_f == 0) break;
		//목표 층수 설정
		printf("이동할 층 (1-10) : ");
		scanf("%d", &out_f);

		//1-10 사이까지만 입력
		if (out_f >10|| out_f <1) {
			printf("유효하지 않은 층입니다. 1-10 사이의 층을 입력하세요.\n");
			Sleep(500); // 0.5초 대기
		}
		else {
			int num = 0;
			printf("몇명이 탑승합니까? : ");
			scanf("%d", &num);
			//탑승 인원만큼 PUSH()
			for (int j = 0; j < num; j++) {
				if (alp[alp_index] == NULL) break;
				push(alp[alp_index++], &elevator);
			}
			moveElevator(&elevator, &in_f, out_f);
		}
		//모든 인원이 탑승한 경우 && 모든 인원이 비어있는 경우
		if (alp_index >= FLOOR && is_empty(&elevator)) break;
		//반복여부 결정
		printf("다른 층으로 이동하시겠습니까? (y/n): ");
		scanf("\n%c", &choice);
	} while (choice == 'y' || choice == 'Y');
	//모든 인원이 탑승 했는지 검사
	if (alp_index >= FLOOR) {
		printf("모든 사람이 이용하였습니다.\n");
	}
	else printf("감사합니다.\n");
	return 0;
}