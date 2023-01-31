#ifndef RESOURCES_H
#define RESOURCES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int type; 
    void *data;

    void *next; 
    void *prev; 
} resource_struct; 

typedef enum {
    UNKNOWN,
    LINK
} resource_type; 

extern resource_struct *resources;
resource_struct *create_resource(resource_type type, void *data, size_t data_length); 
int delete_resource(void *addr); 

#endif 