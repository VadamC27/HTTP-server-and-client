#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#include "HTTPprocessing.h"

char * generate_get_response(const struct http_request request, const char * header);
char * generate_head_response(const struct http_request request, const char * header);
char * generate_put_response(const struct http_request request, const char * header, const char *body);
char * generate_delete_response(const struct http_request request, const char * header);
char * generate_method_not_allowed_response(void);
char * generate_bad_request_response(void);
char * generate_internal_error_response(void);
char * generate_forbidden_response(void);
char * generate_not_found_response(void);
char * create_path(char* inputString);

int fileExists(const char *filename) {
    return access(filename, F_OK) == 0;
}



char *prepare_response(char *request_buffer, size_t request_size) {
    struct http_request request = {0};

    // Find the position of the first newline character
    char *newline = strchr(request_buffer, '\n');
    if (newline == NULL) {
        //return strdup("err1: can't find newline symbol\n");
        return generate_bad_request_response();
    }

    // Extract and parse the first line
    char *firstline = malloc(newline - request_buffer + 1);
    if (firstline == NULL) {
        //return strdup("err3: memory allocation error\n");
        return generate_internal_error_response();
    }

    strncpy(firstline, request_buffer, newline - request_buffer);
    firstline[newline - request_buffer] = '\0';

    request.type = strdup("");  // Initialize to an empty string
    request.address = strdup("");
    request.protocol = strdup("");

    if (request.type == NULL || request.address == NULL || request.protocol == NULL) {
        free(firstline);
        free(request.type);
        free(request.address);
        free(request.protocol);
        //return strdup("err3: memory allocation error\n");
        return generate_internal_error_response();
    }

    int result = sscanf(firstline, "%s %s %s", request.type, request.address, request.protocol);

    free(firstline);

    if (result != 3) {
        free(request.type);
        free(request.address);
        free(request.protocol);
        //return strdup("err2: incorrect first line in request\n");
        return generate_bad_request_response();
    }

    //printf("%s %s %s\n", request.type, request.address, request.protocol);

    char header[2000] = ""; 
    char body[6000] = ""; 

    //int header_parsed = 0;
    //char *header_end = strstr(request_buffer, "\r\n\r\n");
    char *header_end = strstr(request_buffer, "\r\n\r\n");

    if (header_end != NULL) {
        // Copy the header
        char *first_line_end = strstr(request_buffer, "\r\n");
        if (first_line_end != NULL) {
            strncat(header, first_line_end + 2, header_end - (first_line_end + 2));
        }

        // Copy the body (skip the '\r\n\r\n' sequence)
        if (strlen(header_end + 4) < sizeof(body)) {
            strcpy(body, header_end + 4);
        } else {
            return generate_bad_request_response(); //buffer to small for request
        }
    } else {
        strcpy(header, request_buffer);
    }

    //printf("Header: %s\n\n",header);
    //printf("Body: %s",body);

    char * response = strdup("");
    if(strcmp(request.type,"GET")==0){
        printf("RECIVED GET REQUEST ON RESOURCE: %s\n", request.address);
        response = generate_get_response(request, header);
    }else if(strcmp(request.type,"HEAD")==0){
        printf("RECIVED HEAD REQUEST ON RESOURCE: %s\n", request.address);
        response = generate_head_response(request, header);
    }else if(strcmp(request.type,"PUT")==0){
        printf("RECIVED PUT REQUEST ON RESOURCE: %s\n", request.address);
        response = generate_put_response(request, header, body);
    }else if(strcmp(request.type,"DELETE")==0){
        printf("RECIVED DELETE REQUEST ON RESOURCE: %s\n", request.address);
        response = generate_delete_response(request, header);
    }else{
        printf("UNKOWN REQUEST TYPE!\n");
        response = generate_method_not_allowed_response();
    }

    //free(header_end);
    free(request.type);
    free(request.address);
    free(request.protocol);
    free(request.header);
    free(request.body);

    return response;
}

char * generate_get_response(const struct http_request request, const char * header){
    FILE *fptr;
    char resource_path[256] = "resources";
    const char *suffix = ".html";
    if(strcmp(request.address,"/")==0 ){
        strcat(resource_path, "/index.html");
    }else{
        strcat(resource_path, request.address);
        strcat(resource_path, suffix);
    }


    fptr = fopen(resource_path,"r" );

    if(fptr == NULL){        
        return generate_not_found_response();
    }

    fseek(fptr, 0, SEEK_END);
    long file_size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    // Allocate memory to read the file
    char *file_content = (char *)malloc(file_size + 1);

    if (file_content == NULL) {
        fclose(fptr);
        return generate_internal_error_response();
    }


    fread(file_content, 1, file_size, fptr);
    file_content[file_size] = '\0';  // Null-terminate the content

    fclose(fptr);

    char *response = (char *)malloc(300 + file_size);
    
    if (response == NULL) {
        free(file_content);
        return generate_internal_error_response();
    }

    sprintf(response, "HTTP/1.1 200 OK\r\nAccept-Ranges: bytes\r\nContent-Length: %ld\r\nContent-Type: text/html\r\n\r\n%s", file_size, file_content);


    free(file_content);
    return response;
}

