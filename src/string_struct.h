#pragma once

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <malloc.h>

struct string {
    char *string;
    size_t size;
};

struct string *string_create_empty(size_t size);


void string_destroy(struct string *string);

void string_to_uppercase(const struct string *string);

struct string *string_copy_from_char(const char *src, size_t size);