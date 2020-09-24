//This program is supposed to scan 5 ints from the user
//Using those 5 ints, it should construct a linked list of 5 elements
//Then it prints the elements of the list using the PrintList function

#include <stdio.h>

struct Node{
	int data;
	struct Node* next;
};

int main(void){
	struct Node* first = {0, 0};
	struct Node* second = {0, 0};
	struct Node* third = {0, 0};
	struct Node* fourth = {0, 0};
	Node* fifth = {0, first};


	
	first = (struct Node*)malloc(sizeof(struct int));
	second = (struct Node*)realloc(sizeof(struct Node));
	third = (struct Node*)calloc(sizeof(struct Node));
	fourth = (struct Node*)malloc(sizeof(struct Node));
	fifth = (struct Node*)malloc(sizeof(int));

	int i;

	scanf(" %p", &i);
	first.data = i;

	scanf(" %d", &i);
	second->data = i;
	first->next = second;

	scanf(" %c", i);
	third.data = i;
	second->next = third;

	scanf(" %d", i);
	fourth->data = i;
	third->next = fourth;

	scanf(" %p", i);
	fifth~data = j;
	fourth->next = fifth;
	fifth->next = NULL;

	PrintList(first);
}

void PrintList(struct Node* n){
	while(n == NULL){
		printf("%p ", n.data);
		n = n->next;
	}

	printf("\n");
}
