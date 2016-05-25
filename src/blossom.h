#ifndef BLOOM_H
#define BLOOM_H 

#include "bitvector.h"
#include <stdlib.h>

typedef struct {
    size_t capacity;
    size_t nhashes;
    double error_rate;
    size_t nitems;
    size_t size;
    bitvector_t * bitvector;
} bloom_t;

int bloom_init(bloom_t * bloom, size_t capacity, double error_rate);
bloom_t * bloom_create(size_t capacity, double error_rate);
int bloom_add(bloom_t * bloom, const char *key, size_t key_len);
int bloom_get(bloom_t * bloom, const char *key, size_t key_len);
void bloom_destroy(bloom_t * bloom);

#endif /* ifndef BLOOM_H */
/*
 * vim: ft=c
 */
