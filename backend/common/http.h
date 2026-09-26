#ifndef HTTP_H
#define HTTP_H

void send_json_header();

int read_post(char* buffer, int size);

void get_param(
    const char* data,
    const char* key,
    char* output,
    int outputSize
);

#endif