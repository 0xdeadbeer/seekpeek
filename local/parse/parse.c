#include "parse.h"
#include "../global/environment.h"

GtkWidget *
parse_node(lxb_dom_node_t *node) {
    GtkWidget *output_node = NULL; 
    lxb_tag_id_enum_t local_name = node->local_name;

    if (DEBUG)
        printf("Parsing node with local name: %d\n", local_name);

    switch (local_name) {
        case LXB_TAG_P: 
            output_node = gtk_text_new(); 
            lxb_dom_node_t *child_node = node->first_child;

            if (child_node->local_name != 2)
                break; 
            
            lxb_dom_text_t *child_text_node= lxb_dom_interface_text(child_node);
            lxb_char_t *child_text_data = child_text_node->char_data.data.data;
            size_t child_text_length = child_text_node->char_data.data.length; 

            GtkWidget *text_buffer = gtk_entry_buffer_new(child_text_data, child_text_length); 
            gtk_text_set_buffer(output_node, text_buffer);

        break;
    }

    return output_node; 
}


void 
parse_structure(lxb_dom_node_t *node, GtkWidget *container) {
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