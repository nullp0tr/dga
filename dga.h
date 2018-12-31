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

#ifndef _DGA_H_
#define _DGA_H_

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

size_t dga_len(void *arr);

void *dga_resize(void *arr, size_t new_size);

#define dga_resize_or(A, N)                                                    \
    for (void *tmp = dga_resize(A, N); tmp == NULL ? 1 : (A = tmp, 0); tmp = A)
void *dga_grow(void *arr, size_t n);

#define dga_grow_or(A, N)                                                      \
    for (void *tmp = dga_grow(A, N); tmp == NULL ? 1 : (A = tmp, 0); tmp = A)

void *dga_shrink(void *arr, size_t n);

#define dga_shrink_or(A, N)                                                    \
    for (void *tmp = dga_shrink(A, N); tmp == NULL ? 1 : (A = tmp, 0); tmp = A)

#define _dga_alloc_size(header)                                                \
    sizeof(struct dga_header) + header->szt * header->len;

void dga_free(void *arr);

void *dga_dup(void *arr);

#define dga_foreach(p, arr) for (p = arr; p != arr + dga_len(arr); p++)

#endif
