#include "arkapi.h"

/// --------------------------------------------------
/// ARK API - BASE FUNCTIONS
/// --------------------------------------------------

/* callback for curl fetch */
size_t curl_callback (void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;                             /* calculate buffer size */
    ArkRestResponse *p = (ArkRestResponse *) userp;   /* cast pointer to fetch struct */

    /* expand buffer */
    p->data = (char *) realloc(p->data, p->size + realsize + 1);

    /* check buffer */
    if (p->data == NULL) {
        /* this isn't good */
        fprintf(stderr, "ERROR: Failed to expand buffer in curl_callback");
        /* free buffer */
        free(p->data);
        /* return */
        return -1;
    }

    /* copy contents to buffer */
    memcpy(&(p->data[p->size]), contents, realsize);

    /* set new buffer size */
    p->size += realsize;

    /* ensure null termination */
    p->data[p->size] = 0;

    /* return size */
    return realsize;
}

/* fetch and return url body via curl */
ArkRestResponse *ark_api_get(const char *url)
{
    ArkRestResponse restResponse;                    /* curl fetch struct */
    ArkRestResponse *response = &restResponse;       /* pointer to fetch struct */

    /* init payload */
    response->data = (char *) calloc(1, sizeof(response->data));

    /* check payload */
    if (response->data == NULL)
    {
        /* log error */
        fprintf(stderr, "ERROR: Failed to allocate payload in curl_fetch_url");
        /* return error */
        return response;
    }

    CURL *curl;

    curl = curl_easy_init();
    if (!curl)
    {
        fprintf(stderr, "ERROR: Failed to initialize curl");
        return response;
    }

    /* init size */
    response->size = 0;

    /* set url to fetch */
    curl_easy_setopt(curl, CURLOPT_URL, url);

    /* set calback function */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_callback);

    /* pass fetch struct pointer */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) response);

    /* set default user agent */
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    /* set timeout */
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);

    /* enable location redirects */
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

    /* set maximum allowed redirects */
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 1);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "charsets: utf-8");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");

    /* fetch the url */
    CURLcode rcode = curl_easy_perform(curl);

    /* return */
    return response;
}

/// --------------------------------------------------
/// ARK API - ACCOUNT(S) FUNCTIONS
/// --------------------------------------------------

//ArkAccount ark_api_accounts_getByAddress()

/// --------------------------------------------------
/// ARK API - BLOCK(S) FUNCTIONS
/// --------------------------------------------------

//ArkBlockHeight ark_api_blocks_getLastBlockHeight()

ArkBlockHeight ark_api_blocks_getHeight(char* ip, int port)
{
    printf("[ARK API] Getting ArkPeers height: [IP = %s, Port = %d]\n", ip, port);

    char url[255];
    snprintf(url, sizeof url, "%s:%d/api/blocks/getHeight", ip, port);

    ArkBlockHeight arkblockheight = {0};
    ArkRestResponse *ars = ark_api_get(url);

    if (ars->size == 0 || ars->data == NULL)
        return arkblockheight;

    cJSON *root = cJSON_Parse(ars->data);

    if (ark_helpers_isResponseSuccess(root) == 0)
        return arkblockheight;

    arkblockheight.id = cJSON_GetObjectItem(root, "id")->valuestring;
    arkblockheight.height = cJSON_GetObjectItem(root, "height")->valueint;

    free(root);
    ars = NULL;

    return arkblockheight;
}

char* ark_api_blocks_getEpoch(char* ip, int port)
{
    printf("[ARK API] Getting ArkBlock Epoch: [IP = %s, Port = %d]\n", ip, port);

    char url[255];
    snprintf(url, sizeof url, "%s:%d/api/blocks/getEpoch", ip, port);

    ArkRestResponse *ars = ark_api_get(url);

    if (ars->size == 0 || ars->data == NULL)
        return NULL;

    cJSON *root = cJSON_Parse(ars->data);

    if (ark_helpers_isResponseSuccess(root) == 0)
        return NULL;

    char* time = "";
    time = cJSON_GetObjectItem(root, "epoch")->valuestring;

    free(root);
    ars = NULL;

    return time;
}

int ark_api_blocks_getFee(char *ip, int port)
{
    printf("[ARK API] Getting ArkFee from: [IP = %s, Port = %d]\n", ip, port);

    int fee = -1;
    char url[255];

    snprintf(url, sizeof url, "%s:%d/api/blocks/getFee", ip, port);

    ArkRestResponse *ars = ark_api_get(url);

    if (ars->size == 0 || ars->data == NULL)
        return fee;

    cJSON *root = cJSON_Parse(ars->data);

    if (ark_helpers_isResponseSuccess(root) == 0)
        return fee;

    fee = cJSON_GetObjectItem(root, "fee")->valueint;

    free(root);
    ars = NULL;

    return fee;
}

