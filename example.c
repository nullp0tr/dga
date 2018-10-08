#include "dga.h"
#include <assert.h>
#include <stdio.h>
#include <unistd.h>

struct foo {
    int a, b;
};

void struct_array() {
    struct foo *foo_arr = dga_new(4, struct foo);

    for (size_t i = 0; i < dga_len(foo_arr); i++) {
        foo_arr[i] = (struct foo){.a = i, .b = i * 2};
    }

    for (struct foo *f = foo_arr; f < foo_arr + dga_len(foo_arr); f++) {
        printf("a: %d, b: %d\n", f->a, f->b);
    }

    dga_free(foo_arr);
}

void draw_matrix(size_t len, size_t h, size_t w) {
    assert(h * w > 0 && len > 0);

    char(*matrix)[h][w] = dga_new(len, char[h][w]);
    size_t er = fread(matrix, h * w, len, stdin);
    if (er != len) {
        dga_free(matrix);
        fprintf(stderr, "error in fread, with %ld elments read\n", er);
        return;
    }

    getchar(); // a hack to eat up the '\n'

    for (size_t i = 0; i < len; i++) {
        for (size_t j = 0; j < w; j++) {
            for (size_t k = 0; k < h; k++) {
                printf("%c ", matrix[i][k][j]);
            }
            printf("\n");
        }
    }
    dga_free(matrix);
}

int main() {
    struct_array();
    draw_matrix(1, 2, 2);
    draw_matrix(1, 3, 3);

    char *buf = dga_new(2, char);
    int br;
    while (0 != (br = read(STDIN_FILENO, buf, dga_len(buf)))) {
        if (0 > br) {
            perror("read");
            goto fail;
        }

        if (dga_len(buf) == (unsigned)br) {
            char *tmp = dga_grow(buf, dga_len(buf) * 2);
            if (!tmp) {
                fprintf(stderr, "error growing array.\n");
                goto fail;
            }
            buf = tmp;
        }

        if (dga_len(buf) > (unsigned)br) {
            char *tmp = dga_resize(buf, br);
            if (!tmp) {
                fprintf(stderr, "error shrinking array.\n");
                goto fail;
            }
            buf = tmp;
        }

        int bw = write(STDOUT_FILENO, buf, br);
        if (bw == -1) {
            perror("write");
            goto fail;
        }
    }

    dga_free(buf);
    printf("exit..\n");
    return 0;

fail:
    dga_free(buf);
    return 1;
}