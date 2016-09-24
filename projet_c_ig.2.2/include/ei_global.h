#ifndef EI_GLOBAL_H
#define EI_GLOBAL_H

#include "ei_widgetclass.h"
#include "ei_widget.h"
#include "ei_event.h"

/* We can have multiple user_param for each callback so we need to chain those */
typedef struct linked_user_param {
    void* user_param;
    struct linked_user_param* next;
} linked_user_param;


/* Same than below for callbacks defined on a class or all */
typedef struct global_callback {
    ei_callback_t callback;
    linked_user_param* user_param;
    struct global_callback* next;
} global_callback;

/* We are linking the widgets to easily callback everyone of them */
typedef struct linked_widget {
    ei_widget_t* widget;
    struct linked_widget* next;
} linked_widget;


/* We are linking classes so that we can find which class is associated to a class
 * In this struct we also need to link widgets so that we every widgets inside a class to callback when we need to.
 * We also need to store all the events associated to this class.
*/

typedef struct linked_eventtype {
    ei_eventtype_t eventtype;
    global_callback* callback;
    struct linked_eventtype* next;
} linked_eventtype;

typedef struct linked_widget_events {
    ei_widget_t* widget;
    global_callback* callback;
    struct linked_widget_events* next;
} linked_widget_events;

typedef struct linked_widget_type {
    ei_eventtype_t eventtype;
    linked_widget_events* widget_event;
    struct linked_widget_type* next;
} linked_widget_type;

typedef struct linked_event {
    ei_widgetclass_name_t name;
    linked_widget_type* types;
    struct linked_event* next;
} linked_event;

typedef struct linked_class {
    ei_widgetclass_name_t name;
    linked_widget* widget;
    linked_eventtype* types;
    struct linked_class* next;
} linked_class;

linked_class** get_widget_classes(void);

linked_eventtype** get_events(void);

linked_eventtype* get_simple_events(void);

ei_widgetclass_t** get_classes(void);

ei_widgetclass_t* get_simple_classes(void);

linked_class** get_class_events(ei_tag_t tag);

linked_event** get_class_widget_events(ei_tag_t tag);

linked_class* get_simple_class_events(ei_widgetclass_name_t name);

linked_event* get_simple_class_widget_events(ei_tag_t tag);

linked_event** get_widget_events(void);

void get_channel(int* ir,int* ig,int* ib,int* ia);

void set_channel(int ir,int ig,int ib, int ia);

ei_surface_t* ei_app_root_offscreen(void);


/*************************************
 * linked list of invalid rectangles *
 *************************************/

typedef struct linked_rect {
	ei_rect_t rect;
	struct linked_rect* next;
	struct linked_rect* pred;
} linked_rect;

typedef struct rects_list {
	linked_rect* head;
	linked_rect* tail;
} rects_list;


rects_list* get_invalid_rects_list(void);

void add_invalid_rect(ei_rect_t rect);

ei_rect_t* read_next_invalid_rect(void);

void free_bind_struct(void);

#endif
