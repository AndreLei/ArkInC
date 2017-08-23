#include <stdio.h>
#include "curl.h"

typedef struct {
    char *data;
    size_t size;
} RestResponse;

/* callback for curl fetch */
size_t curl_callback (void *contents, size_t size, size_t nmemb, void *userp);

/* fetch and return url body via curl */
RestResponse* ark_api_get(const char *url);
