#pragma once

#include <inttypes.h>

struct pixel {
    uint8_t b, g, r;
};

struct image {
    uint64_t width, height;
    struct pixel *data;
};

struct image image_create(uint64_t width, uint64_t height);

struct image image_copy(const struct image *image);

void image_destroy(struct image *image);
