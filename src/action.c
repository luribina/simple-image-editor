#include "action.h"
#include "actions/rotate.h"
#include "actions/negative.h"
#include "util.h"

typedef struct image action(struct image *image, const struct string *arg);

static action * const action_handler[] = {
        [ROTATE]=rotate,
        [NEGATIVE]=negative
};

struct image do_action(struct image *image, enum action action, const struct string *arg) {
    return action_handler[action](image, arg);
}
