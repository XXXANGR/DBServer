#ifndef EXTCLIB_HTTP_H_
#define EXTCLIB_HTTP_H_

#include <stdint.h>

typedef struct HTTPreq {
    char method[16]; // GET
    char path[2048]; // /books
    char proto[16];  // HTTP/1.1
    uint8_t state;
    size_t index;
} HTTPreq;

typedef struct HTTP HTTP;

typedef enum type_http_t
{
    HTTP_TEXT = 0,
    HTTP_TEXT_TABLE = 1,
    HTTP_IMG_JPG = 2,
} type_http_t;

extern HTTP *new_http(char *address);
extern void free_http(HTTP *http);

extern void handle_http(HTTP *http, char *path, void(*)(int, HTTPreq*));
extern int8_t listen_http(HTTP *http);

extern void parse_http(int conn, char *filename, type_http_t type, char *tbl_name);

#endif /* EXTCLIB_HTTP_H_ */
