#include "http.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void send_json_header()
{
    printf("Content-Type: application/json\r\n\r\n");
}

int read_post(char* buffer, int size)
{
    char* lengthText = getenv("CONTENT_LENGTH");

    if (lengthText == NULL)
    {
        buffer[0] = '\0';
        return 0;
    }

    int length = atoi(lengthText);

    if (length >= size)
    {
        length = size - 1;
    }

    int read = (int)fread(
        buffer,
        1,
        length,
        stdin
    );

    buffer[read] = '\0';

    return read;
}

void get_param(
    const char* data,
    const char* key,
    char* output,
    int outputSize
)
{
    output[0] = '\0';

    char search[100];

    snprintf(
        search,
        sizeof(search),
        "%s=",
        key
    );

    const char* start = strstr(
        data,
        search
    );

    if (start == NULL)
    {
        return;
    }

    start += strlen(search);

    const char* end = strchr(
        start,
        '&'
    );

    int length;

    if (end == NULL)
    {
        length = strlen(start);
    }
    else
    {
        length = end - start;
    }

    if (length >= outputSize)
    {
        length = outputSize - 1;
    }

    strncpy(
        output,
        start,
        length
    );

    output[length] = '\0';
}