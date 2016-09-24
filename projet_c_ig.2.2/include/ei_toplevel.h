#ifndef EI_TOPLEVEL_H
#define EI_TOPLEVEL_H

#include "ei_types.h"
#include "ei_widget.h"

typedef struct {
    ei_widget_t widget;
    ei_color_t color;
    int border_width;
    ei_relief_t relief;
    char* title;
    ei_bool_t closable;
    ei_axis_set_t resizable;
    ei_size_t min_size;
} ei_toplevel_widget_t;

void* alloc_toplevel();

void release_toplevel(ei_widget_t* widget);

void draw_toplevel(ei_widget_t* widget,ei_surface_t surface,ei_surface_t pick_surface,ei_rect_t* clipper);

void set_defaults_toplevel(ei_widget_t* widget);

void geomnotify_toplevel(ei_widget_t* widget,ei_rect_t rect);

#endif
