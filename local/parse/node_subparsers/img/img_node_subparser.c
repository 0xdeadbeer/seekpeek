#include "./img_node_subparser.h"

size_t save_picture(void *ptr, size_t size, size_t nmemb, void *stream) {
    size_t written = fwrite(ptr, size, nmemb, (FILE *) stream);
    return written; 
}

void download_picture(char *url, char *location) {
    CURL *curl_handle = curl_easy_init();
    FILE *picture_file = fopen(location, "wb");

    if (!picture_file) {
        gui_alert(GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Failed to open file"); 
        return; 
    }

    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, save_picture);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, picture_file);

    curl_easy_perform(curl_handle);

    fclose(picture_file);
    curl_easy_cleanup(curl_handle);
}

char *generate_filename(int length) {
    srand(time(NULL) + clock() + getpid());

    char *string = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const size_t string_length = 62;

	char *result = malloc(sizeof(char) * (length +1));
	if(!result) {
		return (char*)0;
	}

	unsigned int key = 0;
	for(int index = 0;index<length;index++) {
		key = rand() % string_length;
		result[index] = string[key];
	}

	result[length] = '\0';
	return result;
}