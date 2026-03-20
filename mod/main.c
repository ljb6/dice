#include <stdio.h>
#include <stdlib.h>

#include "modules.h"

int
main(void)
{
    FILE *file = fopen("input.txt", "r");
    if (!file) {
        fprintf(stderr, "Error: cannot open input.txt\n");
        return 1;
    }

    char buf[4096];
    size_t len = fread(buf, 1, sizeof(buf) - 1, file);
    buf[len]   = '\0';
    fclose(file);

    char *result = resolve_modules(buf);
    if (!result)
        return 1;

    printf("%s\n", result);
    free(result);
    return 0;
}
