#ifndef FETCH_H
#define FETCH_h

#include <gtk/gtk.h>
#include <curl/curl.h>
#include <lexbor/html/html.h>

extern void parse_structure(lxb_dom_node_t *node, GtkWidget *container);

static CURL *curl_handle; 

typedef struct{
    GtkWidget *input_field_object;
    GtkWidget *global_grid_object;
    GtkWidget *global_container_object;
} connect_event_data; 

typedef struct {
    char *ptr; 
    size_t len; 
} html_output;

size_t html_output_init(html_output *structure); 

size_t fetch_html_response(void *content, size_t size, size_t nmemb, html_output *html_structure); 

void connect_event(GtkButton *self, gpointer user_data);

#endif 