#include <curl.h>
#include <stdio.h>
#include "arkmodels.h"
#include "cJSON.h"

ARKPEERSTATUS getArkPeerStatus(const char* string);

/* callback for curl fetch */
size_t curl_callback (void *contents, size_t size, size_t nmemb, void *userp);

/* fetch and return url body via curl */
ArkRestResponse * ark_api_get(const char *url);

ArkPeer* ark_api_get_peers(const char *url);

ArkFee *ark_api_get_fee(ArkPeer peer);
