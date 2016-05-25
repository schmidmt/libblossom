#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "blossom.h"
#include "../reqs/murmur3/murmur3.h"

#define SALT_CONSTANT 0x97c29b3a

#ifdef __GNUC__
# define unlikely(C) __builtin_expect((C), 0)
#else
# define unlikely(C) (C)
#endif

#if __x86_64__
# define hashfunc MurmurHash3_x64_128
#elif __i686__
# define hashfunc MurmurHash3_x86_128
#else
# error "Not a currently supported architecture."
#endif


static size_t bloom_optimal_hash_count(const size_t bits, const size_t items)
{
    size_t k = (size_t) ceil(((double) bits * log(2)) / items);
    if (k < 1)
        return 1;
    return k;
}


/**
 * Return size of filter required to achieve an accuracy of error_rate with capacity items.
 */
static size_t bloom_bits_for_accuracy(size_t capacity, double error_rate)
{
    return (size_t) ceil((-log(error_rate) * capacity) / (log(2) * log(2)));
}


/**
 * hashing function
 * @param key Key to hash
 * @param key_len Length of the key
 * @param hashes pointer to location to place hashes
 * @param nhashes number of hashes to fill
 * @param max_size maximum of each hash
 */
static void gen_hashes(const char * key, size_t key_len, size_t *hashes, size_t nhashes, size_t max_size)
{
    uint32_t hashvalues[4];
    hashfunc(key, key_len, SALT_CONSTANT, hashvalues);
    uint32_t a = hashvalues[0];
    uint32_t b = hashvalues[1];

    hashes[0] = a % max_size;
    size_t i;
    for (i = 1; i < nhashes; ++i)
    {
        hashes[i] = (hashes[i-1] + b) % max_size;
    }
}


/**
 * Initialize bloom filter
 */
int bloom_init(bloom_t * bloom, size_t capacity, double error_rate)
{
    bloom->capacity = capacity;
    bloom->nitems = 0;
    bloom->error_rate = error_rate;
    bloom->size = bloom_bits_for_accuracy(capacity, error_rate);
    bloom->nhashes = bloom_optimal_hash_count(bloom->size, capacity);
    bloom->bitvector = bv_create(bloom->size);
    if (bloom->bitvector == NULL)
    {
        return -1;
    }
    return 0;
}

bloom_t * bloom_create(size_t capacity, double error_rate)
{
    bloom_t * bloom = malloc(sizeof(bloom_t));
    if (bloom == NULL || bloom_init(bloom, capacity, error_rate))
    {
        free(bloom);
        return NULL;
    }
    return bloom;
}


int bloom_add(bloom_t * bloom, const char *key, size_t key_len)
{
    size_t * hashes = calloc(bloom->nhashes, sizeof(size_t));
    if (hashes == NULL)
        return -1;
    gen_hashes(key, key_len, hashes, bloom->nhashes, bloom->size);
    int seen = 1; /* andable values for seeing if the value was already set */
    size_t i;
    for (i = 0; i < bloom->nhashes; ++i)
    {
        int cval = bv_get(bloom->bitvector, hashes[i]);
        seen &= cval;
        if (!cval)
            bv_set(bloom->bitvector, hashes[i]);
    }
    free(hashes);
    if (!seen)
        bloom->nitems++;
    return seen;
}

int bloom_get(bloom_t * bloom, const char *key, size_t key_len)
{
    size_t * hashes = calloc(bloom->nhashes, sizeof(size_t));
    gen_hashes(key, key_len, hashes, bloom->nhashes, bloom->size);
    size_t i;
    for (i = 0; i < bloom->nhashes; ++i)
    {
        if (!bv_get(bloom->bitvector, hashes[i]))
        {
            free(hashes);
            return 0;
        }
    }
    free(hashes);
    return 1;

}

void bloom_destroy(bloom_t * bloom)
{
    bv_destroy(bloom->bitvector);
    free(bloom);
}
