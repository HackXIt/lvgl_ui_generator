/*
 * File: random_ui.h
 * Created on: Thursday, 1970-01-01 @ 01:00:00
 * Author: HackXIt (<hackxit@gmail.com>)
 * -----
 * Last Modified: Monday, 2023-11-20 @ 03:38:50
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
#if USE_SDL
#define SDL_MAIN_HANDLED /*To fix SDL's "undefined reference to WinMain" issue*/
#include <SDL2/SDL.h>
#include "lv_drivers/sdl/sdl.h"
#elif USE_X11
#include "lv_drivers/x11/x11.h"
#endif

void create_random_widget_flex(lv_obj_t *container, const char **widget_types, int type_count, int widget_count);
void create_random_widget_grid(lv_obj_t *container, const char **widget_types, int type_count, int widget_count, int num_columns, int num_rows);
lv_obj_t *create_random_widget(lv_obj_t *container, const char **widget_types, int type_count);
uint8_t *create_random_ui(int width, int height, const char **widget_types, int type_count, int widget_count, uint8_t delay_count);
#endif
