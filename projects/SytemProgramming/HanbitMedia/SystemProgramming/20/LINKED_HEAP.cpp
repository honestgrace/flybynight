/* LINKED_HEAP.cpp                                           */
/*															 */
/*  변경 포인트                                              */
/*  1. 힙을 생성하는 코드가 들어가야 한다.                   */
/*  2. 메모리를 할당 및 해제하는 함수가 변경되어야 한다.     */
/*  3. 리스트의 모든 노드를 삭제하는 방식이 변경된다.        */
/*  그리고 파일의 이름은 LINKED_HEAP.CPP 로 변경하였다.      */

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
	InitListHeap();  /* Dynamic Heap 기반에서만 호출한다. */

	//head = (node*)malloc(sizeof(node));
	head = (node*)HeapAlloc(hHeap, HEAP_NO_SERIALIZE, sizeof(node));

	//tail = (node*)malloc(sizeof(node));
	tail = (node*)HeapAlloc(hHeap, HEAP_NO_SERIALIZE, sizeof(node));
	
	head->next = tail;
	tail->next = tail;
}

/**********************************************************
선언: int DeleteNode(int data)
기능: 입력된 data와 동일한 값을 지니는 첫번째 노드 삭제. 
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
선언: void OrderedInsert(int data)
기능: 입력된 data를 리스트에 삽입하되 정렬해서 삽입. 
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
선언: void PrintAllList()
기능: 리스트에 저장된 모든 데이터 출력.
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
선언: void DeleteAll(void)
기능: 리스트에 저장된 모든 데이터 삭제.
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
	
	/* Heap 소멸후 재 생성 */
	HeapDestroy(hHeap); // 위의 복잡한 코드들을 대체한다. 
	InitList();			// 성능도 향상되고, 에러 발생율도 낮다.

}

int main(int argc, char * argv[])
{
    InitList();
    
	printf("1, 2, 3 입력... \n");
	OrderedInsert(1);
	OrderedInsert(2);
	OrderedInsert(3);
    PrintAllList();

	printf("4, 5, 6 입력...\n");
	OrderedInsert(4);
	OrderedInsert(5);
	OrderedInsert(6);
    PrintAllList();

	printf("2와 5 삭제...\n");
	DeleteNode(2);
	DeleteNode(5);
    PrintAllList();

	printf("모든 리스트 삭제... \n");
	DeleteAll();

	printf("6, 5, 4 입력...\n");
	OrderedInsert(6);
	OrderedInsert(5);
	OrderedInsert(4);
    PrintAllList();

	printf("3, 2, 1 입력...\n");
	OrderedInsert(3);
	OrderedInsert(2);
	OrderedInsert(1);
    PrintAllList();

	printf("모든 리스트 삭제... \n");
	DeleteAll();

	printf("1, 3, 5 입력...\n");
	OrderedInsert(1);
	OrderedInsert(3);
	OrderedInsert(5);
    PrintAllList();

	printf("2, 4, 5 입력...\n");
	OrderedInsert(2);
	OrderedInsert(4);
	OrderedInsert(6);
    PrintAllList();

    return 0;
}


