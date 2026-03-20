#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "modules.h"

struct Test {
    const char *name;
    const char *input;
    const char *output;
};

static int
compare(const char *a, const char *b)
{
    return strcmp(a, b) == 0;
}

void
test(void)
{
    struct Test tests[] = {
        {.name = "Case 1", .input = "a : b c\nb : c\nc :", .output = "c\nb\na"},
        {0},
    };

    for (struct Test *t = tests; t->name; t++) {
        printf("test %s... ", t->name);
        char *output = resolve_modules(t->input);
        assert(output != NULL);
        assert(compare(output, t->output) && "not the same");
        free(output);
        printf("passed\n");
    }
}

int
main(void)
{
    test();
    return 0;
}
