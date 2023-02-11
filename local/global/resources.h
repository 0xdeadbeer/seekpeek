#ifndef RESOURCES_H
#define RESOURCES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int type; 
    void *data;
    void *html_container;

    void *next; 
    void *prev; 
} resource_struct; 

typedef enum {
    UNKNOWN,
    LINK, 
    IMAGE_SRC
} resource_type; 

extern resource_struct *start_resource;
extern resource_struct *end_resource;
resource_struct *create_resource(resource_type type, void *data, size_t data_length, void *html_container); 
void clear_resources();
void clear_resources_by_html_container(void *html_container);

#endif 