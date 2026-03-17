#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N    256
#define MAX_NAME 64

typedef struct {
    int to[MAX_N][MAX_N];
    int size[MAX_N];
    int n;
} Graph;

void
graph_init(Graph *g, int n)
{
    memset(g, 0, sizeof(*g));
    g->n = n;
}

void
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

int
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

int
main(void)
{
    FILE *file = fopen("input.txt", "r");
    if (!file) {
        fprintf(stderr, "Error: cannot open input.txt\n");
        return 1;
    }

    char modules[MAX_N][MAX_NAME];
    char deps[MAX_N][MAX_N][MAX_NAME];
    int dep_count[MAX_N] = {0};
    int n                = 0;
    char line[1024];

    while (fgets(line, sizeof(line), file)) {
        int len = strlen(line);
        if (len == 0)
            continue;
        if (len > 0 && line[len - 1] == '\n')
            line[--len] = '\0';

        char *colon = strchr(line, ':');
        if (!colon) {
            fprintf(stderr, "Error: invalid format at line %d\n", n + 1);
            return 1;
        }

        char name[MAX_NAME] = "";
        int ni              = 0;
        for (char *p = line; p < colon; p++)
            if (*p != ' ' && *p != '\t')
                name[ni++] = *p;
        name[ni] = '\0';

        if (ni == 0) {
            fprintf(stderr, "Error: missing module name at line %d\n", n + 1);
            return 1;
        }

        strcpy(modules[n], name);

        char *tok = strtok(colon + 1, " \t");
        while (tok) {
            strcpy(deps[n][dep_count[n]++], tok);
            tok = strtok(NULL, " \t");
        }

        n++;
    }

    fclose(file);

    Graph g;
    graph_init(&g, n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < dep_count[i]; j++) {
            int dep = find_module(modules, n, deps[i][j]);
            if (dep >= 0)
                add_edge(&g, dep, i);
        }
    }

    int result[MAX_N];
    int len = topo_sort(&g, result);
    if (len < 0)
        return 1;

    for (int i = 0; i < len; i++)
        printf("%s\n", modules[result[i]]);

    return 0;
}