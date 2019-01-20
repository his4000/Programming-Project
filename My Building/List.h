#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 20  //멤버 이름 size
#define MAX_SPACE 4  //최대 칸수
#define INIT_ROOM 3  //초기 방수

typedef struct space {  //칸 구조체
	char personName[MAX_SIZE];
}Space;

typedef struct roomNode {  //방 구조체
	struct roomNode* r_next;  //방 리스트 다음 노드
	struct roomNode* r_prev;  //방 리스트 이전 노드
	int max;  //방 제한 인원수
	int p_number;  //방 현재 인원수
	Space* spaceList[4];  //칸 배열
}RoomNode;

typedef struct roomList {  //방 리스트
	RoomNode* r_header;
	RoomNode* r_trailer;
}RoomList;

typedef struct floorNode {  //층 구조체
	struct floorNode* f_prev;  //층 리스트 이전 노드
	struct floorNode* f_next;  //층 리스트 다음 노드
	RoomList* f_temp;  //방 리스트
}FloorNode;

typedef struct floorList {  //층 리스트
	FloorNode* f_header;
	FloorNode* f_trailer;
}FloorList;

typedef struct building {  //건물 구조체
	FloorList* floor;  //층 리스트
	int personNum;  //현재 총 인원수
	int roomNum;  //현재 방의 갯수
}Building;

void addSpace(Building*, RoomNode*, char*);  //room이라는 방에 personName이라는 사람이 사는 칸 추가
void removeSpace(Building*, RoomNode*, int);  //room이라는 방에 personName이라는 사람이 사는 칸 제거
void addRoom(Building*, RoomList*, int, char*);  //room 추가    
RoomList* setRoomList();  //RoomList 초기화
void addFloor(FloorList*);  //floor라는 FloorList에 층 추가
void setFloor(FloorList*);  //floor라는 FloorList 초기화
void setBuilding(Building*, int*, char* personName[]);  //building 초기화
