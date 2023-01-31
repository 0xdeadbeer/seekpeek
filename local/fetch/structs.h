#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct{
    GtkWidget *search_field;
    GtkWidget *application_container;
    GtkWidget *html_container;
    
    char *url;
} connect_event_data; 

typedef struct {
    char *ptr; 
    size_t len; 
} html_output;

#endif