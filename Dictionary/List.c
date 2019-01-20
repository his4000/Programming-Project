#include"List.h"

NODE* insert_node(NODE* plist, NODE* pprev, char str[])  //node 삽입
{
	NODE *pnew = NULL;  //삽입할 node 선언

	if(!(pnew = (NODE*)malloc(sizeof(NODE))))
	{
		printf("Memory Dynamic Allocation Error!\n");
		exit(0);
	}

	strncpy(pnew->data, str, SIZE);  //data 값 입력
	if(pprev == NULL)  //List 처음에 삽입
	{
		if(plist == NULL)  //List가 비어 있을때
		{
			plist = pnew;
			plist->link = NULL;
		}
		else
		{
			pnew->link = plist;
			plist = pnew;
		}
	}
	else  //pprev node 뒤에 삽입
	{
		pnew->link = pprev->link;
		pprev->link = pnew;
	}
	return plist;  //수정된 list return
}

NODE* delete_node(NODE* plist, NODE* pprev)  //node 삭제
{
	NODE* pcurr = NULL;  //target Node

	if(pprev == NULL)  //List의 처음을 삭제
	{
		pcurr = plist;
		plist = pcurr->link;
	}
	else  //pprev node 뒤의 node를 삭제
	{
		pcurr = pprev->link;
		pprev->link = pcurr->link;
	}

	free(pcurr);  //node 공간 반환
	return plist;  //수정된 list return
}

NODE* search_node(NODE* plist, char str[])  //List에서 str을 data로 가진 node 검색
{
	NODE* search = plist->link;  //검색 Node pointer 초기화
	
	do
	{
		if(strncmp(search->data, str, SIZE) == 0)  //일치
			return search;
		else
			search = search->link;  //다음 node 접근
	}while(search->link != NULL);

	return NULL;  //없으면 NULL return
}
