/**
*   @author Nikita Somenkov
*   @email somenkov.nikita@icloud.com
*   @date   05 May 2018
*/

#include "arcfour.h"

static void
swap(byte *a, byte *b)
{
    byte t;

    t = *b, *b = *a, *a = t;
}

void
arcfour_init(arcfour_context *cxt, const byte *key, size_t size)
{
    size_t i;
    size_t j = 0;

    if (cxt == NULL)
        return;

    for (i = 0; i < S_SIZE; i++)
        cxt->S[i] = (byte) i;

    for (i = 0; i < S_SIZE; i++) {
        j = (j + cxt->S[i] + key[i % size]) % S_SIZE;
        swap(cxt->S + i, cxt->S + j);
    }
}

static byte
generate_key(arcfour_context *cxt)
{
    size_t t;
    size_t i = 0;
    size_t j = 0;

    i = (i + 1) % S_SIZE;
    j = (j + cxt->S[i]) % S_SIZE;
    swap(cxt->S + i, cxt->S + j);

    t = (cxt->S[i] + cxt->S[j]) % S_SIZE;

    return cxt->S[t];
}

static byte
xor(byte a, byte b)
{
    return a ^ b;
}

static int
check_arguments(arcfour_context *ctx,
                const byte *input_bytes,
                const byte *decoded_bytes)
{
    return ctx == NULL || input_bytes == NULL || decoded_bytes == NULL;
}

int
arcfour_decode(arcfour_context *ctx,
               const byte *input_bytes, size_t input_size,
               byte *decoded_bytes, size_t output_size)
{
    byte key;
    size_t i;

    if (check_arguments(ctx, input_bytes, decoded_bytes))
        return CRYPTO_ERROR_INVALID_ARGUMENT;

    if (input_size > output_size)
        return CRYPTO_ERROR_MEMORY;

    for (i = 0; i < input_size; i++)
    {
        key = generate_key(ctx);
        decoded_bytes[i] = xor(input_bytes[i], key);
    }

    return 0;
}

int
arcfour_encode(arcfour_context *ctx,
               const byte *input_bytes, size_t input_size,
               byte *decoded_bytes, size_t output_size)
{
    return arcfour_decode(ctx, input_bytes, input_size,
                          decoded_bytes, output_size);
}
