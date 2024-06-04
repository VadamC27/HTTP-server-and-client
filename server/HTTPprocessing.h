#ifndef HTTP_PROCESSING
#define HTTP_PROCESSING

#include <sys/types.h>
#include <sys/stat.h>

enum request_type {GET, POST, PUT, PATCH, DELETE, HEAD, OPTION};

struct http_request{
    char * type;
    char * address;
    char * protocol; 
    char * header;
    char * body;
};

char * prepare_response(char * request_buffer, size_t request_size);
int fileExists(const char *filename);

#endif //HTTP_PROCESSING