char *ark_api_blocks_getNethash(char* ip, int port)
{
    printf("[ARK API] Getting NetHash from: [IP = %s, Port = %d]\n", ip, port);

    char* nethash = "";
    char url[255];

    snprintf(url, sizeof url, "%s:%d/api/blocks/getNethash", ip, port);

    ArkRestResponse *ars = ark_api_get(url);

    if (ars->size == 0 || ars->data == NULL)
        return nethash;

    cJSON *root = cJSON_Parse(ars->data);

    if (ark_helpers_isResponseSuccess(root) == 0)
        return nethash;

    nethash = cJSON_GetObjectItem(root, "nethash")->valuestring;

    free(root);
    ars = NULL;

    return nethash;
}

int ark_blocks_getMilestone(char *ip, int port)
{
    printf("[ARK API] Getting milestone from: [IP = %s, Port = %d]\n", ip, port);

    int milestone = -1;
    char url[255];

    snprintf(url, sizeof url, "%s:%d/api/blocks/getMilestone", ip, port);

    ArkRestResponse *ars = ark_api_get(url);

    if (ars->size == 0 || ars->data == NULL)
        return milestone;

    cJSON *root = cJSON_Parse(ars->data);

    if (ark_helpers_isResponseSuccess(root) == 0)
        return milestone;

    milestone = cJSON_GetObjectItem(root, "milestone")->valueint;

    free(root);
    ars = NULL;

    return milestone;
}

/// --------------------------------------------------
/// ARK API - DELEGATE(S) FUNCTIONS
/// --------------------------------------------------

ArkDelegateArray ark_api_delegates(char* ip, int port)
{
    printf("[ARK API] Getting delegates: [IP = %s, Port: = %d]\n", ip, port);

    char url[255];
    snprintf(url, sizeof url, "%s:%d/api/delegates", ip, port);

    ArkDelegateArray ada = {0};
    ArkRestResponse *ars = ark_api_get(url);

    if (ars->size == 0 || ars->data == NULL)
        return ada;

    cJSON *root = cJSON_Parse(ars->data);

    if (ark_helpers_isResponseSuccess(root) == 0)
        return ada;

    cJSON *delegates = cJSON_GetObjectItem(root, "delegates");
    int total = cJSON_GetArraySize(delegates);

    ArkDelegate *data = malloc(total * sizeof(ArkDelegate));
    if (!data)
        return ada;

    for (int i = 0; i < total; i++)
    {
        cJSON *delegateJson = cJSON_GetArrayItem(delegates, i);

        data[i] = ark_helpers_getArkDelegate_fromJSON(delegateJson);
    }

    ada.length = total;
    ada.data = data;

    free(delegates);
    free(root);
    ars = NULL;

    return ada;
}

ArkDelegate ark_api_delegates_getByUsername(char* ip, int port, char* username)
{
    printf("Getting delegate by username: [IP = %s, Port: = %d, Username = %s]\n", ip, port, username);

    char url[255];
    snprintf(url, sizeof url, "%s:%d/api/delegates/get?username=%s", ip, port, username);

    ArkDelegate delegate = {0};
    ArkRestResponse *ars = ark_api_get(url);

    if (ars->data == NULL)
        return delegate;

    cJSON *root = cJSON_Parse(ars->data);

    if (ark_helpers_isResponseSuccess(root) == 0)
        return delegate;

    cJSON *delegateJson = cJSON_GetObjectItem(root, "delegate");

    delegate = ark_helpers_getArkDelegate_fromJSON(delegateJson);

    free(delegateJson);
    free(root);
    ars = NULL;

    return delegate;
}

// api/delegates/forging/getForgedByAccount

/// --------------------------------------------------
/// ARK API - FEE(S) FUNCTIONS
/// --------------------------------------------------

ArkFee ark_api_fees_get(char* ip, int port)
{
    printf("[ARK API] Getting fees for an ArkPeer: [IP = %s, Port = %d]\n", ip, port);

    char url[255];
    snprintf(url, sizeof url, "%s:%d/api/blocks/getfees", ip, port);

    ArkFee fee = {0};
    ArkRestResponse *ars = ark_api_get(url);

    if (ars->size == 0 || ars->data == NULL)
        return fee;

    cJSON *root = cJSON_Parse(ars->data);

    if (ark_helpers_isResponseSuccess(root) == 0)
        return fee;

    cJSON *feeJson = cJSON_GetObjectItem(root, "fees");

    fee.send = (long)(cJSON_GetObjectItem(feeJson, "send")->valuedouble + 0.5);
    fee.vote = (long)(cJSON_GetObjectItem(feeJson, "vote")->valuedouble + 0.5);
    fee.secondSignature = (long)(cJSON_GetObjectItem(feeJson, "secondsignature")->valuedouble + 0.5);
    fee.delegate = (long)(cJSON_GetObjectItem(feeJson, "delegate")->valuedouble + 0.5);
    fee.multiSignature = (long)(cJSON_GetObjectItem(feeJson, "multisignature")->valuedouble + 0.5);

    free(feeJson);
    free(root);
    ars = NULL;

    return fee;
}

