#include "negative.h"
#include "../util.h"

struct image negative(struct image *image, const struct string *arg) {
    if (arg->size != 0) {
        err("this command has no arguments\n");
        return (struct image){0,0,NULL};
    }
    struct image negative_image=image_create(image->width,image->width);
    for (uint64_t i = 0; i < image->height; i++) {
        for (uint64_t j = 0; j < image->width; j++) {
            uint64_t index = j + i * image->width;
            negative_image.data[index].b = 255 - image->data[index].b;
            negative_image.data[index].g = 255 - image->data[index].g;
            negative_image.data[index].r = 255 - image->data[index].r;
        }
    }
    return negative_image;
}