char * generate_head_response(const struct http_request request, const char * header){
    FILE *fptr;
    char resource_path[256] = "resources";
    const char *suffix = ".html";
    if(strcmp(request.address,"/")==0 ){
        strcat(resource_path, "/index.html");
    }else{
        strcat(resource_path, request.address);
        strcat(resource_path, suffix);
    }


    fptr = fopen(resource_path,"r" );

    if(fptr == NULL){
        
        return generate_not_found_response();
    }

    fseek(fptr, 0, SEEK_END);
    long file_size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    char *response = (char *)malloc(300 + file_size);
    
    if (response == NULL) {
        generate_internal_error_response();
    }

    sprintf(response, "HTTP/1.1 200 OK\r\nAccept-Ranges: bytes\r\nContent-Length: %ld\r\nContent-Type: text/html\r\n\r\n", file_size);


    return response;
}

char * generate_put_response(const struct http_request request, const char * header, const char *body){
    FILE *fptr;
    char resource_path[256] = "resources";
    const char *suffix = ".html";
    if(strcmp(request.address,"/")==0 || strcmp(request.address,"/index")==0){ //don't allow 
        return generate_forbidden_response();
    }else{
        strcat(resource_path, request.address);
        strcat(resource_path, suffix);
    }

    if(fileExists(resource_path)){
        return generate_forbidden_response();
    }

    create_path(resource_path);
    

    fptr = fopen(resource_path, "w");

    if (fptr == NULL) {
        perror("Error opening file");
        return generate_internal_error_response();
    }

    fprintf(fptr, "%s\n", body);

    char *response = (char *)malloc(300);
    
    if (response == NULL) {
        generate_internal_error_response();
    }
    fclose(fptr);
    sprintf(response, "HTTP/1.1 201 Created\r\nContent-Location: %s\r\n\r\n", resource_path);

    return response;
}

char * generate_delete_response(const struct http_request request, const char * header){
        FILE *fptr;
    char resource_path[256] = "resources";
    const char *suffix = ".html";
    if(strcmp(request.address,"/")==0 || strcmp(request.address,"/index")==0){ //don't allow 
        return generate_forbidden_response();
    }else{
        strcat(resource_path, request.address);
        strcat(resource_path, suffix);
    }

    if(!fileExists(resource_path)){
        return generate_not_found_response();
    }
        fptr = fopen(resource_path,"r" );

    if(fptr == NULL){        
        return generate_not_found_response();
    }

    fseek(fptr, 0, SEEK_END);
    long file_size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    // Allocate memory to read the file
    char *file_content = (char *)malloc(file_size + 1);

    if (file_content == NULL) {
        fclose(fptr);
        return generate_internal_error_response();
    }


    fread(file_content, 1, file_size, fptr);
    file_content[file_size] = '\0';  

    fclose(fptr);

    char *response = (char *)malloc(300 + file_size);
    
    if (response == NULL) {
        free(file_content);
        return generate_internal_error_response();
    }

    if (remove(resource_path) != 0) {
        return generate_internal_error_response(); //couldn't delete resource
    } 

    sprintf(response, "HTTP/1.1 200 OK\r\nAccept-Ranges: bytes\r\nContent-Length: %ld\r\nContent-Type: text/html\r\n\r\n%s", file_size, file_content);
    
    return response;
}

char * create_path(char* input_string) {
    char* copy = strdup(input_string);
    char* token = strtok(copy, "/");
    char* lastWord = NULL;
    char folderPath[256];

    // Initialize folderPath with the current working directory
    snprintf(folderPath, sizeof(folderPath), ".");

    while (token != NULL) {
        if (lastWord != NULL) {
            // Concatenate the current token to the folderPath
            strncat(folderPath, "/", sizeof(folderPath) - strlen(folderPath) - 1);
            strncat(folderPath, lastWord, sizeof(folderPath) - strlen(folderPath) - 1);

            // Create or check if the folder already exists
            struct stat st = {0};
            if (stat(folderPath, &st) == -1) {
                mkdir(folderPath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            }
        }

        lastWord = strdup(token);
        token = strtok(NULL, "/");
    }

    free(copy);

    return input_string;
}

char * generate_method_not_allowed_response(){
    return strdup("HTTP/1.1 405 Method Not Allowed\r\n\r\n");
}

char * generate_bad_request_response(){
    return strdup("HTTP/1.1 400 Bad Request\r\n\r\n");
}

char * generate_internal_error_response(){
    return strdup("HTTP/1.1 500 Internal Server Error\r\n\r\n");
}

char * generate_forbidden_response(){
    return strdup("HTTP/1.1 403 Forbidden\r\n\r\n");
}

char * generate_not_found_response(){
    return strdup("HTTP/1.1 404 Not Found\r\n\r\n");
}
