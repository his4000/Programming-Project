#include<stdio.h>
#include<string.h>
#include"kms_menu.h"
#include"List.h"

void building_cursor(Building*, int*, int*);
void space_cursor(Building*, RoomNode*, int*);
RoomNode* getRoomNode(Building*, int, int);
//void change_member(Space*);

void building_cursor(Building* building, int* x, int* y)
{
	int max_x, max_y;
	int cur = 0;
	char key = 0;

	while(1)
	{
		max_y = count_floor(building);
		while(key != 10 && key != 32)
		{
			max_x = count_room(building, (*y)+1);
			print_building(building, *x, *y);
			key = kms_getch();
			if(key == 27)
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
	
		if(key == 32)
		{
			system("clear");
			building_menu(building);
			key = 0;
		}
		if(key == 10)
		{
			system("clear");
			space_cursor(building, getRoomNode(building, *x, *y), &cur);
			key = 0;
		}
	}
}

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
			print_space(mode, member, *cur);
			key = kms_getch();
			if(key == 27)
			{
				key = kms_getch();
				if(key == 91)
				{
					key = kms_getch();
					if(mode == 2)
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
					if(mode == 3)
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
					if(mode == 4)
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
		if(key == 32)
		{
			system("clear");
			room_menu(building, r_node);
			key = 0;
		}
		if(key == 10)
		{
			change_member(r_node->spaceList[*cur]);
			key = 0;
		}
		if(key == 127)
			return;
	}
}
