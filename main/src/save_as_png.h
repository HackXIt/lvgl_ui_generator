/*
 * File: save_as_png.h
 * Created on: Thursday, 1970-01-01 @ 01:00:00
 * Author: https://github.com/100askTeam/lv_lib_100ask
 * -----
 * Last Modified: Monday, 2023-11-20 @ 05:20:39
 * Modified By:  HackXIt (<hackxit@gmail.com>) @ HACKXIT
 * -----
 */

/**
 * @file save_as_png.h
 *
 */

#ifndef SAVE_AS_PNG_H
#define SAVE_AS_PNG_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lv_lib_100ask.h"
#if LV_USE_100ASK_SCREENSHOT != 0 && LV_USE_PNG

#if LV_USE_PNG == 0
#error "LV_USE_PNG must be defined in lv_conf.h"
#endif

    /*********************
     *      DEFINES
     *********************/

    /**********************
     *      TYPEDEFS
     **********************/

    /***********************
     * GLOBAL VARIABLES
     ***********************/

    /**********************
     * GLOBAL PROTOTYPES
     **********************/
    bool save_as_png_file(uint8_t *image, uint32_t w, uint32_t h, uint32_t bpp, char *filename);

    /*=====================
     * Setter functions
     *====================*/

    /*=====================
     * Getter functions
     *====================*/

    /*=====================
     * Other functions
     *====================*/

    /**********************
     *      MACROS
     **********************/

#endif /*LV_USE_SCREENSHOT*/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*SAVE_AS_PNG_H*/
