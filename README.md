# LibBlossom
Atomic Bloom Filters

[![Build Status](https://travis-ci.org/schmidmt/libblossom.svg?branch=master)](https://travis-ci.org/schmidmt/libblossom)

## Installation
From GIT,

```bash
./autogen.sh
./configure
make
sudo make install
```


## Example
The following is a Bloom filter version of the unique program. Note, it does not required the input to be sorted

```c
#include <stdio.h>
#include <inttypes.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "../src/blossom.h"

#define MAX_LINE_LENGTH 128

int main(int argc, char **argv)
{
    if (argc != 3) {
        printf("usage: %s MAXITEMS MAXERROR\n", argv[0]);
        exit(1);
    }

    uintmax_t size = strtoumax(argv[1], NULL, 10);
    if (size == UINTMAX_MAX || size == 0) {
        printf("Invalid MAXITEMS: %s\n", argv[1]);
        exit(1);
    }

    double error_rate = strtod(argv[2], NULL);
    if (error_rate == 0) {
        printf("Invalid MAXERROR: %s\n", argv[2]);
        exit(1);
    }

    bloom_t * bloom = bloom_create((size_t) size, error_rate);
        char buffer[MAX_LINE_LENGTH];
        while (fgets(buffer, MAX_LINE_LENGTH, stdin) != NULL) {
          if (bloom_add(bloom, buffer, strlen(buffer) - 1) == 0) {
              printf("%s", buffer);
          }
        }
}
```

To build, run `gcc -o uniq uniq.c -lblossom`.
An example of running this could be:

```bash
echo -e 'a\nb\nc\na\nc\n' | ./uniq 500 0.001
```
