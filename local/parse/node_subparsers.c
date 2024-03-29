#include <curl/curl.h>
#include <time.h>
#include "parse.h"

#include "../global/environment.h"
#include "../fetch/structs.h"
#include "./node_subparsers/img/img_node_subparser.h"

extern void connect_to_url(GtkButton *self, gpointer user_data);
extern connect_event_data event_data;

GtkWidget *node_subparser_body_tag(lxb_dom_node_t *node) {
    GtkWidget *output_node = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); 
    for (lxb_dom_node_t *child_node = node->first_child; child_node != NULL; child_node = child_node->next) 
        gtk_box_append(output_node, parse_node(child_node));

    return output_node;
}

GtkWidget *node_subparser_text_tag(lxb_dom_node_t *node) {
    lxb_dom_text_t *text_node = lxb_dom_interface_text(node);
    lxb_char_t *node_data = text_node->char_data.data.data;
    size_t data_length = text_node->char_data.data.length;

    if (empty_string(node_data, data_length))     
        return NULL;

    GtkWidget *output_node = gtk_label_new(node_data); 
    gtk_label_set_wrap(output_node, TRUE);
    gtk_label_set_wrap_mode(output_node, PANGO_WRAP_CHAR); 
    gtk_widget_set_halign(output_node, GTK_ALIGN_START);
    gtk_label_set_selectable(output_node, TRUE);
    
    return output_node; 
}

GtkWidget *node_subparser_p_tag(lxb_dom_node_t *node) {
    GtkWidget *output_node = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    for (lxb_dom_node_t *child_node = node->first_child; child_node != NULL; child_node = child_node->next)
        gtk_box_append(output_node, parse_node(child_node));

    return output_node; 
}

GtkWidget *node_subparser_h_tag(lxb_dom_node_t *node) {
    GtkWidget *output_node = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); 
    for (lxb_dom_node_t *child_node = node->first_child; child_node != NULL; child_node = child_node->next)
        gtk_box_append(output_node, parse_node(child_node)); 

    gtk_widget_add_css_class(output_node, "header"); 
    switch (node->local_name) {
        case LXB_TAG_H1: 
            gtk_widget_add_css_class(output_node, "header-1"); 
        break; 
        case LXB_TAG_H2:
            gtk_widget_add_css_class(output_node, "header-2");
        break; 
        case LXB_TAG_H3: 
            gtk_widget_add_css_class(output_node, "header-3");
        break; 
        case LXB_TAG_H4: 
            gtk_widget_add_css_class(output_node, "header-4");
        break; 
        case LXB_TAG_H5: 
            gtk_widget_add_css_class(output_node, "header-5"); 
        break; 
        case LXB_TAG_H6: 
            gtk_widget_add_css_class(output_node, "header-6");
        break; 
    } 

    return output_node; 
}

GtkWidget *node_subparser_b_tag(lxb_dom_node_t *node) {
    GtkWidget *output_node = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); 
    for (lxb_dom_node_t *child_node = node->first_child; child_node != NULL; child_node = child_node->next) 
        gtk_box_append(output_node, parse_node(child_node));    

    gtk_widget_add_css_class(output_node, "bold-text");

    return output_node; 
}

GtkWidget *node_subparser_i_tag(lxb_dom_node_t *node) {
    GtkWidget *output_node = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); 
    for (lxb_dom_node_t *child_node = node->first_child; child_node != NULL; child_node = child_node->next)
        gtk_box_append(output_node, parse_node(child_node));

    gtk_widget_add_css_class(output_node, "italic-text");

    return output_node; 
}

GtkWidget *node_subparser_a_tag(lxb_dom_node_t *node) {
    GtkWidget *output_node; 
    
    GdkCursor *clicked_cursor;
    clicked_cursor = gdk_cursor_new_from_name("pointer", NULL);

    lxb_dom_node_t *child_node = node->first_child;

    if (child_node->local_name != LXB_TAG__TEXT)
        return NULL;
    
    lxb_dom_text_t *child_text_node= lxb_dom_interface_text(child_node);
    lxb_char_t *child_text_data = child_text_node->char_data.data.data;
    size_t child_text_length = child_text_node->char_data.data.length; 

    output_node = gtk_button_new_with_label(child_text_data); 
    gtk_widget_add_css_class(output_node, "a-link");
    gtk_widget_set_cursor(output_node, clicked_cursor); 
    gtk_widget_set_halign(output_node, GTK_ALIGN_START);

    lxb_dom_element_t *element = lxb_dom_interface_element(node); 
    char *link_attr = element->first_attr->value->data;
    size_t attr_length = element->first_attr->value->length;

    int current_page = gtk_notebook_get_current_page(tabs_notebook);
    GtkWidget *scroll_element = gtk_notebook_get_nth_page(tabs_notebook, current_page); 
    GtkWidget *view_port = gtk_scrolled_window_get_child(scroll_element);
    GtkWidget *html_container = gtk_viewport_get_child(view_port);

    resource_struct *url_resource = create_resource(LINK, link_attr, attr_length, html_container); 

    g_signal_connect(output_node, "clicked", connect_to_url, url_resource);

    return output_node; 
} 

