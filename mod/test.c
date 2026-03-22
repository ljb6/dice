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
        {.name   = "Undeclared dep",
         .input  = "a : b c\nb : c",
         .output = "c\nb\na"},
        {.name = "Single module", .input = "a :", .output = "a"},
        {.name = "Single undeclared", .input = "a : b", .output = "b\na"},
        {.name   = "Chain",
         .input  = "a : b\nb : c\nc : d\nd :",
         .output = "d\nc\nb\na"},
        {.name   = "Chain undeclared tail",
         .input  = "a : b\nb : c\nc : d",
         .output = "d\nc\nb\na"},
        {.name = "No deps", .input = "a :\nb :\nc :", .output = "c\nb\na"},
        {.name   = "Diamond",
         .input  = "a : b c\nb : d\nc : d\nd :",
         .output = "d\nc\nb\na"},
        {.name   = "Diamond undeclared",
         .input  = "a : b c\nb : d\nc : d",
         .output = "d\nc\nb\na"},
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
