#pragma once

typedef struct ListNode { 	// 노드 타입
    int data;
    struct ListNode* link;
} ListNode;

void error(char* message);
ListNode* insert_first(ListNode* , int value);
ListNode* insert(ListNode* head, ListNode* pre, int value);
ListNode* delete_first(ListNode* head);
ListNode* delet(ListNode* head, ListNode* pre);
ListNode* find_and_del(ListNode* head, int find);
int count_node(ListNode* head);
void print_list(ListNode* head);