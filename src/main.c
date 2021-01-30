#include "string_struct.h"
#include "loader.h"
#include "action.h"
#include "image.h"

#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define PROG_NAME "image_editor"

#define GENERATE_VAL(STRING) {STRING, #STRING},


void usage(uint32_t status, const char *error_message) {

    FILE *out = status == EXIT_SUCCESS ? stdout : stderr;
    if (error_message) {
        fprintf(out, "%s", error_message);
    }
    fprintf(out,
            "Usage: ./%s [OPTION] INPUT_FILE OUTPUT_FILE\n"
            "currently supported format: BMP\n\n",
            PROG_NAME);
    if (out == stdout) {
        fprintf(out,
                "Options:\n"
                "-h, --help              print command usage\n"
                "-d, --delete            delete input file\n"
                "-a, --has_action <has_action [argument]>   perform a certain has_action with picture\n"
                "Actions:\n"
                "rotate:[90|180|270]\n"
                "negative:\n");
    } else {
        fprintf(out,
                "Use --help for more information\n");
    }
    exit(status);
}


static const  struct {
    enum format value;
    const char *str;
} formats_array[] = {
        FOREACH_FORMAT(GENERATE_VAL)
};


static const  struct {
    enum action value;
    const char *str;
} actions_array[] = {
        FOREACH_ACTION(GENERATE_VAL)
};


struct action_info {
    enum action action;
    enum format format;
};


struct arg_info {
    bool has_action;
    bool has_format;
    struct string *format_name;
    struct string *action_name;
    struct string *arg;
    struct string *input_name;
    struct string *output_name;
};

void arg_info_destroy(struct arg_info *arg_info) {
    string_destroy(arg_info->format_name);
    string_destroy(arg_info->action_name);
    string_destroy(arg_info->arg);
    string_destroy(arg_info->input_name);
    string_destroy(arg_info->output_name);
}


struct arg_info parse_args(int argc, char **argv) {
    static struct option const long_options[] = {
            {"help",        no_argument,       NULL, 'h'},
            {"format_name", required_argument, NULL, 'f'},
            {"has_action",  required_argument, NULL, 'a'},
            {NULL, 0,                          NULL, 0}
    };
    struct arg_info arg_info;
    int32_t c;
    while ((c = getopt_long(argc, argv, "hda:f:", long_options, NULL)) != -1) {
        switch (c) {
            case 'h':
                usage(EXIT_SUCCESS, NULL);
                break;
            case 'f':
                arg_info.has_format = true;
                arg_info.format_name = string_copy_from_char(optarg, strlen(optarg));
                string_to_uppercase(arg_info.format_name);
                break;
            case 'a':
                arg_info.has_action = true;
                char *delimiter = strchr(optarg, ':');
                if (delimiter) {
                    uint64_t position = delimiter - optarg;
                    arg_info.action_name = string_copy_from_char(optarg, position);
                    string_to_uppercase(arg_info.action_name);
                    arg_info.arg = string_copy_from_char(optarg + position + 1, strlen(optarg) - position - 1);
                } else {
                    usage(EXIT_FAILURE, "wrong action format\n");
                }
                break;
            default:
                usage(EXIT_FAILURE, NULL);
        }
    }

    if (!arg_info.has_action || !arg_info.has_format) {
        usage(EXIT_FAILURE, "not enough required options\n");
    }

    if (optind >= argc) {
        usage(EXIT_FAILURE, "expected arguments after options\n");
    }

    if (optind + 1 == argc - 1) {
        arg_info.input_name = string_copy_from_char(argv[optind], strlen(argv[optind]));
        arg_info.output_name = string_copy_from_char(argv[optind + 1], strlen(argv[optind + 1]));
    } else {
        usage(EXIT_FAILURE, "wrong number of arguments\n");
    }

    return arg_info;
}

struct action_info analyze_args(struct arg_info *arg_info) {
    enum action action;
    bool action_found = false;
    enum format format;
    bool format_found = false;

    for (uint32_t i = 0; i < sizeof(actions_array) / sizeof(actions_array[0]); i++) {
        if (!strcmp(actions_array[i].str, arg_info->action_name->string)) {
            action = actions_array[i].value;
            action_found = true;
        }
    }

    if (!action_found) {
        usage(EXIT_FAILURE, "no such action\n");
    }

    for (uint32_t i = 0; i < sizeof(formats_array) / sizeof(formats_array[0]); i++) {
        if (!strcmp(formats_array[i].str, arg_info->format_name->string)) {
            format = formats_array[i].value;
            format_found = true;
        }
    }

    if (!format_found) {
        usage(EXIT_FAILURE, "no such action\n");
    }

    return (struct action_info) {action, format};
}

int main(int argc, char **argv) {

    struct arg_info arg_info = parse_args(argc, argv);
    struct action_info action_info = analyze_args(&arg_info);
    struct image image;

    if(!load_image(arg_info.input_name, &image)) {
        arg_info_destroy(&arg_info);
        return 1;
    }

    struct image image_copy = do_action(&image, action_info.action, arg_info.arg);

    image_destroy(&image);

    if (!image_copy.data) {
        image_destroy(&image_copy);
        arg_info_destroy(&arg_info);
        return 1;
    }

    if (!write_image(arg_info.output_name, &image_copy)) {
        image_destroy(&image_copy);
        arg_info_destroy(&arg_info);
        return 1;
    }

    image_destroy(&image_copy);
    arg_info_destroy(&arg_info);

    return 0;
}
