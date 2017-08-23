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
    printf("Getting peers: [Url = %s]\n", url);

    RestResponse *ars = ark_api_get(url);

    if (ars->data == NULL)
        return NULL;

    cJSON *root = cJSON_Parse(ars->data);
    cJSON *peers = cJSON_GetObjectItem(root, "peers");
    int total = cJSON_GetArraySize(peers);

    ArkPeer *data = malloc(total * sizeof(ArkPeer));
    if (!data)
        return NULL;

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

    free(peers);
    free(root);
    ars = NULL;

    return data;
}

ArkFee ark_api_get_fee(ArkPeer peer)
{
    printf("Getting fees for a peer: [ip = %s, Port = %d]\n", peer.ip, peer.port);

    char url[255];
    snprintf(url, sizeof url, "%s:%d/api/blocks/getfees", peer.ip, peer.port);

    ArkFee fee;
    RestResponse *ars = ark_api_get(url);

    if (ars->data == NULL)
        return fee;

    cJSON *root = cJSON_Parse(ars->data);
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

ArkPeer ark_api_peers_get(ArkPeer peer, int port, char *ip)
{
    ArkPeer arkpeer;
    char url[255];

    snprintf(url, sizeof url, "%s:%d/api/peers/get?port=%d&ip=%s", peer.ip, peer.port, port, ip);

    RestResponse *ars = ark_api_get(url);

    if (ars->data == NULL)
        return arkpeer;

    cJSON *root = cJSON_Parse(ars->data);

    int success = cJSON_GetObjectItem(root, "success")->valueint;

    cJSON *peerJson = cJSON_GetObjectItem(root, "peer");

    if(success == 1)
    {
        arkpeer.ip = cJSON_GetObjectItem(peerJson, "ip")->valuestring;
        arkpeer.port = cJSON_GetObjectItem(peerJson, "port")->valueint;
        arkpeer.height = cJSON_GetObjectItem(peerJson, "height")->valueint;
        arkpeer.version = cJSON_GetObjectItem(peerJson, "version")->valuestring;
        arkpeer.os = cJSON_GetObjectItem(peerJson, "os")->valuestring;
        arkpeer.status = getArkPeerStatus(cJSON_GetObjectItem(peerJson, "status")->valuestring);
        arkpeer.delay = cJSON_GetObjectItem(peerJson, "delay")->valueint;
    }

    free(peerJson);
    free(root);
    ars = NULL;

    return arkpeer;
}

char* ark_api_blocks_getEpoch(ArkPeer peer)
{
    char url[255];
    snprintf(url, sizeof url, "%s:%d/api/blocks/getEpoch", peer.ip, peer.port);

    RestResponse *ars = ark_api_get(url);

    if (ars->data == NULL)
        return NULL;

    cJSON *root = cJSON_Parse(ars->data);
    int success = cJSON_GetObjectItem(root, "success")->valueint;

    char* time = "";
    if(success == 1)
    {
        time = cJSON_GetObjectItem(root, "epoch")->valuestring;
    }

    free(root);
    ars = NULL;

    return time;
}

ArkBlockHeight ark_api_blocks_getHeight(ArkPeer peer)
{
    ArkBlockHeight arkblockheight;
    char url[255];

    snprintf(url, sizeof url, "%s:%d/api/blocks/getHeight", peer.ip, peer.port);

    RestResponse *ars = ark_api_get(url);

    if (ars->data == NULL)
        return arkblockheight;

    cJSON *root = cJSON_Parse(ars->data);

    int success = cJSON_GetObjectItem(root, "success")->valueint;
    if(success == 1)
    {
        arkblockheight.id = cJSON_GetObjectItem(root, "id")->valuestring;
        arkblockheight.height = cJSON_GetObjectItem(root, "height")->valueint;
    }

    free(root);
    ars = NULL;

    return arkblockheight;
}

int ark_api_blocks_getFee(ArkPeer peer)
{
    int fee = -1;
    char url[255];

    snprintf(url, sizeof url, "%s:%d/api/blocks/getFee", peer.ip, peer.port);

    RestResponse *ars = ark_api_get(url);

    if (ars->data == NULL)
        return fee;

    cJSON *root = cJSON_Parse(ars->data);
    int success = cJSON_GetObjectItem(root, "success")->valueint;

    if(success == 1)
    {
        fee = cJSON_GetObjectItem(root, "fee")->valueint;
    }

    free(root);
    ars = NULL;

    return fee;
}

char *ark_api_blocks_getNethash(ArkPeer peer)
{
    char* nethash = "";
    char url[255];

    snprintf(url, sizeof url, "%s:%d/api/blocks/getNethash", peer.ip, peer.port);

    RestResponse *ars = ark_api_get(url);

    if (ars->data == NULL)
        return nethash;

    cJSON *root = cJSON_Parse(ars->data);
    int success = cJSON_GetObjectItem(root, "success")->valueint;

    if(success == 1)
    {
        nethash = cJSON_GetObjectItem(root, "nethash")->valuestring;
    }

    free(root);
    ars = NULL;

    return nethash;
}
