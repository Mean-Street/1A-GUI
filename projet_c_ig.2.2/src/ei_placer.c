#include <stdlib.h>
#include <stdio.h>
#include "ei_placer.h"

void run_placer (ei_widget_t* widget)
{

	if (widget == NULL)
		return;

	/**************************************************************************
	 * Update of the screen_location's parameters, using the geom_params ones *
	 **************************************************************************/

	/* geom_params of the widget, they belong to the placer */
	ei_geometry_placer_t *src = (ei_geometry_placer_t*)widget->geom_params;

	/* screen_location we'll compute */
	ei_rect_t dest;

	/* size and position of the authorized zone */
	ei_rect_t *father_size = widget->parent->content_rect;

	/* corrections are what we need to remove to the computed position
	 * in order to take of the anchor */
	int x_correction = 0;
	int y_correction = 0;
	ei_anchor_t anchor = src->anchor;

	/* update of the screen_location's values */
	dest.top_left.x = src->x + ((father_size->size).width * src->rel_x);
	dest.top_left.y = src->y + ((father_size->size).height * src->rel_y);

	dest.size.width = src->width + ((father_size->size).width * src->rel_width);
	dest.size.height = src->height + ((father_size->size).height * src->rel_height);
	
	
	if (anchor == ei_anc_north) {
		x_correction = dest.size.width/2;
	} else if (anchor == ei_anc_northeast) {
		x_correction = dest.size.width;
	} else if (anchor == ei_anc_west) {
		y_correction = dest.size.height/2;
	} else if (anchor == ei_anc_center) {
		x_correction = dest.size.width/2;
		y_correction = dest.size.height/2;
	} else if (anchor == ei_anc_east) {
		x_correction = dest.size.width;
		y_correction = dest.size.height/2;
	} else if (anchor == ei_anc_southwest) {
		y_correction = dest.size.height;
	} else if (anchor == ei_anc_south) {
		x_correction = dest.size.width/2;
		y_correction = dest.size.height;
	} else if (anchor == ei_anc_southeast) {
		x_correction = dest.size.width;
		y_correction = dest.size.height;
	}

	dest.top_left.x -= x_correction;
	dest.top_left.y -= y_correction;


	/*******************************************
	 * dealing with the overflows of the frame *
	 *******************************************/

	// if left overflow
	if (dest.top_left.x < 0) {
		fprintf(stderr, "The widget oversteps West bound.\n");
		dest.size.width += dest.top_left.x;
		dest.top_left.x = 0;
	} 
	// if right overflow
	if (dest.top_left.x + dest.size.width > (father_size->size).width) {
		fprintf(stderr, "The widget oversteps East bound.\n");
		dest.size.width -= (dest.top_left.x + dest.size.width - (father_size->size).width);
	}
	// if top overflow
	if (dest.top_left.y < 0) {
		fprintf(stderr, "The widget oversteps North bound.\n");
		dest.size.height += dest.top_left.y;
		dest.top_left.y = 0;
	}
	// if bottom overflow
	if (dest.top_left.y + dest.size.height > (father_size->size).height) {
		fprintf(stderr, "The widget oversteps South bound.\n");
		dest.size.height -= (dest.top_left.y + dest.size.height - (father_size->size).height);
	 }

	// if the widget is entirely out of bounds
	if (dest.size.width < 0 || dest.size.height < 0) {
		dest.size.width = 0;
		dest.size.height = 0;
		fprintf(stderr, "Error : the widget is entirely out of bound.\n");
	}

	// absolute coordinates
	dest.top_left.x += (father_size->top_left).x;
	dest.top_left.y += (father_size->top_left).y;

	// registering of the parameters in the screen_location
	(*(widget->wclass->geomnotifyfunc))(widget, dest);


	/*******************************************************************************
	 * Call of the sons's runfuncs, in order to compute the son's screen_locations *
	 *******************************************************************************/

	ei_widget_t *tmp = widget->children_head;
	while (tmp != NULL) {
		if (tmp->geom_params != NULL) {
			(*(tmp->geom_params->manager->runfunc))(tmp);
		}
		tmp = tmp->next_sibling;
	}

}



void release_placer (ei_widget_t* widget)
{
	ei_geometry_placer_t *tmp = (ei_geometry_placer_t*)widget->geom_params;
	free(tmp);
	widget->geom_params = NULL;
}

