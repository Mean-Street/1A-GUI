#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "ei_types.h"
#include "ei_widget.h"
#include "hw_interface.h"
#include "ei_button.h"
#include "ei_widgetclass.h"
#include "ei_shape.h"
#include "ei_utils.h"
#include "ei_geometrymanager.h"

/*  alloue un bloc mémoire de la taille d'un widget 'button' */
void* alloc_button()
{   
    return calloc(1, sizeof(ei_button_widget_t));
}

/* libère la mémoire allouée au widget (ses ressources) avant sa destruction, mais sans libérer le 'ei_widget_t' */
void release_button(ei_widget_t* widget)
{
    free(widget->pick_color);
    free(widget->geom_params);
    ei_button_widget_t* button = (ei_button_widget_t*) widget;
    free(button->img_rect);
    free(button->text);
}

/* dessine un 'button' */
void draw_button(ei_widget_t* widget,ei_surface_t surface,ei_surface_t pick_surface,ei_rect_t* clipper)
{
    int x_i = widget->screen_location.top_left.x;
    int y_i = widget->screen_location.top_left.y;
    int width = widget->screen_location.size.width;
    int height = widget->screen_location.size.height;
    ei_button_widget_t* button = (ei_button_widget_t*) widget;
    int thick = button->border_width;
    int radius = button->corner_radius;
    ei_color_t color = button->color;
    int8_t relief;
    if (button->relief == ei_relief_raised) {
        relief = 1; 
    } else if (button->relief == ei_relief_none) {
        relief = 0; 
    } else if (button->relief == ei_relief_sunken) {
        relief = -1; 
    } else {
        fprintf(stderr,"button have no relief spécified");
    }
    ei_rect_t background_coordinate = {{x_i + thick,y_i + thick},{width-2*thick, height-2*thick}};
    draw_button_border(&(widget->screen_location), radius, relief, color, surface, clipper);
    //Dessin de la bordure
    //dessine le fond du boutton (bord arondi)
    if (radius > thick) {
        draw_button_background(&background_coordinate, (radius-thick), color, surface, clipper);
    } else {
        draw_button_background(&background_coordinate, 0, color, surface, clipper);
    }
    //Créé l'affichage du text
    ei_size_t text_size;
    if (button->text != NULL && button->text_font != NULL) {
        hw_text_compute_size (button->text,button->text_font, &(text_size.width), &(text_size.height));
        ei_point_t text_pos = {x_i + thick+(width/2)-(text_size.width/2), y_i + thick/2+(height/2)-(text_size.height/2) };
        ei_draw_text(surface, &text_pos, button->text, button->text_font, &(button->text_color), NULL);
    }
    //Affichage de l'image
    if ( button->img != NULL) {
        hw_surface_lock(button->img);
        ei_rect_t src_rect;
        ei_rect_t dest_rect;
        if (button->img_rect != NULL) {
            src_rect = *(button->img_rect);
            if (src_rect.size.width > widget->screen_location.size.width) 
                src_rect.size.width = widget->screen_location.size.width;
            if (src_rect.size.height > widget->screen_location.size.height)
                src_rect.size.height = widget->screen_location.size.height;
            dest_rect = src_rect;
            dest_rect.top_left = widget->screen_location.top_left;
            dest_rect.top_left.x += (widget->screen_location.size.width - dest_rect.size.width)/2 ;
            dest_rect.top_left.y += (widget->screen_location.size.height - dest_rect.size.height)/2 ;
        } else {
            src_rect = hw_surface_get_rect(button->img);
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
        ei_copy_surface(surface, &dest_rect, button->img, &src_rect, EI_TRUE);
        hw_surface_unlock(button->img);
    }
    //met à joure la surface de picking
    draw_button_border(&(widget->screen_location), radius,0 , *(widget->pick_color), pick_surface, clipper);
}

/* initialise un 'button' */
void set_defaults_button(ei_widget_t* widget)
{
    if(widget == NULL)
        return;
    widget->requested_size.width = 0;
    widget->requested_size.height = 0;
    ei_button_widget_t* button = (ei_button_widget_t*) widget;
    button->color = ei_default_background_color;
    button->border_width = k_default_button_border_width;
    button->corner_radius = k_default_button_border_width;
    button->relief = ei_relief_raised;
    button->text = NULL;
    button->text_font = ei_default_font;
    button->text_color = ei_font_default_color;
    button->text_anchor = ei_anc_center;
    button->img = NULL;
    button->img_rect = NULL;
    button->img_anchor = ei_anc_center;
    button->callback = NULL;
    button->user_param = NULL;
}

/* indique au button que le gestionnaire de géométrie a modifié sa forme */
void geomnotify_button(ei_widget_t* widget,ei_rect_t rect)
{
    ei_button_widget_t *button = (ei_button_widget_t*)widget;

    // mise à jour du screen_location
    ei_rect_t *dest = &(widget->screen_location);
    dest->top_left.x = rect.top_left.x;
    dest->top_left.y = rect.top_left.y;
    dest->size.width = rect.size.width;
    dest->size.height = rect.size.height;

    // mise à jour du content_rect
    if (widget->content_rect == &(widget->screen_location))
        widget->content_rect = malloc(sizeof(ei_rect_t));

	/* 0.3 est environ 1 - sqrt(2)/2 */
    widget->content_rect->top_left.x = dest->top_left.x + 0.3*button->corner_radius + button->border_width;
    widget->content_rect->top_left.y = dest->top_left.y + 0.3*button->corner_radius + button->border_width;
    widget->content_rect->size.width = dest->size.width - 0.6*button->corner_radius - 2*button->border_width;
    widget->content_rect->size.height = dest->size.height - 0.6*button->corner_radius - 2*button->border_width;

    if (widget->content_rect->size.width < 0 || widget->content_rect->size.height < 0) {
        widget->content_rect->size.width = 0;
        widget->content_rect->size.height = 0;
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
