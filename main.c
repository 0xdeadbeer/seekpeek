#include <gtk/gtk.h>

static GtkWidget *window; 
static GtkCssProvider *provider; 
static GdkDisplay *display; 

static void activate (GtkApplication *app, gpointer user_data)
{
  GtkWidget *grid; 
  GtkWidget *button; 
  GtkWidget *text;

  grid = gtk_grid_new(); 
  gtk_window_set_child(GTK_WINDOW(window), grid);
  gtk_widget_add_css_class(grid, "searchbar");

  text = gtk_text_new(); 
  gtk_grid_attach(GTK_GRID(grid), text, 0, 0, 1, 1); 
  gtk_widget_add_css_class(text, "search-input");
  gtk_text_set_placeholder_text(text, "query"); 

  button = gtk_button_new_with_label("Search!");
  gtk_grid_attach(GTK_GRID(grid), button, 1, 0, 1, 1); 
  gtk_widget_add_css_class(button, "search-button"); 

  gtk_window_present (GTK_WINDOW (window));
}

static void startup(GApplication *application) {
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

  app = gtk_application_new ("org.osamu-sp.seekpeek", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "startup", G_CALLBACK(startup), NULL);
  g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
