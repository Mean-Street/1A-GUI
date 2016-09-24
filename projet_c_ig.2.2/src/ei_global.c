#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ei_widgetclass.h"
#include "ei_global.h"
#include "ei_utils.h"

static ei_widgetclass_t* CLASSES = NULL;

static linked_class* CLASS_EVENTS = NULL;
static linked_event* WIDGET_EVENTS = NULL;
static linked_eventtype* EVENTS = NULL;
static int IR,IG,IB,IA;

static rects_list INVALID_RECTS = {NULL, NULL};



linked_class** get_widget_classes(void)
{
    return &CLASS_EVENTS;
}

linked_eventtype** get_events(void)
{
    return &EVENTS;
}

linked_eventtype* get_simple_events(void)
{
    return EVENTS;
}

ei_widgetclass_t** get_classes(void)
{
    return &CLASSES;
}

ei_widgetclass_t* get_simple_classes(void)
{
    return CLASSES;
}

linked_class** get_class_events(ei_tag_t tag)
{
    linked_class** tmp = get_widget_classes();
    while(*tmp != NULL){
        if(strcmp((*tmp)->name,tag) == 0)
            return tmp;
        tmp = &((*tmp)->next);
    }
    fprintf(stderr,"This tag doesn't exist.\n");
    return NULL;
}

linked_event** get_class_widget_events(ei_tag_t tag)
{
    linked_event** tmp = get_widget_events();
    while(*tmp != NULL){
        if(strcmp((*tmp)->name,tag) == 0)
            return tmp;
        tmp = &((*tmp)->next);
    }
    fprintf(stderr,"This tag doesn't exist.\n");
    return NULL;
}

linked_class* get_simple_class_events(ei_widgetclass_name_t name)
{
    linked_class** tmp = get_widget_classes();
    while(*tmp != NULL){
        if(strcmp((*tmp)->name,name) == 0)
            return *tmp;
        tmp = &((*tmp)->next);
    }
    fprintf(stderr,"This tag doesn't exist.\n");
    return NULL;
}

linked_event* get_simple_class_widget_events(ei_tag_t tag)
{
    linked_event** tmp = get_widget_events();
    while(*tmp != NULL){
        if(strcmp((*tmp)->name,tag) == 0)
            return *tmp;
        tmp = &((*tmp)->next);
    }
    fprintf(stderr,"This tag doesn't exist.\n");
    return NULL;
}

linked_event** get_widget_events(void)
{
    return &WIDGET_EVENTS;
}

void get_channel(int* ir,int* ig,int* ib,int* ia)
{
    *ir = IR;
    *ig = IG;
    *ib = IB;
    *ia = IA;
}

void set_channel(int ir,int ig,int ib,int ia)
{
    IR = ir;
    IG = ig;
    IB = ib;
    IA = ia;
}


rects_list* get_invalid_rects_list(void)
{
	return &INVALID_RECTS;
}


void add_invalid_rect(ei_rect_t rect)
{
	linked_rect* added_rect = malloc(sizeof(linked_rect));
	added_rect->rect = ei_rect(rect.top_left, rect.size);
	added_rect->next = NULL;
	added_rect->pred = INVALID_RECTS.tail;
	
	if (INVALID_RECTS.head == NULL) {
		INVALID_RECTS.head = added_rect;
		INVALID_RECTS.tail = added_rect;
	} else {
		INVALID_RECTS.tail->next = added_rect;
		INVALID_RECTS.tail = added_rect;
	}
}


ei_rect_t* read_next_invalid_rect(void)
{
	printf("Dans read_next.\n");
	if (INVALID_RECTS.head == NULL) {
		printf("No invalid rect left.\n");
		return NULL;
	}

	linked_rect* to_delete = INVALID_RECTS.head;
	ei_rect_t* dest = malloc(sizeof(ei_rect_t));
	*dest = ei_rect(to_delete->rect.top_left, to_delete->rect.size);

	if (INVALID_RECTS.head == INVALID_RECTS.tail) {
		INVALID_RECTS.head = NULL;
		INVALID_RECTS.tail = NULL;
	} else {
		INVALID_RECTS.head = to_delete->next;
		to_delete->next->pred = NULL;
	}
	free(to_delete);
	return dest;
}


static void free_class_events(){
    linked_class* tmp_lc = CLASS_EVENTS;
    while(tmp_lc != NULL) {
        linked_widget* tmp_lw = tmp_lc->widget;
        while(tmp_lw != NULL) {
            tmp_lc->widget = tmp_lw->next;
            free(tmp_lw);
            tmp_lw = tmp_lc->widget;
        }
        linked_eventtype* tmp_let = tmp_lc->types;
        while(tmp_let != NULL) {
            global_callback* tmp_gc = tmp_let->callback;
            while(tmp_gc != NULL) {
                linked_user_param* tmp_lup = tmp_gc->user_param;
                while(tmp_lup != NULL) {
                    tmp_gc->user_param = tmp_lup->next;
                    free(tmp_lup);
                    tmp_lup = tmp_gc->user_param;
                }
                tmp_let->callback = tmp_gc->next;
                free(tmp_gc);
                tmp_gc = tmp_let->callback;

            }
            tmp_lc->types = tmp_let->next;
            free(tmp_let);
            tmp_let = tmp_lc->types;
        }
        CLASS_EVENTS = tmp_lc->next;
        free(tmp_lc);
        tmp_lc = CLASS_EVENTS;
    }
}


static void free_widget_events() {
    linked_event* tmp_le = WIDGET_EVENTS;
    while(tmp_le != NULL){
        linked_widget_type* tmp_lwt = tmp_le->types;
        while(tmp_lwt != NULL){
            linked_widget_events* tmp_lwe = tmp_lwt->widget_event;
            while(tmp_lwe != NULL){
                global_callback* tmp_gc = tmp_lwe->callback;
                while(tmp_gc != NULL) {
                    linked_user_param* tmp_lup = tmp_gc->user_param;
                    while(tmp_lup != NULL) {
                        tmp_gc->user_param = tmp_lup->next;
                        free(tmp_lup);
                        tmp_lup = tmp_gc->user_param;
                    }
                    tmp_lwe->callback = tmp_gc->next;
                    free(tmp_gc);
                    tmp_gc = tmp_lwe->callback;

                }
                tmp_lwt->widget_event = tmp_lwe->next;
                free(tmp_lwe);
                tmp_lwe = tmp_lwt->widget_event;
            }
            tmp_le->types = tmp_lwt->next;
            free(tmp_lwt);
            tmp_lwt = tmp_le->types;
        }
        WIDGET_EVENTS = tmp_le->next;
        free(tmp_le);
        tmp_le = WIDGET_EVENTS;
    }
}

static void free_events(){
    linked_eventtype* tmp_let = EVENTS;
        while(tmp_let != NULL) {
            global_callback* tmp_gc = tmp_let->callback;
            while(tmp_gc != NULL) {
                linked_user_param* tmp_lup = tmp_gc->user_param;
                while(tmp_lup != NULL) {
                    tmp_gc->user_param = tmp_lup->next;
                    free(tmp_lup);
                    tmp_lup = tmp_gc->user_param;
                }
                tmp_let->callback = tmp_gc->next;
                free(tmp_gc);
                tmp_gc = tmp_let->callback;

            }
            EVENTS = tmp_let->next;
            free(tmp_let);
            tmp_let = EVENTS;
        }
}

void free_bind_struct() {
    free_class_events();
    free_widget_events();
    free_events();
}
