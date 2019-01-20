#include<ctype.h>
#include"List.h"

#define L_SIZE 27  //단어장 Size

int set_menu();  //menu 출력

void set_dic(NODE**, char file_name[]);  //file 읽어서 단어장 초기화

void str_lower(char str[]);  //str을 모두 소문자로 바꿔주는 함수
void str_truc(char str[]);  //str에서 영문자와 띄어쓰기를 제외한 모든 문자를 제거하는 함수
void str_trans(char str[]);  //str 문자열을 사전에 넣을 수 있는 형태로 변환

int add_voc(char str[], NODE**);  //Dic에 특정 단어 삽입 성공하면 1, 실패하면 0
int delete_voc(char str[], NODE**);  //Dic에 특정 단어 삭제 성공하면 1, 실패하면 0
int search_voc(char str[], NODE**);  //Dic에서 특정 단어 있는지 검색 있으면 1, 없으면 0
void imm_voc(NODE**, int index);  //Dic의 index에 있는 Node들 the others로 이주

void Search(NODE**);  //Search 메뉴 수행
void Insert(NODE**);  //Insert 메뉴 수행
void Delete(NODE**);  //Delete 메뉴 수행

int main(void)
{
	int menu = 0;
	NODE* dic[L_SIZE];  //단어장 선언
	set_dic(dic,"voc_sample.txt");  //파일 읽어서 단어장 초기화
	do
	{
		menu = set_menu();
		
		switch(menu)
		{
			case 1:
				Search(dic);
				break;
			case 2 : 
				Insert(dic);
				break;
			case 3 : 
				Delete(dic);
				break;
		}
	}while(menu != 4);

	return 0;
}

int set_menu()
{
	int menu = 0;
	
	do
	{
		printf("기능을 선택하세요\n");
		printf("[1] Search  [2] Insert\n");
		printf("[3] Delete  [4] Quit \n");
		printf(": ");
		scanf("%d",&menu);
		while(getchar()!='\n');  //buffer 비우기

		/******Error Control******/
		if(menu != 1 && menu != 2 && menu != 3 && menu != 4)  //1~4가 아님
		{
			printf("1~4의 숫자를 입력해주세요\n");
		}
	}while(menu != 1 && menu != 2 && menu != 3 && menu != 4);

	return menu;
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
	int count = 0;  //해당 index의 node 수
	NODE* tmp;

	if(search_voc(str,dic))  //이미 있는 단어면 Pass
		return 0;
	else if(str == NULL)  //str이 비어있는 문자열이면
		return 0;
	else
	{
		dic_idx = str[0] - 'a';  //a~z를 0~25로 매칭

		tmp = dic[dic_idx];
		if(tmp != NULL)
		{
			while(tmp->link != NULL)
			{
				tmp = tmp->link;
				count++;
			}
		}
	
		if(count > 4)
		{
			imm_voc(dic, dic_idx);  //node들 이주
			dic[dic_idx] = insert_node(dic[dic_idx],NULL,str);  //맨 앞에 추가
		}
		else
		{
			dic[dic_idx] = insert_node(dic[dic_idx],tmp,str);  //node 추가
		}
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

void imm_voc(NODE** dic, int index)
{
	NODE* tmp;  //dic[index]의 가장 마지막 node
	
	tmp = dic[index];

	while(tmp->link != NULL)  //끝 node까지
	{
		tmp = tmp->link;
	}
	
	tmp->link = dic[L_SIZE-1];  //dic[index] List의 끝과 the others의 앞을 연결
	dic[L_SIZE-1] = dic[index];  //dic의 맨 마지막 index의 시작을 dic[index]로 설정
	dic[index] = NULL;  //dic[index]를 비움
}

void Search(NODE** dic)
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
