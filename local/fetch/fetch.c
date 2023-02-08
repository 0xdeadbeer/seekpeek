#include "fetch.h"

size_t html_output_init(html_output *structure) {
    structure->len = 0; 
    structure->ptr = malloc(structure->len+1); 

    if (&structure->ptr == NULL)
        return 1; 

    structure->ptr[0] = '\0';  

    return 0; 
}

size_t fetch_html_response(void *content, size_t size, size_t nmemb, html_output *html_structure) {
    html_structure->ptr = realloc(html_structure->ptr, html_structure->len+(size*nmemb)+1);
    if (html_structure->ptr == NULL)
        return 1; 

    strncpy(&(html_structure->ptr[html_structure->len]), content, size*nmemb);
    html_structure->ptr[html_structure->len+(size*nmemb)+1] = '\0'; 
    html_structure->len += (size*nmemb);

    return size * nmemb;
}

char *fetch_text_buffer_data(GtkTextBuffer *text_buffer) {
    GtkTextIter text_buffer_start, text_buffer_end; 

    gtk_text_buffer_get_start_iter(text_buffer, &text_buffer_start); 
    gtk_text_buffer_get_end_iter(text_buffer, &text_buffer_end); 

    return gtk_text_buffer_get_text(text_buffer, &text_buffer_start, &text_buffer_end, FALSE);
}

void connect_to_url(GtkWidget *self, gpointer user_data) {
    resource_struct *resource = (resource_struct *) user_data;
    if (resource->type != LINK) return;     

    int status; 
    html_output html_response; 
    CURL *curl_handle; 

    status = html_output_init(&html_response); 
    if (status != 0) {
        gui_alert(window, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Failed to initialize the HTML output buffer");
        return; 
    }

    curl_handle = curl_easy_init(); 
    curl_easy_setopt(curl_handle, CURLOPT_URL, (char *) resource->data); 
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, fetch_html_response); 
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &html_response); 

    status = curl_easy_perform(curl_handle);
    curl_easy_cleanup(curl_handle); 
    if (CURLE_OK != status) {
        gui_alert(window, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Failed fetching the website");
        return;
    }

    lxb_html_document_t *document = lxb_html_document_create(); 
    if (document == NULL) {
        gui_alert(window, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Failed creating a document");
        return; 
    }
    
    status = lxb_html_document_parse(document, html_response.ptr, html_response.len);
    if (status != LXB_STATUS_OK) {
        gui_alert(window, GTK_MESSAGE_ERROR , GTK_BUTTONS_CLOSE, "Failed to parse HTML"); 
        return; 
    }

    GtkWidget *child = gtk_widget_get_first_child(html_document_container);
    while (child != NULL) {
        gtk_box_remove(html_document_container, child); 
        child = gtk_widget_get_first_child(html_document_container);
    }

    clear_resources(); 
    parse_structure(lxb_dom_interface_node(document), html_document_container);
 
    lxb_html_document_destroy(document);
}

void connect_event(GtkButton *self, gpointer user_data) {
    GtkWidget *search_input = (GtkWidget *) user_data; 
    GtkEntryBuffer *text_buffer = gtk_entry_get_buffer(search_input);
    char *url = gtk_entry_buffer_get_text(text_buffer); 
    size_t url_length = gtk_entry_buffer_get_length(text_buffer);

    resource_struct *url_resource = create_resource(LINK, url, url_length);
    connect_to_url(self, url_resource);
}