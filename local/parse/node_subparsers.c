#include <curl/curl.h>
#include <time.h>
#include "parse.h"
#include "../global/environment.h"
#include "../fetch/structs.h"

extern void connect_to_url(GtkButton *self, gpointer user_data);
extern connect_event_data event_data;

GtkWidget *subparser_p_tag(lxb_dom_node_t *node) {
    GtkWidget *output_node = gtk_text_view_new(); 
    lxb_dom_node_t *child_node = node->first_child;

    if (child_node->local_name != LXB_TAG__TEXT)
        return NULL;

    lxb_dom_text_t *child_text_node= lxb_dom_interface_text(child_node);
    lxb_char_t *child_text_data = child_text_node->char_data.data.data;
    size_t child_text_length = child_text_node->char_data.data.length; 

    GtkWidget *text_buffer = gtk_text_buffer_new(NULL); 

    gtk_text_buffer_set_text(text_buffer, child_text_data, child_text_length);
    gtk_text_view_set_buffer(output_node, text_buffer);
    gtk_text_view_set_editable(output_node, (gboolean) FALSE);
    gtk_text_view_set_cursor_visible(output_node, (gboolean) FALSE);
    gtk_text_view_set_wrap_mode(output_node, GTK_WRAP_CHAR); 

    return output_node; 
}

GtkWidget *subparser_h_tag(lxb_dom_node_t *node) {
    GtkWidget *output_node = gtk_text_view_new(); 
    lxb_dom_node_t *child_node = node->first_child;

    if (child_node->local_name != LXB_TAG__TEXT)
        return NULL;
    
    lxb_dom_text_t *child_text_node= lxb_dom_interface_text(child_node);
    lxb_char_t *child_text_data = child_text_node->char_data.data.data;
    size_t child_text_length = child_text_node->char_data.data.length; 

    GtkWidget *text_buffer = gtk_text_buffer_new(NULL); 

    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(text_buffer), child_text_data, child_text_length);
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(output_node), GTK_TEXT_BUFFER(text_buffer));
    gtk_text_view_set_editable(GTK_TEXT_VIEW(output_node), (gboolean) FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(output_node), (gboolean) FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(output_node), GTK_WRAP_CHAR); 

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

GtkWidget *subparser_b_tag(lxb_dom_node_t *node) {
    GtkWidget *output_node = gtk_text_view_new(); 
    lxb_dom_node_t *child_node = node->first_child;

    if (child_node->local_name != LXB_TAG__TEXT)
        return NULL;
    
    lxb_dom_text_t *child_text_node= lxb_dom_interface_text(child_node);
    lxb_char_t *child_text_data = child_text_node->char_data.data.data;
    size_t child_text_length = child_text_node->char_data.data.length; 

    GtkWidget *text_buffer = gtk_text_buffer_new(NULL); 

    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(text_buffer), child_text_data, child_text_length);
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(output_node), GTK_TEXT_BUFFER(text_buffer));
    gtk_text_view_set_editable(GTK_TEXT_VIEW(output_node), (gboolean) FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(output_node), (gboolean) FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(output_node), GTK_WRAP_CHAR); 
    gtk_widget_add_css_class(output_node, "bold-text");

    return output_node; 
}

GtkWidget *subparser_i_tag(lxb_dom_node_t *node) {
    GtkWidget *output_node = gtk_text_view_new(); 
    lxb_dom_node_t *child_node = node->first_child;

    if (child_node->local_name != LXB_TAG__TEXT)
        return NULL;
    
    lxb_dom_text_t *child_text_node= lxb_dom_interface_text(child_node);
    lxb_char_t *child_text_data = child_text_node->char_data.data.data;
    size_t child_text_length = child_text_node->char_data.data.length; 

    GtkWidget *text_buffer = gtk_text_buffer_new(NULL); 

    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(text_buffer), child_text_data, child_text_length);
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(output_node), GTK_TEXT_BUFFER(text_buffer));
    gtk_text_view_set_editable(GTK_TEXT_VIEW(output_node), (gboolean) FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(output_node), (gboolean) FALSE);
    gtk_widget_add_css_class(output_node, "italic-text");
    gtk_text_buffer_set_text(text_buffer, child_text_data, child_text_length);

    return output_node; 
}

GtkWidget *subparser_a_tag(lxb_dom_node_t *node) {
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

    resource_struct *url_resource = create_resource(LINK, link_attr, attr_length); 

    g_signal_connect(output_node, "clicked", connect_to_url, url_resource);

    return output_node; 
} 

size_t save_picture(void *ptr, size_t size, size_t nmemb, void *stream) {
    size_t written = fwrite(ptr, size, nmemb, (FILE *) stream);
    return written;
}

void download_image(char *url, char *location) {
    CURL *curl_handle = curl_easy_init(); 
    FILE *pagefile; 

    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, save_picture); 

    pagefile = fopen(location, "wb");   
    if (pagefile) {
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pagefile); 
        curl_easy_perform(curl_handle); 
        fclose(pagefile);
    }

    curl_easy_cleanup(curl_handle);

    return 1;
}

char *generate_filename(int length) {
    srand(time(NULL) + clock() + getpid());

    char *string = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const size_t string_length = 62;

	char *result = malloc(sizeof(char) * (length +1));
	if(!result) {
		return (char*)0;
	}
	unsigned int key = 0;
	for(int index = 0;index<length;index++) {
		key = rand() % string_length;
		result[index] = string[key];
	}
	result[length] = '\0';
	return result;
}

GtkWidget *subparser_img_tag(lxb_dom_node_t *node) {

    lxb_dom_element_t *element = lxb_dom_interface_element(node); 
    unsigned char *src_attr = element->first_attr->value->data;
    size_t attr_length = element->first_attr->value->length;

    resource_struct *src_resource = create_resource(IMAGE_SRC, src_attr, attr_length); 

    char *filename = generate_filename(90);
    char *folder = "./resources/"; 
    char *location = malloc(strlen(filename) + strlen(folder) + 1); 

    strcpy(location, folder);
    strcat(location, filename);

    download_image(src_attr, location); 

    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(location, NULL);

    GtkWidget *output_node = gtk_picture_new_for_pixbuf(pixbuf);

    gtk_widget_add_css_class(output_node, "image");
    gtk_widget_set_halign(output_node, GTK_ALIGN_START);
    gtk_picture_set_can_shrink(output_node, FALSE);

    free(location);
    free(filename);
    return output_node; 
}

GtkWidget *subparser_div_tag(lxb_dom_node_t *node) {

}