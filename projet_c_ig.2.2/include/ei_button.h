/**
 * @file ei_button.h
 * @brief Deal whith button widget 
 */

#ifndef EI_BUTTON_H
#define EI_BUTTON_H

#include "ei_types.h"
#include "ei_widget.h"

/**
 * widget extension for button
 */
typedef struct {
    ei_widget_t widget;
    ei_color_t color;
    int border_width;
    int corner_radius;
    ei_relief_t relief;
    char* text;
    ei_font_t text_font;
    ei_color_t text_color;
    ei_anchor_t text_anchor;
    ei_surface_t img;
    ei_rect_t* img_rect;
    ei_anchor_t img_anchor;
    ei_callback_t* callback;
    void* user_param;
} ei_button_widget_t;

/**
 * allocate memory to button
 */
void* alloc_button();

/**
 * free memory from button
 */
void release_button(ei_widget_t* widget);

/**
 * Draw the button frame,  the text
 * and the relief
 */
void draw_button(ei_widget_t* widget,ei_surface_t surface,ei_surface_t pick_surface,ei_rect_t* clipper);

/**
 * affect default values to button
 */
void set_defaults_button(ei_widget_t* widget);

/**
 * update the screen_location of the button
 */
void geomnotify_button(ei_widget_t* widget,ei_rect_t rect);

#endif
