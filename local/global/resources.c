#include "resources.h"

resource_struct *start_resource = NULL;
resource_struct *end_resource = NULL;

resource_struct *create_resource(resource_type type, void *data, size_t data_length, void *html_container) {
    resource_struct *new_resource = malloc(sizeof(resource_struct)); 
    new_resource->type = type; 
    new_resource->prev = NULL; 
    new_resource->next = NULL;
    new_resource->html_container = html_container;
    
    new_resource->data = malloc(data_length+1);
    char *resource_data = new_resource->data; 
    memcpy(resource_data, data, data_length);

    resource_data[data_length] = '\0';

    if (end_resource != NULL) {
        end_resource->next = new_resource; 
        new_resource->prev = end_resource; 
    } else {
        start_resource = new_resource;
    }

    end_resource = new_resource; 

    return new_resource; 
}

void clear_resources() {
    resource_struct *current_resource = end_resource;
    resource_struct *temporary_resource; 

    while (current_resource != NULL) {
        if (current_resource->prev != NULL)
            temporary_resource = current_resource->prev; 
        else temporary_resource = NULL;

        free(current_resource);
        current_resource = temporary_resource;
    }

    end_resource = NULL; 
    start_resource = NULL;
}

void clear_resource(resource_struct *resource) {
    resource_struct *next_resource = resource->next;
    resource_struct *prev_resource = resource->prev; 

    if (prev_resource != NULL)
        prev_resource->next = next_resource; 
    if (next_resource != NULL)
        next_resource->prev = prev_resource;

    if (resource == start_resource)
        start_resource = resource->next; 

    free(resource->data); 
    free(resource);
}

void clear_resources_by_html_container(void *html_container) {
    resource_struct *current_resource = start_resource; 
    resource_struct *temporary_resource; 

    while (current_resource != NULL) {
        temporary_resource = current_resource->next; 

        if (current_resource->html_container == html_container)
            clear_resource(current_resource);

        current_resource = temporary_resource; 
    }
}