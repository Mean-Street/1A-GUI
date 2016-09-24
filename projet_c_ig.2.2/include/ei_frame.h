#ifndef EI_FRAME_H
#define EI_FRAME_H

#include "ei_types.h"
#include "ei_widget.h"

/**
 * widget extension for frame
 */
typedef struct {
    ei_widget_t widget;
    ei_color_t color;
    int border_width;
    ei_relief_t relief;
    char* text;
    ei_font_t text_font;
    ei_color_t text_color;
    ei_anchor_t text_anchor;
    ei_surface_t img;
    ei_rect_t* img_rect;
    ei_anchor_t img_anchor;
} ei_frame_widget_t;

/**
 * allocate memory to frame
 */
void* alloc_frame();

/**
 * free memory from frame
 */
void release_frame(ei_widget_t* widget);

/**
 * Draw the frame background, the text
 * and the relief
 */
void draw_frame(ei_widget_t* widget,ei_surface_t surface,ei_surface_t pick_surface,ei_rect_t* clipper);

/**
 * affect default values to frame
 */
void set_defaults_frame(ei_widget_t* widget);

/**
 * update the screen_location of the frame
 */
void geomnotify_frame(ei_widget_t* widget,ei_rect_t rect);

#endif
