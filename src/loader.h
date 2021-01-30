#pragma once

#include <stdio.h>
#include <stdbool.h>

#include "image.h"
#include "string_struct.h"

#define FOREACH_FORMAT(FORMAT) \
        FORMAT(BMP)            \

#define GENERATE_ENUM(ENUM) ENUM,
enum format {
    FOREACH_FORMAT(GENERATE_ENUM)
};



bool load_image(const struct string *filename, struct image* image);


bool write_image(const struct string *filename, const struct image* image);