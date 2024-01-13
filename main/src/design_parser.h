#pragma once

#include "lvgl/lvgl.h"

#define STYLE_PROP_MAX 10 // Adjust based on the number of style properties
#define BASE_16 16

typedef enum
{
    STYLE_PROP_INHERIT,
    STYLE_PROP_WIDTH,
    STYLE_PROP_HEIGHT,
    STYLE_PROP_BG_COLOR,
    // ... other style property indexes ...
} StylePropertyIndex;

typedef struct Style
{
    lv_style_t *lv_style; // LVGL style object
    bool inherit;         // Flag for inheritance
} Style;

typedef struct
{
    lv_coord_t x;
    lv_coord_t y;
    lv_coord_t w;
    lv_coord_t h;
    lv_align_t align;
} AbsoluteLayoutProps;

typedef struct
{
    lv_align_t column_align;
    lv_align_t row_align;
    lv_coord_t *col_dsc; // Placeholder for column descriptors array
    lv_coord_t *row_dsc; // Placeholder for row descriptors array
} GridLayoutProps;

typedef struct
{
    lv_flex_flow_t flex_flow;          // Represents the flex flow (row, column, wrap, etc.)
    lv_flex_align_t main_place;        // Alignment in the main direction (justify-content in CSS)
    lv_flex_align_t cross_place;       // Alignment in the cross direction (align-items in CSS)
    lv_flex_align_t track_cross_place; // Alignment of tracks (align-content in CSS)
    uint8_t grow;                      // Flex grow property for children
} FlexLayoutProps;

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
    Style style;
    struct Portion *next; // Next portion in a linked list
} Portion;

typedef struct Design
{
    Style root;             // The root style
    Portion *first_portion; // First portion in a linked list
} Design;

// Functions to parse each specialized key
Portion *parse_portion(json_t *json_portion);
void parse_layout_type(Portion *portion, json_t *json_layout);
void parse_style(Style *style, json_t *json_style);
void init_root_style(Style *style, json_t *json_style);
// The main parsing function
Design *parse_design(const char *file_path);

// ... other function declarations ...
