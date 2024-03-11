
/*
 * File: main.c
 * Created on: Thursday, 1970-01-01 @ 01:00:00
 * Author: HackXIt (<hackxit@gmail.com>)
 * -----
 * Last Modified: Friday, 2024-01-05 @ 00:11:37
 * Modified By:  HackXIt (<hackxit@gmail.com>) @ HACKXIT
 * ----- About the code -----
 * Purpose:
 *
 * Example call:
 *
 * Example Output:
 *
 * References:
 */

/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#define _DEFAULT_SOURCE /* needed for usleep() */
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "lv_drv_conf.h"
#include "lvgl/lvgl.h"
// #include "lvgl/examples/lv_examples.h"
// #include "lvgl/demos/lv_demos.h"
#if USE_SDL
#define SDL_MAIN_HANDLED /*To fix SDL's "undefined reference to WinMain" issue*/
#include <SDL2/SDL.h>
#include "lv_drivers/sdl/sdl.h"
#elif USE_X11
#include "lv_drivers/x11/x11.h"
#endif
// #include "lv_drivers/display/monitor.h"
// #include "lv_drivers/indev/mouse.h"
// #include "lv_drivers/indev/keyboard.h"
// #include "lv_drivers/indev/mousewheel.h"

/*********************
 *   CUSTOM INCLUDES
 *********************/
#include "random_ui.h"
#include "cmd_parser.h"
#include "design_parser.h"
#include "random_design_ui.h"
#include <stdio.h>
#include <string.h>
#include "lv_100ask_screenshot.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void hal_init(void);
static void hal_deinit(void);
static void *tick_thread(void *data);

/**********************
 *  STATIC VARIABLES
 **********************/
static pthread_t thr_tick;    /* thread */
static bool end_tick = false; /* flag to terminate thread */

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *      VARIABLES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
#if 0
static void user_image_demo()
{
  lv_obj_t * img = lv_gif_create(lv_scr_act());
  lv_gif_set_src(img, "A:lvgl/examples/libs/gif/bulb.gif");
  lv_obj_align(img, LV_ALIGN_BOTTOM_RIGHT, -20, -20);

  lv_color_t bg_color = lv_palette_lighten(LV_PALETTE_LIGHT_BLUE, 5);
    lv_color_t fg_color = lv_palette_darken(LV_PALETTE_BLUE, 4);

    lv_obj_t * qr = lv_qrcode_create(lv_scr_act(), 150, fg_color, bg_color);

    /*Set data*/
    const char * data = "https://lvgl.io";
    lv_qrcode_update(qr, data, strlen(data));
    lv_obj_center(qr);

    /*Add a border with bg_color*/
    lv_obj_set_style_border_color(qr, bg_color, 0);
    lv_obj_set_style_border_width(qr, 5, 0);

    /*Create a font*/
    static lv_ft_info_t info;
    /*FreeType uses C standard file system, so no driver letter is required.*/
    info.name = "./lvgl/examples/libs/freetype/Lato-Regular.ttf";
    info.weight = 24;
    info.style = FT_FONT_STYLE_NORMAL;
    info.mem = NULL;
    if(!lv_ft_font_init(&info)) {
        LV_LOG_ERROR("create failed.");
    }

    /*Create style with the new font*/
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_font(&style, info.font);
    lv_style_set_text_align(&style, LV_TEXT_ALIGN_CENTER);

    /*Create a label with the new style*/
    lv_obj_t * label = lv_label_create(lv_scr_act());
    lv_obj_add_style(label, &style, 0);
    lv_label_set_text(label, "Hello world\nI'm a font created with FreeType");
    lv_obj_set_pos(label, 10, 10);

    lv_obj_t *  img1 = lv_img_create(lv_scr_act());
    /* Assuming a File system is attached to letter 'A'
     * E.g. set LV_USE_FS_STDIO 'A' in lv_conf.h */
    lv_img_set_src(img1, "A:lvgl/examples/libs/png/wink.png");
    lv_obj_align(img1, LV_ALIGN_LEFT_MID, 20, 0);

    lv_obj_t * wp;

    wp = lv_img_create(lv_scr_act());
    /* Assuming a File system is attached to letter 'A'
     * E.g. set LV_USE_FS_STDIO 'A' in lv_conf.h */
    lv_img_set_src(wp, "A:lvgl/examples/libs/sjpg/small_image.sjpg");
    lv_obj_align(wp, LV_ALIGN_RIGHT_MID, -20, 0);

    lv_obj_t * img2 = lv_img_create(lv_scr_act());
    /* Assuming a File system is attached to letter 'A'
     * E.g. set LV_USE_FS_STDIO 'A' in lv_conf.h */
    lv_img_set_src(img2, "A:lvgl/examples/libs/sjpg/lv_example_jpg.jpg");
    //lv_obj_center(img);
    lv_obj_align(img2, LV_ALIGN_TOP_RIGHT, -20, 20);

    lv_obj_t * img3 = lv_img_create(lv_scr_act());
    /* Assuming a File system is attached to letter 'A'
     * E.g. set LV_USE_FS_STDIO 'A' in lv_conf.h */
#if LV_COLOR_DEPTH == 32
    lv_img_set_src(img3, "A:lvgl/examples/libs/bmp/example_32bit.bmp");
#elif LV_COLOR_DEPTH == 16
    lv_img_set_src(img, "A:lvgl/examples/libs/bmp/example_16bit.bmp");
#endif
    lv_obj_align(img3, LV_ALIGN_BOTTOM_MID, 0, -20);

    lv_obj_t * img4 = lv_img_create(lv_scr_act());
    lv_img_set_src(img4, "A:lvgl/examples/libs/ffmpeg/ffmpeg.png");
    lv_obj_align(img4, LV_ALIGN_BOTTOM_LEFT, 20, -20);

    lv_obj_t * player = lv_ffmpeg_player_create(lv_scr_act());
    lv_ffmpeg_player_set_src(player, "./lvgl/examples/libs/ffmpeg/birds.mp4");
    lv_ffmpeg_player_set_auto_restart(player, true);
    lv_ffmpeg_player_set_cmd(player, LV_FFMPEG_PLAYER_CMD_START);
    lv_obj_align(player, LV_ALIGN_TOP_MID, 0, 20);
}
#endif

