#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"

/*
 * ei_main --
 *
 * Main function of the application.
 */

ei_bool_t frame_press(ei_widget_t* widget,ei_event_t* event,void* user_param)
{
    printf("Cliked on %d.\n",widget->pick_id);
    return EI_TRUE;
}

int ei_main(int argc, char** argv)
{
	ei_size_t screen_size = {1000, 600};
	ei_color_t root_bgcol = {0x55, 0x55, 0xff, 0xff};
	int frame_border_width = 6;
	ei_relief_t frame_relief = ei_relief_raised;

	/* Create the application and change the color of the background. */
	ei_app_create(&screen_size, EI_FALSE);
	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);


	/* frame 1 */
	ei_size_t frame1_size = {900,500};
	ei_color_t frame1_color = {0xaa, 0xaa, 0xaa, 0xff};
	ei_anchor_t frame1_anc = ei_anc_center;
	int frame1_x = 500;
	int frame1_y = 300;

	ei_widget_t *frame1 = ei_widget_create("frame", ei_app_root_widget());
	ei_frame_configure(frame1, &frame1_size, &frame1_color, &frame_border_width, &frame_relief, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	ei_place(frame1, &frame1_anc, &frame1_x, &frame1_y, NULL, NULL, NULL, NULL, NULL, NULL);


	/* frame 2 */
	ei_size_t frame2_size = {0,0};
	ei_color_t frame2_color = {0x00, 0x00, 0x00, 0xff};
	ei_anchor_t frame2_anc = ei_anc_southwest;
	float frame2_x_rel = 0.0;
	float frame2_y_rel = 1.0;
	float width2_rel = 1.0;
	int height2 = 100;

	ei_widget_t *frame2 = ei_widget_create("frame", frame1);
	ei_frame_configure(frame2, &frame2_size, &frame2_color, &frame_border_width, &frame_relief, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	ei_place(frame2, &frame2_anc, NULL, NULL, NULL, &height2, &frame2_x_rel, &frame2_y_rel, &width2_rel, NULL);


	/* frame 3 */
	ei_size_t frame3_size = {0,0};
	ei_color_t frame3_color = {0x00, 0xaa, 0xaa, 0xff};
	ei_anchor_t frame3_anc = ei_anc_northeast;
	float frame3_x_rel = 1.0;
	int frame3_x = -10;
	int frame3_y = 10;
	float frame3_width_rel = 1.0;
	float frame3_height_rel = 0.5;
	int frame3_width = -20;

	ei_widget_t *frame3 = ei_widget_create("frame", frame1);
	ei_frame_configure(frame3, &frame3_size, &frame3_color, &frame_border_width, &frame_relief, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	ei_place(frame3, &frame3_anc, &frame3_x, &frame3_y, &frame3_width, NULL, &frame3_x_rel, NULL, &frame3_width_rel, &frame3_height_rel);

	
	/* frame 4 */
	ei_size_t frame4_size = {300,100};
	ei_color_t frame4_color = {0x00, 0xaa, 0x00, 0xff};
	ei_anchor_t frame4_anc = ei_anc_south;
	float frame4_x_rel = 0.5;
	float frame4_y_rel = 0.95;

	ei_widget_t *frame4 = ei_widget_create("frame", frame3);
	ei_frame_configure(frame4, &frame4_size, &frame4_color, &frame_border_width, &frame_relief, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	ei_place(frame4, &frame4_anc, NULL, NULL, NULL, NULL, &frame4_x_rel, &frame4_y_rel, NULL, NULL);


	/* frame 5 */
	ei_size_t frame5_size = {300,100};
	ei_color_t frame5_color = {0x00, 0xaa, 0x00, 0xff};
	ei_anchor_t frame5_anc = ei_anc_north;
	float frame5_x_rel = 0.5;
	float frame5_y_rel = 0.05;

	ei_widget_t *frame5 = ei_widget_create("frame", frame3);
	ei_frame_configure(frame5, &frame5_size, &frame5_color, &frame_border_width, &frame_relief, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	ei_place(frame5, &frame5_anc, NULL, NULL, NULL, NULL, &frame5_x_rel, &frame5_y_rel, NULL, NULL);


	/* frame 6 */
	ei_size_t frame6_size = {400,100};
	ei_color_t frame6_color = {0x00, 0x00, 0xaa, 0x88};
	ei_anchor_t frame6_anc = ei_anc_east;
	float frame6_x_rel = 0.98;
	float frame6_y_rel = 0.5;

	ei_widget_t *frame6 = ei_widget_create("frame", frame3);
	ei_frame_configure(frame6, &frame6_size, &frame6_color, &frame_border_width, &frame_relief, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	ei_place(frame6, &frame6_anc, NULL, NULL, NULL, NULL, &frame6_x_rel, &frame6_y_rel, NULL, NULL);


	/* frame 7 */
	ei_size_t frame7_size = {400,100};
	ei_color_t frame7_color = {0x00, 0x00, 0xaa, 0xff};
	ei_anchor_t frame7_anc = ei_anc_west;
	float frame7_x_rel = 0.02;
	float frame7_y_rel = 0.5;

	ei_widget_t *frame7 = ei_widget_create("frame", frame3);
	ei_frame_configure(frame7, &frame7_size, &frame7_color, &frame_border_width, &frame_relief, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	ei_place(frame7, &frame7_anc, NULL, NULL, NULL, NULL, &frame7_x_rel, &frame7_y_rel, NULL, NULL);


	/* frame 8 */
	ei_size_t frame8_size = {0,0};
	ei_color_t frame8_color = {0xaa, 0x00, 0x00, 0x88};
	ei_anchor_t frame8_anc = ei_anc_southeast;
	float frame8_width_rel = 0.5;
	float frame8_height_rel = 0.5;
	int frame8_width = -10;
	int frame8_height = -10;
	float frame8_x_rel  = 0.5;
	float frame8_y_rel  = 0.5;

	ei_widget_t *frame8 = ei_widget_create("frame", frame2);
	ei_frame_configure(frame8, &frame8_size, &frame8_color, &frame_border_width, &frame_relief, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	ei_place(frame8, &frame8_anc, NULL, NULL, &frame8_width, &frame8_height, &frame8_x_rel, &frame8_y_rel, &frame8_width_rel, &frame8_height_rel);


	/* frame 9 */
	ei_size_t frame9_size = {0,0};
	ei_color_t frame9_color = {0xaa, 0x00, 0x00, 0xff};
	ei_anchor_t frame9_anc = ei_anc_southwest;
	float frame9_width_rel = 0.5;
	float frame9_height_rel = 0.5;
	int frame9_width = -10;
	int frame9_height = -10;
	float frame9_x_rel  = 0.5;
	float frame9_y_rel  = 0.5;

	ei_widget_t *frame9 = ei_widget_create("frame", frame2);
	ei_frame_configure(frame9, &frame9_size, &frame9_color, &frame_border_width, &frame_relief, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	ei_place(frame9, &frame9_anc, NULL, NULL, &frame9_width, &frame9_height, &frame9_x_rel, &frame9_y_rel, &frame9_width_rel, &frame9_height_rel);


	/* frame 10 */
	ei_size_t frame10_size = {0,0};
	ei_color_t frame10_color = {0xaa, 0x00, 0x00, 0xff};
	ei_anchor_t frame10_anc = ei_anc_northwest;
	float frame10_width_rel = 0.5;
	float frame10_height_rel = 0.5;
	int frame10_width = -10;
	int frame10_height = -10;
	float frame10_x_rel  = 0.5;
	float frame10_y_rel  = 0.5;

	ei_widget_t *frame10 = ei_widget_create("frame", frame2);
	ei_frame_configure(frame10, &frame10_size, &frame10_color, &frame_border_width, &frame_relief, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	ei_place(frame10, &frame10_anc, NULL, NULL, &frame10_width, &frame10_height, &frame10_x_rel, &frame10_y_rel, &frame10_width_rel, &frame10_height_rel);


	/* frame 11 */
	ei_size_t frame11_size = {0,0};
	ei_color_t frame11_color = {0xaa, 0x00, 0x00, 0xff};
	ei_anchor_t frame11_anc = ei_anc_northeast;
	float frame11_width_rel = 0.5;
	float frame11_height_rel = 0.5;
	int frame11_width = -10;
	int frame11_height = -10;
	float frame11_x_rel  = 0.5;
	float frame11_y_rel  = 0.5;

	ei_widget_t *frame11 = ei_widget_create("frame", frame2);
	ei_frame_configure(frame11, &frame11_size, &frame11_color, &frame_border_width, &frame_relief, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	ei_place(frame11, &frame11_anc, NULL, NULL, &frame11_width, &frame11_height, &frame11_x_rel, &frame11_y_rel, &frame11_width_rel, &frame11_height_rel);

    /*ei_bind(ei_ev_mouse_buttondown,frame1,NULL,frame_press,NULL);
    ei_bind(ei_ev_mouse_buttondown,frame2,NULL,frame_press,NULL);
    ei_bind(ei_ev_mouse_buttondown,frame3,NULL,frame_press,NULL);
    ei_bind(ei_ev_mouse_buttondown,frame4,NULL,frame_press,NULL);
    ei_bind(ei_ev_mouse_buttondown,frame5,NULL,frame_press,NULL);
    ei_bind(ei_ev_mouse_buttondown,frame6,NULL,frame_press,NULL);
    ei_bind(ei_ev_mouse_buttondown,frame7,NULL,frame_press,NULL);
    ei_bind(ei_ev_mouse_buttondown,frame8,NULL,frame_press,NULL);
    ei_bind(ei_ev_mouse_buttondown,frame9,NULL,frame_press,NULL);
    ei_bind(ei_ev_mouse_buttondown,frame10,NULL,frame_press,NULL);
    ei_bind(ei_ev_mouse_buttondown,frame11,NULL,frame_press,NULL);

    ei_unbind(ei_ev_mouse_buttondown,frame11,NULL,frame_press,NULL);
    ei_unbind(ei_ev_mouse_buttondown,frame2,NULL,frame_press,NULL);
    ei_unbind(ei_ev_mouse_buttondown,frame3,NULL,frame_press,NULL);
    ei_unbind(ei_ev_mouse_buttondown,frame4,NULL,frame_press,NULL);
    ei_unbind(ei_ev_mouse_buttondown,frame7,NULL,frame_press,NULL);
    ei_unbind(ei_ev_mouse_buttondown,frame6,NULL,frame_press,NULL);
    ei_unbind(ei_ev_mouse_buttondown,frame5,NULL,frame_press,NULL);
    ei_unbind(ei_ev_mouse_buttondown,frame8,NULL,frame_press,NULL);
    ei_unbind(ei_ev_mouse_buttondown,frame9,NULL,frame_press,NULL);
    ei_unbind(ei_ev_mouse_buttondown,frame10,NULL,frame_press,NULL);
    ei_unbind(ei_ev_mouse_buttondown,frame1,NULL,frame_press,NULL);*/

    //ei_bind(ei_ev_mouse_buttondown,NULL,"frame",frame_press,NULL);
    ei_bind(ei_ev_mouse_buttondown,NULL,"frame",&frame_press,NULL);


	/* Run the application's main loop. */
	ei_app_run();

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_app_free();

	return EXIT_SUCCESS;
}
