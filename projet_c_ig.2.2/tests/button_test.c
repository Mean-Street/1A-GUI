#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "ei_utils.h"


/*
 * button_press --
 *
 *	Callback called when a user clicks on the button.
 */
ei_bool_t button_press(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	printf("Click !\n");
	return EI_TRUE;
}

/*
 * process_key --
 *
 *	Callback called when any key is pressed by the user.
 *	Simply looks for the "Escape" key to request the application to quit.
 */
ei_bool_t process_key(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	if (event->param.key.key_sym == SDLK_ESCAPE) {
		ei_app_quit_request();
		return EI_TRUE;
	}
	return EI_FALSE;
}

/*
 * ei_main --
 *
 *	Main function of the application.
 */
int ei_main(int argc, char** argv)
{
	ei_size_t	screen_size		= {1200, 800};
	ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};

	ei_widget_t*	button;
	ei_widget_t*	button1;
	ei_widget_t*	button2;
	ei_widget_t*	button3;
	ei_widget_t*	button4;
	ei_widget_t*	button5;
	ei_widget_t*	button6;
	ei_widget_t*	button7;
	ei_size_t	button_size		= {300,200};
	int		button_x		= 15;
	int		button_y		= 580;
	ei_color_t	button_color		= {0x88, 0x88, 0x88, 0xff};
	char*		button_title		= "Mon premier Bouton !";
	ei_color_t	button_text_color	= {0x00, 0x00, 0x00, 0xff};
	int		button_corner_radius	= 25;
	ei_relief_t	button_relief		= ei_relief_raised;
	int		button_border_width	= 6;
	ei_callback_t	button_callback 	= button_press;

    ei_point_t img_top_left = ei_point(50,50);

    ei_rect_t* img_rect = malloc(sizeof(ei_rect_t));
    *img_rect = ei_rect(img_top_left,ei_size(100,100));  

    //WHAT ???????????
//	button_size = ei_size(300,200);

	/* Create the application and change the color of the background. */
	ei_app_create(&screen_size, EI_FALSE);
   
    //AJOUT 
    ei_surface_t img =  hw_image_load("misc/klimt.jpg",ei_app_root_surface());

	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	//ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, &img, NULL, NULL);

	/* Create, configure and place the button on screen. */
	button = ei_widget_create("button", ei_app_root_widget());
	ei_button_configure	(button, &button_size, &button_color,
				 &button_border_width, &button_corner_radius, &button_relief, &button_title, NULL, &button_text_color, NULL,
				 NULL, NULL, NULL, &button_callback, NULL);
	ei_place(button, NULL, &button_x, &button_y, NULL, NULL, NULL, NULL, NULL, NULL );

    button_x = 350;
    button_y = 580;

	button1 = ei_widget_create("button", ei_app_root_widget());
	//ei_button_configure	(button1, &button_size, &button_color,
	ei_button_configure	(button1, NULL, &button_color,
				 &button_border_width, &button_corner_radius, &button_relief, NULL, NULL, NULL, NULL,
				 &img, &img_rect, NULL, &button_callback, NULL);
	ei_place(button1, NULL, &button_x, &button_y, NULL, NULL, NULL, NULL, NULL, NULL );

    button_x = 600;
    button_y = 250;

	button7 = ei_widget_create("button", ei_app_root_widget());
	ei_button_configure	(button7, &button_size, &button_color,
				 &button_border_width, &button_corner_radius, &button_relief, NULL, NULL, NULL, NULL,
				 &img, &img_rect, NULL, &button_callback, NULL);
	ei_place(button7, NULL, &button_x, &button_y, NULL, NULL, NULL, NULL, NULL, NULL );

    button_x = 700;
    button_y = 580;

	button2 = ei_widget_create("button", ei_app_root_widget());
	ei_button_configure	(button2, &button_size, &button_color,
				 &button_border_width, &button_corner_radius, &button_relief, NULL, NULL, NULL, NULL,
				 &img, NULL, NULL, &button_callback, NULL);
	ei_place(button2, NULL, &button_x, &button_y, NULL, NULL, NULL, NULL, NULL, NULL );

    button_x = 20;
    button_y = 20;

	button3 = ei_widget_create("button", ei_app_root_widget());
	ei_button_configure	(button3, NULL, &button_color,
				 &button_border_width, &button_corner_radius, &button_relief, NULL, NULL, NULL, NULL,
				 &img, NULL, NULL, &button_callback, NULL);
	ei_place(button3, NULL, &button_x, &button_y, NULL, NULL, NULL, NULL, NULL, NULL );

    button_x = 600;
    button_y = 20;

    img_rect->size = button_size;  

	button4 = ei_widget_create("button", ei_app_root_widget());
	ei_button_configure	(button4, NULL, &button_color,
				 &button_border_width, &button_corner_radius, &button_relief, NULL, NULL, NULL, NULL,
				 &img, &img_rect, NULL, &button_callback, NULL);
	ei_place(button4, NULL, &button_x, &button_y, NULL, NULL, NULL, NULL, NULL, NULL );

    button_x = 1000;
    button_y = 250;

	button5 = ei_widget_create("button", ei_app_root_widget());
	ei_button_configure	(button5, &button_size, &button_color,
				 &button_border_width, &button_corner_radius, &button_relief, NULL, NULL, NULL, NULL,
				 &img, &img_rect, NULL, &button_callback, NULL);
	ei_place(button5, NULL, &button_x, &button_y, NULL, NULL, NULL, NULL, NULL, NULL );

    button_x = 1000;
    button_y = 650;

	button6 = ei_widget_create("button", ei_app_root_widget());
	ei_button_configure	(button6, NULL, &button_color,
				 &button_border_width, &button_corner_radius, &button_relief, NULL, NULL, NULL, NULL,
				 &img, &img_rect, NULL, &button_callback, NULL);
	ei_place(button6, NULL, &button_x, &button_y, NULL, NULL, NULL, NULL, NULL, NULL );


//	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, &img, NULL, NULL);

/*	button = ei_widget_create("button", ei_app_root_widget());
	ei_button_configure	(button, &button_size, &button_color,
				 &button_border_width, &button_corner_radius, &button_relief, NULL, NULL, &button_text_color, NULL,
				 &img, NULL, NULL, &button_callback, NULL);
*/

	/* Hook the keypress callback to the event. */
	ei_bind(ei_ev_keydown,		NULL, "all", process_key, NULL);

	/* Run the application's main loop. */
	ei_app_run();

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_unbind(ei_ev_keydown,	NULL, "all", process_key, NULL);
	ei_app_free();

	return (EXIT_SUCCESS);
}