/// --------------------------------------------------
/// ARK API - NETWORK(S) FUNCTIONS
/// --------------------------------------------------

ArkNetwork ark_api_network_autoconfigure(char *ip, int port)
{
    printf("[ARK API] Getting network configuration for an ArkPeer: [IP = %s, Port = %d]\n", ip, port);

    char url[255];
    snprintf(url, sizeof url, "%s:%d/api/loader/autoconfigure", ip, port);

    /// Alternative of string concatenating
    //size_t len = (size_t)snprintf(NULL, 0, "%s:%d/api/loader/autoconfigure", ip, port) + 1;
    //char* url = malloc(len);
    //snprintf(url, len, "%s:%d/api/loader/autoconfigure", ip, port);

    ArkNetwork network = {0};
    ArkRestResponse *ars = ark_api_get(url);

    if (ars->size == 0 || ars->data == NULL)
        return network;

    cJSON *root = cJSON_Parse(ars->data);

    if (ark_helpers_isResponseSuccess(root) == 0)
        return network;

    cJSON *networkJson = cJSON_GetObjectItem(root, "network");

    network.netHash = cJSON_GetObjectItem(networkJson, "nethash")->valuestring;
    network.token = cJSON_GetObjectItem(networkJson, "token")->valuestring;
    network.symbol = cJSON_GetObjectItem(networkJson, "symbol")->valuestring;
    network.explorer = cJSON_GetObjectItem(networkJson, "explorer")->valuestring;
    network.version = cJSON_GetObjectItem(networkJson, "version")->valueint;

    free(networkJson);
    free(root);
    ars = NULL;

    return network;
}

/// --------------------------------------------------
/// ARK API - PEER(S) FUNCTIONS
/// --------------------------------------------------

ArkPeerArray ark_api_peers(char* ip, int port)
{
    printf("[ARK API] Getting peers: [IP = %s, Port: = %d]\n", ip, port);

    char url[255];
    snprintf(url, sizeof url, "%s:%d/api/peers", ip, port);

    ArkPeerArray apa = {0};
    ArkRestResponse *ars = ark_api_get(url);

    if (ars->size == 0 || ars->data == NULL)
        return apa;

    cJSON *root = cJSON_Parse(ars->data);

    if (ark_helpers_isResponseSuccess(root) == 0)
        return apa;

    cJSON *peers = cJSON_GetObjectItem(root, "peers");
    int total = cJSON_GetArraySize(peers);

    ArkPeer *data = malloc(total * sizeof(ArkPeer));
    if (!data)
        return apa;

    for (int i = 0; i < total; i++)
    {
        cJSON *peerJson = cJSON_GetArrayItem(peers, i);

        data[i] = ark_helpers_getArkPeer_fromJSON(peerJson);
    }

    apa.length = total;
    apa.data = data;

    free(peers);
    free(root);
    ars = NULL;

    return apa;
}

ArkPeerArray ark_api_peers_getList(char* ip, int port)
{
    printf("[ARK API] Getting peer list: [IP = %s, Port: = %d]\n", ip, port);

    char url[255];
    snprintf(url, sizeof url, "%s:%d/api/peer/list", ip, port);

    ArkPeerArray apa = {0};
    ArkRestResponse *ars = ark_api_get(url);

    if (ars->size == 0 || ars->data == NULL)
        return apa;

    cJSON *root = cJSON_Parse(ars->data);

    if (ark_helpers_isResponseSuccess(root) == 0)
        return apa;

    cJSON *peers = cJSON_GetObjectItem(root, "peers");
    int total = cJSON_GetArraySize(peers);

    ArkPeer *data = malloc(total * sizeof(ArkPeer));
    if (!data)
        return apa;

    for (int i = 0; i < total; i++)
    {
        cJSON *peerJson = cJSON_GetArrayItem(peers, i);

        data[i] = ark_helpers_getArkPeer_fromJSON(peerJson);
    }

    apa.length = total;
    apa.data = data;

    free(peers);
    free(root);
    ars = NULL;

    return apa;
}

