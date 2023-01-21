#include <gtk/gtk.h>
#include <curl/curl.h>
#include <lexbor/html/html.h>
#include <lexbor/dom/interfaces/text.h>
#include <stdio.h>
#include <string.h>

// === GLOBAL === 
#define DEBUG TRUE
static CURL *curl_handle; 

static GtkWindow *window; 
static GtkCssProvider *provider; 
static GdkDisplay *display; 

struct html_output_string {
  char *ptr;
  size_t len; 
}; 

// === GLOBAL GTK ELEMENTS === 
GtkWidget *application_grid;
GtkWidget *html_document_container; 

// === GLOBAL === 
static size_t 
html_output_string_init(struct html_output_string *structure) 
{
  structure->len = 0; 
  structure->ptr = malloc(structure->len+1); 

  if (structure->ptr == NULL)
    return 0; 

  structure->ptr[0] = '\0';
  
  return 1; 
}

static size_t 
get_html_response(void *contents, size_t size, size_t nmemb, struct html_output_string *html_output_structure) 
{
  html_output_structure->ptr = realloc(html_output_structure->ptr, html_output_structure->len+(nmemb * size) + 1); 
  if (html_output_structure->ptr == NULL) {
    printf("Memory allocation failed!\n"); 
    return 0; 
  }

  strncpy(&(html_output_structure->ptr[html_output_structure->len]), contents, size * nmemb);
  html_output_structure->ptr[html_output_structure->len + (nmemb * size) + 1] = '\0';
  html_output_structure->len += (nmemb * size); 

  return size * nmemb; 
}

GtkWidget *
parse_node(struct lxb_dom_node *node) {
  GtkWidget *output_node = NULL;
  lxb_tag_id_enum_t node_local_name = *(&node->local_name); 

  if (DEBUG) printf("Parsing node local name: %d\n", node_local_name); 
  
  switch (node_local_name) {
    case LXB_TAG_P: 
      output_node = gtk_text_new(); 

      struct lxb_dom_node *child_node = *(&node->first_child); 

      if (*(&child_node->local_name) != 2) 
        break; 

      struct lxb_dom_text *text_node = lxb_dom_interface_text(child_node); 
      lxb_char_t *child_text = *(&text_node->char_data.data.data);
      size_t child_text_length = *(&text_node->char_data.data.length);
    
      GtkWidget *text_buffer = gtk_entry_buffer_new((char*) child_text, child_text_length);
      gtk_text_set_buffer(output_node, text_buffer); 

      break; 
  }


  return output_node; 
}

void
serialize(lxb_dom_node_t *node, size_t indentation_layer) {
  struct lxb_dom_node *main_node = node; 

  if (DEBUG) printf("Node: %d\n", *(&main_node->local_name));

  GtkText *new_node = parse_node(main_node); 

  if (new_node != NULL)
    gtk_box_append(html_document_container, new_node);

  if (main_node->first_child != NULL) 
    serialize(*(&main_node->first_child), indentation_layer++); 

  if (main_node->next != NULL)
    serialize(*(&main_node->next), indentation_layer); 
}

void 
connect_event(GtkButton *self, gpointer user_data) 
{
  size_t status; 
  GtkWidget *dialog, *label, *content_area; 

  GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(user_data); 
  int chars = gtk_text_buffer_get_char_count(text_buffer);
  GtkTextIter start_iter, end_iter; 
  
  gtk_text_buffer_get_start_iter(text_buffer, &start_iter); 
  gtk_text_buffer_get_end_iter(text_buffer, &end_iter);

  char *url = gtk_text_buffer_get_text(text_buffer, &start_iter, &end_iter, FALSE);
  struct html_output_string html_output; 

  status = html_output_string_init(&html_output); 
  if (status == 0) {
    printf("Error initializing the html output save buffer!\n");
    return; 
  }
    
  curl_handle = curl_easy_init();
  curl_easy_setopt(curl_handle, CURLOPT_URL, url); 
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, get_html_response); 
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &html_output); 
  status = curl_easy_perform(curl_handle); 
  curl_easy_cleanup(curl_handle);

  if (CURLE_OK != status) {
    printf("Failed fetching the website\n");
    return; 
  } 

  lxb_html_document_t *document = lxb_html_document_create();

  if (document == NULL) {
    printf("Failed creating a document\n");
    return; 
  }

  status = lxb_html_document_parse(document, html_output.ptr, html_output.len); 
  if (status != LXB_STATUS_OK) {
    printf("Failed to parse HTML\n");
    return;
  }

  gtk_grid_remove(GTK_GRID(application_grid), html_document_container); 
  html_document_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5); 
  gtk_grid_attach(GTK_GRID(application_grid), html_document_container, 0, 1, 6, 6);

  serialize(lxb_dom_interface_node(document), 1);

  lxb_html_document_destroy(document);
}

static void 
activate (GtkApplication *app, gpointer user_data)
{
  GtkWidget *button, *text;

  GdkCursor *clicked_cursor; 
  clicked_cursor = gdk_cursor_new_from_name("pointer", NULL);

  application_grid = gtk_grid_new(); 
  gtk_window_set_child(GTK_WINDOW(window), application_grid);
  gtk_widget_add_css_class(application_grid, "searchbar");
  gtk_grid_set_column_homogeneous(application_grid, TRUE); 

  text = gtk_text_view_new(); 
  gtk_grid_attach(GTK_GRID(application_grid), text, 0, 0, 5, 1); 
  gtk_widget_add_css_class(text, "search-input");

  button = gtk_button_new_with_label("Connect!");
  gtk_grid_attach(GTK_GRID(application_grid), button, 5, 0, 1, 1); 
  gtk_widget_add_css_class(button, "search-button"); 
  g_signal_connect(button, "clicked", G_CALLBACK(connect_event), text);
  gtk_widget_set_cursor(button, clicked_cursor); 

  gtk_window_present (GTK_WINDOW (window));
}

static void 
startup(GApplication *application) 
{
  GtkApplication *app = GTK_APPLICATION(application); 
  
  // setup window 
  window = gtk_application_window_new(app); 
  gtk_window_set_title(GTK_WINDOW(window), "Seekpeek"); 
  gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);

  // setup display
  display = gtk_widget_get_display(GTK_WIDGET(window)); 
  provider = gtk_css_provider_new(); 
  gtk_css_provider_load_from_file(provider, g_file_new_for_path("/home/unknown/projects/seekpeek/style/main.css"));
  gtk_style_context_add_provider_for_display(display, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

int 
main (int argc, char **argv)
{
  GtkApplication *app;
  int status;

  // perform regular startup
  curl_global_init(CURL_GLOBAL_ALL); 

  app = gtk_application_new ("org.osamu-sp.seekpeek", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "startup", G_CALLBACK(startup), NULL);
  g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);

  // perform regular cleanup
  g_object_unref (app);
  curl_global_cleanup(); 
  return status;
}
