#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "ei_types.h"
#include "ei_widget.h"
#include "hw_interface.h"
#include "ei_frame.h"
#include "ei_widgetclass.h"
#include "ei_draw.h"
#include "ei_shape.h"
#include "ei_utils.h"
#include "ei_geometrymanager.h"

/*  alloue un bloc mémoire de la taille d'un widget 'frame' */
void* alloc_frame()
{   
    return calloc(1, sizeof(ei_frame_widget_t));
}

/* libère la mémoire allouée au widget (ses ressources) avant sa destruction, mais sans libérer le 'ei_widget_t' */
void release_frame(ei_widget_t* widget)
{
    if(widget == NULL)
        return;
    free(widget->pick_color);
    free(widget->geom_params);
    ei_frame_widget_t* frame = (ei_frame_widget_t*) widget;
    free(frame->img_rect);
    free(frame->text);
}

/* dessine une 'frame' */
void draw_frame(ei_widget_t* widget,ei_surface_t surface,ei_surface_t pick_surface,ei_rect_t* clipper)
{
    int x_i = widget->screen_location.top_left.x;
    int y_i = widget->screen_location.top_left.y;
    int width   = widget->screen_location.size.width;
    int height   = widget->screen_location.size.height;
    ei_frame_widget_t* frame = (ei_frame_widget_t*) widget;
    int thick = frame->border_width;
    ei_color_t color = frame->color;
    ei_rect_t background_coordinate = {{x_i + thick,y_i + thick},{width-2*thick, height-2*thick}};

    int8_t relief;
    if (frame->relief == ei_relief_raised) {
        relief = 1; 
    } else if (frame->relief == ei_relief_none) {
        relief = 0; 
    } else if (frame->relief == ei_relief_sunken) {
        relief = -1; 
    } else {
        fprintf(stderr,"frame have no relief spécified");
    }
    //Dessin de la bordure
    draw_frame_border(&(widget->screen_location), relief, color, surface, clipper);
    //ei_fill(surface,&color,clipper);
    draw_frame_background(&background_coordinate, color, surface, clipper);
    //draw_frame_bord(background_coordinate, *(widget->pick_color), pick_surface, clipper);
    
    //Créer l'affichage du text
    ei_size_t text_size;
    if (frame->text != NULL && frame->text_font != NULL) {
        hw_text_compute_size (frame->text,frame->text_font, &(text_size.width), &(text_size.height));
        ei_point_t text_pos = {x_i + thick+(width/2)-(text_size.width/2), y_i + thick/2+(height/2)-(text_size.height/2) };
        ei_draw_text(surface, &text_pos, frame->text, frame->text_font, &(frame->text_color), NULL); 
    }
    //Affichage de l'image
    if ( frame->img != NULL) {
        hw_surface_lock(frame->img);
        ei_rect_t src_rect;
        ei_rect_t dest_rect;
        if (frame->img_rect != NULL) {

            src_rect = *(frame->img_rect);
            if (src_rect.size.width > widget->screen_location.size.width) 
                src_rect.size.width = widget->screen_location.size.width;
            if (src_rect.size.height > widget->screen_location.size.height)
                src_rect.size.height = widget->screen_location.size.height;
            dest_rect = src_rect;
            dest_rect.top_left = widget->screen_location.top_left; //ei_point_add(widget->screen_location.top_left, ei_point(thick, thick));
            dest_rect.top_left.x += (widget->screen_location.size.width - dest_rect.size.width)/2 ;
            dest_rect.top_left.y += (widget->screen_location.size.height - dest_rect.size.height)/2 ;
        } else {
            src_rect = hw_surface_get_rect(frame->img);
            dest_rect = ei_rect( widget->screen_location.top_left, src_rect.size);
        }
        if (widget->content_rect != NULL ) {
            dest_rect = *(widget->content_rect);
            if (( dest_rect.size.width < src_rect.size.width) && ( dest_rect.size.height < src_rect.size.height )) {
                src_rect = ei_rect( ei_point_zero(), dest_rect.size);
            } else {
                dest_rect = ei_rect( widget->screen_location.top_left , src_rect.size);
                dest_rect.top_left.x += (widget->screen_location.size.width - dest_rect.size.width)/2 ;
                dest_rect.top_left.y += (widget->screen_location.size.height - dest_rect.size.height)/2 ;
            }
        }
        //printf("dest : {pos=(%d,%d) , size=(%d,%d)}\n",dest_rect.top_left.x,dest_rect.top_left.y,dest_rect.size.width,dest_rect.size.height);
        //printf("src  : {pos=(%d,%d) , size=(%d,%d)}\n",src_rect.top_left.x,src_rect.top_left.y,src_rect.size.width,src_rect.size.height);
        ei_copy_surface(surface, &dest_rect, frame->img, &src_rect, EI_TRUE);
        hw_surface_unlock(frame->img);
    }
    draw_frame_border(&(widget->screen_location),0,*(widget->pick_color),pick_surface,clipper);
}

/* initialise une 'frame' */
void set_defaults_frame(ei_widget_t* widget)
{
    if (widget == NULL)
        return;
    widget->requested_size.width = 0;
    widget->requested_size.height = 0;
    ei_frame_widget_t* frame = (ei_frame_widget_t*) widget;
    frame->color = ei_default_background_color;
    frame->border_width = 0;
    frame->relief = ei_relief_none;
    frame->text = NULL;
    frame->text_font = ei_default_font;
    frame->text_color = ei_font_default_color;
    frame->text_anchor = ei_anc_center;
    frame->img = NULL;
    frame->img_rect = NULL;
    frame->img_anchor = ei_anc_center;
}

/* indique à la frame que le gestionnaire de géométrie a modifié sa forme */
void geomnotify_frame(ei_widget_t* widget,ei_rect_t rect)
{
	ei_frame_widget_t *frame = (ei_frame_widget_t*)widget;
	
	// mise à jour du screen_location
   	ei_rect_t *dest = &(widget->screen_location);
	dest->top_left.x = rect.top_left.x;
	dest->top_left.y = rect.top_left.y;
	dest->size.width = rect.size.width;
	dest->size.height = rect.size.height;

	// mise à jour du content_rect
	if (widget->content_rect == &(widget->screen_location))
		widget->content_rect = malloc(sizeof(ei_rect_t));

	widget->content_rect->top_left.x = dest->top_left.x + frame->border_width;
	widget->content_rect->top_left.y = dest->top_left.y + frame->border_width;
	widget->content_rect->size.width = dest->size.width - 2*frame->border_width;
	widget->content_rect->size.height = dest->size.height - 2*frame->border_width;

	if (widget->content_rect->size.width < 0 || widget->content_rect->size.height < 0) {
		widget->content_rect->size.width = 0;
		widget->content_rect->size.height = 0;
		fprintf(stderr, "Insufficient surface in the frame.\n");
	}

	/* update of the other sons's screen_locations */
	ei_widget_t* tmp = widget->children_head;
	while (tmp != NULL) {
		if (tmp->geom_params != NULL) {
			(*(tmp->geom_params->manager->runfunc))(tmp);
		}
		tmp = tmp->next_sibling;
	}
}
