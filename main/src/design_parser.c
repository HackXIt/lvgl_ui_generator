#include "design_parser.h"

#pragma region STYLE

StylePropertyIndex get_style_property_index(const char *prop_name)
{
    if (strcmp(prop_name, "inherit") == 0)
    {
        return STYLE_PROP_INHERIT;
    }
    else if (strcmp(prop_name, "width") == 0)
    {
        return STYLE_PROP_WIDTH;
    }
    else if (strcmp(prop_name, "height") == 0)
    {
        return STYLE_PROP_HEIGHT;
    }
    else if (strcmp(prop_name, "bg_color") == 0)
    {
        return STYLE_PROP_BG_COLOR;
    }
    // ... other cases ...
    return STYLE_PROP_UNKNOWN;
}

// Function to parse style
Style *parse_style(json_t *json_style)
{
    if (!json_style)
    {
        return NULL; // Handle null pointers
    }

    Style *style = calloc(1, sizeof(Style));

    // Initialize lv_style_t object
    lv_style_t *lv_style = calloc(1, sizeof(lv_style_t));
    lv_style_init(lv_style);

    const char *key;
    json_t *value;
    json_object_foreach(json_style, key, value)
    {
        StylePropertyIndex index = get_style_property_index(key);
        switch (index)
        {
        case STYLE_PROP_INHERIT:
            style->inherit = json_boolean_value(value);
            break;
        case STYLE_PROP_WIDTH:
            lv_style_set_width(lv_style, json_integer_value(value));
            break;
        case STYLE_PROP_HEIGHT:
            lv_style_set_height(lv_style, json_integer_value(value));
            break;
        case STYLE_PROP_BG_COLOR:
            const char *hex_str = json_string_value(value);
            uint32_t hex_num = (uint32_t)strtol(hex_str, NULL, BASE_16); // Convert HEX string to integer
            lv_color_t color = lv_color_hex(hex_num);                    // Convert to lv_color_t
            lv_style_set_bg_color(lv_style, color);                      // Set background color
            break;
        // ... other cases ...
        case STYLE_PROP_UNKNOWN:
            // Handle unknown property
            break;
        default:
            // Handle unknown property
            break;
        }
    }
    return style;
}
#pragma endregion STYLE

#pragma region PORTION

/* NOTE Flex alignments
typedef enum {
    LV_FLEX_ALIGN_START,
    LV_FLEX_ALIGN_END,
    LV_FLEX_ALIGN_CENTER,
    LV_FLEX_ALIGN_SPACE_EVENLY,
    LV_FLEX_ALIGN_SPACE_AROUND,
    LV_FLEX_ALIGN_SPACE_BETWEEN,
} lv_flex_align_t;
*/
lv_flex_align_t get_flex_align(const char *align)
{
    if (strcmp(align, "start") == 0)
    {
        return LV_FLEX_ALIGN_START;
    }
    else if (strcmp(align, "end") == 0)
    {
        return LV_FLEX_ALIGN_END;
    }
    else if (strcmp(align, "center") == 0)
    {
        return LV_FLEX_ALIGN_CENTER;
    }
    else if (strcmp(align, "space_evenly") == 0)
    {
        return LV_FLEX_ALIGN_SPACE_EVENLY;
    }
    else if (strcmp(align, "space_around") == 0)
    {
        return LV_FLEX_ALIGN_SPACE_AROUND;
    }
    else if (strcmp(align, "space_between") == 0)
    {
        return LV_FLEX_ALIGN_SPACE_BETWEEN;
    }
    return LV_FLEX_ALIGN_START;
}

