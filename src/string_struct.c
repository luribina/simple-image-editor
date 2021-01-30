#include "string_struct.h"

#include <malloc.h>
#include <string.h>
#include <ctype.h>

struct string *string_create_empty(size_t size) {
    struct string *string = malloc(sizeof(struct string));
    char *char_array = malloc((size + 1) * sizeof(char));
    char_array[size] = '\0';
    string->string = char_array;
    string->size = size;
    return string;
}

void string_destroy(struct string *string) {
    free(string->string);
    free(string);
}

void string_to_uppercase(const struct string *string) {
    char *char_array = string->string;
    for (size_t i = 0; i < string->size; i++) {
        char_array[i] = (char) toupper(char_array[i]);
    }

}

struct string *string_copy_from_char(const char *src, size_t size) {
    struct string *string = string_create_empty(size);
    strncpy(string->string, src, size);
    return string;
}




