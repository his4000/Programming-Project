#include<stdio.h>
#include"struct.h"

/************Main.c에 정의*************/
extern void Event(int,Building*);
extern void ele_menu();
extern int pause_exe;
extern int mode;
extern int q_print;

void console(char,int*,int*,Building*);  //console control
void ele_menu();  //elevator menu

