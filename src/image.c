#include "image.h"

#include <malloc.h>
#include <string.h>

struct image image_create(uint64_t width, uint64_t height) {
    struct pixel *pixel = malloc(width * height * sizeof(struct pixel));
    return (struct image) {.data=pixel, .width=width, .height=height};
}

struct image image_copy(const struct image *image) {
    struct image image_copy = image_create(image->width, image->height);
    memcpy(image_copy.data, image->data, image->height * image->width * sizeof(struct pixel));
    return image_copy;
}

void image_destroy(struct image *image) {
    free(image->data);
}
