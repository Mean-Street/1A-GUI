#include <stdio.h>
#include <stdlib.h>
#include "ei_event.h"
#include "ei_types.h"
#include "ei_application.h"
#include "ei_callback.h"
#include "ei_global.h"
#include "hw_interface.h"
#include "ei_button.h"

#define MAX(a,b) ((a > b)? a : b)

ei_bool_t EI_RESIZE_IN_PROGRESS = EI_FALSE;
ei_bool_t EI_MOVE_IN_PROGRESS = EI_FALSE;
ei_bool_t EI_CLOSE_IN_PROGRESS = EI_FALSE;
ei_widget_t* CLICKED_BUTTON = NULL;
ei_widget_t* CLICKED_TOPLEVEL = NULL;
ei_point_t CLICK_POS = {0, 0};
ei_widget_t* CLICKED_CLOSE_BUTTON = NULL;
ei_button_widget_t* SUNKEN_BUTTON = NULL;

ei_bool_t sunken_button(ei_widget_t* widget,ei_event_t* event,void* user_param)
{
    if(event->param.mouse.button_number == EI_MOUSEBUTTON_LEFT){
        SUNKEN_BUTTON = (ei_button_widget_t*) widget;
        SUNKEN_BUTTON->relief = ei_relief_sunken;
    }
    return EI_FALSE;
}

ei_bool_t raised_button(ei_widget_t* widget,ei_event_t* event,void* user_param)
{
    if(event->param.mouse.button_number == EI_MOUSEBUTTON_LEFT && SUNKEN_BUTTON != NULL){
        SUNKEN_BUTTON->relief = ei_relief_raised;
        SUNKEN_BUTTON = NULL;
    }
    return EI_FALSE;
}

/* detect if a resize has been started */
ei_bool_t resize_started(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	if (event->param.mouse.button_number == EI_MOUSEBUTTON_LEFT) {
		EI_RESIZE_IN_PROGRESS = EI_TRUE;
		CLICKED_BUTTON = widget;
		return EI_TRUE;
	}
	return EI_FALSE;
}

/* detect if a resize has been finished */
ei_bool_t resize_finished(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	if (event->param.mouse.button_number == EI_MOUSEBUTTON_LEFT && EI_RESIZE_IN_PROGRESS) {
		EI_RESIZE_IN_PROGRESS = EI_FALSE;
		CLICKED_BUTTON = NULL;
		return EI_TRUE;
	}
	return EI_FALSE;
}




ei_bool_t resize_window(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	if (EI_RESIZE_IN_PROGRESS) {
		/* computation of the new screen_location */
		ei_rect_t rect = CLICKED_BUTTON->parent->screen_location;

		rect.size.width = MAX(0, event->param.mouse.where.x - CLICKED_BUTTON->parent->content_rect->top_left.x);
		rect.size.height = MAX(0, event->param.mouse.where.y - CLICKED_BUTTON->parent->content_rect->top_left.y);
		/* registering in the toplevel's screen_location */
		(*(CLICKED_BUTTON->parent->wclass->geomnotifyfunc))(CLICKED_BUTTON->parent, rect);
		return EI_TRUE;
	}

	return EI_FALSE;
}




/* detect if a move has been started */
ei_bool_t move_started(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	if (event->param.mouse.button_number == EI_MOUSEBUTTON_LEFT) {
		EI_MOVE_IN_PROGRESS = EI_TRUE;
		CLICKED_TOPLEVEL = widget;
		CLICK_POS.x = event->param.mouse.where.x - widget->screen_location.top_left.x;
		CLICK_POS.y = event->param.mouse.where.y - widget->screen_location.top_left.y;
		return EI_TRUE;
	}
	return EI_FALSE;
}

/* detect if a move has been finished */
ei_bool_t move_finished(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	if (event->param.mouse.button_number == EI_MOUSEBUTTON_LEFT && EI_MOVE_IN_PROGRESS) {
		EI_MOVE_IN_PROGRESS = EI_FALSE;
		CLICKED_TOPLEVEL = NULL;
		return EI_TRUE;
	}
	return EI_FALSE;
}



ei_bool_t move_window(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	if (EI_MOVE_IN_PROGRESS) {
		/* computation of the new screen_location */
		ei_rect_t rect = *(CLICKED_TOPLEVEL->content_rect);
		rect.top_left.x = MAX(0, event->param.mouse.where.x) - CLICK_POS.x;
		rect.top_left.y = MAX(0, event->param.mouse.where.y) - CLICK_POS.y;
		/* registering in the toplevel's screen_location */
		(*(CLICKED_TOPLEVEL->wclass->geomnotifyfunc))(CLICKED_TOPLEVEL, rect);
		return EI_TRUE;
	}

	return EI_FALSE;
}



/* detect if a close has been started */
ei_bool_t close_started(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	if (event->param.mouse.button_number == EI_MOUSEBUTTON_LEFT) {
		CLICKED_CLOSE_BUTTON = widget;
		return EI_TRUE;
	}
	return EI_FALSE;
}



