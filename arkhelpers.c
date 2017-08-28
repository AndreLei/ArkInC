#include "arkhelpers.h"

/// --------------------
/// ARK HELPERS
/// --------------------

ARKPEERSTATUS ark_helpers_getArkPeerStatus_fromString(const char* string)
{
    static struct {
        const char *s;
        ARKPEERSTATUS e;
    } map[] = {
    { "OK", OK },
    { "EUNAVAILABLE", EUNAVAILABLE },
    { "ETIMEOUT", ETIMEOUT }
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

ARKTRANSACTIONTYPE ark_helpers_getArkTransactionType_fromString(const char* string)
{
    static struct {
        const char *s;
        ARKTRANSACTIONTYPE e;
    } map[] = {
    { "SENDARK", SENDARK },
    { "SECONDSIGNATURE", SECONDSIGNATURE },
    { "CREATEDELEGATE", CREATEDELEGATE },
    { "VOTE", VOTE },
    { "MULTISIGNATURE", MULTISIGNATURE }
};

    ARKTRANSACTIONTYPE result = OK;

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

ArkPeer ark_helpers_getArkPeer_fromJSON(const cJSON * const json)
{
    ArkPeer peer = {0};
    cJSON* obj = NULL;

    obj = cJSON_GetObjectItem(json, "ip");
    if (obj != NULL)
        peer.ip = obj->valuestring;

    obj = cJSON_GetObjectItem(json, "port");
    if (obj != NULL)
        peer.port = obj->valueint;

    obj = cJSON_GetObjectItem(json, "version");
    if (obj != NULL)
        peer.version = obj->valuestring;

    obj = cJSON_GetObjectItem(json, "os");
    if (obj != NULL)
        peer.os = obj->valuestring;

    obj = cJSON_GetObjectItem(json, "status");
    if (obj != NULL)
        peer.status = ark_helpers_getArkPeerStatus_fromString(cJSON_GetObjectItem(json, "status")->valuestring);

    obj = cJSON_GetObjectItem(json, "delay");
    if (obj != NULL)
        peer.delay = obj->valueint;

    obj = cJSON_GetObjectItem(json, "height");
    if (obj != NULL)
        peer.height = obj->valueint;

    free(obj);

    return peer;
}

ArkDelegate ark_helpers_getArkDelegate_fromJSON(const cJSON * const json)
{
    ArkDelegate delegate = {0};
    cJSON* obj = NULL;

    obj = cJSON_GetObjectItem(json, "username");
    if (obj != NULL)
        delegate.username = obj->valuestring;

    obj = cJSON_GetObjectItem(json, "address");
    if (obj != NULL)
        delegate.address = obj->valuestring;

    obj = cJSON_GetObjectItem(json, "publicKey");
    if (obj != NULL)
        delegate.publicKey = obj->valuestring;

    obj = cJSON_GetObjectItem(json, "vote");
    if (obj != NULL)
        delegate.vote = obj->valuestring;

    obj = cJSON_GetObjectItem(json, "producedblocks");
    if (obj != NULL)
        delegate.producedBlocks = (long)(obj->valuedouble + 0.5);

    obj = cJSON_GetObjectItem(json, "missedblocks");
    if (obj != NULL)
        delegate.missedBlocks = (long)(obj->valuedouble + 0.5);

    obj = cJSON_GetObjectItem(json, "rate");
    if (obj != NULL)
        delegate.rate = obj->valueint;

    obj = cJSON_GetObjectItem(json, "approval");
    if (obj != NULL)
        delegate.approval = obj->valuedouble;

    obj = cJSON_GetObjectItem(json, "productivity");
    if (obj != NULL)
        delegate.productivity = obj->valuedouble;

    free(obj);

    return delegate;
}

ArkVoter ark_helpers_getArkVoter_fromJSON(const cJSON * const json)
{
    ArkVoter voter = {0};
    cJSON* obj = NULL;

    obj = cJSON_GetObjectItem(json, "username");
    if (obj != NULL)
        voter.username = obj->valuestring;

    obj = cJSON_GetObjectItem(json, "address");
    if (obj != NULL)
        voter.address = obj->valuestring;

    obj = cJSON_GetObjectItem(json, "publicKey");
    if (obj != NULL)
        voter.publicKey = obj->valuestring;

    obj = cJSON_GetObjectItem(json, "balance");
    if (obj != NULL)
        voter.balance = (long)(obj->valuedouble + 0.5);

    free(obj);

    return voter;
}

ArkTransaction ark_helpers_getArkTransaction_fromJSON(const cJSON * const json)
{
    ArkTransaction transaction = {0};
    cJSON* obj = NULL;

    obj = cJSON_GetObjectItem(json, "amount");
    if (obj != NULL)
        transaction.amount = obj->valuedouble;

    obj = cJSON_GetObjectItem(json, "blockid");
    if (obj != NULL)
        transaction.blockId = obj->valuestring;

    obj = cJSON_GetObjectItem(json, "confirmation");
    if (obj != NULL)
        transaction.confirmation = obj->valuestring;

    obj = cJSON_GetObjectItem(json, "fee");
    if (obj != NULL)
        transaction.fee = obj->valuedouble;

    obj = cJSON_GetObjectItem(json, "height");
    if (obj != NULL)
        transaction.height = obj->valueint;

    obj = cJSON_GetObjectItem(json, "id");
    if (obj != NULL)
        transaction.id = obj->valuestring;

    obj = cJSON_GetObjectItem(json, "recipientid");
    if (obj != NULL)
        transaction.recipientId = obj->valuestring;

    obj = cJSON_GetObjectItem(json, "secondsenderpublickey");
    if (obj != NULL)
        transaction.secondSenderPublicKey = obj->valuestring;

    obj = cJSON_GetObjectItem(json, "senderid");
    if (obj != NULL)
        transaction.senderId = obj->valuestring;

    obj = cJSON_GetObjectItem(json, "senderpublickey");
    if (obj != NULL)
        transaction.senderPublicKey = obj->valuestring;

    obj = cJSON_GetObjectItem(json, "signature");
    if (obj != NULL)
        transaction.signature = obj->valuestring;

    obj = cJSON_GetObjectItem(json, "signsignature");
    if (obj != NULL)
        transaction.signSignature = obj->valuestring;

    obj = cJSON_GetObjectItem(json, "status");
    if (obj != NULL)
        transaction.type = ark_helpers_getArkTransactionType_fromString(cJSON_GetObjectItem(json, "type")->valuestring);

    /// TIMESTAMP

    obj = cJSON_GetObjectItem(json, "vendorfield");
    if (obj != NULL)
        transaction.vendorField = obj->valuestring;

    free(obj);

    return transaction;
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

int ark_helpers_isResponseSuccess(const cJSON * const json)
{
    cJSON *obj = cJSON_GetObjectItem(json, "success");
    if (obj == NULL)
        return 0;

    if (obj->valueint == 1)
        return 1;

    return 0;
}
