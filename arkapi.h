#include <stdio.h>

#include "jsonresponse.h"
#include "arkmodels.h"
#include "cJSON.h"

ARKPEERSTATUS getArkPeerStatus(const char* string);

ArkPeer getArkPeer(const cJSON * const json);

ArkDelegate getArkDelegate(const cJSON *const json);

ArkPeer* ark_api_get_peers(char *serverIp, int serverPort);

ArkFee ark_api_get_fee(char *peerIp, int peerPort);

ArkPeer ark_api_peers_get(ArkPeer peer, int port, char *ip);

time_t ark_api_blocks_getEpoch(ArkPeer peer);

ArkDelegate* ark_api_get_delegates(char* serverIp, int serverPort);

ArkDelegate ark_api_get_delegate_by_username(char* serverIp, int serverPort, char* username);
