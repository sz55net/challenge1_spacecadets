#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

#define BASE_URL "https://www.ecs.soton.ac.uk/people/"


size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    bool found_start_of_json = false;
    char *line = strtok(ptr, "\n");
    char *json_string = malloc(4096);
    memset(json_string, 0, 4096);
    size_t json_string_pos = 0;
    while (line != NULL) {
        if (!found_start_of_json) {
            int comparison = strcmp(line, "<script type=\"application/ld+json\">{");
            if (comparison == 0) {
                printf("found\n");
                found_start_of_json = true;
                memcpy(json_string + json_string_pos, &"{", 1);
                json_string_pos++;
            }
        } else {
            int comparison = strcmp(line, "}</script>");
            if (comparison == 0) {
                memcpy(json_string + json_string_pos, &"}", 1);
                json_string_pos++;
                break;
            }
            size_t line_size = strlen(line);
            memcpy(json_string + json_string_pos, line, line_size);
            json_string_pos += line_size;
        }
        line = strtok(NULL, "\n");
    }
    if (found_start_of_json) {
        cJSON *json = cJSON_Parse(json_string);
        cJSON *graph = cJSON_GetObjectItem(json, "@graph");
        cJSON *person = cJSON_GetArrayItem(graph, 1);
        cJSON *name = cJSON_GetObjectItem(person, "name");
        printf("Name is: %s", name->valuestring);
    }
    return 0;
}

int main(void) {
    printf("Please enter the user id to lookup: ");
    char *url = malloc(strlen(BASE_URL) + 32);
    memcpy(url, &BASE_URL, strlen(BASE_URL));
    scanf("%s", url + strlen(BASE_URL));
    CURL *curl = curl_easy_init();

    if (curl) {
        CURLcode result;
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        result = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    };


    return 0;
}
