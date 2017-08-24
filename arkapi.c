#include "arkapi.h"

/// --------------------
/// ARK HELPERS
/// --------------------

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

ArkPeer ark_helpers_get_ArkPeer(const cJSON * const json)
{
    ArkPeer peer = {0};
    peer.ip = cJSON_GetObjectItem(json, "ip")->valuestring;
    peer.port = cJSON_GetObjectItem(json, "port")->valueint;
    peer.version = cJSON_GetObjectItem(json, "version")->valuestring;
    peer.os = cJSON_GetObjectItem(json, "os")->valuestring;
    peer.status = getArkPeerStatus(cJSON_GetObjectItem(json, "status")->valuestring);
    peer.delay = cJSON_GetObjectItem(json, "delay")->valueint;

    return peer;
}

ArkDelegate ark_helpers_get_ArkDelegate(const cJSON * const json)
{
    ArkDelegate delegate = {0};
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

ArkVoter ark_helpers_get_ArkVoter(const cJSON * const json)
{
    ArkVoter voter = {0};
    voter.username = cJSON_GetObjectItem(json, "username")->valuestring;
    voter.address = cJSON_GetObjectItem(json, "address")->valuestring;
    voter.publicKey = cJSON_GetObjectItem(json, "publicKey")->valuestring;
    voter.balance = (long)(cJSON_GetObjectItem(json, "balance")->valuedouble + 0.5);

    return voter;
}

/// --------------------
/// PRIVATE ARK GLOBAL FUNCTIONS
/// --------------------

int ark_helpers_isNetworkNull(ArkNetwork network)
{
    return (network.explorer == NULL &&
            network.netHash == NULL &&
            network.symbol == NULL &&
            network.token == NULL &&
            network.version == 0)
            ? 1 : 0;
}

int ark_helpers_isPeerNull(ArkPeer peer)
{
    return (peer.delay == 0 &&
            peer.height == 0 &&
            peer.ip == NULL &&
            peer.os == NULL &&
            peer.port == 0 &&
            peer.status == NULL &&
            peer.version == NULL)
            ? 1 : 0;
}

int ark_helpers_isFeeNull(ArkFee fee)
{
    return (fee.delegate == 0 &&
            fee.multiSignature == 0 &&
            fee.secondSignature == 0 &&
            fee.send == 0 &&
            fee.vote == 0)
            ? 1 : 0;
}

int ark_global_setEnvrionment(ARKNETWORKTYPE networkType)
{
    printf("[ARK] Setting NetworkType to '%d'\n", networkType);
    global_networkType = networkType;

    ArkNetwork peerNetworkConfiguration = {0};
    ArkPeer randomPeer = {0};

    int fail = 1;
    int iterations = 5;
    while (iterations > 0 && fail == 1)
    {
        randomPeer = ark_helpers_get_randomPeer();
        peerNetworkConfiguration = ark_api_get_network(randomPeer.ip, randomPeer.port);
        fail = ark_helpers_isNetworkNull(peerNetworkConfiguration);

        iterations--;
    }

    if (fail == 1)
        return 0;

    global_network = peerNetworkConfiguration;
    global_selectedPeer = ark_api_peers_get(randomPeer, randomPeer.port, randomPeer.ip);

    if (ark_helpers_isPeerNull(global_selectedPeer) == 1)
        return 0;

    global_selectedPeerFee = ark_api_get_fee(global_selectedPeer.ip, global_selectedPeer.port);
    if (ark_helpers_isFeeNull(global_selectedPeerFee) == 1)
        return 0;

    return ark_global_filterPeers();
}

int ark_global_filterPeers()
{
    printf("[ARK] Filtering peers...\n");

    ArkPeerArray tuple = ark_api_get_peers(global_selectedPeer.ip, global_selectedPeer.port);

    int num=0;
    for (int i=0 ; i<tuple.length ; i++)
    {
        if (tuple.data[i].status == OK)
            num++;
    }
    printf("[ARK] Filtering peers returned '%d' valid peers...\n", num);

    return 1;
}

/// --------------------
/// PUBLIC ARK FUNCTIONS
/// --------------------

ArkNetwork ark_api_get_network(char *ip, int port)
{
    printf("[ARK] Getting network configuration for a peer: [IP = %s, Port = %d]\n", ip, port);

    char url[255];
    snprintf(url, sizeof url, "%s:%d/api/loader/autoconfigure", ip, port);

    ArkNetwork network = {0};
    RestResponse *ars = ark_api_get(url);

    if (ars->size == 0 || ars->data == NULL)
        return network;

    cJSON *root = cJSON_Parse(ars->data);

    if ((cJSON_GetObjectItem(root, "success")->valueint) != 1)
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

ArkPeer ark_helpers_get_randomPeer()
{
    ArkPeer peer = {0};

    int peerCount = 0;

    if (global_networkType == MAIN)
        peerCount = sizeof(SeedArray) / sizeof(SeedArray[0]);
    if (global_networkType == DEVELOPMENT)
        peerCount = sizeof(SeedArrayTest) / sizeof(SeedArrayTest[0]);

    time_t t;
    srand((unsigned) time(&t));
    int index = rand() % peerCount;

    char* element;
    if (global_networkType == MAIN)
        element = SeedArray[index];
    if (global_networkType == DEVELOPMENT)
        element = SeedArrayTest[index];

    int x = strchr(element, ':') - element;
    int y = strlen(element) - x - 1;

    char* ip = malloc(sizeof(char));
    strncpy(ip, element, x);
    ip[x] = '\0';

    char* port = malloc(sizeof(char));
    strncpy(port, element + x + 1, y);
    port[y] = '\0';

    peer.ip = ip;
    peer.port = atoi(port);

    element = NULL;
    ip = NULL;
    port = NULL;

    return peer;
}

ArkPeerArray ark_api_get_peers(char* ip, int port)
{
    printf("Getting peers: [IP = %s, Port: = %d]\n", ip, port);

    char url[255];
    snprintf(url, sizeof url, "%s:%d/api/peers", ip, port);

    ArkPeerArray apa = {0};
    RestResponse *ars = ark_api_get(url);

    if (ars->size == 0 || ars->data == NULL)
        return apa;

    cJSON *root = cJSON_Parse(ars->data);

    if ((cJSON_GetObjectItem(root, "success")->valueint) != 1)
        return apa;

    cJSON *peers = cJSON_GetObjectItem(root, "peers");
    int total = cJSON_GetArraySize(peers);

    ArkPeer *data = malloc(total * sizeof(ArkPeer));
    if (!data)
        return apa;

    for (int i = 0; i < total; i++)
    {
        cJSON *peerJson = cJSON_GetArrayItem(peers, i);

        data[i] = ark_helpers_get_ArkPeer(peerJson);
    }

    apa.length = total;
    apa.data = data;

    free(peers);
    free(root);
    ars = NULL;

    return apa;
}

ArkFee ark_api_get_fee(char* ip, int port)
{
    printf("Getting fees for a peer: [IP = %s, Port = %d]\n", ip, port);

    char url[255];
    snprintf(url, sizeof url, "%s:%d/api/blocks/getfees", ip, port);

    ArkFee fee = {0};
    RestResponse *ars = ark_api_get(url);

    if (ars->size == 0 || ars->data == NULL)
        return fee;

    cJSON *root = cJSON_Parse(ars->data);

    if ((cJSON_GetObjectItem(root, "success")->valueint) != 1)
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

ArkPeer ark_api_peers_get(ArkPeer peer, int port, char *ip)
{
    char url[255];
    snprintf(url, sizeof url, "%s:%d/api/peers/get?port=%d&ip=%s", peer.ip, peer.port, port, ip);

    ArkPeer arkpeer = {0};
    RestResponse *ars = ark_api_get(url);

    if (ars->size == 0 || ars->data == NULL)
        return arkpeer;

    cJSON *root = cJSON_Parse(ars->data);

    if ((cJSON_GetObjectItem(root, "success")->valueint) != 1)
        return arkpeer;

    cJSON *peerJson = cJSON_GetObjectItem(root, "peer");

    arkpeer = ark_helpers_get_ArkPeer(peerJson);

    free(peerJson);
    free(root);
    ars = NULL;

    return arkpeer;
}

char* ark_api_blocks_getEpoch(char* ip, int port)
{
    char url[255];
    snprintf(url, sizeof url, "%s:%d/api/blocks/getEpoch", ip, port);

    RestResponse *ars = ark_api_get(url);

    if (ars->data == NULL)
        return NULL;

    cJSON *root = cJSON_Parse(ars->data);

    if ((cJSON_GetObjectItem(root, "success")->valueint) != 1)
        return NULL;

    char* time = "";
    time = cJSON_GetObjectItem(root, "epoch")->valuestring;

    free(root);
    ars = NULL;

    return time;
}

ArkBlockHeight ark_api_blocks_getHeight(char* ip, int port)
{
    ArkBlockHeight arkblockheight;
    char url[255];

    snprintf(url, sizeof url, "%s:%d/api/blocks/getHeight", ip, port);

    RestResponse *ars = ark_api_get(url);

    if (ars->data == NULL)
        return arkblockheight;

    cJSON *root = cJSON_Parse(ars->data);

    if ((cJSON_GetObjectItem(root, "success")->valueint) != 1)
        return arkblockheight;

    arkblockheight.id = cJSON_GetObjectItem(root, "id")->valuestring;
    arkblockheight.height = cJSON_GetObjectItem(root, "height")->valueint;

    free(root);
    ars = NULL;

    return arkblockheight;
}

int ark_api_blocks_getFee(char *ip, int port)
{
    int fee = -1;
    char url[255];

    snprintf(url, sizeof url, "%s:%d/api/blocks/getFee", ip, port);

    RestResponse *ars = ark_api_get(url);

    if (ars->data == NULL)
        return fee;

    cJSON *root = cJSON_Parse(ars->data);

    if ((cJSON_GetObjectItem(root, "success")->valueint) != 1)
        return fee;

    fee = cJSON_GetObjectItem(root, "fee")->valueint;

    free(root);
    ars = NULL;

    return fee;
}

char *ark_api_blocks_getNethash(char* ip, int port)
{
    char* nethash = "";
    char url[255];

    snprintf(url, sizeof url, "%s:%d/api/blocks/getNethash", ip, port);

    RestResponse *ars = ark_api_get(url);

    if (ars->data == NULL)
        return nethash;

    cJSON *root = cJSON_Parse(ars->data);

    if ((cJSON_GetObjectItem(root, "success")->valueint) != 1)
        return nethash;

    nethash = cJSON_GetObjectItem(root, "nethash")->valuestring;

    free(root);
    ars = NULL;

    return nethash;
}

ArkDelegate* ark_api_get_delegates(char* ip, int port)
{
    printf("Getting delegates: [IP = %s, Port: = %d]\n", ip, port);

    char url[255];
    snprintf(url, sizeof url, "%s:%d/api/delegates", ip, port);

    RestResponse *ars = ark_api_get(url);

    if (ars->data == NULL)
        return NULL;

    cJSON *root = cJSON_Parse(ars->data);

    if ((cJSON_GetObjectItem(root, "success")->valueint) != 1)
        return NULL;

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

ArkDelegate ark_api_get_delegate_by_username(char* ip, int port, char* username)
{
    printf("Getting delegate by username: [IP = %s, Port: = %d, Username = %s]\n", ip, port, username);

    char url[255];
    snprintf(url, sizeof url, "%s:%d/api/delegates/get?username=%s", ip, port, username);

    ArkDelegate delegate;
    RestResponse *ars = ark_api_get(url);

    if (ars->data == NULL)
        return delegate;

    cJSON *root = cJSON_Parse(ars->data);

    if ((cJSON_GetObjectItem(root, "success")->valueint) != 1)
        return delegate;

    cJSON *delegateJson = cJSON_GetObjectItem(root, "delegate");

    delegate = ark_helpers_get_ArkDelegate(delegateJson);

    free(delegateJson);
    free(root);
    ars = NULL;

    return delegate;
}

int ark_blocks_getMilestone(char *ip, int port)
{
    int milestone = -1;
    char url[255];

    snprintf(url, sizeof url, "%s:%d/api/blocks/getMilestone", ip, port);

    RestResponse *ars = ark_api_get(url);

    if (ars->data == NULL)
        return milestone;

    cJSON *root = cJSON_Parse(ars->data);

    if ((cJSON_GetObjectItem(root, "success")->valueint) != 1)
        return milestone;

    milestone = cJSON_GetObjectItem(root, "milestone")->valueint;

    free(root);
    ars = NULL;

    return milestone;
}

ArkVoter* ark_api_get_delegate_voters(char* ip, int port, char* publicKey)
{
    printf("Getting delegate voters: [IP = %s, Port: = %d, PublicKey = %s]\n", ip, port, publicKey);

    char url[255];
    snprintf(url, sizeof url, "%s:%d/api/delegates/voters?publicKey=%s", ip, port, publicKey);

    RestResponse *ars = ark_api_get(url);

    if (ars->data == NULL)
        return NULL;

    cJSON *root = cJSON_Parse(ars->data);

    if ((cJSON_GetObjectItem(root, "success")->valueint) != 1)
        return NULL;

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
