#include<ctype.h>
#include"List.h"

#define L_SIZE 26  //단어장 Size
#define ENCODE 80  //Encoding 변수

void print_manual();  //사용 manual 출력

int set_dic_menu();  //사전 menu 출력
int set_voc_menu();  //단어장 menu 출력
int init_menu();  //초기 menu출력

void set_dic(NODE**, char file_name[]);  //file 읽어서 사전 초기화
void set_voc(NODE**, char file_name[]);  //file 읽어서 단어장 초기화

void init_dic(NODE**);  //List 초기화

void dic_mode(NODE**, NODE**);  //사전 모드
void voc_mode(NODE**);  //단어장 모드

void str_lower(char str[]);  //str을 모두 소문자로 바꿔주는 함수
void str_truc(char str[]);  //str에서 영문자와 띄어쓰기를 제외한 모든 문자를 제거하는 함수
void str_trans(char str[]);  //str 문자열을 사전에 넣을 수 있는 형태로 변환

int add_voc(char str[], NODE**);  //Dic에 특정 단어 삽입 성공하면 1, 실패하면 0
int delete_voc(char str[], NODE**);  //Dic에 특정 단어 삭제 성공하면 1, 실패하면 0
int search_voc(char str[], NODE**);  //Dic에서 특정 단어 있는지 검색 있으면 1, 없으면 0
void encode_ASCII(char str[], char result[]);  //str 문자열을 ASCII코드 int 배열로 변환

void dicSearch(NODE**, NODE**);  //dic_mode Search 메뉴 수행
void vocSearch(NODE**);  //voc_mode Search 메뉴 수행
void Insert(NODE**);  //Insert 메뉴 수행
void Delete(NODE**);  //Delete 메뉴 수행
void StoreBinary(NODE**);  //Binary File로 저장
void StoreASCII(NODE**);  //ASCII File로 저장

int main(void)
{
	int menu = 0;
	NODE* dic[L_SIZE];  //사전 선언
	NODE* voc[L_SIZE];  //단어장 선언
	
	print_manual();

	init_dic(dic);  //dictionary initializing
	init_dic(voc);  //vocabulary initializing

	set_dic(dic,"sample.txt");  //사전 파일 읽어서 초기화
	set_voc(voc,"voc.txt");  //단어장 파일 읽어서 초기화

	do
	{
		menu = init_menu();
		
		switch(menu)
		{
			case 1:
				dic_mode(dic, voc);
				break;
			case 2 : 
				voc_mode(voc);
				break;
		}
	}while(menu != 3);

	return 0;
}

void print_manual()
{
	printf("**********************************************************************************************\n");
	printf("- 처음에 단어장은 비어 있습니다.\n");
	printf("- 사전 mode로 들어가서 search를 하면 search한 단어가 자동으로 단어장에 들어갑니다.\n");
	printf("- 단어장 mode에서 Store in Binary와 Store in ASCII 두가지 형태로 저장할 수 있습니다.\n");
	printf("- 단어장 mode에서 저장하지 않고 종료하면 update된 단어장은 저장되지 않습니다.\n");
	printf("**********************************************************************************************\n");
}
int set_dic_menu()
{
	int menu = 0;

	do
	{
		printf("기능을 선택하세요\n");
		printf("[1] Search  [2] Back\n");
		printf(": ");
		scanf("%d",&menu);
		while(getchar()!='\n');  //stdin buffer 비우기

		/*********Error Control***********/
		if(menu != 1 && menu != 2)
		{
			printf("1~2의 숫자를 입력해주세요\n");
		}
	}while(menu != 1 && menu != 2);

	return menu;
}

int set_voc_menu()
{
	int menu = 0;
	
	do
	{
		printf("기능을 선택하세요\n");
		printf("[1] Search  [2] Insert  [3] Delete\n");
		printf("[4] Store in ASCII  [5] Store in Binary  [6] Back\n");
		printf(": ");
		scanf("%d",&menu);
		while(getchar()!='\n');  //buffer 비우기

		/******Error Control******/
		if(menu != 1 && menu != 2 && menu != 3 && menu != 4 && menu != 5 && menu != 6)  //1~6이 아님
		{
			printf("1~6의 숫자를 입력해주세요\n");
		}
	}while(menu != 1 && menu != 2 && menu != 3 && menu != 4 && menu != 5 && menu != 6);

	return menu;
}

int init_menu()
{
	int menu = 0;

	do
	{
		printf("어느 mode로 들어가시겠습니까?\n");
		printf("[1] 사전  [2] 단어장  [3] Quit\n");
		printf(": ");
		scanf("%d",&menu);
		while(getchar()!='\n');  //stdin buffer 비우기

		/******Error Control*******/
		if(menu != 1 && menu != 2 && menu != 3)  //1~3이 아님
		{
			printf("1~3의 숫자를 입력해주세요\n");
		}

	}while(menu != 1 && menu != 2 && menu != 3);
}

