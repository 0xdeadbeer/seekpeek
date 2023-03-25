#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <gtk/gtk.h>

extern GtkWindow *window;

void gui_alert(GtkMessageType text_type, GtkButtonsType buttons, char *text); 
int empty_string(const char *s, size_t len);

#endif 