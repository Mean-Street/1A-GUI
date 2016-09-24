#include <stdlib.h>
#include <stdio.h>
#include "ei_types.h"
#include "ei_global.h"

int main(void)
{

	void display_rect(ei_rect_t* tmp)
	{	
		if (tmp != NULL) {
			printf("===================\n");
			printf("	x : %d\n", tmp->top_left.x);
			printf("	y : %d\n", tmp->top_left.y);
			printf("	width : %d\n", tmp->size.width);
			printf("	height : %d\n", tmp->size.height);
		} else {
			printf("Pointeur nul.\n");
		}
	}

	void display_list(void)
	{
		rects_list* L = get_invalid_rects_list();
		linked_rect* tmp = L->head;
		while (tmp != NULL) {
			display_rect(&(tmp->rect));
			tmp = tmp->next;
		}
		printf("===================\n\n\n");
	}


	ei_rect_t r1 = {{1,2}, {10, 11}};
	ei_rect_t r2 = {{2,3}, {20, 21}};
	ei_rect_t r3 = {{3,4}, {30, 31}};
	add_invalid_rect(r1);
	add_invalid_rect(r2);
	add_invalid_rect(r3);
	display_list();

	ei_rect_t* res = read_next_invalid_rect();
	printf("res : \n");
	display_rect(res);
	printf("\n\n\n");


	res = read_next_invalid_rect();
	printf("res : \n");
	display_rect(res);
	printf("\n\n\n");


	res = read_next_invalid_rect();
	printf("res : \n");
	display_rect(res);
	printf("\n\n\n");


	res = read_next_invalid_rect();
	printf("res : \n");
	display_rect(res);
	printf("\n\n\n");


	res = read_next_invalid_rect();
	printf("res : \n");
	display_rect(res);
	printf("\n\n\n");


}
