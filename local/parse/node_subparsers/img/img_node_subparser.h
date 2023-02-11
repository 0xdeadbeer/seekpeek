#ifndef IMG_NODE_SUBPARSER_H
#define IMG_NODE_SUBPARSER_H 

#include <stdlib.h>
#include <curl/curl.h>
#include <gtk/gtk.h>
#include <time.h>

size_t save_picture(void *ptr, size_t size, size_t nmemb, void *stream); 
void download_picture(char *url, char *location);
char *generate_filename(int length);

#endif 