#include "slice.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Slice *slice_new(size_t element_size, size_t initial_cap) {
  Slice *s = malloc(sizeof(Slice));
  if (!s) {
    return NULL;
  }

  if (initial_cap == 0) {
    initial_cap = DEFAULT_INITIAL_CAPACITY;
  }

  s->data = malloc(element_size * initial_cap);

  if (!s->data) {
    free(s);
    return NULL;
  }

  s->length = 0;
  s->capacity = initial_cap;
  s->element_size = element_size;

  return s;
}

void slice_free(Slice *s) {
  if (!s) {
    return;
  }
  free(s->data);
  free(s);
}

size_t slice_len(const Slice *s) {
  if (!s) {
    return 0;
  }
  return s->length;
}

size_t slice_capacity(const Slice *s) {
  if (!s) {
    return 0;
  }
  return s->capacity;
}

void *slice_get(const Slice *s, size_t index) {
  if (!s || index >= s->length) {
    return NULL;
  }

  char *base = (char *)s->data;
  return base + (index * s->element_size);
}

int slice_set(Slice *s, size_t index, const void *src) {
  if (!s || !src || index >= s->length) {
    return -1;
  }

  char *base = (char *)s->data;
  void *dest = base + (index * s->element_size);

  memcpy(dest, src, s->element_size);

  return 0;
}

static int slice_grow(Slice *s, size_t min_cap) {
  if (!s || min_cap <= s->capacity) {
    return 0;
  }

  size_t new_cap = s->capacity;

  // double until it reaches 1024, then grow by 25%
  if (s->capacity < 1024) {
    new_cap = s->capacity * 2;
  } else {
    new_cap = s->capacity + (s->capacity / 4);
  }

  if (new_cap < min_cap) {
    new_cap = min_cap;
  }

  void *new_data = realloc(s->data, new_cap * s->element_size);

  if (!new_data) {
    return -1;
  }

  s->data = new_data;
  s->capacity = new_cap;

  return 0;
}

int slice_append(Slice *s, const void *element) {
  if (!s || !element) {
    return -1;
  }

  if (s->length >= s->capacity) {
    if (slice_grow(s, s->length + 1) != 0) {
      return -1;
    }
  }

  char *base = (char *)s->data;
  char *target = base + (s->length * s->element_size);

  memcpy(target, element, s->element_size);
  s->length++;

  return 0;
}

Slice *slice_slice(const Slice *s, size_t start, size_t end) {
  if (!s || start > end || start > s->length || end > s->length) {
    return NULL;
  }

  Slice *sub = malloc(sizeof(Slice));
  if (!sub) {
    return NULL;
  }

  char *base = (char *)s->data;

  sub->data = base + (start * s->element_size);
  sub->length = end - start;
  sub->capacity = s->capacity - start;
  sub->element_size = s->element_size;

  return sub;
}

void slice_free_subslice(Slice *s) {
  if (s) {
    free(s);
  }
}

int slice_insert(Slice *s, size_t index, const void *element) {
  if (!s || !element || index > s->length) {
    return -1;
  }

  if (s->length >= s->capacity) {
    if (slice_grow(s, s->length + 1) != 0) {
      return -1;
    }
  }

  char *base = (char *)s->data;

  if (index == s->length) {
    void *target = base + (s->length * s->element_size);
    memcpy(target, element, s->element_size);
  } else {
    void *src = base + (index * s->element_size);
    void *dest = base + ((index + 1) * s->element_size);
    size_t bytes_to_move = (s->length - index) * s->element_size;

    memmove(dest, src, bytes_to_move);
    memcpy(src, element, s->element_size);
  }

  s->length++;
  return 0;
}

int slice_delete(Slice *s, size_t index) {
  if (!s || index >= s->length) {
    return -1;
  }

  char *base = (char *)s->data;

  if (index == s->length - 1) {
    s->length--;
  } else {
    void *dest = base + (index * s->element_size);
    void *src = base + ((index + 1) * s->element_size);
    size_t bytes_to_move = (s->length - index - 1) * s->element_size;

    memmove(dest, src, bytes_to_move);
    s->length--;
  }

  return 0;
}