GtkWidget *node_subparser_img_tag(lxb_dom_node_t *node) {
    lxb_dom_element_t *element = lxb_dom_interface_element(node); 
    unsigned char *src_attr = element->first_attr->value->data;
    size_t attr_length = element->first_attr->value->length;

    int current_page = gtk_notebook_get_current_page(tabs_notebook);
    GtkWidget *scroll_element = gtk_notebook_get_nth_page(tabs_notebook, current_page); 
    GtkWidget *view_port = gtk_scrolled_window_get_child(scroll_element);
    GtkWidget *html_container = gtk_viewport_get_child(view_port);

    resource_struct *src_resource = create_resource(IMAGE_SRC, src_attr, attr_length, html_container); 

    char *filename = generate_filename(90);
    char *folder = "./resources/"; 
    char *location = malloc(strlen(filename) + strlen(folder) + 1); 

    strcpy(location, folder);
    strcat(location, filename);

    download_picture(src_attr, location); 

    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(location, NULL);
    GtkWidget *output_node = gtk_picture_new_for_pixbuf(pixbuf);

    gtk_widget_add_css_class(output_node, "image");
    gtk_widget_set_halign(output_node, GTK_ALIGN_START);
    gtk_picture_set_can_shrink(output_node, FALSE);

    free(location);
    free(filename);
    return output_node; 
}

GtkWidget *node_subparser_li_tag(lxb_dom_node_t *node) {
    GtkWidget *output_node = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); 
    for (lxb_dom_node_t *child_node = node->first_child; child_node != NULL; child_node = child_node->next)
        gtk_box_append(output_node, parse_node(child_node)); 
    
    return output_node; 
}

GtkWidget *node_subparser_ul_tag(lxb_dom_node_t *node) {
    GtkWidget *output_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); 

    for (lxb_dom_node_t *child_node = node->first_child; child_node != NULL; child_node = child_node->next) {
        GtkWidget *ul_line_content = parse_node(child_node);
        if (ul_line_content == NULL)
            continue;

        GtkWidget *ul_line_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

        char *bullet_point_text = "  • ";
        GtkWidget *bullet_point = gtk_label_new(bullet_point_text);
        gtk_label_set_wrap(bullet_point, TRUE); 
        gtk_label_set_wrap_mode(bullet_point, PANGO_WRAP_CHAR); 
        gtk_widget_set_halign(bullet_point, GTK_ALIGN_START);
        gtk_label_set_selectable(bullet_point, TRUE);

        gtk_box_append(ul_line_box, bullet_point);
        gtk_box_append(ul_line_box, ul_line_content);

        gtk_box_append(output_box, ul_line_box);
    }

    return output_box;
}

GtkWidget *node_subparser_ol_tag(lxb_dom_node_t *node) {
    GtkWidget *output_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); 

    size_t child_iterations = 1;
    for (lxb_dom_node_t *child_node = node->first_child; child_node != NULL; child_node = child_node->next) {
        GtkWidget *ol_line_content = parse_node(child_node); 
        if (ol_line_content == NULL)
            continue;

        GtkWidget *ol_line_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);        

        char *number_symbol_content = NULL;
        asprintf(&number_symbol_content, "  %d. ", child_iterations);

        GtkWidget *number_symbol = gtk_label_new(number_symbol_content);
        gtk_label_set_wrap(number_symbol, TRUE); 
        gtk_label_set_wrap_mode(number_symbol, PANGO_WRAP_CHAR); 
        gtk_widget_set_halign(number_symbol, GTK_ALIGN_START);
        gtk_label_set_selectable(number_symbol, TRUE);

        gtk_box_append(ol_line_box, number_symbol);    
        gtk_box_append(ol_line_box, ol_line_content);

        gtk_box_append(output_box, ol_line_box);
        child_iterations++; 
    }

    return output_box;
}

GtkWidget *node_subparser_br_tag(lxb_dom_node_t *node) {
    return gtk_label_new(NULL);
}

GtkWidget *node_subparser_hr_tag(lxb_dom_node_t *node) {
    GtkWidget *output_node = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_widget_add_css_class(output_node, "separator");
    return output_node;
}

GtkWidget *node_subparser_default(lxb_dom_node_t *node) {
    GtkWidget *output_node = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    for (lxb_dom_node_t *child_node = node->first_child; child_node != NULL; child_node = child_node->next)
        gtk_box_append(output_node, parse_node(child_node));
    return output_node;
}