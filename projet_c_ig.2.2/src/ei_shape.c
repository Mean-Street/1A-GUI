#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "ei_frame.h"
#include "ei_shape.h"
#include "ei_utils.h"

#define PI (3.141592653589793)

/**
 * add a point at the head's list
 */
static void add_head_linked_point(ei_linked_point_t** list, int x, int y)
{
    //printf("x : %d\ty : %d\n",x,y);
    ei_linked_point_t* tmp = malloc(sizeof(ei_linked_point_t));
    (tmp->point).x = x;
    (tmp->point).y = y;   
    tmp->next = *list;
    *list = tmp;
}



/**
 * recursively free the point's list
 */
static void free_linked_point(ei_linked_point_t* list)
{
    ei_linked_point_t* tmp = list;
    while(tmp != NULL){
        list = tmp->next;
        free(tmp);
        tmp = list;
    }
}



/**
 * increase the luminosity of a color
 * note : we can decrease it with a negative value
 */
static ei_color_t relative_value(ei_color_t color, char value)
{
	/* for each shade, the value can increase up to 255 and decrease down to 0 */
    ei_color_t c = color;
    if (c.red + value > 255) {
        c.red = 255;
    } else if (c.red + value < 0) {
        c.red = 0;
    } else {
        c.red += value;
    }

    if (c.green + value > 255) {
        c.green = 255;
    } else if (c.green + value < 0) {
        c.green = 0;
    } else {
        c.green += value;
    }
    if (c.blue + value > 255) {
        c.blue = 255;
    } else if (c.blue + value < 0) {
        c.blue = 0;
    } else {
        c.blue += value;
    }

    return c;
}



/**
 * add a point's list which describes an arc
 * Warning : the angles aren't in radian, but a double between 0 and 1 (in the trigonometric way)
 */
static void add_arc(ei_linked_point_t** list, ei_point_t center, int radius, double from_angle, double to_angle)
{
    if ( radius <= 0 ) {
        add_head_linked_point(list, center.x, center.y);
    } else {
        double x,y;
        int nb_point=radius;
        //step of the angle
        double theta = 2*PI*(to_angle-from_angle)/nb_point;
        //initial angle
        from_angle *= 2*PI;
        for(int t = 0; t<=nb_point; t++){
            x = radius *  cos(theta*t+from_angle)+center.x; 
            y = radius * -sin(theta*t+from_angle)+center.y; 
            add_head_linked_point(list,(int)round(x),(int)round(y));
        }   
    }   
}



/**
 * fill the surface with a color, on the rectangle
 */
void draw_frame_background(ei_rect_t* coordinate, ei_color_t color, ei_surface_t surface, ei_rect_t* clipper)
{
    int x_0 = coordinate->top_left.x;
    int x_1 = coordinate->top_left.x + coordinate->size.width;
    int y_0 = coordinate->top_left.y;
    int y_1 = coordinate->top_left.y + coordinate->size.height;
    
	ei_linked_point_t* frame = NULL;
    add_head_linked_point(&frame,x_0,y_0);
    add_head_linked_point(&frame,x_0,y_1);
    add_head_linked_point(&frame,x_1,y_1);
    add_head_linked_point(&frame,x_1,y_0);
    // drawing of the rectangle
	ei_draw_polygon(surface,frame,color,clipper);
	// free the allocated memory
    free_linked_point(frame);
}



/**
 * draw the window's relief
 */
void draw_frame_border(ei_rect_t* coordinate, int8_t relief, ei_color_t color, ei_surface_t surface, ei_rect_t* clipper)
{
	//Note : points are added in the trigonometric way
	// top border
    ei_linked_point_t* border = NULL;
    add_head_linked_point(&border, coordinate->top_left.x , coordinate->top_left.y );
    add_head_linked_point(&border, coordinate->top_left.x , coordinate->top_left.y + coordinate->size.height);
    add_head_linked_point(&border, coordinate->top_left.x + coordinate->size.height/2, coordinate->top_left.y + coordinate->size.height/2);
    add_head_linked_point(&border, coordinate->top_left.x + coordinate->size.width-coordinate->size.height/2, coordinate->top_left.y + coordinate->size.height/2);
    add_head_linked_point(&border, coordinate->top_left.x + coordinate->size.width, coordinate->top_left.y );
    
	ei_draw_polygon(surface,border,relative_value(color,50 * relief),clipper);
    free_linked_point(border);
    
	// bottom border
	border = NULL;
    add_head_linked_point(&border, coordinate->top_left.x + coordinate->size.width, coordinate->top_left.y + coordinate->size.height);
    add_head_linked_point(&border, coordinate->top_left.x + coordinate->size.width, coordinate->top_left.y );
    add_head_linked_point(&border, coordinate->top_left.x + coordinate->size.width-coordinate->size.height/2, coordinate->top_left.y + coordinate->size.height/2);
    add_head_linked_point(&border, coordinate->top_left.x + coordinate->size.width-coordinate->size.height/2, coordinate->top_left.y + coordinate->size.height/2);
    add_head_linked_point(&border, coordinate->top_left.x + coordinate->size.height/2, coordinate->top_left.y + coordinate->size.height/2);
    add_head_linked_point(&border, coordinate->top_left.x , coordinate->top_left.y + coordinate->size.height);
    
	ei_draw_polygon(surface,border,relative_value(color,-50 * relief),clipper);
    free_linked_point(border);
}



