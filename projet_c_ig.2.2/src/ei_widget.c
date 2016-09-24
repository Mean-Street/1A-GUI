#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ei_types.h"
#include "ei_widget.h"
#include "hw_interface.h"
#include "ei_frame.h"
#include "ei_button.h"
#include "ei_toplevel.h"
#include "ei_widgetclass.h"
#include "ei_callback.h"
#include "ei_geometrymanager.h"
#include "ei_placer.h"
#include "ei_global.h"
#include "ei_event.h"

static uint32_t PICK_COLOR=0x0000000;

void ei_frame_configure(ei_widget_t* widget,
                        ei_size_t* requested_size,
                        const ei_color_t* color,
                        int* border_width,
                        ei_relief_t* relief,
                        char** text,
                        ei_font_t* text_font,
                        ei_color_t* text_color,
                        ei_anchor_t* text_anchor,
                        ei_surface_t* img,
                        ei_rect_t** img_rect,
                        ei_anchor_t* img_anchor)
{
    if (widget == NULL){
        fprintf(stderr,"widget is set to NULL\n");
        return;
    }
    ei_frame_widget_t* frame = (ei_frame_widget_t*) widget;
    if (requested_size != NULL)
        memcpy(&(frame->widget.requested_size),requested_size,sizeof(ei_size_t));
    if (color != NULL)
        frame->color = *color;
    if (border_width != NULL)
        frame->border_width = *border_width;
    if (relief != NULL)
        frame->relief=*relief;
    if (text_font != NULL)
        frame->text_font=*text_font;
    if (text != NULL){
        if(*text == NULL){
            frame->text = NULL;
        }
        else {
            if(frame->text == NULL)
                frame->text=malloc((strlen(*text)+1)*sizeof(char));
            strcpy(frame->text,*text);
            int width,height;
            hw_text_compute_size(*text,frame->text_font,&width,&height);
            if(width > frame->widget.requested_size.width)
                frame->widget.requested_size.width = width;
            if(height > frame->widget.requested_size.height)
                frame->widget.requested_size.height = height;
            if(img != NULL){
                fprintf(stderr,"A frame can't have image and text.\n");
                img = NULL;
            }
        }
    }
    if (text_color != NULL)
        frame->text_color=*text_color;
    if (text_anchor != NULL)
        frame->text_anchor=*text_anchor;
    if (img != NULL){
        frame->img=*img;
    }
    if (img_rect != NULL && *img_rect != NULL){
        frame->img_rect = malloc(sizeof(ei_rect_t));
        memcpy(frame->img_rect,*img_rect,sizeof(ei_rect_t));
    }
    if (img_anchor != NULL)
        frame->img_anchor=*img_anchor;
}



void ei_button_configure(ei_widget_t* widget,
                        ei_size_t* requested_size,
                        const ei_color_t* color,
                        int* border_width,
                        int* corner_radius,
                        ei_relief_t* relief,
                        char** text,
                        ei_font_t* text_font,
                        ei_color_t* text_color,
                        ei_anchor_t* text_anchor,
                        ei_surface_t* img,
                        ei_rect_t** img_rect,
                        ei_anchor_t* img_anchor,
                        ei_callback_t* callback,
                        void** user_param)
{
    if (widget == NULL){
        fprintf(stderr,"widget is set to NULL\n");
        return;
    }
    ei_button_widget_t* button = (ei_button_widget_t*) widget;
    if (color != NULL) 
		button->color = *color;
    if (border_width != NULL) 
        button->border_width = *border_width;
    if (corner_radius != NULL) 
        button->corner_radius = *corner_radius;
    if (relief != NULL) 
        button->relief=*relief;
    if (text_font != NULL) 
        button->text_font=*text_font;
    if (text != NULL){
        button->text = malloc((strlen(*text)+1)*sizeof(char));
        strcpy(button->text,*text);
        int width,height;
        hw_text_compute_size(*text,button->text_font,&width,&height);
        if(width > button->widget.requested_size.width)
            button->widget.requested_size.width = width;
        if(height > button->widget.requested_size.height)
            button->widget.requested_size.height = height;
        if(img != NULL){
            fprintf(stderr,"A button can't have image and text.\n");
            img = NULL;
        }
    }
    if (text_color != NULL)
        button->text_color=*text_color;
    if (text_anchor != NULL)
        button->text_anchor=*text_anchor;
    if (img != NULL){
        button->widget.requested_size = hw_surface_get_size(*img);
        button->img=*img;
    }
    if (img_rect != NULL){
        button->img_rect = malloc(sizeof(ei_rect_t));
        memcpy(button->img_rect,*img_rect,sizeof(ei_rect_t));
        button->widget.requested_size = (*img_rect)->size;
    }
    if (requested_size != NULL)
        memcpy(&(button->widget.requested_size),requested_size,sizeof(ei_size_t));
    if (img_anchor != NULL)
        button->img_anchor=*img_anchor;
    if(user_param != NULL){
        button->user_param = *user_param;
    }
    if(callback != NULL){
        button->callback = callback;
        ei_bind(ei_ev_mouse_buttondown,widget,NULL,*callback,button->user_param);
    }
}



