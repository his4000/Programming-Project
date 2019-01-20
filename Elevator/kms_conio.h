#ifndef H_CONIO
#define H_CONIO
#include<stdio.h>
#include<termios.h>
#include<unistd.h>
#include<fcntl.h>

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

char kms_putch(const int ch)
{
	printf("%c",ch);

	return (char)ch;
}

char kms_getche(void)
{
	int get;
	
	get = kms_getch();
	kms_putch(get);

	return (char)get;
}

void kms_wait(const char opt)
{
	while(kms_getch() != opt);
}

int kms_kbhit(void)
{
	struct termios oldt, newt;
	int ch;
	int oldf;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;

	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO,F_GETFL,0);
	fcntl(STDIN_FILENO,F_SETFL, oldf | O_NONBLOCK);

	ch = kms_getch();
	if(ch == 27)
	{
		ch = kms_getch();
		ch = kms_getch();
		if(ch == -1)  //ESC key가 눌리면
			ch = 27;
	}

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	
	if(ch!=EOF)
	{
		return ch;
	}

	return 0;
}
#endif
