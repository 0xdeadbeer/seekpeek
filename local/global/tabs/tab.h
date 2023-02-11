#ifndef TAB_H
#define TAB_H

#include <stdlib.h>

typedef struct {
    void *html_container;
    void *next_tab; 
    void *prev_tab;
} tab; 

extern tab *start_tab; 
extern tab *current_tab; 

tab *append_tab(void *html_container);
void remove_tab(tab *tab_structure); 

#endif 