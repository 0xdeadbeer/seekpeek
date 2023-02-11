#ifndef FETCH_H
#define FETCH_h

#include <gtk/gtk.h>
#include <curl/curl.h>
#include <lexbor/html/html.h>
#include "structs.h"
#include "../global/resources.h"
#include "../global/global.h"

extern GtkWidget *tabs_notebook;
extern GtkWidget *application_container; 
extern void parse_structure(lxb_dom_node_t *node, GtkWidget *container);
static CURL *curl_handle; 

size_t html_output_init(html_output *structure); 

size_t fetch_html_response(void *content, size_t size, size_t nmemb, html_output *html_structure); 

void connect_to_url(GtkWidget *self, gpointer user_data);

void connect_event(GtkButton *self, gpointer user_data);

#endif 