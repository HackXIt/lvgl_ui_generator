#pragma once

#include "design_parser.h" // Include the design parser definitions
#include "lvgl/lvgl.h"
#include <stdlib.h>
#include <time.h>

// Function to create a randomized UI based on a design file
void create_randomized_design_ui(const char *design_file);
