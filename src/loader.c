#include "loader.h"
#include "formats/bmp.h"
#include "util.h"


static const char *read_message[] = {
        [READ_OK]=NULL,
        [READ_ERROR]="could not read bmp image",
        [READ_INVALID_SIGNATURE]="invalid bmp signature",
        [READ_INVALID_HEADER]="unsupported file options",
};

static const char *write_message[] = {
        [WRITE_OK] = NULL,
        [WRITE_ERROR] = "could not write to file"
};


enum open_status {
    OPEN_OK,
    OPEN_ERROR
};


enum open_status open_file(FILE **file, const struct string *filename, const char *mode) {
    *file = fopen(filename->string, mode);
    if (!*file) {
        return OPEN_ERROR;
    } else {
        return OPEN_OK;
    }
}

enum close_status {
    CLOSE_OK,
    CLOSE_ERROR
};


enum close_status close_file(FILE **file) {
    if (fclose(*file)) {
        return CLOSE_ERROR;
    } else {
        return CLOSE_OK;
    }
}


bool load_image(const struct string *filename, struct image* image) {
    FILE *file = NULL;

    if (open_file(&file, filename, "rb") == OPEN_ERROR) {
        err("can not open file","\n");
        return false;
    }

    enum read_status read_status = from_bmp(file, image);
    if (read_status!=READ_OK) {
        err(read_message[read_status],"\n");
        return false;
    }

    if (close_file(&file) == CLOSE_ERROR) {
        err("can not close first file","\n");
        return false;
    }
    return true;
}

bool write_image(const struct string *filename, const struct image* image) {

    FILE *file = NULL;
    if (open_file(&file, filename, "wb") == OPEN_ERROR) {
        err("can not save file","\n");
        return false;
    }

    enum write_status write_status = to_bmp(file, image);

    if (write_status!=WRITE_OK) {
        err(write_message[write_status],"\n");
        return false;
    }

    if (close_file(&file) == CLOSE_ERROR) {
        err("can not close second file","\n");
        return false;
    }

    return true;
}