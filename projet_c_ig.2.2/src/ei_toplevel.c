#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "ei_types.h"
#include "ei_widget.h"
#include "hw_interface.h"
#include "ei_toplevel.h"
#include "ei_widgetclass.h"
#include "ei_geometrymanager.h"
#include "ei_shape.h"

#define MAX(a,b)((a > b)? a : b)

/*  alloc a memory bloc, with the size of a toplevel widget */
void* alloc_toplevel()
{   
    return calloc(1, sizeof(ei_toplevel_widget_t));
}

/* free the memory allocated to the widget before its destruction (warning : doesn't free the ei_widget_t) */
void release_toplevel(ei_widget_t* widget)
{
    if(widget == NULL)
        return;
    free(widget->pick_color);
    free(widget->geom_params);
    ei_toplevel_widget_t* toplevel = (ei_toplevel_widget_t*) widget;
    free(toplevel->title);
}

/* draw a 'toplevel' */
void draw_toplevel(ei_widget_t* widget,ei_surface_t surface,ei_surface_t pick_surface,ei_rect_t* clipper)
{
	ei_toplevel_widget_t* toplevel = (ei_toplevel_widget_t*) widget;

	int text_height;
	hw_text_compute_size(toplevel->title, ei_default_font, NULL, &text_height);
	int header_height = 2*toplevel->border_width + text_height;
	int radius = header_height/2;
    
	ei_color_t color = {0x2f, 0x2f, 0x2f, 0x60};

	/* drawing of the background and the pick_surface of the window */
	draw_toplevel_background(&(widget->screen_location), color, radius, toplevel->closable, toplevel->title, toplevel->border_width, surface, clipper, widget->pick_color, pick_surface);
}


/* initialize a 'toplevel' */
void set_defaults_toplevel(ei_widget_t* widget)
{
    if(widget == NULL)
        return;
    widget->requested_size.width = 320;
    widget->requested_size.height = 240;
    ei_toplevel_widget_t* toplevel = (ei_toplevel_widget_t*) widget;
    toplevel->color = ei_default_background_color;
    toplevel->border_width = 4;
    toplevel->title = "Toplevel";
    toplevel->closable = EI_TRUE;
    toplevel->resizable = ei_axis_both;
    toplevel->min_size.width = 160;
    toplevel->min_size.height = 120;
}


/* update the screen_location of a central_frame */
static void geomnotify_toplevel_central_frame(ei_toplevel_widget_t* father)
{
	ei_rect_t* father_size = father->widget.content_rect;
	ei_rect_t dest;

	/* computation of the new screen_location */
	dest.top_left.x = father_size->top_left.x;
	dest.top_left.y = father_size->top_left.y;
	dest.size.width = father_size->size.width;
	dest.size.height = father_size->size.height;

	ei_widget_t* widget = father->widget.children_head;
	(*(widget->wclass->geomnotifyfunc))(widget, dest);
}


/* update the screen_location of a close_button */
static void geomnotify_toplevel_close_button(ei_toplevel_widget_t* father)
{
	ei_rect_t* father_size = &(father->widget.screen_location);
	ei_rect_t dest;

	/* computation of the new screen_location */
	dest.top_left.x = (father->border_width == 0) ? father_size->top_left.x + 2 : father_size->top_left.x + 2*father->border_width;
	dest.top_left.y = (father->border_width == 0) ? father_size->top_left.y + 2 : father_size->top_left.y + 2*father->border_width;

	int text_height;
	hw_text_compute_size(father->title, ei_default_font, NULL, &text_height);
	dest.size.width = (father->border_width == 0) ? text_height - 4 : text_height - 4*father->border_width;
	dest.size.height = dest.size.width;

	/* registering in the screen_location of the close-button */
	ei_widget_t* widget = father->widget.children_head->next_sibling;
	(*(widget->wclass->geomnotifyfunc))(widget, dest);
}


/* update the screen_location of a resize_button */
static void geomnotify_toplevel_resize_button(ei_toplevel_widget_t* father)
{
	ei_rect_t* father_size = &(father->widget.screen_location);
	ei_rect_t dest;
	
	/* computation of the new screen_location */
	dest.top_left.x = father_size->top_left.x + father_size->size.width - 8;
	dest.top_left.y = father_size->top_left.y + father_size->size.height - 8;
	dest.size.width = 8;
	dest.size.height = 8;

	/* registering in the screen_location of the resize_button */
	ei_widget_t* widget = father->widget.children_head->next_sibling;
	if (father->closable)
		widget = widget->next_sibling;
	(*(widget->wclass->geomnotifyfunc))(widget, dest);
}



/* update the screen_location of the toplevel */
void geomnotify_toplevel(ei_widget_t* widget,ei_rect_t rect)
{
	ei_toplevel_widget_t *toplevel = (ei_toplevel_widget_t*)widget;
	
	int title_height;
	hw_text_compute_size(toplevel->title, ei_default_font, NULL, &title_height);

	/* update of the screen_location */
	ei_rect_t *dest = &(widget->screen_location);
	dest->top_left.x = rect.top_left.x;
	dest->top_left.y = rect.top_left.y;

	dest->size.width = MAX(rect.size.width, toplevel->min_size.width) + 2*toplevel->border_width;
	dest->size.height = MAX(rect.size.height, toplevel->min_size.height) + 3*toplevel->border_width + title_height;

	/* update of the content_rect */
	if (widget->content_rect == &(widget->screen_location))
		widget->content_rect = malloc(sizeof(ei_rect_t));

	widget->content_rect->top_left.x = rect.top_left.x + toplevel->border_width;
	widget->content_rect->top_left.y = rect.top_left.y + title_height + 2*toplevel->border_width;
	
	widget->content_rect->size.width = dest->size.width - 2*toplevel->border_width;
	widget->content_rect->size.height = dest->size.height - 3*toplevel->border_width - title_height;

	if (widget->content_rect->size.width < 0 || widget->content_rect->size.height < 0) {
		widget->content_rect->size.width = 0;
		widget->content_rect->size.height = 0;
		fprintf(stderr, "Insufficient surface in the toplevel.\n");
	}

	/* update of the son's screen_locations */
	/* the central frame is the first
	 * the close-button, if it exists, is the second
	 * the resize-button, if it exists, is the third (the second if close-button doesn't exists */
	geomnotify_toplevel_central_frame(toplevel);
	if (toplevel->closable)
		geomnotify_toplevel_close_button(toplevel);
	if (toplevel->resizable != ei_axis_none)
		geomnotify_toplevel_resize_button(toplevel);

	/* update of the other sons's screen_locations */
	ei_widget_t* tmp = widget->children_head;
	while (tmp != NULL) {
		if (tmp->geom_params != NULL) {
			(*(tmp->geom_params->manager->runfunc))(tmp);
		}
		tmp = tmp->next_sibling;
	}
}
