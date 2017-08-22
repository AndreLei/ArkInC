#include <stdio.h>
#include <curl.h>
#include "cJSON.h"

/* holder for curl fetch */
struct curl_fetch_st {
    char *payload;
    size_t size;
};

/* callback for curl fetch */
size_t curl_callback (void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;                             /* calculate buffer size */
    struct curl_fetch_st *p = (struct curl_fetch_st *) userp;   /* cast pointer to fetch struct */

    /* expand buffer */
    p->payload = (char *) realloc(p->payload, p->size + realsize + 1);

    /* check buffer */
    if (p->payload == NULL) {
        /* this isn't good */
        fprintf(stderr, "ERROR: Failed to expand buffer in curl_callback");
        /* free buffer */
        free(p->payload);
        /* return */
        return -1;
    }

    /* copy contents to buffer */
    memcpy(&(p->payload[p->size]), contents, realsize);

    /* set new buffer size */
    p->size += realsize;

    /* ensure null termination */
    p->payload[p->size] = 0;

    /* return size */
    return realsize;
}

/* fetch and return url body via curl */
CURLcode curl_fetch_url(CURL *ch, const char *url, struct curl_fetch_st *fetch) {
    CURLcode rcode;                   /* curl result code */

    /* init payload */
    fetch->payload = (char *) calloc(1, sizeof(fetch->payload));

    /* check payload */
    if (fetch->payload == NULL) {
        /* log error */
        fprintf(stderr, "ERROR: Failed to allocate payload in curl_fetch_url");
        /* return error */
        return CURLE_FAILED_INIT;
    }

    /* init size */
    fetch->size = 0;

    /* set url to fetch */
    curl_easy_setopt(ch, CURLOPT_URL, url);

    /* set calback function */
    curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, curl_callback);

    /* pass fetch struct pointer */
    curl_easy_setopt(ch, CURLOPT_WRITEDATA, (void *) fetch);

    /* set default user agent */
    curl_easy_setopt(ch, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    /* set timeout */
    curl_easy_setopt(ch, CURLOPT_TIMEOUT, 5);

    /* enable location redirects */
    curl_easy_setopt(ch, CURLOPT_FOLLOWLOCATION, 1);

    /* set maximum allowed redirects */
    curl_easy_setopt(ch, CURLOPT_MAXREDIRS, 1);

    /* fetch the url */
    rcode = curl_easy_perform(ch);

    /* return */
    return rcode;
}

int main()
{
    CURL *curl;
    CURLcode rcode;

    curl = curl_easy_init();
    if (curl)
    {
        struct curl_fetch_st curl_fetch;                        /* curl fetch struct */
        struct curl_fetch_st *cf = &curl_fetch;                 /* pointer to fetch struct */

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "charsets: utf-8");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");

        char *url = "http://192.168.0.108/haha/updates.json";

        rcode = curl_fetch_url(curl, url, cf);

        /* always cleanup */
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        curl_global_cleanup();

        /* check return code */
        if (rcode != CURLE_OK || cf->size < 1)
        {
            /* log error */
            fprintf(stderr, "ERROR: Failed to fetch url (%s) - curl said: %s", url, curl_easy_strerror(rcode));
            /* return error */
            return 2;
        }

        /* check payload */
        if (cf->payload != NULL)
        {
            /* print result */
            printf("kekekekekekeekekekekek\n");
            printf("CURL Returned: \n %s\n", cf->payload);
            /* parse return */
            ///json = json_tokener_parse_verbose(cf->payload, &jerr);
            /* free payload */
            free(cf->payload);
        }
        else
        {
            /* error */
            fprintf(stderr, "ERROR: Failed to populate payload");
            /* free payload */
            free(cf->payload);
            /* return */
            return 3;
        }
    }

    return 0;
}
