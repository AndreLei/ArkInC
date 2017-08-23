#include <stdio.h>

#include "jsonresponse.h"
#include "arkmodels.h"
#include "cJSON.h"

ARKPEERSTATUS getArkPeerStatus(const char* string);

ArkPeer* ark_api_get_peers(char *serverIp, int serverPort);

ArkFee ark_api_get_fee(ArkPeer peer);
