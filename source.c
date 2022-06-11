#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Visual Studio 관련
#pragma warning (disable: 4996)
#pragma warning (disable: 6031)
// bool 자료형 만들기
typedef unsigned char bool;
#define true 1;
#define false 0;

enum CARD_TYPE {
	PREDEF, MATERIAL
};

/*
* 자료 구조: 연결리스트와 구조체
*  Element: 연결리스트 데이터(Entry)를 담는 각 요소 구조체
*  Entry: 데이터가 될 구조체
*/
typedef struct _Element* ElementPtr;
typedef struct _Entry* EntryPtr;
typedef struct _Element {
	EntryPtr data;
	ElementPtr prev;
	ElementPtr next;
} Element, *ElementPtr;
// 스탯의 Index는 아래 규칙으로 결정.
/*
*	0: 가속		1: 질주 속도		2: 숏 패스		3: 롱 패스		4: 중거리 슛
*	5: 슈팅력		6: 크로스			7: 태클			8: 가로채기		9: 시야
*	10: 다이빙	11: GK위치선정	12: 반사신경		13: 핸들링		14: 킥
*/
typedef struct _Entry {
	char Name[65];
	bool Is_GK;
	unsigned char Stats[15];					// 스탯 저장소.
	unsigned char Level;
	unsigned char Type;
} Entry;

/*
* const Entry entries[]: 기본적으로 설정된 선수 목록.
*/
const Entry entries[] = {
	{"로베르트 레반도프스키", 0, { 201, 197, 205, 194, 201, 232, 190, 155, 140, 193, 0, 0, 0, 0, 0 }, 0, PREDEF},
    {"페트르 체흐", 1, { 160, 154, 153, 171, 150, 140, 140, 155, 140, 149, 216, 189, 202, 209, 169 }, 0, PREDEF},
    {"T. 알렉산더-아놀드", 0, { 187, 187, 197, 194, 161, 177, 200, 174, 180, 164, 0, 0, 0, 0, 0 }, 0, PREDEF},
    {"마르키뉴스", 0, { 204, 179, 167, 178, 134, 167, 168, 204, 190, 164, 0, 0, 0, 0, 0 }, 0, PREDEF},
    {"G. 키엘리니", 0, { 186, 179, 177, 170, 179, 163, 202, 190, 190, 150, 0, 0, 0, 0, 0 }, 0, PREDEF},
    {"파울로 말디니", 0, { 184, 182, 171, 190, 153, 174, 169, 170, 133, 133, 0, 0, 0, 0, 0 }, 0, PREDEF},
    {"가린샤", 0, { 201, 193, 194, 181, 197, 133, 140, 155, 140, 185, 0, 0, 0, 0, 0 }, 0, PREDEF},
    {"P. 비에이라", 0, { 182, 180, 217, 180, 160, 148, 192, 192, 180, 166, 0, 0, 0, 0, 0 }, 0, PREDEF},
    {"손흥민", 0, { 205, 200, 186, 181, 193, 215, 195, 155, 140, 185, 0, 0, 0, 0, 0 }, 0, PREDEF},
    {"지네딘 지단", 0, { 167, 170, 182, 183, 199, 180, 167, 148, 140, 185, 0, 0, 0, 0, 0 }, 0, PREDEF},
    {"크리스티아누 호날두", 0, { 197, 192, 184, 183, 192, 217, 186, 141, 140, 175, 0, 0, 0, 0, 0 }, 0, PREDEF},
};

const char candidates[][65] = {	"C: 졸리스", "B: 트라오레", "A: 파비오 실바", "S: 무뇨스" };

/*
* ElementPtr head: 연결리스트가 시작되는 머리 노드
*  반드시 data가 NULL인 Element가 할당되어야 함
*/
Element head = { NULL, NULL, NULL };

/*
* int count: 현재 내가 보유하고 있는 선수의 수.
*/
int count = 0;

/*
* 함수 선언
*/
void Clear();
void Init();
void ViewMyEntries();
void EntryReinforce();
void ViewGotcha();

/*
* 메인 함수: 메인 화면과 메뉴 선택창을 표시한다.
*/
int main(int argc, char** argv)
{
	Init();
	srand((unsigned int)time(NULL));
	while (1)
	{
		int Option = -1;
		Clear();
		// 메뉴 화면 그리기.
		printf("1. 내 선수 보기\n");
		printf("2. 선수 강화\n");
		printf("3. 선수 뽑기\n");
		printf("4. 종료\n");
		printf("\n>> ");
		scanf("%d%*c", &Option);
		
		// 선택한 메뉴에 따라 분기.
		switch (Option)
		{
		case 3:
			ViewGotcha();
			break;
		case 2:
			EntryReinforce();
			break;
		case 1:
			ViewMyEntries();
			break;
		case 4:
			exit(EXIT_SUCCESS);	// 종료.
		default:
			break;
		}
	}
}

