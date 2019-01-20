#include<stdio.h>
#include<string.h>
#include"kms_menu.h"
#include"List.h"

void print_space(int mode, char member[mode][MAX_SIZE], int);  //Room View 출력
void print_room(Building*, int, int ,int);  //Building View 출력
void print_line(Building*, int, int, int);  //Building View에서 각 층 출력
void print_building(Building*, int, int);  //Building View 출력

RoomNode* getRoomNode(Building*, int, int);  //(x, y)에 있는 room 접근
void changeName(Space*, char*);  //space에 있는 멤버 이름 변경
int count_room(Building*, int);  //Building 특정 층에 있는 room 수 계산
int count_floor(Building*);  //Building의 총 층수 계산

void change_member(Space*);  //space에 있는 멤버 변경
void add_new_member(Building*, RoomNode*);  //room에 새로운 멤버 추가
void add_new_room(Building*);  //Building에 새로운 방 추가
void add_new_floor(Building*);  //Building에 새로운 층 추가
void remove_member(Building*, RoomNode*, int*);  //member delete

void building_menu(Building*);  //Building mode menu
void room_menu(Building*, RoomNode*);  //Room mode menu
void space_menu(Building*, RoomNode*, int*);  //Space mode menu

/****************dir_ctr.c에 정의******************/
extern void building_cursor(Building*, int*, int*);
extern void space_cursor(Building*, RoomNode*, int*);
/**************************************************/

int main(void)
{
	Building* building = (Building*)malloc(sizeof(Building));  //Building Allocation
	int i,j;
	int x=0, y=0;  //좌표
	int init_max[3] = {4,4,4};  //초기 세개의 방 max값 초기화
	char* init_name[3] = {"Kim","Jung","Song"};  //초기 세개의 방 멤버 이름

	setBuilding(building, init_max, init_name);  //Building Setting

	building_cursor(building, &x, &y);  //Building View 불러오기

	return 0;

}
void print_space(int mode, char member[mode][MAX_SIZE], int cur)
{
	system("clear");

	switch(mode)
	{
		case 1 :   //칸이 1개
			kms_scolor(member[0], BLACK, GRAY);
			break;
		case 2 :   //칸이 2개
			if(cur == 0)
			{
				kms_scolor(member[0], BLACK, GRAY);
				printf("\t");;
				printf("%s",member[1]);
			}
			else if(cur == 1)
			{
				printf("%s",member[0]);
				printf("\t");
				kms_scolor(member[1], BLACK, GRAY);
			}
			break;
		case 3 :   //칸이 3개
			if(cur == 0)
			{
				kms_scolor(member[0], BLACK, GRAY);
				printf("\t");
				printf("%s",member[1]);
				printf("\n\n%s",member[2]);
			}
			else if(cur == 1)
			{
				printf("%s",member[0]);
				printf("\t");
				kms_scolor(member[1],BLACK,GRAY);
				printf("\n\n%s",member[2]);
			}
			else if(cur == 2)
			{
				printf("%s",member[0]);
				printf("\t");
				printf("%s",member[1]);
				printf("\n\n");
				kms_scolor(member[2], BLACK, GRAY);
			}
			break;
		case 4 :  //칸이 4개
			if(cur == 0)
			{
				kms_scolor(member[0],BLACK,GRAY);
				printf("\t");
				printf("%s\n\n",member[1]);
				printf("%s\t",member[2]);
				printf("%s",member[3]);
			}
			else if(cur == 1)
			{
				printf("%s\t",member[0]);
				kms_scolor(member[1], BLACK, GRAY);
				printf("\n\n%s\t",member[2]);
				printf("%s",member[3]);
			}
			else if(cur == 2)
			{
				printf("%s\t%s\n\n",member[0], member[1]);
				kms_scolor(member[2],BLACK,GRAY);
				printf("\t%s",member[3]);
			}
			else if(cur == 3)
			{
				printf("%s\t%s\n\n",member[0], member[1]);
				printf("%s\t",member[2]);
				kms_scolor(member[3],BLACK,GRAY);
			}
			break;
	}
	printf("\n\n");
	printf("===========================\n");
	kms_scolor("Enter : Member Menu",YELLOW,BLACK);
	printf("\n");
	kms_scolor("Spacebar : Room Menu",YELLOW,BLACK);
	printf("\n");
	kms_scolor("Backspace : Building View",YELLOW,BLACK);
	printf("\n");
	printf("===========================\n");
}

void print_room(Building* building, int x, int y, int select)
//room print. select = 0이면 음영x, select = 1이면 음영
{
	int room_num = 100*(y+1) + (x+1);

	if(select == 1)
		kms_icolor(room_num, BLACK, GRAY);  //음영
	else
		printf("%4d",room_num);  //음영x
}