void set_dic(NODE** dic, char file_name[SIZE])
{
	FILE* fp;
	char* token;
	char buf[SIZE];
	int i;

	for(i=0;i<L_SIZE;i++)
		dic[i] = NULL;  //dic 초기화
	
	fp = fopen(file_name,"r");

	if(fp == NULL)
	{
		printf("Dictionary File Open Fail\n");
		exit(0);
	}
	while(!feof(fp))
	{
		while(fgets(buf, SIZE, fp))
		{
			str_trans(buf);  //buf내의 문자를 정리(알파벳X-> 삭제, 대문자->소문자)
			token = strtok(buf," ");  //token 분리
	
			while(token != NULL)
			{
				add_voc(token, dic);	//단어장에 추가
				token = strtok(NULL," ");  //다음 token 접근
			}
		}
	}

	fclose(fp);
}

void init_dic(NODE** dic)
{
	int i;

	for(i=0;i<L_SIZE;i++)
		dic[i] = NULL;
}

void dic_mode(NODE** dic, NODE** voc)
{
	int menu = 0;

	do
	{
		menu = set_dic_menu();

		if(menu == 1)
			dicSearch(dic, voc);
	}while(menu != 2);
}

void voc_mode(NODE** voc)
{
	int menu = 0;

	do
	{
		menu = set_voc_menu();

		switch(menu)
		{
			case 1 : 
				vocSearch(voc);
				break;
			case 2 : 
				Insert(voc);
				break;
			case 3 : 
				Delete(voc);
				break;
			case 4 : 
				StoreASCII(voc);
				break;
			case 5 : 
				StoreBinary(voc);
				break;
		}
	}while(menu != 6);
}

void str_lower(char str[])
{
	int i;
	
	for(i=0;str[i]!='\0';i++)
	{
		if(str[i] == " ");
		else if(isupper(str[i]))
			str[i] = tolower(str[i]);  //소문자로 바꾼다
	}
}  

void str_truc(char str[])
{
	char tmp[SIZE];
	int i, j=0;
	
	for(i=0;str[i]!='\0';i++)
	{
		if(isalpha(str[i]) || str[i] == ' ')
		{
			tmp[j] = str[i];  //알파벳과 띄어쓰기 인수만 복사
			j++;
		}	
		else
		{
			tmp[j] = ' ';
			j++;
		}
	}
	tmp[j] = '\0';  //문자열 마지막에 NULL 입력

	strncpy(str, tmp, SIZE);  //문자열 복사
}  

void str_trans(char str[])
{
	str_truc(str);  //알파벳 외 문자 제거
	str_lower(str);  //모두 소문자로 변환
}  

int add_voc(char str[], NODE** dic)
{
	int dic_idx;  //해당 단어의 단어장 index
	NODE* tmp;

	if(search_voc(str,dic))  //이미 있는 단어면 Pass
		return 0;
	else if(str == NULL)  //str이 비어있는 문자열이면
		return 0;
	else
	{
		dic_idx = str[0] - 'a';  //a~z를 0~25로 매칭

		tmp = dic[dic_idx];
	
		while(tmp != NULL)
			tmp = tmp->link;
	
		dic[dic_idx] = insert_node(dic[dic_idx],tmp,str);  //node 추가
	}	
	return 1;
}  

int delete_voc(char str[], NODE** dic)
{
	int i=0;
	NODE* prev;  //target node의 prev node
	NODE* tmp;

	if(!search_voc(str,dic))  //단어가 존재하지 않음
		return 0;
	else
	{
		do
		{
			tmp = dic[i];  //dic index의 첫부분부터
			if(tmp != NULL)
			{
				if(strncmp(tmp->data, str, SIZE) == 0)  //list의 첫부분이 일치
				{
					dic[i] = delete_node(dic[i],NULL);
					return 1;
				}
				else
				{
					while(tmp->link != NULL)  //List의 끝까지
					{
						prev = tmp;
						tmp = tmp->link;
	
						if(strncmp(tmp->data, str, SIZE) == 0)  //target 발견
						{
							dic[i] = delete_node(dic[i],prev);
							return 1;
						}
					}
				}
			}
			i++;  //다음 index 접근
		}while(i < L_SIZE);
	}
}

int search_voc(char str[], NODE** dic)
{
	int i=0;
	NODE* tmp;

	do
	{
		tmp = dic[i];

		while(tmp != NULL)
		{
			if(strncmp(tmp->data, str, SIZE)==0)
				return 1;
			else
				tmp = tmp->link;
		}
		i++;
	}while(i < L_SIZE);

	return 0;
}

