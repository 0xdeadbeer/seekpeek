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

void connect_event(GtkButton *self, gpointer user_data) {
    connect_event_data *event_data = (connect_event_data *) user_data; 
    GtkWidget *dialog, *label, *content_area; 
    int status; 

    GtkEntryBuffer *text_buffer = gtk_entry_get_buffer(event_data->search_field);
    char *url = gtk_entry_buffer_get_text(text_buffer); 

    html_output html_response; 
    status = html_output_init(&html_response); 
    if (status != 0) {
        printf("Error initializing the html output buffer!\n");
        return; 
    }

    curl_handle = curl_easy_init(); 
    curl_easy_setopt(curl_handle, CURLOPT_URL, url); 
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, fetch_html_response); 
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &html_response); 

    status = curl_easy_perform(curl_handle);
    curl_easy_cleanup(curl_handle); 
    if (CURLE_OK != status) {
        printf("Failed fetching the website\n");
        printf("Error: %s\n", curl_easy_strerror(status));
        return;
    }

    lxb_html_document_t *document = lxb_html_document_create(); 
    if (document == NULL) {
        printf("Failed creating a document\n");
        return; 
    }
    
    status = lxb_html_document_parse(document, html_response.ptr, html_response.len);
    if (status != LXB_STATUS_OK) {
        printf("Failed to parse HTML\n");
        return; 
    }

    gtk_box_remove(event_data->application_container, event_data->html_container);
    event_data->html_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5); 
    gtk_widget_add_css_class(event_data->html_container, "html_container"); 
    gtk_widget_set_size_request(event_data->html_container, 1000, 600);
    gtk_widget_set_halign(event_data->html_container, GTK_ALIGN_CENTER); 
    gtk_box_append(event_data->application_container, event_data->html_container);

    parse_structure(lxb_dom_interface_node(document), event_data->html_container);

    lxb_html_document_destroy(document);
}