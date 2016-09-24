#ifndef EI_PLACER_H
#define EI_PLACER_H

#include "ei_geometrymanager.h"

typedef struct ei_geometry_placer_t {
		ei_geometrymanager_t* manager;
		ei_anchor_t anchor;
		int x;
		int y;
		int width;
		int height;
		float rel_x;
		float rel_y;
		float rel_width;
		float rel_height;
} ei_geometry_placer_t;


void run_placer (ei_widget_t* widget);

void release_placer (ei_widget_t* widget);

#endif
