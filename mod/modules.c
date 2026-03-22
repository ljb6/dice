#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "modules.h"

#define MAX_N    256
#define MAX_NAME 64

typedef struct {
    int to[MAX_N][MAX_N];
    int size[MAX_N];
    int n;
} Graph;

static void
graph_init(Graph *g, int n)
{
    memset(g, 0, sizeof(*g));
    g->n = n;
}

static void
add_edge(Graph *g, int u, int v)
{
    g->to[u][g->size[u]++] = v;
}

static int
dfs(Graph *g, int u, int *state, int *stack, int *top)
{
    state[u] = 1;

    for (int i = 0; i < g->size[u]; i++) {
        int v = g->to[u][i];
        if (state[v] == 1)
            return 1;
        if (state[v] == 0)
            if (dfs(g, v, state, stack, top))
                return 1;
    }

    state[u]        = 2;
    stack[(*top)++] = u;
    return 0;
}

static int
topo_sort(Graph *g, int *result)
{
    int state[MAX_N] = {0};
    int stack[MAX_N];
    int top = 0;

    for (int i = 0; i < g->n; i++) {
        if (state[i] == 0) {
            if (dfs(g, i, state, stack, &top)) {
                fprintf(stderr, "Error: cycle detected!\n");
                return -1;
            }
        }
    }

    for (int i = 0; i < top; i++)
        result[i] = stack[top - 1 - i];

    return top;
}

static int
find_module(char names[][MAX_NAME], int n, const char *name)
{
    for (int i = 0; i < n; i++)
        if (strcmp(names[i], name) == 0)
            return i;
    return -1;
}

char *
resolve_modules(const char *input)
{
    char buf[4096];
    strncpy(buf, input, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    char modules[MAX_N][MAX_NAME];
    char deps[MAX_N][MAX_N][MAX_NAME];
    int dep_count[MAX_N] = {0};
    int n                = 0;

    char *p = buf;
    while (*p) {
        char *end  = strchr(p, '\n');
        char *line = p;
        if (end) {
            *end = '\0';
            p    = end + 1;
        } else {
            p += strlen(p);
        }

        if (*line == '\0')
            continue;

        char *colon = strchr(line, ':');
        if (!colon) {
            fprintf(stderr, "Error: invalid format\n");
            return NULL;
        }

        char name[MAX_NAME] = "";
        int ni              = 0;
        for (char *q = line; q < colon; q++)
            if (*q != ' ' && *q != '\t')
                name[ni++] = *q;
        name[ni] = '\0';

        if (ni == 0) {
            fprintf(stderr, "Error: missing module name\n");
            return NULL;
        }

        strcpy(modules[n], name);

        char *tok = strtok(colon + 1, " \t");
        while (tok) {
            strcpy(deps[n][dep_count[n]++], tok);
            tok = strtok(NULL, " \t");
        }

        n++;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < dep_count[i]; j++) {
            if (find_module(modules, n, deps[i][j]) < 0) {
                strcpy(modules[n], deps[i][j]);
                dep_count[n] = 0;
                n++;
            }
        }
    }

    Graph g;
    graph_init(&g, n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < dep_count[i]; j++) {
            int dep = find_module(modules, n, deps[i][j]);
            add_edge(&g, dep, i);
        }
    }

    int result[MAX_N];
    int len = topo_sort(&g, result);
    if (len < 0)
        return NULL;

    size_t out_size = (size_t)len * (MAX_NAME + 1) + 1;
    char *out       = malloc(out_size);
    if (!out)
        return NULL;

    out[0] = '\0';
    for (int i = 0; i < len; i++) {
        strcat(out, modules[result[i]]);
        if (i < len - 1)
            strcat(out, "\n");
    }

    return out;
}
