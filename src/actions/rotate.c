#include "rotate.h"
#include "../util.h"

#include <stdlib.h>
#include <math.h>

#define M_PI		3.14159265358979323846

struct image rotate(struct image *image, const struct string *arg) {
    int64_t angle_degree = strtol(arg->string, NULL, 10);
    if (!((angle_degree == 90) || (angle_degree == 180) || (angle_degree == 270))) {
        err("wrong argument\n");
        return (struct image){0,0,NULL};
    }
    const double angle = (double) angle_degree * M_PI / 180;
    const double cos_angle = cos(angle);
    const double sin_angle = sin(angle);
    const uint64_t width = fabs(image->width * cos_angle) + fabs(image->height * sin_angle);
    const uint64_t height = fabs(image->width * sin_angle) + fabs(image->height * cos_angle);
    struct image rotated_image = image_create(width, height);
    const int64_t length = width * height;
    for (uint64_t i = 0; i < image->height; i++) {
        for (uint64_t j = 0; j < image->width; j++) {
            const uint64_t index = j + i * image->width;
            int64_t new_index = image->height * (index + 1) - 1;
            if (new_index > length - 1) {
                new_index = new_index % (length) - new_index / (length);
            }
            const int64_t a = index * cos_angle;
            const int64_t b = new_index * sin_angle;
            new_index = a + b;
            if (cos_angle < 0 || sin_angle < 0 || new_index < 0) new_index = length - 1 - labs(new_index);
            rotated_image.data[new_index] = image->data[index];
        }
    }
    return rotated_image;
}
