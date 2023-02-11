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

GtkWindow *window; 
static GtkCssProvider *provider; 
static GdkDisplay *display; 

GtkWidget *application_container, *search_container; 
GtkWidget *scrolled_window, *tabs_notebook; 
GtkWidget *search_input;
// connect_event_data event_data; 

// === GLOBAL GTK ELEMENTS === 

// === GLOBAL === 
 
void shortcut_switch_next_tab() {
  gtk_notebook_next_page(tabs_notebook);
}
  
void shortcut_switch_prev_tab() {
  gtk_notebook_prev_page(tabs_notebook);
}

void shortcut_search_grab_focus() {
  gtk_widget_grab_focus(search_input);
}

void shortcut_create_new_tab() {
  GtkWidget *new_html_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); 
  gtk_widget_add_css_class(new_html_container, "html_container"); 

  GtkWidget *scrolled_container = gtk_scrolled_window_new ();
  gtk_scrolled_window_set_child(scrolled_container, new_html_container);

  gtk_scrolled_window_set_min_content_height(scrolled_container, 600);
  gtk_widget_set_vexpand(scrolled_container, TRUE);

  int page_index = gtk_notebook_append_page(tabs_notebook, scrolled_container, NULL);
  gtk_notebook_set_current_page(tabs_notebook, page_index);
}

void shortcut_delete_current_tab() {
  int current_page = gtk_notebook_get_current_page(tabs_notebook);
  GtkWidget *scroll_element = gtk_notebook_get_nth_page(tabs_notebook, current_page); 
  GtkWidget *view_port = gtk_scrolled_window_get_child(scroll_element);
  GtkWidget *html_container = gtk_viewport_get_child(view_port);

  clear_resources_by_html_container(html_container);
  gtk_notebook_remove_page(tabs_notebook, current_page);

  // create a new page if there's 0 left
  int number_of_tabs = gtk_notebook_get_n_pages(tabs_notebook);
  if (number_of_tabs <= 0)
    shortcut_create_new_tab();
}

void activate (GtkApplication *app, gpointer user_data)
{
  GdkCursor *clicked_cursor; 
  clicked_cursor = gdk_cursor_new_from_name("pointer", NULL);

  application_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); 
  search_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5); 
  search_input = gtk_entry_new(); 

  gtk_window_set_child(GTK_WINDOW(window), GTK_BOX(application_container));
  gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
  gtk_box_append(GTK_BOX(application_container), GTK_BOX(search_container)); 

  gtk_widget_add_css_class(search_container, "search_bar");
  gtk_widget_set_halign(search_container, GTK_ALIGN_CENTER); 

  gtk_box_append(GTK_BOX(search_container), search_input); 
  gtk_widget_add_css_class(search_input, "search_input");
  gtk_widget_set_size_request(search_input, 1000, 20);
  g_signal_connect(G_OBJECT(search_input), "activate", G_CALLBACK(connect_event), search_input);
  gtk_entry_set_placeholder_text(search_input, "Connection URL:");

  tabs_notebook = gtk_notebook_new();   
  gtk_widget_add_css_class(tabs_notebook, "tabs_notebook"); 
  gtk_widget_set_vexpand(tabs_notebook, FALSE);
  gtk_box_append(GTK_BOX(application_container), tabs_notebook); 
  gtk_notebook_set_scrollable(tabs_notebook, TRUE);

  // create initial tab
  GtkWidget *html_document_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); 
  gtk_widget_add_css_class(html_document_container, "html_container"); 

  scrolled_window = gtk_scrolled_window_new ();
  gtk_scrolled_window_set_child(scrolled_window, html_document_container);

  gtk_scrolled_window_set_min_content_height(scrolled_window, 600);
  gtk_widget_set_vexpand(scrolled_window, TRUE);
  gtk_widget_set_vexpand(tabs_notebook, TRUE);

  gtk_notebook_append_page(tabs_notebook, scrolled_window, NULL);
  gtk_notebook_popup_enable(tabs_notebook);  

  gtk_window_present (GTK_WINDOW (window));
  gtk_widget_show(window);
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

  // setup shortcuts
  gtk_widget_class_add_binding(GTK_WIDGET_GET_CLASS(window), GDK_KEY_l, GDK_CONTROL_MASK, shortcut_switch_next_tab, NULL);
  gtk_widget_class_add_binding(GTK_WIDGET_GET_CLASS(window), GDK_KEY_h, GDK_CONTROL_MASK, shortcut_switch_prev_tab, NULL);
  gtk_widget_class_add_binding(GTK_WIDGET_GET_CLASS(window), GDK_KEY_p, GDK_CONTROL_MASK, shortcut_search_grab_focus, NULL);
  gtk_widget_class_add_binding(GTK_WIDGET_GET_CLASS(window), GDK_KEY_t, GDK_CONTROL_MASK, shortcut_create_new_tab, NULL);
  gtk_widget_class_add_binding(GTK_WIDGET_GET_CLASS(window), GDK_KEY_w, GDK_CONTROL_MASK, shortcut_delete_current_tab, NULL);
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
