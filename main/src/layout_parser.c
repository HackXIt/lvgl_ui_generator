#include "layout_parser.h"
#include <jansson.h>
#include <stdlib.h>
#include <string.h>

// Helper function to parse style
Style parse_style(json_t *json_style)
{
    Style style;
    // Extract style properties from json_style
    // Example: style.backgroundColor = strdup(json_string_value(json_object_get(json_style, "backgroundColor")));
    // Repeat for other properties
    return style;
}

// Helper function to free style
void free_style(Style *style)
{
    // Free each string in style
    // Example: free(style->backgroundColor);
    // Repeat for other properties
}

Layout *parse_layout(const char *file_path)
{
    json_t *root;
    json_error_t error;

    root = json_load_file(file_path, 0, &error);
    if (!root)
    {
        // Handle error
        return NULL;
    }

    Layout *layout = malloc(sizeof(Layout));
    if (!layout)
    {
        // Handle memory allocation failure
        json_decref(root);
        return NULL;
    }

    json_t *json_rootStyle = json_object_get(root, "rootStyle");
    layout->rootStyle = parse_style(json_rootStyle);

    json_t *json_portions = json_object_get(root, "portions");
    size_t index;
    json_t *json_portion;
    layout->portion_count = json_array_size(json_portions);
    layout->portions = malloc(layout->portion_count * sizeof(Portion));

    json_array_foreach(json_portions, index, json_portion)
    {
        json_t *json_id = json_object_get(json_portion, "id");
        layout->portions[index].id = strdup(json_string_value(json_id));

        json_t *json_style = json_object_get(json_portion, "style");
        layout->portions[index].style = parse_style(json_style);

        // ... Parse widgets array similarly
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
        // Free widgets array
    }

    free(layout->portions);
    free(layout);
}
