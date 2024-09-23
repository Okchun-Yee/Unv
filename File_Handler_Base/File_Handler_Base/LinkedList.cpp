#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

// LinkedList start ==================================================================================================================== :)
typedef struct ListNode { 	// 노드 타입
    int data;
    struct ListNode* link;
} ListNode;

// 오류 처리 함수
void error(char* message)
{
    fprintf(stderr, "%s\n", message);
    exit(1);
}
ListNode* insert_first(ListNode* head, int value)
{
    ListNode* p = (ListNode*)malloc(sizeof(ListNode));	//(1)
    p->data = value;					// (2)
    p->link = head;	// 헤드 포인터의 값을 복사	//(3)
    head = p;	// 헤드 포인터 변경		//(4)
    return head;	// 변경된 헤드 포인터 반환
}

// 노드 pre 뒤에 새로운 노드 삽입
ListNode* insert(ListNode* head, ListNode* pre, int value)
{
    ListNode* p = (ListNode*)malloc(sizeof(ListNode));	//(1)
    p->data = value;		//(2)
    p->link = pre->link;	//(3)	
    pre->link = p;		//(4)	
    return head;		//(5)	
}

ListNode* delete_first(ListNode* head)
{
    ListNode* removed;
    if (head == NULL) return NULL;

    removed = head;	// (1)
    head = removed->link;	// (2)
    free(removed);		// (3)
    return head;		// (4)
}
// pre가 가리키는 노드의 다음 노드를 삭제한다. 
ListNode* delet(ListNode* head, ListNode* pre)
{
    ListNode* removed;
    removed = pre->link;
    pre->link = removed->link;		// (2)
    free(removed);		// (3)
    return head;			// (4)
}
//리스트에서 해당 데이터 찾아서 삭제
ListNode* find_and_del(ListNode* head, int find) {
    if (head->data == find) {
        head = delete_first(head);
        return head;
    }
    ListNode* now = head;
    for (ListNode* p = head; p != NULL; p = p->link) {
        if (p->link != NULL) now = p->link;
        if (now->data == find) {
            delet(head, p);
            return head;
        }
    }
    printf("%d는 존재하지 않습니다.\n", find);
    return head;
}
//노드 개수 파악 함수
int count_node(ListNode* head) {
    int cnt = 0;
    if (head == NULL) return 0;
    for (ListNode* p = head; p != NULL; p = p->link) cnt++;
    return cnt;
}
// 연결리스트 출력
void print_list(ListNode* head)
{
    for (ListNode* p = head; p != NULL; p = p->link)
        printf("%d->", p->data);
    printf("NULL \n");
}

// LinkedList End ==================================================================================================================== :)