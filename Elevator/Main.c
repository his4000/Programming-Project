#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>
#include<time.h>
#include"struct.h"
#include"kms_menu.h"

#define P_NUM 1  //몇번 씹혀야 우선순위가 최우선이 되나
#define VIP_NUM 1000  //VIP 작업 우선순위 식별 숫자

#define SPEED 50000  //속도
#define SENSITIVE 1000  //민감도
#define T_SLICE SPEED/SENSITIVE

#define CCTV_SLICE 4000  //CCTV 속도 단위
#define N_CCTV_SLICE 100  //CCTV 민감도

#define PASSWORD "aaaa"
#define PW_SIZE 20

void setBuilding(Building*);  //building 초기화
void setEle(Elevator*);  //elevator 초기화

void stopEle(Elevator*);  //elevator stop
void movingEle(Elevator*, int);  //nowAct값으로 elevator 움직이기
int selectWork(Queue*,int*);  //workList Queue에서 작업 선택
int randAct(int);  //random으로 층수 누르기(elevator 내부)
void arriveEle(Building*, int);  //elevator가 도착 했을때 해야할 작업
int noWork(Building*,int);  //작업이 없이 놀고있는 elevator index를 1번부터 우선으로 return
void Event(int, Building*);  //새로운 event가 발생했을때 해야할 작업
void newWork(Building*, int);  //새로운 작업을 elevator에 넣기

void ele_menu();  //elevator menu 출력

void up(Elevator*);  //elevator 올리기
void down(Elevator*);  //elevator 내리기
void print_building(Building*,int*,int*,int);  //building view 출력
void print_eleMark(Elevator*,int);  //elevator mark print
void moving(Building*);  //움직이기
void working(int*, int*, Building*,FILE*);  //프로그램 running

void storeRecord(Building*,int,int,int,FILE*);  //binary file로 저장
void loadRecord(Building*,int*,int*,FILE*);  //binary file load
void CCTV();  //CCTV 모드
void print_exit_CCTV();  //CCTV 종료 화면

int PW();  //password 입력 함수

void Ele_Key_Manual();  //elevator 단축키 설명 출력
void CCTV_Key_Manual();  //CCTV 단축키 설명 출력

void start_view();  //시작 화면

void move_cur(int,int);  //cursor 이동
void clr_screen();  //clear screen

int pause_exe = 0;  //실행이 멈췄는지 알려주는 변수
int mode = 0;  //Normal Mode = 0, VIP Mode = 1, CCTV = 2
int q_print = 0;  //Queue를 출력 할 것인가

int main(void)
{
	int cursor = 0;
	int sel = FLOOR;
	Building* building = (Building*)malloc(sizeof(Building));
	int i;
	int tmp_mode;
	FILE* sfile;  //저장 파일

	srand(time(NULL));

	start_view();

	setBuilding(building);

	sfile = fopen("elevator.dat","wb");
	fclose(sfile);
	while(1)
	{	
		sfile = fopen("elevator.dat","ab");
		while(mode != 2)
		{
			tmp_mode = mode;  //mode 임시 저장
			working(&cursor, &sel, building,sfile);
		}
		fclose(sfile);
		CCTV();  //CCTV 재생
		mode = tmp_mode;  //mode 되돌리기
	}

	return 0;
}

void setBuilding(Building* building)
{
	int i;

	building->workList = (Queue*)malloc(sizeof(Queue));

	for(i=0;i<AMT_ELE;i++)
		building->elevator[i] = (Elevator*)malloc(sizeof(Elevator));

	setQueue(building->workList);  //workList Setting

	for(i=0;i<FLOOR;i++)
		building->button[i] = 0;

	for(i=0;i<AMT_ELE;i++)
		setEle(building->elevator[i]);  //elevator 초기화
}

void setEle(Elevator* ele)
{
	int i;
	
	stopEle(ele);
	ele->nowFloor = 0;
	ele->division = 0;
}

void stopEle(Elevator* ele)
{
	ele->nowTarget = -1;
	ele->nowDir = 0;
	ele->nowAct = 0;
}

