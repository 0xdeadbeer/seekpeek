#ifndef ATTR_SUBPARSERS_H 
#define ATTR_SUBPARSERS_H 

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <lexbor/html/html.h>
#include <lexbor/dom/interfaces/text.h>

void attr_subparser_p_tag(GtkWidget *widget, lxb_dom_node_t *node);
void attr_subparser_h_tag(GtkWidget *widget, lxb_dom_node_t *node);
void attr_subparser_b_tag(GtkWidget *widget, lxb_dom_node_t *node); 
void attr_subparser_i_tag(GtkWidget *widget, lxb_dom_node_t *node); 
void attr_subparser_a_tag(GtkWidget *widget, lxb_dom_node_t *node); 
void attr_subparser_img_tag(GtkWidget *widget, lxb_dom_node_t *node); 

#endif 