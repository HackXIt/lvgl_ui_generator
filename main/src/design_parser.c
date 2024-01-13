#include "design_parser.h"
// ... other includes ...

// Function to parse a portion
Portion *parse_portion(json_t *json_portion)
{
    // Allocate a new Portion and parse its contents from json_portion
    // Call other parsing functions for specialized keys like layout and style
}

// Function to parse layout type
void parse_layout_type(Portion *portion, json_t *json_layout)
{
    // Parse the layout type and set the portion's layout_type accordingly
}

// https://stackoverflow.com/a/69812981
// Jenkins OAAT (one at a time) hash function
// NOTE it's not great, but does the job for my purpose I think and is easy to add here
uint32_t hash(const char *str)
{
    uint32_t hash, i;
    for (hash = i = 0; str[i] != '\0'; ++i)
    {
        hash += str[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

unsigned int style_property_hashes[STYLE_PROP_MAX] = {
    hash("inherit"),
    hash("width"),
    hash("height"),
    hash("bg_color"),
    // ... other hashed property names ...
};

StylePropertyIndex get_style_property_index(const char *prop_name)
{
    unsigned int hash_value = hash(prop_name);
    for (int i = 0; i < STYLE_PROP_MAX; i++)
    {
        if (style_property_hashes[i] == hash_value)
        {
            return (StylePropertyIndex)i;
        }
    }
    return -1; // Invalid index
}

// Function to parse style
void parse_style(Style *style, json_t *json_style)
{
    if (!json_style || !style)
    {
        return; // Handle null pointers
    }

    // Initialize lv_style_t object
    lv_style_t *lv_style = malloc(sizeof(lv_style_t));
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
        default:
            // Handle unknown property
            break;
        }
    }

    style->lv_style = lv_style;
}

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

    Design *design = malloc(sizeof(Design));
    if (!design)
    {
        json_decref(root);
        return NULL;
    }
    memset(design, 0, sizeof(Design));

    // Parse root style
    json_t *json_root_style = json_object_get(root, "rootStyle");
    if (json_root_style)
    {
        init_style(&design->root, json_root_style);
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

// Free function for Style struct
void free_style(Style *style)
{
    if (style)
    {
        if (style->lv_style)
        {
            lv_style_reset(style->lv_style);
            free(style->lv_style);
        }
        free(style);
    }
}

// Free function for Portion struct
void free_portion(Portion *portion)
{
    if (portion)
    {
        free_style(portion->style);
    }
}

// Free function for Design struct
void free_design(Design *design)
{
    if (design)
    {
        free_style(&design->root);

        Portion *current_portion = design->first_portion;
        while (current_portion)
        {
            Portion *next_portion = current_portion->next;
            free_portion(current_portion);
            current_portion = next_portion;
        }

        free(design);
    }
}