void movingEle(Elevator* ele, int Act)
{
	int ActFloor = abs(Act%100);
	
	ele->nowAct = Act;  //작업 설정
	ele->nowTarget = ActFloor;  //elevator 목적층 설정

	if(ele->nowFloor > ActFloor)
		ele->nowDir = -1;  //elevator는 아래로
	else if(ele->nowFloor < ActFloor)
		ele->nowDir = 1;  //elevator는 위로
	else  //elevator가 딱 그 위치면
	{	
		if(Act > 0)  //up work
			ele->nowDir = 1;  
		else  //down work
			ele->nowDir = -1;
	}
}

int selectWork(Queue* q, int* idx)
{
	int work;  //작업 임시 저장소
	int outAct;  //out을 위한 변수
	int i;  //i와 j는 for문을 위한 변수, z는 삭제를 위한 변수

	for(i = 0;i < q->index;i++)
	{
		work = q->array[i];  //작업 불러오기

		if(abs(work) >= VIP_NUM)  //VIP 작업이면
		{
			outAct = work;  //내보낼 작업은
			(*idx) = i;
			return outAct;
		}
	}
	for(i = 0;i < q->index;i++)
	{
		work = q->array[i];  //작업 불러오기	
	
		if(abs(work / 100) > P_NUM)  //우선순위가 높으면
		{
			outAct = work;  //내보낼 작업은 
			(*idx) = i;
			return outAct;
		}
	}
	outAct = q->array[0];
	(*idx) = 0;

	return outAct;
}
int randAct(int work)  //아래 랜덤과 위 랜덤을 따로
{
	int Act;  //목적층 결정
	int target = abs(work % 100);  //도착한 층

	if(work < 0)  //아래로를 눌렀으면
		Act = rand() % target;
	else if(work > 0) //위로를 눌렀으면
		Act = (rand() % (FLOOR-target-1)) + target + 1;

	Act = Act + 10000;  //안에서 누른 작업임을 알림
	
	return Act;
}

void arriveEle(Building* building, int index)
{
	int arriveFloor;
	Elevator* ele = building->elevator[index];
	
	arriveFloor = ele->nowTarget;  //목적층
	if(ele->nowFloor == arriveFloor)
	{
		if(ele->division == 0)  //elevator에 사람이 타고있지 않았으면
		{
			//button off
			if(abs(ele->nowAct) >= 1000)  //VIP Mode
			{
				if(ele->nowAct > 0)  //up button
					building->button[ele->nowFloor] -= 1000;
				else if(ele->nowAct < 0)  //down button
					building->button[ele->nowFloor] -= 100;
			}
			else
			{
				if(ele->nowAct > 0)  //up button
					building->button[ele->nowFloor] -= 10;
				else if(ele->nowAct < 0)  //down button
					(building->button[ele->nowFloor])--;
			}
			ele->division = 1;  //사람 탑승

			movingEle(ele, randAct(ele->nowAct));  //elevator 내부에서 random 층 누르기
		}
		else
		{
			ele->division = 0;  //사람 내림
			if(isEmpty(building->workList))  //대기하고 있는 새로운 작업이 없음
				stopEle(ele);  //stop elevator
			else
				newWork(building, index);  //새로운 작업 workList에서 할당
		}
	}
}
int noWork(Building* building, int Act)
{
	int ele_num = -1;
	int i;
	int dist = FLOOR;  //거리차
	int ActFloor = abs(Act % 100);  //Act의 층
	int tmp;
	Elevator* ele;

	for(i=AMT_ELE-1;i>=0;i--)
	{
		ele = building->elevator[i];

		if(ele->nowAct == 0)  //elevator가 작업이 없으면
		{
			tmp = abs(ele->nowFloor - ActFloor);
			if(tmp <= dist )
			{	
				dist = tmp;	
				ele_num = i;  //해당 index return
			}
		}
	}	
	
	return ele_num;
}

