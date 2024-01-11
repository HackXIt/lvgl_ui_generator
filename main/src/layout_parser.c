#include "layout_parser.h"
#include <jansson.h>
#include <stdlib.h>
#include <string.h>

// Helper function to parse style
Style parse_style(json_t *json_style)
{
    Style style;
    memset(&style, 0, sizeof(Style)); // Initialize style structure

    // Extract and duplicate style properties from json_style
    style.backgroundColor = strdup(json_string_value(json_object_get(json_style, "backgroundColor")));
    style.textColor = strdup(json_string_value(json_object_get(json_style, "textColor")));
    style.height = strdup(json_string_value(json_object_get(json_style, "height")));

    return style;
}

/// Helper function to free style
void free_style(Style *style)
{
    if (style)
    {
        free(style->backgroundColor);
        free(style->textColor);
        free(style->height);
    }
}

Layout *parse_layout(const char *file_path)
{
    json_t *root;
    json_error_t error;

    root = json_load_file(file_path, 0, &error);
    if (!root)
    {
        // Handle JSON parsing error
        return NULL;
    }

    Layout *layout = malloc(sizeof(Layout));
    if (!layout)
    {
        json_decref(root);
        return NULL; // Memory allocation failure
    }

    // Parse root style
    json_t *json_rootStyle = json_object_get(root, "rootStyle");
    layout->rootStyle = parse_style(json_rootStyle);

    // Parse portions
    json_t *json_portions = json_object_get(root, "portions");
    layout->portion_count = json_array_size(json_portions);
    layout->portions = malloc(layout->portion_count * sizeof(Portion));

    for (size_t i = 0; i < layout->portion_count; i++)
    {
        json_t *json_portion = json_array_get(json_portions, i);

        // Parse portion id
        layout->portions[i].id = strdup(json_string_value(json_object_get(json_portion, "id")));

        // Parse portion style
        layout->portions[i].style = parse_style(json_object_get(json_portion, "style"));

        // Parse widgets
        json_t *json_widgets = json_object_get(json_portion, "widgets");
        layout->portions[i].widget_count = json_array_size(json_widgets);
        layout->portions[i].widgets = malloc(layout->portions[i].widget_count * sizeof(char *));

        for (size_t j = 0; j < layout->portions[i].widget_count; j++)
        {
            json_t *json_widget = json_array_get(json_widgets, j);
            layout->portions[i].widgets[j] = strdup(json_string_value(json_widget));
        }
    }

    json_decref(root);
    return layout;
}

void free_layout(Layout *layout)
{
    if (!layout)
        return;

    free_style(&layout->rootStyle);

    for (size_t i = 0; i < layout->portion_count; i++)
    {
        free(layout->portions[i].id);
        free_style(&layout->portions[i].style);

        for (size_t j = 0; j < layout->portions[i].widget_count; j++)
        {
            free(layout->portions[i].widgets[j]);
        }

        free(layout->portions[i].widgets);
    }

    free(layout->portions);
    free(layout);
}

// Helper function to create a widget in LVGL based on a string type
lv_obj_t *create_widget(const char *type)
{
    // Implement logic to create LVGL widget based on type string
    // Example:
    // if (strcmp(type, "button") == 0) return lv_btn_create(lv_scr_act(), NULL);
    // ... handle other widget types
    return NULL; // Default case if type is not recognized
}

// Helper function to apply style to a widget
void apply_style_to_widget(lv_obj_t *widget, const Style *style)
{
    // Implement logic to apply style to the widget
    // LVGL specific functions will be used here
    // Example:
    // lv_obj_set_style_local_bg_color(widget, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hexstr(style->backgroundColor));
    // ... apply other style properties
}

void create_lvgl_widgets_from_layout(const Layout *layout)
{
    // Iterate through the portions in the layout
    for (size_t i = 0; i < layout->portion_count; i++)
    {
        Portion portion = layout->portions[i];

        // Create and style widgets for this portion
        for (size_t j = 0; j < portion.widget_count; j++)
        {
            lv_obj_t *widget = create_widget(portion.widgets[j]);
            if (widget)
            {
                apply_style_to_widget(widget, &portion.style);
                // Position and size the widget as per layout portion
                // lv_obj_set_pos(widget, x, y); // Set position
                // lv_obj_set_size(widget, width, height); // Set size
            }
        }
    }

    // Apply the root style to the overall screen or container if needed
    // ... additional logic for root style
}
