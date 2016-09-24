#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ei_event.h"
#include "ei_callback.h"
#include "ei_global.h"

static void add_event(global_callback** link_callback,ei_callback_t callback,void* user_param)
{
    while(*link_callback != NULL){
        if((*link_callback)->callback == callback){
            linked_user_param** link_user_param = &((*link_callback)->user_param);
            while(*link_user_param != NULL){
                if((*link_user_param)->user_param == user_param){
                   return;
                }
                link_user_param = &((*link_user_param)->next);
            }
            linked_user_param* tmp = malloc(sizeof(linked_user_param));
            tmp->user_param = user_param;
            tmp->next = (*link_callback)->user_param;
            (*link_callback)->user_param = tmp;
            return;
        }
        link_callback = &((*link_callback)->next);
    }
    global_callback* tmp = malloc(sizeof(global_callback));
    tmp->callback = callback;
    tmp->next = *link_callback;
    *link_callback = tmp;
    (*link_callback)->user_param = malloc(sizeof(linked_user_param));
    (*link_callback)->user_param->user_param = user_param;
    (*link_callback)->user_param->next = NULL;
    return;
}

static void remove_event(global_callback** link_callback,ei_callback_t callback,void* user_param)
{
    global_callback** root = link_callback;
    linked_user_param** tmp;
    global_callback** father = link_callback;
    while(*link_callback != NULL){
        if((*link_callback)->callback == callback){
            linked_user_param** link_user_param = &((*link_callback)->user_param);
            if((*link_user_param)->next == NULL){
                (*link_callback)->user_param = NULL;
                free(*link_user_param);
            }
            tmp = link_user_param;
            while(*link_user_param != NULL){
                if((*link_user_param)->user_param == user_param){
                    (*tmp)->next = (*link_user_param)->next;
                    free(link_user_param);
                }
                tmp = link_user_param;
                link_user_param = &((*link_user_param)->next);
            }
            if((*link_callback)->user_param == NULL){
                if((*root)->next == NULL){
                    free(*root);
                    *root = NULL;
                }
                else {
                    (*father)->next = (*link_callback)->next;
                    free(*link_callback);
                }
                return;
            }
        }
        father = link_callback;
        link_callback = &((*link_callback)->next);
    }
}


void ei_bind(ei_eventtype_t eventtype,ei_widget_t* widget,ei_tag_t tag,ei_callback_t callback,void* user_param)
{
    linked_eventtype** types;
    /* bind event to a widget */
    if (widget != NULL){
        linked_event** widget_events = get_class_widget_events(widget->wclass->name);
        if(*widget_events == NULL){
            fprintf(stderr,"This widget seems to have no class.\n");
            return;
        }
        linked_widget_type** widget_types = &((*widget_events)->types);
        linked_widget_events* widget_event;
        while(*widget_types != NULL){
            if((*widget_types)->eventtype == eventtype){
                widget_event = (*widget_types)->widget_event;
                while(widget_event != NULL){
                    if(widget_event->widget == widget){
                        add_event(&(widget_event->callback),callback,user_param);
                        return;
                    }
                    widget_event = widget_event->next;
                }
                linked_widget_events* tmp = malloc(sizeof(linked_widget_events));
                tmp->widget = widget;
                tmp->callback = NULL;
                tmp->next = (*widget_types)->widget_event;
                (*widget_types)->widget_event = tmp;
                add_event(&((*widget_types)->widget_event->callback),callback,user_param);
                return;
            }
            widget_types = &((*widget_types)->next);
        }
        linked_widget_type* tmp = malloc(sizeof(linked_widget_type));
        tmp->eventtype = eventtype;
        tmp->widget_event = malloc(sizeof(linked_widget_events));
        tmp->widget_event->widget = widget;
        tmp->widget_event->callback = NULL;
        tmp->widget_event->next = NULL;
        tmp->next = (*widget_events)->types;
        (*widget_events)->types = tmp;
        add_event(&((*widget_events)->types->widget_event->callback),callback,user_param);
        return;
    }


    /* bind event to all */
    else if (strcmp(tag,"all") == 0) {
        types = get_events();
        while(*types != NULL){
            if((*types)->eventtype == eventtype){
                add_event(&((*types)->callback),callback,user_param);
                return;
            }
            types = &((*types)->next);
        }
        linked_eventtype* tmp = malloc(sizeof(linked_eventtype));
        tmp->eventtype = eventtype;
        tmp->callback = NULL;
        tmp->next = *types;
        *types = tmp;
        add_event(&((*types)->callback),callback,user_param);
    }


    /* Bind event to a class */
    else {
        linked_class** class_events = get_class_events(tag);
        if(*class_events == NULL){
            fprintf(stderr,"This tag is not valid.\n");
            return;
        }
        types = &((*class_events)->types);
        while(*types != NULL){
            if((*types)->eventtype == eventtype){
                add_event(&((*types)->callback),callback,user_param);
                return;
            }
            types = &((*types)->next);
        }
        linked_eventtype* tmp = malloc(sizeof(linked_eventtype));
        tmp->eventtype = eventtype;
        tmp->callback = NULL;
        tmp->next = NULL;
        *types = tmp;
        add_event(&((*types)->callback),callback,user_param);
    }
}

void ei_unbind(ei_eventtype_t eventtype,ei_widget_t* widget,ei_tag_t tag,ei_callback_t callback,void* user_param)
{
    linked_eventtype** types;
    if(widget != NULL){
        linked_event** widget_events = get_class_widget_events(widget->wclass->name);
        if(*widget_events == NULL){
            fprintf(stderr,"This widget seems to have no class.\n");
            return;
        }
        linked_widget_type** widget_types = &((*widget_events)->types);
        linked_widget_events* widget_event;
        while(*widget_types != NULL){
            if((*widget_types)->eventtype == eventtype){
                widget_event = (*widget_types)->widget_event;
                while(widget_event != NULL){
                    if(widget_event->widget == widget){
                        remove_event(&(widget_event->callback),callback,user_param);
                    }
                    widget_event = widget_event->next;
                }
            }
            widget_types = &((*widget_types)->next);
        }
        return;
    }
    else if(strcmp(tag,"all") == 0)
    {
        types = get_events();
        while(*types != NULL){
            if((*types)->eventtype == eventtype){
                remove_event(&((*types)->callback),callback,user_param);
                return;
            }
            types = &((*types)->next);
        }
    }
    else
    {
        linked_class** class_events = get_class_events(tag);
        if(*class_events == NULL){
            fprintf(stderr,"This tag is not valid.\n");
            return;
        }
        types = &((*class_events)->types);
        while(*types != NULL){
            if((*types)->eventtype == eventtype){
                remove_event(&((*types)->callback),callback,user_param);
                return;
            }
            types = &((*types)->next);
        }
    }
}
