#include "resources.h"

resource_struct *resources;

resource_struct *create_resource(resource_type type, void *data, size_t data_length) {
    resource_struct *new_resource = malloc(sizeof(resource_struct)); 
    new_resource->type = type; 
    
    new_resource->data = malloc(data_length+1);
    char *resource_data = new_resource->data; 
    memcpy(resource_data, data, data_length);

    resource_data[data_length] = '\0';

    if (resources != NULL) {
        resources->next = new_resource; 
        new_resource->prev = resources; 
    }

    resources = new_resource; 
    return new_resource; 
}

int delete_resource(void *addr) {
    resource_struct *resource = resources; 
    while (resource != (resource_struct *) addr) {
        if (resources->prev == NULL) return -1; 
        resource = (resource_struct *) resources->prev; 
    }

    if (resource->next != NULL) {
        resource_struct *next_resource = (resource_struct *) resource->next;
        next_resource->prev = (resource_struct *) resource->prev; 
    }
    
    if (resource->prev != NULL) {
        resource_struct *prev_resource = (resource_struct *) resource->prev; 
        prev_resource->next = (resource_struct *) resource->next; 
    }

    free(resource);
    return 0; 
}