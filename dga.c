#include "dga.h"

void *_dga_new(size_t n, size_t obj_size);
inline void *_dga_new(size_t n, size_t obj_size) {
    size_t alloc_size = sizeof(struct dga_header) + obj_size * n;
    struct dga_header *h = malloc(alloc_size);
    if (h) {
        h->len = n;
        h->szt = obj_size;
    }
    // cppcheck-suppress memleak
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
        // cppcheck-suppress memleak
        return (char *)tmp + sizeof(struct dga_header);
    }

    return NULL;
}