void draw_button_border(ei_rect_t* coordinate, int radius, int8_t relief, ei_color_t color, ei_surface_t surface, ei_rect_t* clipper)
{
	// top border
    int half_height = coordinate->size.height/2;
    ei_linked_point_t* border = NULL;
    ei_point_t p = {coordinate->top_left.x + radius,coordinate->top_left.y + radius};
    add_arc(&border,p,radius,1.0/4,1.0/2); 
    p.y = coordinate->top_left.y + coordinate->size.height-radius;
    add_arc(&border,p,radius,1.0/2,5.0/8); 
    add_head_linked_point(&border,coordinate->top_left.x + half_height,coordinate->top_left.y + half_height);
    add_head_linked_point(&border,coordinate->top_left.x + coordinate->size.width-half_height,coordinate->top_left.y + half_height);
    p.x = coordinate->top_left.x + coordinate->size.width-radius;
    p.y = coordinate->top_left.y + radius;
    add_arc(&border,p,radius,1.0/8,1.0/4); 
    ei_draw_polygon(surface,border,relative_value(color,50*relief),clipper);
    free_linked_point(border);

	// bottom border
    border = NULL;
    p.x = coordinate->top_left.x + radius;
    p.y = coordinate->top_left.y + coordinate->size.height-radius;
    add_arc(&border,p,radius,5.0/8,3.0/4); 
    p.x = coordinate->top_left.x + coordinate->size.width-radius;
    add_arc(&border,p,radius,3.0/4,1); 
    p.y = coordinate->top_left.y + radius;
    add_arc(&border,p,radius,0,1.0/8);
    add_head_linked_point(&border,coordinate->top_left.x + coordinate->size.width-half_height,coordinate->top_left.y + half_height);
    add_head_linked_point(&border,coordinate->top_left.x + half_height,coordinate->top_left.y + half_height);
    
	ei_draw_polygon(surface,border,relative_value(color,-50 * relief),clipper);
    free_linked_point(border);
}



void draw_button_background(ei_rect_t* coordinate,int radius,ei_color_t color,ei_surface_t surface,ei_rect_t* clipper)
{
    int x_0 = coordinate->top_left.x + radius ;
    int x_1 = coordinate->top_left.x + coordinate->size.width - radius;
    int y_0 = coordinate->top_left.y + radius;
    int y_1 = coordinate->top_left.y + coordinate->size.height - radius;
    ei_linked_point_t* frame = NULL;
    ei_point_t p = {x_0,y_0};
    add_arc(&frame,p,radius,1.0/4,1.0/2); 
    p.y = y_1;
    add_arc(&frame,p,radius,1.0/2,3.0/4); 
    p.x = x_1;
    add_arc(&frame,p,radius,3.0/4,1); 
    p.y = y_0;
    add_arc(&frame,p,radius,0,1.0/4); 
    ei_draw_polygon(surface,frame,color,clipper);
    free_linked_point(frame);
}



void draw_toplevel_background (	ei_rect_t* pos,
                                ei_color_t color,
								int radius, 
								ei_bool_t closable,
								char* title, 
								int border_width,
								ei_surface_t surface, 
								ei_rect_t* clipper,
								ei_color_t* pick_color,
								ei_surface_t pick_surface)
{
	/**********************************
	 * drawing of the grey background *
	 **********************************/
	ei_linked_point_t* list = NULL;
	
	// NorthWest arc
	ei_point_t center = ei_point(pos->top_left.x + radius, pos->top_left.y + radius);
	add_arc(&list, center, radius, 0.25, 0.5);
	// South side
	add_head_linked_point(&list, pos->top_left.x, pos->top_left.y + pos->size.height);
	add_head_linked_point(&list, pos->top_left.x + pos->size.width, pos->top_left.y + pos->size.height);
	// NorthEast arc
	center = ei_point(pos->top_left.x + pos->size.width - radius, pos->top_left.y + radius);
	add_arc(&list, center, radius, 0.0, 0.25);
	
	/* drawing of the window */
	ei_draw_polygon(surface, list, color, clipper);

	/* drawing of the picking surface of the window */
	ei_draw_polygon(pick_surface, list, *pick_color, clipper);

	free_linked_point(list);

	
	/*******************************
	 * drawing of the window title *
	 *******************************/

	int text_shift = 0;
	if (closable)
		text_shift = 2*radius;

	ei_point_t text_pos = ei_point(pos->top_left.x + text_shift + 2*border_width, pos->top_left.y + border_width);
	ei_color_t text_color = {0xff, 0xff, 0xff, 0xff};
	ei_draw_text(surface, &text_pos, title, ei_default_font, &text_color, NULL);
}

