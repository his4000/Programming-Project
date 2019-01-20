#ifndef H_MENU
#define H_MENU
#include<stdio.h>
#include"kms_conio.h"
#include"kms_color.h"

/**********menu를 출력하고 그 입력을 받아오는 함수**************/
int kms_menu(const char** list, const int n)
{
	int select = 0;  //선택된 메뉴
	char key = 0;  //keyboard input

	while(key!=10)
	{
		kms_set(list, n, select);  //menu print
		key = kms_getch();  //keyboard input
		if(key == 27)  //방향키
		{
			key = kms_getch();
			if(key == 91)
			{
				key = kms_getch();
				switch(key)
				{
					case 65 :  //up 
						if(select == 0)
							select = n-1;
						else
							select--;
						break;
					case 66 :   //down
						if(select == n-1)
							select = 0;
						else
							select++;
						break;
				}
			}
		}
	}
	system("clear");
	return select;  //선택된 menu 반환
}

/***********menu print******************/
void kms_set(const char** str, const int n, const int sel)
{
	int i;

	system("clear");
	for(i=0;i<n;i++)
	{
		if(i == sel)
		{
			kms_scolor(str[i],BLACK,GRAY);
			printf("\n");
		}
		else
		{
			printf("%s\n",str[i]);
		}
	}
}
#endif
