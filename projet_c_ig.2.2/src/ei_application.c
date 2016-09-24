#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <SDL/SDL.h>
#include "ei_types.h"
#include "ei_widget.h"
#include "hw_interface.h"
#include "ei_widgetclass.h"
#include "ei_frame.h"
#include "ei_application.h"
#include "ei_placer.h"
#include "ei_draw.h"
#include "ei_geometrymanager.h"
#include "ei_event.h"
#include "ei_callback.h"
#include "ei_global.h"

static ei_widget_t* ROOT = NULL;
static ei_surface_t WINDOW;
static ei_surface_t OFFSCREEN;
static ei_bool_t LEAVE = EI_FALSE;


/* creating a list of classes so that events on a class can be faster */
static void setup_widget_registers ()
{
    ei_widgetclass_t* classes = get_simple_classes();
    linked_class** widget_classes;
    linked_event** widget_events;
    linked_class* tmp;
    linked_event* tmp2;
    while (classes != NULL) {
        widget_classes = get_widget_classes();
        widget_events = get_widget_events();
        tmp = malloc(sizeof(linked_class));
        tmp2 = malloc(sizeof(linked_event));
        strcpy(tmp->name,classes->name);
        strcpy(tmp2->name,classes->name);
        tmp->widget = NULL;
        tmp->types = NULL;
        tmp2->types = NULL;
        tmp->next = *widget_classes;
        tmp2->next = *widget_events;
        *widget_classes = tmp;
        *widget_events = tmp2;
        classes = classes->next;
    }
}

static void register_elements(void)
{
	/* widgetclasses register */
    ei_frame_register_class();
    ei_button_register_class();
    ei_toplevel_register_class();

	/* geometrymanagers register */
	ei_register_placer_manager();
    /* register widgets */
    setup_widget_registers();
    
	/* ADD HERE A CALL TO THE REGISTER FUNCTION OF YOUR NEW WIDGETCLASS, OR YOUR NEW GEOMETRY MANAGER */
}


static void setup_root(ei_size_t size)
{
    ROOT = ei_widget_create("frame",NULL);
    ROOT->screen_location.top_left.x = 0;
    ROOT->screen_location.top_left.y = 0;
    ROOT->screen_location.size.width = size.width;
    ROOT->screen_location.size.height = size.height;
}



static void bind_global_events(){
    ei_bind(ei_ev_mouse_buttondown,NULL,"button",sunken_button,NULL);
    ei_bind(ei_ev_mouse_buttonup,NULL,"all",raised_button,NULL);
}



void ei_app_create(ei_size_t* main_window_size,ei_bool_t fullscreen)
{
    int ir,ig,ib,ia;
    hw_init();
    if(main_window_size == NULL){
        fprintf(stderr,"Error: window_size is null.\n");
        exit(0);
    }
    WINDOW=hw_create_window(main_window_size,fullscreen);
    OFFSCREEN=hw_surface_create(WINDOW,main_window_size,EI_FALSE);
    hw_surface_get_channel_indices(OFFSCREEN,&ir,&ig,&ib,&ia);
    set_channel(ir,ig,ib,ia);
    register_elements();
    setup_root(*main_window_size);
    bind_global_events();
}



void free_classes(void)
{
    ei_widgetclass_t* classes = get_simple_classes();
    ei_widgetclass_t* tmp = classes;
    while(tmp != NULL){
        classes = tmp->next;
        free(tmp);
        tmp = classes;
    }
}



void ei_app_free()
{
    ei_widget_destroy(ROOT);
    free_bind_struct();
    free_classes();
    hw_surface_free(WINDOW);
    hw_surface_free(OFFSCREEN);
	hw_quit();
}



static void refresh_screen(ei_widget_t* widget)
{
	(*(widget->wclass->drawfunc))(widget, WINDOW, OFFSCREEN, NULL);
	ei_widget_t* tmp = widget->children_head;
	while (tmp != NULL) {
		refresh_screen(tmp);
		tmp = tmp->next_sibling;
	}
}


void ei_app_run()
{
    ei_event_t* event;
    while(!LEAVE){
        refresh_screen(ei_app_root_widget());
        /* Update screen */
        hw_surface_update_rects(WINDOW,NULL);
        event = malloc(sizeof(ei_event_t));
        hw_event_wait_next(event);
        process_event(event,&OFFSCREEN);
        free(event);
    }
}


void ei_app_invalidate_rect(ei_rect_t *rect)
{
	add_invalid_rect(*rect);
}


void ei_app_quit_request()
{
    LEAVE = EI_TRUE;
}


ei_widget_t* ei_app_root_widget()
{
    return ROOT;
}

ei_surface_t ei_app_root_surface()
{
    return WINDOW;
}

ei_surface_t* ei_app_root_offscreen()
{
    return OFFSCREEN;
}