void print_line(Building* building, int floor, int cur_x, int select)
//line print, select = 0이면 음영x, select = 1이면 음영
{
	int i;
	int max = count_room(building, floor);  //현재 층의 최대 room 수

	for(i=0;i<max;i++)
		printf("┌----┐");
	printf("\n");

	for(i=0;i<max;i++)
	{
		if(select == 1 && i == cur_x)
		{
			printf("|");
			print_room(building, i, floor-1, 1);
			printf("|");
		}
		else
		{
			printf("|");
			print_room(building, i, floor-1, 0);
			printf("|");
		}
	}
	printf("\n");	

	for(i=0;i<max;i++)
		printf("└----┘");
	printf("\n");
}

void print_building(Building* building, int cur_x, int cur_y)
{
	int i;
	int max_floor = count_floor(building);  //building의 현재 최대 층수

	system("clear");
	for(i = max_floor;i>0;i--)
	{
		if(cur_y+1 == i)  //현재 층에 커서가 있으면
			print_line(building, i, cur_x, 1);
		else
			print_line(building, i, cur_x, 0);
	}
	printf("\n");
	printf("===========================\n");
	printf("현재 총 거주자 수 : %d\n",building->personNum);
	printf("현재 총 방의 수 : %d\n",building->roomNum);
	printf("===========================\n");
	printf("\n");
	printf("===========================\n");
	kms_scolor("Enter : Room View",YELLOW,BLACK);
	printf("\n");
	kms_scolor("Spacebar : Building Menu",YELLOW,BLACK);
	printf("\n");
	printf("===========================\n");
}

RoomNode* getRoomNode(Building* building, int x, int y)
{
	FloorNode* f_tmp;
	RoomNode* r_tmp;
	int i, j;
	int max_floor = count_floor(building);  //현재 building의 최대 층 수 
	int max_room = count_room(building, y+1);  //해당 층의 최대 방수

	if(y+1 > max_floor)  //접근하려는 층이 max_floor 이상이라면
		return NULL;
	if(x+1 > max_room)  //접근하려는 방이 max_room 이상이라면
		return NULL;

	f_tmp = building->floor->f_header->f_next;  //1층 접근

	for(i=0;i<y;i++)
		f_tmp = f_tmp->f_next;  //해당 층 도착까지 접근
	r_tmp = f_tmp->f_temp->r_header->r_next;  //해당 층 첫번째 방 접근
	for(j=0;j<x;j++)
		r_tmp = r_tmp->r_next;  //해당 방 접근

	return r_tmp;
}

void changeName(Space* sp, char* name)
{
	strncpy(sp->personName, name, MAX_SIZE);  //space에 거주하는 거주자 이름 name으로 변경
}

int count_room(Building* building, int floorNum)
{
	FloorNode* tmp;  //검색 floor Node
	RoomNode* n_tmp;  //검색 room Node
	int i, count = 0;
	int max_floor = count_floor(building);  //최대 층 수

	if(!(floorNum <= max_floor && floorNum > 0))  //현재 존재하지 않는 층에 접근하면
		return 0;

	tmp = building->floor->f_header->f_next;  //첫번째 층 접근
	
	for(i=0;i<floorNum-1;i++)
		tmp = tmp->f_next;  //해당 층 접근
	n_tmp = tmp->f_temp->r_header->r_next;  //해당 층 첫번째 방 접근

	while(n_tmp->r_next != NULL)
	{
		n_tmp = n_tmp->r_next;
		count++;  //해당 층 방 갯수 counting
	}

	return count;
}

int count_floor(Building* building)
{
	FloorNode* f_tmp;  //검색 floor Node
	int i, count = 0;

	f_tmp = building->floor->f_header->f_next;  //첫번째 층 접근

	while(f_tmp->f_next != NULL)
	{
		f_tmp = f_tmp->f_next;
		count++;  //층 갯수 counting
	}

	return count;
}

void change_member(Space* space)
{
	char newMem[MAX_SIZE];  //새로운 멤버 이름

	system("clear");

	printf("새롭게 바꿀 멤버의 이름을 입력하세요 : ");
	scanf("%s",newMem);
	while(getchar() != '\n');  //stdin buffer 비우기
	strncpy(space->personName, newMem, MAX_SIZE);  //member change
}

void add_new_member(Building* building, RoomNode* r_node)
{
	char newMem[MAX_SIZE];  //new member name

	system("clear");

	if(r_node->p_number == r_node->max)  //방이 꽉 차면
	{
		printf("방이 꽉 찼습니다.\n");
		kms_wait(10);  //entre 입력 기다림
		return;
	}
	printf("새 멤버의 이름을 입력하세요 : ");
	scanf("%s",newMem);
	while(getchar() != '\n');  //stdin buffer 비우기

	addSpace(building, r_node, newMem);  //새로운 space 추가
}

