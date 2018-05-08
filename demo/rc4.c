/**
*   @author Nikita Somenkov
*   @email somenkov.nikita@icloud.com
*   @date   07 May 2018
*/

#include <stdio.h>
#include <string.h>
#include <arcfour.h>
#include <stdlib.h>

#define PATH_MAX    (256)
#define KEY_MAX     (128)
#define BUFFER_MAX  (1024)


int usage()
{
    fprintf(stderr, "     ./rc4 --input=<input-file> --output=<output-file> --key=<key> [--encode] [--decode]\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "            --input - file with encode\n");
    fprintf(stderr, "           --output - file with decode\n");
    fprintf(stderr, "              --key - string with key\n");
    fprintf(stderr, "           --encode - run in encode mode\n");
    fprintf(stderr, "           --decode - run in decode mode\n");
    return -1;
}

typedef enum mode {
    ENCODE = 0x10,
    DECODE = 0x20,
} mode;

struct options {
    char input[PATH_MAX];
    char output[PATH_MAX];
    char key[KEY_MAX];
    mode mode;
};

#define OPTIONS_INIT (struct options) { \
    .input = "",    \
    .output = "",   \
    .key = ""       \
};

static const char *
start_from(const char *string, const char *pattern)
{
    const size_t pattern_size = strlen(pattern);
    if (strncmp(string, pattern, pattern_size) == 0)
        return string + pattern_size;
    return NULL;
}

static int
parse_args(struct options *opts, int argc, const char *argv[])
{
    size_t i;
    const char *arg;

    if (argc < 3)
        return usage();

    for (i = 1; i < argc; i++) {
        const char *argument = argv[i];
        if ((arg = start_from(argv[i], "--input=")) != NULL)
            strcpy(opts->input, arg);
        else if ((arg = start_from(argv[i], "--output=")) != NULL)
            strcpy(opts->output, arg);
        else if ((arg = start_from(argv[i], "--key=")) != NULL)
            strcpy(opts->key, arg);
        else if ((arg = start_from(argv[i], "--encode")) != NULL)
            opts->mode = ENCODE;
        else if ((arg = start_from(argv[i], "--decode")) != NULL)
            opts->mode = DECODE;
        else
            return usage();
    }

    if (opts->mode != ENCODE && opts->mode != DECODE)
        return usage();

    return 0;
}

static size_t
read_file(const char * path, void *buffer, size_t buffer_size)
{
    size_t bytes;
    FILE *file = fopen(path, "rb");

    if (file == NULL)
        return SIZE_MAX;

    bytes = fread(buffer, sizeof(*path), buffer_size, file);

    fclose(file);
    return bytes;
}

static size_t
write_file(const char * path, const void *buffer, size_t buffer_size)
{
    size_t bytes;
    FILE *file = fopen(path, "wb");

    if (file == NULL)
        return SIZE_MAX;

    bytes = fwrite(buffer, sizeof(*path), buffer_size, file);

    fclose(file);
    return bytes;
}

int
main(int argc, const char *argv[])
{
    int rc = 0;
    size_t bytes;
    struct arcfour_context context;
    struct options opts = OPTIONS_INIT;
    byte *input_bytes;
    byte *output_bytes;

    if ((rc = parse_args(&opts, argc, argv)) != 0) {
        return rc;
    }

    input_bytes = malloc(sizeof(byte) * BUFFER_MAX);
    output_bytes = malloc(sizeof(byte) * BUFFER_MAX);
    if ((bytes = read_file(opts.input, input_bytes, BUFFER_MAX)) == SIZE_MAX) {
        fprintf(stderr, "Incorrect input file");
        rc = -1;
        goto free;
    }

    arcfour_init(&context, (byte *) opts.key, strlen(opts.key));

    if (opts.mode == ENCODE) {
        arcfour_encode(&context,
                       input_bytes, bytes,
                       output_bytes, BUFFER_MAX);
    }
    else if (opts.mode == DECODE) {
        arcfour_decode(&context,
                       input_bytes, bytes,
                       output_bytes, BUFFER_MAX);
    }

    if (write_file(opts.output, output_bytes, bytes) == SIZE_MAX) {
        fprintf(stderr, "Incorrect output file");
        rc = -2;
    }

free:
    free(input_bytes);
    free(output_bytes);

    return rc;
}