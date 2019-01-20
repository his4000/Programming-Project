#ifndef H_COLOR
#define H_COLOR
#include<stdio.h>
#include<string.h>

#define VERSION 1  //0 검정 배경  1 흰 배경

typedef enum color{BLACK = 30, RED, GREEN, YELLOW, BLUE, PINK, SKY, GRAY}COLOR;

void kms_scolor(const char str[], const COLOR f, const COLOR b)
{
	char fcolor[] = "\x1b[";
	char bcolor[] = "\x1b[";
	char tmp[3];

	sprintf(tmp,"%d",f);
	strcat(tmp,"m");
	strcat(fcolor,tmp);  //instr 완성

	sprintf(tmp,"%d",b+10);
	strcat(tmp,"m");
	strcat(bcolor,tmp);  //instr 완성

	printf("%s",fcolor);  //font setting
	printf("%s",bcolor);  //background setting
	printf("%s",str);

	printf("\x1b[0m");  //color cancel		
}
void kms_icolor(const int i, const COLOR f, const COLOR b)
{
	char fcolor[] = "\x1b[";
	char bcolor[] = "\x1b[";
	char tmp[3];

	sprintf(tmp,"%d",f);
	strcat(tmp,"m");
	strcat(fcolor,tmp);  //instr 완성
	
	sprintf(tmp,"%d",b+10);
	strcat(tmp,"m");
	strcat(bcolor,tmp);  //instr 완성

	printf("%s",fcolor);  //font setting
	printf("%s",bcolor);  //background setting
	printf("%d",i);
	
	printf("\x1b[0m");  //color cancel
}
#endif
