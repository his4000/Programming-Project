#include<stdio.h>
#include<string.h>

#define BUFFER 500  //BUFFER Array size
#define CYCLE 5  //반복 횟수

int str_check(char* input_str, char* sample);  //찾고자 하는 input_str의 조건에 sample이 맞는지 확인

int main(void)
{
	FILE *fp;
	char buf[BUFFER];  //File로 부터 읽어온 문자 라인 저장 Buffer
	char* tok;  //token
	char input[BUFFER];  //Input Buffer
	int result;  //str_check의 결과. 0이면 실패, 1이면 성공
	int line_count;  //line에서 찾은 개수 검사. 처음이면 0, 아니면 횟수
	int count;  //찾은 문자열 개수 저장
	int line;  //몇 번째 라인인가?
	int i;
	int cycle;  //cycle 횟수

	for(cycle=0;cycle<CYCLE;cycle++)  //CYCLE회 반복
	{
		fp = fopen("sample.txt","r");

		count = 0;  //찾은 문자열 개수 초기화
		line = 1;  //찾은 Line수 초기화
		for(i=0;i<BUFFER;i++)  //Buffer 초기화
		{
			buf[i] = '\0';
			input[i] = '\0';		
		}
		printf("단어를 입력 하세요 : ");
		scanf("%s",input);
		printf("\n");

		while(!feof(fp))
		{
			while(fgets(buf,BUFFER,fp))
			{
				line_count = 0;
				tok = strtok(buf," ");  //token 분리

				while(tok != NULL)
				{
					result = str_check(input,tok);  //str_check
					if(result == 1 && line_count == 0)  //그 줄에서 처음 찾음
					{
						printf("line#%d : ",line);
						line_count++;
					}
					else if(result == 1)
						line_count++;
					if(result == 1)
					{
						printf("%s ",tok);
					}
					count = count + result;  //찾은 문자열 개수 증가
					tok = strtok(NULL," ");  //다음 token 접근
				}
				line++;	
				if(line_count != 0)
					printf("\n");
			}	
		}
		printf("\n총 %d 개의 %s 을 찾았습니다.\n",count,input);

		fclose(fp);
	}

	return 0;
}

int str_check(char* input_str, char* sample)
{
	int input_idx = 0, sample_idx = 0;  //input_str과 sample의 검색 위치를 추적하는 index pointer
	int q_count;  //물음표 개수를 저장
	int result;  //0이면 불일치, 1이면 일치
	int i;

	while(sample[sample_idx] != '\0')  //sample이 끝에 도달하면 break
	{
		q_count = 0;
		result = 0;
		/*********'*'이 나온경우**********/
		if(input_str[input_idx] == '*')
		{
			for(;(input_str[input_idx] == '*') || (input_str[input_idx] == '?');input_idx++);
			//'*'나 '?'가 아닌곳으로 포인터 이동 => '*'뒤에 나오는 '?'는 무의미
			
			/***맨 마지막에 '*'를 입력한 경우***/
			if(input_str[input_idx] == '\0')
				return 1;  //성공-끝
			/***'*'뒤에 문자가 나온 경우***/
			else
			{
				do
				{
					if(input_str[input_idx] == sample[sample_idx])  //같은걸 찾았다
					{
						result = 1;  //성공-계속진행
						break;  //do-while문 탈출
					}
					else  //같지 않다
					{
						sample_idx++;  //다음 포인터 접근
					}
				}while(sample[sample_idx] != '\0');  //끝에 도달할 때 까지
			}
		}
		
		/*********'?'이 나온경우**********/
		else if(input_str[input_idx] == '?')
		{
			for(i=0;input_str[input_idx+i] == '?';i++)  //'?'가 몇개 있는지 count
				q_count++;
			input_idx += q_count;  //'?'가 아닌 곳으로 포인터 이동
			
			/***맨 마지막에 '?'를 입력한 경우***/
			if(input_str[input_idx] == '\0')
			{
				for(i=0;i<q_count;i++)
				{
					if(sample[sample_idx] == '\0')  //'?'들을 다 매치시키기 전에 sample의 끝에 도달
						return 1;  //성공-끝
					sample_idx++;  //다음 포인터 접근		
				}
				if(sample[sample_idx] != '\0')  //'?'들을 다 매치 시켰는데도 뒤에 문자가 남은 경우
					return 0;  //실패
				else
					return 1;  //성공-끝
			}
			/***'?'뒤에 문자가 나온 경우***/
			else
			{
				if(input_str[input_idx] == '*')  //'?*'가 나온 경우
					result = 1;  //성공-계속진행 =>뒤에 '*'이 오면 '?'는 힘이 없다
				else
				{
					for(i=0;i<q_count;i++)  //sample의 현재위치에서 '?'개수만큼 이동한 동안
					{
						if(input_str[input_idx] == sample[sample_idx])  //같은걸 찾았다
						{
							result = 1;  //성공-계속진행
							break;  //for문 탈출
						}
						else if(sample[sample_idx] == '\0')  //'?'들을 다 매치시키기 전에 sample의 끝에 도달
							return 0;  //실패
						sample_idx++;  //다음 포인터 접근
					}
					if(input_str[input_idx] == sample[sample_idx])
						result = 1;  //성공-계속진행
					else
						return 0;  //실패 - 입력된 '?'개수 이내에 sample에서 그 다음 입력 char를 찾지 못함
				}
			}
		}
		/*********문자가 나온 경우*********/
		else
		{
			if(input_str[input_idx] == sample[sample_idx])  //비교한 문자가 같으면
			{
				result = 1;  //성공-계속진행
				input_idx++;
				sample_idx++;  //다음 포인터 접근
			}
			else  //비교한 문자가 다르면
				return 0;  //실패
		}
	}
	if(input_str[input_idx] != '\0')  //탐색을 마치지 못한채로 sample이 끝에 닿으면 실패
		return 0;
	
	return result;  //마지막까지 다 성공이면 일치
}
