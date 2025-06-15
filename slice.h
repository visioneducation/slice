#ifndef SLICE_H
#define SLICE_H

#include <stdbool.h>
#include <stddef.h>

#define DEFAULT_INITIAL_CAPACITY 4

typedef struct {
  void *data;
  size_t length;
  size_t capacity;
  size_t element_size;
} Slice;

Slice *slice_new(size_t element_size, size_t initial_cap);
void slice_free(Slice *s);
size_t slice_len(const Slice *s);
size_t slice_capacity(const Slice *s);
void *slice_get(const Slice *s, size_t index);
int slice_set(Slice *s, size_t index, const void *src);
int slice_append(Slice *s, const void *element);
Slice *slice_slice(const Slice *s, size_t start, size_t end);
void slice_free_subslice(Slice *s);
int slice_insert(Slice *s, size_t index, const void *element);
int slice_delete(Slice *s, size_t index);

#endif
