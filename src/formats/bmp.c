#include "bmp.h"

#include <string.h>


enum read_status check_header(struct bmp_header *bmp_header) {
    if (bmp_header->bfType[0] != 'B' || bmp_header->bfType[1] != 'M') {
        return READ_INVALID_SIGNATURE;
    }

    if ((bmp_header->biBitCount != 24) || (bmp_header->biCompression != 0)) {
        return READ_INVALID_HEADER;
    }

    return READ_OK;
}

enum read_status from_bmp(FILE *file, struct image *image) {
    struct bmp_header bmp_header = {0};
    if (fread(&bmp_header, sizeof(struct bmp_header), 1, file) < 1) {
        return READ_ERROR;
    }

    enum read_status header_status= check_header(& bmp_header);
    if (header_status!=READ_OK) return header_status;

    *image =image_create(bmp_header.biWidth, bmp_header.biHeight);

    const int64_t row_padding = bmp_header.biWidth% 4;

    const int64_t padded_row_size = bmp_header.biWidth * 3 + row_padding;

    const uint64_t width = bmp_header.biWidth;
    const uint64_t height = bmp_header.biHeight;

    for (uint64_t i = 0; i < height; i++) {
        fseek(file, bmp_header.bOffBits + (i * padded_row_size), SEEK_SET);
        size_t res_read = fread(image->data + (height - 1 - i) * width, sizeof(struct pixel), width, file);
        if (res_read < 1) {
            return READ_ERROR;
        }
    }

    return READ_OK;
}



struct bmp_header get_bmp_header (const struct image* image, const uint64_t row_padding) {
    uint32_t bSizeImage =image->height*(image->width* sizeof(struct pixel)+row_padding);
    return (struct bmp_header) {
        .bfType="BM",
        .bfileSize=bSizeImage + sizeof(struct bmp_header),
        .bfReserved=0,
        .bOffBits = sizeof(struct bmp_header),
        .biSize = 40,
        .biWidth =image->width,
        .biHeight = image->height,
        .biPlanes = 1,
        .biBitCount =24,
        .biCompression = 0,
        .biSizeImage=bSizeImage,
        .biXPelsPerMeter = 3780,
        .biYPelsPerMeter =3780,
        .biClrUsed = 0,
        .biClrImportant = 0,
    };
}

enum write_status to_bmp(FILE *file, const struct image *image) {
    static uint8_t zero = 0;

    const uint64_t row_padding = image->width % 4;

    struct bmp_header bmp_header= get_bmp_header(image, row_padding);

    if (fwrite(&bmp_header, sizeof(struct bmp_header), 1, file) < 1) {
        return WRITE_ERROR;
    }

    const uint64_t width = image->width;
    const uint64_t height = image->height;

    for (int64_t i = height - 1; i >= 0; --i) {
        if (fwrite(image->data + i * width, sizeof(struct pixel), width, file) < width) {
            return WRITE_ERROR;
        }
        if (fwrite(&zero,  row_padding, 1, file) != 1) {
            return WRITE_ERROR;
        }
    }

    return WRITE_OK;
}
