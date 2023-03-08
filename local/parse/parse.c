#include "parse.h"
#include "../global/environment.h"

GtkWidget *parse_node(lxb_dom_node_t *node) {
    GtkWidget *output_node = NULL; 
    lxb_tag_id_enum_t local_name = node->local_name;

    if (DEBUG)
        printf("Parsing node with local name: %d\n", local_name);

    switch (local_name) {
        case LXB_TAG_P: 
            output_node = node_subparser_p_tag(node); 
        break;

        case LXB_TAG_H1: 
        case LXB_TAG_H2: 
        case LXB_TAG_H3: 
        case LXB_TAG_H4: 
        case LXB_TAG_H5: 
        case LXB_TAG_H6: 
            output_node = node_subparser_h_tag(node);
        break;

        case LXB_TAG_B: 
            output_node = node_subparser_b_tag(node); 
        break; 

        case LXB_TAG_I: 
            output_node = node_subparser_i_tag(node);
        break; 

        case LXB_TAG_A: 
            output_node = node_subparser_a_tag(node); 
        break; 

        case LXB_TAG_IMG: 
            output_node = node_subparser_img_tag(node);
        break; 

    }

    return output_node; 
}


void parse_structure(lxb_dom_node_t *node, GtkWidget *container) {
    if (DEBUG) 
        printf("Node: %d\n", node->local_name); 

    GtkWidget *parsed_node = parse_node(node); 
    if (parsed_node != NULL)
        gtk_box_append(GTK_BOX(container), parsed_node);

    if (node->first_child != NULL) 
        parse_structure(node->first_child, container); 
    
    if (node->next != NULL) 
        parse_structure(node->next, container);
}