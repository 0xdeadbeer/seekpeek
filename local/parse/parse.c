#include "parse.h"
#include "../global/environment.h"

GtkWidget *parse_node(lxb_dom_node_t *node) {
    GtkWidget *output_node = NULL; 
    lxb_tag_id_enum_t local_name = node->local_name;

    if (DEBUG)
        printf("Parsing node with local name: %d\n", local_name);

    switch (local_name) {
        case LXB_TAG_P: 
            output_node = subparser_p_tag(node); 
        break;
    }

    return output_node; 
}


void parse_structure(lxb_dom_node_t *node, GtkWidget *container) {
    if (DEBUG) 
        printf("Node: %d\n", node->local_name); 

    GtkText *parsed_node = parse_node(node); 
    if (parsed_node != NULL)
        gtk_box_append(container, parsed_node);

    if (node->first_child != NULL) 
        parse_structure(node->first_child, container); 
    
    if (node->next != NULL) 
        parse_structure(node->next, container);
}