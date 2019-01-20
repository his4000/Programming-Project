#include "queue.h"

void insert(Queue* q, int job)
{
	q->array[q->index] = job;//변수로 입력받은 job을 확보한 공간에 넣는다
	q->index++;//index값 증가
}

int out(Queue* q, int i)
{
	int j;//for문을 돌리기 위한 변수

	int tmp;//array에 저장된 job을 꺼내기 위한 임시변수
	tmp = q->array[i];//i번째(출력하고 싶은 job)을 tmp에 저장한 후
	for(j=i;j<q->index;j++)
		q->array[j] = q->array[j+1];//j+1번째 job을 j번째로 복사

	q->array[j] = 0;
	q->index--;  //index 감소

	return tmp;//임시변수에 저장된 i번째 job을 출력한다.
}

void setQueue(Queue* q)
{
	int i;
	
	for(i=0;i<SIZE;i++)
		q->array[i] = 0;  //queue 초기화
	
	q->index = 0;	
}

int isEmpty(Queue* q)
{
	if(q->index == 0)
		return 1;
	else
		return 0;
}

void print_ele(Queue* q, int idx, int first_floor, int pri_num)
{
	int work = q->array[idx];	
	int pri = abs(work / 100);  //우선순위
	int target = abs(work%100);  //목적층

	printf("| ");

	if(pri > pri_num)  //우선순위 높으면
	{
		printf("\x1b[31m");
		printf("\x1b[46m");
	}
	if(target > first_floor)  //지상
	{
		printf("%d",target);  //목적 층수 출력
		printf("F");
	}
	else  //지하
	{
		printf("B");
		printf("%d",pri_num+1-target);  //목적 층수 출력
	}
	if(work > 0)  //up
	{	
		printf("▲");		
	}
	else if(work < 0)  //down
	{
		printf("▼");
	}
	printf("\x1b[0m");
	printf(" |");
	
}
void print_q(Queue* q, int first_floor, int pri_num)
{
	int i;

	printf("workList : ");
	
	if(!isEmpty(q))
	{
		for(i=0;i<q->index;i++)
		{
			print_ele(q, i, first_floor, pri_num);	
			if(i % 8 == 7)
				printf("\n");
		}
	}
	printf("\n");
}
