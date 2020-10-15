/* LINKED_HEAP.cpp                                           */
/*															 */
/*  ���� ����Ʈ                                              */
/*  1. ���� �����ϴ� �ڵ尡 ���� �Ѵ�.                   */
/*  2. �޸𸮸� �Ҵ� �� �����ϴ� �Լ��� ����Ǿ�� �Ѵ�.     */
/*  3. ����Ʈ�� ��� ��带 �����ϴ� ����� ����ȴ�.        */
/*  �׸��� ������ �̸��� LINKED_HEAP.CPP �� �����Ͽ���.      */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h> 

typedef int ListElementDataType;

typedef struct _node
{
    ListElementDataType data;
    struct _node *next;
} node;

node *head, *tail;

HANDLE hHeap = 0;

void InitListHeap()
{
	SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    
    UINT pageSize = sysInfo.dwPageSize;
    
    hHeap =   // OPTION, COMMIT PAGE, RESERV PAGE
       HeapCreate( HEAP_NO_SERIALIZE, pageSize * 10, pageSize * 100 );
}

void InitList(void)
{
	InitListHeap();  /* Dynamic Heap ��ݿ����� ȣ���Ѵ�. */

	//head = (node*)malloc(sizeof(node));
	head = (node*)HeapAlloc(hHeap, HEAP_NO_SERIALIZE, sizeof(node));

	//tail = (node*)malloc(sizeof(node));
	tail = (node*)HeapAlloc(hHeap, HEAP_NO_SERIALIZE, sizeof(node));
	
	head->next = tail;
	tail->next = tail;
}

/**********************************************************
����: int DeleteNode(int data)
���: �Էµ� data�� ������ ���� ���ϴ� ù��° ��� ����. 
***********************************************************/
int DeleteNode(int data)
{
	node * nd = head;
	node * ndNext = nd->next;

	while (ndNext->data != data && ndNext != tail)
	{
		nd = nd->next;
		ndNext = nd->next;
	}

	if (ndNext != tail)
	{
		nd->next = ndNext->next;
		
		//free(ndNext);
		HeapFree(hHeap, HEAP_NO_SERIALIZE, ndNext);
		return 1;
	}
	else
		return 0;
}

/**********************************************************
����: void OrderedInsert(int data)
���: �Էµ� data�� ����Ʈ�� �����ϵ� �����ؼ� ����. 
***********************************************************/
void OrderedInsert(int data)
{
	node * nd = head;
	node * ndNext = nd->next;
	node * newNode;
	while (ndNext->data <= data && ndNext != tail)
	{
		nd = nd->next;
		ndNext = nd->next;
	}
	
	//newNode = (node*)malloc(sizeof(node));
	newNode = (node*)HeapAlloc(hHeap, HEAP_NO_SERIALIZE, sizeof(node));

	newNode->data = data;
	nd->next = newNode;
	newNode->next = ndNext;
}

/**********************************************************
����: void PrintAllList()
���: ����Ʈ�� ����� ��� ������ ���.
***********************************************************/
void PrintAllList()
{
	node * startNode=head->next;
	while (startNode != tail)
	{
		printf("%-4d", startNode->data);
		startNode = startNode->next;
	}
	printf("\n\n");
}

/**********************************************************
����: void DeleteAll(void)
���: ����Ʈ�� ����� ��� ������ ����.
***********************************************************/
void DeleteAll(void)
{
	//node* nd=head->next;
	//node* delNode;
	//while(nd != tail)
	//{
	//    delNode = nd;
	//    nd = nd->next;
	//    free(delNode);
	//}
	//head->next=tail;
	
	/* Heap �Ҹ��� �� ���� */
	HeapDestroy(hHeap); // ���� ������ �ڵ���� ��ü�Ѵ�. 
	InitList();			// ���ɵ� ���ǰ�, ���� �߻����� ����.

}

int main(int argc, char * argv[])
{
    InitList();
    
	printf("1, 2, 3 �Է�... \n");
	OrderedInsert(1);
	OrderedInsert(2);
	OrderedInsert(3);
    PrintAllList();

	printf("4, 5, 6 �Է�...\n");
	OrderedInsert(4);
	OrderedInsert(5);
	OrderedInsert(6);
    PrintAllList();

	printf("2�� 5 ����...\n");
	DeleteNode(2);
	DeleteNode(5);
    PrintAllList();

	printf("��� ����Ʈ ����... \n");
	DeleteAll();

	printf("6, 5, 4 �Է�...\n");
	OrderedInsert(6);
	OrderedInsert(5);
	OrderedInsert(4);
    PrintAllList();

	printf("3, 2, 1 �Է�...\n");
	OrderedInsert(3);
	OrderedInsert(2);
	OrderedInsert(1);
    PrintAllList();

	printf("��� ����Ʈ ����... \n");
	DeleteAll();

	printf("1, 3, 5 �Է�...\n");
	OrderedInsert(1);
	OrderedInsert(3);
	OrderedInsert(5);
    PrintAllList();

	printf("2, 4, 5 �Է�...\n");
	OrderedInsert(2);
	OrderedInsert(4);
	OrderedInsert(6);
    PrintAllList();

    return 0;
}


