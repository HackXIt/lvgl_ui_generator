#include "cmd_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

bool parse_randomizer_args(int argc, char *argv[], RandomizerArgs *args);
bool parse_design_file_args(int argc, char *argv[], DesignFileArgs *args);
bool parse_test_parser_args(int argc, char *argv[], DesignFileArgs *args);

void print_help(const char *program_name)
{
    printf("Usage: %s [OPTIONS]\n", program_name);
    printf("Options:\n");
    printf("  -m <mode>                   Set the operation mode ('randomizer' or 'design')\n");
    printf("Randomizer Mode Options:\n");
    printf("  -w <width>                  Set the width of the UI\n");
    printf("  -h <height>                 Set the height of the UI\n");
    printf("  -c <widget_count>           Set the number of widgets to be randomized\n");
    printf("  -t <widget_types>           Comma-separated list of widget types to be used\n");
    printf("  -o <output_file>            Set the output file path\n");
    printf("  -d <delay_count>            Set the screenshot delay count\n");
    printf("  -l <layout>                 Set the layout type\n");
    printf("Design File Mode Options:\n");
    printf("  -f <design_file>            Specify the design file path\n");
    printf("\n");
    printf("Example:\n");
    printf("  %s -m randomizer -w 800 -h 600 -c 10 -t button,label,slider -l flex -o output.jpg -d 5\n", program_name);
    printf("  %s -m design -f design.json\n", program_name);
}

bool parse_cmd_args(int argc, char *argv[], CmdArgs *args)
{
    int opt;
    opterr = 0;
    // Determine the operation mode first
    while ((opt = getopt(argc, argv, "m:")) != -1)
    {
        switch (opt)
        {
        case 'm':
            if (strcmp(optarg, "randomizer") == 0)
            {
                args->mode = MODE_RANDOMIZER;
            }
            else if (strcmp(optarg, "design") == 0)
            {
                args->mode = MODE_DESIGN_FILE;
            }
            else if (strcmp(optarg, "test-parser") == 0)
            {
                args->mode = MODE_TEST_PARSER;
            }
            else
            {
                fprintf(stderr, "Invalid mode\n");
                fprintf(stderr, "Usage: %s -m [randomizer|design] [mode options]\n", argv[0]);
                return false;
            }
            break;
        case '?':
            print_help(argv[0]);
            break;
        default:
            break;
        }
        if (args->mode == MODE_RANDOMIZER || args->mode == MODE_DESIGN_FILE)
        {
            // mode is set, break the loop
            break;
        }
    }

    // Reset optind for the next parsing phase
    optind = 2;

    // Parse arguments based on the mode
    switch (args->mode)
    {
    case MODE_RANDOMIZER:
        return parse_randomizer_args(argc, argv, &args->randomizer_args);
    case MODE_DESIGN_FILE:
        return parse_design_file_args(argc, argv, &args->design_file_args);
    case MODE_TEST_PARSER:
        return parse_test_parser_args(argc, argv, &args->design_file_args);
    default:
        fprintf(stderr, "Mode not set\n");
        return false;
    }
}

bool parse_randomizer_args(int argc, char *argv[], RandomizerArgs *args)
{
    int opt;
    while ((opt = getopt(argc, argv, "w:h:c:t:o:d:l:")) != -1)
    {
        switch (opt)
        {
        case 'w':
            args->width = atoi(optarg);
            break;
        case 'h':
            args->height = atoi(optarg);
            break;
        case 'c':
            args->widget_count = atoi(optarg);
            break;
        case 't':
            args->widget_types_str = optarg;
            break;
        case 'o':
        {
            size_t len = strlen(optarg) + 2; // +2 for '/' prefix and '\0'
            args->output_file = (char *)malloc(len);
            if (args->output_file == NULL)
            {
                fprintf(stderr, "Failed to allocate memory for output file path\n");
                return false;
            }
            snprintf(args->output_file, len, "/%s", optarg);
            break;
        }
        case 'd':
            args->delay_count = atoi(optarg);
            break;
        case 'l':
            args->layout = optarg;
            break;
        default:
            fprintf(stderr, "Invalid option for randomizer mode\n");
            return false;
        }
    }

    if (args->width <= 0 || args->height <= 0 || args->widget_count <= 0 || args->widget_types_str == NULL)
    {
        fprintf(stderr, "Invalid arguments for randomizer mode\n");
        return false;
    }

    // Parse widget types
    args->type_count = 0;
    char *token = strtok(args->widget_types_str, ",");
    while (token != NULL && args->type_count < MAX_WIDGET_TYPES)
    {
        args->widget_types[args->type_count++] = token;
        token = strtok(NULL, ",");
    }

    return true;
}

bool parse_design_file_args(int argc, char *argv[], DesignFileArgs *args)
{
    int opt;
    while ((opt = getopt(argc, argv, "f:")) != -1)
    {
        switch (opt)
        {
        case 'f':
            args->design_file = optarg;
            break;
        default:
            fprintf(stderr, "Invalid option for design file mode\n");
            return false;
        }
    }

    if (args->design_file == NULL)
    {
        fprintf(stderr, "Design file argument is required for design file mode\n");
        return false;
    }

    return true;
}

bool parse_test_parser_args(int argc, char *argv[], DesignFileArgs *args)
{
    int opt;
    while ((opt = getopt(argc, argv, "f:")) != -1)
    {
        switch (opt)
        {
        case 'f':
            args->design_file = optarg;
            break;
        default:
            fprintf(stderr, "Invalid option for test parser mode\n");
            return false;
        }
    }

    if (args->design_file == NULL)
    {
        fprintf(stderr, "Design file argument is required for test parser mode\n");
        return false;
    }

    return true;
}
