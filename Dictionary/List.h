#include<stdio.h>
#include<string.h>

#define SIZE 50  //Node data size

typedef struct Node{  //Node 구조체 정의
	struct NODE* link;
	char data[SIZE];
}NODE;

NODE* insert_node(NODE*, NODE*, char str[]);  //node 삽입
NODE* delete_node(NODE*, NODE*);  //node 삭제
NODE* search_node(NODE*, char str[]);  //node 검색
