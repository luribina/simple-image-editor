#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "../image.h"

enum read_status {
    READ_OK,
    READ_ERROR,
    READ_INVALID_SIGNATURE,
    READ_INVALID_HEADER,
};

enum write_status {
    WRITE_OK,
    WRITE_ERROR,
};


#define FOR_BMP_HEADER(FOR_FIELD) \
        FOR_FIELD( char,bfType[2])\
        FOR_FIELD( uint32_t,bfileSize)\
        FOR_FIELD( uint32_t,bfReserved)\
        FOR_FIELD( uint32_t,bOffBits)\
        FOR_FIELD( uint32_t,biSize)\
        FOR_FIELD( uint32_t,biWidth)\
        FOR_FIELD( uint32_t,biHeight)\
        FOR_FIELD( uint16_t,biPlanes)\
        FOR_FIELD( uint16_t,biBitCount)\
        FOR_FIELD( uint32_t,biCompression)\
        FOR_FIELD( uint32_t,biSizeImage)\
        FOR_FIELD( uint32_t,biXPelsPerMeter)\
        FOR_FIELD( uint32_t,biYPelsPerMeter)\
        FOR_FIELD( uint32_t,biClrUsed)\
        FOR_FIELD( uint32_t,biClrImportant)

#define DECLARE_FIELD(t, n) t n ;

struct __attribute__((packed)) bmp_header {
    FOR_BMP_HEADER(DECLARE_FIELD)
};


enum read_status from_bmp (FILE *file, struct image *image);

enum write_status to_bmp(FILE *file, const struct image *image);
