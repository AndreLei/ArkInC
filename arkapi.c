#include "arkapi.h"

ARKPEERSTATUS getArkPeerStatus(const char* string)
{
    static struct {
        const char *s;
        ARKPEERSTATUS e;
    } map[] = {
        { "OK", OK },
        { "EUNAVAILABLE", EUNAVAILABLE },
        { "ETIMEOUT", ETIMEOUT },
    };

    ARKPEERSTATUS result = OK;

    for (int i = 0 ; i < sizeof(map)/sizeof(map[0]); i++)
    {
        if (strcmp(string, map[i].s) == 0)
        {
            result = map[i].e;
            break;
        }
    }

    return result;
}

ArkPeer* ark_api_get_peers(const char *url)
{
    RestResponse *ars = ark_api_get(url);

    if (ars->data == NULL)
        return NULL;

    cJSON *root = cJSON_Parse(ars->data);
    cJSON *peers = cJSON_GetObjectItem(root, "peers");
    int total = cJSON_GetArraySize(peers);

    ArkPeer *data = malloc(total * sizeof(ArkPeer));

    for (int i = 0; i < total; i++)
    {
        cJSON *peerJson = cJSON_GetArrayItem(peers, i);

        ArkPeer peer;
        peer.ip = cJSON_GetObjectItem(peerJson, "ip")->valuestring;
        peer.port = cJSON_GetObjectItem(peerJson, "port")->valueint;
        peer.version = cJSON_GetObjectItem(peerJson, "version")->valuestring;
        peer.os = cJSON_GetObjectItem(peerJson, "os")->valuestring;
        peer.status = getArkPeerStatus(cJSON_GetObjectItem(peerJson, "status")->valuestring);
        peer.delay = cJSON_GetObjectItem(peerJson, "delay")->valueint;

        data[i] = peer;
    }
/*
    free(ars);
    free(peers);
    free(root);
*/
    return data;
}

ArkFee* ark_api_get_fee(ArkPeer peer)
{
    //char *url;
    //asprintf(&url, "%s:%d/api/blocks/getfees", peer.ip, peer.port);

    char url[256];
    snprintf(url, sizeof url, "%s:%d/api/blocks/getfees", peer.ip, peer.port);

    RestResponse *ars = ark_api_get(url);

    if (ars->data == NULL)
        return NULL;

    cJSON *root = cJSON_Parse(ars->data);
    cJSON *feeJson = cJSON_GetObjectItem(root, "fees");

    ArkFee *fee;
    fee = malloc(2 * sizeof *fee);

    double d = cJSON_GetObjectItem(feeJson, "send")->valuedouble;
    fee->send = (long)(cJSON_GetObjectItem(feeJson, "send")->valuedouble + 0.5);
    fee->vote = (long)(cJSON_GetObjectItem(feeJson, "vote")->valuedouble + 0.5);
    fee->secondSignature = (long)(cJSON_GetObjectItem(feeJson, "secondsignature")->valuedouble + 0.5);
    fee->delegate = (long)(cJSON_GetObjectItem(feeJson, "delegate")->valuedouble + 0.5);
    fee->multiSignature = (long)(cJSON_GetObjectItem(feeJson, "multisignature")->valuedouble + 0.5);

    return fee;
}

ArkPeer* ark_api_peers_get(ArkPeer peer, int port, char *ip)
{
    char url[255];
    snprintf(url, sizeof url, "%s:%d/api/peers/get?port=%d&ip=%s", peer.ip, peer.port, port, ip);

    RestResponse *ars = ark_api_get(url);

    if (ars->data == NULL)
        return NULL;

    cJSON *root = cJSON_Parse(ars->data);

    int success = cJSON_GetObjectItem(root, "success")->valueint;

    ArkPeer* arkpeer = (ArkFee*) malloc(sizeof *arkpeer);
    cJSON *peerJson = cJSON_GetObjectItem(root, "peer");

    if(success == 1)
    {
        arkpeer->ip = cJSON_GetObjectItem(peerJson, "ip")->valuestring;
        arkpeer->port = cJSON_GetObjectItem(peerJson, "port")->valueint;
        arkpeer->height = cJSON_GetObjectItem(peerJson, "height")->valueint;
        arkpeer->version = cJSON_GetObjectItem(peerJson, "version")->valuestring;
        arkpeer->os = cJSON_GetObjectItem(peerJson, "os")->valuestring;
        arkpeer->status = getArkPeerStatus(cJSON_GetObjectItem(peerJson, "status")->valuestring);
        arkpeer->delay = cJSON_GetObjectItem(peerJson, "delay")->valueint;
    }
    else arkpeer = NULL;

    free(peerJson);
    free(root);
    ars = NULL;

    return arkpeer;
}

time_t ark_api_blocks_getEpoch(ArkPeer peer)
{
    return NULL;
}