/* NOTE Flex flows
typedef enum {
    LV_FLEX_FLOW_ROW                 = 0x00,
    LV_FLEX_FLOW_COLUMN              = _LV_FLEX_COLUMN,
    LV_FLEX_FLOW_ROW_WRAP            = LV_FLEX_FLOW_ROW | _LV_FLEX_WRAP,
    LV_FLEX_FLOW_ROW_REVERSE         = LV_FLEX_FLOW_ROW | _LV_FLEX_REVERSE,
    LV_FLEX_FLOW_ROW_WRAP_REVERSE    = LV_FLEX_FLOW_ROW | _LV_FLEX_WRAP | _LV_FLEX_REVERSE,
    LV_FLEX_FLOW_COLUMN_WRAP         = LV_FLEX_FLOW_COLUMN | _LV_FLEX_WRAP,
    LV_FLEX_FLOW_COLUMN_REVERSE      = LV_FLEX_FLOW_COLUMN | _LV_FLEX_REVERSE,
    LV_FLEX_FLOW_COLUMN_WRAP_REVERSE = LV_FLEX_FLOW_COLUMN | _LV_FLEX_WRAP | _LV_FLEX_REVERSE,
} lv_flex_flow_t;
*/
lv_flex_flow_t get_flex_flow(const char *flow)
{
    if (strcmp(flow, "row") == 0)
    {
        return LV_FLEX_FLOW_ROW;
    }
    else if (strcmp(flow, "column") == 0)
    {
        return LV_FLEX_FLOW_COLUMN;
    }
    else if (strcmp(flow, "row_wrap") == 0)
    {
        return LV_FLEX_FLOW_ROW_WRAP;
    }
    else if (strcmp(flow, "column_wrap") == 0)
    {
        return LV_FLEX_FLOW_COLUMN_WRAP;
    }
    else if (strcmp(flow, "row_reverse") == 0)
    {
        return LV_FLEX_FLOW_ROW_REVERSE;
    }
    else if (strcmp(flow, "column_reverse") == 0)
    {
        return LV_FLEX_FLOW_COLUMN_REVERSE;
    }
    else if (strcmp(flow, "row_wrap_reverse") == 0)
    {
        return LV_FLEX_FLOW_ROW_WRAP_REVERSE;
    }
    else if (strcmp(flow, "column_wrap_reverse") == 0)
    {
        return LV_FLEX_FLOW_COLUMN_WRAP_REVERSE;
    }
    return LV_FLEX_FLOW_ROW;
}

void parse_flex_layout(FlexLayoutProps *flex_props, json_t *json_flex_props)
{
    if (!flex_props || !json_flex_props)
        return;

    // Parse flex_flow
    const char *flex_flow = json_string_value(json_object_get(json_flex_props, "flex_flow"));
    flex_props->flex_flow = get_flex_flow(flex_flow);

    // Parse main_place (justify-content)
    const char *main_place = json_string_value(json_object_get(json_flex_props, "main_place"));
    flex_props->main_place = get_flex_align(main_place);

    // Parse cross_place (align-items)
    const char *cross_place = json_string_value(json_object_get(json_flex_props, "cross_place"));
    flex_props->cross_place = get_flex_align(cross_place);

    // Parse track_cross_place (align-content)
    const char *track_cross_place = json_string_value(json_object_get(json_flex_props, "track_cross_place"));
    flex_props->track_cross_place = get_flex_align(track_cross_place);

    // Parse grow (flex-grow)
    flex_props->grow = (uint8_t)json_integer_value(json_object_get(json_flex_props, "grow"));
}

/* NOTE Grid alignments
typedef enum {
    LV_GRID_ALIGN_START,
    LV_GRID_ALIGN_CENTER,
    LV_GRID_ALIGN_END,
    LV_GRID_ALIGN_STRETCH,
    LV_GRID_ALIGN_SPACE_EVENLY,
    LV_GRID_ALIGN_SPACE_AROUND,
    LV_GRID_ALIGN_SPACE_BETWEEN,
} lv_grid_align_t;
*/
lv_grid_align_t get_grid_align(const char *align)
{
    if (strcmp(align, "start") == 0)
    {
        return LV_GRID_ALIGN_START;
    }
    else if (strcmp(align, "center") == 0)
    {
        return LV_GRID_ALIGN_CENTER;
    }
    else if (strcmp(align, "end") == 0)
    {
        return LV_GRID_ALIGN_END;
    }
    else if (strcmp(align, "stretch") == 0)
    {
        return LV_GRID_ALIGN_STRETCH;
    }
    else if (strcmp(align, "space_evenly") == 0)
    {
        return LV_GRID_ALIGN_SPACE_EVENLY;
    }
    else if (strcmp(align, "space_around") == 0)
    {
        return LV_GRID_ALIGN_SPACE_AROUND;
    }
    else if (strcmp(align, "space_between") == 0)
    {
        return LV_GRID_ALIGN_SPACE_BETWEEN;
    }
    return LV_GRID_ALIGN_START;
}