void encode_ASCII(char str[], char result[])
{
	int i = 0, k;
	char tmp[2];  //ASCII code 임시 저장소
	int itmp;  //ASCII code 임시 저장소

	for(k=0;k<(2*SIZE);k++)
		result[k] = NULL;  //result string 초기화
	
	while(str[i] != NULL)
	{
		itmp = str[i] - ENCODE;  //str의 char를 ASCII code로 변환하여 저장
		sprintf(tmp, "%d", itmp);  //int type을 string type으로 변환하여 저장
		strncat(result, tmp, 2);  //ASCII code를 result 배열에 순차적으로 stack
		i++;
	}
}
void dicSearch(NODE** dic, NODE** voc)
{
	char str[SIZE];

	printf("찾을 단어를 입력하세요: ");
	scanf("%s",str);
	while(getchar()!='\n');  //buffer 비우기
	
	str_trans(str);  //dic에 적절한 형태로 변환

	if(search_voc(str,dic))
	{
		printf("-> 단어장에 있습니다.\n");
		add_voc(str, voc);  //단어장에 추가
	}
	else
		printf("-> 단어장에 없습니다.\n");
}

void vocSearch(NODE** dic)
{
	char str[SIZE];

	printf("찾을 단어를 입력하세요: ");
	scanf("%s",str);
	while(getchar()!='\n');  //buffer 비우기
	
	str_trans(str);  //dic에 적절한 형태로 변환

	if(search_voc(str,dic))
		printf("-> 단어장에 있습니다.\n");
	else
		printf("-> 단어장에 없습니다.\n");
}

void Insert(NODE** dic)
{
	char str[SIZE];

	printf("추가할 단어를 입력하세요: ");
	scanf("%s",str);
	while(getchar()!='\n');  //buffer 비우기

	str_trans(str);  //dic에 적절한 형태로 변환
	
	if(add_voc(str, dic))
		printf("-> 추가 되었습니다.\n");
	else
		printf("-> 이미 존재하는 단어입니다.\n");
}

void Delete(NODE** dic)
{
	char str[SIZE];

	printf("삭제할 단어를 입력하세요: ");
	scanf("%s",str);
	while(getchar()!='\n');  //buffer 비우기

	str_trans(str);  //dic에 적절한 형태로 변환

	if(delete_voc(str, dic))
		printf("-> 삭제 되었습니다.\n");
	else
		printf("-> 단어장에 없는 단어 입니다.\n");
}

void StoreBinary(NODE** voc)
{
	FILE* bfp;  //binary file pointer
	NODE* tmp;  //검색을 위한 접근 NODE pointer
	int i;

	if((bfp = fopen("voc.txt","wb")) == NULL)
	{
		printf("File Open Error");  //file open fail
		exit(0);
	}

	for(i=0;i<L_SIZE;i++)
	{
		tmp = voc[i];  //i번째 list 접근
		while(tmp != NULL)
		{
			fwrite(tmp->data, SIZE, 1, bfp);  //binary file write
			tmp = tmp->link;  //다음 Node 접근
		}
	}

	fclose(bfp);
	printf("Storing in binary file is completed\n");  //store 완료 메세지 출력
}
void StoreASCII(NODE** voc)
{
	FILE* afp;  //ASCII file pointer
	NODE* tmp;  //검색을 위한 접근 NODE pointer
	int i;
	
	if((afp = fopen("ASCII.txt","w")) == NULL)
	{
		printf("File Open Error\n");  //file open fail
		exit(0);
	}

	for(i=0;i<L_SIZE;i++)
	{
		tmp = voc[i];  //i번째 list 접근
		fprintf(afp,"%c : ",'a'+i);
		while(tmp!=NULL)
		{
			fprintf(afp,"%s -> ",tmp->data);
			tmp = tmp->link;  //다음 Node 접근
		}
		fprintf(afp,"NULL\n");
	}

	fclose(afp);
	printf("Storing in ASCII file is completed\n");  //store 완료 메세지 출력
}
void set_voc(NODE** dic, char file_name[])
{
	FILE* bfp;  //binary file pointer
	NODE* tmp;  //Data 입력을 위한 접근 NODE pointer
	char str[SIZE];  //binary file에서 읽어온 data를 임시 저장할 String

	if((bfp = fopen(file_name,"rb")) == NULL)  //file open fail
		return;
	else
	{	
		while(!feof(bfp))
		{
			fread(str, SIZE, 1, bfp);  //binary file read
			if(isalpha(str[0]))
				add_voc(str, dic);  //List에 data 추가
		}
	}
}