static void ei_toplevel_create_central_frame(ei_toplevel_widget_t* toplevel)
{
	/* initialisation */
	ei_widget_t* frame = ei_widget_create("frame", (ei_widget_t*)toplevel);
	/* configuration */
	int border_width = 0;
	ei_relief_t relief = ei_relief_raised;
	ei_frame_configure(frame, NULL, &(toplevel->color), &border_width, &relief,
						NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}



// Don't know what to do with that motherfuckin' function
static void ei_toplevel_create_close_button(ei_toplevel_widget_t* toplevel)
{
	if (toplevel->closable) {
		/* initialisation */
		ei_widget_t* button = ei_widget_create("button", (ei_widget_t*)toplevel);
		/* configuration */
		int text_height;
		hw_text_compute_size(toplevel->title, ei_default_font, NULL, &text_height);
		ei_color_t color = {0xff, 0x00, 0x00, 0xff};
		int border_width = 3;
		int radius = (toplevel->border_width == 0) ? text_height/2 - 2 :
			text_height/2 - 2*toplevel->border_width;

		ei_button_configure(button, NULL, &color, &border_width, &radius, NULL,
							NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

		ei_bind(ei_ev_mouse_buttondown, button, NULL, &(close_started), NULL);
		ei_bind(ei_ev_mouse_buttonup, NULL, "all", &(close_finished), NULL);
	}
}



static void ei_toplevel_create_resize_button(ei_toplevel_widget_t* toplevel)
{
	if (toplevel->resizable) {
		/* initialisation */
		ei_widget_t* button = ei_widget_create("button", (ei_widget_t*)toplevel);
		/* configuration */

		ei_color_t color = {0x2f, 0x2f, 0x2f, 0xff};
		int border_width = 0;
		int radius = 0;
		ei_button_configure(button, NULL, &color, &border_width, &radius, NULL,
							NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

		ei_bind(ei_ev_mouse_buttondown, button, NULL, &(resize_started), NULL);
		ei_bind(ei_ev_mouse_buttonup, NULL, "all", &(resize_finished), NULL);
		ei_bind(ei_ev_mouse_move, NULL, "all", &(resize_window), NULL);
	}
}




void ei_toplevel_configure (ei_widget_t* widget,
                            ei_size_t* requested_size,
                            ei_color_t* color,
                            int* border_width,
                            char** title,
                            ei_bool_t* closable,
                            ei_axis_set_t* resizable,
                            ei_size_t** min_size)
{
    if (widget == NULL){
        fprintf(stderr,"widget is set to NULL\n");
        return;
    }

    ei_toplevel_widget_t* toplevel = (ei_toplevel_widget_t*) widget;

    if (requested_size != NULL)
        memcpy(&(toplevel->widget.requested_size),requested_size,sizeof(ei_size_t));
    if (color != NULL) toplevel->color = *color;
    if (border_width != NULL) 
        toplevel->border_width = *border_width;
    if (title != NULL){
        toplevel->title=malloc((strlen(*title)+1)*sizeof(char));
        strcpy(toplevel->title,*title);
        int width,height;
        hw_text_compute_size(*title,ei_default_font,&width,&height);
        if(width > toplevel->widget.requested_size.width)
            toplevel->widget.requested_size.width = width;
        if(height > toplevel->widget.requested_size.height)
            toplevel->widget.requested_size.height = height;
    }
    if(closable != NULL)
        toplevel->closable = *closable;
    if(resizable != NULL)
        toplevel->resizable = *resizable;
    if(min_size != NULL && *min_size != NULL){
        toplevel->min_size.width = (**min_size).width;
        toplevel->min_size.height = (**min_size).height;
    }

	ei_toplevel_create_central_frame(toplevel);
	ei_toplevel_create_close_button(toplevel);
	ei_toplevel_create_resize_button(toplevel);

	ei_bind(ei_ev_mouse_buttondown, NULL, "toplevel", &(move_started), NULL);
	ei_bind(ei_ev_mouse_buttonup, NULL, "all", &(move_finished), NULL);
	ei_bind(ei_ev_mouse_move, NULL, "all", &(move_window), NULL);
}



static void set_default_widget(ei_widget_t*  widget)
{
    int ir,ig,ib,ia;
    widget->next_sibling = NULL;
    widget->children_head = NULL;
    widget->children_tail = NULL;
    widget->geom_params = NULL;
    widget->screen_location.top_left.x = 0;
    widget->screen_location.top_left.y = 0;
    widget->screen_location.size.width = 0;
    widget->screen_location.size.height = 0;
    widget->content_rect = &(widget->screen_location);
    widget->pick_color = malloc(sizeof(ei_color_t));
    ei_color_t pick_color;
    get_channel(&ir,&ig,&ib,&ia);
    pick_color.red =    (PICK_COLOR & (0xff<<(8*ir)))>>(8*ir);
    pick_color.green =  (PICK_COLOR & (0xff<<(8*ig)))>>(8*ig);
    pick_color.blue =   (PICK_COLOR & (0xff<<(8*ib)))>>(8*ib);
    pick_color.alpha = 0xFF;
    *(widget->pick_color) = pick_color;
    widget->pick_id = PICK_COLOR;
    PICK_COLOR+=1;
}




ei_widget_t* ei_widget_create(ei_widgetclass_name_t class_name,ei_widget_t* parent)
{
	if (parent != NULL && strcmp(parent->wclass->name, "toplevel") == 0) {
		if (strcmp(class_name, "toplevel") == 0) {
			fprintf(stderr, "Error : you cannot create a toplevel in a toplevel.\n");
			return NULL;
		}
		ei_widget_create(class_name, parent->children_head);
	}

    /* Find the class of widget and call default,alloc of class */
    ei_widgetclass_t* wclass = ei_widgetclass_from_name(class_name);
    if(wclass == NULL) {
		printf("wclass nul.\n");
        return NULL;
	}
    ei_widget_t* widget = (ei_widget_t*) (*(wclass->allocfunc))();
    (*wclass->setdefaultsfunc)(widget);

    /* Set the default values for common parameters */
    widget->wclass = wclass;
    widget->parent = parent;
    set_default_widget(widget);

    /* linking widgets */
    if(parent != NULL){
        if(parent->children_head == NULL)
            parent->children_head = widget;
        else
            parent->children_tail->next_sibling = widget;
        parent->children_tail = widget;
    }

    /* Setting up a linked list of widgets by classes for events */
    linked_class** widget_classes = get_class_events(widget->wclass->name);
    linked_widget* tmp = malloc(sizeof(linked_widget));
    tmp->widget = widget;
    tmp->next = (*widget_classes)->widget;
    (*widget_classes)->widget = tmp;
    //debug_widget_classes();
    return widget;
}


static void remove_widget_from_list(ei_widget_t* widget)
{
    linked_class** widget_classes = get_widget_classes();
    linked_widget* tmp;
    linked_widget* tmp2;
    linked_widget** tmp3;
    if(widget == NULL){
        fprintf(stderr,"Error: Widget is set to NULL.\n");
        return;
    }
    while(*widget_classes != NULL){
        if(strcmp((*widget_classes)->name,widget->wclass->name) == 0){
            tmp = (*widget_classes)->widget;
            if(tmp == NULL){
                fprintf(stderr,"This widget is not stored in the widget list.\n");
                return;
            }
            if(tmp->widget == widget){
                (*widget_classes)->widget = tmp->next;
                free(tmp);
                return;
            }
            tmp3 = &((*widget_classes)->widget);
            while(*tmp3 != NULL && (*tmp3)->next->widget != widget){
                tmp3 = &((*tmp3)->next);
            }
            if(tmp3 == NULL){
                fprintf(stderr,"This widget is not stored in the widget list.\n");
                return;
            }
            tmp2 = (*tmp3)->next->next;
            free((*tmp3)->next);
            (*tmp3)->next = tmp2;
            break;
        }
        widget_classes = &((*widget_classes)->next);
    }
}

static void destroy_sons(ei_widget_t* widget)
{
    ei_widget_t* tmp = widget;
    ei_widget_t* tmp2;
    while(tmp != NULL){
        if(tmp->children_head != NULL)
            destroy_sons(tmp->children_head);
        tmp2 = tmp->next_sibling;
        (*(tmp->wclass->releasefunc))(tmp);
        remove_widget_from_list(tmp);
        free(tmp);
        tmp = tmp2;
    }
}

static ei_widget_t* get_predecessor(ei_widget_t* widget){
    ei_widget_t* tmp = widget->parent->children_head;
    ei_widget_t* predecessor = NULL;
    while(tmp != NULL){
        if(tmp == widget)
            break;
        predecessor = tmp;
        tmp = tmp->next_sibling;
    }
    return predecessor;
}

void ei_widget_destroy(ei_widget_t* widget)
{
    /* Not sure if everything is freed */
    if(widget == NULL)
        return;
    destroy_sons(widget->children_head);
    remove_widget_from_list(widget);
    (*(widget->wclass->releasefunc))(widget);
    if(widget->parent == NULL){
        return ;
    }
    else if(widget == widget->parent->children_head){
        widget->parent->children_head = widget->next_sibling;
    }
    else {
        ei_widget_t* predecessor = get_predecessor(widget);
        predecessor->next_sibling = widget->next_sibling;
    }
    free(widget);
}

ei_widget_t* ei_widget_pick(ei_point_t* where)
{
    return NULL;
}

