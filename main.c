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

GtkWidget *application_grid;
GtkWidget *html_document_container; 
connect_event_data event_data; 

// === GLOBAL GTK ELEMENTS === 

// === GLOBAL === 
 
void activate (GtkApplication *app, gpointer user_data)
{
  GtkWidget *button, *text, *tmp_text;

  GdkCursor *clicked_cursor; 
  clicked_cursor = gdk_cursor_new_from_name("pointer", NULL);

  application_grid = gtk_grid_new(); 
  gtk_window_set_child(GTK_WINDOW(window), application_grid);
  gtk_widget_add_css_class(application_grid, "searchbar");
  gtk_grid_set_column_homogeneous(application_grid, TRUE); 

  text = gtk_text_view_new(); 
  gtk_grid_attach(GTK_GRID(application_grid), text, 0, 0, 5, 1); 
  gtk_widget_add_css_class(text, "search-input");

  event_data.input_field_object = text; 
  event_data.global_grid_object = application_grid; 
  event_data.global_container_object = html_document_container; 

  button = gtk_button_new_with_label("Connect!");
  gtk_grid_attach(GTK_GRID(application_grid), button, 5, 0, 1, 1); 
  gtk_widget_add_css_class(button, "search-button"); 
  g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(connect_event), &event_data);
  gtk_widget_set_cursor(button, clicked_cursor); 

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
  gtk_css_provider_load_from_file(provider, g_file_new_for_path("/home/unknown/projects/seekpeek/style/main.css"));
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