void Event(int newEvent, Building* building)
{
	int back;
	int search;
	int tmp,dist;  //차이가 얼마나 나는지를 나타내기 위한 변수
	int newEventFloor;
	int i, idx=-1, work;
	Elevator* ele;

	work = noWork(building,newEvent);
	if(work != -1)  //노는 elevator가 있으면
		movingEle(building->elevator[work], newEvent);  //우선 할당
	else
	{
		back = newEvent;
		newEventFloor = abs(newEvent % 100);  //새로운 event의 층

		if(abs(newEvent) >= VIP_NUM)  //VIP 작업이면
		{
			tmp = 100;

			for(i=0;i<AMT_ELE;i++)
			{
				ele = building->elevator[i];

				if(ele->division == 0)  //사람이 안타고 있으면
				{
					dist = abs(newEventFloor - ele->nowFloor);  //event 층과 거리차
					if(tmp > dist)
					{
						idx = i;  //최소거리 elevator index 저장
						tmp = dist;  //최소거리 저장
					}
				}
			}
			if(idx != -1)  //적합한 elevator를 찾으면
			{
				back = building->elevator[idx]->nowAct;  //작업 되돌리기
				if(back > 0)  //back이 up이면
					back += 100;  //우선순위 올리기
				else if(back < 0)  //back이 down이면
					back -= 100;
				movingEle(building->elevator[idx], newEvent);  //newEvent 새로운 작업으로
			}
		}
		else
		{
			for(i=0;i<AMT_ELE;i++)
			{
				ele = building->elevator[i];
	
				if(abs(ele->nowAct/100) < (P_NUM+1) && ele->division == 0)  //우선순위 작업이 아니고 비어있으면
				{
					tmp = newEventFloor - (ele->nowFloor);  //event floor와 현재 층 거리 비교
					search = (ele->nowTarget) - (ele->nowFloor);  //elevator target층과 현재 층 거리 비교
					search = abs(search);
	
					if(tmp*ele->nowDir >= 0)  //elevator의 현재 방향쪽에서 newEvent가 발생
					{
						tmp = abs(tmp);
		
						if(tmp < search)  //newEvent가 더 적합하다
						{
							back = ele->nowAct;  //기존 작업 되돌리고
							if(back > 0)  //back이 up이면
								back += 100;  //기존 작업 우선순위 올려주기
							else if(back < 0)
								back -= 100;
							movingEle(building->elevator[i], newEvent);  //newEvent를 새로운 작업으로
							break;  //선택받으면 빠져나가기
						}
					}
				}
			}
		}

		insert(building->workList, back);  //선택받지 못한 작업을 workList queue에 넣기
	}
}

void newWork(Building* building, int index)
{
	int idx;

	movingEle(building->elevator[index], selectWork((building->workList),&idx));
	out(building->workList, idx);  //workList에서 작업 제거
}

