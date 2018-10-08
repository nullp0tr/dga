/*
 * Copyright 2018 Ahmed Alsharif
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdlib.h>
#include <string.h>

struct dga_header {
    size_t len;
    size_t cap;
    size_t szt;
};

#define _dga_header(arr)                                                       \
    ((struct dga_header *)((char *)arr - sizeof(struct dga_header)))

#define dga_new(n, T) _dga_new(n, sizeof(T))

void *_dga_new(size_t n, size_t obj_size);
inline void *_dga_new(size_t n, size_t obj_size) {
    size_t alloc_size = sizeof(struct dga_header) + obj_size * n;
    struct dga_header *h = malloc(alloc_size);
    if (h) {
        h->len = n;
        h->szt = obj_size;
    }
    return (void *)(h + 1);
}

size_t dga_len(void *arr);
inline size_t dga_len(void *arr) {
    struct dga_header *h = _dga_header(arr);
    return h->len;
}

void *dga_resize(void *arr, size_t new_size);
inline void *dga_resize(void *arr, size_t new_size) {
    struct dga_header *h = _dga_header(arr);
    h->len = new_size;

    size_t alloc_size = sizeof(struct dga_header) + (h->szt * h->len);
    void *tmp = realloc(h, alloc_size);
    if (tmp) {
        return (char *)tmp + sizeof(struct dga_header);
    }
    return tmp;
}

void *dga_grow(void *arr, size_t n);
inline void *dga_grow(void *arr, size_t n) {
    return dga_resize(arr, dga_len(arr) + n);
}

void *dga_shrink(void *arr, size_t n);
inline void *dga_shrink(void *arr, size_t n) {
    return dga_resize(arr, dga_len(arr) - n);
}

#define _dga_alloc_size(header)                                                \
    sizeof(struct dga_header) + header->szt * header->len;

void dga_free(void *arr);
inline void dga_free(void *arr) {
    struct dga_header *h = _dga_header(arr);
    free(h);
}

void *dga_dup(void *arr);
inline void *dga_dup(void *arr) {
    struct dga_header *h = _dga_header(arr);
    size_t size = _dga_alloc_size(h);

    void *tmp = malloc(size);
    if (tmp) {
        memcpy(tmp, h, size);
        return (char *)tmp + sizeof(struct dga_header);
    }

    return NULL;
}

#define dga_foreach(p, arr) for (p = arr; p != arr + dga_len(arr); p++)