void parse_grid_layout(GridLayoutProps *grid_props, json_t *json_grid_props)
{
    if (!grid_props || !json_grid_props)
        return;

    // Parse rows and columns as arrays of lv_coord_t values
    json_t *json_rows = json_object_get(json_grid_props, "rows");
    json_t *json_cols = json_object_get(json_grid_props, "columns");

    size_t rows_count = json_array_size(json_rows);
    size_t cols_count = json_array_size(json_cols);

    grid_props->row_dsc = calloc((rows_count + 1), sizeof(lv_coord_t)); // +1 for LV_GRID_TEMPLATE_LAST
    grid_props->col_dsc = calloc((cols_count + 1), sizeof(lv_coord_t)); // +1 for LV_GRID_TEMPLATE_LAST

    for (size_t i = 0; i < rows_count; i++)
    {
        grid_props->row_dsc[i] = (lv_coord_t)json_integer_value(json_array_get(json_rows, i));
    }
    grid_props->row_dsc[rows_count] = LV_GRID_TEMPLATE_LAST;

    for (size_t i = 0; i < cols_count; i++)
    {
        grid_props->col_dsc[i] = (lv_coord_t)json_integer_value(json_array_get(json_cols, i));
    }
    grid_props->col_dsc[cols_count] = LV_GRID_TEMPLATE_LAST;

    // Parse alignment properties
    const char *align_str = json_string_value(json_object_get(json_grid_props, "cell_align"));
    grid_props->column_align = get_grid_align(align_str);
    grid_props->row_align = get_grid_align(align_str);

    // Handle justify_content if needed
    const char *justify_content_str = json_string_value(json_object_get(json_grid_props, "justify_content"));
    if (justify_content_str)
    {
        grid_props->justify_content = get_grid_align(justify_content_str);
    }
}

/* NOTE Absolute alignments
enum {
    LV_ALIGN_DEFAULT = 0,
    LV_ALIGN_TOP_LEFT,
    LV_ALIGN_TOP_MID,
    LV_ALIGN_TOP_RIGHT,
    LV_ALIGN_BOTTOM_LEFT,
    LV_ALIGN_BOTTOM_MID,
    LV_ALIGN_BOTTOM_RIGHT,
    LV_ALIGN_LEFT_MID,
    LV_ALIGN_RIGHT_MID,
    LV_ALIGN_CENTER,

    LV_ALIGN_OUT_TOP_LEFT,
    LV_ALIGN_OUT_TOP_MID,
    LV_ALIGN_OUT_TOP_RIGHT,
    LV_ALIGN_OUT_BOTTOM_LEFT,
    LV_ALIGN_OUT_BOTTOM_MID,
    LV_ALIGN_OUT_BOTTOM_RIGHT,
    LV_ALIGN_OUT_LEFT_TOP,
    LV_ALIGN_OUT_LEFT_MID,
    LV_ALIGN_OUT_LEFT_BOTTOM,
    LV_ALIGN_OUT_RIGHT_TOP,
    LV_ALIGN_OUT_RIGHT_MID,
    LV_ALIGN_OUT_RIGHT_BOTTOM,
};
typedef uint8_t lv_align_t;
*/
lv_align_t get_absolute_align(const char *align)
{
    if (strcmp(align, "top_left") == 0)
    {
        return LV_ALIGN_TOP_LEFT;
    }
    else if (strcmp(align, "top_mid") == 0)
    {
        return LV_ALIGN_TOP_MID;
    }
    else if (strcmp(align, "top_right") == 0)
    {
        return LV_ALIGN_TOP_RIGHT;
    }
    else if (strcmp(align, "bottom_left") == 0)
    {
        return LV_ALIGN_BOTTOM_LEFT;
    }
    else if (strcmp(align, "bottom_mid") == 0)
    {
        return LV_ALIGN_BOTTOM_MID;
    }
    else if (strcmp(align, "bottom_right") == 0)
    {
        return LV_ALIGN_BOTTOM_RIGHT;
    }
    else if (strcmp(align, "left_mid") == 0)
    {
        return LV_ALIGN_LEFT_MID;
    }
    else if (strcmp(align, "right_mid") == 0)
    {
        return LV_ALIGN_RIGHT_MID;
    }
    else if (strcmp(align, "center") == 0)
    {
        return LV_ALIGN_CENTER;
    }
    else if (strcmp(align, "out_top_left") == 0)
    {
        return LV_ALIGN_OUT_TOP_LEFT;
    }
    else if (strcmp(align, "out_top_mid") == 0)
    {
        return LV_ALIGN_OUT_TOP_MID;
    }
    else if (strcmp(align, "out_top_right") == 0)
    {
        return LV_ALIGN_OUT_TOP_RIGHT;
    }
    else if (strcmp(align, "out_bottom_left") == 0)
    {
        return LV_ALIGN_OUT_BOTTOM_LEFT;
    }
    else if (strcmp(align, "out_bottom_mid") == 0)
    {
        return LV_ALIGN_OUT_BOTTOM_MID;
    }
    else if (strcmp(align, "out_bottom_right") == 0)
    {
        return LV_ALIGN_OUT_BOTTOM_RIGHT;
    }
    else if (strcmp(align, "out_left_top") == 0)
    {
        return LV_ALIGN_OUT_LEFT_TOP;
    }
    else if (strcmp(align, "out_left_mid") == 0)
    {
        return LV_ALIGN_OUT_LEFT_MID;
    }
    else if (strcmp(align, "out_left_bottom") == 0)
    {
        return LV_ALIGN_OUT_LEFT_BOTTOM;
    }
    else if (strcmp(align, "out_right_top") == 0)
    {
        return LV_ALIGN_OUT_RIGHT_TOP;
    }
    else if (strcmp(align, "out_right_mid") == 0)
    {
        return LV_ALIGN_OUT_RIGHT_MID;
    }
    else if (strcmp(align, "out_right_bottom") == 0)
    {
        return LV_ALIGN_OUT_RIGHT_BOTTOM;
    }
    return LV_ALIGN_DEFAULT;
}

