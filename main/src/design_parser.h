#pragma once

#include "lvgl/lvgl.h"
#include "jansson.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma region STYLE
#define STYLE_PROP_MAX 10 // Adjust based on the number of style properties
#define BASE_16 16
typedef enum
{
    STYLE_PROP_INHERIT,
    STYLE_PROP_WIDTH,
    STYLE_PROP_HEIGHT,
    STYLE_PROP_BG_COLOR,
    // ... other style property indexes ...
    STYLE_PROP_UNKNOWN
} StylePropertyIndex;

typedef struct Style
{
    lv_style_t *lv_style; // LVGL style object
    bool inherit;         // Flag for inheritance
} Style;
StylePropertyIndex get_style_property_index(const char *prop_name);
Style *parse_style(json_t *json_style);

#pragma endregion STYLE

#pragma region PORTION

typedef struct
{
    lv_flex_flow_t flex_flow;          // Represents the flex flow (row, column, wrap, etc.)
    lv_flex_align_t main_place;        // Alignment in the main direction (justify-content in CSS)
    lv_flex_align_t cross_place;       // Alignment in the cross direction (align-items in CSS)
    lv_flex_align_t track_cross_place; // Alignment of tracks (align-content in CSS)
    uint8_t grow;                      // Flex grow property for children
} FlexLayoutProps;
lv_flex_align_t get_flex_align(const char *align);
lv_flex_flow_t get_flex_flow(const char *flow);
void parse_flex_layout(FlexLayoutProps *flex_props, json_t *json_flex_props);

typedef struct
{
    uint8_t *rows;
    uint8_t *cols;
    lv_align_t column_align;
    lv_align_t row_align;
    lv_align_t justify_content;
    lv_coord_t *col_dsc; // Placeholder for column descriptors array
    lv_coord_t *row_dsc; // Placeholder for row descriptors array
} GridLayoutProps;
lv_grid_align_t get_grid_align(const char *align);
void parse_grid_layout(GridLayoutProps *grid_props, json_t *json_grid_props);

typedef struct
{
    lv_coord_t x;
    lv_coord_t y;
    lv_coord_t w;
    lv_coord_t h;
    lv_align_t align;
} AbsoluteLayoutProps;
lv_align_t get_absolute_align(const char *align);
void parse_absolute_layout(AbsoluteLayoutProps *abs_props, json_t *json_abs_props);

typedef enum
{
    LAYOUT_ABSOLUTE,
    LAYOUT_GRID,
    LAYOUT_FLEX,
} lv_layout_t;

typedef struct Portion
{
    char *id;
    lv_layout_t layout_type; // Placeholder for the LVGL layout enum
    union
    {
        FlexLayoutProps flex_props; // Flex layout properties
        GridLayoutProps grid_props; // Actual LVGL grid layout properties
        AbsoluteLayoutProps absolute_props;
    } layout_props;
    Style *style;
    struct Portion *next; // Next portion in a linked list
} Portion;

void parse_layout_type(Portion *portion, json_t *json_layout);
Portion *parse_portion(json_t *json_portion);
#pragma endregion PORTION

#pragma region DESIGN
typedef struct Design
{
    Style *root;            // The root style
    Portion *first_portion; // First portion in a linked list
} Design;

Design *parse_design(const char *file_path);
#pragma endregion DESIGN

#pragma region CLEANUP
void free_style(Style *style);
void free_portion(Portion *portion);
void free_design(Design *design);
#pragma endregion CLEANUP
