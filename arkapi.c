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

ArkPeer ark_helpers_get_ArkPeer(const cJSON *const json)
{
    ArkPeer peer;
    peer.ip = cJSON_GetObjectItem(json, "ip")->valuestring;
    peer.port = cJSON_GetObjectItem(json, "port")->valueint;
    peer.version = cJSON_GetObjectItem(json, "version")->valuestring;
    peer.os = cJSON_GetObjectItem(json, "os")->valuestring;
    peer.status = getArkPeerStatus(cJSON_GetObjectItem(json, "status")->valuestring);
    peer.delay = cJSON_GetObjectItem(json, "delay")->valueint;

    return peer;
}

ArkDelegate ark_helpers_get_ArkDelegate(const cJSON *const json)
{
    ArkDelegate delegate;
    delegate.username = cJSON_GetObjectItem(json, "username")->valuestring;
    delegate.address = cJSON_GetObjectItem(json, "address")->valuestring;
    delegate.publicKey = cJSON_GetObjectItem(json, "publicKey")->valuestring;
    delegate.vote = cJSON_GetObjectItem(json, "vote")->valuestring;
    delegate.producedBlocks = (long)(cJSON_GetObjectItem(json, "producedblocks")->valuedouble + 0.5);
    delegate.missedBlocks = (long)(cJSON_GetObjectItem(json, "missedblocks")->valuedouble + 0.5);
    delegate.rate = cJSON_GetObjectItem(json, "rate")->valueint;
    delegate.approval = cJSON_GetObjectItem(json, "approval")->valuedouble;
    delegate.productivity = cJSON_GetObjectItem(json, "productivity")->valuedouble;

    return delegate;
}

ArkVoter ark_helpers_get_ArkVoter(const cJSON *const json)
{
    ArkVoter voter;
    voter.username = cJSON_GetObjectItem(json, "username")->valuestring;
    voter.address = cJSON_GetObjectItem(json, "address")->valuestring;
    voter.publicKey = cJSON_GetObjectItem(json, "publicKey")->valuestring;
    voter.balance = (long)(cJSON_GetObjectItem(json, "balance")->valuedouble + 0.5);

    return voter;
}

ArkPeer* ark_api_get_peers(char* serverIp, int serverPort)
{
    printf("Getting peers: [ServerIP = %s, ServerPort: = %d]\n", serverIp, serverPort);

    char url[255];
    snprintf(url, sizeof url, "%s:%d/api/peers", serverIp, serverPort);

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

        data[i] = ark_helpers_get_ArkPeer(peerJson);
    }

    free(peers);
    free(root);
    ars = NULL;

    return data;
}

ArkFee ark_api_get_fee(char* peerIp, int peerPort)
{
    printf("Getting fees for a peer: [ip = %s, Port = %d]\n", peerIp, peerPort);

    char url[255];
    snprintf(url, sizeof url, "%s:%d/api/blocks/getfees", peerIp, peerPort);

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
        arkpeer = ark_helpers_get_ArkPeer(peerJson);
        /*arkpeer.ip = cJSON_GetObjectItem(peerJson, "ip")->valuestring;
        arkpeer.port = cJSON_GetObjectItem(peerJson, "port")->valueint;
        arkpeer.height = cJSON_GetObjectItem(peerJson, "height")->valueint;
        arkpeer.version = cJSON_GetObjectItem(peerJson, "version")->valuestring;
        arkpeer.os = cJSON_GetObjectItem(peerJson, "os")->valuestring;
        arkpeer.status = getArkPeerStatus(cJSON_GetObjectItem(peerJson, "status")->valuestring);
        arkpeer.delay = cJSON_GetObjectItem(peerJson, "delay")->valueint;*/
    }

    free(peerJson);
    free(root);
    ars = NULL;

    return arkpeer;
}

time_t ark_api_blocks_getEpoch(ArkPeer peer)
{
    return NULL;
}

ArkDelegate* ark_api_get_delegates(char* serverIp, int serverPort)
{
    printf("Getting delegates: [ServerIP = %s, ServerPort: = %d]\n", serverIp, serverPort);

    char url[255];
    snprintf(url, sizeof url, "%s:%d/api/delegates", serverIp, serverPort);

    RestResponse *ars = ark_api_get(url);

    if (ars->data == NULL)
        return NULL;

    cJSON *root = cJSON_Parse(ars->data);
    cJSON *delegates = cJSON_GetObjectItem(root, "delegates");
    int total = cJSON_GetArraySize(delegates);

    ArkDelegate *data = malloc(total * sizeof(ArkDelegate));
    if (!data)
        return NULL;

    for (int i = 0; i < total; i++)
    {
        cJSON *delegateJson = cJSON_GetArrayItem(delegates, i);

        data[i] = ark_helpers_get_ArkDelegate(delegateJson);
    }

    free(delegates);
    free(root);
    ars = NULL;

    return data;
}

ArkDelegate ark_api_get_delegate_by_username(char* serverIp, int serverPort, char* username)
{
    printf("Getting delegate by username: [ServerIP = %s, ServerPort: = %d, Username = %s]\n", serverIp, serverPort, username);

    char url[255];
    snprintf(url, sizeof url, "%s:%d/api/delegates/get?username=%s", serverIp, serverPort, username);

    ArkDelegate delegate;
    RestResponse *ars = ark_api_get(url);

    if (ars->data == NULL)
        return delegate;

    cJSON *root = cJSON_Parse(ars->data);
    cJSON *delegateJson = cJSON_GetObjectItem(root, "delegate");

    delegate = ark_helpers_get_ArkDelegate(delegateJson);

    free(delegateJson);
    free(root);
    ars = NULL;

    return delegate;
}

ArkVoter* ark_api_get_delegate_voters(char* serverIp, int serverPort, char* publicKey)
{
    printf("Getting delegate voters: [ServerIP = %s, ServerPort: = %d, PublicKey = %s]\n", serverIp, serverPort, publicKey);

    char url[255];
    snprintf(url, sizeof url, "%s:%d/api/delegates/voters?publicKey=", serverIp, serverPort, publicKey);

    RestResponse *ars = ark_api_get(url);

    if (ars->data == NULL)
        return NULL;

    cJSON *root = cJSON_Parse(ars->data);
    cJSON *voters = cJSON_GetObjectItem(root, "accounts");
    int total = cJSON_GetArraySize(voters);

    ArkVoter *data = malloc(total * sizeof(ArkVoter));
    if (!data)
        return NULL;

    for (int i = 0; i < total; i++)
    {
        cJSON *voterJson = cJSON_GetArrayItem(voters, i);

        data[i] = ark_helpers_get_ArkVoter(voterJson);
    }

    free(voters);
    free(root);
    ars = NULL;

    return data;
}
