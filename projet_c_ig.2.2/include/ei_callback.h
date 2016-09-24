#ifndef EI_CALLBACK_H
#define EI_CALLBACK_H

#include "ei_types.h"
#include "ei_widget.h"
#include "ei_event.h"

ei_bool_t close_window(ei_widget_t* widget,ei_event_t* event,void* user_param);


ei_bool_t resize_started	(ei_widget_t* widget, ei_event_t* event, void* user_param);
ei_bool_t resize_finished	(ei_widget_t* widget, ei_event_t* event, void* user_param);
ei_bool_t resize_window		(ei_widget_t* widget, ei_event_t* event, void* user_param);


ei_bool_t move_started		(ei_widget_t* widget, ei_event_t* event, void* user_param);
ei_bool_t move_finished		(ei_widget_t* widget, ei_event_t* event, void* user_param);
ei_bool_t move_window		(ei_widget_t* widget, ei_event_t* event, void* user_param);


ei_bool_t close_started		(ei_widget_t* widget, ei_event_t* event, void* user_param);
ei_bool_t close_finished	(ei_widget_t* widget, ei_event_t* event, void* user_param);


void process_event(ei_event_t* event,ei_surface_t* offscreen);

ei_bool_t sunken_button(ei_widget_t* widget,ei_event_t* event,void* user_param);

ei_bool_t raised_button(ei_widget_t* widget,ei_event_t* event,void* user_param);

#endif