/*
* SetStat(...): 주어진 엔트리의 스탯을 설정함.
*/
void SetStat(EntryPtr ep, int index, unsigned char data)
{
	ep->Stats[index] = data;
}

/*
* Init(): 첫 실행 때 준비하기 위한 초기화 함수.
*/
void Init()
{
#ifdef __INIT
	return;
#endif
#define __INIT
	Element* EP_Array[11];
	for (int i = 0; i < 11; i++)
	{
		Element* __T = malloc(sizeof(Element));
		__T->data = malloc(sizeof(Entry));
		EP_Array[i] = __T;
		EP_Array[i]->prev = NULL;
		EP_Array[i]->next = NULL;
		// 링크 형성.
		if (i != 0)
		{
			EP_Array[i]->prev = EP_Array[i - 1];
			EP_Array[i - 1]->next = EP_Array[i];
		}
		else
			EP_Array[0]->prev = &head;
		// -- 아래 블럭은 지우기, 모든 스탯을 0, 레벨은 1으로, 이름은 Entry, 골키퍼가 아닌 것으로 설정.
		{
			strcpy(EP_Array[i]->data->Name, entries[i].Name);
			EP_Array[i]->data->Level = entries[i].Level;
			EP_Array[i]->data->Is_GK = entries[i].Is_GK;
			EP_Array[i]->data->Type = entries[i].Type;
			for (int j = 0; j < 15; j++)
				EP_Array[i]->data->Stats[j] = entries[i].Stats[j];
		}
		count++;
	}
	// HEAD에 링크 형성.
	head.next = EP_Array[0];
}

/*
* EntryReinforce(): 선수 강화 메뉴.
*/
void EntryReinforce()
{
	Clear();
	printf("=== 선수 강화 ===\n");
	printf("현재 보유하고 있는 선수의 목록은 다음과 같습니다 :\n\n");
	printf("[%3s] [%3s] [%5s] %-64s\n", "#", "+", "Type", "선수명");
	ElementPtr seek = head.next;
	int index = 1;
	for (; seek; seek = seek->next)
		printf("[%3d] [%3d] [%5s] %-64s\n", index++, seek->data->Level, seek->data->Type ? "MAT" : "PRE", seek->data->Name);
	printf("\n강화할 선수의 번호를 입력하세요: ");
	int target = 0, material = 0;
	scanf("%d", &target);
	printf("%d번 선수를 강화할 재료 선수의 번호를 입력하세요: ", target);
	scanf("%d", &material);
	if (target == material)
	{
		printf("[오류] 강화 대상과 재료는 같을 수 없습니다! 아무 키나 눌러 나가기...");
		system("PAUSE > NUL");
		return;
	}
	else if (target > count || material > count || target < 1 || material < 1)
	{
		printf("[오류] 범위 밖 숫자를 입력하셨습니다! 아무 키나 눌러 나가기...");
		system("PAUSE > NUL");
		return;
	}
	ElementPtr target_card = head.next;
	ElementPtr material_card = head.next;
	for (int i = 1; i < target; i++)
		target_card = target_card->next;
	for (int i = 1; i < material; i++)
		material_card = material_card->next;
	printf("\n\n");
	printf("=== 강화 대상 정보 ===\n대상: %s\n재료: %s\n", target_card->data->Name, material_card->data->Name);

	// 카드 유효성 검사
	if (!(target_card->data->Type == PREDEF && material_card->data->Type == MATERIAL))
	{
		printf("[오류] 강화 대상은 미리 정의된 카드, 강화 재료카드는 재료카드만 선택 가능합니다.");
		system("PAUSE > NUL");
		return;
	}

	// 강화 처리, 확률 계산.
	int randval = rand() % 100;	// 0 ~ 99: 100가지 숫자.
	int threshold = -1;
	int card_type = material_card->data->Name[0];
	if (card_type == 'C')
		threshold = 10;
	else if (card_type == 'B')
		threshold = 20;
	else if (card_type == 'A')
		threshold = 30;
	else if (card_type == 'S')
		threshold = 40;
	if (randval < threshold)
	{
		// 성공, 스탯 ++, 강화수치 ++
		for (int i = 0; i < 15; i++)
			target_card->data->Stats[i] += 3;
		target_card->data->Level++;
		printf("강화에 성공했습니다.\n");
	}
	else
	{
		// 실패, 스탯--, 강화수차 --, 0카 제외.
		if (target_card->data->Level != 0)
		{
			for (int i = 0; i < 15; i++)
				target_card->data->Stats[i] -= 3;
			target_card->data->Level--;
		}
		printf("강화에 실패했습니다.\n");
	}
	// 재료카드 삭제(연결리스트).
	ElementPtr prev = material_card->prev;
	ElementPtr next = material_card->next;
	prev->next = next;
	next->prev = prev;
	free(material_card->data);
	free(material_card);
	count--;
	system("PAUSE > NUL");
}

