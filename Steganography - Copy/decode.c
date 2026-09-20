#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "common.h"


Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if(argv[2] != NULL && strstr(argv[2], ".bmp") != NULL)
    {
        decInfo->stego_image_fname = argv[2];
    }
    else
    {
        return e_failure;
    }

    if(argv[3] != NULL)
    {
        decInfo->output_fname = argv[3];
    }
    else
    {
        decInfo->output_fname = "decoded";
    }

    return e_success;
}



Status open_decode_files(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");

    if(decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        printf("Error: Unable to open %s\n", decInfo->stego_image_fname);
        return e_failure;
    }

    return e_success;
}


char decode_byte_from_lsb(char *image_buffer)
{
    char ch = 0;

    for(int i = 0; i < 8; i++)
    {
        ch |= (image_buffer[i] & 1) << i;
    }

    return ch;
}



Status decode_magic_string(DecodeInfo *decInfo)
{
    int i;
    char arr[8];
    char magic_string[3];

    for(i = 0; i < strlen(MAGIC_STRING); i++)
    {
        fread(arr, 8, 1, decInfo->fptr_stego_image);

        magic_string[i] = decode_byte_from_lsb(arr);
    }

    magic_string[2] = '\0';

    if(strcmp(magic_string, MAGIC_STRING) == 0)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}





Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    char arr[32];

    fread(arr, 32, 1, decInfo->fptr_stego_image);

    int size = 0;

    for(int i = 0; i < 32; i++)
    {
        size = size | (arr[i] & 1) << i;
    }

    decInfo->extn_size = size;

    printf("Secret file extension size is decoded successfully\n");
    return e_success;
}



Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char arr[8];

    for(int i = 0; i < decInfo->extn_size; i++)
    {
        fread(arr, 8, 1, decInfo->fptr_stego_image);

        decInfo->extn_secret_file[i] = decode_byte_from_lsb(arr);
    }

    decInfo->extn_secret_file[decInfo->extn_size] = '\0';

    printf("Secret file extension is decoded successfully\n");
    return e_success;
}



Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char arr[32];

    fread(arr, 32, 1, decInfo->fptr_stego_image);

    int size = 0;

    for(int i = 0; i < 32; i++)
    {
        size = size | (arr[i] & 1) << i;
    }

    decInfo->size_secret_file = size;

    printf("Secret file data size is decoded successfully\n");
    return e_success;
}


Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char buffer[8];
    char ch;

    char output_file[100];

    strcpy(output_file, decInfo->output_fname);
    strcat(output_file, decInfo->extn_secret_file);

    decInfo->fptr_output = fopen(output_file, "w");

    if(decInfo->fptr_output == NULL)
    {
        perror("fopen");
        return e_failure;
    }


    for(int i = 0; i < decInfo->size_secret_file; i++)
    {
        fread(buffer, 8, 1, decInfo->fptr_stego_image);

        ch = decode_byte_from_lsb(buffer);

        fwrite(&ch, 1, 1, decInfo->fptr_output);
    }


    fclose(decInfo->fptr_output);

    printf("Secret file data is decoded successfully\n");
    return e_success;
}