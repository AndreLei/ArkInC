#include <stdio.h>
#include <time.h>

#include "jsonresponse.h"
#include "arkmodels.h"
#include "cJSON.h"

ARKPEERSTATUS getArkPeerStatus(const char* string);

ArkPeer ark_helpers_get_ArkPeer(const cJSON * const json);

ArkDelegate ark_helpers_get_ArkDelegate(const cJSON *const json);

ArkVoter ark_helpers_get_ArkVoter(const cJSON *const json);

ArkPeer* ark_api_get_peers(char *serverIp, int serverPort);

ArkFee ark_api_get_fee(char *peerIp, int peerPort);

ArkPeer ark_api_peers_get(ArkPeer peer, int port, char *ip);

char* ark_api_blocks_getEpoch(ArkPeer peer);

ArkBlockHeight ark_api_blocks_getHeight(ArkPeer peer);

int ark_api_blocks_getFee(ArkPeer peer);

char *ark_api_blocks_getNethash(ArkPeer peer);


ArkDelegate* ark_api_get_delegates(char* serverIp, int serverPort);

ArkDelegate ark_api_get_delegate_by_username(char* serverIp, int serverPort, char* username);

ArkVoter* ark_api_get_delegate_voters(char* serverIp, int serverPort, char* publicKey);
