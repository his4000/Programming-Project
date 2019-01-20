#include"dir_ctr.h"

void console(char key, int* cursor, int* select, Building* building)
{
	int up = (building->button[(*cursor)])/10;  //up key가 눌러졌는가
	int down = (building->button[(*cursor)])%10;  //down key가 눌러졌는가

	switch(key)
	{
		case 65 :  //Up Key 
		if((*select) != FLOOR)  //방향키 입력 모드이면
		{
			if((*select) != FLOOR-1)  //꼭대기 층이 아니면
			{
				if(!up)  //up key가 안 눌러져 있으면
				{
					if(mode == 0)  //Normal Mode일때
					{
						building->button[(*cursor)] += 10;
						Event((*cursor)+100, building);  //새로운 작업	
					}
					else if(mode == 1)  //VIP Mode일때
					{
						building->button[(*cursor)] += 1000;
						Event((*cursor)+1000, building);  //vip작업
					}
				}
			}
			(*select) = FLOOR;  //방향키 입력 모드 빠져나가기
		}
		else if((*cursor)<HEIGHT)
			(*cursor)++;
		break;
				
		case 66 :  //Down Key
		if((*select) != FLOOR) 
		{
			if((*select) != 0)  //가장 아래층이 아니면
			{
				if(!down)  //down key가 안 눌러져 있으면
				{
					if(mode == 0)
					{
						building->button[(*cursor)] += 1;
						Event((-1)*((*cursor)+100),building);  //새로운 작업
					}
					else if(mode == 1)
					{
						building->button[(*cursor)] += 100;
						Event((-1)*((*cursor)+1000),building);  //vip 작업
					}
				}
			}
			(*select) = FLOOR;  //방향키 입력 모드 빠져나가기
		}
		else if((*cursor)>((-1)*U_GROUND + 1))
			(*cursor)--;
		break;

		case 10 :   //Enter
		(*select) = (*cursor);
		break;
	
		case 27 :   //ESC
		if((*select) != FLOOR)  //방향키 입력 모드이면
			(*select) = FLOOR;  //방향키 입력 모드 빠져나가기
		else  //방향키 입력 모드가 아니면
			ele_menu();
		break;
		
		case 32 :   //Spacebar
		pause_exe = 1;
		break;
		
		case 'q' :  //q가 눌러졌을때
		q_print = !q_print;  //q_print 반전
		break;
	}
}
