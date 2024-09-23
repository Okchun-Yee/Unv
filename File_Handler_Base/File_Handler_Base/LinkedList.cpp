#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

// LinkedList start ==================================================================================================================== :)
typedef struct ListNode { 	// ��� Ÿ��
    int data;
    struct ListNode* link;
} ListNode;

// ���� ó�� �Լ�
void error(char* message)
{
    fprintf(stderr, "%s\n", message);
    exit(1);
}
ListNode* insert_first(ListNode* head, int value)
{
    ListNode* p = (ListNode*)malloc(sizeof(ListNode));	//(1)
    p->data = value;					// (2)
    p->link = head;	// ��� �������� ���� ����	//(3)
    head = p;	// ��� ������ ����		//(4)
    return head;	// ����� ��� ������ ��ȯ
}

// ��� pre �ڿ� ���ο� ��� ����
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
// pre�� ����Ű�� ����� ���� ��带 �����Ѵ�. 
ListNode* delet(ListNode* head, ListNode* pre)
{
    ListNode* removed;
    removed = pre->link;
    pre->link = removed->link;		// (2)
    free(removed);		// (3)
    return head;			// (4)
}
//����Ʈ���� �ش� ������ ã�Ƽ� ����
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
    printf("%d�� �������� �ʽ��ϴ�.\n", find);
    return head;
}
//��� ���� �ľ� �Լ�
int count_node(ListNode* head) {
    int cnt = 0;
    if (head == NULL) return 0;
    for (ListNode* p = head; p != NULL; p = p->link) cnt++;
    return cnt;
}
// ���Ḯ��Ʈ ���
void print_list(ListNode* head)
{
    for (ListNode* p = head; p != NULL; p = p->link)
        printf("%d->", p->data);
    printf("NULL \n");
}

// LinkedList End ==================================================================================================================== :)