void parse_absolute_layout(AbsoluteLayoutProps *abs_props, json_t *json_abs_props)
{
    if (!abs_props || !json_abs_props)
        return;

    // Parse x and y coordinates
    abs_props->x = (lv_coord_t)json_integer_value(json_object_get(json_abs_props, "x"));
    abs_props->y = (lv_coord_t)json_integer_value(json_object_get(json_abs_props, "y"));

    // Parse width and height
    abs_props->w = (lv_coord_t)json_integer_value(json_object_get(json_abs_props, "w"));
    abs_props->h = (lv_coord_t)json_integer_value(json_object_get(json_abs_props, "h"));

    // Parse alignment (if applicable)
    const char *align_str = json_string_value(json_object_get(json_abs_props, "align"));
    abs_props->align = get_absolute_align(align_str);
}

void parse_layout_type(Portion *portion, json_t *json_layout)
{
    if (!portion || !json_layout)
        return;

    const char *layout_type = json_string_value(json_object_get(json_layout, "type"));
    const char *layout_props = json_object_get(json_layout, "properties");
    if (strcmp(layout_type, "flex") == 0)
    {
        portion->layout_type = LAYOUT_FLEX;
        parse_flex_layout(&portion->layout_props.flex_props, layout_props);
    }
    else if (strcmp(layout_type, "grid") == 0)
    {
        portion->layout_type = LAYOUT_GRID;
        parse_grid_layout(&portion->layout_props.grid_props, layout_props);
    }
    else if (strcmp(layout_type, "absolute") == 0)
    {
        portion->layout_type = LAYOUT_ABSOLUTE;
        parse_absolute_layout(&portion->layout_props.absolute_props, layout_props);
    }
    else
    {
        fprintf(stderr, "Unknown layout type: %s", layout_type);
        exit(1);
    }
}

/* NOTE WIDGET TYPES
    WIDGET_ARC,
    WIDGET_BAR,
    WIDGET_BUTTON,
    WIDGET_BUTTONMATRIX,
    WIDGET_CANVAS,
    WIDGET_CHECKBOX,
    WIDGET_DROPDOWN,
    WIDGET_IMAGE,
    WIDGET_LABEL,
    WIDGET_LINE,
    WIDGET_ROLLER,
    WIDGET_SLIDER,
    WIDGET_SWITCH,
    WIDGET_TABLE,
    WIDGET_TEXTAREA
*/
void parse_widget_types(Portion *portion, json_t *json_widget_types)
{
    if (!json_widget_types || !json_is_array(json_widget_types))
    {
        return; // No widget types or not an array
    }

    size_t index;
    json_t *json_widget_type;
    size_t types = json_array_size(json_widget_types);
    portion->widget_types = calloc(types, sizeof(lv_widget_types_t));
    json_array_foreach(json_widget_types, index, json_widget_type)
    {
        const char *widget_type = json_string_value(json_widget_type);
        // Store or process the widget type string
        // For example, add it to a list in the Portion struct

        if (strcmp(widget_type, "arc") == 0)
        {
            portion->widget_types[index] = WIDGET_ARC;
        }
        else if (strcmp(widget_type, "bar") == 0)
        {
            portion->widget_types[index] = WIDGET_BAR;
        }
        else if (strcmp(widget_type, "button") == 0)
        {
            portion->widget_types[index] = WIDGET_BUTTON;
        }
        else if (strcmp(widget_type, "buttonmatrix") == 0)
        {
            portion->widget_types[index] = WIDGET_BUTTONMATRIX;
        }
        else if (strcmp(widget_type, "canvas") == 0)
        {
            portion->widget_types[index] = WIDGET_CANVAS;
        }
        else if (strcmp(widget_type, "checkbox") == 0)
        {
            portion->widget_types[index] = WIDGET_CHECKBOX;
        }
        else if (strcmp(widget_type, "dropdown") == 0)
        {
            portion->widget_types[index] = WIDGET_DROPDOWN;
        }
        else if (strcmp(widget_type, "image") == 0)
        {
            portion->widget_types[index] = WIDGET_IMAGE;
        }
        else if (strcmp(widget_type, "label") == 0)
        {
            portion->widget_types[index] = WIDGET_LABEL;
        }
        else if (strcmp(widget_type, "line") == 0)
        {
            portion->widget_types[index] = WIDGET_LINE;
        }
        else if (strcmp(widget_type, "roller") == 0)
        {
            portion->widget_types[index] = WIDGET_ROLLER;
        }
        else if (strcmp(widget_type, "slider") == 0)
        {
            portion->widget_types[index] = WIDGET_SLIDER;
        }
        else if (strcmp(widget_type, "switch") == 0)
        {
            portion->widget_types[index] = WIDGET_SWITCH;
        }
        else if (strcmp(widget_type, "table") == 0)
        {
            portion->widget_types[index] = WIDGET_TABLE;
        }
        else if (strcmp(widget_type, "textarea") == 0)
        {
            portion->widget_types[index] = WIDGET_TEXTAREA;
        }
        else
        {
            // default to button if unknown
            fprintf(stderr, "[%u] Unknown widget type: %s => Defaulting to BUTTON.", index, widget_type);
            portion->widget_types[index] = WIDGET_BUTTON;
        }
    }
}

