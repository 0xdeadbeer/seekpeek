#include "./global.h"

void gui_alert(GtkMessageType text_type, GtkButtonsType buttons, char *text) {
    GtkWidget *dialog = gtk_message_dialog_new (window,
                                    GTK_DIALOG_MODAL,
                                    text_type,
                                    buttons,
                                    text);

    g_signal_connect(dialog, "response", G_CALLBACK(gtk_window_destroy), NULL);

    gtk_widget_show(GTK_DIALOG(dialog));
}

int empty_string(const char *s, size_t len) {
    for (int c = 0; c < len; c++)
        if (!isspace(*(s+c)))
            return 0;
    
    return 1;
}