void ele_menu()
{
	int password;  //비밀번호 통과 여부
	char* list[4] = {"Normal Mode","VIP Mode","CCTV","Program 종료"};
	mode = kms_menu(list,4);
	if(mode == 1)  //VIP Mode 이면
	{
		password = PW();
		if(password == 0)  //비밀번호 입력 포기
			ele_menu();
	}
}
void print_eleMark(Elevator* ele, int floor)
{
	int div = ele->division;  //사람이 타고 있는가	
	char tmp[10];  //elevator 출력을 임시 저장
	char frag[10];	

	if(ele->nowFloor == floor)
	{
		if(ele->nowAct == 0)  //elevator에 작업이 없으면
		{
			printf("| nop | ");
			return;
		}
		if(ele->nowDir > 0)  //elevator가 위로 향하면
		{
			if(ele->nowTarget >= U_GROUND)  //지상층이 Target일때
			{
				if(ele->nowTarget-(U_GROUND-1) >= 10)  //10보다 높은 층 Target
				{
					strncpy(tmp, "|↑", 10);
					sprintf(frag,"%d",ele->nowTarget-(U_GROUND-1));
					strcat(tmp,frag);
					strcat(tmp,"F |");
				}
				else  //10보다 낮은층 Target
				{
					strncpy(tmp, "| ↑", 10);
					sprintf(frag,"%d",ele->nowTarget-(U_GROUND-1));
					strcat(tmp,frag);
					strcat(tmp,"F |");
				}
			}
			else  //지하층이 Target일때
			{
					strncpy(tmp, "| ↑B", 10);
					sprintf(frag,"%d",U_GROUND-(ele->nowTarget));
					strcat(tmp,frag);
					strcat(tmp," |");
			}
		}	
		else  //elevator가 아래로 향하면
		{
			if(ele->nowTarget >= U_GROUND)
			{
				if(ele->nowTarget-(U_GROUND-1) >= 10)  //10보다 높은 층 Target
				{
					strncpy(tmp, "|↓", 10);
					sprintf(frag,"%d",ele->nowTarget-(U_GROUND-1));
					strcat(tmp,frag);
					strcat(tmp,"F |");
				}
				else
				{
					strncpy(tmp, "| ↓", 10);
					sprintf(frag,"%d",ele->nowTarget-(U_GROUND-1));
					strcat(tmp,frag);
					strcat(tmp,"F |");
				}
			}
			else
			{
					strncpy(tmp, "| ↓B", 10);
					sprintf(frag,"%d",U_GROUND-(ele->nowTarget));
					strcat(tmp,frag);
					strcat(tmp," |");
			}
		}
		if(div == 1)
			if(VERSION == 0)
				kms_scolor(tmp,RED,BLUE);
			else
				kms_scolor(tmp,RED,SKY);
		else
			printf("%s",tmp);
	}
	else
		printf("        ");
	printf(" ");
}
void print_building(Building* building, int* cursor, int* sel, int p_mode)
{
	int i,k;

	clr_screen();
	
	printf("========================================\n");
	switch(p_mode)
	{
		case 0 : 
			if(VERSION == 0)
				kms_scolor("        < N o r m a l   M o d e >\n",YELLOW,BLACK);
			else
				kms_scolor("        < N o r m a l   M o d e >\n",RED,0);
			break;
		case 1 :
			if(VERSION == 0)
				kms_scolor("           < V I P   M o d e >\n",YELLOW,BLACK);
			else
				kms_scolor("           < V I P   M o d e >\n",RED,0);
			break;
		case 2 : 
			if(VERSION == 0)
				kms_scolor("          < C C T V   M o d e >\n",YELLOW,BLACK);
			else
				kms_scolor("          < C C T V   M o d e >\n",RED,0);
			break;
	}
	printf("========================================\n\n");

	for(i=FLOOR-1;i>U_GROUND-1;i--)
	{
		printf("----------------------------------------\n");
		printf("   ");
		if((*sel) == i)
		{
			kms_icolor(i-U_GROUND+1, YELLOW, BLUE);
			kms_scolor("F", YELLOW, BLUE);
			printf(" ");
		}
		else if((*cursor) == i)
		{
			if(VERSION == 0)
			{
				kms_icolor(i-U_GROUND+1,BLACK,GRAY);
				kms_scolor("F",BLACK,GRAY);
			}
			else
			{
				kms_icolor(i-U_GROUND+1,BLACK,GREEN);
				kms_scolor("F",BLACK,GREEN);
			}
			printf(" ");
		}
		else
			printf("%dF ",i-U_GROUND+1);
		if(i>9)
			printf("| ");
		else
			printf(" | ");
		if(i == FLOOR-1)  //꼭대기 층이면
		{
			if(building->button[i] == 0)
				printf(" ▽ ");
			else if(building->button[i] == 1)  //Normal Mode
				printf(" ▼ ");
			else if(building->button[i] == 100)  //VIP Mode
			{
				if(VERSION == 0)
					kms_scolor(" ▼ ",YELLOW,BLACK);
				else
					kms_scolor(" ▼ ",SKY,0);
			}
		}

		else  //꼭대기 층이 아니면
		{
			if(building->button[i] == 0)
				printf("△ ▽");
			//Normal Mode
			else if(building->button[i] == 10)
				printf("▲ ▽");
			else if(building->button[i] == 1)
				printf("△ ▼");
			else if(building->button[i] == 11)
				printf("▲ ▼");
			//VIP Mode
			else if(building->button[i] == 1000)
			{
				if(VERSION == 0)
					kms_scolor("▲",YELLOW,BLACK);
				else
					kms_scolor("▲",SKY,0);
				printf(" ▽");
			}
			else if(building->button[i] == 100)
			{
				printf("△ ");
				if(VERSION == 0)
					kms_scolor("▼",YELLOW,BLACK);
				else
					kms_scolor("▼",SKY,0);
			}
			else if(building->button[i] == 1100)
				if(VERSION == 0)
					kms_scolor("▲ ▼",YELLOW,BLACK);
				else
					kms_scolor("▲ ▼",SKY,0);
			//Normal + VIP
			else if(building->button[i] == 1001)
			{
				if(VERSION == 0)
					kms_scolor("▲",YELLOW,BLACK);
				else
					kms_scolor("▲",SKY,0);
				printf(" ▼");
			}
			else if(building->button[i] == 110)
			{
				printf("▲ ");
				if(VERSION == 0)
					kms_scolor("▼",YELLOW,BLACK);
				else
					kms_scolor("▼",SKY,0);
			}
		}
		printf(" | ");
		for(k=0;k<AMT_ELE;k++)
			print_eleMark(building->elevator[k], i);
		printf("\n");
	}
	for(i=U_GROUND-1;i>=0;i--)
	{
		printf("----------------------------------------\n");
		printf("   ");
		if((*sel) == i)
		{
			kms_scolor("B",YELLOW,BLUE);
			kms_icolor(U_GROUND-i,YELLOW,BLUE);
			printf(" ");
		}
		else if((*cursor) == i)
		{
			if(VERSION == 0)
			{
				kms_scolor("B",BLACK,GRAY);
				kms_icolor(U_GROUND-i,BLACK,GRAY);
			}
			else
			{
				kms_scolor("B",BLACK,GREEN);
				kms_icolor(U_GROUND-i,BLACK,GREEN);
			}
			printf(" ");
		}
		else
			printf("B%d ",U_GROUND-i);
		printf(" | ");
		if(i == 0)  //맨 아래층이면
		{
			if(building->button[i] == 0)
				printf(" △ ");
			else if(building->button[i] == 10)  //Normal Mode
				printf(" ▲ ");
			else if(building->button[i] == 1000)  //VIP Mode
			{
				if(VERSION == 0)
					kms_scolor(" ▲ ",YELLOW,BLACK);
				else
					kms_scolor(" ▲ ",SKY,0);
			}
		}
		else  //맨 아래층이 아니면
		{
			if(building->button[i] == 0)
				printf("△ ▽");
			//Normal Mode
			else if(building->button[i] == 10)
				printf("▲ ▽");
			else if(building->button[i] == 1)
				printf("△ ▼");
			else if(building->button[i] == 11)
				printf("▲ ▼");
			//VIP Mode
			else if(building->button[i] == 1000)
			{
				if(VERSION == 0)
					kms_scolor("▲",YELLOW,BLACK);
				else
					kms_scolor("▲",SKY,0);
				printf(" ▽");
			}
			else if(building->button[i] == 100)
			{
				printf("△ ");
				if(VERSION == 0)
					kms_scolor("▼",YELLOW,BLACK);
				else
					kms_scolor("▼",SKY,0);
			}
			else if(building->button[i] == 1100)
			{
				if(VERSION == 0)
					kms_scolor("▲ ▼",YELLOW,BLACK);
				else
					kms_scolor("▲ ▼",SKY,0);
			}
			//Normal + VIP
			else if(building->button[i] == 1001)
			{
				if(VERSION == 0)
					kms_scolor("▲",YELLOW,BLACK);
				else
					kms_scolor("▲",SKY,0);
				printf(" ▼");
			}
			else if(building->button[i] == 110)
			{
				printf("▲ ");
				if(VERSION == 0)
					kms_scolor("▼",YELLOW,BLACK);
				else
					kms_scolor("▼",SKY,0);
			}
		}
		printf(" | ");
		for(k=0;k<AMT_ELE;k++)
			print_eleMark(building->elevator[k], i);
		printf("\n");
	}
	printf("----------------------------------------\n\n");
	if(!(mode != 0 && mode != 1))  //elevator view mode이면
		Ele_Key_Manual();
	else if(mode == 2)  //CCTV view mode이면
		CCTV_Key_Manual();
	
	if(q_print == 1 && mode != 2)  //queue 출력 모드이면
		print_q(building -> workList, U_GROUND-1, P_NUM);  //queue 출력
	if(pause_exe == 1)
	{
		if(VERSION == 0)
			kms_scolor("<-------------- P A U S E -------------->",YELLOW,0);
		else
			kms_scolor("<-------------- P A U S E -------------->",BLUE,0);
		kms_wait(32);  //멈춤
		pause_exe = 0;  //재 실행
	}

}

