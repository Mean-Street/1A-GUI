/**
 * @file ei_shape.h
 * @brief Graphical primitives to draw 
 * shape from dimension.
 * So do not use widget !
 *
 * \author 
 * Created by Loiodice Thomas 
 *
 */

#ifndef EI_SHAPE_H
#define EI_SHAPE_H


#include <stdio.h>
#include <stdlib.h>

/************************
 *        Frames        *
 ************************/

/**
 * draw the border relief of frame 
 * relief = {-1,0,1} 
 * 1 : raised relief 
 * -1: sunken relief
 * 0 : without relief
 */
void draw_frame_background(	ei_rect_t* coordinate,
							ei_color_t color, 
							ei_surface_t surface, 
							ei_rect_t* clipper);

/**
* Draw the border relief of frame.
 */
void draw_frame_border( ei_rect_t* coordinate,
                        int8_t relief, 
                        ei_color_t color,
                        ei_surface_t surface,
                        ei_rect_t* clipper);



/************************
 *       Buttons        *
 ************************/

/**
 * draw the border relief of curved button
 * relief = {-1,0,1} 
 * 1 : raised relief 
 * -1: sunken relief
 * 0 : without relief
 */
void draw_button_border( ei_rect_t* coordinate, 
                        int radius, 
                        int8_t relief, 
                        ei_color_t color, 
                        ei_surface_t surface, 
                        ei_rect_t* clipper);

/**
 * draw the curved button backgroud  
 */
void draw_button_background(	ei_rect_t* coordinate,
							int radius,
							ei_color_t color,
							ei_surface_t surface,
							ei_rect_t* clipper);



/************************
 *      Toplevels       *
 ************************/

/**
 * Draw the toplevel background and the title
 */
void draw_toplevel_background (	ei_rect_t* pos,
                                ei_color_t color,
								int radius, 
								ei_bool_t closable,
								char* title,
								int border_width,
								ei_surface_t surface, 
								ei_rect_t* clipper,
								ei_color_t* pick_color,
								ei_surface_t pick_surface);

#endif
