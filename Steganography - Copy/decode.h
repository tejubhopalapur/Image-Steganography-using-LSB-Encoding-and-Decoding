#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include "types.h"

#ifndef MAX_FILE_SUFFIX
#define MAX_FILE_SUFFIX 10
#endif


typedef struct _DecodeInfo
{
    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

    /* Output File Info */
    char *output_fname;
    FILE *fptr_output;

    /* Secret File Info */
    char extn_secret_file[MAX_FILE_SUFFIX];
    long size_secret_file;
    int extn_size;

} DecodeInfo;


/* Read and validate decode arguments */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Open stego image */
Status open_decode_files(DecodeInfo *decInfo);

/* Decode one byte from image */
char decode_byte_from_lsb(char *image_arr);

/* Decode magic string */
Status decode_magic_string(DecodeInfo *decInfo);

/* Decode extension size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* Decode extension */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode secret data */
Status decode_secret_file_data(DecodeInfo *decInfo);

#endif