void up(Elevator* ele)
{
	(ele->nowFloor)++;  //층 증가
}

void down(Elevator* ele)
{
	(ele->nowFloor)--;  //층 감소
}

void moving(Building* building)
{
	int i;
	Elevator* ele;

	for(i=0;i<AMT_ELE;i++)
	{
		ele = building->elevator[i];

		if(ele->nowFloor == ele->nowTarget)
			arriveEle(building, i);
		if(ele->nowAct)  //일이 있을때만
		{
			if(ele->nowDir > 0)  //up 상태
				up(ele);
			else if(ele->nowDir < 0)  //down 상태
				down(ele);
		
		}
		if(ele->nowFloor >= FLOOR)  //범위를 위로 벗어나면
		{
			insert(building->workList, ele->nowAct);  //작업 되돌리기
			stopEle(ele);  //정지
			ele->nowFloor--;  //층 내리기
		}
		else if(ele->nowFloor < 0)  //범위를 아래로 벗어나면
		{
			insert(building->workList, ele->nowAct);  //작업 되돌리기
			stopEle(ele);  //정지
			ele->nowFloor++;  //층 올리기
		}
	}
}
void working(int* cursor, int* sel, Building* building, FILE* sfile)
{
	int i;
	int key = 0;
	int work;  //새로운 작업
	int empty = -1;  //노는 elevator index
	int dump;

	print_building(building, cursor, sel, mode);
	storeRecord(building,*cursor,*sel,0,sfile);
	if(!isEmpty(building->workList))  //작업이 있으면
	{
		work = selectWork(building->workList,&dump);  //작업 색출
		empty = noWork(building, work);  //노는 elevator 색출
	}
	if(empty != -1)  //노는 elevator가 있으면
		newWork(building, empty);  //작업 할당

	moving(building);
	for(i=0;i<SENSITIVE;i++)
	{
		usleep(T_SLICE);
		key = kms_kbhit();
		if(!(key != 'q' && key != 10 && key != 65 && key != 66 && key != 27 && key != 32))
		{
			console(key, cursor, sel, building);
			key = 0;
			if(mode == 3)
			{
				fclose(sfile);
				exit(0);
			}
			print_building(building, cursor, sel, mode);
			storeRecord(building,*cursor,*sel,1,sfile);
		}
	}
}

