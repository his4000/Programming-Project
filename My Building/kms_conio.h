#ifndef H_CONIO
#define H_CONIO
#include<stdio.h>
#include<termios.h>
#include<unistd.h>
#include<fcntl.h>

/*********************************/
/*Cygwin에서 getch 함수 구현을 위해
 *인용해온 코드 입니다.***********/ 
int kms_getch(void)
{
	struct termios oldt,
	newt;
	int ch;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO,TCSANOW,&newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
}

/********opt가 입력될 때까지 wait********/
void kms_wait(const char opt)
{
	while(kms_getch() != opt);
}
#endif
