/*
 * File: random_ui.h
 * Created on: Thursday, 1970-01-01 @ 01:00:00
 * Author: HackXIt (<hackxit@gmail.com>)
 * -----
 * Last Modified: Thursday, 2024-01-04 @ 23:56:23
 * Modified By:  HackXIt (<hackxit@gmail.com>) @ HACKXIT
 * -----
 */

#ifndef RANDOM_UI_H
#define RANDOM_UI_H
#include "lv_drv_conf.h"
#include "lvgl/lvgl.h"
#include <sys/time.h> // For gettimeofday()
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#define MAX_LAYOUT_OPTIONS 2
#define MAX_FLEX_FLOW_OPTIONS 8
#define MAX_GRID_ALIGN_OPTIONS 7

typedef struct coords
{
    int x1;
    int y1;
    int x2;
    int y2;
} coords_t;

typedef struct random_ui_element
{
    lv_obj_t *widget;
    lv_area_t coords;
    int width;
    int content_width;
    int height;
    int content_height;
    coords_t rel_coords;
    char *type;
} random_ui_element_t;

typedef struct random_ui
{
    int width;
    int height;
    const char **widget_types;
    int type_count;
    int widget_count;
    uint8_t delay_count;
    lv_obj_t *container;
    const char *layout;
    random_ui_element_t *elements;
} random_ui_t;

typedef struct
{
    int row;
    int col;
} Cell;

void seed_random(void);
void create_random_layout_flex(random_ui_t *random_ui);
void shuffleCells(Cell *cells, int count);
void create_random_layout_grid(random_ui_t *random_ui);
void write_widget_type(char **widget_type, const char *type);
lv_obj_t *create_random_widget(lv_obj_t *container, const char **widget_types, int type_count, char **widget_type);
void get_element_spatial_info(random_ui_t *random_ui, random_ui_element_t *element);
random_ui_t *create_random_ui(int width, int height, const char **widget_types, int type_count, int widget_count, uint8_t delay_count, const char *layout);
void destroy_random_ui(random_ui_t *random_ui);
#endif