int main(int argc, char **argv)
{
    CmdArgs args; // Declare a variable to hold command-line arguments

    if (!parse_cmd_args(argc, argv, &args))
    {
        // If parsing fails, return with an error code
        return 1;
    }

    /*Initialize LVGL*/
    lv_init();

    /*Initialize the HAL (display, input devices, tick) for LVGL*/
    hal_init();

    // Create a randomized UI
    random_ui_t *random_ui;
    switch (args.mode)
    {
    case MODE_RANDOMIZER:
        random_ui = create_random_ui(args.randomizer_args.width, args.randomizer_args.height, (const char **)args.randomizer_args.widget_types, args.randomizer_args.type_count, args.randomizer_args.widget_count, args.randomizer_args.delay_count, args.randomizer_args.layout);
        break;
    case MODE_DESIGN_FILE:
        // Create a UI based on the design file
        create_randomized_design_ui(args.design_file_args.design_file);
        break;
    case MODE_TEST_PARSER:
        // Test the parser
        printf("Test parser\n");
        Design *design = parse_design(args.design_file_args.design_file);
        free_design(design);
        exit(0);
        break;
    default:
        fprintf(stderr, "Invalid mode\n");
        return 1;
    }

    for (int i = 0; i < args.randomizer_args.widget_count; i++)
    {
        // FIXME These coordinates are bullshit for my desired output
        printf("Widget [%d]: %s coords:(x1=%u,x2=%u,y1=%u,y2=%u) size:(w=%u,h=%u) content:(w=%u,h=%u) rel_coords:(x1=%u,x2=%u,y1=%u,y2=%u)\n", i, random_ui->elements[i].type,
               random_ui->elements[i].coords.x1, random_ui->elements[i].coords.x2, random_ui->elements[i].coords.y1, random_ui->elements[i].coords.y2,
               random_ui->elements[i].width, random_ui->elements[i].height,
               random_ui->elements[i].content_width, random_ui->elements[i].content_height,
               random_ui->elements[i].rel_coords.x1, random_ui->elements[i].rel_coords.x2, random_ui->elements[i].rel_coords.y1, random_ui->elements[i].rel_coords.y2);
    }

    lv_img_cf_t cf = LV_IMG_CF_TRUE_COLOR_ALPHA;
    // Dump the screenshot
    if (lv_100ask_screenshot_create(random_ui->container, cf, LV_100ASK_SCREENSHOT_SV_JPEG, args.randomizer_args.output_file))
    {
        printf("Screenshot saved to %s\n", args.randomizer_args.output_file);
    }
    else
    {
        fprintf(stderr, "Failed to save screenshot to %s\n", args.randomizer_args.output_file);
    }
    // Write YOLO annotations to text file (output_file.txt)
    char *dot = strrchr(args.randomizer_args.output_file, '.');
    strcpy(dot + 1, "txt"); // Replace the extension after the dot
    // Open the new file for writing
    FILE *annotation_file = fopen(args.randomizer_args.output_file + 1, "w"); // Skip the first character (the slash)
    if (annotation_file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    // FIXME These coordinates are bullshit for my desired output
    // Write a line to the file (YOLO annotation format: <class> <x_center> <y_center> <width> <height>)
    for (int i = 0; i < args.randomizer_args.widget_count; i++)
    {
        int x_center = random_ui->elements[i].rel_coords.x1 + random_ui->elements[i].width / 2;
        int y_center = random_ui->elements[i].rel_coords.y1 + random_ui->elements[i].height / 2;
        printf("YOLO [%u]: %s (x_center=%u,y_center=%u,width=%u,height=%u)\n", i, random_ui->elements[i].type, x_center, y_center, random_ui->elements[i].width, random_ui->elements[i].height);
        fprintf(annotation_file, "%s %u %u %u %u\n", random_ui->elements[i].type, x_center, y_center, random_ui->elements[i].width, random_ui->elements[i].height);
    }

    // Close the file
    fclose(annotation_file);
    hal_deinit();
    // Cleanup
    destroy_random_ui(random_ui);
    return 0;

    //  lv_example_switch_1();
    //  lv_example_calendar_1();
    //  lv_example_btnmatrix_2();
    //  lv_example_checkbox_1();
    //  lv_example_colorwheel_1();
    //  lv_example_chart_6();
    //  lv_example_table_2();
    //  lv_example_scroll_2();
    //  lv_example_textarea_1();
    //  lv_example_msgbox_1();
    //  lv_example_dropdown_2();
    //  lv_example_btn_1();
    //  lv_example_scroll_1();
    //  lv_example_tabview_1();
    //  lv_example_tabview_1();
    //  lv_example_flex_3();
    //  lv_example_label_1();

    // lv_demo_widgets();

    //  lv_demo_keypad_encoder();
    //  lv_demo_benchmark();
    //  lv_demo_stress();
    //  lv_demo_music();

    //  user_image_demo();

    while (1)
    {
        /* Periodically call the lv_task handler.
         * It could be done in a timer interrupt or an OS task too.*/
        lv_timer_handler();
        usleep(5 * 1000);
    }

    hal_deinit();
    return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Initialize the Hardware Abstraction Layer (HAL) for the LVGL graphics
 * library
 */
static void hal_init(void)
{
    /* mouse input device */
    static lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1);
    indev_drv_1.type = LV_INDEV_TYPE_POINTER;

    /* keyboard input device */
    static lv_indev_drv_t indev_drv_2;
    lv_indev_drv_init(&indev_drv_2);
    indev_drv_2.type = LV_INDEV_TYPE_KEYPAD;

    /* mouse scroll wheel input device */
    static lv_indev_drv_t indev_drv_3;
    lv_indev_drv_init(&indev_drv_3);
    indev_drv_3.type = LV_INDEV_TYPE_ENCODER;

    lv_group_t *g = lv_group_create();
    lv_group_set_default(g);

    lv_disp_t *disp = NULL;

#if USE_SDL
    /* Use the 'monitor' driver which creates window on PC's monitor to simulate a display*/
    sdl_init();

    /*Create a display buffer*/
    static lv_disp_draw_buf_t disp_buf1;
    static lv_color_t buf1_1[MONITOR_HOR_RES * 100];
    static lv_color_t buf1_2[MONITOR_HOR_RES * 100];
    lv_disp_draw_buf_init(&disp_buf1, buf1_1, buf1_2, MONITOR_HOR_RES * 100);

    /*Create a display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv); /*Basic initialization*/
    disp_drv.draw_buf = &disp_buf1;
    disp_drv.flush_cb = sdl_display_flush;
    disp_drv.hor_res = MONITOR_HOR_RES;
    disp_drv.ver_res = MONITOR_VER_RES;
    disp_drv.antialiasing = 1;

    disp = lv_disp_drv_register(&disp_drv);

    /* Add the input device driver */
    // mouse_init();
    indev_drv_1.read_cb = sdl_mouse_read;

    // keyboard_init();
    indev_drv_2.read_cb = sdl_keyboard_read;

    // mousewheel_init();
    indev_drv_3.read_cb = sdl_mousewheel_read;

#elif USE_X11
    lv_x11_init("LVGL Simulator Demo", DISP_HOR_RES, DISP_VER_RES);

    /*Create a display buffer*/
    static lv_disp_draw_buf_t disp_buf1;
    static lv_color_t buf1_1[DISP_HOR_RES * 100];
    static lv_color_t buf1_2[DISP_HOR_RES * 100];
    lv_disp_draw_buf_init(&disp_buf1, buf1_1, buf1_2, DISP_HOR_RES * 100);

    /*Create a display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &disp_buf1;
    disp_drv.flush_cb = lv_x11_flush;
    disp_drv.hor_res = DISP_HOR_RES;
    disp_drv.ver_res = DISP_VER_RES;
    disp_drv.antialiasing = 1;

    disp = lv_disp_drv_register(&disp_drv);

    /* Add the input device driver */
    indev_drv_1.read_cb = lv_x11_get_pointer;
    indev_drv_2.read_cb = lv_x11_get_keyboard;
    indev_drv_3.read_cb = lv_x11_get_mousewheel;
#endif
    /* Set diplay theme */
    lv_theme_t *th = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), LV_THEME_DEFAULT_DARK, LV_FONT_DEFAULT);
    lv_disp_set_theme(disp, th);

    /* Tick init */
    end_tick = false;
    pthread_create(&thr_tick, NULL, tick_thread, NULL);

    /* register input devices */
    lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv_1);
    lv_indev_t *kb_indev = lv_indev_drv_register(&indev_drv_2);
    lv_indev_t *enc_indev = lv_indev_drv_register(&indev_drv_3);
    lv_indev_set_group(kb_indev, g);
    lv_indev_set_group(enc_indev, g);

    /* Set a cursor for the mouse */
    LV_IMG_DECLARE(mouse_cursor_icon);                  /*Declare the image file.*/
    lv_obj_t *cursor_obj = lv_img_create(lv_scr_act()); /*Create an image object for the cursor*/
    lv_img_set_src(cursor_obj, &mouse_cursor_icon);     /*Set the image source*/
    lv_indev_set_cursor(mouse_indev, cursor_obj);       /*Connect the image  object to the driver*/
}

/**
 * Releases the Hardware Abstraction Layer (HAL) for the LVGL graphics library
 */
static void hal_deinit(void)
{
    end_tick = true;
    pthread_join(thr_tick, NULL);

#if USE_SDL
    // nop
#elif USE_X11
    lv_x11_deinit();
#endif
}

/**
 * A task to measure the elapsed time for LVGL
 * @param data unused
 * @return never return
 */
static void *tick_thread(void *data)
{
    (void)data;

    while (!end_tick)
    {
        usleep(5000);
        lv_tick_inc(5); /*Tell LittelvGL that 5 milliseconds were elapsed*/
    }

    return NULL;
}
