#include <gtk/gtk.h>
#include <curl/curl.h>
#include <lexbor/html/html.h>
#include <lexbor/dom/interfaces/text.h>
#include <stdio.h>
#include <string.h>
#include "local/parse/parse.h"
#include "local/fetch/fetch.h"

// === GLOBAL === 

#define DEBUG TRUE

// === GLOBAL GTK ELEMENTS === 

static GtkWindow *window; 
static GtkCssProvider *provider; 
static GdkDisplay *display; 

GtkWidget *application_container, *search_container; 
GtkWidget *html_document_container; 
// connect_event_data event_data; 

// === GLOBAL GTK ELEMENTS === 

// === GLOBAL === 
 
void activate (GtkApplication *app, gpointer user_data)
{
  GtkWidget *search_button, *search_input;

  GdkCursor *clicked_cursor; 
  clicked_cursor = gdk_cursor_new_from_name("pointer", NULL);

  application_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); 
  search_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5); 
  search_input = gtk_entry_new(); 
  search_button = gtk_button_new_with_label("Connect!");

  gtk_window_set_child(GTK_WINDOW(window), GTK_BOX(application_container));
  gtk_box_append(GTK_BOX(application_container), search_container); 

  gtk_widget_add_css_class(search_container, "search_bar");
  gtk_widget_set_halign(search_container, GTK_ALIGN_CENTER); 

  gtk_box_append(GTK_BOX(search_container), search_input); 
  gtk_widget_add_css_class(search_input, "search_input");
  gtk_widget_set_size_request(search_input, 800, 20);

  gtk_box_append(GTK_BOX(search_container), search_button); 
  gtk_widget_add_css_class(search_button, "search_button"); 
  g_signal_connect(G_OBJECT(search_button), "clicked", G_CALLBACK(connect_event), search_input);
  gtk_widget_set_cursor(search_button, clicked_cursor); 
  gtk_widget_set_size_request(search_button, 200, 20);

  gtk_window_present (GTK_WINDOW (window));
}

void startup(GApplication *application) 
{
  GtkApplication *app = GTK_APPLICATION(application); 
  
  // setup window 
  window = gtk_application_window_new(app); 
  gtk_window_set_title(GTK_WINDOW(window), "Seekpeek"); 
  gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);

  // setup display
  display = gtk_widget_get_display(GTK_WIDGET(window)); 
  provider = gtk_css_provider_new(); 
  gtk_css_provider_load_from_file(provider, g_file_new_for_path("./style/main.css"));
  gtk_style_context_add_provider_for_display(display, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

int main (int argc, char **argv)
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
