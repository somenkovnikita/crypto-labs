/**
*   @author Nikita Somenkov
*   @email somenkov.nikita@icloud.com
*   @date   07 May 2018
*/

#pragma once

#include "crypto.h"

#define N       (8u)
#define S_SIZE  (1u << N)

typedef struct arcfour_context {
    byte S[S_SIZE];
} arcfour_context;

void arcfour_init(arcfour_context *cxt, const byte *key, size_t key_size);

int arcfour_decode(arcfour_context *cxt,
                   const byte *input_bytes, size_t input_size,
                   byte *decoded_bytes, size_t output_size);

int arcfour_encode(arcfour_context *cxt,
                   const byte *bytes, size_t input_size,
                   byte *decoded_bytes, size_t output_size);