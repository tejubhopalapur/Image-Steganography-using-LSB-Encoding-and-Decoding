#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include<string.h>
#include "types.h"

OperationType check_operation_type(char *argv[])
{
    if(argv[1] != NULL)
    {
    if(strcmp(argv[1], "-e") == 0)
    return e_encode;
    if(strcmp(argv[1], "-d") == 0)
    return e_decode;
    }
    else
    return e_unsupported;
}

int main(int argc, char * argv[])
{
    if(argc < 2)
    {
        printf("Error: Invalid arguments\n");
        return 1;
    }
    OperationType res = check_operation_type(argv);

    if(res == e_encode)
    {
        EncodeInfo encInfo;
        if(read_and_validate_encode_args(argv, &encInfo) == e_success)
        {
            if(do_encoding(&encInfo) == e_failure)
            {
                printf("Error: Encoding failed\n");
                return 1;
            }
            else
            {
                printf("Encoding completed successfully\n");
                return 0;
            }

        }
        else
        {
            printf("Error: Validation failed\n");
            return 1;
        }
    }
    else if(res == e_decode)
{
    DecodeInfo decInfo;

    if(read_and_validate_decode_args(argv, &decInfo) == e_success)
    {
        if(open_decode_files(&decInfo) == e_success)
        {
            // Skip BMP header
            fseek(decInfo.fptr_stego_image, 54, SEEK_SET);

            if(decode_magic_string(&decInfo) == e_success)
            {
                printf("Magic string decoded successfully\n");

                decode_secret_file_extn_size(&decInfo);

                decode_secret_file_extn(&decInfo);

                decode_secret_file_size(&decInfo);

                decode_secret_file_data(&decInfo);

                printf("Decoding completed successfully\n");
            }
            else
            {
                printf("Error: Magic string mismatch\n");
                return 1;
            }
        }
        else
        {
            printf("Error: Unable to open stego image\n");
            return 1;
        }
    }
    else
    {
        printf("Error: Validation failed\n");
        return 1;
    }

    return 0;
}
    else
    {
        printf("Error: Unsupported operation\n");
        return 1;
    }
    return 0;
}
