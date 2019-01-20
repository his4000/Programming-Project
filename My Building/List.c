#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"List.h"

void addSpace(Building* building, RoomNode* room, char* personName) {  //room이라는 방에 personName이라는 사람이 사는 칸 추가
	Space* tmp = (Space*)malloc(sizeof(Space));

	room->spaceList[room->p_number] = tmp;
	strcpy(room->spaceList[room->p_number]->personName, personName);
	room->p_number += 1;  //room 인원수 증가
	building->personNum += 1;  //building 인원수 증가
}

void removeSpace(Building* building, RoomNode* room, int cur)
{
	int i;
	for(i=cur;i<(room->p_number)-1;i++)
		room->spaceList[i] = room->spaceList[i+1];  //인원 방 당기기
	room->spaceList[i] = NULL;
	room->p_number--;  //room 인원 감소
	building->personNum--;  //building 인원 감소
}

void addRoom(Building* building, RoomList* room, int max, char* personName) {  
//room이라는 RoomList에 max인원만큼 살 수 있는 방 추가. 첫 입주자는 personName
	RoomNode *rn = (RoomNode*)malloc(sizeof(RoomNode));
	rn->max = max;  //방 제한 인원수 설정
	rn->p_number = 1;  //방 인원 초기화 -> 처음에는 한명만 입주
	building->personNum += 1;  //building 총 거주자 증가
	rn->spaceList[0] = (Space*)malloc(sizeof(Space));  //첫 칸 만들기
	strcpy(rn->spaceList[0]->personName, personName);  //personName 입주
	
	rn->r_next = room->r_trailer;
	rn->r_prev = room->r_trailer->r_prev;
	room->r_trailer->r_prev->r_next = rn;
	room->r_trailer->r_prev = rn;  
	//리스트에 추가
	building->roomNum += 1;  //room 갯수 증가
}

RoomList* setRoomList() {  //RoomList 초기화
	int i;

	RoomList *rl = (RoomList*)malloc(sizeof(RoomList));
	rl->r_header = (RoomNode*)malloc(sizeof(RoomNode));
	rl->r_trailer = (RoomNode*)malloc(sizeof(RoomNode));  
	//RoomList 구조체 초기화
	rl->r_header->max = 0;
	rl->r_header->p_number = 0;
	rl->r_header->r_prev = NULL;
	for (i = 0; i < MAX_SPACE; i++) {
		rl->r_header->spaceList[i] = NULL;
	}
	//RoomList Header 초기화
	rl->r_header->r_next = rl->r_trailer;  //header -> trailer

	rl->r_trailer->max = 0;
	rl->r_trailer->p_number = 0;
	rl->r_trailer->r_next = NULL;
	for (i = 0; i < MAX_SPACE; i++) {
		rl->r_trailer->spaceList[i] = NULL;
	}
	//RoomList Trailer 초기화
	rl->r_trailer->r_prev = rl->r_header;  //trailer -> header

	return rl;
}

void addFloor(FloorList* floor) {  //floor라는 FloorList에 층 추가
	FloorNode *newFloor = (FloorNode*)malloc(sizeof(FloorNode));  //새로운 층 할당

	newFloor->f_next = floor->f_trailer;
	newFloor->f_prev = floor->f_trailer->f_prev;
	floor->f_trailer->f_prev->f_next = newFloor;
	floor->f_trailer->f_prev = newFloor;
	//newFloor를 FloorList에 추가
	newFloor->f_temp = setRoomList();  //새로운 층에 첫 방 추가
}

void setFloor(FloorList* floor) {  //floor라는 FloorList 초기화
	floor->f_header = (FloorNode*)malloc(sizeof(FloorNode));
	floor->f_trailer = (FloorNode*)malloc(sizeof(FloorNode));
	//header와 trailer 할당
	floor->f_header->f_prev = NULL;
	floor->f_trailer->f_next = NULL;
	floor->f_header->f_temp = NULL;
	floor->f_trailer->f_temp = NULL;
	floor->f_header->f_next = floor->f_trailer;
	floor->f_trailer->f_prev = floor->f_header;
	//header와 trailer 초기화
}

void setBuilding(Building* mine, int* max, char* personName[]) {  
//mine이라는 building에 Building 초기화(max와 personName은 초기 방 3개 max값과 personName 배열)
	int i;	

	mine->floor = (FloorList*)malloc(sizeof(FloorList));  //FloorList 할당
	setFloor(mine->floor);
	addFloor(mine->floor);
	//첫 Floor 추가
	for (i = 0; i < INIT_ROOM; i++) {
		addRoom(mine, mine->floor->f_trailer->f_prev->f_temp, max[i], personName[i]);
	}  //초기 방 추가
}
