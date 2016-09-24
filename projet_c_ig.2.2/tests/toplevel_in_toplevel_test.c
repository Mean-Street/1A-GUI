#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "ei_callback.h"
#include "ei_types.h"


ei_bool_t button_press(ei_widget_t* widget, ei_event_t* event, void* user_param);
ei_bool_t process_key(ei_widget_t* widget, ei_event_t* event, void* user_param);
ei_callback_t button_callback = button_press;


ei_bool_t button_press(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
    printf("Click !\n");
    return EI_TRUE;
}

ei_bool_t process_key(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	if (event->param.key.key_sym == SDLK_ESCAPE) {
		ei_app_quit_request();
		return EI_TRUE;
	}
        
        return EI_FALSE;
}


int ei_main(int argc, char** argv)
{
	ei_size_t	screen_size	= {800, 800};
        ei_color_t      root_bgcol      = {0x52, 0x7f, 0xb4, 0xff};

        ei_color_t	button_color	= {0x88, 0x88, 0x88, 0xff};
        ei_color_t      text_color      = {0x00, 0x00, 0x00, 0xff};
        ei_relief_t     relief          = ei_relief_raised;
        int             button_border_width    = 4;
        char*           button_title    = "click";

	ei_anchor_t	button_anchor	= ei_anc_southeast;
        float           button_rel_x    = 1.0;
        float           button_rel_y    = 1.0;
        int		button_x	= -16;
        int		button_y	= -16;
        float           button_rel_size_x = 0.5;
	
        ei_size_t       window_size     = {400,400};
        char*           window_title    = "Hello World";
        ei_color_t      window_color    = {0xA0,0xA0,0xA0, 0xff};
        int             window_border_width    = 2;
        ei_bool_t       closable        = EI_TRUE;
        ei_axis_set_t   window_resizable = ei_axis_both;
        ei_point_t	window_position	 = {30, 10};        
        ei_point_t	window_position1	 = {100, 20};        

        
        ei_widget_t*    button;
        ei_widget_t*    toplevel;
        ei_widget_t*    toplevel1;

	ei_app_create(&screen_size, EI_FALSE); 
        ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);        

        toplevel = ei_widget_create("toplevel", ei_app_root_widget());

        ei_toplevel_configure(toplevel, &window_size, &window_color, &window_border_width, &window_title, &closable, &window_resizable, NULL);
		
		button = ei_widget_create("button", toplevel);
        
		
        ei_button_configure(button, NULL, &button_color, 
                            &button_border_width, NULL, &relief, &button_title, NULL, &text_color, NULL,
                            NULL, NULL, NULL, &button_callback, NULL);



        toplevel1 = ei_widget_create("toplevel", toplevel);

        ei_toplevel_configure(toplevel1, &window_size, &window_color, &window_border_width, &window_title, &closable, &window_resizable, NULL);
		

        ei_place(toplevel, NULL, &(window_position.x), &(window_position.y), NULL, NULL, NULL, NULL, NULL, NULL);
        ei_place(button, &button_anchor, &button_x, &button_y, NULL,NULL, &button_rel_x, &button_rel_y, &button_rel_size_x, NULL);
        ei_place(toplevel1, NULL, &(window_position1.x), &(window_position1.y), NULL, NULL, NULL, NULL, NULL, NULL);

	ei_bind(ei_ev_keydown, NULL, "all", process_key, NULL);

	//TESTS

	ei_bind(ei_ev_mouse_buttondown, NULL, "toplevel", &(move_started), NULL);
	ei_bind(ei_ev_mouse_buttonup, NULL, "all", &(move_finished), NULL);
	ei_bind(ei_ev_mouse_move, NULL, "all", &(move_window), NULL);

	ei_bind(ei_ev_mouse_buttondown, toplevel->children_head->next_sibling, NULL, &(close_started), NULL);
	ei_bind(ei_ev_mouse_buttonup, NULL, "all", &(close_finished), NULL);


        ei_app_run();

	ei_unbind(ei_ev_keydown, NULL, "all", process_key, NULL);

	ei_app_free();

	return (EXIT_SUCCESS);
}