void storeRecord(Building* building, int cursor, int sel, int loc, FILE* sfile)
{
	Queue buf_q;
	Elevator buf_e[AMT_ELE];
	Elevator* ele;
	int i;
	
	/********workList Copy**********/
	for(i=0;i<SIZE;i++)
		buf_q.array[i] = building->workList->array[i];
	buf_q.index = building->workList->index;
	
	/********Elevator Copy**********/
	for(i=0;i<AMT_ELE;i++)
	{
		ele = building->elevator[i];
		buf_e[i].nowFloor = ele->nowFloor;
		buf_e[i].nowTarget = ele->nowTarget;
		buf_e[i].nowDir = ele->nowDir;
		buf_e[i].nowAct = ele->nowAct;
		buf_e[i].division = ele->division;
	}

	/************Building 구조체 저장*****************/
	fwrite(&buf_q,sizeof(Queue),1,sfile);  //workList 저장
	fwrite(buf_e,sizeof(Elevator),AMT_ELE,sfile);  //elevator 저장
	fwrite(building->button,sizeof(int),FLOOR,sfile);  //button 저장
		
	/************나머지 Data 저장***************/
	fwrite(&cursor,sizeof(int),1,sfile);  //cursor 저장
	fwrite(&sel,sizeof(int),1,sfile);  //sel 저장
	/************record location 저장**********/
	fwrite(&loc,sizeof(int),1,sfile);
}