// Function to parse a portion
Portion *parse_portion(json_t *json_portion)
{
    if (!json_portion)
        return NULL;

    Portion *portion = calloc(1, sizeof(Portion));
    if (!portion)
    {
        return NULL; // TODO Handle allocation failure
    }

    // Parse ID
    portion->id = strdup(json_string_value(json_object_get(json_portion, "id")));

    // Parse layout type
    json_t *json_layout = json_object_get(json_portion, "layout");
    parse_layout_type(portion, json_layout);

    // Parse style
    json_t *json_style = json_object_get(json_portion, "style");
    if (json_style)
    {
        portion->style = parse_style(json_style);
    }

    // Parse widget types
    json_t *json_widget_types = json_object_get(json_portion, "widgets");
    parse_widget_types(portion, json_widget_types);

    portion->next = NULL; // Initialize the next pointer
    return portion;
}

#pragma endregion PORTION

#pragma region DESIGN
// The main parsing function
Design *parse_design(const char *file_path)
{
    json_error_t error;
    json_t *root = json_load_file(file_path, 0, &error);
    if (!root)
    {
        // TODO Handle JSON parsing error
        return NULL;
    }

    Design *design = calloc(1, sizeof(Design));
    if (!design)
    {
        json_decref(root);
        return NULL;
    }

    // Parse root style
    json_t *json_root_style = json_object_get(root, "rootStyle");
    if (json_root_style)
    {
        design->root = parse_style(json_root_style);
    }

    // Initialize the first portion
    Portion *head = NULL;
    Portion *tail = NULL;

    json_t *json_portions = json_object_get(root, "portions");
    size_t index;
    json_t *json_portion;

    json_array_foreach(json_portions, index, json_portion)
    {
        Portion *current_portion = parse_portion(json_portion);

        if (!head)
        {
            head = current_portion;
        }
        else
        {
            tail->next = current_portion;
        }
        tail = current_portion;
    }

    design->first_portion = head;
    json_decref(root);

    return design;
}
#pragma endregion DESIGN

#pragma region CLEANUP
// Free function for Style struct
void free_style(Style *style)
{
    if (!style)
        return;

    lv_style_reset(style->lv_style);
    free(style->lv_style);
}

// Free function for Portion struct
void free_portion(Portion *portion)
{
    if (!portion)
        return;

    free(portion->id);
    free(portion->widget_types);
    free_style(portion->style);

    // Free additional properties based on layout
    if (portion->layout_type == LAYOUT_GRID)
    {
        free(portion->layout_props.grid_props.col_dsc);
        free(portion->layout_props.grid_props.row_dsc);
    }

    // Free the next portion in the list
    if (portion->next)
    {
        free_portion(portion->next);
    }

    free(portion);
}

// Free function for Design struct
void free_design(Design *design)
{
    if (!design)
        return;

    free_style(design->root);
    if (design->first_portion)
    {
        free_portion(design->first_portion);
    }

    free(design);
}
#pragma endregion CLEANUP
