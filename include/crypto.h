/**
*   @author Nikita Somenkov
*   @email somenkov.nikita@icloud.com
*   @date   05 May 2018
*/

#pragma once

#include <stddef.h>

enum crypto_errors {
    CRYPTO_ERROR_OK,
    CRYPTO_ERROR_MEMORY,
    CRYPTO_ERROR_INVALID_ARGUMENT,
    CRYPTO_ERROR_UNKNOWN,
};

typedef struct crypto_context {} crypto_context;
typedef unsigned char byte;


typedef int (*crypto_decode)(crypto_context *cxt,
                             const byte *input_bytes, size_t input_size,
                             byte *decoded_bytes, size_t output_size);

typedef int (*crypto_encode)(crypto_context *cxt,
                             const byte *bytes, size_t input_size,
                             byte *decoded_bytes, size_t output_size);

