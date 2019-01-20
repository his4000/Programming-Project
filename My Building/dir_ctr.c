#include"List.h"

void building_cursor(Building*, int*, int*);  //building view console control
void space_cursor(Building*, RoomNode*, int*);  //room view console control

/*********Main.c에 정의*************/
extern void print_building(Building*, int, int);
extern RoomNode* getRoomNode(Building*, int, int);
extern void building_menu(Building*);
extern void room_menu(Building*, RoomNode*);
extern void space_menu(Building*, RoomNode*, int*);
/***********************************/

/******building view control*******/
void building_cursor(Building* building, int* x, int* y)
{
	int max_x, max_y;  //현재 층과 해당 층의 방 수 최댓값
	int cur = 0;  //room view에서 cursor 위치. 0으로 초기화
	char key = 0;  //key board 입력 변수

	while(1)
	{
		max_y = count_floor(building);  //최대 층수 계산
		while(key != 10 && key != 32)
		{
			max_x = count_room(building, (*y)+1);  //해당 층에서 최대 방수 계산
			print_building(building, *x, *y);  //building view print
			key = kms_getch();  //keyboard input
			if(key == 27)  //방향키
			{
				key = kms_getch();
				if(key == 91)
				{
					key = kms_getch();
					switch(key)
					{
						case 65 :  //위쪽
						if((*y)<max_y-1 && getRoomNode(building, *x, (*y)+1) != NULL)  //위층에 방이 없으면 
							(*y)++;
						break;
	
						case 66 :  //아래쪽
						if((*y)>0)
							(*y)--;
						break;
	
						case 67 :  //오른쪽
						if((*x)<max_x-1)
							(*x)++;
						break; 
						
						case 68 :  //왼쪽
						if((*x)>0)
							(*x)--;
						break;
					}
				}
			}
		}
	
		if(key == 32)  //spacebar
		{
			system("clear");
			building_menu(building);  //building menu print
			key = 0;
		}
		if(key == 10)  //enter
		{
			system("clear");
			space_cursor(building, getRoomNode(building, *x, *y), &cur);  //room view print
			key = 0;
		}
	}
}

/********room view control*********/
void space_cursor(Building* building, RoomNode* r_node, int* cur)
{
	int mode = r_node->p_number;  //방 형태(몇 칸인가)
	char key = 0;
	char member[MAX_SPACE][MAX_SIZE];  //room member List
	int i;

	while(1)
	{
		mode = r_node->p_number;  //room 형태 정보 갱신
		for(i=0;i<mode;i++)
		{
			strncpy(member[i], r_node->spaceList[i]->personName, MAX_SIZE);  //member List 만들기 & 갱신	
		}
		while(key != 10 && key != 32 && key != 127)
		{
			print_space(mode, member, *cur);  //room view print
			key = kms_getch();  //keyboard input
			if(key == 27)  //방향키
			{
				key = kms_getch();
				if(key == 91)
				{
					key = kms_getch();
					if(mode == 2)  //room에 칸이 2개일 때
					{
						switch(key)
						{
							case 67 :   //오른쪽
							if((*cur) == 0)
								(*cur) = 1;
							break;
							case 68 :  //왼쪽
							if((*cur) == 1)
								(*cur) = 0;
							break;
						}
					}
				
					if(mode == 3)  //room에 칸이 3개일 때
					{
						switch(key)
						{
							case 65 :  //위쪽
							if((*cur) == 2)
								(*cur) = 0;
							break;
							case 66 :  //아래쪽
							if((*cur) == 0 || (*cur) == 1)
								(*cur) = 2;
							break;
							case 67 :  //오른쪽
							if((*cur) == 0 || (*cur) == 2)
								(*cur) = 1;
							break;
							case 68 :  //왼쪽
							if((*cur) == 1)
								(*cur) = 0;
							break;
						}
					}
					if(mode == 4)  //room에 칸이 4개일 때
					{
						switch(key)
						{
							case 65 :  //위쪽
							if((*cur) == 2)
								(*cur) = 0;
							else if((*cur) == 3)
								(*cur) = 1;
							break;
							case 66 :  //아래쪽
							if((*cur) == 0)
								(*cur) = 2;
							else if((*cur) == 1)
								(*cur) = 3;
							break;
							case 67 :  //오른쪽
							if((*cur) == 0)
								(*cur) = 1;
							else if((*cur) == 2)
								(*cur) = 3;
							break;
							case 68 :  //왼쪽
							if((*cur) == 1)
								(*cur) = 0;
							else if((*cur) == 3)
								(*cur) = 2;
							break;
						}
					}
				}
			}
		}
		if(key == 32)  //spacebar
		{
			system("clear");
			room_menu(building, r_node);  //room menu print
			key = 0;
		}
		if(key == 10)  //enter
		{
			system("clear");
			space_menu(building, r_node, cur);
			key = 0;
		}
		if(key == 127)  //backspace
			return;
	}
}
