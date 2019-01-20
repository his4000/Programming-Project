#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define SIZE 20

typedef struct queue{
	int array[SIZE];  //작업이 들어갈 array
	int index;  //다음 insert될 index 값
}Queue;

void insert(Queue*, int);  //Queue에 작업 추가
int out(Queue*, int);  //index i에 있는 작업 제거
void setQueue(Queue*);  //Queue setting
int isEmpty(Queue*);  //Queue가 비어있는가
void print_ele(Queue*,int,int,int);  //Queue의 element 출력
void print_q(Queue*,int,int);  //Queue 출력
