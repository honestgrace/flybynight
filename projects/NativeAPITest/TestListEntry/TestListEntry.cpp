// TestListEntry.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define CONTAINING_RECORD(address, type, field) (\
    (type *)((char*)(address) -(unsigned long)(&((type *)0)->field)))

typedef struct _LIST_ENTRY
{
	struct _LIST_ENTRY *FLink;
	struct _LIST_ENTRY *BLink;
} LIST_ENTRY, *PLIST_ENTRY;

typedef struct _MY_STRUCT
{
	int x = 0;
	int y = 0;
	int z = 0;

	LIST_ENTRY link;
} MY_STRUCT, *PMY_STRUCT;


void IntializeListEntry(PLIST_ENTRY listHead)
{
	listHead->FLink = listHead->BLink = listHead;
}

LIST_ENTRY head;

int main()
{
	IntializeListEntry(&head);

	MY_STRUCT node1;
	node1.x = 1;
	node1.y = 1;
	node1.z = 1;
	node1.link.FLink = head.FLink;
	head.FLink = &node1.link;
	node1.link.BLink = &head;

	MY_STRUCT node2;
	node2.x = 2;
	node2.y = 2;
	node2.z = 2;
	
	node2.link.FLink = node1.link.FLink;
	node1.link.FLink = &node2.link;
	node2.link.BLink = &node1.link;

	MY_STRUCT node3;
	node3.x = 3;
	node3.y = 3;
	node3.z = 3;
	node3.link.FLink = node2.link.FLink;
	node2.link.FLink = &node3.link;
	node3.link.BLink = &node2.link;

	MY_STRUCT *p = nullptr;

	//node1
	p = CONTAINING_RECORD(head.FLink, MY_STRUCT, link);
	printf("%d\r\n", p->y);
	if (p->link.FLink == &head)
	{
		printf("end node reached");
	}
	else
	{
		printf("not yet");
	}

	//node2
	p = CONTAINING_RECORD(head.FLink->FLink, MY_STRUCT, link);
	printf("%d\r\n", p->y);
	if (p->link.FLink == &head)
	{
		printf("end node reached");
	}
	else
	{
		printf("not yet");
	}

	//node3
	p = CONTAINING_RECORD(head.FLink->FLink->FLink, MY_STRUCT, link);
	printf("%d\r\n", p->y);
	if (p->link.FLink == &head)
	{
		printf("end node reached");
	}
	else
	{
		printf("not yet");
	}

	MY_STRUCT A;
	PMY_STRUCT pA = &A;
	pA->x = 1;
	pA->y = 2;
	pA->z = 3;
	
	void * address = (void *)&(pA->z);

	PMY_STRUCT pB = CONTAINING_RECORD(address, MY_STRUCT, z);
	printf("%d", pB->y);

    return 0;
}

