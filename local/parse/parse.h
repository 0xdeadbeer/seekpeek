#ifndef PARSE_H
#define PARSE_H

#include <gtk/gtk.h>
#include <lexbor/html/html.h>
#include <lexbor/dom/interfaces/text.h>

GtkWidget *
parse_node(lxb_dom_node_t *node); 

void 
parse_structure(lxb_dom_node_t *node, GtkWidget *container); 

#endif 