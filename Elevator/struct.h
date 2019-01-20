#include"queue.h"

#define AMT_ELE 3  //elevator 숫자

#define HEIGHT 10
#define U_GROUND 1
#define FLOOR (HEIGHT + U_GROUND)

/********elevator 구조체************/
typedef struct elevator{
	int nowFloor;  //현재 층
	int nowTarget;  //현재 목적층
	int nowDir;  //현재 방향. 아래면 -1, 위면 1, 정지면 0
	int nowAct;  //현재 수행하는 작업
	int division;  //사람이 타고 있는가
}Elevator;

/********building 구조체***********/
typedef struct building{
	Queue* workList;  //작업 리스트
	Elevator* elevator[AMT_ELE];  //elevator array
	int button[FLOOR];
}Building;
