#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ei_types.h"
#include "ei_widget.h"
#include "hw_interface.h"
#include "ei_geometrymanager.h"
#include "ei_placer.h"
#include "ei_application.h"
#include "ei_toplevel.h"

struct ei_geometrymanager_t* MANAGERS = NULL;


void ei_geometrymanager_register (ei_geometrymanager_t* geometrymanager)
{
	if (geometrymanager == NULL)
		return;
	else if (MANAGERS == NULL) {
		MANAGERS = malloc(sizeof(ei_geometrymanager_t));
		memcpy(MANAGERS, geometrymanager, sizeof(ei_geometrymanager_t)); 
	} else {
		ei_geometrymanager_t* tmp = MANAGERS;
		while (tmp != NULL) {
			if (strcmp((char*)tmp->name, (char*)geometrymanager->name) == 0)
				return;
			tmp = tmp->next;
		}
		tmp = malloc(sizeof(ei_geometrymanager_t));
		memcpy(tmp, geometrymanager, sizeof(ei_geometrymanager_t));
		tmp->next = MANAGERS;
		MANAGERS = tmp;
	}
}


ei_geometrymanager_t* ei_geometrymanager_from_name (ei_geometrymanager_name_t name)
{
	ei_geometrymanager_t* tmp = MANAGERS;
	while (tmp != NULL) {
		if (strcmp((char*)tmp->name, (char*)name) == 0)
			return tmp;
		tmp = tmp->next;
	}
	fprintf(stderr, "%s is not a valid class.\n", (char*)name);
	return NULL;
}


void ei_geometrymanager_unmap (ei_widget_t* widget)
{
	/* si il n'y a pas de gestionnaire de géométrie, on arrête */
	if (widget->geom_params == NULL)
		return;

	/* appel de la release_func, qui libère les arguments spécifiques au placeur */
	(*(widget->geom_params->manager->releasefunc))(widget);

	/* invalidation de la surface du widget */
	/* ei_app_invalidate_rect(&(widget->screen_location)); // DOESN'T WORK */

	/* remise à zéro du screen_location */
	widget->screen_location.top_left.x = 0;
	widget->screen_location.top_left.y = 0;
	widget->screen_location.size.width = 0;
	widget->screen_location.size.height = 0;
}


void ei_place(ei_widget_t* widget,
		ei_anchor_t* anchor,
		int* x,
		int* y,
		int* width,
		int* height,
		float* rel_x,
		float* rel_y,
		float* rel_width,
		float* rel_height)
{
	/* idée : nouvelle structure contenant les paramètres ci-dessus, remplie par ei_place (valeurs par défaut surtout)
	 * ei_place appelle ensuite la runfunc du placeur, qui à partir de ces paramètres et de requested_size, calcule les champs de screen_location
	 * => on va faire des sous-types de geometry_params, et étendre avec des champs supplémentaires fonctions du geometry_manager (comme pour les widgets)
	 * (on fait du polymorphisme de polymorphismes en fait)
	 */

	if (widget == NULL)
		return;

	/* if the current geometry manager is undefined or not 'placer', then the geom_param field is updated */
	if (widget->geom_params == NULL) {
		widget->geom_params = malloc(sizeof(ei_geometry_placer_t));
		widget->geom_params->manager = ei_geometrymanager_from_name("placer");
	} else if (strcmp(widget->geom_params->manager->name, "placer") != 0) {
		ei_geometrymanager_unmap(widget);
		widget->geom_params = malloc(sizeof(ei_geometry_placer_t));
		widget->geom_params->manager = ei_geometrymanager_from_name("placer");
	}

	/* cast of the structure */
	ei_geometry_placer_t* params = (ei_geometry_placer_t*) widget->geom_params;

	/* filling all parameters (with given or default values) */
	params->anchor = (anchor == NULL)? ei_anc_northwest : *anchor;
	params->x = (x == NULL)? 0 : *x;
	params->y = (y == NULL)? 0 : *y;
	params->width = (width == NULL)? (widget->requested_size).width : *width;
	params->height = (height == NULL)? (widget->requested_size).height : *height;
	params->rel_x = (rel_x == NULL)? 0 : *rel_x;
	params->rel_y = (rel_y == NULL)? 0 : *rel_y;
	params->rel_width = (rel_width == NULL)? 0 : *rel_width;
	params->rel_height = (rel_height == NULL)? 0 : *rel_height;

	/* call of the placer's runfunc */
	(*(widget->geom_params->manager->runfunc))(widget);
}



void ei_register_placer_manager ()
{
	ei_geometrymanager_t manager;
	strcpy(manager.name, "placer");
	manager.runfunc = &run_placer;
	manager.releasefunc = &release_placer;
	manager.next = NULL;
	ei_geometrymanager_register(&manager);
}