/* detect if a close has been finished */
ei_bool_t close_finished(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	if (event->param.mouse.button_number == EI_MOUSEBUTTON_LEFT && CLICKED_CLOSE_BUTTON != NULL) {
		if (widget == CLICKED_CLOSE_BUTTON) {
			ei_widget_destroy(CLICKED_CLOSE_BUTTON->parent);
		}
		CLICKED_CLOSE_BUTTON = NULL;
		return EI_TRUE;
	}
	return EI_FALSE;
}



ei_bool_t process_callback(ei_widget_t* widget,ei_event_t* event,global_callback* callback)
{
    linked_user_param* user_param = NULL;
    while(callback != NULL){
        user_param = callback->user_param;
        while(user_param != NULL){
            if((*(callback->callback))(widget,event,user_param->user_param) == EI_TRUE)
                return EI_TRUE;
            user_param = user_param->next;
        }
        callback = callback->next;
    }
    return EI_FALSE;
}

ei_widget_t* find_widget(uint32_t pick_id,ei_widget_t* widget)
{
    ei_widget_t* clicked;
    while(widget != NULL){
        if(widget->pick_id == pick_id)
            return widget;
        if(widget->children_head != NULL){
            if((clicked=find_widget(pick_id,widget->children_head)) != NULL)
                return clicked;
        }
        widget = widget->next_sibling;
    }
    return NULL;
}



static uint32_t set_pick_id(uint8_t* color){
    uint32_t x = 0;
    int ir,ig,ib,ia;
    get_channel(&ir,&ig,&ib,&ia);
    x = *(color+ir)<<(8*ir);
    x = *(color+ig)<<(8*ig) | x;
    x = *(color+ib)<<(8*ib) | x;
    return x;
}



static uint8_t* get_buffer(ei_surface_t offscreen,ei_point_t clicked,uint32_t width)
{
    uint8_t* color;
    hw_surface_lock(offscreen);
    color = hw_surface_get_buffer(offscreen) + (clicked.y)*width*4 + clicked.x*4;
    hw_surface_unlock(offscreen);
    return color;
}



static ei_widget_t* get_widget(ei_event_t* event,ei_surface_t* offscreen)
{
    uint8_t* color;
    uint32_t x;
    ei_point_t clicked;
    ei_widget_t* widget;
    if(event->type == ei_ev_mouse_buttondown || event->type == ei_ev_mouse_buttonup || event->type == ei_ev_mouse_move){
        clicked = event->param.mouse.where;
        widget = ei_app_root_widget();
        color = get_buffer(*offscreen,clicked,widget->screen_location.size.width);
        x = set_pick_id(color);
        widget = find_widget(set_pick_id(color),ei_app_root_widget());
        return widget;
    }
    else 
        return NULL;
}

static ei_bool_t process_event_all(ei_widget_t* widget,ei_event_t* event,ei_surface_t* offscreen)
{
    linked_eventtype* types = get_simple_events();
    if(event == NULL){
        fprintf(stderr,"Event is set to NULL.\n");
        return EI_FALSE;
    }
    while(types != NULL){
        if(types->eventtype == event->type){
            if(process_callback(widget,event,types->callback))
                return EI_TRUE;
        }
        types = types->next;
    }
    return EI_FALSE;
}

static ei_bool_t process_event_class(ei_widget_t* widget,ei_event_t* event,ei_surface_t* offscreen)
{
    linked_class* class_events = get_simple_class_events(widget->wclass->name);
    if(class_events == NULL){
        fprintf(stderr,"The class of this widget is not registered.\n");
        return EI_FALSE;
    }
    linked_eventtype* types = class_events->types;
    global_callback* callback = NULL;
    linked_widget* linked_widget;
    while(types != NULL){
        if(types->eventtype == event->type){
            callback = types->callback;
            while(callback != NULL){
                linked_widget = class_events->widget;
                while(linked_widget != NULL){
                    if(linked_widget->widget == widget){
                        if(process_callback(widget,event,callback))
                            return EI_TRUE;
                    }
                    linked_widget = linked_widget->next;
                }
                callback = callback->next;
            }
        }
        types = types->next;
    }
    return EI_FALSE;
}

static ei_bool_t process_event_widget(ei_widget_t* widget,ei_event_t* event,ei_surface_t* offscreen)
{
    linked_event* linked_event = get_simple_class_widget_events(widget->wclass->name);
    if(linked_event == NULL){
        fprintf(stderr,"The class of this widget is not registered.\n");
        return EI_FALSE;
    }
    linked_widget_type* type = linked_event->types;
    linked_widget_events* widget_events = NULL;
    while(type != NULL){
        if(type->eventtype == event->type){
            widget_events = type->widget_event;
            while(widget_events != NULL){
                if(widget_events->widget == widget){
                    if(process_callback(widget,event,widget_events->callback))
                        return EI_TRUE;
                }
                widget_events = widget_events->next;
            }
        }
        type = type->next;
    }
    return EI_FALSE;
}

void process_event(ei_event_t* event,ei_surface_t* offscreen)
{
    ei_widget_t* widget = get_widget(event,offscreen);
    if(process_event_all(widget,event,offscreen))
        return;
    if(widget == NULL)
        return;
    if(process_event_class(widget,event,offscreen))
        return;
    process_event_widget(widget,event,offscreen);
}
