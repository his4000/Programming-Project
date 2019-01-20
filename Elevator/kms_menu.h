#ifndef H_MENU
#define H_MENU
#include<stdio.h>
#include"kms_conio.h"
#include"kms_color.h"

int kms_menu(const char** list,const int n)
{
	int select = 0;
	char key = 0;

	while(key!=10)
	{
		kms_set(list, n, select);
		key = kms_getch();
		if(key == 27)
		{
			key = kms_getch();
			if(key == 91)
			{
				key = kms_getch();
				switch(key)
				{
					case 65 : 
						if(select == 0)
							select = n-1;
						else
							select--;
						break;
					case 66 : 
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
	return select;
}

void kms_set(const char** str, const int n, const int sel)
{
	int i;

	system("clear");
	printf("\n\n==============메뉴를 선택하세요=============\n\n");
	for(i=0;i<n;i++)
	{
		if(i == sel)
		{
			printf("   ");
			if(VERSION == 0)
				kms_scolor(str[i],BLACK,GRAY);
			else
				kms_scolor(str[i],BLACK,GREEN);
			printf("\n\n");
		}
		else
		{
			printf("   %s\n\n",str[i]);
		}
	}
	printf("============================================\n");
}
#endif