void CCTV()
{
	FILE* f;
	int i, b_cursor, b_sel;
	int loc1, loc2;
	int key;  //입력 key
	Building* building = (Building*)malloc(sizeof(Building));  //building instance 생성
	
	building->workList = (Queue*)malloc(sizeof(Queue));  //workList allocation
	
	for(i=0;i<AMT_ELE;i++)
		building->elevator[i] = (Elevator*)malloc(sizeof(Elevator));  //elevator allocation
	
	f = fopen("elevator.dat","rb");
	loadRecord(building,&b_cursor,&b_sel,f);
	fread(&loc1,sizeof(int),1,f);
	print_building(building,&b_cursor,&b_sel,mode);
	while(!feof(f))
	{
		loadRecord(building,&b_cursor, &b_sel,f);
		fread(&loc2,sizeof(int),1,f);

		if(loc1 != loc2)
		{	
			for(i=0;i<N_CCTV_SLICE/2;i++)
			{
				usleep(CCTV_SLICE);
				key = kms_kbhit();
				if(key == 32)  //spacebar input
				{
					if(VERSION == 0)
						kms_scolor("<--------- C C T V   P A U S E --------->",YELLOW,0);
					else
						kms_scolor("<--------- C C T V   P A U S E --------->",BLUE,0);
					key = 0;  //key 값 지우기
					kms_wait(32);
				}
				else if(key == 27)  //ESC input
					return;
			}
		}
		else
		{
			for(i=0;i<N_CCTV_SLICE;i++)
			{
				usleep(CCTV_SLICE);
				key = kms_kbhit();
				if(key == 32)  //spacebar input
				{
					if(VERSION == 0)
						kms_scolor("<--------- C C T V   P A U S E --------->",YELLOW,0);
					else
						kms_scolor("<--------- C C T V   P A U S E --------->",BLUE,0);
					key = 0;
					kms_wait(32);
				}
				else if(key == 27)  //ESC input
					return;
			}
		}
		print_building(building,&b_cursor,&b_sel,mode);
		loc1 = loc2;
	}
	fclose(f);

	print_exit_CCTV();
}
void loadRecord(Building* building, int* b_cursor, int* b_sel, FILE* f)
{
	int i;

	fread(building->workList,sizeof(Queue),1,f);  //workList load
	for(i=0;i<AMT_ELE;i++)
		fread(building->elevator[i],sizeof(Elevator),1,f);  //elevator load
	fread(building->button,sizeof(int),FLOOR,f);  //button load
	
	fread(b_cursor,sizeof(int),1,f);
	fread(b_sel,sizeof(int),1,f);
}
void print_exit_CCTV()
{
	system("clear");
	printf("\n\n");
	printf("==================================\n\n");
	printf("          E N D   C C T V\n");
	printf("           <press Enter>\n\n");
	printf("==================================");
	kms_wait(10);
}

