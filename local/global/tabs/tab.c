#include "./tab.h"

tab *start_tab = NULL; 
tab *current_tab = NULL; 
tab *end_tab = NULL; 
int tabs = 0; 

tab *append_tab(void *html_container) {
    tab *new_tab = malloc(sizeof(tab));

    new_tab->html_container = html_container;
    new_tab->next_tab = NULL;
    new_tab->prev_tab = NULL;

    if (start_tab == NULL) {
        start_tab = new_tab;
        current_tab = start_tab;
    } else {
        end_tab->next_tab = new_tab; 
        new_tab->prev_tab = end_tab; 
    }

    end_tab = new_tab;
    end_tab->next_tab = start_tab;
    start_tab->prev_tab = end_tab;

    tabs++; 
    return new_tab;
}

void remove_tab(tab *tab_structure) {
    if (tab_structure== NULL) return;

    tab *prev_tab = tab_structure->prev_tab; 
    tab *next_tab = tab_structure->next_tab;
    prev_tab->next_tab = tab_structure->next_tab;
    next_tab->prev_tab = tab_structure->prev_tab;

    free(tab_structure);
    tabs--; 

    if (tabs <= 0) {
        start_tab = NULL; 
        end_tab = NULL; 
    }
}