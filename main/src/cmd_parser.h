#pragma once

#include <stdbool.h>
#include <stdint.h>

#define MAX_WIDGET_TYPES 10 // Adjust as needed

typedef enum
{
    MODE_UNSPECIFIED,
    MODE_RANDOMIZER,
    MODE_DESIGN_FILE,
    MODE_TEST_PARSER
} OperationMode;

typedef struct
{
    int width;
    int height;
    int widget_count;
    char *layout;
    char *widget_types_str;
    char *output_file;
    uint8_t delay_count;
    char *widget_types[MAX_WIDGET_TYPES];
    int type_count;
} RandomizerArgs;

typedef struct
{
    char *design_file;
} DesignFileArgs;

typedef struct
{
    OperationMode mode;
    RandomizerArgs randomizer_args;
    DesignFileArgs design_file_args;
} CmdArgs;

void print_help(const char *program_name);
bool parse_cmd_args(int argc, char *argv[], CmdArgs *args);
