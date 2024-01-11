#ifndef LAYOUT_PARSER_H
#define LAYOUT_PARSER_H

#include <jansson.h>
#include <stddef.h> // For size_t

typedef struct
{
    char *backgroundColor;
    char *textColor;
    char *height; // Additional style properties can be added as needed
} Style;

typedef struct
{
    char *id;
    Style style;
    char **widgets; // Array of strings representing widget types
    size_t widget_count;
} Portion;

typedef struct
{
    Style rootStyle;
    Portion *portions;
    size_t portion_count;
} Layout;

// Parses the layout file and returns a pointer to a Layout structure
Layout *parse_layout(const char *file_path);

// Function to free the allocated Layout structure
void free_layout(Layout *layout);

// Additional helper functions
Style parse_style(json_t *json_style);
void free_style(Style *style);

#endif // LAYOUT_PARSER_H