/*
* ViewMyEntries(): 내 선수 목록을 보여줌.
*/
void ViewMyEntries()
{
	while (1)
	{
		Clear();
		printf("현재 보유하고 있는 선수의 목록은 다음과 같습니다 :\n\n");
		printf("[%3s] [%3s] [%5s] %-64s\n", "#", "+", "Type", "선수명");
		Element* seek = head.next;
		int index = 1;
		for (; seek; seek = seek->next)
			printf("[%3d] [%3d] [%5s] %-64s\n", index++, seek->data->Level, seek->data->Type ? "MAT" : "PRE", seek->data->Name);
		int menu = 0;
		printf("\n자세한 정보를 확인하려면 선수 번호를 입력하세요(유효하지 않은 숫자는 나가기입니다): ");
		scanf("%d", &menu);
		// 유효하지 않은 숫자면 나가기.
		if (menu < 1 || menu > count)
			break;
		seek = head.next;
		for (int i = 1; i < menu; i++)
			seek = seek->next;
		printf("- %4d - [+%d]%-65s\n\n", menu, seek->data->Level, seek->data->Name);
		printf("STAT --\n");
		{
			printf("%-12s: %d\n", "가속", seek->data->Stats[0]);
			printf("%-12s: %d\n", "질주 속도", seek->data->Stats[1]);
			printf("%-12s: %d\n", "숏 패스", seek->data->Stats[2]);
			printf("%-12s: %d\n", "롱 패스", seek->data->Stats[3]);
			printf("%-12s: %d\n", "중거리 슛", seek->data->Stats[4]);
			printf("%-12s: %d\n", "슈팅력", seek->data->Stats[5]);
			printf("%-12s: %d\n", "크로스", seek->data->Stats[6]);
			printf("%-12s: %d\n", "태클", seek->data->Stats[7]);
			printf("%-12s: %d\n", "가로채기", seek->data->Stats[8]);
			printf("%-12s: %d\n", "시야", seek->data->Stats[9]);
			if (seek->data->Is_GK)
			{
				printf("[GK 특수스탯]\n");
				printf("%-12s: %d\n", "다이빙", seek->data->Stats[10]);
				printf("%-12s: %d\n", "GK위치선정", seek->data->Stats[11]);
				printf("%-12s: %d\n", "반사신경", seek->data->Stats[12]);
				printf("%-12s: %d\n", "핸들링", seek->data->Stats[13]);
				printf("%-12s: %d\n", "킥", seek->data->Stats[14]);
			}
		}
		printf("\n아무 키나 눌러 돌아가기...");
		system("PAUSE > NUL");
	}
}

/*
* ViewGotcha(): 선수 뽑기 메뉴.
*/
void ViewGotcha()
{
	Clear();
	printf("=== 선수 뽑기 ===\n");
	int randval = rand() % 100;
	int ent = -1;
	if (randval < 50)
		ent = 0;
	else if (randval < 80)
		ent = 1;
	else if (randval < 95)
		ent = 2;
	else
		ent = 3;
	// 선수 추가: 연결리스트 추가.
	ElementPtr next = head.next;

	// 선수 추가를 위해 해당 선수가 저장될 메모리 할당
	ElementPtr element = malloc(sizeof(Element));
	element->data = malloc(sizeof(Entry));
	strcpy(element->data->Name, candidates[ent]);
	element->data->Is_GK = 0;
	element->data->Level = 0;
	element->data->Type = MATERIAL;
	for (int i = 0; i < 15; i++)
		element->data->Stats[i] = 0;

	// 연결리스트의 연결 관계를 조작하여 head 앞으로 추가시킴.
	head.next = element;
	element->prev = &head;
	element->next = next;
	next->prev = element;
	count++;
	printf("선수: %s가 뽑혔습니다.", candidates[ent]);
	system("PAUSE > NUL");
}

/*
* Clear(): 화면 내용을 지우는 함수
*/
void Clear()
{
	system("CLS");
}