ArkPeer ark_api_peers_get(ArkPeer peer, char *ip, int port)
{
    printf("[ARK API] Getting ArkPeer details: [IP = %s, Port = %d]\n", ip, port);

    char url[255];
    snprintf(url, sizeof url, "%s:%d/api/peers/get?port=%d&ip=%s", peer.ip, peer.port, port, ip);

    ArkPeer arkpeer = {0};
    ArkRestResponse *ars = ark_api_get(url);

    if (ars->size == 0 || ars->data == NULL)
        return arkpeer;

    cJSON *root = cJSON_Parse(ars->data);

    if (ark_helpers_isResponseSuccess(root) == 0)
        return arkpeer;

    cJSON *peerJson = cJSON_GetObjectItem(root, "peer");

    arkpeer = ark_helpers_getArkPeer_fromJSON(peerJson);

    free(peerJson);
    free(root);
    ars = NULL;

    return arkpeer;
}

int ark_api_peers_getStatus(char* ip, int port)
{
    printf("[ARK API] Getting ArkPeer status: [IP = %s, Port: = %d]\n", ip, port);

    char url[255];
    snprintf(url, sizeof url, "%s:%d/api/peer/status?port=%d&ip=%s", ip, port, port, ip);

    ArkPeerArray apa = {0};
    ArkRestResponse *ars = ark_api_get(url);

    if (ars->size == 0 || ars->data == NULL)
        return 0;

    cJSON *root = cJSON_Parse(ars->data);

    if (ark_helpers_isResponseSuccess(root) == 0)
        return 0;

    cJSON *peers = cJSON_GetObjectItem(root, "peers");
    int total = cJSON_GetArraySize(peers);

    ArkPeer *data = malloc(total * sizeof(ArkPeer));
    if (!data)
        return 0;

    for (int i = 0; i < total; i++)
    {
        cJSON *peerJson = cJSON_GetArrayItem(peers, i);

        data[i] = ark_helpers_getArkPeer_fromJSON(peerJson);
    }

    apa.length = total;
    apa.data = data;

    free(peers);
    free(root);
    ars = NULL;

    return 1;
}

/// --------------------------------------------------
/// ARK API - TRANSACTION(S) FUNCTIONS
/// --------------------------------------------------

ArkTransaction ark_api_transactions_get(char* id)
{
    printf("[ARK API] Getting ArkTransaction details: [ID = %s]\n", id);

    char url[255];
    snprintf(url, sizeof url, "%s:%d/transactions/get?id=%s", "TBD", 0, id);

    ArkTransaction arkTransaction = {0};
    ArkRestResponse *ars = ark_api_get(url);

    if (ars->size == 0 || ars->data == NULL)
        return arkTransaction;

    cJSON *root = cJSON_Parse(ars->data);

    if (ark_helpers_isResponseSuccess(root) == 0)
        return arkTransaction;

    cJSON *transactionJson = cJSON_GetObjectItem(root, "transaction");

    arkTransaction = ark_helpers_getArkTransaction_fromJSON(transactionJson);

    free(transactionJson);
    free(root);
    ars = NULL;

    return arkTransaction;
}

/// --------------------------------------------------
/// ARK API - VOTER(S) FUNCTIONS
/// --------------------------------------------------

ArkVoterArray ark_api_voters_getByDelegate(char* ip, int port, char* publicKey)
{
    printf("[ARK API] Getting ArkDelegate voters: [IP = %s, Port: = %d, PublicKey = %s]\n", ip, port, publicKey);

    char url[255];
    snprintf(url, sizeof url, "%s:%d/api/delegates/voters?publicKey=%s", ip, port, publicKey);

    ArkVoterArray ava = {0};
    ArkRestResponse *ars = ark_api_get(url);

    if (ars->size == 0 || ars->data == NULL)
        return ava;

    cJSON *root = cJSON_Parse(ars->data);

    if (ark_helpers_isResponseSuccess(root) == 0)
        return ava;

    cJSON *voters = cJSON_GetObjectItem(root, "accounts");
    int total = cJSON_GetArraySize(voters);

    ArkVoter *data = malloc(total * sizeof(ArkVoter));
    if (!data)
        return ava;

    for (int i = 0; i < total; i++)
    {
        cJSON *voterJson = cJSON_GetArrayItem(voters, i);

        data[i] = ark_helpers_getArkVoter_fromJSON(voterJson);
    }

    ava.length = total;
    ava.data = data;

    free(voters);
    free(root);
    ars = NULL;

    return ava;
}

/// --------------------------------------------------
/// ARK API - OTHER FUNCTIONS
/// --------------------------------------------------
