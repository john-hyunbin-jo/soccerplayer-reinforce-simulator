#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Visual Studio ����
#pragma warning (disable: 4996)
#pragma warning (disable: 6031)
// bool �ڷ��� �����
typedef unsigned char bool;
#define true 1;
#define false 0;

enum CARD_TYPE {
	PREDEF, MATERIAL
};

/*
* �ڷ� ����: ���Ḯ��Ʈ�� ����ü
*  Element: ���Ḯ��Ʈ ������(Entry)�� ��� �� ��� ����ü
*  Entry: �����Ͱ� �� ����ü
*/
typedef struct _Element* ElementPtr;
typedef struct _Entry* EntryPtr;
typedef struct _Element {
	EntryPtr data;
	ElementPtr prev;
	ElementPtr next;
} Element, *ElementPtr;
// ������ Index�� �Ʒ� ��Ģ���� ����.
/*
*	0: ����		1: ���� �ӵ�		2: �� �н�		3: �� �н�		4: �߰Ÿ� ��
*	5: ���÷�		6: ũ�ν�			7: ��Ŭ			8: ����ä��		9: �þ�
*	10: ���̺�	11: GK��ġ����	12: �ݻ�Ű�		13: �ڵ鸵		14: ű
*/
typedef struct _Entry {
	char Name[65];
	bool Is_GK;
	unsigned char Stats[15];					// ���� �����.
	unsigned char Level;
	unsigned char Type;
} Entry;

/*
* const Entry entries[]: �⺻������ ������ ���� ���.
*/
const Entry entries[] = {
	{"�κ���Ʈ ���ݵ�����Ű", 0, { 201, 197, 205, 194, 201, 232, 190, 155, 140, 193, 0, 0, 0, 0, 0 }, 0, PREDEF},
    {"��Ʈ�� ü��", 1, { 160, 154, 153, 171, 150, 140, 140, 155, 140, 149, 216, 189, 202, 209, 169 }, 0, PREDEF},
    {"T. �˷����-�Ƴ��", 0, { 187, 187, 197, 194, 161, 177, 200, 174, 180, 164, 0, 0, 0, 0, 0 }, 0, PREDEF},
    {"����Ű����", 0, { 204, 179, 167, 178, 134, 167, 168, 204, 190, 164, 0, 0, 0, 0, 0 }, 0, PREDEF},
    {"G. Ű������", 0, { 186, 179, 177, 170, 179, 163, 202, 190, 190, 150, 0, 0, 0, 0, 0 }, 0, PREDEF},
    {"�Ŀ�� �����", 0, { 184, 182, 171, 190, 153, 174, 169, 170, 133, 133, 0, 0, 0, 0, 0 }, 0, PREDEF},
    {"������", 0, { 201, 193, 194, 181, 197, 133, 140, 155, 140, 185, 0, 0, 0, 0, 0 }, 0, PREDEF},
    {"P. ���̶�", 0, { 182, 180, 217, 180, 160, 148, 192, 192, 180, 166, 0, 0, 0, 0, 0 }, 0, PREDEF},
    {"�����", 0, { 205, 200, 186, 181, 193, 215, 195, 155, 140, 185, 0, 0, 0, 0, 0 }, 0, PREDEF},
    {"���׵� ����", 0, { 167, 170, 182, 183, 199, 180, 167, 148, 140, 185, 0, 0, 0, 0, 0 }, 0, PREDEF},
    {"ũ����Ƽ�ƴ� ȣ����", 0, { 197, 192, 184, 183, 192, 217, 186, 141, 140, 175, 0, 0, 0, 0, 0 }, 0, PREDEF},
};

const char candidates[][65] = {	"C: ������", "B: Ʈ�����", "A: �ĺ�� �ǹ�", "S: ������" };

/*
* ElementPtr head: ���Ḯ��Ʈ�� ���۵Ǵ� �Ӹ� ���
*  �ݵ�� data�� NULL�� Element�� �Ҵ�Ǿ�� ��
*/
Element head = { NULL, NULL, NULL };

/*
* int count: ���� ���� �����ϰ� �ִ� ������ ��.
*/
int count = 0;

