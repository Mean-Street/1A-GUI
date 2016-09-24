#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hw_interface.h"
#include "ei_draw.h"
#include "ei_frame.h"
#include "ei_button.h"
#include "ei_toplevel.h"
#include "ei_widget.h"
#include "ei_widgetclass.h"
#include "ei_global.h"

void ei_widgetclass_register(ei_widgetclass_t* widgetclass)
{
    widgetclass->next = NULL;

    ei_widgetclass_t** classes = get_classes();
    if(widgetclass == NULL){
        return;
    }
    else if(*classes == NULL){
        *classes = malloc(sizeof(ei_widgetclass_t));
        memcpy(*classes,widgetclass,sizeof(ei_widgetclass_t));
    }
    else {
        ei_widgetclass_t** tmp = classes;
        while(*tmp != NULL){
            if(strcmp((*tmp)->name,widgetclass->name) == 0)
                return ;
            tmp = &((*tmp)->next);
        }
        ei_widgetclass_t* tmp2 = malloc(sizeof(ei_widgetclass_t));
        memcpy(tmp2,widgetclass,sizeof(ei_widgetclass_t));
        tmp2->next = *classes;
        *classes = tmp2;
    }
}

void ei_frame_register_class()
{
    ei_widgetclass_t wclass;
    strcpy(wclass.name,"frame");
    wclass.allocfunc = &alloc_frame;
    wclass.releasefunc = &release_frame;
    wclass.drawfunc = &draw_frame;
    wclass.setdefaultsfunc = &set_defaults_frame;
    wclass.geomnotifyfunc = &geomnotify_frame;
    ei_widgetclass_register(&wclass);
}

void ei_button_register_class()
{
    ei_widgetclass_t wclass;
    strcpy(wclass.name,"button");
    wclass.allocfunc = &alloc_button;
    wclass.releasefunc = &release_button;
    wclass.drawfunc = &draw_button;
    wclass.setdefaultsfunc = &set_defaults_button;
    wclass.geomnotifyfunc = &geomnotify_button;
    ei_widgetclass_register(&wclass);
}

void ei_toplevel_register_class()
{
    ei_widgetclass_t wclass;
    strcpy(wclass.name,"toplevel");
    wclass.allocfunc = &alloc_toplevel;
    wclass.releasefunc = &release_toplevel;
    wclass.drawfunc = &draw_toplevel;
    wclass.setdefaultsfunc = &set_defaults_toplevel;
    wclass.geomnotifyfunc = &geomnotify_toplevel;
    ei_widgetclass_register(&wclass);
}


/* WRITE HERE YOUR REGISTER_CLASS FUNCTION */

ei_widgetclass_t* ei_widgetclass_from_name(ei_widgetclass_name_t name)
{
    ei_widgetclass_t** tmp = get_classes();
    while(*tmp != NULL){
        if(strcmp((*tmp)->name,name)==0)
            return *tmp;
        tmp=&((*tmp)->next);
    }
    fprintf(stderr,"%s is not a valid class.\n",(char*) name);
    return NULL;
}
