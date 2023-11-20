/*
 * File: random_ui.c
 * Created on: Thursday, 1970-01-01 @ 01:00:00
 * Author: HackXIt (<hackxit@gmail.com>)
 * -----
 * Last Modified: Monday, 2023-11-20 @ 03:57:22
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

#include "random_ui.h"

#if USE_SDL

extern SDL_Renderer *lv_disp_sdl_renderer;
extern SDL_Texture *lv_disp_sdl_texture;

uint8_t *dump_sdl_framebuffer()
{
    SDL_Texture *target = SDL_GetRenderTarget(lv_disp_sdl_renderer);
    SDL_SetRenderTarget(lv_disp_sdl_renderer, lv_disp_sdl_texture);

    int width, height;
    SDL_QueryTexture(lv_disp_sdl_texture, NULL, NULL, &width, &height);

    uint8_t *pixels = malloc(width * height * 4); // Assuming 32-bit color depth
    SDL_RenderReadPixels(lv_disp_sdl_renderer, NULL, SDL_PIXELFORMAT_ARGB8888, pixels, width * 4);

    SDL_SetRenderTarget(lv_disp_sdl_renderer, target);
    return pixels;
}
#endif

#if USE_X11
#include <X11/Xlib.h>
#include <X11/Xutil.h>

extern Display *display;
extern Window window;

uint8_t *dump_x11_framebuffer()
{
    /*
    // open default display
    Display *display = XOpenDisplay(NULL);
    // get default root window of display
    Window window = (XID)-1;
    */
    // Get window attributes
    XWindowAttributes gwa;
    if (!XGetWindowAttributes(display, window, &gwa))
    {
        fprintf(stderr, "Failed to get window attributes\n");
        XCloseDisplay(display);
        return NULL;
    }
    int width = gwa.width;
    int height = gwa.height;

    // Get image from the window
    XImage *image = XGetImage(display, window, 0, 0, width, height, AllPlanes, ZPixmap);
    if (image == NULL)
    {
        fprintf(stderr, "Failed to get image from window\n");
        XCloseDisplay(display);
        return NULL;
    }

    uint8_t *pixels = malloc(width * height * 4); // Assuming 32-bit color depth
    memcpy(pixels, image->data, width * height * 4);

    XDestroyImage(image);
    return pixels;
}
#endif

void seed_random()
{
    struct timeval time;
    gettimeofday(&time, NULL);
    srand((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void create_random_widget_flex(lv_obj_t *container, const char **widget_types, int type_count, int widget_count)
{
    // Flex layout with random flow
    int flex_flow = rand() % 8; // There are 8 flex flow options
    switch (flex_flow)
    {
    case 0:
        lv_obj_set_flex_flow(container, LV_FLEX_FLOW_ROW);
        break;
    case 1:
        lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
        break;
    case 2:
        lv_obj_set_flex_flow(container, LV_FLEX_FLOW_ROW_WRAP);
        break;
    case 3:
        lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN_WRAP);
        break;
    case 4:
        lv_obj_set_flex_flow(container, LV_FLEX_FLOW_ROW_REVERSE);
        break;
    case 5:
        lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN_REVERSE);
        break;
    case 6:
        lv_obj_set_flex_flow(container, LV_FLEX_FLOW_ROW_WRAP_REVERSE);
        break;
    case 7:
        lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN_WRAP_REVERSE);
        break;
    }
    for (int i = 0; i < widget_count; ++i)
    {
        lv_obj_t *widget = create_random_widget(container, widget_types, type_count);
    }
}
void create_random_widget_grid(lv_obj_t *container, const char **widget_types, int type_count, int widget_count, int num_columns, int num_rows)
{
    // Grid layout with random column and row count
    lv_obj_set_layout(container, LV_LAYOUT_GRID);

    static lv_coord_t column_dsc[16] = {0}; /*2 columns with 100 and 400 ps width*/
    static lv_coord_t row_dsc[16] = {0};    /*3 100 px tall rows*/
    for (int i = 0; i < num_columns; ++i)
    {
        column_dsc[i] = LV_GRID_CONTENT; /*1/3 part of the grid*/
    }
    for (int i = 0; i < num_rows; ++i)
    {
        row_dsc[i] = LV_GRID_CONTENT; /*1/3 part of the grid*/
    }
    column_dsc[15] = LV_GRID_TEMPLATE_LAST;
    row_dsc[15] = LV_GRID_TEMPLATE_LAST;
    lv_obj_set_grid_dsc_array(container, column_dsc, row_dsc);
    lv_grid_align_t align_x = rand() % 7; // There are 7 grid align options
    lv_grid_align_t align_y = rand() % 7; // There are 7 grid align options
    /* Align options by number
    0 - LV_GRID_ALIGN_START
    1 - LV_GRID_ALIGN_CENTER
    2 - LV_GRID_ALIGN_END
    3 - LV_GRID_ALIGN_STRETCH
    4 - LV_GRID_ALIGN_SPACE_AROUND
    5 - LV_GRID_ALIGN_SPACE_BETWEEN
    6 - LV_GRID_ALIGN_SPACE_EVENLY
    */
    for (int i = 0; i < widget_count; ++i)
    {
        lv_obj_t *widget = create_random_widget(container, widget_types, type_count);
        lv_obj_set_grid_cell(widget, align_x, rand() % num_columns, 1, align_y, rand() % num_rows, 1);
    }
}