int PW()
{
	char input[PW_SIZE];  //password 입력 공간
	char buf;  //password 임시 저장 공간
	int index = 0;  //저장된 비밀번호 자릿수
	int result = 0;  //비밀번호 일치여부
	int i;
	
	do
	{
		system("clear");

		printf("\n\n========= P A S S W O R D   S Y S T E M ===========\n");
		if(VERSION == 0)  //black background
			printf("\n     (비밀번호는 aaaa입니다 조교님^^)\n");
		else
			printf("\n");
		printf("     비밀번호를 입력하세요 : ");
		while(1)
		{
			buf = kms_getch();
			if(buf != 27 && buf != 127 && buf != 10 && index<20)  //enter입력 전이면
			{
				kms_putch('*');  //*출력
				input[index++] = buf;  //비밀번호 입력
			}
			else if(buf == 27)  //ESC 입력
				return result;
			else if(buf == 127)  //backspace 입력
			{
				printf("\b \b");
				input[--index] = '\0';
			}
			else if(buf == 10)
			{
				input[index] = '\0';  //문자열 종료
				if(strncmp(input, PASSWORD, PW_SIZE) == 0)  //비밀번호 일치
				{
					result = 1;
					break;
				}
				else
				{
					printf("\n\n     비밀번호가 틀렸습니다(Press Enter)\n");
					kms_wait(10);
					for(i=0;i<PW_SIZE;i++)  //input 지우기
					{
						input[i] = '\0';
						index = 0;
						buf = 0;
					}
					break;
				}
			}
		}
	}while(result == 0);

	return result;
}
void Ele_Key_Manual()
{
	if(VERSION == 0)  //black background
	{
		kms_scolor("=============== 단 축 키 ===============\n",YELLOW,0);
		kms_scolor("   E  S  C  :  MENU\n",YELLOW,0);
		kms_scolor("   SpaceBar :  PAUSE On/Off\n",YELLOW,0);
		kms_scolor("      q     :  대기 작업리스트 On/Off\n",YELLOW,0);
		kms_scolor("========================================\n",YELLOW,0);
	}
	else  //white background
	{
		kms_scolor("=============== 단 축 키 ===============\n",BLUE,0);
		kms_scolor("   E  S  C  :  MENU\n",BLUE,0);
		kms_scolor("   SpaceBar :  PAUSE On/Off\n",BLUE,0);
		kms_scolor("      q     :  대기 작업리스트 On/Off\n",BLUE,0);
		kms_scolor("========================================\n",BLUE,0);
	}
}
void CCTV_Key_Manual()
{
	if(VERSION == 0)  //black background
	{
		kms_scolor("=============== 단 축 키 ===============\n",YELLOW,0);
		kms_scolor("   E  S  C  :  CCTV 종료\n",YELLOW,0);
		kms_scolor("   SpaceBar :  PAUSE On/Off\n",YELLOW,0);
		kms_scolor("========================================\n",YELLOW,0);
	}
	else
	{
		kms_scolor("=============== 단 축 키 ===============\n",BLUE,0);
		kms_scolor("   E  S  C  :  CCTV 종료\n",BLUE,0);
		kms_scolor("   SpaceBar :  PAUSE On/Off\n",BLUE,0);
		kms_scolor("========================================\n",BLUE,0);
	}
}

void start_view()
{
	system("clear");
	printf("\n\n\n\n");
	printf("       ■■■■■  ■      ■■■■■  ■   ■    ■    ■■■■■   ■■■   ■■■■\n");
	printf("       ■      ■      ■      ■   ■   ■ ■     ■    ■   ■  ■   ■\n");
	printf("       ■■■■   ■      ■■■■    ■ ■   ■■■■■    ■    ■   ■  ■■■■\n");
	printf("       ■      ■      ■       ■ ■   ■   ■    ■    ■   ■  ■   ■\n");
	printf("       ■■■■■  ■■■■■  ■■■■■    ■    ■   ■    ■     ■■■   ■   ■\n");
	printf("\n\n");
	printf("  ■■■■■  ■■■  ■   ■  ■   ■  ■        ■    ■■■■■  ■■■   ■■■   ■   ■\n");
	printf("  ■       ■   ■■ ■■  ■   ■  ■       ■ ■     ■     ■   ■   ■  ■■  ■\n");
	printf("  ■■■■■   ■   ■ ■ ■  ■   ■  ■      ■■■■■    ■     ■   ■   ■  ■ ■ ■\n");
	printf("      ■   ■   ■ ■ ■  ■   ■  ■      ■   ■    ■     ■   ■   ■  ■  ■■\n");
	printf("  ■■■■■  ■■■  ■   ■   ■■■   ■■■■■  ■   ■    ■    ■■■   ■■■   ■   ■\n");
	printf("\n\n\n");
	printf("                      ============================\n\n");
	printf("                        2016 Programming Project \n\n");
	printf("                                 Team 7\n\n");
	printf("                      ============================\n");

	if(VERSION == 0)  //black background
		kms_scolor("\n\n                        < Press Enter to Start >\n",YELLOW,0);
	else
		kms_scolor("\n\n                        < Press Enter to Start >\n",BLUE,0);
	kms_wait(10);
}
void move_cur(int x, int y)
{
	printf("\033[%dd\033[%dG",y,x);
}
void clr_screen()
{
	int i;

	move_cur(0,0);
		
	for(i=0;i<50;i++)
		printf("                                                                    \n");

	move_cur(0,0);
}
