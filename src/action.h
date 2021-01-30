#pragma once

#include "image.h"
#include "string_struct.h"

#define FOREACH_ACTION(ACTION) \
        ACTION(ROTATE)         \
        ACTION(NEGATIVE)       \

#define GENERATE_ENUM(ENUM) ENUM,

enum action {
    FOREACH_ACTION(GENERATE_ENUM)
};


struct image do_action(struct image *image, enum action action, const struct string *arg);