void add_new_room(Building* building)
{
	int max_floor = count_floor(building);  //building의 최대 층수
	int floor;  //추가할 방의 층
	char newMem[MAX_SIZE];  //new member name
	int max;  //추가할 방의 정원
	FloorNode* f_tmp = building->floor->f_header->f_next;  //층에 접근할 임시 floor node. 1층으로 초기화
	int i;
	int cur_floor_count, down_floor_count;  //선택된 층수와 그 아랫층
	
	do
	{
		system("clear");
		printf("몇 층에 추가하시겠습니까? ");
		scanf("%d",&floor);
		while(getchar() != '\n');  //stdin buffer 비우기

		if(!(floor>0 && floor<=max_floor))  //입력 error
		{
			printf("최대 층은 %d층 입니다.",max_floor);
			kms_wait(10);  //enter 입력까지 기다림
		}
	}while(!(floor>0 && floor <= max_floor));

	cur_floor_count = count_room(building, floor);  //현재 층수에 존재하는 방 갯수
	down_floor_count = count_room(building, floor-1);  //아랫층에 존재하는 방 갯수

	if(cur_floor_count == down_floor_count)
	{
		system("clear");
		printf("아랫층 방보다 많은 방을 추가할 수 없습니다.\n");
		kms_wait(10);  //enter 입력 기다림
		return;
	}

	for(i=1;i<floor;i++)
		f_tmp = f_tmp->f_next;  //해당 층 접근

	system("clear");
	printf("새 멤버의 이름을 입력하세요 : ");
	scanf("%s",newMem);
	while(getchar() != '\n');  //stdin buffer 비우기

	do
	{
		system("clear");
		printf("최대 방 인원을 설정하세요 : ");
		scanf("%d",&max);
		while(getchar() != '\n');  //stdin buffer 비우기

		if(!(max>0 && max<=MAX_SPACE))
		{
			printf("제한인원은 1~%d명 입니다.",MAX_SPACE);
			kms_wait(10);  //enter 입력 기다림
		}
	}while(!(max>0 && max<=MAX_SPACE));

	addRoom(building, f_tmp->f_temp, max, newMem);  //새로운 방 추가
	
	system("clear");
	printf("새로운 방이 추가되었습니다.\n");
	kms_wait(10);  //enter 입력 기다림
}

void add_new_floor(Building* building)
{
	char newMem[MAX_SIZE];  //new member name
	int max;  //추가할 방의 정원
	int i;
	int floor;  //추가된 층은 몇층인가
	FloorNode* f_tmp = building->floor->f_header->f_next;  //추가된 층에 접근한 floor node. 1층으로 초기화

	addFloor(building->floor);  //새로운 층 추가

	floor = count_floor(building);  //building의 층수

	for(i=1;i<floor;i++)
		f_tmp = f_tmp->f_next;  //해당 층 접근

	system("clear");
	printf("새 멤버의 이름을 입력하세요 : ");
	scanf("%s",newMem);
	while(getchar() != '\n');  //stdin buffer 비우기

	do
	{
		system("clear");
		printf("최대 방 인원을 설정하세요 : ");
		scanf("%d",&max);
		while(getchar() != '\n');  //stdin buffer 비우기

		if(!(max>0 && max<=MAX_SPACE))
		{
			printf("제한인원은 1~%d명 입니다.",MAX_SPACE);
			kms_wait(10);  //enter 입력 기다림
		}
	}while(!(max>0 && max<=MAX_SPACE));

	addRoom(building, f_tmp->f_temp, max, newMem);  //새로운 방 추가

	system("clear");
	printf("새로운 층이 추가되었습니다.\n");
	kms_wait(10);  //enter 입력 기다림
}

void remove_member(Building* building, RoomNode* r_node, int* cur)
{
	if(r_node->p_number == 1)
	{
		system("clear");
		printf("방 인원이 1명이므로 삭제할 수 없습니다.\n");
		kms_wait(10);
	}
	else
	{
		removeSpace(building, r_node, *cur);
		(*cur) = 0;
	}
}
void building_menu(Building* building)
{
	char *b_menu[] = {"1. 새로운 층 추가", "2. 새로운 방 추가", "3. Building View로 돌아가기", "4. Program 종료"};  //menu list
	int menu;

	menu = kms_menu(b_menu, 4);  //menu 선택

	switch(menu)
	{
		case 0 : 
			add_new_floor(building);  //층 추가
			break;
		case 1 : 
			add_new_room(building);  //방 추가	
			break;
		case 3 :
			exit(0);  //program 종료
	}
}

void room_menu(Building* building, RoomNode* r_node)
{
	char* r_menu[] = {"1. New Member 입주", "2. Room View로 돌아가기"};  //menu list
	int menu;

	menu = kms_menu(r_menu, 2);  //menu 선택

	if(menu == 0)
		add_new_member(building, r_node);  //새로운 멤버 추가
}

void space_menu(Building* building, RoomNode* r_node, int* cur)
{
	char* s_menu[] = {"1. Member 변경", "2. Member 퇴거", "3. Room View로 돌아가기"};  //menu list
	int menu;

	menu = kms_menu(s_menu, 3);  //menu 선택

	switch(menu)
	{
		case 0 : 
			change_member(r_node->spaceList[*cur]);  //멤버 변경
			break;
		case 1 : 
			remove_member(building, r_node, cur);  //space delete
			break;
	}
}