/*
* �Լ� ����
*/
void Clear();
void Init();
void ViewMyEntries();
void EntryReinforce();
void ViewGotcha();

/*
* ���� �Լ�: ���� ȭ��� �޴� ����â�� ǥ���Ѵ�.
*/
int main(int argc, char** argv)
{
	Init();
	srand((unsigned int)time(NULL));
	while (1)
	{
		int Option = -1;
		Clear();
		// �޴� ȭ�� �׸���.
		printf("1. �� ���� ����\n");
		printf("2. ���� ��ȭ\n");
		printf("3. ���� �̱�\n");
		printf("4. ����\n");
		printf("\n>> ");
		scanf("%d%*c", &Option);
		
		// ������ �޴��� ���� �б�.
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
			exit(EXIT_SUCCESS);	// ����.
		default:
			break;
		}
	}
}

/*
* SetStat(...): �־��� ��Ʈ���� ������ ������.
*/
void SetStat(EntryPtr ep, int index, unsigned char data)
{
	ep->Stats[index] = data;
}

/*
* Init(): ù ���� �� �غ��ϱ� ���� �ʱ�ȭ �Լ�.
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
		// ��ũ ����.
		if (i != 0)
		{
			EP_Array[i]->prev = EP_Array[i - 1];
			EP_Array[i - 1]->next = EP_Array[i];
		}
		else
			EP_Array[0]->prev = &head;
		// -- �Ʒ� ���� �����, ��� ������ 0, ������ 1����, �̸��� Entry, ��Ű�۰� �ƴ� ������ ����.
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
	// HEAD�� ��ũ ����.
	head.next = EP_Array[0];
}

/*
* EntryReinforce(): ���� ��ȭ �޴�.
*/
void EntryReinforce()
{
	Clear();
	printf("=== ���� ��ȭ ===\n");
	printf("���� �����ϰ� �ִ� ������ ����� ������ �����ϴ� :\n\n");
	printf("[%3s] [%3s] [%5s] %-64s\n", "#", "+", "Type", "������");
	ElementPtr seek = head.next;
	int index = 1;
	for (; seek; seek = seek->next)
		printf("[%3d] [%3d] [%5s] %-64s\n", index++, seek->data->Level, seek->data->Type ? "MAT" : "PRE", seek->data->Name);
	printf("\n��ȭ�� ������ ��ȣ�� �Է��ϼ���: ");
	int target = 0, material = 0;
	scanf("%d", &target);
	printf("%d�� ������ ��ȭ�� ��� ������ ��ȣ�� �Է��ϼ���: ", target);
	scanf("%d", &material);
	if (target == material)
	{
		printf("[����] ��ȭ ���� ���� ���� �� �����ϴ�! �ƹ� Ű�� ���� ������...");
		system("PAUSE > NUL");
		return;
	}
	else if (target > count || material > count || target < 1 || material < 1)
	{
		printf("[����] ���� �� ���ڸ� �Է��ϼ̽��ϴ�! �ƹ� Ű�� ���� ������...");
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
	printf("=== ��ȭ ��� ���� ===\n���: %s\n���: %s\n", target_card->data->Name, material_card->data->Name);

	// ī�� ��ȿ�� �˻�
	if (!(target_card->data->Type == PREDEF && material_card->data->Type == MATERIAL))
	{
		printf("[����] ��ȭ ����� �̸� ���ǵ� ī��, ��ȭ ���ī��� ���ī�常 ���� �����մϴ�.");
		system("PAUSE > NUL");
		return;
	}

	// ��ȭ ó��, Ȯ�� ���.
	int randval = rand() % 100;	// 0 ~ 99: 100���� ����.
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
		// ����, ���� ++, ��ȭ��ġ ++
		for (int i = 0; i < 15; i++)
			target_card->data->Stats[i] += 3;
		target_card->data->Level++;
		printf("��ȭ�� �����߽��ϴ�.\n");
	}
	else
	{
		// ����, ����--, ��ȭ���� --, 0ī ����.
		if (target_card->data->Level != 0)
		{
			for (int i = 0; i < 15; i++)
				target_card->data->Stats[i] -= 3;
			target_card->data->Level--;
		}
		printf("��ȭ�� �����߽��ϴ�.\n");
	}
	// ���ī�� ����(���Ḯ��Ʈ).
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
* ViewMyEntries(): �� ���� ����� ������.
*/
void ViewMyEntries()
{
	while (1)
	{
		Clear();
		printf("���� �����ϰ� �ִ� ������ ����� ������ �����ϴ� :\n\n");
		printf("[%3s] [%3s] [%5s] %-64s\n", "#", "+", "Type", "������");
		Element* seek = head.next;
		int index = 1;
		for (; seek; seek = seek->next)
			printf("[%3d] [%3d] [%5s] %-64s\n", index++, seek->data->Level, seek->data->Type ? "MAT" : "PRE", seek->data->Name);
		int menu = 0;
		printf("\n�ڼ��� ������ Ȯ���Ϸ��� ���� ��ȣ�� �Է��ϼ���(��ȿ���� ���� ���ڴ� �������Դϴ�): ");
		scanf("%d", &menu);
		// ��ȿ���� ���� ���ڸ� ������.
		if (menu < 1 || menu > count)
			break;
		seek = head.next;
		for (int i = 1; i < menu; i++)
			seek = seek->next;
		printf("- %4d - [+%d]%-65s\n\n", menu, seek->data->Level, seek->data->Name);
		printf("STAT --\n");
		{
			printf("%-12s: %d\n", "����", seek->data->Stats[0]);
			printf("%-12s: %d\n", "���� �ӵ�", seek->data->Stats[1]);
			printf("%-12s: %d\n", "�� �н�", seek->data->Stats[2]);
			printf("%-12s: %d\n", "�� �н�", seek->data->Stats[3]);
			printf("%-12s: %d\n", "�߰Ÿ� ��", seek->data->Stats[4]);
			printf("%-12s: %d\n", "���÷�", seek->data->Stats[5]);
			printf("%-12s: %d\n", "ũ�ν�", seek->data->Stats[6]);
			printf("%-12s: %d\n", "��Ŭ", seek->data->Stats[7]);
			printf("%-12s: %d\n", "����ä��", seek->data->Stats[8]);
			printf("%-12s: %d\n", "�þ�", seek->data->Stats[9]);
			if (seek->data->Is_GK)
			{
				printf("[GK Ư������]\n");
				printf("%-12s: %d\n", "���̺�", seek->data->Stats[10]);
				printf("%-12s: %d\n", "GK��ġ����", seek->data->Stats[11]);
				printf("%-12s: %d\n", "�ݻ�Ű�", seek->data->Stats[12]);
				printf("%-12s: %d\n", "�ڵ鸵", seek->data->Stats[13]);
				printf("%-12s: %d\n", "ű", seek->data->Stats[14]);
			}
		}
		printf("\n�ƹ� Ű�� ���� ���ư���...");
		system("PAUSE > NUL");
	}
}

/*
* ViewGotcha(): ���� �̱� �޴�.
*/
void ViewGotcha()
{
	Clear();
	printf("=== ���� �̱� ===\n");
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
	// ���� �߰�: ���Ḯ��Ʈ �߰�.
	ElementPtr next = head.next;

	// ���� �߰��� ���� �ش� ������ ����� �޸� �Ҵ�
	ElementPtr element = malloc(sizeof(Element));
	element->data = malloc(sizeof(Entry));
	strcpy(element->data->Name, candidates[ent]);
	element->data->Is_GK = 0;
	element->data->Level = 0;
	element->data->Type = MATERIAL;
	for (int i = 0; i < 15; i++)
		element->data->Stats[i] = 0;

	// ���Ḯ��Ʈ�� ���� ���踦 �����Ͽ� head ������ �߰���Ŵ.
	head.next = element;
	element->prev = &head;
	element->next = next;
	next->prev = element;
	count++;
	printf("����: %s�� �������ϴ�.", candidates[ent]);
	system("PAUSE > NUL");
}

/*
* Clear(): ȭ�� ������ ����� �Լ�
*/
void Clear()
{
	system("CLS");
}