lv_obj_t *create_random_widget(lv_obj_t *container, const char **widget_types, int type_count)
{
    const char *type = widget_types[rand() % type_count];
    lv_obj_t *widget = NULL;

    if (strcmp(type, "btn") == 0)
    {
        widget = lv_btn_create(container);
        lv_obj_set_size(widget, 100, 50);
        lv_obj_t *label = lv_label_create(widget);
        lv_label_set_text(label, "Button");
    }
    else if (strcmp(type, "checkbox") == 0)
    {
        widget = lv_checkbox_create(container);
    }
    else if (strcmp(type, "slider") == 0)
    {
        widget = lv_slider_create(container);
        lv_slider_set_range(widget, 0, 100);
    }
    else if (strcmp(type, "label") == 0)
    {
        widget = lv_label_create(container);
        lv_label_set_text(widget, "Label");
    }
    else if (strcmp(type, "switch") == 0)
    {
        widget = lv_switch_create(container);
    }
    else if (strcmp(type, "progressbar") == 0)
    {
        widget = lv_bar_create(container);
        lv_bar_set_range(widget, 0, 100);
        lv_bar_set_value(widget, rand() % 100, LV_ANIM_OFF);
    }
    return widget;
}

uint8_t *create_random_ui(int width, int height, const char **widget_types, int type_count, int widget_count, uint8_t delay_count)
{
    // Seed the random number generator with a more fine-grained time
    seed_random();
    lv_obj_t *win = lv_obj_create(lv_scr_act());
    lv_obj_set_size(win, width, height);
    lv_obj_center(win);

    // Create a container to hold widgets
    lv_obj_t *container = lv_obj_create(win);
    lv_obj_set_size(container, width, height);

    // Randomly choose between Flex and Grid layouts
    int layout_option = rand() % 2;
    if (layout_option == 0)
    {
        create_random_widget_flex(container, widget_types, type_count, widget_count);
    }
    else
    {
        create_random_widget_grid(container, widget_types, type_count, widget_count, 15, 15);
    }

    // Trigger a render to ensure the framebuffer is updated
    lv_refr_now(NULL);

    for (uint8_t i = 0; i < delay_count; i++)
    {
        /* Periodically call the lv_task handler.
         * It could be done in a timer interrupt or an OS task too.*/
        lv_timer_handler();
        usleep(5 * 1000);
    }

    // Dump the framebuffer
    uint8_t *pixels = NULL;
#if USE_SDL
    pixels = dump_sdl_framebuffer();
#elif USE_X11
    pixels = dump_x11_framebuffer();
#endif

    // Clean up
    lv_obj_del(win);

    return pixels;
}
