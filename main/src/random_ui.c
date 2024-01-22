#include "random_ui.h"

void seed_random(void)
{
    struct timeval time;
    gettimeofday(&time, NULL);
    srand((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void create_random_layout_flex(random_ui_t *random_ui)
{
    // Flex layout with random flow
    int flex_flow_choice = rand() % MAX_FLEX_FLOW_OPTIONS;
    switch (flex_flow_choice)
    {
    case 0:
        lv_obj_set_flex_flow(random_ui->container, LV_FLEX_FLOW_ROW);
        break;
    case 1:
        lv_obj_set_flex_flow(random_ui->container, LV_FLEX_FLOW_COLUMN);
        break;
    case 2:
        lv_obj_set_flex_flow(random_ui->container, LV_FLEX_FLOW_ROW_WRAP);
        break;
    case 3:
        lv_obj_set_flex_flow(random_ui->container, LV_FLEX_FLOW_COLUMN_WRAP);
        break;
    case 4:
        lv_obj_set_flex_flow(random_ui->container, LV_FLEX_FLOW_ROW_REVERSE);
        break;
    case 5:
        lv_obj_set_flex_flow(random_ui->container, LV_FLEX_FLOW_COLUMN_REVERSE);
        break;
    case 6:
        lv_obj_set_flex_flow(random_ui->container, LV_FLEX_FLOW_ROW_WRAP_REVERSE);
        break;
    case 7:
        lv_obj_set_flex_flow(random_ui->container, LV_FLEX_FLOW_COLUMN_WRAP_REVERSE);
        break;
    }
    for (int i = 0; i < random_ui->widget_count; ++i)
    {
        random_ui->elements[i].widget = create_random_widget(random_ui->container, random_ui->widget_types, random_ui->type_count, &(random_ui->elements[i].type));
        get_element_spatial_info(random_ui, &(random_ui->elements[i]));
    }
}

void shuffleCells(Cell *cells, int count)
{
    for (int i = 0; i < count; i++)
    {
        int j = rand() % (i + 1);
        Cell temp = cells[i];
        cells[i] = cells[j];
        cells[j] = temp;
    }
}

void create_random_layout_grid(random_ui_t *random_ui)
{
    // Grid layout with random column and row count
    lv_obj_set_layout(random_ui->container, LV_LAYOUT_GRID);
    // Calculate the size of the 2D array for grid placements
    int mem_size = (int)ceil(sqrt(random_ui->widget_count));
    static lv_coord_t column_dsc[16] = {0}; /*2 columns with 100 and 400 ps width*/
    static lv_coord_t row_dsc[16] = {0};    /*3 100 px tall rows*/
    for (int i = 0; i < mem_size; ++i)
    {
        column_dsc[i] = LV_GRID_CONTENT; /*1/3 part of the grid*/
    }
    for (int i = 0; i < mem_size; ++i)
    {
        row_dsc[i] = LV_GRID_CONTENT; /*1/3 part of the grid*/
    }
    column_dsc[15] = LV_GRID_TEMPLATE_LAST;
    row_dsc[15] = LV_GRID_TEMPLATE_LAST;
    lv_obj_set_grid_dsc_array(random_ui->container, column_dsc, row_dsc);
    lv_grid_align_t align_x = rand() % MAX_GRID_ALIGN_OPTIONS; // There are 7 grid align options
    lv_grid_align_t align_y = rand() % MAX_GRID_ALIGN_OPTIONS; // There are 7 grid align options
    /* Align options by number
    0 - LV_GRID_ALIGN_START
    1 - LV_GRID_ALIGN_CENTER
    2 - LV_GRID_ALIGN_END
    3 - LV_GRID_ALIGN_STRETCH
    4 - LV_GRID_ALIGN_SPACE_AROUND
    5 - LV_GRID_ALIGN_SPACE_BETWEEN
    6 - LV_GRID_ALIGN_SPACE_EVENLY
    */
    int **grid_placements = calloc(mem_size, sizeof(int *)); // Temporary grid placement
    // Create and shuffle a list of all cell positions
    int total_cells = mem_size * mem_size;
    Cell *cells = malloc(total_cells * sizeof(Cell));
    for (int i = 0, k = 0; i < mem_size; i++)
    {
        for (int j = 0; j < mem_size; j++, k++)
        {
            cells[k].row = i;
            cells[k].col = j;
        }
    }
    shuffleCells(cells, total_cells);
    for (int i = 0; i < random_ui->widget_count; ++i)
    {
        // Create the widget
        random_ui->elements[i].widget = create_random_widget(random_ui->container, random_ui->widget_types, random_ui->type_count, &(random_ui->elements[i].type));
        lv_obj_set_grid_cell(random_ui->elements[i].widget, align_x, cells[i].row, 1, align_y, cells[i].col, 1);
        get_element_spatial_info(random_ui, &(random_ui->elements[i]));
    }
    free(grid_placements);
    free(cells);
}

void write_widget_type(char **widget_type, const char *type)
{
    *widget_type = calloc(strlen(type) + 1, sizeof(char));
    strcpy(*widget_type, type);
}
/*
lv_obj_t *create_styled_widget(const char *type, lv_obj_t *parent)
{
    // Logic to create a widget of the specified type
}
*/

lv_obj_t *create_random_widget(lv_obj_t *container, const char **widget_types, int type_count, char **widget_type)
{
    const char *type = widget_types[rand() % type_count];
    lv_obj_t *widget = NULL;

    if (strcmp(type, "button") == 0)
    {
        widget = lv_btn_create(container);
        lv_obj_set_size(widget, 100, 50);
        lv_obj_t *label = lv_label_create(widget);
        lv_label_set_text(label, "Button");
        write_widget_type(widget_type, "lv_btn");
    }
    else if (strcmp(type, "checkbox") == 0)
    {
        widget = lv_checkbox_create(container);
        write_widget_type(widget_type, "lv_checkbox");
    }
    else if (strcmp(type, "slider") == 0)
    {
        widget = lv_slider_create(container);
        lv_slider_set_range(widget, 0, 100);
        write_widget_type(widget_type, "lv_slider");
    }
    else if (strcmp(type, "label") == 0)
    {
        widget = lv_label_create(container);
        lv_label_set_text(widget, "Label");
        write_widget_type(widget_type, "lv_label");
    }
    else if (strcmp(type, "switch") == 0)
    {
        widget = lv_switch_create(container);
        write_widget_type(widget_type, "lv_switch");
    }
    else if (strcmp(type, "progressbar") == 0)
    {
        widget = lv_bar_create(container);
        lv_bar_set_range(widget, 0, 100);
        lv_bar_set_value(widget, rand() % 100, LV_ANIM_OFF);
        write_widget_type(widget_type, "lv_bar");
    }
    return widget;
}

void get_element_spatial_info(random_ui_t *random_ui, random_ui_element_t *element)
{
    // FIXME These coordinates are bullshit for my desired output
    lv_task_handler();
    lv_obj_update_layout(random_ui->container);
    lv_area_t container_coords;
    lv_obj_get_coords(random_ui->container, &container_coords);
    lv_obj_get_coords(element->widget, &(element->coords));
    element->width = lv_obj_get_width(element->widget);
    element->height = lv_obj_get_height(element->widget);
    element->rel_coords.x1 = element->coords.x1 - container_coords.x1; // left corner
    element->rel_coords.y1 = element->coords.y1 - container_coords.y1; // Top corner
    element->rel_coords.x2 = element->coords.x1 + element->width;      // right corner
    element->rel_coords.y2 = element->coords.y1 + element->height;     // bottom corner
    element->content_width = lv_obj_get_content_width(element->widget);
    element->content_height = lv_obj_get_content_height(element->widget);
    printf("Widget: %s\tcontainer(x1:%d,y1:%d,x2:%d,y2:%d) widget(x1:%d,y1:%d,x2:%d,y2:%d) size(w:%d,h:%d) widget_calc(x:%d,y:%d,w:%d,h:%d)\n",
           element->type,
           container_coords.x1,
           container_coords.y1,
           container_coords.x2,
           container_coords.y2,
           element->coords.x1,
           element->coords.y1,
           element->coords.x2,
           element->coords.y2,
           element->width,
           element->height,
           element->rel_coords.x1,
           element->rel_coords.y1,
           element->rel_coords.x2,
           element->rel_coords.y2);
}

random_ui_t *create_random_ui(int width, int height, const char **widget_types, int type_count, int widget_count, uint8_t delay_count, const char *layout)
{
    // FIXME Not checking for NULL return values from calloc
    // FIXME Not verifying numeric parameters (e.g. width > 0, height > 0, etc.)
    random_ui_t *random_ui = calloc(1, sizeof(random_ui_t));
    random_ui->width = width;
    random_ui->height = height;
    random_ui->widget_types = widget_types;
    random_ui->type_count = type_count;
    random_ui->widget_count = widget_count;
    random_ui->delay_count = delay_count;
    random_ui->layout = layout;
    random_ui->elements = calloc(widget_count, sizeof(random_ui_element_t));
    // Seed the random number generator with a more fine-grained time
    seed_random();
    random_ui->container = lv_obj_create(lv_scr_act());
    // FIXME tested to get rid of container, but doesn't make the spatial_info any better
    // random_ui->container = lv_scr_act();
    lv_obj_set_size(random_ui->container, width, height);
    lv_obj_set_scrollbar_mode(random_ui->container, LV_SCROLLBAR_MODE_OFF);
    // lv_obj_center(random_ui->container);
    lv_obj_set_pos(random_ui->container, 30, 30);

    // Randomly choose between Flex and Grid layouts
    if (strcmp(layout, "flex") == 0)
    {
        create_random_layout_flex(random_ui);
    }
    else if (strcmp(layout, "grid") == 0)
    {
        create_random_layout_grid(random_ui);
    }
    else if (strcmp(layout, "none") == 0)
    {
        for (int i = 0; i < random_ui->widget_count; ++i)
        {
            random_ui->elements[i].widget = create_random_widget(random_ui->container, random_ui->widget_types, random_ui->type_count, &(random_ui->elements[i].type));
            // Calculate the maximum x and y positions to ensure the widget fits within the container
            int max_x = width - random_ui->elements[i].width - 50;
            int max_y = height - random_ui->elements[i].height - 50;

            // Set the position of the widget randomly within the maximum x and y positions
            int x = rand() % max_x;
            int y = rand() % max_y;
            lv_obj_set_pos(random_ui->elements[i].widget, x, y);
            // Update the spatial information of the element
            get_element_spatial_info(random_ui, &(random_ui->elements[i]));
        }
    }
    else
    {
        printf("Invalid layout option: %s\n", layout);
        exit(1);
    }

    // Trigger a render to ensure the framebuffer is updated
    // lv_refr_now(NULL);
    lv_task_handler();

    for (uint8_t i = 0; i < delay_count; i++)
    {
        /* Periodically call the lv_task handler.
         * It could be done in a timer interrupt or an OS task too.*/
        lv_timer_handler();
        usleep(5 * 1000);
    }

    return random_ui;
}

void destroy_random_ui(random_ui_t *random_ui)
{
    for (int i = 0; i < random_ui->widget_count; ++i)
    {
        lv_obj_del(random_ui->elements[i].widget);
        char *type = random_ui->elements[i].type;
        free(type);
    }
    lv_obj_del(random_ui->container);
    free(random_ui->elements);
    free(random_ui);
}
/*
// New function to create widgets based on the layout
void create_widgets_according_to_layout(Layout *layout)
{
    // Iterate through the portions in the layout
    for (size_t i = 0; i < layout->portion_count; i++)
    {
        Portion portion = layout->portions[i];

        // Create widgets for this portion
        for (size_t j = 0; j < portion.widget_count; j++)
        {
            // Create widget based on the type defined in the layout
            lv_obj_t *widget = create_random_widget_based_on_type(portion.widgets[j]);
            if (widget)
            {
                apply_style_to_widget(widget, &portion.style);
                // Position the widget within the portion's area
            }
        }
    }
}
*/
